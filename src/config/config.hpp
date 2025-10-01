#pragma once

#include "application/ports/secondary/movie_registry.hpp"
#include "application/ports/secondary/theater_registry.hpp"
#include "application/ports/secondary/seats_registry.hpp"

#include <memory>

class Config {
  public:
    Config() {
    }
    virtual ~Config() {
    }

  public:
    virtual MovieRegistry& get_movie_registry() = 0;
    virtual TheaterRegistry& get_theater_registry() = 0;
    virtual SeatsRegistry& get_seats_registry() = 0;
};

std::unique_ptr<Config> make_config();