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

// 전설 아이템은 변하지 않음
TEST(GildedRoseTest, Sulfuras1) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", 0, 5)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}
// 기한 마감 후에도 변하지 않음
TEST(GildedRoseTest, Sulfuras2) {
  std::vector<Item> items = {Item("Sulfuras, Hand of Ragnaros", -1, 5)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(5, app.items[0].quality);
}
// 기한 지난 Aged Brie는 품질 +2
TEST(GildedRoseTest, AgedBrie1) {
  std::vector<Item> items = {Item("Aged Brie", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(2, app.items[0].quality);
}
// Aged Brie품질 상한 테스트
TEST(GildedRoseTest, AgedBrie2) {
  std::vector<Item> items = {Item("Aged Brie", 0, 50)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(50, app.items[0].quality);
}
