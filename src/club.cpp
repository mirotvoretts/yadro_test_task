#include "club.hpp"
#include <algorithm>
#include <sstream>

Club::Club(const int tables_count, const int opening_time,
           const int closing_time, const int cost_per_hour)
    : opening_time_(opening_time), closing_time_(closing_time),
      cost_per_hour_(cost_per_hour), tables_(tables_count),
      sequence_counter_(0) {}

void Club::add_input_event(const Event &event) { event_queue_.push(event); }

void Club::process_events() {
  while (!event_queue_.empty()) {
    Event event = event_queue_.top();
    event_queue_.pop();
    handle_event(event);
  }
  generate_closing_events();
}

const std::vector<Event> &Club::get_output_events() const {
  return output_events_;
}

const std::vector<Table> &Club::get_tables() const { return tables_; }

int Club::get_next_sequence() { return sequence_counter_++; }

void Club::handle_event(const Event &event) {
  output_events_.push_back(event);
  switch (event.id_) {
  case 1:
    handle_client_arrival(event);
    break;
  case 2:
  case 12:
    handle_client_sit(event);
    break;
  case 3:
    handle_client_wait(event);
    break;
  case 4:
  case 11:
    handle_client_leave(event);
    break;
  default:
    break;
  }
}

void Club::handle_client_sit(const Event &event) {
  const std::string &client = event.body_[0];
  const int table_num = std::stoi(event.body_[1]) - 1;

  if (!clients_.contains(client)) {
    generate_error(event.time_, "ClientUnknown", event.sequence_);
    return;
  }

  if (table_num < 0 || table_num >= static_cast<int>(tables_.size())) {
    return;
  }

  if (tables_[table_num].is_taken) {
    generate_error(event.time_, "PlaceIsBusy", event.sequence_);
    return;
  }

  if (clients_[client] != -1) {
    free_table(clients_[client], event.time_);
  }

  tables_[table_num].is_taken = true;
  tables_[table_num].client = client;
  tables_[table_num].start_time = event.time_;
  clients_[client] = table_num;
}

void Club::handle_client_arrival(const Event &event) {
  const std::string &client = event.body_[0];

  if (event.time_ < opening_time_ || event.time_ >= closing_time_) {
    generate_error(event.time_, "NotOpenYet", event.sequence_);
    return;
  }

  if (clients_.contains(client)) {
    generate_error(event.time_, "YouShallNotPass", event.sequence_);
    return;
  }

  clients_[client] = -1;
}

void Club::handle_client_wait(const Event &event) {
  const std::string &client = event.body_[0];

  if (!clients_.contains(client)) {
    generate_error(event.time_, "ClientUnknown", event.sequence_);
    return;
  }

  for (const auto &table : tables_) {
    if (!table.is_taken) {
      generate_error(event.time_, "ICanWaitNoLonger!", event.sequence_);
      return;
    }
  }

  if (waiting_queue_.size() >= tables_.size()) {
    clients_.erase(client);
    output_events_.emplace_back(event.time_, 11, std::vector{client},
                                get_next_sequence());
    return;
  }

  waiting_queue_.push_back(client);
}

void Club::handle_client_leave(const Event &event) {
  const std::string &client = event.body_[0];

  if (!clients_.contains(client)) {
    generate_error(event.time_, "ClientUnknown", event.sequence_);
    return;
  }

  if (clients_[client] != -1) {
    free_table(clients_[client], event.time_);
  }

  clients_.erase(client);
}

void Club::generate_error(const int time, const std::string &error,
                          int trigger_sequence) {
  const auto it =
      std::ranges::find_if(output_events_, [trigger_sequence](const Event &e) {
        return e.sequence_ == trigger_sequence;
      });

  if (it != output_events_.end()) {
    output_events_.insert(it + 1,
                          Event(time, 13, {error}, get_next_sequence()));
  }
}

void Club::generate_closing_events() {
  std::vector<std::string> remaining_clients;
  remaining_clients.reserve(clients_.size());

  for (const auto &[client, table] : clients_) {
    remaining_clients.push_back(client);
  }

  std::ranges::sort(remaining_clients);

  for (const auto &client : remaining_clients) {
    output_events_.emplace_back(closing_time_, 11, std::vector{client},
                                get_next_sequence());
    if (clients_[client] != -1) {
      free_table(clients_[client], closing_time_);
    }
  }
}

void Club::free_table(const int table_index, int end_time) {
  Table &table = tables_[table_index];
  const int duration = end_time - table.start_time;
  const int hours = (duration + 59) / 60;

  table.total_revenue += hours * cost_per_hour_;
  table.total_time += duration;
  table.is_taken = false;

  if (!waiting_queue_.empty()) {
    const std::string new_client = waiting_queue_.front();
    waiting_queue_.pop_front();

    table.is_taken = true;
    table.client = new_client;
    table.start_time = end_time;
    clients_[new_client] = table_index;

    output_events_.emplace_back(
        end_time, 12, std::vector{new_client, std::to_string(table_index + 1)},
        get_next_sequence());
  }
}
