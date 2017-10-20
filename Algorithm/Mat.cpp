#include "StdAfx.h"
#include "Mat.h"
#include <math.h>

Mat::Mat(void)
{
}


Mat::~Mat(void)
{
}


void Mat::mul(int n,int m)
{
    int i,j,k;
    int maxt;
    if(n>m)
        maxt=n;
    else
        maxt=m;
    clearc();
    for(i=0;i<maxt;i++)
    {
        for(j=0;j<maxt;j++)
        {
            c[i][j]=0;
            for(k=0;k<maxt;k++)
            {
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
}

void Mat::mul(float x[][M8],float y[][M8],float z[][M8],int n,int m)
{
    Seta(x,n,m);
    Setb(y,n,m);
    mul(n,m);
    Getc(z,n,m);
}

void Mat::T(int n,int m)
{
    int i,j;
    clearc();
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            c[j][i]=a[i][j];
        }
    }
}

void Mat::T(float x[][M8],float z[][M8],int n,int m)
{
    Seta(x,n,m);
    T(n,m);
    Getc(z,n,m);
}
void Mat::inv(int n)
{
    Gauss(n);
}

void Mat::inv(float x[][M8],float z[][M8],int n,int m)
{
    Seta(x,n,m);
    inv(n);
    Getc(z,n,m);
}
/**********************************
**功能：求矩阵nxn的行列式**********
**入口：矩阵首地址p，矩阵行数n*****
**返回：矩阵行列式值***************
**********************************/
float Mat::comput_D(int n)
{
    int i,j,m=0;
    int loop=0;
    float result=0;
    float mid=1;

    if(n!=1)
    {
        loop=(n==2)?1:n;
        for(i=0,j=m;i<n;i++,j++)
        {
            result+=mid;
        }
        for(m=0;m<loop;m++)
        {
            mid=1;
            for(i=0,j=n-1-m+n;i<n;i++,j--)
            {
                mid=mid*a[i][j%n];
            }
            result-=mid;
        }
    }
    else
    {
        result=a[0][0];
    }
    return result;
}

/***********************************************
//功能: 求矩阵(n*n)的行列式
//入口参数: 矩阵的首地址，矩阵的行数
//返回值: 矩阵的行列式值
************************************************/
float Mat::MatDet(float *p, int n)
{
    int r, c, m;
    int lop = 0;
    float result = 0;
    float mid = 1;

    if (n != 1)
    {
        lop = (n == 2) ? 1 : n;            //控制求和循环次数,若为2阶，则循环1次，否则为n次
        for (m = 0; m < lop; m++)
        {
            mid = 1;            //顺序求和, 主对角线元素相乘之和
            for (r = 0, c = m; r < n; r++, c++)
            {
                mid = mid * (*(p+r*n+c%n));
            }
            result += mid;
        }
        for (m = 0; m < lop; m++)
        {
            mid = 1;            //逆序相减, 减去次对角线元素乘积
            for (r = 0, c = n-1-m+n; r < n; r++, c--)
            {
                mid = mid * (*(p+r*n+c%n));
            }
            result -= mid;
        }
    }
    else
        result = *p;
    return result;
}

/***********************************************
*功能：求kxk矩阵中元素A（nm）的代数余子式*******
*入口：kxk矩阵首地址，元素A的下标n,m；矩阵行数k
*返回：kxk矩阵中元素A（nm）的代数余子式*********
************************************************/
float Mat::Creat_M(float *p,int n,int m,int k)
{
    int len;
    int i,j;
    float mid_result=0;
    int sign=1;
    float *p_creat,*p_mid;

    len=(k-1)*(k-1);
    p_creat=(float*)calloc(len,sizeof(float));
    p_mid=p_creat;
    for(i=0;i<k;i++)
    {
        for(j=0;j<k;j++)
        {
            if(i!=m&&j!=n)
            {
                *p_mid++=*(p+i*k+j);
            }
        }
    }
    sign=(m+n)%2==0?1:-1;
    mid_result=(float)sign*MatDet(p_creat,k-1);
    free(p_creat);
    return mid_result;
}
/***********************************************
*功能：高斯法求逆矩阵***************************
*入口：nxn矩阵，矩阵行数n***********************
*返回：是否有逆矩阵（false 无逆矩阵）***********
************************************************/
bool Mat::Gauss(int n)
{
    int i,j,k;
    float max,temp;
    clearc();
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            c[i][j]=(i==j)?(float)1:0;
        }
    }
    for(i=0;i<n;i++)
    {
        max=a[i][i];
        k=i;
        for(j=i+1;j<n;j++)
        {
            if(fabs(a[j][i])>fabs(max))
            {
                max=a[j][i];
                k=j;
            }
        }

        if(k!=i)
        {
            for(j=0;j<n;j++)
            {
                temp=a[i][j];
                a[i][j]=a[k][j];
                a[k][j]=temp;
                temp=c[i][j];
                c[i][j]=c[k][j];
                c[k][j]=temp;
            }
        }
        if(a[i][i]==0)
        {
            return false;
        }

        temp=a[i][i];
        for(j=0;j<n;j++)
        {
            a[i][j]=a[i][j]/temp;
            c[i][j]=c[i][j]/temp;
        }
        for(j=0;j<n;j++)
        {
            if(j!=i)
            {
                temp=a[j][i];
                for(k=0;k<n;k++)
                {
                    a[j][k]=a[j][k]-a[i][k]*temp;
                    c[j][k]=c[j][k]-c[i][k]*temp;
                }
            }
        }
    }
    return true;
}

void Mat::Seta(float p[][M8],int n,int m)
{
    int i,j;
    int maxt;
    if(n>m)
        maxt=n;
    else
        maxt=m;
    for(i=0;i<maxt;i++)
    {
        for(j=0;j<maxt;j++)
        {
            a[i][j]=p[i][j];
        }
    }
}

void Mat::Setb(float p[][M8],int n,int m)
{
    int i,j;
    int maxt;
    if(n>m)
        maxt=n;
    else
        maxt=m;
    for(i=0;i<maxt;i++)
    {
        for(j=0;j<maxt;j++)
        {
            b[i][j]=p[i][j];
        }
    }
}

void Mat::Getc(float p[][M8],int n,int m)
{
    int i,j;
    if(n<1||n>24)
        return;
    if(m<1||m>24)
        return;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            p[i][j]=c[i][j];
        }
    }
}

void Mat::clearc()
{
    int i,j;
    for(i=0;i<N_MAT;i++)
    {
        for(j=0;j<N_MAT;j++)
        {
            c[j][i]=0;
        }
    }
}
