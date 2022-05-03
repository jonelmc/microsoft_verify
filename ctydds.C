/**********************************************************************
    AD9850 DDS发生器
**********************************************************************/
#pragma db oe
#include <reg52.h>
#include <absacc.h>
#include <string.h>
#include <stdio.h>
#include <intrins.h>

#include <MATH.H>
#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long

#define AD9850  XBYTE[0xC000]

extern void yj_init(void);				/*液晶初始化*/
extern void yj_clrtext(void);				/*清除文本屏*/
extern void yj_clrgraph(void);				/*清除图形屏*/
extern void dishz(uchar x,uchar y,uchar *a);		/*显示一个汉字或一个字符*/
extern void dishzf(uchar x,uchar y,uchar *a);

void initsiu(void);
void hm0(void);
void hm1(void);
void hm2(void);
void hm3(void);
void scsel(void);
void delay(uint n);
extern uchar inkey(void);
extern uchar keyin(void);
void szdpz(void);
void szspz(void);
void szspz1(void);
void spdat_dis(void);
void spdat_dis1(void);
void spxm_dis(void);
void ad9850wr(void);

uchar zfbuf[20];
float sddat,jsspz;
float spdat[4];
long kzz;
uchar scgnh,spgnh;
uint sjjg,sdjg;
union sjlx
	{
	long pp;
	char qq[4];
	}sjdat;
sbit AD9850RST = P1^4;
sbit AD9850UP  = P1^3;
sbit ZZD=P1^0;
main()
	{
        uchar i,j;
        spdat[3]=0.001;
        sdjg=spdat[3]*1000;
        initsiu();
        TR0=0;
        yj_init();
        hm0();		//欢迎画面
        inkey();
        scgnh=0;
        do
        {
        scsel();
        if (scgnh==0)
        	{
        	hm1();
        	sddat=1.0;
loop1:     	szdpz();
        	kzz=(long)(107374182.4*sddat);
        	AD9850UP=0;
       		ad9850wr();
loop2:		i=inkey();
			if (i==7)
				goto loop1;
			else if (i!=5)
				goto loop2;
			else
				_nop_();
			kzz=0;
			ad9850wr();
        	}
        else if (scgnh==1)
        	{
        	hm3();
        	spgnh=0;
        	spdat[0]=0.001;
        	spdat[1]=1.0;
        	spdat[2]=0.001;
        	spdat[3]=0.001;
        	szspz();
        	TR0=1;
        	sjjg=0;
        	sdjg=spdat[3]*1000;
        	jsspz=spdat[0];
        	kzz=(long)(107374182.4*jsspz);
        	ad9850wr();
        	do
        		{
        		if (sjjg == sdjg)
        			{
        			ZZD=!ZZD;
        			sjjg=0;
        			jsspz=jsspz+spdat[2];
					if (jsspz > spdat[1])
						jsspz=spdat[0];
					kzz=(long)(107374182.4*jsspz);
					ad9850wr();
					}
				i=keyin();
				}
			while(i!=7);
			_nop_();
			kzz=0;
			ad9850wr();
			TR0=0;
			}
		else
			_nop_();
	}
	while (1);
	}
void ad9850wr(void)
	{
	AD9850RST=0;
	AD9850RST=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	AD9850RST=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	AD9850UP=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	sjdat.pp=kzz;
	AD9850=0;
	_nop_();
	AD9850=sjdat.qq[0];
	_nop_();
	AD9850=sjdat.qq[1];
	_nop_();
	AD9850=sjdat.qq[2];
	_nop_();
	AD9850=sjdat.qq[3];
	_nop_();
	_nop_();
	_nop_();
	AD9850UP=1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	AD9850UP=0;
	_nop_();
	}
void hm0(void)
	{
	yj_clrtext();
	yj_clrgraph();
	dishz(3,10,"DDS信号源");
	dishz(0,46,"河北大学电信学院");
	}
void hm1(void)
	{
	yj_clrtext();
	yj_clrgraph();
	dishz(2,8,"输出频率");
	dishz(2,32,"F=");
	}
void hm2(void)
	{
	yj_clrtext();
	yj_clrgraph();
	if (scgnh==0)
		{
		dishzf(4,8,"点频输出");
		dishz(4,32,"扫频输出");
		}
	else
		{
		dishz(4,8,"点频输出");
		dishzf(4,32,"扫频输出");
		}
	_nop_();
	}
void hm3(void)
	{
	yj_clrtext();
	yj_clrgraph();
	dishz(1,0,"Fs:");
	dishz(1,16,"Fe:");
	dishz(1,32,"Fd:");
	dishz(1,48,"Td:");
	_nop_();
	}
void spxm_dis(void)
	{
	dishz(1,0,"Fs:");
	dishz(1,16,"Fe:");
	dishz(1,32,"Fd:");
	dishz(1,48,"Td:");
	if (spgnh==0)
		dishzf(1,0,"Fs:");
	else if (spgnh==1)
		dishzf(1,16,"Fe:");
	else if (spgnh==2)
		dishzf(1,32,"Fd:");
	else 
		dishzf(1,48,"Td:");
	}
void spdat_dis(void)
	{
	sprintf(zfbuf,"%6.3fMHz",spdat[0]);
	dishz(4,0,zfbuf);
	sprintf(zfbuf,"%6.3fMHz",spdat[1]);
	dishz(4,16,zfbuf);
	sprintf(zfbuf,"%6.3fMHz",spdat[2]);
	dishz(4,32,zfbuf);
	sprintf(zfbuf,"%6.3fs",spdat[3]);
	dishz(4,48,zfbuf);
	}
void spdat_dis1(void)
	{
	if (spgnh==0)
		{
		sprintf(zfbuf,"%6.3fMHz",spdat[0]);
		dishzf(4,0,zfbuf);
		}
	else if (spgnh==1)
		{
		sprintf(zfbuf,"%6.3fMHz",spdat[1]);
		dishzf(4,16,zfbuf);
		}
	else if (spgnh==2)
		{
		sprintf(zfbuf,"%6.3fMHz",spdat[2]);
		dishzf(4,32,zfbuf);
		}
	else if (spgnh==3)
		{
		sprintf(zfbuf,"%6.3fs",spdat[3]);
		dishzf(4,48,zfbuf);
		}
	else 
		_nop_();
	_nop_();
	}
void scsel(void)
	{
	uchar k;
	hm2();
	do
		{
		k=keyin();
		if (k < 5)
			{
			if (scgnh==0)
				scgnh=1;
			else
				scgnh=0;
			hm2();
			}
		}
	while(k!=6);
	_nop_();
	}
void szdpz(void)
	{
	uchar i;
	do
		{
		sprintf(zfbuf,"%9.6fMHz",sddat);
		dishzf(4,32,zfbuf);
		i=inkey();
		if (i != 0xff)
			{
			switch (i)
        		{
        		case 0x01:
        				sddat++;
             			if (sddat>=10.0)
             				sddat=9.999999;
                		break;
             	case 0x04:
             			if (sddat<=1.0)
             				sddat=0.000001;
             			else
             				sddat--;
             			break;
           		case 0x02:	
             			sddat=sddat+0.001;
             			if (sddat>=10.0)
             				sddat=9.999999;
                    	break;
                case 0x03:	
             			if (sddat<=0.001)
             				sddat=0.000001;
             			else
             				sddat=sddat-0.001;
                    	break;
                case 0x05:
                    	sddat=sddat+0.000001;
             			if (sddat>=10.0)
             				sddat=9.999999;
                    		break;
                case 0x06:
                    	if (sddat<=0.000001)
             				sddat=0.000001;
                   		else
                   			sddat=sddat-0.000001;
                   		break;
           		default:
                   		break;
               	}
			}
		}
	while(i!=7);               
	sprintf(zfbuf,"%9.6fMHz",sddat);
	dishz(4,32,zfbuf);
	_nop_();
	}
void szspz(void)
	{
	uchar i;
	spdat_dis();
	do
		{
		spxm_dis();
		i=inkey();
		if (i==1 | i==2)
			{
			if (spgnh==0)
				spgnh=3;
			else
				spgnh--;
			}
		else if (i==3 | i==4)
			{
			spgnh++;
			if (spgnh > 3 )
				spgnh=0;
			}
		else if (i==6)
			{
			szspz1();
			spdat_dis();
			i=6;
			}
		else 
			_nop_();
		}	
	while(i!=7);               
	hm3();
	spdat_dis();
	_nop_();
	}
void szspz1(void)
	{
	uchar i;
	do
		{
		spdat_dis1();
		i=inkey();
		if (i != 0xff)
			{
			switch (i)
        		{
        		case 0x01:
        				spdat[spgnh]++;
             			if (spdat[spgnh]>=10.0)
             				spdat[spgnh]=9.999;
                		break;
             	case 0x04:
             			if (spdat[spgnh]<=1.0)
             				spdat[spgnh]=0.001;
             			else
             				spdat[spgnh]--;
             			break;
           		case 0x02:	
             			spdat[spgnh]=spdat[spgnh]+0.1;
             			if (spdat[spgnh]>=10.0)
             				spdat[spgnh]=9.999;
                    	break;
                case 0x03:	
             			if (spdat[spgnh]<=0.1)
             				spdat[spgnh]=0.001;
             			else
             				spdat[spgnh]=spdat[spgnh]-0.1;
                    	break;
                case 0x05:
                    	spdat[spgnh]=spdat[spgnh]+0.001;
             			if (spdat[spgnh]>=10.0)
             				spdat[spgnh]=9.999;
                    		break;
                case 0x06:
                    	if (spdat[spgnh]<=0.001)
             				spdat[spgnh]=0.001;
                   		else
                   			spdat[spgnh]=spdat[spgnh]-0.001;
                   		break;
           		default:
                   		break;
               	}
			}
		}
	while(i!=7);               
	spdat_dis();
	_nop_();
	}
void initsiu(void)	//初始化单片机
	{
	TMOD=0x11;	//定时器1：gate＝0->TR1＝1开始计数，C/T=0->对内部计数，模式1->16位计数
	TH0=0X0FC;
	TL0=0X018;	//1ms中断
	ET0=1;		//打开定时器0中断
	ET1=0;		//关闭定时器1中断
	TR0=1;		//打开计数器0
	TR1=0;		//关闭计数器1
	EA=1;		//总中断允许
	}
void inttime0(void) interrupt 1 using 1
	{
	TH0=0X0FC;
	TL0=0X018;
	sjjg++;
	}	
/**************************************************************
延时程序 延时时间＝n*15 us
**************************************************************/
/*void delay(uint n)
	{
	do
		{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();    //5
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();    //10
		_nop_();
		//_nop_();
		//_nop_();
		n--;
		}
	while (n);	
	}*/

