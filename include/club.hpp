#ifndef CLUB_HPP
#define CLUB_HPP

#include <deque>
#include <queue>
#include <unordered_map>
#include <vector>
#include "event.hpp"

struct Table {
    bool is_taken = false;
    std::string client;
    int start_time = 0;
    int total_revenue = 0;
    int total_time = 0;
};

class Club {
public:
    Club(
        int tables_count,
        int opening_time,
        int closing_time,
        int cost_per_hour
    );
    void add_input_event(const Event &event);
    void process_events();
    const std::vector<Event> &get_output_events() const;
    const std::vector<Table> &get_tables() const;

private:
    int opening_time_;
    int closing_time_;
    int cost_per_hour_;
    std::vector<Table> tables_;
    std::unordered_map<std::string, int> clients_;
    std::deque<std::string> waiting_queue_;
    std::priority_queue<Event> event_queue_;
    std::vector<Event> output_events_;
    int sequence_counter_;

    int get_next_sequence();
    void handle_event(const Event &event);
    void handle_client_arrival(const Event &event);
    void handle_client_sit(const Event &event);
    void handle_client_wait(const Event &event);
    void handle_client_leave(const Event &event);
    void free_table(int table_index, int end_time);
    void
    generate_error(int time, const std::string &error, int trigger_sequence);
    void generate_closing_events();
};

#endif  // CLUB_HPP