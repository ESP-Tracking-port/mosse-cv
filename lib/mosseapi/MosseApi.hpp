#if !defined(MOSSE_MOSSEAPI_HPP_)
#define MOSSE_MOSSEAPI_HPP_

#include <utility>

namespace Mosse {

const float *getHann(unsigned rows, unsigned cols);
std::pair<const float *, const float *> getGaussKernelFft(unsigned rows, unsigned cols);
const float *getGaussKernelFft3d(unsigned rows, unsigned cols);
const float *getGaussKernelFft3dScaled125(unsigned &aRows, unsigned &aCols);

void getClosestWindow(unsigned &aRows, unsigned &aCols);
const float *getLogTable8bit();  ///< Returns a float log table for integers from 1 to 256

}  // namespace Mosse

#endif

