#include "types.hpp"

enum class SeatState {
    AVAILABLE,
    RESERVED,
    BOOKED,
};

class Seats {
    std::map<seat_guid_t, SeatState> m_seats;
  public:
    Seats() {
    }
    ~Seats() {
    }

  public:
    const std::map<seat_guid_t, SeatState> get_seats() const {
      return m_seats;
    }
};