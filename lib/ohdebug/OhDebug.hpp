//
// OhDebug.hpp
//
// Created: 2022-09-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> GMAIL)
//

#if !defined(ONE_HEADER_DEBUG_HPP_)
#define ONE_HEADER_DEBUG_HPP_

#if 1

#if !defined(OHDEBUG_ENABLE_ALL_BY_DEFAULT)
# define OHDEBUG_ENABLE_ALL_BY_DEFAULT 1
#endif  // OHDEBUG_ENABLE_ALL_BY_DEFAULT

# include <iostream>
#include <tuple>
#include <type_traits>

namespace OhDebug {

#if OHDEBUG_ENABLE_ALL_BY_DEFAULT

template <int G>
struct Enabled : std::true_type {
};

# define ohdebuggroup(g) \
	namespace OhDebug { \
	template <> \
	struct Enabled<g> : std::false_type { \
	}; \
	}  // namespace OhDebug
#else

template <int G>
struct Enabled : std::false_type {
};

# define ohdebuggroup(g) \
	namespace OhDebug { \
	template <> \
	struct Enabled<g> : std::true_type { \
	}; \
	}  // namespace OhDebug

#endif  // OHDEBUG_ENABLE_ALL_BY_DEFAULT

struct Stub {
};

template <int G, class ...Ts>
void ohdebugImpl(const char *, Stub, Ts &&...)
{
}

template <int G, class T>
void ohdebugImpl(const char *aName, T &&aT)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	std::cout << aName << "=" << aT << "  ";
}

template <int G>
void ohDebugPrintGroup(const char *aGroup)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	std::cout << aGroup << ": ";
}

template <int G>
void ohDebugPrintNl()
{
	if (OhDebug::Enabled<G>::value) {
		std::cout << std::endl;
	}
}

}  // namespace OhDebug

# define ohdebugflimpl(line) __FILE__ ":" #line
# define ohdebugfl(line) ohdebugflimpl(line)

# define ohdebug0__(context, a, ...) OhDebug::ohDebugPrintGroup<static_cast<int>(context)>(ohdebugfl(__LINE__)); \
	OhDebug::ohDebugPrintGroup<static_cast<int>(context)>(#context); \
	ohdebug1__(context, a, ## __VA_ARGS__)
# define ohdebug1__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebug2__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebug2__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebug3__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebug3__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebug4__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebug4__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebug5__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebug5__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebug6__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebug6__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	ohdebugend__(static_cast<int>(context), ## __VA_ARGS__);
# define ohdebugend__(context, a, ...) OhDebug::ohdebugImpl<static_cast<int>(context)>(#a, a); \
	OhDebug::ohDebugPrintNl<static_cast<int>(context)>()
# define ohdebug(context, ...) ohdebug0__(context, ##__VA_ARGS__, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, \
	OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{})
# define ohdebugstr(a) std::cout << (a) << std::endl;
#else
# define ohdebug(...)
# define ohdebugstr(...)
# define ohdebuggroup(...)
#endif

#if defined(OHDEBUG_ENABLE_ALL_BY_DEFAULT)
# undef OHDEBUG_ENABLE_ALL_BY_DEFAULT
#endif

#endif
