#pragma once

#include "types.hpp"

enum class SeatState {
    AVAILABLE,
    BOOKED,
};

class Seats {
    std::map<seat_guid_t, SeatState> m_seats;
  public:
    Seats() {
    }
    Seats(std::map<seat_guid_t, SeatState> seats): m_seats(seats) {
    }
    ~Seats() {
    }

  public:
    const std::map<seat_guid_t, SeatState> get_seats() const {
      return m_seats;
    }

    void book_seats(const std::set<seat_guid_t>& seats) {
      for (const auto& seat : seats) {
        m_seats[seat] = SeatState::BOOKED;
      }
    }

    bool is_seat_available(const seat_guid_t& seat) const {
      const auto it = m_seats.find(seat);
      if (it == m_seats.end()) {
        return false;
      }
      return it->second == SeatState::AVAILABLE;
    }
};