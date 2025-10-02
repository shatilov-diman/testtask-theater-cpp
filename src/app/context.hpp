#pragma once

#include "application/ports/secondary/movie_registry.hpp"
#include "application/ports/secondary/theater_registry.hpp"
#include "application/ports/secondary/seats_registry.hpp"
#include <memory>

class AppContext {
  public:
    AppContext() {}
    virtual ~AppContext() {}

  public:
    virtual MovieRegistry& movies() = 0;
    virtual TheaterRegistry& theaters() = 0;
    virtual SeatsRegistry& seats() = 0;
};

std::unique_ptr<AppContext> make_app_context();

