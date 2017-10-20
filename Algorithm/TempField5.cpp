#include "StdAfx.h"
#include "TempField5.h"
#include <math.h>

#pragma warning(disable:4244)
#pragma warning(disable:4305)


TempField5::TempField5(void)
{
	int i, n;
	float temp[6][8] = {
		0.1962,-0.0001,0.1962,-0.0001,-0.0918,-0.0319,0.0394,-0.0319,
		0.0244,0.0470,-0.0246,-0.0468,0.0916,0.0769,-0.0397,-0.0127,
		-0.0246,-0.0468,0.0244,0.0470,0.0916,-0.0127,-0.0397,0.0769,
		-0.1209,0.2475,-0.1216,0.0867,-0.0912,-0.1154,0.0401,0.0505,
		0.2422,-0.3339,0.2422,-0.3339,0.0633,0.1611,0.1946,0.1611,
		-0.1216,0.0867,-0.1209,0.2475,-0.0912,0.0505,0.0401,-0.1154};
	float tempib[3][3]={
		0.1961,-0.1312,0.1961,
		0.1961,0.1312,-0.1961,
		-0.1961,0.1312,0.1961};										//[12;25;15]
	for(i=0;i<3;i++)
	{
		for(n=0;n<3;n++)
		{
//			ib[i][n]=tempib[i][n];
		}
	}
	for (i = 0; i < 6; i++)
	{
		for (n = 0; n < 8; n++)
		{
			S[i][n] = temp[i][n];
		}
	}
	Z = 19.07;
	NumTempField = 8;
	for (i = 0; i < M; i++)
	{
		for (n = 0; n < M; n++)
		{
			MyTraveTime[i][n] = 0;
			Ttime[i][n] = 0;
		}
		AreaT[i] = 0;
	}
	daOld[0] = 4860;
	daOld[1] = 9950;
	daOld[2] = 4860;
	daOld[3] = 9950;
	daOld[4] = 7524;
	daOld[5] = 9714;
	daOld[6] = 7524;
	daOld[7] = 9714;
	for(i=0;i<8;i++)
	{
		averTold[i]=0;
	}
	SigmoidTemp=800.0;
}
//TempField5::TempField5(void)
//{
//	Z=20.05;
//	NumTempField=8;
//	float ftda[10][20] = {
//		{ 2.43,    0,  2.43,     0,     0,     0,     0,     0, 4860 },
//		{ 1.66,    0,  3.31,  1.66,     0,  3.32,     0,     0, 9950 },
//		{    0, 2.43,     0,     0,  2.43,     0,     0,     0, 4860 },
//		{    0, 1.66,     0,  1.66,  3.31,     0,     0,  3.32, 9950 },
//		{    0,    0, 2.508, 2.508, 2.508,     0,     0,     0, 7524 },
//		{    0,    0, 3.238, 1.619,     0,     0, 1.619, 3.238, 9714 },
//		{    0,    0,     0,     0,     0, 2.508, 2.508, 2.508, 7524 },
//		{    0,    0,     0, 1.619, 3.238, 3.238, 1.619,     0, 9714 }};
//		int i,n;
//		for(i=0;i<10;i++)
//		{
//			for(n=0;n<20;n++)
//			{
//				da[i][n]=ftda[i][n];
//			}
//		}
//		for(i=0;i<M;i++)
//		{
//			for(n=0;n<M;n++)
//			{
//				MyTraveTime[i][n]=0;
//				Ttime[i][n]=0;
//			}
//			AreaT[i]=0;
//		}
//		daOld[0]=4860;
//		daOld[1]=9950;
//		daOld[2]=4860;
//		daOld[3]=9950;
//		daOld[4]=7524;
//		daOld[5]=9714;
//		daOld[6]=7524;
//		daOld[7]=9714;
//}


TempField5::~TempField5(void)
{
}
float TempField5::sigmoid(float x)
{
	float a=0.0;
	float temp=0.0;
	temp=exp((SigmoidTemp-x)/SigmoidTemp);
	a=1600.0/(1.0+temp);
	return a;
}

float TempField5::sigmoid(float x,float sigtemp,float toptemp)
{
	float a=0.0;
	float temp=0.0;
	temp=exp((sigtemp-x)/sigtemp);
	a=toptemp/(1.0+temp);
	return a;
}
void TempField5::BuildAreaT_LSM(bool sflag)
{
	int i, n ;
	float temp,avertemp;
	float Ptemp[8];
	bool DataErr;
	temp = 0;
	DataErr=false;
//	float temp_Dis[8]={4.86,9.95,4.86,9.95,7.524,9.714,7.524,9.714};
	float temp_Dis[8]={5.1,9.68,5.1,9.68,7.62,9.4,7.62,9.4};
	float PMax[8]={71,134,71,134,119,119,119,119};
	float PMin[8]={6.65,12.625,6.65,12.625,8.83,13.15,8.83,13.15};
	//float PMin[8] = { 0 };
	P[0] = (MyTraveTime[0][1]) / 19.7;			//39.4=2*19700/1000
	P[1] = (MyTraveTime[0][2]) / 19.7;
	P[2] = (MyTraveTime[0][4]) / 19.7;
	P[3] = (MyTraveTime[0][3]) / 19.7;
	P[4] = (MyTraveTime[1][4]) / 19.7;
	P[5] = (MyTraveTime[1][3]) / 19.7;
	P[6] = (MyTraveTime[2][3]) / 19.7;
	P[7] = (MyTraveTime[2][4]) / 19.7;
	DP[0]= (MyTraveTime[1][0]) / 19.7;			//39.4=2*19700/1000
	DP[1] = (MyTraveTime[2][0]) / 19.7;
	DP[2] = (MyTraveTime[4][0]) / 19.7;
	DP[3] = (MyTraveTime[3][0]) / 19.7;
	DP[4] = (MyTraveTime[4][1]) / 19.7;
	DP[5] = (MyTraveTime[3][1]) / 19.7;
	DP[6] = (MyTraveTime[3][2]) / 19.7;
	DP[7] = (MyTraveTime[4][2]) / 19.7;
	//数据容错处理
	//temp=P[0]*0.7+P[1]*0.3;
	//for(i=1;i<8;i++)
	//{
	//	if(abs(DP[i]-P[i])>2)
	//	{
	//		if(abs(P[i]-temp)>abs(DP[i]-temp))
	//		{
	//			P[i]=DP[i];
	//		}
	//	}
	//}
	for(i=0;i<8;i++)
	{
		if (P[i]<=5)
		{
			P[i]=PMax[i];
			DataErr=true;
		}
		else if(P[i]>PMax[i])
		{
			P[i]=PMax[i];
		}
		else if(P[i]<PMin[i])
		{
			P[i]=PMin[i];
		}
		if (DP[i]<=5)
		{
			DP[i]=PMax[i];
		}
		else if(DP[i]>PMax[i])
		{
			DP[i]=PMax[i];
		}
		else if(DP[i]<PMin[i])
		{
			DP[i]=PMin[i];
		}
	}
	for(i=0;i<8;i++)
	{
		temp=temp_Dis[i]/P[i]/Z;
		aver_T_P[i]=temp*temp*1000000;
		if(i==5||i==7)
		{
			if(aver_T_P[i]>SigmoidTemp)
			{
				aver_T_P[i]=sigmoid(aver_T_P[i],SigmoidTemp,2300);
			}
		}
		else
		{
			if(aver_T_P[i]>SigmoidTemp)
			{
				aver_T_P[i]=sigmoid(aver_T_P[i]);
			}
		}
		aver_P[i]=temp_Dis[i]/(sqrtf(aver_T_P[i]/1000000)*Z);
	}
	for(i=0;i<8;i++)
	{
		temp=temp_Dis[i]/DP[i]/Z;
		aver_T_P[i+8]=temp*temp*1000000;
		if(aver_T_P[i+8]>SigmoidTemp)
		{
			aver_T_P[i+8]=sigmoid(aver_T_P[i+8]);
		}
		aver_P[i+8]=temp_Dis[i]/(sqrtf(aver_T_P[i+8]/1000000)*Z);
	}
	temp=0;
	if(!DataErr)
	{
		Ptemp[0]=aver_P[0]+0.5;	//7.64;
		Ptemp[1]=2*aver_P[0]+0.6;	//14.5
		Ptemp[2]=aver_P[2]+1.2;	//7,64
		Ptemp[3]=2*aver_P[2]+0.6;	//14.5;
		Ptemp[4]=aver_P[4]-1.9;	//9.76;
		Ptemp[5]=aver_P[5]+3.0;	//15.41;
		Ptemp[6]=aver_P[6]-1.5;	//9.76;
		Ptemp[7]=aver_P[7]+3.0;	//15.41;

		avertemp=aver_T_P[8]*0.3+aver_T_P[0]*0.2+aver_T_P[4]*0.4+aver_T_P[2]*0.1;
		aver_T_P[0]=avertemp;
	
		if((Ptemp[2]-Ptemp[0])>0.3)
		{
			Ptemp[2]=Ptemp[0]+0.3;
		}
		else if((Ptemp[0]-Ptemp[2])>0.3)
		{
			Ptemp[2]=Ptemp[0]-0.3;
		}
		if((Ptemp[3]-Ptemp[1])>0.3)
		{
			Ptemp[3]=Ptemp[1]+0.3;
		}
		else if((Ptemp[1]-Ptemp[3])>0.3)
		{
			Ptemp[3]=Ptemp[1]-0.3;
		}

		if((Ptemp[5]-Ptemp[1])>3.0)
		{
			Ptemp[5]=Ptemp[1]+3.0;
		}
		else if(Ptemp[5]<(Ptemp[1]+0.3))
		{
			Ptemp[5]=Ptemp[1]+0.3;
		}
		if((Ptemp[7]-Ptemp[3])>3.0)
		{
			Ptemp[7]=Ptemp[3]+3.0;
		}
		else if(Ptemp[7]<(Ptemp[3]+0.3))
		{
			Ptemp[7]=Ptemp[3]+0.3;
		}

		if(Ptemp[0]/Ptemp[4]>0.72)
		{
			Ptemp[4]=1.38*Ptemp[0];
		}
		else if(Ptemp[0]/Ptemp[4]<0.65)
		{
			Ptemp[4]=1.40*Ptemp[0];
		}
		if(Ptemp[4]/Ptemp[5]<0.7)
		{
			Ptemp[5]=Ptemp[4]/0.7;
		}
		if((Ptemp[6]-Ptemp[4])>0.2)
		{
			Ptemp[6]=Ptemp[4]+0.2;
		}
		else if((Ptemp[4]-Ptemp[6])>0.2)
		{
			Ptemp[6]=Ptemp[4]-0.2;
		}
		if((Ptemp[5]-Ptemp[7])>0.5)
		{
			Ptemp[7]=Ptemp[5]-0.5;
		}
		else if((Ptemp[7]-Ptemp[5])>0.5)
		{
			Ptemp[7]=Ptemp[5]+0.5;
		}
	}
	else
	{
		Ptemp[0]=aver_P[0];	//7.64;
		Ptemp[1]=aver_P[1];	//14.5
		Ptemp[2]=aver_P[2];	//7,64
		Ptemp[3]=aver_P[3];	//14.5;
		Ptemp[4]=aver_P[4];	//9.76;
		Ptemp[5]=aver_P[5];	//15.41;
		Ptemp[6]=aver_P[6];	//9.76;
		Ptemp[7]=aver_P[7];	//15.41;
	}
	if(sflag)
	{
		for (i = 0; i < 6; i++)
		{
			temp = 0;
			for (n = 0; n < 8; n++)
			{
				temp = temp + S[i][n] * Ptemp[n];
			}
			if (temp > 0)
			{
				AreaT[i] = 1000000 / (temp*temp*Z*Z);
				if(AreaT[i]>SigmoidTemp)
				{
					AreaT[i]=sigmoid(AreaT[i]);
				}
			}
			else
			{
				AreaT[i] = 0;
			}
		}
	}
	else
	{
		for (i = 0; i < 6; i++)
		{
			temp = 0;
			for (n = 0; n < 8; n++)
			{
				temp = temp + S[i][n] * Ptemp[n];
			}
			if (1)
			{
				AreaT[i] = 1000000 / (temp*temp*Z*Z);
				if(AreaT[i]>SigmoidTemp)
				{
					AreaT[i]=sigmoid(AreaT[i]);
				}
				if((AreaT[i]-averTold[i])>100)
				{
//					AreaT[i]=averTold[i]+100;
				}
				else if((averTold[i]-AreaT[i])>100)
				{
//					AreaT[i]=averTold[i]-100;
				}
			}
		}
	}
	if((AreaT[3]-AreaT[4])>200.0)
	{
	//	AreaT[4]=(AreaT[5]+AreaT[3])/2.0;
	}
	if((AreaT[5]-AreaT[4])>200.0)
	{
	//	AreaT[4]=(AreaT[4]+AreaT[5])/2.0;
	}
	avertemp=AreaT[0]*0.5+AreaT[1]*0.4+AreaT[2]*0.1;
	for(i=1;i<6;i++)
	{
		temp=AreaT[i]-avertemp;
		if(temp>300.0)
		{
//			AreaT[i]=(AreaT[i]+avertemp)/2+300;
		}
		if(temp<-300.0)
		{
//			AreaT[i]=(AreaT[i]+avertemp)/2-300;
		}
	}

 	for(i=0;i<6;i++)
	{
		averTold[i]=AreaT[i];
	}
}
/*
void TempField5::BuildAreaT()
{
	int i, n ;
	float temp;
	temp = 0;
	float temp_Dis[8]={5.1,9.68,5.1,9.68,7.62,9.4,7.62,9.4};
	P[0] = (MyTraveTime[0][1]) / 19.7;			//39.4=2*19700/1000
	P[1] = (MyTraveTime[0][2]) / 19.7;
	P[2] = (MyTraveTime[0][4]) / 19.7;
	P[3] = (MyTraveTime[0][3]) / 19.7;
	P[4] = (MyTraveTime[1][4]) / 19.7;
	P[5] = (MyTraveTime[1][3]) / 19.7;
	P[6] = (MyTraveTime[2][3]) / 19.7;
	P[7] = (MyTraveTime[2][4]) / 19.7;
	DP[0]= (MyTraveTime[1][0]) / 19.7;			//39.4=2*19700/1000
	DP[1] = (MyTraveTime[2][0]) / 19.7;
	DP[2] = (MyTraveTime[4][0]) / 19.7;
	DP[3] = (MyTraveTime[3][0]) / 19.7;
	DP[4] = (MyTraveTime[4][1]) / 19.7;
	DP[5] = (MyTraveTime[3][1]) / 19.7;
	DP[6] = (MyTraveTime[3][2]) / 19.7;
	DP[7] = (MyTraveTime[4][2]) / 19.7;
	for(i=0;i<8;i++)
	{
		temp=temp_Dis[i]/P[i]/19.07;
		aver_T_P[i]=temp*temp*1000000;
		if(aver_T_P[i]>SigmoidTemp)
		{
			aver_T_P[i]=sigmoid(aver_T_P[i]);
		}
	}
	for(i=0;i<8;i++)
	{
		temp=temp_Dis[i]/DP[i]/19.07;
		aver_T_P[i+8]=temp*temp*1000000;
		if(aver_T_P[i+8]>SigmoidTemp)
		{
			aver_T_P[i+8]=sigmoid(aver_T_P[i+8]);
		}
	}
	temp=0;
//	PB[0]=P[0];
//	PB[1]=P[4];
//	PB[2]=P[2];
	for (i = 0; i < 6; i++)
	{
		temp = 0;
		for (n = 0; n < 8; n++)
		{
			temp = temp + S[i][n] * P[n];
		}
		if (temp != 0)
		{
			AreaT[i] = 1000000 / (temp*temp*Z*Z);
		}
		else
		{
			AreaT[i] = 0;
		}
	}
	for(i=0;i<6;i++)
	{
		if(AreaT[i]>SigmoidTemp)
		{
			AreaT[i]=SigmoidTemp;
		}
	}
	if(AreaT[3]-AreaT[4]>200)
		AreaT[4]=(AreaT[5]+AreaT[3])/2.0;
	if(AreaT[5]-AreaT[4]>200)
		AreaT[4]=(AreaT[4]+AreaT[5])/2.0;
	for(i=0;i<3;i++)
	{
		temp=0;
		for(n=0;n<3;n++)
		{
			temp=temp+ib[i][n]*PB[n];
		}
		if (temp != 0)
		{
			AreaTB[i] = 1000000 / (temp*temp*Z*Z);
			if(AreaTB[i]>SigmoidTemp)
				AreaTB[i]=sigmoid(AreaTB[i]);
		}
		else
		{
			AreaTB[i] = 0;
		}
	}
}
*/
void TempField5::ProcessResult()
{
	int i;
	for (i = 0; i < NumTempField; i++)
	{
		f_da[i] = da[i][NumTempField];
	}

	for (i = 0; i < NumTempField; i++)
	{
		if ((da[i][NumTempField] - daOld[i]) > 100)
		{
			da[i][NumTempField] = daOld[i] + 100;
			daOld[i] = da[i][NumTempField];
		}
		else if ((daOld[i] - da[i][NumTempField]) > 100)
		{
			da[i][NumTempField] = daOld[i] - 100;
			daOld[i] = da[i][NumTempField];
		}
		else
		{
			daOld[i] = da[i][NumTempField];
		}
	}
}

float TempField5::fabs(float fnum)
{
	if(fnum<0)
		return -1*fnum;
	else
		return fnum;
}