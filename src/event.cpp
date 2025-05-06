#include "event.hpp"

Event::Event(
    const int time,
    const int id,
    const std::vector<std::string> &body,
    const int sequence
)
    : time_(time), id_(id), body_(body), sequence_(sequence) {
}

bool Event::operator<(const Event &other) const {
    if (time_ != other.time_) {
        return time_ > other.time_;
    }
    return sequence_ > other.sequence_;
}