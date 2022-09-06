#if !defined(ONE_HEADER_DEBUG_HPP_)
#define ONE_HEADER_DEBUG_HPP_

#if 1 || OH_DEBUG

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
void debug(const char *, Stub, Ts &&...)
{
}

template <int G, class T>
void debug(const char *aName, T &&aT)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	std::cout << aName << "=" << aT << "  ";
}

}  // namespace OhDebug

# define ohdebug1__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebug2__(context, ## __VA_ARGS__);
# define ohdebug2__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebug3__(context, ## __VA_ARGS__);
# define ohdebug3__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebug4__(context, ## __VA_ARGS__);
# define ohdebug4__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebug5__(context, ## __VA_ARGS__);
# define ohdebug5__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebug6__(context, ## __VA_ARGS__);
# define ohdebug6__(context, a, ...) OhDebug::debug<context>(#a, a); ohdebugend__(context, ## __VA_ARGS__);
# define ohdebugend__(context, a, ...) OhDebug::debug<context>(#a, a)
# define ohdebug(context, ...) ohdebug1__(context, ##__VA_ARGS__, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, \
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
