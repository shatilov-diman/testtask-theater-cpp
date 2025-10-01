#include "config/config.hpp"

#include "application/adapters/secondary/in-memory/movie_registry.hpp"
#include "application/adapters/secondary/in-memory/theater_registry.hpp"
#include "application/adapters/secondary/in-memory/seats_registry.hpp"

#include <memory>

class ConfigImpl : public Config {
  private:
    std::unique_ptr<MovieRegistry> m_movie_registry;
    std::unique_ptr<TheaterRegistry> m_theater_registry;
    std::unique_ptr<SeatsRegistry> m_seats_registry;

  public:
    ConfigImpl() {
      m_movie_registry = std::make_unique<MovieRegistryImpl>();
      m_theater_registry = std::make_unique<TheaterRegistryImpl>();
      m_seats_registry = std::make_unique<SeatsRegistryImpl>();
    }

  public:
    MovieRegistry& get_movie_registry() override {
      return *m_movie_registry;
    }
    TheaterRegistry& get_theater_registry() override {
      return *m_theater_registry;
    }
    SeatsRegistry& get_seats_registry() override {
      return *m_seats_registry;
    }
};

std::unique_ptr<Config> make_config() {
  return std::make_unique<ConfigImpl>();
}