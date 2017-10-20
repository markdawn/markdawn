#include "StdAfx.h"
#include "DataClass.h"
#include <math.h>
#pragma warning(disable:4244)
#pragma warning(disable:4305)
DataClass::DataClass(void)
{
	coef=1.732;
	RMSLen1=100;
	RMSLen2=20;
}


DataClass::~DataClass(void)
{

}

int DataClass::RMS()
{
	float bWave[fN]={0};
	int i=0,n=0;
	float tmpWave[fN]={0};
	for(i=0;i<fN;i++)
	{
		bWave[i]=OutData[i];
	}
	for(n=0;n<fN-RMSLen1;n++)
	{
		for(i=0;i<RMSLen2;i++)
		{
			tmpWave[i]=0;
		}
		for(i=n;i<(n+RMSLen2);i++)
		{
			if(i==n)
			{
				if(bWave[i]>0)
					tmpWave[i-n]=(bWave[i]/((i-n)+1));
				else
					tmpWave[i-n]=(bWave[i]*(-1.0)/((i-n)+1));
			}
			else
			{
				tmpWave[i-n]=tmpWave[i-n-1]*tmpWave[i-n-1]*(i-n)/(i-n+1)+bWave[i]*bWave[i]/(i-n+1);
				tmpWave[i-n]=sqrtf(tmpWave[i-n]);
			}
		}
		VmsData[n]=tmpWave[i-n-1];
	}
	for(i=0;i<fN;i++)
		OutData[i]=VmsData[i];
	return 0;
}

int DataClass::DC_Init(bool FF,bool NF,bool VF,int number,int SR)
{
	FilterFlag=FF;
	if(FilterFlag!=FF)  return 0;
	Nto1Flag=NF;
	if(Nto1Flag!=NF)  return 0;
	VoltFlag=VF;
	if(VoltFlag!=VF)  return 0;
	Nlength=number;
	if(Nlength!=number)  return 0;
	SampRate=SR;
	if(SampRate!=SR)  return 0;
	return 1;
}

void DataClass::DC_ReadData(float data[])
{
	unsigned int i=0;
	for(i=0;i<Nlength;i++)
	{
		InData[i]=data[i]*3.0/4095;
		Vdata[i]=InData[i];
	}
}

void DataClass::DC_GetData(float data[])
{
	unsigned int i;
	for(i=0;i<Nlength;i++)
	{
		data[i]=OutData[i];
	}
}

void DataClass::DC_GetVolt(float data[],int number)
{
	int i;
	for(i=0;i<number;i++)
	{
		data[i]=Vdata[i];
	}
}

void DataClass::VmsNto1(int Nbegin,int Nend)
{
	int i;
	float ftemp=0;
	DataMax=0.0;
	for(i=Nbegin;i<Nend;i++)
	{
		ftemp=fabs(VmsData[i]);
		if(ftemp>DataMax)
		{
			DataMax=ftemp;
		}
	}
	for(i=0;i<fN;i++)
	{
		if(OutData[i]>0)
			OutData[i]=OutData[i]/DataMax;
		else
			OutData[i]=-1.0*OutData[i]/DataMax;
	}
}

int DataClass::sub_2(int StartPoint,int EndPoint)
{
	float subtemp[fN]={0};
	int i;
	for(i=StartPoint;i<EndPoint;i++)
	{
		subtemp[i]=(OutData[i]-OutData[i-1])*10.0;
	}
	for(i=0;i<fN;i++)
	{
		OutData[i]=subtemp[i];
	}
	return 0;
}
void DataClass::Nto1(int Nbegin,int Nend)
{
	int i;
	float ftemp=0;
	DataMax=0.0;
	for(i=Nbegin;i<Nend;i++)
	{
		ftemp=fabs(OutData[i]);
		if(ftemp>DataMax)
		{
			DataMax=ftemp;
		}
	}
	for(i=Nbegin;i<Nend;i++)
	{
		OutData[i]=OutData[i]/DataMax;
	}
}

void DataClass::Amplifer(unsigned int Nlen,int SFlag)           //SFlag=1 ½ü¶Ë =2 Ô¶¶Ë
{
	float tempx[fN]={0},tempy[fN]={0};
	unsigned int i;
	if(SFlag==1)
	{
		for(i=0;i<Nlen;i++)
		{
			tempx[i]=abs(OutData[i]);
			if(tempx[i]<0.5)
			{
				tempx[i]=0.0;
			}
			else
			{
//				tempx[i]=coef*sqrtf(tempx[i]);
				tempx[i]=1;
			}
			OutData[i]=tempx[i];
		}
	}
	else
	{
		for(i=0;i<Nlen;i++)
		{
			tempx[i]=abs(OutData[i]);
			if(tempx[i]<0.2)
			{
				tempx[i]=0.0;
			}
			else
			{
				//				tempx[i]=coef*sqrtf(tempx[i]);
				tempx[i]=1;
			}
			OutData[i]=tempx[i];
		}
	}
}

int DataClass::DC_CalcTime(float value,int SPoint,int EPoint,int SoundFlag)
{
	int i;
	MyFilter();
	Nto1(1,1500);
	Amplifer(1500,SoundFlag);
	for(i=SPoint;i<EPoint;i++)
	{
		if(OutData[i]>=value)
		{
			return i;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}