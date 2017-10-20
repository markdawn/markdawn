#include "stdafx.h"
#include "eqation.h"
#include <math.h>

eqation::eqation()
{
    //ctor
    EPS=0.0001;
    Iteration_max=100;
    Eqation_N=10;
    n=5;
	int i;
	for(i=0;i<N2;i++)
	{
		X[i]=0;
	}
}

eqation::~eqation()
{
    //dtor
}
int eqation::init(double dEPS,int IMax,int EN)
{
    EPS=dEPS;
    Iteration_max=IMax;
    Eqation_N=EN;
    n=EN;
    return 1;
}
int eqation::Setag(double** a,double* dg)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            a1[i][j]=a[i][j];
        }
        g[i]=dg[i];
    }
    return 1;
}
double eqation::norm_inf(double xz[])
{
    double norm;
    int i;
    norm=fabs(xz[0]);
    for(i=1;i<n;i++)
    {
        if(fabs(xz[i])>norm)
            norm=fabs(xz[i]);
    }
    return(norm);
}

int eqation::sater()
{
       //b表示迭代矩阵，x1_x0表示迭代前后向量x0与x1之差向量
	double b[N2][N2]={0},x0[N2]={0},x1[N2]={0},x1_x0[N2]={0},norm,temp;
    int i, j,k;
    for(i=0; i<n;i++)
    {
        g[i]=g[i]/a1[i][i];
        for(j=0;j<n;j++)
        {
            if(i==j)
                continue;
            b[i][j]=-a1[i][j]/a1[i][i];
        }
    }
    for(i=0;i<n;i++)       //选取初始向量并构成迭代条件
    {
        x0[i]=0;
        x1[i]=1;
        x1_x0[i]=x1[i]-x0[i];
    }
    k=0;
    norm=norm_inf(x1_x0);        //求差向量的无穷范数
    while((norm>=EPS)&&(k<Iteration_max))  //开始迭代
    {
        for(i=0;i<n;i++)
            x0[i]=x1[i];
        for(i=0;i<n;i++)
        {
            temp=0;
            for(j=0;j<=i-1;j++)
            {
                temp=temp+b[i][j]*x1[j];
            }
            for(j=i+1;j<n;j++)
            {
                temp=temp+b[i][j]*x0[j];
            }
            x1[i]=temp+g[i];
            x1_x0[i]=x1[i]-x0[i];
        }
        norm=norm_inf(x1_x0);            //求差向量的无穷范数
        k++;                            //累加迭代次数
    }
    for(i=0;i<n;i++)
    {
        X[i]=x1[i];
    }
    Itk=k;
    if(Itk==Iteration_max)      //无解
    {
        for(i=0;i<n;i++)
        {
            X[i]=0.0;
        }
    }
    return 1;
}
//void eqation::myNewT(int n1)
//{
//    int i,j,m,k,t;
//	if(n1>N2)
//	{
//		return;
//	}
//    float b[200]={0};
//    k=0;
//    for(t=0;t<n1-1;t++)
//    {
//        for(m=0;m<n1;m++)
//        {
//            for(j=0;j<n1;j++)
//            {
//                if(j!=m)
//                {
//                    b[k]=a1[j][m]/a1[m][m];
//                    for(i=0;i<n1+1;i++)
//                    {
//                        a1[j][i]-=a1[m][i]*b[k];
//                    }
//                    b[k]=0;
//                    k++;
//                }
//            }
//        }
//    }
//    for(j=0;j<n1;j++)
//    {
//        a1[j][n1]/=a1[j][j];
//        a1[j][j]/=a1[j][j];
//    }
//    for(j=0;j<n1;j++)
//    {
//        X[j]=a1[j][n1];
//    }
//}

bool eqation::myNewT(int n1)
{
	int i,j,m;
	bool FlagCalc;
	FlagCalc=true;
	for(j=0;j<n1;j++)
	{
		double maxc=0;
		double imax=0;
		for(i=j;i<n1;i++)
		{
			if(imax<fabs(a1[i][j]))
			{
				imax=fabs(a1[i][j]);
				maxc=a1[i][j];
				m=i;
			}
		}
		if(maxc+1.0==1.0)
		{
			FlagCalc=false;
		}
		if(fabs(a1[j][j])!=maxc)
		{
			double b=0;
			for(int k=j;k<n1+1;k++)
			{
				b=a1[j][k];
				a1[j][k]=a1[m][k];
				a1[m][k]=b;
			}
		}
		for(int r=j;r<n1+1;r++)
		{
			a1[j][r]=a1[j][r]/maxc;
		}
		for(i=j+1;i<n1;i++)
		{
			double c=a1[i][j];
			if(c==0)
			{
				continue;
			}
			for(int s=j;s<n1+1;s++)
			{
				double tempdata=a1[i][s];
				a1[i][s]=a1[i][s]-a1[j][s]*c;
			}
		}
	}
	if(fabs(a1[n1-1][n1-1])+1.0==1.0)
	{	
		FlagCalc=false;
	}
	if(FlagCalc)
	{
		for(i=n1-2;i>=0;i--)
		{
			for(j=i+1;j<n1;j++)
			{
				double tempData=a1[i][j];
				double data1=a1[i][n1];
				double data2=a1[j][n1];
				a1[i][n1]=a1[i][n1]-a1[j][n1]*a1[i][j];
			}
		}
		for(int k=0;k<n1;k++)
		{
			X[k]=a1[k][n1];
		}
		return true;
	}
	else
	{
		for(int k=0;k<n1;k++)
		{
			X[k]=0;
		}
		return false;
	}
}
