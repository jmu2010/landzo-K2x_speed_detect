#include "common.h"
#include "KL2x_dma.h"
#include "KL2x_gpio.h"









/*************************************************************************
*                             野火嵌入式开发工作室
*
*  函数名称：DMA_PORTx2BUFF_Init
*  功能说明：DMA初始化，读取端口数据到内存
*  参数说明：DMA_CHn              通道号（DMA_CH0 ~ DMA_CH4）
*            SADDR                源地址( (void * )&PTx_BYTEx_INT 或 (void * )&PTx_WORDx_INT   )
*            DADDR                目的地址
*            PTxn                 触发端口
*            DMA_BYTEn            每次DMA传输字节数
*            count                一个主循环传输字节数
*  函数返回：无
*  修改时间：2012-2-20
*  备    注：
*************************************************************************/



void DMA_PORTx2BUFF_Init(DMA_CHn CHn, void *SADDR, void *DADDR, PTxn ptxn, DMA_BYTEn byten, uint32_t count)
{
    uint8_t n, tmp;
    uint8_t BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 0 ) ) ); //计算传输字节数
    uint8_t ptx0;
    uint32_t temp32 ;
    //断言，检测传递进来参数是否正确
    ASSERT(                                             //用断言检测 源地址和每次传输字节数是否正确
        (   (byten == DMA_BYTE1)                    //传输一个字节
            && ( (SADDR >= &PTA_B0_IN) && (SADDR <= ( &PTE_B3_IN )))
        )

        || (   (byten == DMA_BYTE2)                   //传输两个字节(注意，不能跨端口)
               && ( (SADDR >= &PTA_B0_IN)
                    && (SADDR <= ( &PTE_W1_IN ))
                    && (((uint32)SADDR & 0x03) != 0x03) )         //保证不跨端口
           )

        || (   (byten == DMA_BYTE4)                   //传输四个字节
               && ((SADDR >= &PTA_B0_IN) && (SADDR <= ( &PTE_B0_IN )))
               && (((uint32)SADDR & 0x03) == 0x00)           //保证不跨端口
           )
    );

    ASSERT(count < 0x8000); //断言，最大只支持0x7FFF

    //DMA 寄存器 配置

    /* 开启时钟 */
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;                     //打开DMA多路复用器时钟
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                        //打开DMA模块时钟



                                       
    DMAMUX0_CHCFG(CHn)=0x00;
    DMA_DSR_BCR(CHn)|=DMA_DSR_BCR_DONE_MASK;

    /* 配置 DMA 通道 的 传输控制块 TCD ( Transfer Control Descriptor ) */
    DMA_SAR(CHn) =    (uint32)SADDR;                      // 设置  源地址
    DMA_DSR_BCR(CHn) =(0
                     | DMA_DSR_BCR_BCR(count)              //传输数目
                     );
    DMA_DAR(CHn) =    (uint32)DADDR;                      // 设置目的地址
    DMA_DCR(CHn) =    (0
                       | DMA_DCR_SSIZE(byten)
                       | DMA_DCR_DSIZE(byten)
                       //| DMA_DCR_SINC_MASK                  //传输后源地址增加(根据位宽)
                       | DMA_DCR_DINC_MASK                  //传输后目的地址增加(根据位宽)
                       | DMA_DCR_CS_MASK                  // 0为不停得传输，直到BCR为0；1为一次请求传输一次
                       //| DMA_DCR_START_MASK               //软件触发传输
                       | DMA_DCR_ERQ_MASK                   //硬件触发传输（与上面START二选一）
                       | DMA_DCR_D_REQ_MASK                 //传输完成后硬件自动清ERQ
                       | DMA_DCR_EADREQ_MASK
                       );
    /* 配置 DMA 触发源 */

    DMAMUX_CHCFG_REG(DMAMUX0_BASE_PTR, CHn) = (0
           // | DMAMUX_CHCFG_ENBL_MASK                        /* Enable routing of DMA request */
            //| DMAMUX_CHCFG_TRIG_MASK                        /* Trigger Mode: Periodic   PIT周期触发传输模式   通道1对应PIT1，必须使能PIT1，且配置相应的PIT定时触发 */
            |DMAMUX_CHCFG_SOURCE(DMA_PORTA)
              //| DMAMUX_CHCFG_SOURCE( PTX(ptxn) + DMA_PORTA) /* 通道触发传输源:     */
                                             );

    //配置触发源（默认是 上升沿触发）
    temp32 =  ALT1 | DMA_FALLING;                                          
    gpio_init(ptxn, GPI, 0);               //设置为输入
    Port_init(ptxn, temp32);


    /*  配置输入源   */
    //SADDR 实际上就是 GPIO的 输入寄存器 PDIR 的地址
    //GPIOA、GPIOB、GPIOC、GPIOD、GPIOE 的地址 分别是 0x400FF000u 、0x400FF040u 、0x400FF080u、 0x400FF0C0u、0x400FF100u
    //sizeof(GPIO_MemMap) = 0x18
    //每个GPIO地址 &0x1C0 后，得到 0x000 , 0x040 , 0x080 ,0x0C0 ,0x100
    //再 /0x40 后得到 0 、 1 、 2、 3、4 ，刚好就是 PTA、PTB、PTC 、PTD 、PTD 、PTE
    ptx0 = ((((uint32)SADDR) & 0x1C0) / 0x40 ) * 32;

    //每个GPIO 对应的寄存器地址， &0x 3F 后得到的值都是相同的。
    //PTA_B0_IN 即 GPIOA 的 输入寄存器 PDIR 的 地址
    // (SADDR & 0x3f - PTA_B0_IN & 0x3f) 等效于 (SADDR - PTA_B0_IN) & 0x3f
    //假设需要采集的位 为 0~7、8~15、16~23、24~31 ，则 上面式子对应的值 为 0、1、2、3
    //刚好是  0~7、8~15、16~23、24~31 位的地址偏移，再 * 8 就变成 0、8、16、24

    n = (uint8)(((uint32)SADDR - ((uint32)(&PTA_BYTE0_INT))) & 0x3f) * 8;       //最小的引脚号

    ptx0 += n;
    tmp = ptx0 + (BYTEs * 8 ) - 1;                                          //最大的引脚号
    while(ptx0 <= tmp)
    {
        //这里加入 GPIO 初始化为输入
        gpio_init((PTxn )ptx0, GPI, 0);               //设置为输入
        Port_init((PTxn )ptx0 , ALT1 | PULLDOWN );    //输入源应该下拉，默认读取到的是0

        ptx0 ++;
    }

    DMA_DIS(CHn);                                    //禁止通道CHn 硬件请求
    //DMA_IRQ_CLEAN(CHn);
    /* 开启中断 */
    //DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    //DMA_IRQ_EN(CHn);                                //允许DMA通道传输
}
