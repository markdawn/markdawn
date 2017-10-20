#pragma once
#include "mysql.h"
class CDatabaseOperate
{
public:
	CDatabaseOperate(void);
	~CDatabaseOperate(void);
	// 写Mysql数据库
	int WriteMysql(MYSQL* mysql,char* cSql,UINT lenth);
protected:
	// myqsl
//	MYSQL m_mysql;
public:
	// 初始化数据库
	BOOL InitMysql(MYSQL* mysql);
	// 连接数据库
	BOOL ConnectMysql(MYSQL* mysql, char * user, char * password,char* dbName,int port);
};

