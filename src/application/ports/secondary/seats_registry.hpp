#pragma once

#include "domain/seats.hpp"

#include <optional>

class SeatsRegistry {
  public:
    SeatsRegistry() {
    }
    virtual ~SeatsRegistry() {
    }

  public:
    virtual std::optional<Seats> load(const theater_guid_t& theater_guid, const movie_guid_t& movie_guid) = 0;

    virtual bool book_seats(const theater_guid_t& theater_guid, const movie_guid_t& movie_guid, const std::set<seat_guid_t>& seats) = 0;
};