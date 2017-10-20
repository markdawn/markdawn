#pragma once
#ifndef EQATION_H
#define EQATION_H
#endif
#include "alcommon.h"
class eqation
{
public:
    eqation();
    ~eqation();
    double X[N2];    //得到解
    int Itk;          //得到迭代次数
protected:
    double EPS;
    int Iteration_max;
    int Eqation_N;
    double norm_inf(double xz[]);
    int n;
public:
	double a1[N2][N2],g[N2];
    int init(double dEPS,int IMax,int EN);
    int Setag(double** a,double* dg);
    int sater();
    bool myNewT(int n1);
//	bool MyNewT8(int n1);
private:
};

