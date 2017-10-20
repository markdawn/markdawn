#include "StdAfx.h"
#include "ArmModule.h"
#include <string>
#pragma warning(disable:4244)
ArmModule::ArmModule(void)
{
	Num=0;
	SensorFlag=0;
	ArmDataFlag=0;
	ArmFlag=0;
	ArmStatus=0;
	GasStatus=1;
	memset(fData,0,fN*sizeof(float));
	VMax=4095.0;
	Volt=3.0;
}

ArmModule::~ArmModule(void)
{
}

int ArmModule::AMInit(int num,CString addr,int port,int timeout)
{
	Num=num;
	IP=addr;
	Port=port;
	TimeOut=timeout*1000;
	return Comm_Init(IP,port);
}

void ArmModule::TestSensor()
{
	float Vsum=0;
	int i=0;
	for(i=0;i<fN;i++)
	{
		Vsum=Vsum+fData[i]*Volt/VMax;
	}
	Vsum=Vsum/fN;
	if((Vsum<1.0)||(Vsum>2.5))
	{
		SensorFlag=1;
	}
	else
	{
		SensorFlag=0;
	}
}

int ArmModule::GasOpen()
{
	if(Comm_Conn())
	{
		if(Comm_Init(IP,Port))
		{
			ArmFlag=TRUE;
			return 1;
		}
		if(Comm_Conn())
		{
			ArmFlag=TRUE;
			return 1;
		}
	}
	if(!Comm_GasCtl(1))
	{
		GasStatus=1;
		ArmFlag=FALSE;
		Comm_Disconn();
		return 0;
	}
	ArmFlag=TRUE;
	return 1;
}

int ArmModule::GasClose()
{
	if(Comm_Conn())
	{
		if(Comm_Init(IP,Port))
		{
			ArmFlag=TRUE;
			return 1;
		}
		if(Comm_Conn())
		{
			ArmFlag=TRUE;
			return 1;
		}
	}
	if(!Comm_GasCtl(0))
	{
		GasStatus=0;
		ArmFlag=TRUE;
		Comm_Disconn();
		return 0;
	}
	ArmFlag=TRUE;
	return 1;
}

int ArmModule::Sound()
{
	if(Comm_Conn())
	{
		if(Comm_Init(IP,Port))
		{
			ArmFlag=TRUE;
			return 1;
		}
		if(Comm_Conn())
		{
			ArmFlag=TRUE;
			return 1;
		}
	}
	if(!Comm_Sound())
	{
		ArmStatus=1;
		ArmFlag=FALSE;
		Comm_Disconn();
		return 0;
	}
	ArmFlag=TRUE;
	return 1;
}

int ArmModule::RecData()
{
	int i;
	ArmStatus=2;
	Sleep(500);
	CString tempDbgStr;
	tempDbgStr.Format("ArmStatus=%d\n",ArmStatus);
	OutputDebugString(tempDbgStr);
	if(Comm_Conn())
	{
		ArmStatus=0;
		if(Comm_Init(IP,Port))
		{
			ArmFlag=TRUE;
			return 1;
		}
		if(Comm_Conn())
		{
			ArmFlag=TRUE;
			return 1;
		}
	}
	i=0;
	while(Comm_recData())
	{
		i++;
		ArmStatus=0;
		if(i>2)
		{
			Comm_Disconn();
			ArmFlag=TRUE;
			return 1;
		}
	}
	if(transBuff2Data()==0)
	{
		ArmDataFlag=false;
		for(i=0;i<fN;i++)
		{
			fData[i]=1.0*recData[i];
		}
		TestSensor();
		Comm_Disconn();
		ArmFlag=FALSE;
		ArmStatus=0;
		return 0;
	}
	else
	{
		ArmDataFlag=TRUE;
		for(i=0;i<fN;i++)
		{
			fData[i]=0;
		}
		ArmStatus=0;
		ArmFlag=TRUE;
		Comm_Disconn();
		return 1;
	}
	ArmFlag=TRUE;
	return 1;
}