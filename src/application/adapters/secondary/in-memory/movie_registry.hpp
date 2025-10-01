#include "application/ports/secondary/movie_registry.hpp"

#include <mutex>

class MovieRegistryImpl : public MovieRegistry {
  private:
    mutable std::mutex m_mutex;
    std::map<movie_guid_t, std::optional<Movie>> m_movies;

  public:
    MovieRegistryImpl() {
    }
    virtual ~MovieRegistryImpl() {
    }

  public:
    void save(const Movie& movie) {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_movies[movie.get_guid()] = movie;
    }

    std::optional<Movie> load(const movie_guid_t& guid) const {
      std::lock_guard<std::mutex> lock(m_mutex);
      auto it = m_movies.find(guid);
      if (it == m_movies.end()) {
        return std::nullopt;
      }
      return it->second;
    }

    std::vector<Movie> load_movies(const std::size_t offset, const std::size_t limit) const {
      std::lock_guard<std::mutex> lock(m_mutex);
      std::vector<Movie> movies;

      std::size_t index = 0;
      for (const auto& movie : m_movies) {
        if (!movie.second) {
          continue;
        }
        if (index++ < offset) {
          continue;
        }
        movies.push_back(*movie.second);
        if (movies.size() >= limit) {
          break;
        }
      }
      return movies;
    }
};