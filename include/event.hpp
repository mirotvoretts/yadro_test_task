#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <vector>

struct Event {
  int time_;
  int id_;
  std::vector<std::string> body_;
  int sequence_;

  Event(int time, int id, const std::vector<std::string> &body, int sequence);
  bool operator<(const Event &other) const;
};

#endif // EVENT_HPP