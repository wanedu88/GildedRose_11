#pragma once
#include "Item.h"
class SulfurasItem {
  Item &item_;

public:
  explicit SulfurasItem(Item &item) : item_(item) {}
  void updateQuality() { /* 변화 없음 */ }
};