#include "MosseApi.hpp"
#include "MosseApiDebug.hpp"
#include "MosseTables/MosseTables.hpp"
#include <array>
#include <limits>
#include <cstdlib>
#include <type_traits>
#include <algorithm>

namespace Mosse {

template <class T, class ...Ts>
static constexpr auto makeArray(T &&a, Ts &&...as) -> std::array<typename std::decay<T>::type, 1 + sizeof...(Ts)>
{
	using Type = typename std::decay<T>::type;
	// static_assert(std::is_same<typename std::decay<const float(*)[80]>::type, float **>::value, "");
	return std::array<Type, 1 + sizeof...(Ts)>{{Type{a}, Type{as}...}};
}

static constexpr auto kWindowSizes = makeArray(
	std::array<unsigned, 2>{64, 64},
	std::array<unsigned, 2>{64, 32},
	std::array<unsigned, 2>{32, 64}
);

static constexpr auto kHannMap = makeArray(
	kHann64x64Raw,
	kHann64x32Raw,
	kHann32x64Raw
);

static constexpr auto kGaussKernelFftScaled125 = makeArray(
	kGaussKernelFft64x64Scaled125Raw,
	kGaussKernelFft64x32Scaled125Raw,
	kGaussKernelFft32x64Scaled125Raw
);

static constexpr auto kGaussKernelFft3dScaled125 = makeArray(
	kGaussKernelFft64x643dScaled125Raw,
	kGaussKernelFft64x323dScaled125Raw,
	kGaussKernelFft32x643dScaled125Raw
);

static constexpr auto kGaussKernelFftMapImReal = makeArray(
	std::pair<const float *, const float *>{&kGaussKernelFft64x64ImReal[0][0], &kGaussKernelFft64x64ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft64x32ImReal[0][0], &kGaussKernelFft64x32ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft32x64ImReal[0][0], &kGaussKernelFft32x64ImReal[1][0]}
);

static constexpr auto kGaussKernelFftMapImReal3d = makeArray(
	kGaussKernelFft64x64ImReal3dRaw,
	kGaussKernelFft64x32ImReal3dRaw,
	kGaussKernelFft32x64ImReal3dRaw
);


/// \brief Compares two window size matrices using weighted preferences
/// regarding aspect ratio and difference in area sizes
///
static float windowGetDistance(const std::array<unsigned, 2> &a, const std::array<unsigned, 2> &b)
{
	auto a0 = static_cast<float>(a[0]);
	auto a1 = static_cast<float>(a[1]);
	auto b0 = static_cast<float>(b[0]);
	auto b1 = static_cast<float>(b[1]);

	constexpr auto kWeightAspectRatio = 0.6f;
	constexpr auto kWeightAreaSize = 1 - kWeightAspectRatio;
	const auto diffAspectRatio = pow(a0 - b0 * a1 / b1, 2);  // Compare by aspect ratio numerators, ensure common denominator, raise to the power of 2 to get "px^2" units
	const auto diffAreaSize = abs(a0 * a1 - b0 * b1);  // Difference in areas, units - "px^2"
	const auto distance = kWeightAreaSize * diffAspectRatio + kWeightAreaSize * diffAreaSize;

	return distance;
}

void getClosestWindow(unsigned &aRows, unsigned &aCols)
{
	const std::array<unsigned, 2> ref{{aRows, aCols}};
	auto itClosest = std::min_element(std::begin(kWindowSizes), std::end(kWindowSizes),
		[&ref](const std::array<unsigned, 2> &aLhs, const std::array<unsigned, 2> &aRhs)
		{
			return windowGetDistance(aLhs, ref) < windowGetDistance(aRhs, ref);
		});
	aRows = (*itClosest)[0];
	aCols = (*itClosest)[1];
}

int checkWindowExists(unsigned aRows, unsigned aCols)
{
	int counter = 0;

	for (const auto size : kWindowSizes) {
		if (aRows == size[0] && aCols == size[1]) {
			return counter;
		}

		counter += 1;
	}

	return -1;
}

const float *getHann(unsigned aRows, unsigned aCols)
{
	int id = checkWindowExists(aRows, aCols);

	if (id < 0) {
		return nullptr;
	}

	return kHannMap[id];
}

std::pair<const float *, const float *> getGaussKernelFft(unsigned aRows, unsigned aCols)
{
	int id = checkWindowExists(aRows, aCols);

	if (id < 0) {
		return {nullptr, nullptr};
	}

	return kGaussKernelFftMapImReal[id];
}

const float *getGaussKernelFft3d(unsigned aRows, unsigned aCols)
{
	int id = checkWindowExists(aRows, aCols);

	if (id < 0) {
		return nullptr;
	}

	return kGaussKernelFftMapImReal3d[id];
}

const float *getLogTable8bit()  ///< Returns a float log table for integers from 1 to 256
{
	return kLogMatrix8bit;
}

const float *getGaussKernelFft3dScaled125(unsigned aRows, unsigned aCols)
{
	int id = checkWindowExists(aRows, aCols);

	if (id < 0) {
		return nullptr;
	}

	return kGaussKernelFft3dScaled125[id];
}

const float *getGaussKernelFftScaled125(unsigned aRows, unsigned aCols)
{
	int id = checkWindowExists(aRows, aCols);

	if (id < 0) {
		return nullptr;
	}

	return kGaussKernelFftScaled125[id];
}



}  // namespace Mosse
