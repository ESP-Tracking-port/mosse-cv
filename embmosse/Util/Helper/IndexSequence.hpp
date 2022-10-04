#if !defined(MOSSE_UTIL_INDEXSEQUENCE_HPP_)
#define MOSSE_UTIL_INDEXSEQUENCE_HPP_

namespace Mosse {
namespace Ut {

template <unsigned ...Is>
struct IndexSequence {
};

template <unsigned Ibase, unsigned Iceil, unsigned ...Is>
struct MakeIndexSequence {
	using Type = typename MakeIndexSequence<Ibase + 1, Iceil, Is..., Ibase>::Type;
};

template <unsigned Iceil, unsigned ...Is>
struct MakeIndexSequence<Iceil, Iceil, Is...> {
	using Type = IndexSequence<Is...>;
};

template <unsigned N>
constexpr auto makeIndexSequence() -> decltype(typename MakeIndexSequence<0, N>::Type{})
{
	return typename MakeIndexSequence<0, N>::Type{};
}

}  // namespace Ut
}  // namespace Mosse

#endif // MOSSE_UTIL_INDEXSEQUENCE_HPP_
