#include "StdAfx.h"
#include "CalTime.h"
#include <math.h>
#pragma warning(disable:4305)
CalTime::CalTime(void)
{
	SaLen=200;
	SaCut=800;
	SabLen=2200;
}

CalTime::~CalTime(void)
{
}


void CalTime::CalcTime()
{
	int i,n;
	for(i=0;i<iNArm;i++)
	{
		for(n=0;n<iNSound;n++)
		{
			DC_ReadData(RawData[i][n]);
			if(i==MainSound)
			{
				Time[i][n] = OneCalcTime(0.5, 500, 1000,1);
			}
			else
			{
				Time[i][n]=OneCalcTime(0.2, 700, 1500,2);
			}
		}
	}
}

int CalTime::CTPreProcessData(bool SToSFlag)
{
	int dataOut;
	if (SToSFlag)
	{
		dataOut = OneCalcTime(0.7, 500, 1500,1);//½ü¶Ë
	}
	else
	{
		dataOut = OneCalcTime(0.5, 700, 2000,2);//Ô¶¶Ë
	}
	return dataOut;
}


void CalTime::Init(unsigned int uSL,unsigned int uSC,unsigned int uSb,float SF,float lV)
{
	SaLen=uSL;
	SaCut=uSC;
	SabLen=uSb;
	lVal=lV;
	DC_Init(1,1,0,5120,19700);
	float pa[9]={1.0,-2.8882,6.5012,-8.9618,10.2209,-8.0319,5.2216,-2.0713,0.6429};
	float pb[9]={1.488926959736568655e-02,-3.405162416733406583e-02,5.907373783983541765e-02,
		-7.573863913723941077e-02,8.799047165710537077e-02,-7.573863913723953567e-02,
		5.907373783983541765e-02,-3.405162416733406583e-02,1.488926959736568655e-02};
	SetFilterPara(pa,pb,5120);
}

int  CalTime::ReadData(float mydata[iNumArm][iNumArm][iNumSound][fN],int iArm,int iSound)
{
	int i,n;
	unsigned int m;
	iNArm=iArm;
	iNSound=iSound;
	if((iArm>iNumArm)||(iArm<=0))
	{
		return 1;
	}
	if((iSound>iNumSound)||(iSound<=0))
	{
		return 1;
	}
	for(i=0;i<iArm;i++)
	{
		for(n=0;n<iSound;n++)
		{
			for(m=0;m<Nlength;m++)
			{
				RawData[i][n][m]=mydata[MainSound][i][n][m];
			}
		}
	}
	return 0;
}

int CalTime::Find_First_Big(float data[],int num)
{
	int ffbcount=0,ffbtemp=0,b=0,i=0;
	b=num;
	for(i=0;i<b;i++)
	{
		if(data[i]>0.8)
		{
			ffbtemp=i;
			if(ffbcount!=0)
			{
				if((ffbtemp-ffbcount)<100)
				{
					ffbcount=ffbtemp;
				}
				else
				{
					break;
				}
			}
			else
			{
				ffbcount=ffbtemp;
			}
		}
	}
	return ffbcount;
}
int CalTime::OneCalcTime(float value,int SPoint,int EPoint,int SoundFlag)
{
	int i;
	MyFilter();
//	Nto1(1,1500);
	RMS();
	VmsNto1(1,EPoint);
	Amplifer(EPoint,SoundFlag);
//	Amplifer(1500,SoundFlag);
	sub_2(SPoint,EPoint);
	i=Find_First_Big(OutData,EPoint);
	return i;
	/***
	for(i=SPoint;i<EPoint;i++)
	{
		if(OutData[i]>=value)
		{
			return i;
		}
	}
	return 0;
	***/
}
