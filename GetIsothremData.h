#pragma once
#include "commonFile.h"
//获取画等温线的数据
class CGetIsothremData
{
public:
	CGetIsothremData(void);
	~CGetIsothremData(void);
	BOOL ReadIsothermData(float**& pBuffer,GridBaiscInfo* gridInfo);
	
private:
public:
	// 保存网格数据
	BOOL SaveIsothermData(float** pBuff, GridBaiscInfo* gridInfo);
};

