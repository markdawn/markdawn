#pragma once
#ifndef CTempField8_H
#define CTempField8_H
#endif

#include "alcommon.h"
#include "mat.h"

class TempField8 :
	public Mat
{
public:
	TempField8(void);
	~TempField8(void);
	float s[M8][M8],t[M8][M8],d[M8][M8];
	float z;
	void lsm();
	void GetT(float tei[],int m1);
	int n,m;
	void Sets(float x[][M8],int n1,int m1);
	void Sett(float x[][M8],int n1,int m1);
	void Sett(float x[],int n1);
	float tt[M8][M8],ti[M8][M8],ts[M8][M8];
};
