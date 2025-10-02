#include <gtest/gtest.h>

#include "app/context.hpp"

TEST(MovieRegistryTest, SaveAndListMovies) {
  auto ctx = make_app_context();
  auto& movies = ctx->movies();

  movies.save(Movie(movie_guid_t("m1"), "Movie 1", "Desc"));
  movies.save(Movie(movie_guid_t("m2"), "Movie 2", "Desc"));

  auto m1 = movies.load(movie_guid_t("m1"));
  ASSERT_TRUE(m1.has_value());
  EXPECT_EQ(m1->get_name(), "Movie 1");

  auto list = movies.load_movies(0, 10);
  EXPECT_EQ(list.size(), 2u);
}
