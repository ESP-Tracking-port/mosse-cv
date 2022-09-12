//
// OhDebug.hpp
//
// Created: 2022-09-06
//  Author: Dmitry Murashov (dmtr <DOT> murashov <AT> GMAIL)
//

#if !defined(ONE_HEADER_DEBUG_HPP_)
#define ONE_HEADER_DEBUG_HPP_

#if 1 && !defined(OHDEBUG_DISABLE)  // OHDEBUG_DISABLE can be used to disable OHDEBUG locally
# define OHDEBUG  // Flag macro. Can be used to configure depending functionality

# if !defined(OHDEBUG_ENABLE_ALL_BY_DEFAULT)
#  define OHDEBUG_ENABLE_ALL_BY_DEFAULT 1
# endif  // OHDEBUG_ENABLE_ALL_BY_DEFAULT

# include <iostream>
# include <tuple>
# include <type_traits>
# include <limits>
# include <cassert>

namespace OhDebug {

// Compile-time CRC32 on strings, courtesy of tower120
// https://stackoverflow.com/questions/2111667/compile-time-string-hashing
// https://stackoverflow.com/users/1559666/tower120

static constexpr unsigned int crc_table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3,    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de,	0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,	0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5,	0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,	0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940,	0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,	0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};


template<int size, int idx = 0, class dummy = void>
struct MM{
  static constexpr unsigned int crc32(const char * str, unsigned int prev_crc = 0xFFFFFFFF)
  {
      return MM<size, idx+1>::crc32(str, (prev_crc >> 8) ^ crc_table[(prev_crc ^ str[idx]) & 0xFF] );
  }
};

// This is the stop-recursion function
template<int size, class dummy>
struct MM<size, size, dummy>{
  static constexpr unsigned int crc32(const char * str, unsigned int prev_crc = 0xFFFFFFFF)
  {
      return prev_crc^ 0xFFFFFFFF;
  }
};

// This don't take into account the nul char
#define OHDEBUG_COMPILE_TIME_CRC32_STR(x) (OhDebug::MM<sizeof(x)-1>::crc32(x))

#if OHDEBUG_ENABLE_ALL_BY_DEFAULT

template <unsigned G>
struct Enabled : std::true_type {
};

# define ohdebuggroup(g) \
	namespace OhDebug { \
	template <> \
	struct Enabled<OHDEBUG_COMPILE_TIME_CRC32_STR(#g)> : std::false_type { \
	}; \
	}  // namespace OhDebug
#else

template <unsigned G>
struct Enabled : std::false_type {
};

# define ohdebuggroup(g) \
	namespace OhDebug { \
	template <> \
	struct Enabled<OHDEBUG_COMPILE_TIME_CRC32_STR(#g)> : std::true_type { \
	}; \
	}  // namespace OhDebug

#endif  // OHDEBUG_ENABLE_ALL_BY_DEFAULT

struct Stub {
};

template <unsigned G, class ...Ts>
void ohdebugImpl(const char *, Stub, Ts &&...)
{
}

template <unsigned G, class T>
void ohdebugImpl(const char *aName, T &&aT)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	std::cout << aName << "=" << aT << "  ";
}

template <unsigned G>
void ohdebugImpl(const char *aName, const char *aValue)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	if (aName[0] == '"' || aValue[0] == '\0') {  // aName is a stringified ("-quoted) const char. It, therefore, contains the same value as `aValue`.
		std::cout << aName << "  ";
	} else {
		std::cout << aName << "=" << aValue << "  ";
	}
}

template <unsigned G>
void ohDebugPrintGroup(const char *aGroup)
{
	if (!OhDebug::Enabled<G>::value) {
		return;
	}

	std::cout << aGroup << " :   ";
}

template <unsigned G>
void ohDebugPrintNl()
{
	if (OhDebug::Enabled<G>::value) {
		std::cout << std::endl;
	}
}

}  // namespace OhDebug

# define ohdebugstringify__(a) #a
# define ohdebugflimpl__(line) __FILE__ ":" #line
# define ohdebugfl__(line) ohdebugflimpl__(line)
# define ohdebugvoid(a) (a, "")

# define ohdebug0__(context, a, ...) \
	OhDebug::ohDebugPrintGroup<OHDEBUG_COMPILE_TIME_CRC32_STR(#context)>(ohdebugfl__(__LINE__)); \
	OhDebug::ohDebugPrintGroup<OHDEBUG_COMPILE_TIME_CRC32_STR(#context)>(#context); \
	ohdebug1__(OHDEBUG_COMPILE_TIME_CRC32_STR(#context), a, ## __VA_ARGS__)

# define ohdebug1__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug2__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug2__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug3__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug3__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug4__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug4__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug5__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug5__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug6__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug6__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug7__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug7__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug8__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug8__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug9__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug9__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug10__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug10__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug11__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug11__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug12__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug12__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug13__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug13__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug14__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug14__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug15__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug15__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug16__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug16__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug17__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug17__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug18__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug18__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug19__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug19__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug20__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug20__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug21__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug21__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug22__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug22__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug23__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug23__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug24__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug24__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug25__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug25__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug26__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug26__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug27__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug27__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug28__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug28__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug29__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug29__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug30__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug30__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug31__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug31__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug32__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug32__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug33__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug33__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug34__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug34__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug35__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug35__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug36__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug36__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug37__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug37__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug38__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug38__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug39__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug39__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug40__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug40__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug41__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug41__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug42__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug42__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug43__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug43__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug44__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug44__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug45__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug45__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug46__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug46__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug47__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug47__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug48__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug48__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug49__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug49__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebug50__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug50__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	ohdebugend__(static_cast<unsigned>(context), ## __VA_ARGS__);
# define ohdebug(context, ...) ohdebug0__(context, ##__VA_ARGS__, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{}, OhDebug::Stub{})

# define ohdebugend__(context, a, ...) OhDebug::ohdebugImpl<static_cast<unsigned>(context)>(#a, a); \
	OhDebug::ohDebugPrintNl<static_cast<unsigned>(context)>()

# define ohdebugstr(context, a, ...) do {(void)a; } while (0); ohdebug(context, #a, ## __VA_ARGS__)

# define ohdebugsecteveryn(bump, ...) \
	do { \
		if (bump > 0) { \
			static unsigned n = 0; \
			if (n % bump == 0) { \
				n = 0; \
				__VA_ARGS__; \
			} \
			++n; \
		} \
	} while (0)

# define ohdebugonce(hit, ...) \
	do { \
		static int n = 0; \
		if (n == hit && hit >= 0) { \
			__VA_ARGS__ ; \
		}; \
		++n; \
	} while (0)

# define ohdebugsectif(cond, ...) \
	do { \
		if ( cond ) { \
			__VA_ARGS__ ; \
		} \
	} while(0)

# define ohdebugsect(...) \
	do { \
		__VA_ARGS__ ; \
	} while (0)

# define ohdebugassert(ctx, cond, ...) \
	do { \
		if (!(cond)) { \
			ohdebug(ctx, "assertion triggered", #cond, ## __VA_ARGS__ ) ; \
			assert(false); \
		} \
	} while(0)

#else
# define ohdebug(...)
# define ohdebugstr(...)
# define ohdebuggroup(...)
# define ohdebugsecteveryn(...)
# define ohdebugsectif(...)
# define ohdebugsect(...)
# define ohdebugassert(...)
#endif

#if defined(OHDEBUG_ENABLE_ALL_BY_DEFAULT)
# undef OHDEBUG_ENABLE_ALL_BY_DEFAULT
#endif

#endif
