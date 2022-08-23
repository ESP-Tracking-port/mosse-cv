#include "MosseApi.hpp"
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
	std::array<unsigned, 2>{80, 50},
	std::array<unsigned, 2>{50, 80},
	std::array<unsigned, 2>{95, 60},
	std::array<unsigned, 2>{60, 95},
	std::array<unsigned, 2>{70, 70},
	std::array<unsigned, 2>{60, 60},
	std::array<unsigned, 2>{50, 50},
	std::array<unsigned, 2>{40, 40}
);

static constexpr auto kHannMap = makeArray(
	kHann80x50Raw,
	kHann50x80Raw,
	kHann95x60Raw,
	kHann60x95Raw,
	kHann70x70Raw,
	kHann60x60Raw,
	kHann50x50Raw,
	kHann40x40Raw
);

static constexpr auto kGaussKernelFftMapImReal = makeArray(
	std::pair<const float *, const float *>{&kGaussKernelFft80x50ImReal[0][0], &kGaussKernelFft80x50ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft50x80ImReal[0][0], &kGaussKernelFft50x80ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft95x60ImReal[0][0], &kGaussKernelFft95x60ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft60x95ImReal[0][0], &kGaussKernelFft60x95ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft70x70ImReal[0][0], &kGaussKernelFft70x70ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft60x60ImReal[0][0], &kGaussKernelFft60x60ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft50x50ImReal[0][0], &kGaussKernelFft50x50ImReal[1][0]},
	std::pair<const float *, const float *>{&kGaussKernelFft40x40ImReal[0][0], &kGaussKernelFft40x40ImReal[1][0]}
);

/// \brief Compares two window size matrices using weighted preferences
/// regarding aspect ratio and difference in area sizes
///
static float windowGetDistance(const std::array<unsigned, 2> &a, const std::array<unsigned, 2> &b)
{
	constexpr auto kWeightAspectRatio = 0.2f;
	constexpr auto kWeightAreaSize = 1 - kWeightAspectRatio;
	const auto diffAspectRatio = static_cast<float>(a[0]) * b[1] / (a[1] * b[0]);
	const auto diffAreaSize  = static_cast<float>(a[0]) * a[1] / (b[0] * b[1]);
	const auto distance = diffAspectRatio * kWeightAspectRatio + diffAreaSize * kWeightAreaSize;

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

}  // namespace Mosse

