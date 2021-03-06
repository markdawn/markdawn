#include "StdAfx.h"
#include "Xcorr.h"
#include <math.h>
#include "commonFile.h"

CXcorr::CXcorr(void)
	: m_xcorrArray(NULL)
{
	m_xcorrArray = new float[SAMPLE_DATA_NUM];
	memset(m_xcorrArray,0.0,SAMPLE_DATA_NUM*sizeof(float));
}


CXcorr::~CXcorr(void)
{
	if (m_xcorrArray!=NULL)
	{
		delete [] m_xcorrArray;
	}
	m_xcorrArray=NULL;
}


// fft逆变换
void CXcorr::ifft(float* xArray, int length)
{
	int i=0,j=0,k=0,m=0;
	int i1=0,i2=0,i3=0,i4=0,i5=0,i6=0,i7=0,i8=0;
	int n2=0,n4=0,n8=0;
	int id=0,is=0;

	float a=0.0,e=0.0,a3=0.0;
	float t1=0.0,t2=0.0,t3=0.0,t4=0.0,t5=0.0;
	float cc1=0.0,cc3=0.0;
	float ss1=0.0,ss3=0.0;

	for(j=1,i=1;i<16;i++)
	{
		m=i;
		j=2*j;
		if(j==length)
			break;
	}
	n2=2*length;
	for(k=1;k<m;k++)
	{
		is=0;
		id=n2;
		n2=n2/2;
		n4=n2/4;
		n8=n4/2;
		e=6.28318530718/n2;

		do
		{
			for(i=is;i<length;i+=id)
			{
				i1=i;
				i2=i1+n4;
				i3=i2+n4;
				i4=i3+n4;
				t1=xArray[i1]-xArray[i3];
				xArray[i1]=xArray[i1]+xArray[i3];
				xArray[i2]=2*xArray[i2];
				xArray[i3]=t1-2*xArray[i4];
				xArray[i4]=t1+2*xArray[i4];

				if(n4==1)
					continue;

				i1 += n8;
				i2 += n8;
				i3 += n8;
				i4 += n8;
				t1=(xArray[i2]-xArray[i1])/sqrt(2.0);
				t2=(xArray[i4]-xArray[i3])/sqrt(2.0);

				xArray[i1]=xArray[i1]+xArray[i2];
				xArray[i2]=xArray[i4]-xArray[i3];
				xArray[i3]=2*(-t2-t1);
				xArray[i4]=2*(-t2+t1);
			}
			is=2*id-n2;
			id=4*id;
		}while(is<(length-1));
		a=e;
		for(j=1;j<n8;j++)
		{
			a3=3*a;
			cc1=cos(a);
			ss1=sin(a);
			cc3=cos(a3);
			ss3=sin(a3);
			a=(j+1)*e;
			is=0;
			id=2*n2;

			do
			{
				for(i=is;i<=(length-1);i=i+id)
				{
					i1=i-j;
					i2=i1+n4;
					i3=i2+n4;
					i4=i3+n4;
					i5=i+n4-j;
					i6=i5+n4;
					i7=i6-n4;
					i8=i7+n4;

					t1=xArray[i1]-xArray[i6];
					xArray[i1]=xArray[i1]+xArray[i6];
					t2=xArray[i5]-xArray[i2];
					xArray[i5]=xArray[i2]+xArray[i5];
					t3=xArray[i8]-xArray[i3];
					xArray[i6]=xArray[i8]-xArray[i3];
					t4=xArray[i4]+xArray[i7];
					xArray[i2]=xArray[i4]-xArray[i7];
					t5=t1-t4;
					t1=t1+t4;
					t4=t2-t3;
					t2=t2+t3;
					xArray[i3]=t5*cc1+t4*ss1;
					xArray[i7]=-t4*cc1+t5*ss1;
					xArray[i4]=t1*cc3-t2*ss3;
					xArray[i8]=t2*cc3+t1*ss3;
				}
				is=2*id-n2;
				id=4*id;
			}while(is<(length-1));

		}
	}
	is=0;
	id=4;
	do
	{
		for(i=is;i<length;i=i+id)
		{
			i1=i+1;
			t1=xArray[i];
			xArray[i]=t1+xArray[i1];
			xArray[i1]=t1-xArray[i1];

		}
		is=2*id-2;
		id=4*id;

	}while(is<(length-1));

	for(j=0,i=0;i<(length-1);i++)
	{
		if(i<j)
		{
			t1=xArray[j];
			xArray[j]=xArray[i];
			xArray[i]=t1;		
		}
		k=length/2;
		while(k<(j+1))
		{
			j=j-k;
			k=k/2;
		}
		j=j+k;
	}
	for(i=0;i<length;i++)
		xArray[i]=xArray[i]/length;
}


// fft变换
void CXcorr::fft(float* xArray, int length)
{
	int i=0,j=0,k=0,m=0;
	int i1=0,i2=0,i3=0,i4=0;
	int n1=0,n2=0,n4=0;
	float a=0.0,e=0.0,cc=0.0,ss=0.0,xt=0.0,t1=0.0,t2=0.0;
	for(j=1,i=1;i<16;i++)
	{
		m=i;
		j=1<<i;
		if(j==length)
			break;
	}
	n1=length-1;
	for(j=0,i=0;i<n1;i++)
	{
		if(i<j)
		{
			xt=xArray[j];
			xArray[j]=xArray[i];
			xArray[i]=xt;
		}
		k=length/2;
		while(k<(j+1))
		{
			j=j-k;
			k=k/2;
		}
		j=j+k;
	}
	for(i=0;i<length;i+=2)
	{
		xt=xArray[i];
		xArray[i]=xt+xArray[i+1];
		xArray[i+1]=xt-xArray[i+1];
	}
	n2=1;
	for(k=2;k<=m;k++)
	{
		n4=n2;
		n2=2*n4;
		n1=2*n2;
		e=6.28318530718/n1;
		for(i=0;i<length;i+=n1)
		{
			xt=xArray[i];
			xArray[i+n2]=xt-xArray[i+n2];
			xArray[i+n2+n4]=-xArray[i+n2+n4];
			a=e;
			for(j=1;j<=(n4-1);j++)
			{
				i1=i+j;
				i2=i-j+n2;
				i3=i+j+n2;
				i4=i-j+n1;
				cc=cos(a);
				ss=sin(a);
				a=a+e;
				t1=cc*xArray[i3]+ss*xArray[i4];
				t2=ss*xArray[i3]-cc*xArray[i4];
				xArray[i4]=xArray[i2]-t2;
				xArray[i3]=-xArray[i2]-t2;
				xArray[i2]=xArray[i1]-t1;
				xArray[i1]=xArray[i1]+t1;
			}
		}
	}
}


// 互相关函数
void CXcorr::xcorr(float* xArray, float* yArray, int xLen, int yLen, int length)
{
	int i = 0;
	int len2 = 0;
	float t= 0.0;
	float *zArray=NULL;
	zArray = new float[length];
	memset(zArray,0.0,length*sizeof(float));
//	zArray=malloc(length*sizeof(double));
	for(i=xLen;i<length;i++)
	{
		xArray[i]=0.0;
	}
	for(i=0;i<(xLen-1);i++)
	{
		zArray[i]=0.0;
	}
	for(i=(xLen-1);i<=(xLen+yLen-2);i++)
	{
		zArray[i]=yArray[i-xLen+1];
	}
	for(i=(xLen+yLen-1);i<length;i++)
	{
		zArray[i]=0.0;
	}
	fft(xArray,length);
	fft(zArray,length);

	len2=length/2;
	xArray[0]=xArray[0]*zArray[0];
	xArray[len2]=xArray[len2]*zArray[len2];
	for(i=1;i<len2;i++)
	{
		t=xArray[i]*zArray[i]+xArray[length-i]*zArray[length-i];
		xArray[length-i]=xArray[i]*zArray[length-i]-xArray[length-i]*zArray[i];
		xArray[i]=t;
	}
	ifft(xArray,length);
	delete [] zArray;
//	free(zArray);
}


// 找到相关性最大值时的点
int CXcorr::FindPointOfMaxvalue(float* xArray,float*yArray ,int xLen,int yLen,float& maxValue)
{
	int length = 0;
	length = xLen+yLen;
	if (length%2)
	{
		length = 2*SAMPLE_DATA_NUM;
	}
//	xcorr(xArray,yArray,xLen,yLen,length);
	xcorr2(xArray,yArray,xLen,yLen,length);
	memcpy(m_xcorrArray,xArray,xLen*sizeof(float));

	
	float temp = 0.0,temp1=0.0;
	int count = 0;
	int i =0;
	for (i=0;i<SAMPLE_DATA_NUM;i++)
	{
		temp1=abs(m_xcorrArray[i]);
		if (temp1>temp)
		{
			temp = temp1;
			count = i;
		}
	}
	maxValue=temp;
	return (count+1);
}


void CXcorr::fft2(float *Real, float *Imag, int Ncounter)
{

	int i,j,k,LH,M;
	int J,K,L,B,Be,P;
	int FF = Ncounter;
	float Real_Temp,Imag_Temp,W_Real,W_Imag,T_Real,T_Imag;

	for(M=1;(FF=FF/2)!=1;M++){;}

	LH = (int)Ncounter / 2;
	j = LH;

	for (i = 1; i<Ncounter - 2; i++)
	{
		if(i<j)
		{
			Real_Temp = Real[j];
			Imag_Temp = Imag[j];
			Real[j] = Real[i];
			Imag[j] = Imag[i];
			Real[i] = Real_Temp;
			Imag[i] = Imag_Temp;

		}
		k = LH;
		while(j>=k)
		{
			j = j-k;
			k = k/2;
		}
		j = j+k;
	}

	for(L=1;L<=M;L++)
	{
		// Be = (int)pow(2,L);
		Be =1;
		for(i=0;i<L;i++)
		{
			Be = Be*2;
		}
		B = Be/2;

		for(J=0;J<=B-1;J++)
		{
			P = (int)(J*powf(2,M-L));

			W_Real = cos(2 * PI / Ncounter*P);
			W_Imag = -sin(2 * PI / Ncounter*P);

			for (K = J; K <= Ncounter - 1; K = K + Be)
			{
				T_Real = Real[K+B]*W_Real - Imag[K+B]*W_Imag;
				T_Imag = Real[K+B]*W_Imag + Imag[K+B]*W_Real;

				Real[K+B] = Real[K] - T_Real;
				Imag[K+B] = Imag[K] - T_Imag;

				Real[K] = Real[K] + T_Real;
				Imag[K] = Imag[K] + T_Imag;


			}
		}
	}

}

void CXcorr::bitrp(float* xreal, float* ximag, int n)
{
	// 位反转置换 Bit-reversal Permutation
	int i, j, a, b, p;

	for (i = 1, p = 0; i < n; i *= 2)
	{
		p ++;
	}
	for (i = 0; i < n; i ++)
	{
		a = i;
		b = 0;
		for (j = 0; j < p; j ++)
		{
			b = (b << 1) + (a & 1);     // b = b * 2 + a % 2;
			a >>= 1;         // a = a / 2;
		}
		if ( b > i)
		{
			swap (xreal [i], xreal [b]);
			swap (ximag [i], ximag [b]);
		}
	}
}


void CXcorr::ifft3(float* xreal, float* ximag, int n)
{
	// 快速傅立叶逆变换
	float wreal [SAMPLE_DATA_NUM ], wimag [SAMPLE_DATA_NUM ], treal, timag, ureal, uimag, arg;
	int m, k, j, t, index1, index2;

	bitrp (xreal, ximag, n);

	// 计算 1 的前 n / 2 个 n 次方根 Wj = wreal [j] + i * wimag [j] , j = 0, 1,   , n / 2 - 1
	arg = 2 * PI / n;
	treal = cos (arg);
	timag = sin (arg);
	wreal [0] = 1.0;
	wimag [0] = 0.0;
	for (j = 1; j < n / 2; j ++)
	{
		wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
		wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j ++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;     // 旋转因子 w 的实部在 wreal [] 中的下标为 t
				treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
				timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
				ureal = xreal [index1];
				uimag = ximag [index1];
				xreal [index1] = ureal + treal;
				ximag [index1] = uimag + timag;
				xreal [index2] = ureal - treal;
				ximag [index2] = uimag - timag;
			}
		}
	}

	for (j=0; j < n; j ++)
	{
		xreal [j] /= n;
		ximag [j] /= n;
	}
}


void CXcorr::fft3(float* xreal, float* ximag, int n)
{
	// 快速傅立叶变换，将复数 x 变换后仍保存在 x 中，xreal, ximag 分别是 x 的实部和虚部
	float wreal [SAMPLE_DATA_NUM], wimag [SAMPLE_DATA_NUM], treal, timag, ureal, uimag, arg;
	int m, k, j, t, index1, index2;

	bitrp (xreal, ximag, n);

	// 计算 1 的前 n / 2 个 n 次方根的共轭复数 W'j = wreal [j] + i * wimag [j] , j = 0, 1,   , n / 2 - 1
	arg = - 2 * PI / n;
	treal = cos (arg);
	timag = sin (arg);
	wreal [0] = 1.0;
	wimag [0] = 0.0;
	for (j = 1; j < n / 2; j ++)
	{
		wreal [j] = wreal [j - 1] * treal - wimag [j - 1] * timag;
		wimag [j] = wreal [j - 1] * timag + wimag [j - 1] * treal;
	}

	for (m = 2; m <= n; m *= 2)
	{
		for (k = 0; k < n; k += m)
		{
			for (j = 0; j < m / 2; j ++)
			{
				index1 = k + j;
				index2 = index1 + m / 2;
				t = n * j / m;     // 旋转因子 w 的实部在 wreal [] 中的下标为 t
				treal = wreal [t] * xreal [index2] - wimag [t] * ximag [index2];
				timag = wreal [t] * ximag [index2] + wimag [t] * xreal [index2];
				ureal = xreal [index1];
				uimag = ximag [index1];
				xreal [index1] = ureal + treal;
				ximag [index1] = uimag + timag;
				xreal [index2] = ureal - treal;
				ximag [index2] = uimag - timag;
			}
		}
	}
}


//void CXcorr::xcorr2(float* xArrayReal, float* xArrayImag,float* yArrayReal,float* yArrayImag, int xLen, int yLen, int length)
void CXcorr::xcorr2(float* xArrayReal, float* yArrayReal, int xLen, int yLen, int length)
{
	//xArray[2]:xArray[2].real + xArray[2].imag*i;
	int i = 0;
	int len2 = 0;
	float t= 0.0;
	float test[SAMPLE_DATA_NUM*2] = {0.0f};
	float *XXtempR=NULL;//构造长度为length的XX矩阵实部
	XXtempR = new float[length];
	memset(XXtempR,0.0,length*sizeof(float));

	float *XXtempI=NULL;//构造长度为length的XX矩阵虚部
	XXtempI = new float[length];
	memset(XXtempI,0.0,length*sizeof(float));

	float *YYtempR=NULL;//构造长度为length的YY矩阵实部
	YYtempR = new float[length];
	memset(YYtempR,0.0,length*sizeof(float));

	float *YYtempI=NULL;//构造长度为length的YY矩阵虚部
	YYtempI = new float[length];
	memset(YYtempI,0.0,length*sizeof(float));

	//	zArray=malloc(length*sizeof(double));
	
	//构造XX矩阵：XX=x(n)  ,n=0,1,2......xLen-1;
	//                          XX=0,      n=xLen,xLen+1,......,length-1;
	for(i=xLen;i<length;i++)
	{
		XXtempR[i]=0.0;
		XXtempI[i]=0.0;
	}
	for (i=0;i<xLen;i++)
	{
		XXtempR[i]=xArrayReal[i];
		XXtempI[i]=0.0f;
	}

	//构造YY矩阵：YY=0, n=0,1,2,......,xLen-2;
	//                          YY=y(n),n=xLen-1,xLen,......,xLen+yLen-2;
	//                          YY=0,n=xLen+yLen-1,xLen+yLen,.....,Length-1;
	for(i=0;i<(xLen-1);i++)
	{
		YYtempR[i]=0.0;
		YYtempI[i]=0.0;
	}
	for(i=(xLen-1);i<=(xLen+yLen-2);i++)
	{
		YYtempR[i]=yArrayReal[i-xLen+1];
		YYtempI[i]=0.0f;
	}
	for(i=(xLen+yLen-1);i<length;i++)
	{
		YYtempR[i]=0.0;
		YYtempI[i]=0.0;
	}
	fft3(XXtempR,XXtempI,length);
	memcpy(test,XXtempR,length*sizeof(float));
	memcpy(test,XXtempI,length*sizeof(float));
	fft3(YYtempR,YYtempI,length);
	memcpy(test,YYtempR,length*sizeof(float));
	memcpy(test,YYtempI,length*sizeof(float));

	//zz=conj(XX).*YY;
	float tmp1 = 0.0f,tmp2 = 0.0f;
	for(i = 0;i<length;i++)
	{
		tmp1=XXtempR[i]* YYtempR[i]+ XXtempI[i]*YYtempI[i]; //实部
		tmp2 =XXtempR[i]*YYtempI[i]- XXtempI[i]*YYtempR[i];
		XXtempR[i]=tmp1;
		XXtempI[i]=tmp2;
	}
	//len2=length/2;
	//xArray[0]=xArray[0]*zArray[0];
	//xArray[len2]=xArray[len2]*zArray[len2];
	//for(i=1;i<len2;i++)
	//{
	//	t=xArray[i]*zArray[i]+xArray[length-i]*zArray[length-i];
	//	xArray[length-i]=xArray[i]*zArray[length-i]-xArray[length-i]*zArray[i];
	//	xArray[i]=t;
	//}
	memcpy(test,XXtempR,length*sizeof(float));
	memcpy(test,XXtempI,length*sizeof(float));
	ifft3(XXtempR,XXtempI,length);
	memcpy(test,XXtempR,length*sizeof(float));
	memcpy(test,XXtempI,length*sizeof(float));
	int length2 = length>>1;
	for(i = 0;i<length2;i++)
	{
		xArrayReal[i]=XXtempR[i];
	}
	delete [] XXtempR;
	delete [] XXtempI;
	delete [] YYtempR;
	delete [] YYtempI;
}
