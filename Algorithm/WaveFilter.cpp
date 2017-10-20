#include "StdAfx.h"
#include "WaveFilter.h"
#include <memory>
// 
#pragma warning(disable:4018)
WaveFilter::WaveFilter(void)
{
	Fil_N=IIR_N;
	int i;
	for(i=0;i<Fil_N;i++)
	{
		a[i]=0;
		b[i]=0;
	}
	for(i=0;i<fN;i++)
	{
		OutData[i]=0;
	}
}

WaveFilter::~WaveFilter(void)
{
}

void WaveFilter::MyFilter()
{
	float fIn[fN]={0},fOut[fN]={0};
	unsigned int nIn,nOut;
	unsigned int i,m;
	float fXn[IIR_N]={0},fYn[IIR_N]={0};
	float fSum;
	i=0;m=0;
	nIn=0;nOut=0;
	while(m<iNlength)
	{
		for(i=Fil_N-1;i>0;i--)
		{
			if(i<1)
			{
				fXn[i]=0;
			}
			else
			{
				fXn[i]=fXn[i-1];
			}
			if(nOut<i)
			{
				fYn[i]=0;
			}
			else
			{
				fYn[i]=OutData[nOut-i];
			}
		}
		fXn[0]=InData[m];
		fYn[0]=0;
		fIn[nIn]=fXn[0];
		fSum=0;
		for(i=0;i<Fil_N;i++)
		{
			fSum+=(fXn[i]*b[i]);
			fSum-=(fYn[i]*a[i]);
		}
		OutData[nOut]=fSum;
		nOut++;
		nIn++;
		m++;
	}
}

void WaveFilter::SetFilterPara(float pa[],float pb[],int Nl)
{
	int i=0;
	for(i=0;i<Fil_N;i++)
	{
		a[i]=pa[i];
		b[i]=pb[i];
	}
	iNlength=Nl;
}