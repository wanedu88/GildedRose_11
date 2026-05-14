#pragma once
#include "Item.h"
class NormalItem {
  Item &item_;

public:
  explicit NormalItem(Item &item) : item_(item) {}
  void updateQuality() {
    static constexpr int MIN = 0;
    if (item_.quality > MIN)
      item_.quality = item_.quality - 1;
    if (item_.sellIn < 1 && item_.quality > MIN)
      item_.quality = item_.quality - 1;
  }
};