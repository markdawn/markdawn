#pragma once
#ifndef INTERPOLATE_H
#define INTERPOLATE_H
#include "alcommon.h"
class interpolate
{
private:
//	static const int OutOfPointMax=100;
//	static const int InOfPointMax=40;
//	static const int M=8;
public:
    interpolate();
    virtual ~interpolate();
    double xi[InOfPointMax],yi[InOfPointMax];
    int bnin,bnc;
	double SideLineT[IM];
    double sin[OutOfPointMax],yout[OutOfPointMax];    // yout【】数组为计算结果存储数据
    int  init(double fx[],double fy[],int Iin);
    int  interp(int Iin);
protected:
private:
    int nin,nc;
    double b[OutOfPointMax],d[OutOfPointMax],c[OutOfPointMax];
    double cal_xi[InOfPointMax],cal_yi[InOfPointMax];
    double cal_xib[InOfPointMax],cal_yib[InOfPointMax];
    double cal_sin[OutOfPointMax],cal_yout[OutOfPointMax];
    double cal_sinb[OutOfPointMax],cal_youtb[OutOfPointMax];
    int flag,last;
    int SPL();
    double seval(double u);
    int spline(int end1,int end2,double slope1,double slope2);
};
#endif // INTERPOLATE_H
