#include "stdafx.h"
#include "SoundAndReceive.h"
#include <afx.h>
#include <afxwin.h>


SoundAndReceive::SoundAndReceive(void)
{
}

SoundAndReceive::~SoundAndReceive(void)
{
	delete []SnRModule;
}

int SoundAndReceive::SnR_Init(int *order,CString *ip,int *port,int n,int GasTime,int timeout)
{
	iArmModuleNum=n;
	GasDelayTime=GasTime;
	if(n>iNumArm)
	{
		AfxMessageBox("输入的模块个数大于系统默认值");
		return 1;
	}
	SnRModule=new ArmModule[n];
	int i;
	for(i=0;i<iArmModuleNum;i++)
	{
		SoundOrder[i]=order[i];
		if(SnRModule[i].AMInit(order[i],ip[i],port[i],timeout))
		{
			CString tempdbg;
			tempdbg.Format("No. %n Arm Init failure! \n",order[i]);
			OutputDebugString(tempdbg);
		}
	}
	return 0;
}

void SoundAndReceive::DbgCallOutArmStatus(int n)
{
		CString debugstr;
		debugstr.Format("Arm %d Status=%d \n",n,SnRModule[n].ArmStatus);
		OutputDebugString(debugstr);
}

int SoundAndReceive::SnR_Circle(int ArmSound)
{
	int iReceive,n,i,m;
	int iTimeSound;
	CString tmpdbg;
	n=ArmSound;
	//SnRModule[1].GasClose();
	//for(m=0;m<GasDelayTime;m++)
	//	Sleep(1000);
	for(iTimeSound=0;iTimeSound<iNumSound;iTimeSound++)
	{
//		DbgCallOutArmStatus(n);
		if(SnRModule[n].Sound())
		{
			tmpdbg.Format("No.%d Arm Module Sound failed!\n",n+1);
			OutputDebugString(tmpdbg);
			return 1;
		}
		Sleep(1000);    //sound status
		//DbgCallOutArmStatus(n);
		for(iReceive=0;iReceive<iArmModuleNum;iReceive++)
		{
//			Sleep(1000);
			m=SoundOrder[iReceive]-1;
//			DbgCallOutArmStatus(m);
			if(SnRModule[m].RecData())
			{
				tmpdbg.Format("No.%d Arm Module recdata failed\n",m+1);
				OutputDebugString(tmpdbg);
			}
			{
				tmpdbg.Format("No.%d Arm Module status=%d\n",m+1,SnRModule[m].ArmStatus);
				OutputDebugString(tmpdbg);
			}
			if(SnRModule[m].ArmFlag)
			{
				tmpdbg.Format("No.%d Arm Module Warning\n",m+1);
				OutputDebugString(tmpdbg);
			}
//			DbgCallOutArmStatus(m);
			if(!SnRModule[m].SensorFlag)
			{
				for(i=0;i<5120;i++)
				{
					ArmSoundData[ArmSound][m][iTimeSound][i]=SnRModule[m].fData[i];
				}
			}
			else
			{
				tmpdbg.Format("No. %d sensor is fault!\n",m+1);
				OutputDebugString(tmpdbg);
			}
		}
	}
    //SnRModule[1].GasOpen();
	//for(m=0;m<GasDelayTime;m++)
	//	Sleep(1000);
	return 0;
}