#pragma once

//复数结构体
typedef struct _Conj
{
	float* real;  //实部
	float* imag;//虚部
}Conj;


class CXcorr
{
public:
	CXcorr(void);
	~CXcorr(void);
	// fft逆变换
	void ifft(float* xArray, int length);
	// fft变换
	void fft(float* xArray, int length);
	void fft2(float *Real, float *Imag, int Ncounter);
	// 互相关函数
	void xcorr(float* xArray, float* yArray, int xLen, int yLen, int length);
	// 保存互相关计算后的数组
	float* m_xcorrArray;
	// 找到相关性最大值时的点
	int FindPointOfMaxvalue(float* xArray, float*yArray, int xLen, int yLen, float& maxValue);

	void swap(float& a, float& b)
	{
		float t;
		t = a;
		a = b;
		b = t;
	}
	void bitrp(float* xreal, float* ximag, int n);
	void ifft3(float* xreal, float* ximag, int n);
	void fft3(float* xreal, float* ximag, int n);
	void xcorr2(float* xArrayReal, float* yArrayReal, int xLen, int yLen, int length);
};

