// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:		FFT
// Authors:		Miroslav Voinarovsky, Anton Egorov
// Description:	Fast Fourier Transformtaion is based on the
//				freeware algorithm written by Miroslav Voinarovsky
//				with insignificant modifications
//
// UPD 2022-09-16. Added modifications of even lesser significance. The version is based on
// https://github.com/WowSoLaggy/MosseTracker
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

#ifndef FFT_H
#define FFT_H

#include <vector>

class Fft
{
public:

	/// \brief Init for 1-dimensional FFT
	/// \arg size - signal length ("Re, Im, Re, Im" sequence has length of 2)
	///
	/// \arg step - step between 2 consecutive elements in a sequence. E.g., for "Re Im Re Im" layout it will have value
	/// 2, for "Re Re ... Im Im ..." - 1
	///
	void init(std::size_t len, std::size_t step);

	void init2(std::size_t rows, std::size_t cols, std::size_t step);  ///< Init for (I)FFT2
	void transformDirect2(float *pRe, float *pIm);  ///< FFT2
	void transformComplement2(float *pRe, float *pIm);  ///< IFFT2
	void transformDirect(float *pRe, float *pIm);  ///< FFT
	void transformComplement(float *pRe, float *pIm);  ///< IFFT
private:
	std::size_t step;
	std::size_t cols;  ///< Signal length
	std::size_t rows;
	std::size_t innerStride;
	std::size_t outerStride;
	int m_logN;
	unsigned int m_nMax;
	std::vector<float> m_wStore_re;
	std::vector<float> m_wStore_im;
};


#endif // FFT_H
