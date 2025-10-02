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

TEST(MovieRegistryTest, PaginationWorks) {
  auto ctx = make_app_context();
  auto& movies = ctx->movies();

  for (int i = 1; i <= 5; ++i) {
    movies.save(Movie(movie_guid_t("m" + std::to_string(i)), "Movie" + std::to_string(i), ""));
  }

  auto page1 = movies.load_movies(0, 2);
  ASSERT_EQ(page1.size(), 2u);
  EXPECT_EQ(page1[0].get_guid(), movie_guid_t("m1"));

  auto page2 = movies.load_movies(2, 2);
  ASSERT_EQ(page2.size(), 2u);
  EXPECT_EQ(page2[0].get_guid(), movie_guid_t("m3"));

  auto page3 = movies.load_movies(4, 2);
  ASSERT_EQ(page3.size(), 1u);
  EXPECT_EQ(page3[0].get_guid(), movie_guid_t("m5"));
}
