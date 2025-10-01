#include <iostream>

#include "application/ports/secondary/movie_registry.hpp"
#include "application/ports/secondary/theater_registry.hpp"
#include "application/ports/secondary/seats_registry.hpp"

int main(int args, char* argv[]) {
  // TODO: inject the registries
  MovieRegistry& movie_registry = *(MovieRegistry*)(nullptr);
  TheaterRegistry& theater_registry = *(TheaterRegistry*)(nullptr);
  SeatsRegistry& seats_registry = *(SeatsRegistry*)(nullptr);

  movie_registry.save(Movie(movie_guid_t("movie_1"), "John Wick: Chapter 4", "The fourth installment of the John Wick franchise, directed by Chad Stahelski and starring Keanu Reeves."));
  movie_registry.save(Movie(movie_guid_t("movie_2"), "Transformers: Rise of the Beasts", "The fourth installment of the Transformers franchise, directed by Michael Bay and starring Shia LaBeouf."));
  movie_registry.save(Movie(movie_guid_t("movie_3"), "Barbie", "A Barbie movie, directed by Greta Gerwig and starring Margot Robbie."));
  movie_registry.save(Movie(movie_guid_t("movie_4"), "Oppenheimer", "A movie about the life of J. Robert Oppenheimer, directed by Christopher Nolan and starring Cillian Murphy."));
  movie_registry.save(Movie(movie_guid_t("movie_5"), "Barbie", "A Barbie movie, directed by Greta Gerwig and starring Margot Robbie."));

  theater_registry.save(Theater(theater_guid_t("theater_1"), "Dubai Mall Reel Cinema", "Enjoy a movie experience like never before in the world's largest and most luxurious cineplex", {
    movie_guid_t("movie_1"),
    movie_guid_t("movie_3"),
    movie_guid_t("movie_5"),
  }));
  theater_registry.save(Theater(theater_guid_t("theater_2"), "Mall of the Emirates Vox Cinema", "VOX Cinemas offers an exciting movie experience for all ages with IMAX & 3D. Book now to enjoy the latest Hollywood blockbusters, action, comedies & more.", {
    movie_guid_t("movie_1"),
    movie_guid_t("movie_2"),
    movie_guid_t("movie_3"),
  }));

  const auto movies = movie_registry.load_movies(0, 10);

  for (const auto& movie : movies) {
    const auto theaters = theater_registry.load_theaters_showing_movie(movie.get_guid(), 0, 10);
    for (const auto& theater : theaters) {
      std::cout << movie.get_name() << " is showing at " << theater.get_name() << std::endl;
    }
  }

  const auto theaters = theater_registry.load_theaters(0, 10);
  for (const auto& theater : theaters) {
    for (const auto& movie_guid : theater.get_movies_guids()) {
      const auto movie = movie_registry.load(movie_guid);
      if (!movie) {
        std::cout << "Movie not found: " << movie_guid << std::endl;
        continue;
      }
      std::cout << theater.get_name() << " is showing " << movie->get_name() << std::endl;
    }
  }

  const auto seats = seats_registry.load(theater_guid_t("theater_1"), movie_guid_t("movie_1"));
  seats_registry.book_seats(theater_guid_t("theater_1"), movie_guid_t("movie_1"), {
    seat_guid_t("a1"),
    seat_guid_t("a2"),
    seat_guid_t("b1"),
    seat_guid_t("b2"),
  });

  return 0;
}