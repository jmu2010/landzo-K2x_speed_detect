/***********************************************************
 * �ļ���       ��KL2x_flash.c
 * ˵��         ��flashд�����
 * ����         ��landzo ������
 * �Ա���       ��http://landzo.taobao.com/
 * ��˾��վ     ��http://www.landzo.com/
 *��̳          ��http://www.landzo.cn
 *��汾        ��V2.0
 *ʱ��          ��15.5.26
************************************************************/

//����ͷ�ļ�
#include "KL2x_flash.h" 



// Flash�������궨�� ���ڲ�ʹ�� 
#define CCIF    (1<<7)
#define ACCERR  (1<<5)
#define FPVIOL  (1<<4)
#define MGSTAT0 (1<<0)


//Flash����궨�壬�ڲ�ʹ��
//#define RD1BLK    0x00   // ������Flash
#define RD1SEC    0x01   // ����������
#define PGMCHK    0x02   // д����
#define RDRSRC    0x03   // ��Ŀ������
#define PGM4      0x06   // д�볤��
#define ERSBLK    0x08   // ��������Flash
#define ERSSCR    0x09   // ����Flash����
//#define PGMSEC    0x0B   // д������
#define RD1ALL    0x40   // �����еĿ�
#define RDONCE    0x41   // ֻ��һ��
#define PGMONCE   0x43   // ֻдһ��
#define ERSALL    0x44   // �������п�
#define VFYKEY    0x45   // ��֤���ŷ���Կ��
//#define PGMPART   0x80   // д�����
//#define SETRAM    0x81   // �趨FlexRAM����


/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�flash_init
*  ����˵������ʼ��flashģ��
*  ����˵����
*  �������أ���
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
__RAMFUN void hw_flash_init(void)
{
/*	//���FMC������
    flash_sign_off();
    
    // ��ֹ���Ź�
    WDOG_UNLOCK = 0xC520;
    WDOG_UNLOCK = 0xD928;
    WDOG_STCTRLH = 0;    // ��ֹ���Ź�
*/    
    // �ȴ��������
    while(!(FTFA_FSTAT & CCIF));
    
    // ������ʳ����־λ
    FTFA_FSTAT = ACCERR | FPVIOL;
    BSET(MCM_PLACR_ESFC_SHIFT,MCM_PLACR);
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�flash_erase_sector
*  ����˵��������ָ��flash����
*  ����˵����sectorNo��������
*  �������أ�����ִ��״̬��0=��������0=�쳣��
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
__RAMFUN uint8 flash_erase_sector(uint16 sectorNo)
{

    union
    {
        uint32  word;
        uint8   byte[4];
    } dest;
    
    dest.word    = (uint32)(sectorNo*(1<<10));
//
    // ���ò�������
    FTFA_FCCOB0 = ERSSCR; // ������������
    
//     ����Ŀ���ַ
    FTFA_FCCOB1 = dest.byte[2];
    FTFA_FCCOB2 = dest.byte[1];
    FTFA_FCCOB3 = dest.byte[0];

 
//    // ִ����������
//    if(1 == hw_flash_cmd_launch())    //��ִ��������ִ���
//        return 1;     //�����������
    
    FTFA_FSTAT = ACCERR | FPVIOL;    // ������ʴ����־λ�ͷǷ����ʱ�־λ
    FTFA_FSTAT = CCIF;               // ��������
    while(!(FTFA_FSTAT & CCIF));    // �ȴ��������
    if(FTFA_FSTAT & (ACCERR | FPVIOL | MGSTAT0))    // �������־
        return 1 ; //ִ���������

    
    // ������sector0ʱ��������豸
    if(dest.word <= 0x800)
    {
        // д��4�ֽ�
        FTFA_FCCOB0 = PGM4; 
        // ����Ŀ���ַ
        FTFA_FCCOB1 = 0x00;
        FTFA_FCCOB2 = 0x04;
        FTFA_FCCOB3 = 0x0C;
        // ����
        FTFA_FCCOB4 = 0xFF;
        FTFA_FCCOB5 = 0xFF;
        FTFA_FCCOB6 = 0xFF;
        FTFA_FCCOB7 = 0xFE;
        // ִ����������
//        if(1 == hw_flash_cmd_launch())  //��ִ��������ִ���
//            return 2;   //�����������
        FTFA_FSTAT = ACCERR | FPVIOL;    // ������ʴ����־λ�ͷǷ����ʱ�־λ
        FTFA_FSTAT = CCIF;               // ��������
        while(!(FTFA_FSTAT & CCIF));    // �ȴ��������
        if(FTFA_FSTAT & (ACCERR | FPVIOL | MGSTAT0))    // �������־
            return 2 ; //ִ���������        
    }  
    return 0;  //�ɹ�����
}
/*************************************************************************
*                             ������ӿƼ����޹�˾
*
*  �������ƣ�flash_write
*  ����˵����flashд�����
*  ����˵����sectNo��Ŀ�������� ��KL25ʵ��ʹ��0~127��
*         offset:д�������ڲ�ƫ�Ƶ�ַ��0~1020��
*         cnt��д���ֽ���Ŀ��4~1024��
*         buf��Դ���ݻ������׵�ַ
*  �������أ�����ִ��״̬��0=��������0=�쳣��
*  �޸�ʱ�䣺2015-5-15   �Ѳ���
*  ��    ע�����ݴ�ѧ������
*************************************************************************/
__RAMFUN uint8 flash_write(uint16 sectNo,uint16 offset,uint16 cnt,uint8 buf[])
{
    uint32 size;
    uint32 destaddr;
    
    union
    {
        uint32   word;
        uint8_t  byte[4];
    } dest;
    
    if(offset%4 != 0)
        return 1;   //�����趨����ƫ����δ���루4�ֽڶ��룩
    
    // ����д������
    FTFA_FCCOB0 = PGM4;
    destaddr = (uint32)(sectNo*(1<<10) + offset);//�����ַ
    dest.word = destaddr;
    for(size=0; size<cnt; size+=4, dest.word+=4, buf+=4)
    {
        // ����Ŀ���ַ
        FTFA_FCCOB1 = dest.byte[2];
        FTFA_FCCOB2 = dest.byte[1];
        FTFA_FCCOB3 = dest.byte[0];
 
        // ��������
        FTFA_FCCOB4 = buf[3];
        FTFA_FCCOB5 = buf[2];
        FTFA_FCCOB6 = buf[1];
        FTFA_FCCOB7 = buf[0];
        

        FTFA_FSTAT = ACCERR | FPVIOL;    // ������ʴ����־λ�ͷǷ����ʱ�־λ
        FTFA_FSTAT = CCIF;               // ��������
        while(!(FTFA_FSTAT & CCIF));    // �ȴ��������
        if(FTFA_FSTAT & (ACCERR | FPVIOL | MGSTAT0))    // �������־
             return 2;  //д���������
    }
    
    return 0;  //�ɹ�ִ��
}




