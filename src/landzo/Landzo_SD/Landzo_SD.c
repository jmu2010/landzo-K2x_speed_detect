#include "landzo_sd.h"
#include "string.h"

static const enum 
{
  CMD0 = 0,
  CMD8,
  CMD58,
  CMD55,
  ACMD41,
  CMD9,
  CMD16,
  CMD59,
  CMD13
} COMMANDS_INDEX;

static const SDCARD_SPI_COMMAND SDCARD_SPI_COMMANDS[] =
{
  {SDCARD_SPI_CMD0_GO_IDLE_STATE,  0x00, 0x00, 0x00, 0x00, 0x95},
  {SDCARD_SPI_CMD8_SEND_IF_COND,   0x00, 0x00, 0x01, 0xAA, 0x87},
  {SDCARD_SPI_CMD58_READ_OCR,      0x00, 0x00, 0x00, 0x00, 0xFD},
  {SDCARD_SPI_CMD55_APP_CMD,       0x00, 0x00, 0x00, 0x00, 0x65},
  {SDCARD_SPI_ACMD41_SEND_OP_COND, 0x40, 0x00, 0x00, 0x00, 0x77},
  {SDCARD_SPI_CMD9_SEND_CSD,       0x00, 0x00, 0x00, 0x00, 0xAF},
  {SDCARD_SPI_CMD16_SET_BLOCK512,  0x00, 0x00, 0x02, 0x00, 0x15},
  {SDCARD_SPI_CMD59_CRC_ON,        0x00, 0x00, 0x00, 0x01, 0x83},
  {SDCARD_SPI_CMD13_SEND_STATUS,   0x00, 0x00, 0x00, 0x00, 0x0D},
};

static const uint32 SDCARD_SPI_CRC7_POLY  = 0x89;                   //该值为10001001B，产生CRC7多项式为X^7+X^3+X^0
static const uint32 SDCARD_SPI_CRC16_POLY = 0x11021;                //该值为00010001000000100001B,产生CRC16多项式为X^16+X^12+X^5+X^0
//=========================================================================
//函数名称：spi_sdcard_crc
//函数参数：poly：    CRC多项式
//          degree：  多项式次数
//          init：    初始值
//          input[]： 输入的位串
//          len：     位串长度
//函数返回：无
//功能概要：crc校验算法
//=========================================================================
static uint32 spi_sdcard_crc (uint32 poly, uint32 degree, uint32 init, 
                              const uint8 input[], uint32 len)
{
  static const uint32 ZEROSONES[2] = {0x00000000, 0xFFFFFFFF};
  uint32  i, mask;
  mask = (1 << degree) - 1;
  degree--;
  for (; 0 != len; len--, input++)
  {
    for (i = 7; i != (uint32)-1; i--)
    {
      init = ((init << 1) ^ (poly & ZEROSONES[((init >> degree) ^ (input[0] >> i)) & 1])) & mask;
    }
  }
  return init;
}

//=========================================================================
//函数名称：spi_sdcard_communicate
//函数参数：sdcard_ptr：SD卡信息数据结构
//          command：   发送的命令数据
//          response[]：响应的数据
//          len：       数据长度
//          buff[]：    发送的数据（当和response不同时）
//函数返回：成功：TURE 失败：FALSE
//功能概要：SD卡收发数据通讯函数
//=========================================================================
static uint8 spi_sdcard_communicate (SDCARD_STRUCT_PTR sdcard_ptr, 
                                       const uint8 command[], uint8 response[], 
                                       uint32 len, uint8 buff[])
{
  uint32  i;
  uint8   tmp[2];
  uint8  result = TRUE;
	
  // 发送命令
  spi_wr_stream(SPI0,(uint8*)command,6,NULL);

  // 等待响应
  tmp[0] = SDCARD_SPI_BUSY;
  for (i = 0; i < sdcard_ptr->TIMEOUT; i++)
  {
    response[0] = spi_wr_byte(SPI0,tmp[0]);
    if(0==(response[0] & SDCARD_SPI_RESPONSE_START_MASK))
      break;
  }
  if(i >= sdcard_ptr->TIMEOUT)
  {
    return FALSE;
  }
  
  if(NULL != buff)
  {
    if (response == buff)//接收数据
    {
      // 等待起始标志
      tmp[0] = SDCARD_SPI_BUSY;
      for (i = 0; i < sdcard_ptr->TIMEOUT; i++)
      {
        tmp[0] =spi_wr_byte(SPI0,tmp[0]);
        if (SDCARD_SPI_START == tmp[0]) 
          break;
      }
      if (i >= sdcard_ptr->TIMEOUT)
      {
        return FALSE;
      }
        // 接收数据
        memset (response, SDCARD_SPI_BUSY, len);
        for(i=0;i<len;i++)
        {
          response[i]=spi_wr_byte(SPI0,response[i]);
        }
        // 接收CRC 
        tmp[0] = SDCARD_SPI_BUSY;
        tmp[1] = SDCARD_SPI_BUSY;
        for(i=0;i<2;i++)
        {
          tmp[i]=spi_wr_byte(SPI0,tmp[i]);
        }
        i = tmp[0];
        i = (i << 8) | tmp[1];
        //CRC校验
        if (command[0] == SDCARD_SPI_COMMANDS[CMD9][0])
        {
          if (((spi_sdcard_crc (SDCARD_SPI_CRC7_POLY, 7, 0, response, len - 1) << 1) | SDCARD_SPI_END_BIT) != response[len - 1])
          {
            return FALSE;
          }
        }
        else
        {
          if (spi_sdcard_crc (SDCARD_SPI_CRC16_POLY, 16, 0, response, len) != i)
          {
            return FALSE;
          }
        }
     
    }
    else//发送数据
    {
      // 发起始令牌 .
      tmp[0] = SDCARD_SPI_START;
      spi_wr_byte(SPI0,tmp[0]);
      // 发数据 .
      for(i=0;i<len;i++)
      {
        spi_wr_byte(SPI0,buff[i]); 
      }
      // 发CRC
      i = spi_sdcard_crc (SDCARD_SPI_CRC16_POLY, 16, 0, buff, len);
      tmp[0] = i >> 8;
      tmp[1] = i;
      for(i=0;i<2;i++)
      {
        tmp[i]=spi_wr_byte(SPI0,tmp[i]);
      }
      // 读响应 .
      tmp[0] = SDCARD_SPI_BUSY;
      for (i = 0; i < sdcard_ptr->TIMEOUT; i++)
      {
        tmp[0] = spi_wr_byte(SPI0,tmp[0]);
        if ((tmp[0] & SDCARD_SPI_DATA_RESPONSE_MASK) < 16)
        {
          break;
        }
      } 
      if (i >= sdcard_ptr->TIMEOUT)
      {
        return FALSE;
      }
      if (SDCARD_SPI_DATA_RESPONSE_OK != (tmp[0] & SDCARD_SPI_DATA_RESPONSE_MASK))
      {
        return FALSE;
      }
      //等忙 .
      tmp[0] = SDCARD_SPI_BUSY;
      for (i = 0; i < sdcard_ptr->TIMEOUT; i++)
      {
        tmp[0]=spi_wr_byte(SPI0,tmp[0]);
        if (SDCARD_SPI_OK != tmp[0])
        {
          break;
        }
      } 
      if (i >= sdcard_ptr->TIMEOUT)
      {
        return FALSE;
      }		
    }
  }
  else
  {
    // 读响应
    len--;
    if (0 != len)
    {
      memset (response + 1, SDCARD_SPI_BUSY, len);
      for(i=0;i<len;i++)
      {
        response[i+1]=spi_wr_byte(SPI0,SDCARD_SPI_BUSY);
      }
    }
  }
  // 空读一次 .
  tmp[0] = SDCARD_SPI_BUSY;
  spi_wr_byte(SPI0,tmp[0]);
	
  // deassert CS .
  //mcf52xx_spi_flush(SDCARD_SPI_CHANNEL);

  return result;
}
 
	
//=========================================================================
//函数名称：SD_Init
//函数参数：spin：SPI通道号。
//          sdcard_ptr：SD卡信息数据结构
//函数返回：成功：TURE 失败：FALSE
//功能概要：初始化SD卡获得当前SD卡相关信息。
//=========================================================================
uint8 SD_Init (SPIn spin,SDCARD_STRUCT_PTR  sdcard_ptr)
{
  uint8  response[16],temp;
  uint32 i,c_size, c_size_mult, read_bl_len;
  K60_SPI_INIT_STRUCT  spi_initStruct;
  
  if (NULL == sdcard_ptr) 
    return FALSE;

  //初始化SPI为低速模式，SD低速时，SCK需要小于400K
  spi_initStruct.TRANSFER_MODE = MASTER ;
  spi_initStruct.PBR = 1;                                     //SCK baud rate =(fsys/PBR)*[(1+DBR)/BR]=(200/4)*[(1+0)/128]=390k 
  spi_initStruct.BR  = 7;
 // spi_init( spin,&spi_initStruct);  ??

  sdcard_ptr->TIMEOUT = 375000 / 37;                          //设置TIMEOUT时间约为1S
  
  // 空发送80个时钟周期
  temp = SDCARD_SPI_BUSY;
  for(i=0;i<10;i++)
  {
    spi_wr_byte( spin,temp);
  }

  //发送COM0 重置SD卡
  for (i = 0; i < SDCARD_SPI_RETRIES; i++)
  {
    if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD0], response, 1, NULL))
    {
      return FALSE;
    }
    if (SDCARD_SPI_RESPONSE_IDLE_STATE == response[0])
    {
      break;
    }
  }
  if (i >= SDCARD_SPI_RETRIES)
  {
    return FALSE;
  }
  
  //发送CMD8 
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD8], response, 5, NULL))
  {
    return FALSE;
  }
  sdcard_ptr->VERSION2 = FALSE;
  if (0 == (response[0] & SDCARD_SPI_RESPONSE_ILLEGAL_CMD))
  {
    sdcard_ptr->VERSION2 = TRUE;
    if ((response[4] != SDCARD_SPI_COMMANDS[CMD8][4]) || ((response[3] & SDCARD_SPI_IF_COND_VHS_MASK) != SDCARD_SPI_COMMANDS[CMD8][3]))
    {
      return FALSE;
    }
  }
	
  //发送CMD58 读OCR寄存器
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD58], response, 5, NULL))
  {
    return FALSE;
  }
  if (0 == (response[0] & SDCARD_SPI_RESPONSE_ILLEGAL_CMD))
  {
    if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
    {
      return FALSE;
    }		
    if (SDCARD_SPI_OCR_VOLTAGE_3_3V != (response[2] & SDCARD_SPI_OCR_VOLTAGE_3_3V))
    {
      return FALSE;
    }
  }	
  
  // 发送 CMD55 + ACMD41初始化卡
  for (i = 0; i < SDCARD_SPI_RETRIES; i++)
  {
    if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD55], response, 1, NULL))
    {
      return FALSE;
    }
    if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
    {
      return FALSE;
    }		
    if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[ACMD41], response, 5, NULL))
    {
      return FALSE;
    }
    if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
    {
      return FALSE;
    }		
    if (0 == (response[0] & SDCARD_SPI_RESPONSE_IDLE_STATE)) 
    {
      break;
    }
  }
  if (i >= SDCARD_SPI_RETRIES)
  {
    return FALSE;
  }
  
  // version 2 或之后的卡
  sdcard_ptr->SDHC = FALSE;
  if (sdcard_ptr->VERSION2)
  {
    // 发送 CMD58 读CCS
    if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD58], response, 5, NULL))
    {
      return FALSE;
    }
    if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
    {
      return FALSE;
    }		
    if (response[1] & SDCARD_SPI_OCR_CCS)
    {
      sdcard_ptr->SDHC = TRUE;
    }
  }
  
  //发送CMD9 读CSD
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD9], response, 16, response))
  {
    return FALSE;
  }
  if (0 == (response[0] & SDCARD_SPI_CSD_VERSION_MASK))
  {
    read_bl_len = response[5] & 0x0F;
    c_size = response[6] & 0x03;
    c_size = (c_size << 8) | response[7];
    c_size = (c_size << 2) | (response[8] >> 6);
    c_size_mult = ((response[9] & 0x03) << 1) | (response[10] >> 7);
    sdcard_ptr->NUM_BLOCKS = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
    sdcard_ptr->BLOCK_LEN=(1<<read_bl_len);
  }
  else
  {
    c_size = response[7] & 0x3F;
    c_size = (c_size << 8) | response[8];
    c_size = (c_size << 8) | response[9];
    sdcard_ptr->NUM_BLOCKS = (c_size + 1) << 10;
    sdcard_ptr->BLOCK_LEN=512;
  }

  //发送CMD16 设置一块为512字节
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD16], response, 1, NULL))
  {
    return FALSE;
  }
  if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
  {
    return FALSE;
  }		
  
  //发送 CMD59 打开 CRC校验 
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD59], response, 1, NULL))
  {
    return FALSE;
  }
  if (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK)
  {
    return FALSE;
  }
  
  //设置SPI到高速模式，SD高速时，SCK需要小于25M
  spi_initStruct.TRANSFER_MODE=MASTER ;
  spi_initStruct.PBR = 1;                                     //SCK baud rate =(fsys/PBR)*[(1+DBR)/BR]=(200/4)*[(1+0)/2]=25M
  spi_initStruct.BR  = 0;
//  spi_init( spin,&spi_initStruct);   ???
  
  sdcard_ptr->TIMEOUT = 24000000/50;  
  
  // 空发送，待准备完成, 这里是否可以减少发送次数以提高SD卡初始化时间？有待验证
  response[0] = SDCARD_SPI_BUSY;
  for(i=0;i<3000;i++)
  {
    spi_wr_byte(SPI0,response[0]);
  }
  return TRUE;
}

//=========================================================================
//函数名称：SD_Read_Block
//函数参数：sdcard_ptr：SD卡信息数据结构
//          buffer：存放读出的数据，通常512字节
//          index： 指出需要读SD卡哪一块
//函数返回：成功：TURE 失败：FALSE
//功能概要：读出SD卡某一个块数据。
//=========================================================================
uint8 SD_Read_Block (SDCARD_STRUCT_PTR  sdcard_ptr, uint8 *buffer,uint32 index)
{
  uint8  command[6];

  if (NULL == sdcard_ptr) 
  {
    return FALSE;
  }

  // Create SD SPI read command 
  command[0] = SDCARD_SPI_CMD17_READ_BLOCK;
  if (! sdcard_ptr->SDHC)
  {
    index <<= IO_SDCARD_BLOCK_SIZE_POWER;
  }
  command[1] = (index >> 24) & 0xFF;
  command[2] = (index >> 16) & 0xFF;
  command[3] = (index >>  8) & 0xFF;
  command[4] = (index >>  0) & 0xFF;
  command[5] = (spi_sdcard_crc (SDCARD_SPI_CRC7_POLY, 7, 0, command, 5) << 1) | SDCARD_SPI_END_BIT;
  
  return spi_sdcard_communicate (sdcard_ptr, command, buffer, IO_SDCARD_BLOCK_SIZE, buffer);
}

//=========================================================================
//函数名称：SD_Write_Block
//函数参数：sdcard_ptr：SD卡信息数据结构
//          buffer：需要写入的数据，通常512字节
//          index： 指出需要写入到SD卡的哪一块中
//函数返回：成功：TURE 失败：FALSE
//功能概要：读出SD卡某一个块数据。
//=========================================================================

uint8 SD_Write_Block (SDCARD_STRUCT_PTR sdcard_ptr , uint8 *buffer, uint32 index)
{
  uint8 command[6];
  uint8 response[2];
  
  if (NULL == sdcard_ptr) 
  {
    return FALSE;
  }
	
  // Create SD SPI write command 
  command[0] = SDCARD_SPI_CMD24_WRITE_BLOCK; 
  if (! sdcard_ptr->SDHC)
  {
    index <<= IO_SDCARD_BLOCK_SIZE_POWER;
  }
  command[1] = (index >> 24) & 0xFF;
  command[2] = (index >> 16) & 0xFF;
  command[3] = (index >>  8) & 0xFF;
  command[4] = (index >>  0) & 0xFF;
  command[5] = (spi_sdcard_crc (SDCARD_SPI_CRC7_POLY, 7, 0, command, 5) << 1) | SDCARD_SPI_END_BIT;
	
  // Write and check result 
  if (! spi_sdcard_communicate (sdcard_ptr, command, response, IO_SDCARD_BLOCK_SIZE, buffer))
  {
    return FALSE;
  }
  if (! spi_sdcard_communicate (sdcard_ptr, SDCARD_SPI_COMMANDS[CMD13], response, 2, NULL))
  {
    return FALSE;
  }
  if ((0 != response[1]) || (response[0] & SDCARD_SPI_RESPONSE_ERROR_MASK))
  {
    return FALSE;
  }
  
  return TRUE;
}








#if 0
//相当的好用！
uint8 WriteFile(uint8* str,uint16 lenth)
{ 
  uint16 i=0,startaddr=0x209;                                          //第一个文件的地址开始于0X208
  uint32 record_bytes=0,from_blkhead_2_here=0; 
  
  lenth<<=1;  
  if(Rd_Block(0x1FF,RXBUF)) return 1;                               //偏移字节存放在0X1FF块地址里面
  record_bytes=(RXBUF[510]) | (RXBUF[509]<<8) | (RXBUF[508]<<16);   //文件字写之前已经有的节数    
  startaddr+=(record_bytes/512);
  if(Rd_Block(startaddr,RXBUF)) return 1;
  //if((RXBUF[0]==0xff)||(RXBUF[1]==0xff)) record_bytes+=2;
  from_blkhead_2_here=record_bytes%512;
  if((from_blkhead_2_here+lenth)<=512)
  {
    for(i=0;i<lenth;i+=2)
      *(RXBUF + i + from_blkhead_2_here) = *(str + (i>>1));
    if(Wt_Block(startaddr,RXBUF)) return 1;
    View_Buf8(RXBUF);
  } 
  else
  {
    for(i=from_blkhead_2_here;i<512;i+=2)
      *(RXBUF+from_blkhead_2_here) = *(str+ (i>>1) -from_blkhead_2_here);
    if(Wt_Block(startaddr,RXBUF)) return 1;
    printp("\naddress is:0x%04x",startaddr);
    View_Buf8(RXBUF);
    //CLS_Buf8(RXBUF);
    if(Rd_Block(startaddr+1,RXBUF)) return 1;
    //if((RXBUF[0]==0xff)||(RXBUF[1]==0xff)) record_bytes+=2;
    for(i=0;i<(from_blkhead_2_here+lenth - 512);i +=2)
      *(RXBUF+i) = *(str + (i>>1) + 512 - from_blkhead_2_here);
    if(Wt_Block(startaddr+1,RXBUF)) return 1;
    printp("\naddress is:0x%04x",startaddr+1);
    View_Buf8(RXBUF);    
  }  
  if(Rd_Block(0x1FF,RXBUF)) return 1;
  RXBUF[510]=(uint8)( record_bytes+lenth);
  RXBUF[509]=(uint8)((record_bytes+lenth)>>8);
  RXBUF[508]=(uint8)((record_bytes+lenth)>>16);    
  if(Wt_Block(0x1FF,RXBUF)) return 1; 
  //View_Buf8(RXBUF);
  printf("\nfile bytes is:0x%02x%02x%02x",RXBUF[508],RXBUF[509],RXBUF[510]);
  return 0; 
}
#endif
