#include "GildedRose.h"
#include <gtest/gtest.h>

TEST(GildedRoseTest, Foo) {
  std::vector<Item> items = {Item("foo", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ("fixme", app.items[0].name);
}
// 일반 아이템 품질 하한
TEST(GildedRoseTest, noname1) {
  std::vector<Item> items = {Item("noname", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

// 기한 지난 일반 아이템
TEST(GildedRoseTest, noname2) {
  std::vector<Item> items = {Item("noname", 0, 5)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(3, app.items[0].quality);
}
