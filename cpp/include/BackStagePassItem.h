#pragma once
// #include "Item.h"
#include "GildedRoseItem.h"

class BackstagePassItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    static constexpr int MAX = 50;
    if (item_.quality < MAX)
      item_.quality++;
    if (item_.sellIn < 11 && item_.quality < MAX)
      item_.quality++;
    if (item_.sellIn < 6 && item_.quality < MAX)
      item_.quality++;
    if (item_.sellIn < 1)
      item_.quality = 0;
  }
};