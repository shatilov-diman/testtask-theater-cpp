#include "application/ports/secondary/seats_registry.hpp"

#include <mutex>

class SeatsRegistryImpl : public SeatsRegistry {
  private:
    mutable std::mutex m_mutex;
    std::map<theater_guid_t, std::map<movie_guid_t, Seats>> m_seats;
    Seats m_default_seats;

  public:
    SeatsRegistryImpl() {
      auto seats = std::map<seat_guid_t, SeatState>();
      for (const auto& seat : {
        seat_guid_t("a1"), seat_guid_t("a2"), seat_guid_t("a3"), seat_guid_t("a4"), seat_guid_t("a5"),
        seat_guid_t("b1"), seat_guid_t("b2"), seat_guid_t("b3"), seat_guid_t("b4"), seat_guid_t("b5"),
        seat_guid_t("c1"), seat_guid_t("c2"), seat_guid_t("c3"), seat_guid_t("c4"), seat_guid_t("c5"),
        seat_guid_t("d1"), seat_guid_t("d2"), seat_guid_t("d3"), seat_guid_t("d4"), seat_guid_t("d5"),
      }) {
        seats[seat] = SeatState::AVAILABLE;
      }
      m_default_seats = Seats(seats);
    }
    virtual ~SeatsRegistryImpl() {
    }

  public:
    std::optional<Seats> load(const theater_guid_t& theater_guid, const movie_guid_t& movie_guid) {
      std::lock_guard<std::mutex> lock(m_mutex);
      auto it = m_seats.find(theater_guid);
      if (it == m_seats.end()) {
        return std::nullopt;
      }
      auto it2 = it->second.find(movie_guid);
      if (it2 == it->second.end()) {
        return std::nullopt;
      }
      return it2->second;
    }

    bool book_seats(const theater_guid_t& theater_guid, const movie_guid_t& movie_guid, const std::set<seat_guid_t>& seats) {
      std::lock_guard<std::mutex> lock(m_mutex);
      auto it = m_seats.find(theater_guid);
      if (it == m_seats.end()) {
        auto seats = std::map<movie_guid_t, Seats>();
        it = m_seats.insert(std::make_pair(theater_guid, seats)).first;
      }
      auto it2 = it->second.find(movie_guid);
      if (it2 == it->second.end()) {
        it2 = it->second.insert(std::make_pair(movie_guid, m_default_seats)).first;
      }
      for (const auto& seat : seats) {
        if (!it2->second.is_seat_available(seat)) {
          return false;
        }
      }
      it2->second.book_seats(seats);
      return true;
    }
};