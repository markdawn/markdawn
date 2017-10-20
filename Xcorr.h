#pragma once

//�����ṹ��
typedef struct _Conj
{
	float* real;  //ʵ��
	float* imag;//�鲿
}Conj;


class CXcorr
{
public:
	CXcorr(void);
	~CXcorr(void);
	// fft��任
	void ifft(float* xArray, int length);
	// fft�任
	void fft(float* xArray, int length);
	void fft2(float *Real, float *Imag, int Ncounter);
	// ����غ���
	void xcorr(float* xArray, float* yArray, int xLen, int yLen, int length);
	// ���滥��ؼ���������
	float* m_xcorrArray;
	// �ҵ���������ֵʱ�ĵ�
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

