#include <gtest/gtest.h>

#include "app/context.hpp"

#include <thread>
#include <future>

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


TEST(SeatsRegistryTest, ConcurrentOverbookingIsPrevented) {
  auto ctx = make_app_context();
  auto& seats = ctx->seats();

  const theater_guid_t t("t1");
  const movie_guid_t m("m1");

  auto f1 = std::async(std::launch::async, [&]() {
    return seats.book_seats(t, m, {seat_guid_t("a1"), seat_guid_t("a2")} );
  });
  auto f2 = std::async(std::launch::async, [&]() {
    return seats.book_seats(t, m, {seat_guid_t("a1"), seat_guid_t("a2")} );
  });

  const auto r1 = f1.get();
  const auto r2 = f2.get();

  EXPECT_TRUE(r1.has_value() ^ r2.has_value());
  if (r1.has_value()) {
    EXPECT_EQ(r1->value_of(), "a1 is not available");
  } else {
    EXPECT_EQ(r2->value_of(), "a1 is not available");
  }

  auto after = seats.load(t, m);
  ASSERT_TRUE(after.has_value());
  EXPECT_FALSE(after->is_seat_available(seat_guid_t("a1")));
  EXPECT_FALSE(after->is_seat_available(seat_guid_t("a2")));
}
