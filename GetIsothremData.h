#pragma once
#include "commonFile.h"
//��ȡ�������ߵ�����
class CGetIsothremData
{
public:
	CGetIsothremData(void);
	~CGetIsothremData(void);
	BOOL ReadIsothermData(float**& pBuffer,GridBaiscInfo* gridInfo);
	
private:
public:
	// ������������
	BOOL SaveIsothermData(float** pBuff, GridBaiscInfo* gridInfo);
};

