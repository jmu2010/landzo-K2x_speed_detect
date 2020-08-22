
#ifndef _K6X_SD_H
#define _K6X_SD_H
#include "common.h"
#include "k6x_spi.h"

/********************************************************************/

#ifndef FALSE
  #define FALSE 0u
#endif

typedef uint8 SDCARD_SPI_COMMAND[6];

typedef struct sdcard_struct
{
   uint32                 TIMEOUT;                        //响应超时时间，需要大于250ms
   uint32                 NUM_BLOCKS;                     //SD卡BLOCK数
   uint16                 BLOCK_LEN;                      //每个block的大小
   uint8                  SDHC;                           //是否高容量SD
   uint8                  VERSION2;                       //是否version2或者之后规范的卡
} SDCARD_STRUCT, *SDCARD_STRUCT_PTR;

#define SDCARD_SPI_RETRIES                (10000)

#define SDCARD_SPI_RESPONSE_IDLE_STATE    (1 << 0)
#define SDCARD_SPI_RESPONSE_ERASE_RESET   (1 << 1)
#define SDCARD_SPI_RESPONSE_ILLEGAL_CMD   (1 << 2)
#define SDCARD_SPI_RESPONSE_CRC_ERROR     (1 << 3)
#define SDCARD_SPI_RESPONSE_ERASE_SEQ     (1 << 4)
#define SDCARD_SPI_RESPONSE_ADDRESS_ERROR (1 << 5)
#define SDCARD_SPI_RESPONSE_PARAM_ERROR   (1 << 6)
#define SDCARD_SPI_RESPONSE_START_MASK    (1 << 7)
#define SDCARD_SPI_RESPONSE_ERROR_MASK    (0x7E)

#define SDCARD_SPI_OK                     (0x00)
#define SDCARD_SPI_START                  (0xFE)
#define SDCARD_SPI_BUSY                   (0xFF)
#define SDCARD_SPI_DATA_RESPONSE_MASK     (0x1F)
#define SDCARD_SPI_DATA_RESPONSE_OK       (0x05)
#define SDCARD_SPI_DATA_ERROR_MASK        (0x0F)
#define SDCARD_SPI_END_BIT                (0x01)
#define SDCARD_SPI_IF_COND_VHS_MASK       (0x0F)
#define SDCARD_SPI_OCR_VOLTAGE_3_3V       (0x30)
#define SDCARD_SPI_OCR_CCS                (0x40)
#define SDCARD_SPI_CSD_VERSION_MASK       (0xC0)

#define SDCARD_SPI_CMD0_GO_IDLE_STATE     ( 0 | (0x40))
#define SDCARD_SPI_CMD8_SEND_IF_COND      ( 8 | (0x40))
#define SDCARD_SPI_CMD9_SEND_CSD          ( 9 | (0x40))
#define SDCARD_SPI_CMD13_SEND_STATUS      (13 | (0x40))
#define SDCARD_SPI_CMD16_SET_BLOCK512     (16 | (0x40))
#define SDCARD_SPI_CMD17_READ_BLOCK       (17 | (0x40))
#define SDCARD_SPI_CMD17_READ_MULTIBLOCK  (18 | (0x40))
#define SDCARD_SPI_CMD24_WRITE_BLOCK      (24 | (0x40))

#define SDCARD_SPI_ACMD41_SEND_OP_COND    (41 | (0x40))
#define SDCARD_SPI_CMD55_APP_CMD          (55 | (0x40))
#define SDCARD_SPI_CMD58_READ_OCR         (58 | (0x40))
#define SDCARD_SPI_CMD59_CRC_ON           (59 | (0x40))

#define IO_SDCARD_BLOCK_SIZE_POWER   (9)
#define IO_SDCARD_BLOCK_SIZE         (1 << IO_SDCARD_BLOCK_SIZE_POWER)


uint8 SD_Init (SPIn spin,SDCARD_STRUCT_PTR  sdcard_ptr);
uint8 SD_Read_Block(SDCARD_STRUCT_PTR  sdcard_ptr, uint8 *buffer, uint32 index);
uint8 SD_Write_Block(SDCARD_STRUCT_PTR sdcard_ptr , uint8 *buffer, uint32 index);

#endif

