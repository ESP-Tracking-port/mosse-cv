#if !defined(MOSSE_MOSSEAPI_HPP_)
#define MOSSE_MOSSEAPI_HPP_

#include <utility>

namespace Mosse {

const float *getHann(unsigned rows, unsigned cols);
std::pair<const float *, const float *> getGaussKernelFft(unsigned rows, unsigned cols);
void getClosestWindow(unsigned &aRows, unsigned &aCols);

}  // namespace Mosse

#endif

