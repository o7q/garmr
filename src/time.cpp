#include "time.hpp"

#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>

std::string getFormattedTime(std::time_t t)
{
    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t); // Windows
#else
    localtime_r(&t, &tm); // POSIX
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%m-%d-%Y_%H-%M-%S");
    return oss.str();
}