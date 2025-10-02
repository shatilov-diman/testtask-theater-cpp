#include <gtest/gtest.h>

#include "app/context.hpp"

TEST(SeatsRegistryTest, LoadReturnsEmptyWhenNotInitialized) {
  auto ctx = make_app_context();
  SeatsRegistry& seats = ctx->seats();
  auto s = seats.load(theater_guid_t("t1"), movie_guid_t("m1"));
  EXPECT_FALSE(s.has_value());
}

TEST(SeatsRegistryTest, BookAndLoadReflectsBookedSeats) {
  auto ctx = make_app_context();
  SeatsRegistry& seats = ctx->seats();

  EXPECT_FALSE(seats.load(theater_guid_t("t1"), movie_guid_t("m1")).has_value());

  const std::set<seat_guid_t> to_book{ seat_guid_t("a1"), seat_guid_t("a2") };
  EXPECT_EQ(seats.book_seats(theater_guid_t("t1"), movie_guid_t("m1"), to_book), std::nullopt);

  auto after = seats.load(theater_guid_t("t1"), movie_guid_t("m1"));
  ASSERT_TRUE(after.has_value());
  EXPECT_FALSE(after->is_seat_available(seat_guid_t("a1")));
  EXPECT_FALSE(after->is_seat_available(seat_guid_t("a2")));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

