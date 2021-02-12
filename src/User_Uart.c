/******************************************************************************
�ļ�����: Uart.c
�ļ���ʶ: STC8A8K64S4A12
ժ    Ҫ: UartӲ����������
��ǰ�汾: V1.0	
�������: 2021.02.12
*******************************************************************************/
#define	USER_UART_GLOBALS
#include "include.h"
#include "wifi.h"

char wptr,wptr4;
char rptr,rptr4;
char buffer[16],buffer4[16];
bit busy;
/*****************************************************************************
�������� : Uart1Init
�������� : uart1��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/

void Uart1Init()	// 9600bps@11.059200MHz
{
	SCON  = 0x50;		// 8λ����,�ɱ䲨����
	AUXR |= 0x40;		// ��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;		// ����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;		// �趨��ʱ��1Ϊ16λ�Զ���װ��ʽ
    TL1= BRT;
    TH1= BRT >> 8;
	ET1   = 0;		    // ��ֹ��ʱ��1�ж�
	TR1   = 1;		    // ������ʱ��1
	ES    = 1;          // �������ж�
	EA    = 1;          // �����ж�
}
/*****************************************************************************
�������� : Uart1_PutChar
�������� : ���ڷ���һ���ֽ�
������� : dat:����������
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart1_PutChar(char dat)
{
    while (busy);  // ��busy=0ʱ������ѭ������ʾ���Է���������
    busy = 1;      // ���ͱ�־��1��Ϊ�´η�����׼��
    SBUF = dat;    // ��ʼ�����ֽ�   (SXBUF = SBUF��S2BUF��S3BUF��S4BUF)
}
/*****************************************************************************
�������� : Uart1SendStr
�������� : ���ڷ���һ���ַ���
������� : p:�ַ���ָ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart1SendStr(char *p)
{
    while (*p)
    {
        Uart1_PutChar(*p++);
    }
}
/*****************************************************************************
�������� : UART1_Isr
�������� : Uart1�����жϴ�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void UART1_Isr() interrupt 4
{
    if (TI)                   // �����жϱ�־λ�ж�
    {
        TI = 0;               // ���жϱ�־
		busy = 0;             // ���ͱ�־λ��0����ʾ���㷢������
        LEDT1=~LEDT1;         // ���Զ˿�
    }
    if (RI)                   // �����жϱ�־λ�ж�
    {
        RI = 0;               // ���жϱ�־
        LEDR1=~LEDR1;         // ���Զ˿�
		//buffer[wptr++] = SBUF;
		//wptr &= 0x0f;
        uart_receive_input(SBUF);
    }
}
/*****************************************************************************
�������� : Uart4Init
�������� : uart4��ʼ��
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/

void Uart4Init()	
{
    P_SW2 |= 0x04;      //ѡ�񴮿�4�ĵڶ���P5.2R P5.3T
    P5M0 = 0x08;
    P5M1 = 0x00;
	S4CON  = 0x10;		// 8λ����,�ɱ䲨����
	S4CON &= 0xBF;		// ����4ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR  |= 0x04;		// ��ʱ��2ʱ��ΪFosc,��1T
	T2L    = BRT;		// 65536-12000000/9600/4=FEC7H
	T2H    = BRT >> 8;	// �趨��ʱ��ֵ
	AUXR  |= 0x10;		// ������ʱ��2
	IE2    = ES4;       // ʹ�ܴ����ж�
	EA     = 1;         // �����ж�
}
/*****************************************************************************
�������� : Uart4_PutChar
�������� : ���ڷ���һ���ֽ�
������� : dat:����������
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void Uart4_PutChar(char dat)
{
    while (busy);  // ��busy=0ʱ������ѭ������ʾ���Է���������
    busy = 1;      // ���ͱ�־��1��Ϊ�´η�����׼��
    S4BUF = dat;    // ��ʼ�����ֽ�   (SXBUF = SBUF��S2BUF��S3BUF��S4BUF)
}
/*****************************************************************************
�������� : UART4_Isr
�������� : Uart4�����жϴ�������
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void UART4_Isr() interrupt 18
{
    if (S4CON & S4TI)           // �����жϱ�־�ж�
    {
        S4CON &= ~S4TI;         // ���жϱ�־
        LEDT1=~LEDT1;           // ���Զ˿�
		busy = 0;
    }
    if (S4CON & S4RI)           // �����жϱ�־�ж�
    {
        S4CON &= ~S4RI;         // ���жϱ�־
        LEDR1=~LEDR1;           // ���Զ˿�
		//buffer4[wptr4++] = S4BUF; // ��������
		//wptr4 &= 0x0f;           // ȷ���������ݲ��������鳤��
        uart_receive_input(S4BUF);
    }
}