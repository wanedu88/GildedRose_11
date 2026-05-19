#pragma once
// #include "Item.h"
#include "GildedRoseItem.h"
class NormalItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override {
    if (item_.quality > 0)
      item_.quality--;
    if (item_.sellIn < 1 && item_.quality > 0)
      item_.quality--;
  }
};