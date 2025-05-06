#include "club.hpp"
#include "event.hpp"
#include "time.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool parse_input(const std::string &filename, int &tables_count, int &open,
                 int &close, int &cost, std::vector<Event> &events) {
  try {
    std::ifstream file(filename);
    std::string line;
    int line_number = 0;
    int sequence = 0;

    if (!std::getline(file, line)) {
      throw std::runtime_error("Line 1 missing");
    }
    tables_count = std::stoi(line);
    if (tables_count <= 0) {
      throw std::runtime_error("Invalid table count");
    }

    if (!std::getline(file, line)) {
      throw std::runtime_error("Line 2 missing");
    }
    std::stringstream ss(line);
    std::string open_str, close_str;
    if (!(ss >> open_str >> close_str)) {
      throw std::runtime_error("Invalid time format");
    }
    open = parse_time(open_str);
    close = parse_time(close_str);

    if (!std::getline(file, line)) {
      throw std::runtime_error("Line 3 missing");
    }
    cost = std::stoi(line);
    if (cost <= 0) {
      throw std::runtime_error("Invalid cost value");
    }

    while (std::getline(file, line)) {
      std::stringstream ess(line);
      std::string time_str, id_str;
      if (!(ess >> time_str >> id_str)) {
        throw std::runtime_error("Invalid event format");
      }

      int time = parse_time(time_str);
      int id = std::stoi(id_str);
      std::vector<std::string> body;
      std::string part;

      while (ess >> part) {
        body.push_back(part);
      }

      events.emplace_back(time, id, body, sequence++);
      line_number++;
    }

    return true;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return false;
  }
}

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }

  int tables_num, open_time, close_time, cost;
  std::vector<Event> input_events;

  if (!parse_input(argv[1], tables_num, open_time, close_time, cost,
                   input_events)) {
    return 1;
  }

  Club club(tables_num, open_time, close_time, cost);

  for (const auto &event : input_events) {
    club.add_input_event(event);
  }

  club.process_events();

  std::cout << format_time(open_time) << "\n";
  for (const auto &event : club.get_output_events()) {
    std::cout << format_time(event.time_) << " " << event.id_;
    for (const auto &part : event.body_) {
      std::cout << " " << part;
    }
    std::cout << "\n";
  }
  std::cout << format_time(close_time) << "\n";

  const auto &tables = club.get_tables();
  for (size_t i = 0; i < tables.size(); ++i) {
    const auto &table = tables[i];
    std::cout << (i + 1) << " " << table.total_revenue << " "
              << format_time(table.total_time) << "\n";
  }

  return 0;
}