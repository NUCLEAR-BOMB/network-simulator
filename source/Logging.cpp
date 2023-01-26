#include "Logging.hpp"

#include <cstdarg>
#include <cstdio>

void net::log(const char* const format, ...) noexcept
{
	std::putchar('\t');

	va_list args;
	va_start(args, format);
	std::vfprintf(stdout, format, args);
	va_end(args);

	std::putchar('\n');
}
