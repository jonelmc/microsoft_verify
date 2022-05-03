/**********************************************************************
        7�����ļ�����������
**********************************************************************/
#pragma db oe
#include <reg52.h>
#include <absacc.h>

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long

#define KEY_A_PORT  XBYTE[0xB800]

uchar inkey(void);
uchar keyin(void);
uchar keydata(void);
void delay(void);

/*�����ӳ��򣬵�����*/
uchar inkey(void)
	{
	uchar i;
	do
        	{
        	delay();
        	i=keyin();
        	delay();
        	}
      	while (i == 0xff);
      	return (i);
	}
/*�����ӳ��򣬲�������*/
uchar keyin(void)
      {
      uchar j;
      j == 0xff;
      KEY_A_PORT = 0x00;
      j = KEY_A_PORT;
      if (j != 0xff)
         {
         delay();
         KEY_A_PORT = 0x00;
         j = KEY_A_PORT;
         if (j != 0xff)
            j = keydata();
         }
      return (j);
      }
/*�趨��ֵ�ӳ���*/      
uchar keydata(void)
      {
      uchar j,k;
      k = 0xff;
      KEY_A_PORT = 0;
      j = KEY_A_PORT;
      if (j == 0xdf)
         k = 0x03;	/*��		*/
      if (j == 0xef)
         k = 0x01;	/*��		*/
      if (j == 0xfd)
         k = 0x04;	/*��		*/
      if (j == 0x7f)
         k = 0x02;	/*��		*/
      if (j == 0xfe)
         k = 0x06;	/*ENT		*/
      if (j == 0xfb)
         k = 0x05;	/*Esc		*/
      if (j == 0xf7)			
      	 k = 0x07;	/*��		*/
      do
        {
        KEY_A_PORT = 0x0;
        j = KEY_A_PORT;
        }
      while (j != 0xff);
      return (k);
      }
void delay(void)
     {
     uint i=100;
     while (i--);
     }
