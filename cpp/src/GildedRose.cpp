#include "GildedRose.h"

#include <algorithm>

GildedRose::GildedRose(std::vector<Item>& items) : items(items) {}

namespace {
constexpr int MIN_QUALITY = 0;
constexpr int MAX_QUALITY = 50;
constexpr int SULFURAS_QUALITY = 80;

bool isAgedBrie(const Item& item) {
    return item.name == "Aged Brie";
}

bool isBackstagePass(const Item& item) {
    return item.name == "Backstage passes to a TAFKAL80ETC concert";
}

bool isSulfuras(const Item& item) {
    return item.name == "Sulfuras, Hand of Ragnaros";
}

bool isConjured(const Item& item) {
    return item.name.rfind("Conjured", 0) == 0;
}

void increaseQuality(Item& item, int amount) {
    item.quality = std::min(MAX_QUALITY, item.quality + amount);
}

void decreaseQuality(Item& item, int amount) {
    item.quality = std::max(MIN_QUALITY, item.quality - amount);
}
}

void GildedRose::updateQuality() {
    for (Item& item : items) {
        if (isSulfuras(item)) {
            item.quality = SULFURAS_QUALITY;
            continue;
        }

        if (isAgedBrie(item)) {
            increaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        } else if (isBackstagePass(item)) {
            if (item.sellIn <= 0) {
                item.quality = MIN_QUALITY;
            } else if (item.sellIn <= 5) {
                increaseQuality(item, 3);
            } else if (item.sellIn <= 10) {
                increaseQuality(item, 2);
            } else {
                increaseQuality(item, 1);
            }
        } else if (isConjured(item)) {
            decreaseQuality(item, item.sellIn <= 0 ? 4 : 2);
        } else {
            decreaseQuality(item, item.sellIn <= 0 ? 2 : 1);
        }

        item.sellIn = item.sellIn - 1;
    }
}
