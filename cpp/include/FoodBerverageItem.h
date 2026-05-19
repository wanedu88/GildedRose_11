#pragma once
#include "GildedRoseItem.h"

class FoodBeverageItem : public GildedRoseItem {
public:
  explicit FoodBeverageItem(Item &item) : GildedRoseItem(item) {}
  void updateQuality() override {
    // 일반의 2배 감소
    if (item_.quality > 0) {
      item_.quality--;
      if (item_.quality > 0)
        item_.quality--; // 2배
    }
    // sellIn 지나면 추가 2배 (총 4배)

    if (item_.sellIn < 1) {
      if (item_.quality > 0) {
        item_.quality--;
        if (item_.quality > 0)
          item_.quality--;
      }
    }
  }
};
