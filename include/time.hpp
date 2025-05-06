#ifndef TIME_HPP
#define TIME_HPP

#include <string>

int parse_time(const std::string &time_as_string);
std::string format_time(int minutes);

#endif // TIME_HPP