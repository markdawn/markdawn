#pragma once
#ifndef CMat_H
#define CMat_H
#endif
#include "alcommon.h"

//#define N_MAT 100
class Mat
{
public:
	Mat(void);
	~Mat(void);
	void Seta(float t[][M8],int n,int m);
	void Setb(float t[][M8],int n,int m);
	void Getc(float t[][M8],int n,int m);
	void mul(int n,int m);
	void mul(float x[][M8],float y[][M8],float z[][M8],int n,int m);
	void T(float x[][M8],float z[][M8],int n,int m);
	void T(int n,int m);
	void inv(int n);
	void inv(float x[][M8],float z[][M8],int n,int m);
	float comput_D(int n);
	float Creat_M(float *p,int n,int m,int k);
	bool Gauss(int n);
	float MatDet(float *p, int n);
protected:
	float a[N_MAT][N_MAT],b[N_MAT][N_MAT],c[N_MAT][N_MAT];
	void clearc();
private:
};

