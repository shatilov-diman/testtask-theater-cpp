#include "domain/theater.hpp"

class TheaterRegistry {
  public:
    TheaterRegistry() {
    }
    virtual ~TheaterRegistry() {
    }

  public:
    virtual void save(const Theater& theater) = 0;

    virtual std::optional<Theater> load(const theater_guid_t& guid) const = 0;

    virtual std::vector<Theater> load_theaters(const std::size_t offset, const std::size_t limit) const = 0;

    virtual std::vector<Theater> load_theaters_showing_movie(const movie_guid_t& movie_guid, const std::size_t offset, const std::size_t limit) const = 0;
};
