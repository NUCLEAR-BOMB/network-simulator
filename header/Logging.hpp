#pragma once

//#define ENABLE_LOGGING

namespace net
{

void log(const char* const format, ...) noexcept;

#ifdef ENABLE_LOGGING
#define LOG(...) ::net::log(__VA_ARGS__)
#else
#define LOG(...) ((void)0)
#endif

}
