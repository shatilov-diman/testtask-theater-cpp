#include <iostream>

#include "config/config.hpp"

int main(int args, char* argv[]) {
  std::cout << "Theater Application" << std::endl;

  auto config = make_config();
  MovieRegistry& movie_registry = config->get_movie_registry();
  TheaterRegistry& theater_registry = config->get_theater_registry();
  SeatsRegistry& seats_registry = config->get_seats_registry();

  std::cout << "Initializing movies" << std::endl;

  movie_registry.save(Movie(movie_guid_t("movie_1"), "John Wick: Chapter 4", "The fourth installment of the John Wick franchise, directed by Chad Stahelski and starring Keanu Reeves."));
  movie_registry.save(Movie(movie_guid_t("movie_2"), "Transformers: Rise of the Beasts", "The fourth installment of the Transformers franchise, directed by Michael Bay and starring Shia LaBeouf."));
  movie_registry.save(Movie(movie_guid_t("movie_3"), "Barbie", "A Barbie movie, directed by Greta Gerwig and starring Margot Robbie."));
  movie_registry.save(Movie(movie_guid_t("movie_4"), "Oppenheimer", "A movie about the life of J. Robert Oppenheimer, directed by Christopher Nolan and starring Cillian Murphy."));
  movie_registry.save(Movie(movie_guid_t("movie_5"), "The Super Mario Bros. Movie", "A movie about the life of J. Robert Oppenheimer, directed by Christopher Nolan and starring Cillian Murphy."));

  std::cout << "Initializing theaters" << std::endl;

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

  std::cout << "Loading movies" << std::endl;
  const auto movies = movie_registry.load_movies(0, 10);
  std::cout << "Movies: " << movies.size() << std::endl;

  for (const auto& movie : movies) {
    const auto theaters = theater_registry.load_theaters_showing_movie(movie.get_guid(), 0, 10);
    for (const auto& theater : theaters) {
      std::cout << movie.get_name() << " is showing at " << theater.get_name() << std::endl;
    }
  }

  std::cout << "Loading theaters" << std::endl;
  const auto theaters = theater_registry.load_theaters(0, 10);
  std::cout << "Theaters: " << theaters.size() << std::endl;

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

  std::cout << "Loading seats" << std::endl;

  const auto seats = seats_registry.load(theater_guid_t("theater_1"), movie_guid_t("movie_1"));
  std::cout << "Seats: " << seats.get_seats().size() << std::endl;

  std::cout << "Booking seats a1, a2, b1, b2" << std::endl;
  const std::set<seat_guid_t> book_seats = { seat_guid_t("a1"), seat_guid_t("a2"), seat_guid_t("b1"), seat_guid_t("b2") };
  const auto booked = seats_registry.book_seats(theater_guid_t("theater_1"), movie_guid_t("movie_1"), book_seats);
  if (!booked) {
    std::cout << "Failed to book seats" << std::endl;
  }

  std::cout << "Booking seats a3, a4, b1, b2" << std::endl;
  const std::set<seat_guid_t> book_seats_2 = { seat_guid_t("a3"), seat_guid_t("a4"), seat_guid_t("b1"), seat_guid_t("b2") };
  const auto booked_2 = seats_registry.book_seats(theater_guid_t("theater_1"), movie_guid_t("movie_1"), book_seats_2);
  if (!booked_2) {
    std::cout << "Failed to book seats" << std::endl;
  }

  return 0;
}