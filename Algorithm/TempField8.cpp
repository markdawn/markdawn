#include "StdAfx.h"
#include "TempField8.h"

TempField8::TempField8(void)
{
	int i,j;
	float temps[M8][M8]={{212.13,106.07,0,0,106.07,0,0,0,0,0,0,0,0,0,0,0},
	{83.85,251.56,167.71,0,167.71,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,225,225,225,225,0,0,0,0,0,0,0,0},
	{0,0,0,0,237.17,237.17,237.17,237.17,0,0,0,0,0,0,0},
	{0,0,0,0,212.13,0,0,0,106.07,212.13,0,0,0,106.07,212.13,0},
	{0,0,0,0,167.71,0,0,0,251.56,0,0,0,83.85,167.71,0,0},
	{83.85,167.71,0,0,251.56,0,0,0,167.71,0,0,0,0,0,0,0},
	{0,106.07,212.13,0,106.07,212.13,0,0,212.13,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,237.17,237.17,237.17,237.17,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,225,225,225,225,0,0,0,0},
	{0,0,0,0,0,0,0,0,167.71,0,0,0,83.85,251.56,167.7,0},
	{0,0,0,0,0,0,0,0,106.07,0,0,0,212.13,106.07,0,0},
	{0,225,0,0,0,225,0,0,0,225,0,0,0,225,0,0},
	{0,0,237.17,0,0,0,237.17,0,0,237.17,0,0,0,237.17,0,0},
	{0,0,0,0,0,0,0,212.13,0,0,212.13,106.07,0,212.13,106.07,0},
	{0,0,0,0,0,0,0,0,0,0,0,167.71,0,167.71,251.6,83.85},
	{0,231.17,0,0,0,237.17,0,0,0,0,237.17,0,0,0,237.17,0},
	{0,0,225,0,0,0,225,0,0,0,225,0,0,0,225,0},
	{0,0,0,0,0,0,0,167.71,0,0,0,251.56,0,0,167.71,83.85},
	{0,0,0,0,0,0,0,0,0,0,0,106.07,0,0,106.07,212.13},
	{0,212.13,106.07,0,0,0,212.13,106.07,0,0,0,212.13,0,0,0,0},
	{0,0,167.71,83.85,0,0,0,251.56,0,0,0,167.71,0,0,0,0},
	{0,167.71,251.56,83.85,0,0,0,167.71,0,0,0,0,0,0,0,0},
	{0,0,106.07,212.13,0,0,0,106.07,0,0,0,0,0,0,0,0}};
	z=20.05;
	for(i=0;i<M8;i++)
	{
		for(j=0;j<M8;j++)
		{
			t[i][j]=0;
			s[i][j]=0;
			d[i][j]=0;
			tt[i][j]=0;
			ti[i][j]=0;
			ts[i][j]=0;
			s[i][j]=temps[i][j];
		}
	}
	float tempt[1][M8]={0.011953288,0.018899814,0.024616755,0.025948248,
		0.023557626,0.018899814,0.018899814,0.023557626,0.025948248,
		0.024616755,0.018899532,0.011953288,0.024616755,0.025948248,
		0.023557626,0.018900941,0.025779206,0.024616755,0.018899814,
		0.011953288,0.023557626,0.018899814,0.018899814,0.011953288};
	for(i=0;i<24;i++)
	{
		t[0][i]=tempt[0][i];
	}
}

TempField8::~TempField8(void)
{
}

void TempField8::lsm()
{
//    int i,j,k;
    T(s,ts,n,m);
    mul(ts,s,tt,n,m);
    inv(tt,ti,n,m);
    mul(ti,ts,tt,n,m);
    mul(tt,t,d,n,m);
}

void TempField8::GetT(float tei[],int m1)
{
    int i;
    for(i=0;i<m1;i++)
    {
        tei[i]=1/((d[i][0]*z)*(d[i][0]*z));
    }
}

void TempField8::Sets(float x[][M8],int n1,int m1)
{
    int i,j;
    n=n1;
    m=m1;
    for(i=0;i<n1;i++)
        for(j=0;j<m1;j++)
            s[i][j]=x[i][j]/100;
}

void TempField8::Sett(float x[],int n1)
{
    int i;
    for(i=0;i<n1;i++)
        t[i][0]=x[i];
}

void TempField8::Sett(float x[][M8],int n1,int m1)
{
    int i,j;
    for(i=0;i<n1;i++)
        for(j=0;j<m1;j++)
            t[j][i]=x[i][j];
}
