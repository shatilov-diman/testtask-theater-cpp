#include "app/context.hpp"

#include "application/adapters/secondary/in-memory/movie_registry.hpp"
#include "application/adapters/secondary/in-memory/theater_registry.hpp"
#include "application/adapters/secondary/in-memory/seats_registry.hpp"

class AppContextImpl : public AppContext {
  private:
    std::unique_ptr<MovieRegistry> m_movies;
    std::unique_ptr<TheaterRegistry> m_theaters;
    std::unique_ptr<SeatsRegistry> m_seats;

  public:
    AppContextImpl() {
      m_movies = std::make_unique<MovieRegistryImpl>();
      m_theaters = std::make_unique<TheaterRegistryImpl>();
      m_seats = std::make_unique<SeatsRegistryImpl>();
    }

  public:
    MovieRegistry& movies() override { return *m_movies; }
    TheaterRegistry& theaters() override { return *m_theaters; }
    SeatsRegistry& seats() override { return *m_seats; }
};

std::unique_ptr<AppContext> make_app_context() {
  return std::make_unique<AppContextImpl>();
}

