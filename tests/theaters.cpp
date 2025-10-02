#include <gtest/gtest.h>

#include "app/context.hpp"

TEST(TheaterRegistryTest, SaveAndQueryTheatersForMovie) {
  auto ctx = make_app_context();
  auto& movies = ctx->movies();
  auto& theaters = ctx->theaters();

  movies.save(Movie(movie_guid_t("m1"), "Movie 1", "Desc"));
  movies.save(Movie(movie_guid_t("m2"), "Movie 2", "Desc"));

  theaters.save(Theater(theater_guid_t("t1"), "Theater 1", "Desc", { movie_guid_t("m1") }));
  theaters.save(Theater(theater_guid_t("t2"), "Theater 2", "Desc", { movie_guid_t("m2"), movie_guid_t("m1") }));

  auto res = theaters.load_theaters_showing_movie(movie_guid_t("m1"), 0, 10);
  ASSERT_EQ(res.size(), 2u);
}

TEST(TheaterRegistryTest, AddMovieToTheater) {
  auto ctx = make_app_context();
  auto& movies = ctx->movies();
  auto& theaters = ctx->theaters();

  const auto mg = movie_guid_t("m1");
  const auto tg = theater_guid_t("t1");
  movies.save(Movie(mg, "M1", ""));
  theaters.save(Theater(tg, "T1", "", {}));

  auto t = theaters.load(tg);
  ASSERT_TRUE(t.has_value());
  t->add_movie_guid(mg);
  theaters.save(*t);

  auto res = theaters.load_theaters_showing_movie(mg, 0, 10);
  ASSERT_EQ(res.size(), 1u);
  EXPECT_EQ(res[0].get_guid(), tg);
}
