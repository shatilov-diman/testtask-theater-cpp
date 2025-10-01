# Theater (CLI)

Interactive console app to manage theaters, movies, and seat bookings. Dependencies and builds are handled by Conan + CMake.

## Requirements
- Conan 2.x
- CMake (provided as a build-require by Conan)
- C++17 compiler

## Build (Debug)

```bash
conan install . -of=.build --build=missing -s build_type=Debug
conan build . -of=.build --build=missing -s build_type=Debug
```

## Run

```bash
./.build/build/Debug/theater
```

Available commands:
- help
- add-movie
- add-theater
- add-movie-to-theater
- list-movies
- list-movies-in-theater
- list-theaters
- theaters-for-movie
- show-seats
- book-seats
- exit

Initial data (including some pre-booked seats) is seeded automatically at startup.
