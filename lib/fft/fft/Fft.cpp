#include <vector>
#include "Fft.h"
#include <cmath>
#include "Fft_arrays.h"
#include <OhDebug.hpp>

inline void mulComplex(float &re1, float &im1, float re2, float im2)
{
	float tmp = re1;
	re1 = tmp * re2 - im1 * im2;
	im1 = tmp * im2 + im1 * re2;
}


void Fft::init(std::size_t sz, std::size_t st)
{
	step = st;
	m_logN = static_cast<int>(ceilf(log2(static_cast<float>(sz))));
	m_nMax = 1 << m_logN;

	// Rotation multiplier array allocation
	m_wStore_re.resize(m_nMax / 2);
	m_wStore_im.resize(m_nMax / 2);
}

void Fft::init2(std::size_t aRows, std::size_t aCols, std::size_t aStep)
{
	step = aStep;
	rows = aRows;
	cols = aCols;
	innerStride = aStep;
	outerStride = step * cols;
}

/// \pre cols and rows must be initialized w/ values more than 0
///
void Fft::transformDirect2(float *pRe, float *pIm)
{
	ohdebugonce(Fft::transformDirect2, 0, rows, cols, step, outerStride);
	// Fft row-wise
	init(cols, innerStride);
	float *re = pRe;
	float *im = pIm;

	for (auto row = rows; row > 0; --row, re += outerStride, im += outerStride) {
		transformDirect(re, im);
	}

	// Fft col-wise
	init(rows, outerStride);
	re = pRe;
	im = pIm;

	for (auto col = cols; col > 0; --col, re += innerStride, im += innerStride) {
		transformDirect(re, im);
	}
}

/// \pre cols and rows must be initialized w/ values more than 0
///
void Fft::transformComplement2(float *pRe, float *pIm)
{
	ohdebugonce(Fft::transformDirect2, 0, rows, cols, step, outerStride);
	// IFFT row-wise
	float *re = pRe;
	float *im = pIm;
	init(cols, innerStride);

	for (std::size_t row = rows; row > 0; --row, re += outerStride, im += outerStride) {
		transformComplement(re, im);
	}

	// IFFT col-wise
	re = pRe;
	im = pIm;
	init(rows, outerStride);

	for (auto col = cols; col > 0; --col, re += innerStride, im += innerStride) {
		transformComplement(re, im);
	}
}

void Fft::transformDirect(float *pRe, float *pIm)
{
	float tmp_re;
	float tmp_im;

	// First interchanging

	unsigned int n1, n2;
	unsigned char *ptrN1 = (unsigned char *)&n1;
	unsigned char *ptrN2 = (unsigned char *)&n2;
	for (n1 = 1; n1 < m_nMax - 1; n1++)
	{
		ptrN2[0] = g_reverse256[ptrN1[3]];
		ptrN2[1] = g_reverse256[ptrN1[2]];
		ptrN2[2] = g_reverse256[ptrN1[1]];
		ptrN2[3] = g_reverse256[ptrN1[0]];
		n2 >>= (32 - m_logN);
		if (n1 < n2)
		{
			tmp_re = pRe[step * n1];
			tmp_im = pIm[step * n1];
			pRe[step * n1] = pRe[step * n2];
			pIm[step * n1] = pIm[step * n2];
			pRe[step * n2] = tmp_re;
			pIm[step * n2] = tmp_im;
		}
	}

	// Reset rotation multiplier array

	m_wStore_re[0] = 1.0f;
	m_wStore_im[0] = 0.0f;

	// Main loop

	unsigned int N, Nd2;
	float *ptrW2n_re, *ptrW2n_im;
	unsigned int skew = 0, k = 0, m = 0, mpNd2 = 0;
	float wn_re = 0.0f, wn_im = 0.0f, w_re = 0.0f, w_im = 0.0f;
	float *ptrWArray_re = nullptr, *ptrWArray_im = nullptr;
	for (N = 2, Nd2 = 1, ptrW2n_re = g_w2n_re, ptrW2n_im = g_w2n_im, skew = m_nMax >> 1;
	N <= m_nMax;
		Nd2 = N, N += N, ptrW2n_re++, ptrW2n_im++, skew >>= 1)
	{
		// wn = W(1, N) = exp(-2 * pi * j / N)
		wn_re = *ptrW2n_re;
		wn_im = *ptrW2n_im;

		for (ptrWArray_re = &m_wStore_re[0], ptrWArray_im = &m_wStore_im[0], k = 0;
		k < Nd2;
			k++, ptrWArray_re += skew, ptrWArray_im += skew)
		{
			if (k & 1)
			{
				mulComplex(w_re, w_im, wn_re, wn_im);
				*ptrWArray_re = w_re;
				*ptrWArray_im = w_im;
			}
			else
			{
				w_re = *ptrWArray_re;
				w_im = *ptrWArray_im;
			}

			for (m = k; m < m_nMax; m += N)
			{
				mpNd2 = m + Nd2;

				tmp_re = w_re;
				tmp_im = w_im;
				mulComplex(tmp_re, tmp_im, pRe[step * mpNd2], pIm[step * mpNd2]);
				pRe[step * mpNd2] = pRe[step * m];
				pIm[step * mpNd2] = pIm[step * m];
				pRe[step * mpNd2] -= tmp_re;
				pIm[step * mpNd2] -= tmp_im;
				pRe[step * m] += tmp_re;
				pIm[step * m] += tmp_im;
			}
		}
	}
}

void Fft::transformComplement(float *pRe, float *pIm)
{
	float tmp_re;
	float tmp_im;

	// First interchanging

	unsigned int n1, n2;
	unsigned char *ptrN1 = (unsigned char *)&n1;
	unsigned char *ptrN2 = (unsigned char *)&n2;
	for (n1 = 1; n1 < m_nMax - 1; n1++)
	{
		ptrN2[0] = g_reverse256[ptrN1[3]];
		ptrN2[1] = g_reverse256[ptrN1[2]];
		ptrN2[2] = g_reverse256[ptrN1[1]];
		ptrN2[3] = g_reverse256[ptrN1[0]];
		n2 >>= (32 - m_logN);
		if (n1 < n2)
		{
			tmp_re = pRe[step * n1];
			tmp_im = pIm[step * n1];
			pRe[step * n1] = pRe[step * n2];
			pIm[step * n1] = pIm[step * n2];
			pRe[step * n2] = tmp_re;
			pIm[step * n2] = tmp_im;
		}
	}

	// Reset rotation multiplier array

	m_wStore_re[0] = 1.0f;
	m_wStore_im[0] = 0.0f;

	// Main loop

	unsigned int N, Nd2;
	float *ptrW2n_re = nullptr, *ptrW2n_im = nullptr;
	unsigned int skew = 0, k = 0, m = 0, mpNd2 = 0;
	float wn_re = 0.0f, wn_im = 0.0f, w_re = 0.0f, w_im = 0.0f;
	float *ptrWArray_re = nullptr, *ptrWArray_im = nullptr;
	for (N = 2, Nd2 = 1, ptrW2n_re = g_w2n_re, ptrW2n_im = g_w2n_im, skew = m_nMax >> 1;
	N <= m_nMax;
		Nd2 = N, N += N, ptrW2n_re++, ptrW2n_im++, skew >>= 1)
	{
		// wn = W(1, N) = exp(-2 * pi * j / N)
		wn_re = *ptrW2n_re;
		wn_im = -(*ptrW2n_im);	// Minus because it is a complement transform

		for (ptrWArray_re = &m_wStore_re[0], ptrWArray_im = &m_wStore_im[0], k = 0;
		k < Nd2;
			k++, ptrWArray_re += skew, ptrWArray_im += skew)
		{
			if (k & 1)
			{
				mulComplex(w_re, w_im, wn_re, wn_im);
				*ptrWArray_re = w_re;
				*ptrWArray_im = w_im;
			}
			else
			{
				w_re = *ptrWArray_re;
				w_im = *ptrWArray_im;
			}

			for (m = k; m < m_nMax; m += N)
			{
				mpNd2 = m + Nd2;

				tmp_re = w_re;
				tmp_im = w_im;
				mulComplex(tmp_re, tmp_im, pRe[step * mpNd2], pIm[step * mpNd2]);
				pRe[step * mpNd2] = pRe[step * m];
				pIm[step * mpNd2] = pIm[step * m];
				pRe[step * mpNd2] -= tmp_re;
				pIm[step * mpNd2] -= tmp_im;
				pRe[step * m] += tmp_re;
				pIm[step * m] += tmp_im;
			}
		}
	}

	// Because it is a complement transform
	for (n1 = 0; n1 < m_nMax; n1++)
	{
		pRe[step * n1] /= m_nMax;
		pIm[step * n1] /= m_nMax;
	}
}
