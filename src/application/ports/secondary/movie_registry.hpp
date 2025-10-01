#pragma once

#include "domain/movie.hpp"

class MovieRegistry {
  public:
    MovieRegistry() {
    }
    virtual ~MovieRegistry() {
    }

  public:
    virtual void save(const Movie& movie) = 0;

    virtual std::optional<Movie> load(const movie_guid_t& guid) const = 0;

    virtual std::vector<Movie> load_movies(const std::size_t offset, const std::size_t limit) const = 0;
};