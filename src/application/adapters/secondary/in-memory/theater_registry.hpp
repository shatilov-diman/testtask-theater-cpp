#include "application/ports/secondary/theater_registry.hpp"

#include <mutex>

class TheaterRegistryImpl : public TheaterRegistry {
  private:
    mutable std::mutex m_mutex;
    std::map<theater_guid_t, std::optional<Theater>> m_theaters;

  public:
    TheaterRegistryImpl() {
    }
    virtual ~TheaterRegistryImpl() {
    }

  public:
    void save(const Theater& theater) {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_theaters[theater.get_guid()] = theater;
    }

    std::optional<Theater> load(const theater_guid_t& guid) const {
      std::lock_guard<std::mutex> lock(m_mutex);
      auto it = m_theaters.find(guid);
      if (it == m_theaters.end()) {
        return std::nullopt;
      }
      return it->second;
    }

    std::vector<Theater> load_theaters(const std::size_t offset, const std::size_t limit) const {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::vector<Theater> theaters;

      std::size_t index = 0;
      for (const auto& theater : m_theaters) {
        if (!theater.second) {
          continue;
        }
        if (index++ < offset) {
          continue;
        }
        theaters.push_back(*theater.second);
        if (theaters.size() >= limit) {
          break;
        }
      }
      return theaters;
    }

    std::vector<Theater> load_theaters_showing_movie(const movie_guid_t& movie_guid, const std::size_t offset, const std::size_t limit) const {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::vector<Theater> theaters;

      std::size_t index = 0;
      for (const auto& theater : m_theaters) {
        if (!theater.second) {
          continue;
        }
        const auto& movies_guids = theater.second->get_movies_guids();
        if (movies_guids.find(movie_guid) != movies_guids.end()) {
          if (index++ < offset) {
            continue;
          }
          theaters.push_back(*theater.second);
          if (theaters.size() >= limit) {
            break;
          }
        }
      }
      return theaters;
    }
};