#include "StdAfx.h"
#include <math.h>
#include "interpolate.h"
#pragma warning(disable:4244)
interpolate::interpolate()
{
}

interpolate::~interpolate()
{
}

int interpolate::spline(int end1,int end2,double slope1,double slope2)
{
    int nm1,ib,i,ascend;
    double t;
    nm1=nin-1;
    flag=0;
    if(nin<2)
    {
    //ino possible interpolation
        flag=1;
        return 0;
    }
    ascend=1;
    for(i=1;i<nin;++i)
    {
        if(cal_xi[i]<cal_xi[i-1])
        {
            ascend=0;
        }
    }
    if(!ascend)
    {
        flag=2;
        return 0;
    }
    if(nin>=3)
    {
        d[0]=cal_xi[1]-cal_xi[0];
        c[1]=(cal_yi[1]-cal_yi[0])/d[0];
        for(i=1;i<nm1;++i)
        {
            d[i]=cal_xi[i+1]-cal_xi[i];
            b[i]=2.0*(d[i-1]+d[i]);
            c[i+1]=(cal_yi[i+1]-cal_yi[i])/d[i];
            c[i]=c[i+1]-c[i];
        }
        //default end conditions
        b[0]=-d[0];
        b[nm1]=-d[nin-2];
        c[0]=0.0;
        c[nm1]=0.0;
        if(nin!=3)
        {
            c[0]=c[2]/(cal_xi[3]-cal_xi[1])-c[1]/(cal_xi[2]-cal_xi[0]);
            c[nm1]=c[nin-2]/(cal_xi[nm1]-cal_xi[nin-3])-c[nin-3]/(cal_xi[nin-2]-cal_xi[nin-4]);
            c[0]=c[0]*d[0]*d[0]/(cal_xi[3]-cal_xi[0]);
            c[nm1]=-c[nm1]*d[nin-2]*d[nin-2]/(cal_xi[nm1]-cal_xi[nin-4]);
        }
        //Alternative end conditions-known slopes
        if(end1==1)
        {
            b[0]=2.0*(cal_xi[1]-cal_xi[0]);
            c[0]=(cal_yi[1]-cal_yi[0])/(cal_xi[1]-cal_xi[0])-slope1;
        }
        if(end2==1)
        {
            b[nm1]=2.0*(cal_xi[nm1]-cal_xi[nin-2]);
            c[nm1]=slope2-(cal_yi[nm1]-cal_yi[nin-2])/(cal_xi[nm1]-cal_xi[nin-2]);
        }
        //forward elimination
        for(i=1;i<nin;++i)
        {
            t=d[i-1]/b[i-1];
            b[i]=b[i]-t*d[i-1];
            c[i]=c[i]-t*c[i-1];
        }
        //back substitution
        c[nm1]=c[nm1]/b[nm1];
        for(ib=0;ib<nm1;++ib)
        {
            i=nin-ib-2;
            c[i]=(c[i]-d[i]*c[i+1])/b[i];
        }
        b[nm1]=(cal_yi[nm1]-cal_yi[nin-2])/d[nin-2]+d[nin-2]*(c[nin-2]+2.0*c[nm1]);
        for(i=0;i<nm1;++i)
        {
            b[i]=(cal_yi[i+1]-cal_yi[i])/d[i]-d[i]*(c[i+1]+2.0*c[i]);
            d[i]=(c[i+1]-c[i])/d[i];
            c[i]=3.0*c[i];
        }
        c[nm1]=3.0*c[nm1];
        d[nm1]=d[nin-2];
    }
    else
    {
        b[0]=(cal_yi[1]-cal_yi[0])/(cal_xi[1]-cal_xi[0]);
        c[0]=0.0;
        d[0]=0.0;
        b[1]=b[0];
        c[1]=0.0;
        d[1]=0.0;
    }
    return 0;
}

double interpolate::seval(double u)
{
    int i,j,k;
    double w;
    i=last;
    if(i>=nc-1) i=0;
    if(i<0) i=0;
    if((cal_xi[i]>u)||(cal_xi[i+1]<u))
    {
        i=0;
        j=nc;
        do
        {
            k=(i+j)/2;
            if(u<cal_xi[k]) j=k;
            if(u>=cal_xi[k]) i=k;
        }while(j>i+1);
    }
    last=i;
    w=u-cal_xi[i];
    w=yi[i]+w*(b[i]+w*(c[i]+w*d[i]));
    return (w);
}

int interpolate::SPL()
{
    int i;
    for(i=0;i<nin;i++)
    {
        cal_xi[i]=xi[i];
        cal_yi[i]=yi[i];
    }
    for(i=0;i<nc;i++)
    {
        cal_sin[i]=sin[i];
    }
    if(!d)
    {
//      printf("No enough memory for b,c,d\n");
        return -1;
    }
    else
    {
        spline(0,0,0,0);
        if(flag==0)
        {
            for (i=0;i<nc;i++)
            {
                cal_yout[i]=seval(cal_sin[i]);
            }
            return 0;
            //printf("I got coef b,c,d now\n");
        }
        else
        {
            //printf("x not in order or other error\n");
            return -1;
        }
    }
}

int interpolate::init(double fx[],double fy[],int Iin)
{
    int i;
    flag=0;
    last=0;
    for(i=0;i<InOfPointMax;i++)
    {
        xi[i]=0;
        yi[i]=0;
    }
    for(i=0;i<OutOfPointMax;i++)
    {
        sin[i]=0;
        yout[i]=1.0;
        b[i]=0;
        c[i]=0;
        d[i]=0;
    }
    if(Iin>InOfPointMax )
    {
        return -1;
    }
    bnin=Iin;
    nin=bnin;
    for(i=0;i<nin;i++)
    {
        xi[i]=fx[i];
        yi[i]=fy[i];
    }
    return 0;
}

int interpolate::interp(int Iin)
{
    int i,n,m,mode;
    if(Iin>OutOfPointMax)
    {
        return -1;
    }
    bnc=Iin;
    nin=bnin;
    n=nin-1;
    nc=n;
    for(i=0;i<n;i++)
    {
        sin[i]=ceil((xi[i]+xi[i+1])/2.0);
    }
    i=SPL();
    for(i=0;i<nin;i++)
    {
        cal_xib[2*i]=cal_xi[i];
        cal_yib[2*i]=cal_yi[i];
    }
    for(i=0;i<n;i++)
    {
        cal_xib[2*i+1]=cal_sin[i];
        cal_yib[2*i+1]=cal_yout[i];
    }
    for(i=0;i<n+nin;i++)
    {
        xi[i]=cal_xib[i];
        yi[i]=cal_yib[i];
        cal_youtb[int(xi[i])]=cal_yib[i];
    }
    nin=n+nin;
    nc=n;
    nc=nin-1;
    n=nc;
    mode=xi[1]-xi[0];
    for(m=0;m<(bnc-nin)/nc;m++)
    {
        for(i=0;i<n;i++)
        {
            sin[i]=i*mode+1+m;
        }
        SPL();
        for(i=0;i<nin;i++)
        {
            cal_xib[n*i/2]=cal_xi[i];
            cal_yib[n*i/2]=cal_yi[i];
        }
        for(i=0;i<n;i++)
        {
            cal_xib[n*i/2+1]=cal_sin[i];
            cal_yib[n*i/2+1]=cal_yout[i];
            cal_youtb[int(cal_sin[i])]=cal_yout[i];
        }
    }
    for(i=0;i<bnc;i++)
    {
        yout[i]=cal_youtb[i];
    }
    if(i<0)
        return i;
    return 0;
}



