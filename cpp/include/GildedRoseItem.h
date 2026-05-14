#pragma once
#include "Item.h"
#include <vector>

class GildedRoseItem {
protected:
  Item &item_;

public:
  explicit GildedRoseItem(Item &item) : item_(item) {}
  virtual void updateQuality() = 0; // 순수 가상 함수 (추상 메서드)
  virtual ~GildedRoseItem() = default;
};