#include "StdAfx.h"
#include "DatabaseOperate.h"


CDatabaseOperate::CDatabaseOperate(void)
{
	
}


CDatabaseOperate::~CDatabaseOperate(void)
{

}


// дMysql���ݿ�,-1 --- д����
int CDatabaseOperate::WriteMysql(MYSQL* mysql,char* cSql,UINT lenth)
{
	if(mysql_real_query(mysql,cSql,lenth))
	{ 
		return -1;
	}
	return 0;
}


// ��ʼ�����ݿ�
BOOL CDatabaseOperate::InitMysql(MYSQL* mysql)
{
	if (mysql_init (mysql)==NULL)
	{
		//��ʼ��ʧ��
		return FALSE;
	}

	return TRUE;
}

// �������ݿ�
BOOL CDatabaseOperate::ConnectMysql(MYSQL* mysql, char * user, char * password,char* dbName,int port)
{
	if(!mysql_real_connect(mysql,_T("localhost"),user,password,dbName,port,NULL,0))
	{ 
		return FALSE;
	}
	return TRUE;
}
