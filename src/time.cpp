#include "time.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>

int parse_time(const std::string &time_as_string) {
  int hours, minutes;
  char colon;
  std::istringstream iss(time_as_string);
  if (!(iss >> hours >> colon >> minutes) || colon != ':' || hours < 0 ||
      hours >= 24 || minutes < 0 || minutes >= 60) {
    throw std::invalid_argument("Invalid time format");
  }
  return hours * 60 + minutes;
}

std::string format_time(const int minutes) {
  const int hours = minutes / 60;
  const int remaining_minutes = minutes % 60;
  std::ostringstream oss;
  oss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2)
      << std::setfill('0') << remaining_minutes;
  return oss.str();
}