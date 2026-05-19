#include <gtest/gtest.h>
#include "GildedRose.h"

class GildedRoseTest : public ::testing::Test {
protected:
    static constexpr const char* NORMAL = "foo";
    static constexpr const char* AGED_BRIE = "Aged Brie";
    static constexpr const char* BACKSTAGE_PASS = "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* SULFURAS = "Sulfuras, Hand of Ragnaros";
    static constexpr const char* CONJURED = "Conjured Mana Cake";
};

TEST_F(GildedRoseTest, NormalDecreasesQualityAndSellInBeforeExpiry) {
    // Given
    std::vector<Item> items = { Item(NORMAL, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(19, app.items[0].quality);
}

TEST_F(GildedRoseTest, NormalDegradesTwiceAsFastWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(NORMAL, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseTest, NormalDegradesTwiceAsFastWhenSellInIsNegative) {
    // Given
    std::vector<Item> items = { Item(NORMAL, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseTest, NormalQualityNeverDropsBelowZero) {
    // Given
    std::vector<Item> items = { Item(NORMAL, 5, 0) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(4, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, NormalExpiredQualityNeverDropsBelowZero) {
    // Given
    std::vector<Item> items = { Item(NORMAL, 0, 1) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(-1, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesQualityBeforeExpiry) {
    // Given
    std::vector<Item> items = { Item(AGED_BRIE, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(21, app.items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(AGED_BRIE, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieIncreasesTwiceWhenSellInIsNegative) {
    // Given
    std::vector<Item> items = { Item(AGED_BRIE, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieQualityNeverExceedsFifty) {
    // Given
    std::vector<Item> items = { Item(AGED_BRIE, 5, 50) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(4, app.items[0].sellIn);
    ASSERT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseTest, AgedBrieExpiredQualityCapsAtFifty) {
    // Given
    std::vector<Item> items = { Item(AGED_BRIE, 0, 49) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(-1, app.items[0].sellIn);
    ASSERT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByOneWhenMoreThanTenDaysRemain) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, 11, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(10, app.items[0].sellIn);
    EXPECT_EQ(21, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByTwoWhenTenDaysRemain) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(22, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassIncreasesByThreeWhenFiveDaysRemain) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, 5, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(4, app.items[0].sellIn);
    EXPECT_EQ(23, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassDropsToZeroWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(-1, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassDropsToZeroWhenSellInIsNegative) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(-2, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, BackstagePassQualityNeverExceedsFifty) {
    // Given
    std::vector<Item> items = { Item(BACKSTAGE_PASS, 5, 49) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(4, app.items[0].sellIn);
    ASSERT_EQ(50, app.items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasNeverChangesBeforeExpiry) {
    // Given
    std::vector<Item> items = { Item(SULFURAS, 10, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(10, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasNeverChangesWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(SULFURAS, 0, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(0, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasNeverChangesWhenSellInIsNegative) {
    // Given
    std::vector<Item> items = { Item(SULFURAS, -1, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasKeepsLegendaryQualityAcrossMultipleUpdates) {
    // Given
    std::vector<Item> items = { Item(SULFURAS, 5, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();
    app.updateQuality();

    // Then
    ASSERT_EQ(5, app.items[0].sellIn);
    ASSERT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseTest, SulfurasIsNotCappedToNormalMaximumQuality) {
    // Given
    std::vector<Item> items = { Item(SULFURAS, 5, 80) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(5, app.items[0].sellIn);
    ASSERT_EQ(80, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredDecreasesQualityByTwoBeforeExpiry) {
    // Given
    std::vector<Item> items = { Item(CONJURED, 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(18, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredDecreasesQualityByFourWhenSellInIsZero) {
    // Given
    std::vector<Item> items = { Item(CONJURED, 0, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-1, app.items[0].sellIn);
    EXPECT_EQ(16, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredDecreasesQualityByFourWhenSellInIsNegative) {
    // Given
    std::vector<Item> items = { Item(CONJURED, -1, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(-2, app.items[0].sellIn);
    EXPECT_EQ(16, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredQualityNeverDropsBelowZero) {
    // Given
    std::vector<Item> items = { Item(CONJURED, 5, 1) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(4, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredExpiredQualityNeverDropsBelowZero) {
    // Given
    std::vector<Item> items = { Item(CONJURED, 0, 3) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    ASSERT_EQ(-1, app.items[0].sellIn);
    ASSERT_EQ(0, app.items[0].quality);
}

TEST_F(GildedRoseTest, ConjuredDetectionUsesPrefix) {
    // Given
    std::vector<Item> items = { Item("Not Conjured", 10, 20) };
    GildedRose app(items);

    // When
    app.updateQuality();

    // Then
    EXPECT_EQ(9, app.items[0].sellIn);
    EXPECT_EQ(19, app.items[0].quality);
}
