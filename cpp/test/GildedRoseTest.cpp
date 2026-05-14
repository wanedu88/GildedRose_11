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

// back stage 10일 초과 시 +1
TEST(GildedRoseTest, backstange1) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 15, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(14, app.items[0].sellIn);
  EXPECT_EQ(1, app.items[0].quality);
}
// 공연 지난 후 품질 0
TEST(GildedRoseTest, backstange2) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 0)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}
// 품질 50 초과 후 공연 종료
TEST(GildedRoseTest, backstange3) {
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", 0, 51)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(-1, app.items[0].sellIn);
  EXPECT_EQ(0, app.items[0].quality);
}

// == == == == == == == == == = 강의 자료 예시 == == == == == == == == == == ==

// 경계값: quality는 0 미만이 되어선 안 된다
TEST(GildedRoseTest, QualityNeverNegative) {
  std::vector<Item> items = {Item("Normal Item", 5, 0)};
  GildedRose gr(items);
  gr.updateQuality();
  EXPECT_GE(items[0].quality, 0); // quality >= 0 보장
}
// 경계값: quality는 50을 초과해선 안 된다
TEST(GildedRoseTest, AgedBrieQualityMax50) {
  std::vector<Item> items = {Item("Aged Brie", 5, 50)};
  GildedRose gr(items);
  gr.updateQuality();
  EXPECT_LE(items[0].quality, 50); // quality <= 50 보장
}
// 경계값: sellIn 지나면 quality 2배 감소
TEST(GildedRoseTest, NormalItemDegradesTwiceAfterSellDate) {
  std::vector<Item> items = {Item("Normal Item", 0, 10)};
  GildedRose gr(items);
  gr.updateQuality();
  EXPECT_EQ(8, items[0].quality); // 10 - 2 =
}

// Backstage Passes 경계값—파라미터화테스트(C++)
class BackstagePassTest
    : public ::testing::TestWithParam<std::tuple<int, int, int>> {};
// <sellIn, initialQuality, expectedQuality>
TEST_P(BackstagePassTest, QualityUpdate) {
  auto [sellIn, initQ, expectedQ] = GetParam();
  std::vector<Item> items = {
      Item("Backstage passes to a TAFKAL80ETC concert", sellIn, initQ)};
  GildedRose gr(items);
  gr.updateQuality();
  EXPECT_EQ(expectedQ, items[0].quality);
}
TEST(FoodBeverageTest, DegradesTwiceAsNormal) {
  std::vector<Item> items = {Item("[F&B] Bread", 5, 20)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(4, items[0].sellIn);
  EXPECT_EQ(18, items[0].quality); // 20 -2 = 18
}
TEST(FoodBeverageTest, DegradesFourTimesAfterSellIn) {
  std::vector<Item> items = {Item("[F&B] Milk", 0, 20)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(16, items[0].quality); // 20 -4 = 16
}
TEST(FoodBeverageTest, QualityNeverBelowZero) {
  std::vector<Item> items = {Item("[F&B] Water", 0, 1)};
  GildedRose app(items);
  app.updateQuality();
  EXPECT_EQ(0, items[0].quality); // 0 유지
}

INSTANTIATE_TEST_SUITE_P(
    BackstageBoundary, BackstagePassTest,
    ::testing::Values(
        // (sellIn, initQ, expectedQ)
        std::make_tuple(15, 20, 21), // > 10: +1
        std::make_tuple(11, 20, 21), // 경계: sellIn=11 → +1
        std::make_tuple(10, 20, 22), // 경계: sellIn=10 → +2
        std::make_tuple(6, 20, 22),  // 경계: sellIn=6 → +2
        std::make_tuple(5, 20, 23),  // 경계: sellIn=5 → +3
        std::make_tuple(1, 20, 23),  // 경계: sellIn=1 → +3
        std::make_tuple(0, 20, 0),   // 경계: sellIn=0 → quality=0
        std::make_tuple(5, 50, 50),  // quality 상한50
        std::make_tuple(0, 50, 0)    // concert 후→ 0
        ));