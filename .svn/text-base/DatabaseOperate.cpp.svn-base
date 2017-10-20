#include "StdAfx.h"
#include "DatabaseOperate.h"


CDatabaseOperate::CDatabaseOperate(void)
{
	
}


CDatabaseOperate::~CDatabaseOperate(void)
{

}


// 写Mysql数据库,-1 --- 写错误
int CDatabaseOperate::WriteMysql(MYSQL* mysql,char* cSql,UINT lenth)
{
	if(mysql_real_query(mysql,cSql,lenth))
	{ 
		return -1;
	}
	return 0;
}


// 初始化数据库
BOOL CDatabaseOperate::InitMysql(MYSQL* mysql)
{
	if (mysql_init (mysql)==NULL)
	{
		//初始化失败
		return FALSE;
	}

	return TRUE;
}

// 连接数据库
BOOL CDatabaseOperate::ConnectMysql(MYSQL* mysql, char * user, char * password,char* dbName,int port)
{
	if(!mysql_real_connect(mysql,_T("localhost"),user,password,dbName,port,NULL,0))
	{ 
		return FALSE;
	}
	return TRUE;
}
