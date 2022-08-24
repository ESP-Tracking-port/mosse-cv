#if !defined(MOSSE_MOSSEAPIDEBUG_HPP_)
#define MOSSE_MOSSEAPIDEBUG_HPP_

#if 1
# include <iostream>

template <class ...Ts>
void debugmImpl(Ts &&...aArgs)
{
	using List = int[];
	List{(void(std::cout << " " << aArgs), 0)...};
	std::cout << std::endl;
}

# define debug(a) std::cout << (#a) << " : " << (a) << std::endl
# define debugm(context, ...) debugmImpl(#context, # __VA_ARGS__, ##__VA_ARGS__)
# define debugstr(a) std::cout << (a) << std::endl;
#else
# define debug(...)
# define debugm(...)
# define debugstr(...)
#endif

#endif

