#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <set>

#include "app/context.hpp"
#include "domain/movie.hpp"
#include "domain/theater.hpp"
#include "domain/seats.hpp"

namespace {
  template <typename ST = std::string>
  ST prompt(const std::string& text) {
    std::cout << text;
    std::string line;
    std::getline(std::cin, line);
    return ST(line);
  }

  void init_data(MovieRegistry& movie_registry, TheaterRegistry& theater_registry, SeatsRegistry& seats_registry) {
    // Movies
    movie_registry.save(Movie(movie_guid_t("movie_1"), "John Wick: Chapter 4", "Fourth installment of John Wick."));
    movie_registry.save(Movie(movie_guid_t("movie_2"), "Transformers: Rise of the Beasts", "Transformers chapter."));
    movie_registry.save(Movie(movie_guid_t("movie_3"), "Barbie", "Comedy by Greta Gerwig."));
    movie_registry.save(Movie(movie_guid_t("movie_4"), "Oppenheimer", "Christopher Nolan biopic."));
    movie_registry.save(Movie(movie_guid_t("movie_5"), "The Super Mario Bros. Movie", "Animated adventure."));

    // Theaters
    theater_registry.save(Theater(theater_guid_t("theater_1"), "Dubai Mall Reel Cinema", "Luxurious cineplex", {
      movie_guid_t("movie_1"),
      movie_guid_t("movie_3"),
      movie_guid_t("movie_5"),
    }));
    theater_registry.save(Theater(theater_guid_t("theater_2"), "Mall of the Emirates Vox Cinema", "IMAX & 3D experience", {
      movie_guid_t("movie_1"),
      movie_guid_t("movie_2"),
      movie_guid_t("movie_3"),
    }));

    // Book some seats
    seats_registry.book_seats(theater_guid_t("theater_1"), movie_guid_t("movie_1"), {
      seat_guid_t("a1"), seat_guid_t("a2"), seat_guid_t("b1"), seat_guid_t("b2")
    });
    seats_registry.book_seats(theater_guid_t("theater_1"), movie_guid_t("movie_3"), {
      seat_guid_t("a3"), seat_guid_t("a4")
    });
    seats_registry.book_seats(theater_guid_t("theater_2"), movie_guid_t("movie_2"), {
      seat_guid_t("c1"), seat_guid_t("c2"), seat_guid_t("d5")
    });
  }

  void cmd_add_movie(MovieRegistry& movie_registry) {
    const auto guid = prompt<movie_guid_t>("Movie GUID: ");
    const auto name = prompt("Movie name: ");
    const auto description = prompt("Movie description: ");
    movie_registry.save(Movie(guid, name, description));
    std::cout << "OK: movie saved\n";
  }

  void cmd_add_theater(TheaterRegistry& theater_registry) {
    const auto guid = prompt<theater_guid_t>("Theater GUID: ");
    const auto name = prompt("Theater name: ");
    const auto description = prompt("Theater description: ");
    theater_registry.save(Theater(guid, name, description, {}));
    std::cout << "OK: theater saved\n";
  }

  void cmd_add_movie_to_theater(TheaterRegistry& theater_registry, MovieRegistry& movie_registry) {
    const auto tguid = prompt<theater_guid_t>("Theater GUID: ");
    const auto mguid = prompt<movie_guid_t>("Movie GUID: ");

    const auto movie = movie_registry.load(mguid);
    if (!movie) {
      std::cout << "ERR: movie not found\n";
      return;
    }

    auto theater = theater_registry.load(tguid);
    if (!theater) {
      std::cout << "ERR: theater not found\n";
      return;
    }

    theater->add_movie_guid(mguid);
    theater_registry.save(*theater);
    std::cout << "OK: movie added to theater\n";
  }

  void cmd_list_movies(MovieRegistry& movie_registry) {
    const auto movies = movie_registry.load_movies(0, 1000);
    if (movies.empty()) {
      std::cout << "No movies\n";
      return;
    }
    for (const auto& m : movies) {
      std::cout << "- " << m.get_guid() << ": " << m.get_name() << "\n";
    }
  }

  void cmd_list_movies_in_theater(TheaterRegistry& theater_registry, MovieRegistry& movie_registry) {
    const auto tguid = prompt<theater_guid_t>("Theater GUID: ");
    const auto theater = theater_registry.load(tguid);
    if (!theater) {
      std::cout << "No theater found\n";
      return;
    }
    const auto movies = theater->get_movies_guids();
    if (movies.empty()) {
      std::cout << "No movies in this theater\n";
      return;
    }
    for (const auto& m : movies) {
      const auto movie = movie_registry.load(m);
      if (!movie) {
        std::cout << "No movie found\n";
        continue;
      }
      std::cout << "- " << movie->get_guid() << ": " << movie->get_name() << "\n";
    }
  }

  void cmd_list_theaters(TheaterRegistry& theater_registry) {
    const auto theaters = theater_registry.load_theaters(0, 1000);
    if (theaters.empty()) {
      std::cout << "No theaters\n";
      return;
    }
    for (const auto& t : theaters) {
      std::cout << "- " << t.get_guid() << ": " << t.get_name() << "\n";
    }
  }

  void cmd_theaters_for_movie(MovieRegistry& movie_registry, TheaterRegistry& theater_registry) {
    const auto mguid = prompt<movie_guid_t>("Movie GUID: ");
    const auto movie = movie_registry.load(mguid);
    if (!movie) {
      std::cout << "No movie found\n";
      return;
    }
    const auto theaters = theater_registry.load_theaters_showing_movie(mguid, 0, 1000);
    if (theaters.empty()) {
      std::cout << "No theaters for this movie\n";
      return;
    }
    for (const auto& t : theaters) {
      std::cout << "- " << t.get_guid() << ": " << t.get_name() << "\n";
    }
  }

  void print_seats(const Seats& seats) {
    std::stringstream ss;
    std::size_t index = 0;
    for (const auto& kv : seats.get_seats()) {
      if (kv.second == SeatState::AVAILABLE) {
        ss << '[' << kv.first << ']';
      } else {
        ss << '<' << kv.first << '>';
      }
      if (index++ % 5 == 4) {
        ss << '\n';
      }
    }
    std::cout << ss.str() << "\n";
  }

  void cmd_show_seats(TheaterRegistry& theater_registry, MovieRegistry& movie_registry, SeatsRegistry& seats_registry) {
    const auto tguid = prompt<theater_guid_t>("Theater GUID: ");
    const auto mguid = prompt<movie_guid_t>("Movie GUID: ");
    const auto theater = theater_registry.load(tguid);
    if (!theater) {
      std::cout << "No theater found\n";
      return;
    }
    const auto movie = movie_registry.load(mguid);
    if (!movie) {
      std::cout << "No movie found\n";
      return;
    }
    const auto s = seats_registry.load(tguid, mguid);
    if (!s) {
      std::cout << "No movie showing in this theater\n";
      return;
    }
    print_seats(*s);
  }

  void cmd_book_seats(TheaterRegistry& theater_registry, MovieRegistry& movie_registry, SeatsRegistry& seats_registry) {
    const auto tguid = prompt<theater_guid_t>("Theater GUID: ");
    const auto mguid = prompt<movie_guid_t>("Movie GUID: ");
    const auto theater = theater_registry.load(tguid);
    if (!theater) {
      std::cout << "No theater found\n";
      return;
    }
    const auto movie = movie_registry.load(mguid);
    if (!movie) {
      std::cout << "No movie found\n";
      return;
    }
    const auto seats_csv = prompt("Seats to book (comma separated, e.g. a1,b2,c3): ");
    std::set<seat_guid_t> seats_to_book;
    std::istringstream ss(seats_csv);
    std::string token;
    while (std::getline(ss, token, ',')) {
      if (!token.empty()) {
        seats_to_book.insert(seat_guid_t(token));
      }
    }
    const auto result = seats_registry.book_seats(tguid, mguid, seats_to_book);
    if (result) {
      std::cout << *result << "\n";
    } else {
      std::cout << "OK: seats booked\n";
    }
  }

  void print_help() {
    std::cout << "Commands:\n"
              << "  help                     - Show this help\n"
              << "  add-movie                - Add a movie\n"
              << "  add-theater              - Add a theater\n"
              << "  add-movie-to-theater     - Assign a movie to a theater\n"
              << "  list-movies              - List movies\n"
              << "  list-movies-in-theater   - List movies in a theater\n"
              << "  list-theaters            - List theaters\n"
              << "  theaters-for-movie       - List theaters that show a movie\n"
              << "  show-seats               - Show seats availability for theater+movie\n"
              << "  book-seats               - Book seats for theater+movie\n"
              << "  exit                     - Quit\n";
  }
}

int main(int /*args*/, char* /*argv*/[]) {
  std::cout << "Theater Application (interactive)" << std::endl;

  auto context = make_app_context();
  MovieRegistry& movie_registry = context->movies();
  TheaterRegistry& theater_registry = context->theaters();
  SeatsRegistry& seats_registry = context->seats();

  // Seed initial data unconditionally
  init_data(movie_registry, theater_registry, seats_registry);

  print_help();
  for (;;) {
    std::cout << "> ";
    std::string cmd;
    if (!std::getline(std::cin, cmd)) break;
    if (cmd == "help" || cmd == "h") {
      print_help();
    } else if (cmd == "add-movie") {
      cmd_add_movie(movie_registry);
    } else if (cmd == "add-theater") {
      cmd_add_theater(theater_registry);
    } else if (cmd == "add-movie-to-theater") {
      cmd_add_movie_to_theater(theater_registry, movie_registry);
    } else if (cmd == "list-movies") {
      cmd_list_movies(movie_registry);
    } else if (cmd == "list-movies-in-theater") {
      cmd_list_movies_in_theater(theater_registry, movie_registry);
    } else if (cmd == "list-theaters") {
      cmd_list_theaters(theater_registry);
    } else if (cmd == "theaters-for-movie") {
      cmd_theaters_for_movie(movie_registry, theater_registry);
    } else if (cmd == "show-seats") {
      cmd_show_seats(theater_registry, movie_registry, seats_registry);
    } else if (cmd == "book-seats") {
      cmd_book_seats(theater_registry, movie_registry, seats_registry);
    } else if (cmd == "exit" || cmd == "quit" || cmd == "q") {
      break;
    } else if (cmd.empty()) {
      continue;
    } else {
      std::cout << "Unknown command. Type 'help'\n";
    }
  }

  return 0;
}