// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:		FFT
// Authors:		Miroslav Voinarovsky, Anton Egorov
// Description:	Fast Fourier Transformtaion is based on the
//				freeware algorithm written by Miroslav Voinarovsky
//				with insignificant modifications
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef FFT_H
#define FFT_H

#include <vector>

class Fft
{
public:

	void init(std::size_t rows, std::size_t cols, std::size_t step);
	void transformDirect(std::vector<float> &pRe, std::vector<float> &pIm);
	void transformComplement(std::vector<float> &pRe, std::vector<float> &pIm);

private:

	std::size_t step;
	int m_logN;
	unsigned int m_nMax;
	std::vector<float> m_wStore_re;
	std::vector<float> m_wStore_im;
};


#endif // FFT_H
