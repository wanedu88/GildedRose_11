#pragma once
//  #include "Item.h"
#include "GildedRoseItem.h"
class SulfurasItem : public GildedRoseItem {
public:
  using GildedRoseItem::GildedRoseItem;
  void updateQuality() override { /* 변화 없음 */ }
};