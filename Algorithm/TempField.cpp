#include "stdafx.h"
#include "TempField.h"

TempField::TempField(void)
{
	Z=20.05;
	NumTempField=5;
	float ftda[10][20] = {
		{ 2.38, 2.62, 0, 0, 0, 4810 },
		{ 4.211, 0, 2.197, 2.427, 0, 9336 },
		{ 0, 2, 3.62, 2, 0, 9144 },
		{ 0, 0, 0, 2.62, 2.38, 4810 },
		{ 0, 2.427, 2.197, 0, 4.211, 9336 } };
	int i,n;
	for(i=0;i<10;i++)
	{
		for(n=0;n<20;n++)
		{
			da[i][n]=ftda[i][n];
		}
	}
	for(i=0;i<M;i++)
	{
		for(n=0;n<M;n++)
		{
			MyTraveTime[i][n]=0;
			Ttime[i][n]=0;
		}
		AreaT[i]=0;
	}
	daOld[0]=4810;
	daOld[1]=9336;
	daOld[2]=9144;
	daOld[3]=4810;
	daOld[4]=9336;
}

TempField::~TempField(void)
{
}

void TempField::BuildAreaT()
{
	int i,j,n;
	for(i=0;i<NumTempField;i++)
	{
		for(n=0;n<NumTempField;n++)
		{
			Ttime[i+1][n+1]=MyTraveTime[i][n]/Z; //Ttime坐标从1开始
		}
	}
	da[0][NumTempField] = 5.0*(5.0*5.0 / (Z*Z*Ttime[1][3] * Ttime[1][3]) * 1000000 - 273.16+500);				//+ 1000);
	da[1][NumTempField] = 8.835*(8.835*8.835 / (Z*Z*Ttime[1][4] * Ttime[1][4]) * 1000000 - 273.16 -300);		//+ 100);
	da[2][NumTempField] = 7.62*(7.62*7.62 / (Z*Z*Ttime[3][4] * Ttime[3][4]) * 1000000 - 273.16 +1500);			//+ 1000);
	da[3][NumTempField] = 5.0*(5.0*5.0 / (Z*Z*Ttime[2][4] * Ttime[2][4]) * 1000000 - 273.16+1250);				// + 300);
	da[4][NumTempField] = 8.835*(8.835*8.835 / (Z*Z*Ttime[2][3] * Ttime[2][3]) * 1000000 - 273.16+1100);		// + 1000);
//	ProcessResult();

	for(i=0;i<NumTempField;i++)
	{
		for(j=0;j<NumTempField+1;j++)
		{
			a1[i][j]=da[i][j];
		}
	}
	if(myNewT(NumTempField))
	{
		for(i=0;i<NumTempField;i++)
		{
			AreaT[i]=X[i];
			CString outdebug;
			outdebug.Format("X%d=%f    ",i,AreaT[i]);
			OutputDebugString(outdebug);
		}
	}
	else
	{
		CString outdebug;
		outdebug.Format("TempArea Eqation error!");
		OutputDebugString(outdebug);
	}
}

void TempField::ProcessResult()
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