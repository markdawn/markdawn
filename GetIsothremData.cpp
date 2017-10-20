#include "StdAfx.h"
#include "GetIsothremData.h"
#pragma warning(disable:4996)
CGetIsothremData::CGetIsothremData(void)
{
	
}


CGetIsothremData::~CGetIsothremData(void)
{

}

//读取上次保存的网格数据
BOOL CGetIsothremData::ReadIsothermData(float** &pBuffer,GridBaiscInfo* gridInfo)
{
	CString pathName;
	GetModuleFileName(NULL,pathName.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	pathName.ReleaseBuffer();
	int m_iPosIndex;
	m_iPosIndex = pathName.ReverseFind('\\');
	pathName = pathName.Left(m_iPosIndex);
	pathName = pathName+_T("\\Include\\SimIsothermData.txt");
	FILE* fp=fopen( (LPSTR)(LPCTSTR)pathName,"rt");

	if(!fp)
	{
		CString strTmp;
		strTmp.Format(_T("Open %s error!!"),pathName);
		AfxMessageBox(strTmp);
		return FALSE;
	}
	/////////////////////////////////////////////////

	char head[4];
	//ASCII SURF grid file
	fread( head, sizeof( char ), 4, fp );
	if(!(head[0]=='T' && head[1]=='E' && head[2]=='S' && head[3]=='T'))
	{
		AfxMessageBox(_T("不能打开文件"));
		fclose(fp);
		return FALSE;
	}

	//读文件头数据
	UINT cols = 0;
	UINT rows = 0;
	float width,hight,vMin,vMax;

	fscanf(fp,"%d%d%f%f%f%f",&cols,&rows,
		&width,&hight,
		&vMin,&vMax);

	gridInfo->col = cols;
	gridInfo->row = rows;
	gridInfo->width = width;
	gridInfo->hight = hight;
	gridInfo->maxValue = vMax;
	gridInfo->minValue = vMin;

	///////////////////////////////////////
	//读网格数据
	ASSERT( pBuffer!=NULL );
	for(UINT j=0; j<gridInfo->row;j++)
	{
		//making the map looks like in "SURF", so changing the order of data reading
		for(UINT i=0;i<gridInfo->col;i++)
			fscanf(fp,"%f",&pBuffer[j][i]);
	}
	fclose(fp);

	return TRUE;
}



// 保存网格数据
BOOL CGetIsothremData::SaveIsothermData(float** pBuff, GridBaiscInfo* gridInfo)
{
	FILE      *stream; 
	int       i,j;   
	CString   str; 

	CString pathName;
	GetModuleFileName(NULL, pathName.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	pathName.ReleaseBuffer();
	int m_iPosIndex;
	m_iPosIndex = pathName.ReverseFind('\\');
	pathName = pathName.Left(m_iPosIndex);
	pathName = pathName + _T("\\Include\\SimIsothermData.txt");
	//FILE* fp = fopen((LPSTR)(LPCTSTR)pathName, "w+");
	//if((stream=fopen("\\Include\\SimIsothermData.txt ", "w+ ")) != NULL) 

	float maxT = gridInfo->maxValue;
	float minT = gridInfo->minValue;
	int cols = gridInfo->col;
	int rows = gridInfo->row;
	int width = gridInfo->width;
	int hight = gridInfo->hight;
	if ((stream = fopen((LPSTR)(LPCTSTR)pathName, "w+ ")) != NULL)
	{ 
		fprintf(stream, "TEST\n"); 
		fprintf(stream, "%d   %d \n",cols, rows); 
		fprintf(stream, "%d   %d \n",width, hight); 
		fprintf(stream, "%d   %d \n",minT, maxT); 

		for(i=0;i<=30;i++) 
		{
			for(j = 0;j<=50;j++)
			{
				fprintf(stream, "%f    ", pBuff[i][j]); 
			}
			fprintf(stream, "\n "); 
		}
		fclose(stream); 
	}
	return TRUE;
}
