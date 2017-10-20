#include "StdAfx.h"
#include "FlightTime.h"
#pragma warning(disable:4244)
FlightTime::FlightTime(void)
{
	int i,n,m;
	for(i=0;i<iNumArm;i++)
	{
		for(n=0;n<iNumArm;n++)
		{
			for(m=0;m<iNumSound;m++)
			{
				Pdata[i][n][m]=0;
			}
			ValidCount[i][n]=0;
			FTime[i][n]=0;
		}
	}
}

FlightTime::~FlightTime(void)
{
}

int FlightTime::Max(int* data,int num)
{
	int i=0;
	int m=0;
	for(i=0;i<num;i++)
	{
		if(m<data[i])
		{
			m=data[i];
		}
	}
	return m;
}

float FlightTime::Max(float* data,int num)
{
	int i=0;
	float m=0;
	for(i=0;i<num;i++)
	{
		if(m<data[i])
		{
			m=data[i];
		}
	}
	return m;
}

int FlightTime::Min(int* data,int num)
{
	int i=0;
	int m=2000;
	for(i=0;i<num;i++)
	{
		if(m>data[i])
		{
			m=data[i];
		}
	}
	return m;
}
float FlightTime::Min(float* data,int num)
{
	int i=0;
	float m=2000.0;
	for(i=0;i<num;i++)
	{
		if(m>data[i])
		{
			m=data[i];
		}
	}
	return m;
}

int FlightTime::PreProcessData(int n)       
{
	int temp;
	temp=MyData[1][n]-MyData[0][n];
	if(temp<=0)
	{
		temp=0;
	}
	return temp;
}

float FlightTime::ThreeInFive(int* data,int num,int imode)  //imode:  1--wape of values of the max and min
														   //       2--wape of values of the max and min in variance
{
	int total=0;
	int i=0;
	float result=0.0;
	float aver=0;
	float dis[iNumSound]={0};
	int t;
	float p;
	int cn[2]={0};
	switch(imode)
	{
	case 1:
		for (i=0;i<num;i++)
		{
			total=total+data[i];
		}
		total=total-Max(data,num)-Min(data,num);
		result=total/(num-2);
		break;
	case 2:
		for(i=0;i<num;i++)
		{
			total=total+data[i];
		}
		aver=1.0*(total-Max(data,num)-Min(data,num))/(num-2);
		for(i=0;i<num;i++)
		{
			dis[i]=(data[i]-aver)*(data[i]-aver);
		}
		for(t=0;t<2;t++)
		{
			p=Max(dis,num);
			for(i=0;i<num;i++)
			{
				if(dis[i]==p)
				{
					cn[t]=i;
					dis[i]=0;
				}
			}
		}
		result=1.0*(total-data[cn[0]]-data[cn[1]])/(num-2);
		break;
	default:
		result=0;
	}
	return result; 
}

int FlightTime::CalcFlightTime(int arithflag)
{
	int i=0,n=0;
	if(arithflag==1)
	{
		for(i=0;i<iNumSound;i++)
		{
			MyData[0][i]=iTime[iSound][i];
		}
		for(i=0;i<iNumArm;i++)
		{
			ValidCount[iSound][i]=0;
		}
		for(i=0;i<iNumArm;i++)
		{	
			if(i!=iSound)
			{
				for(n=0;n<iNumSound;n++)
				{
					MyData[1][n]=iTime[i][n];
					Pdata[iSound][i][n]=PreProcessData(n);
					if(Pdata[iSound][i][n]!=0)
					{
						ValidCount[iSound][i]++;
					}
				}
				if(ValidCount[iSound][i]<3)
				{
					iTime[iSound][i]=0;
				}
				else
				{
					FTime[iSound][i]=ThreeInFive(Pdata[iSound][i],ValidCount[iSound][i],1);
				}
			}
		}
	}
	else
	{
		int temp[2];
		int zeroCount;
		zeroCount=0;
		for(i=0;i<iNumSound;i++)
		{
			if(iTime[iSound][i]==0)
			{
				zeroCount++;
			}
			else
			{
				MyData[0][i-zeroCount]=iTime[iSound][i];
			}
		}
		if(zeroCount>2)
		{
			temp[0]=0;
		}
		else
		{
			temp[0]=ThreeInFive(MyData[0],iNumSound-zeroCount,1);
		}
		zeroCount=0;
		for(i=0;i<iNumArm;i++)
		{	
			if(i==iSound)
			{
				continue;
			}
			else
			{
				for(n=0;n<iNumSound;n++)
				{
					if(iTime[i][n]==0)
					{
						zeroCount++;
					}
					else
					{
						MyData[1][n-zeroCount]=iTime[i][n];
					}
				}
				if(zeroCount>2)
				{
					temp[1]=0;
				}
				else
				{
					temp[1]=ThreeInFive(MyData[1],iNumSound-zeroCount,1);
				}
			}
			if(temp[0]<1)
			{
				pFTime[iSound][i]=0;
			}
			else
			{
				if(temp[1]<1)
				{
					pFTime[iSound][i]=0;
				}
				else
				{
					pFTime[iSound][i]=temp[1]-temp[0];
					if(pFTime[iSound][i]<1)
					{
						pFTime[iSound][i]=0;
					}
				}
			}
		}
	}
	for(i=0;i<iNumArm;i++)
	{
//		AfterProcessData(iSound,i);
		FTime[iSound][i]=pFTime[iSound][i];
	}
	return 0;
}

void FlightTime::AfterProcessData(int iSound,int iRec)
{
	int Level[5][5]={0};
	Level[0][1]=153;
	Level[1][0]=Level[0][1];
	Level[0][2]=300;
	Level[2][0]=Level[0][2];
	Level[0][4]=155;
	Level[4][0]=Level[0][4];
	Level[0][3]=308;
	Level[3][0]=Level[0][3];
	Level[1][4]=231;
	Level[4][1]=Level[1][4];
	Level[1][3]=289;
	Level[3][1]=Level[1][3];
	Level[2][3]=215;
	Level[3][2]=Level[2][3];
	Level[2][4]=292;
	Level[4][2]=Level[2][4];

	if(iSound==0)
	{
		if(iRec==2)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]-30;
		if(iRec==3)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]-30;	
		if(iRec==4)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]-20;
	}
	if(iSound==1)
	{
		if(iRec==4)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]+40;
		if(iRec==3)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]+40;
	}
	if(iSound==2)
	{
		if(iRec==3)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]-35;
	}
	if(iSound==4)
	{
		if(iRec==2)
			pFTime[iSound][iRec]=pFTime[iSound][iRec]+20;
	}
	if(pFTime[iSound][iRec]<Level[iSound][iRec])
	{
		pFTime[iSound][iRec]=Level[iSound][iRec];
	}
}