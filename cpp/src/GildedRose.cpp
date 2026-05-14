#include "GildedRose.h"
#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "NormalItem.h"
#include "SulfurasItem.h"


GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}

void GildedRose::updateQuality() {
  for (auto &item : items) {
    if (item.name == AGED_BRIE)
      AgedBrieItem{item}.updateQuality();
    else if (item.name == BACKSTAGE_PASS)
      BackstagePassItem{item}.updateQuality();
    else if (item.name == SULFURAS)
      SulfurasItem{item}.updateQuality();
    else
      NormalItem{item}.updateQuality();
    updateSellIn(item);
  }
}
void GildedRose::updateAgedBrie(Item &item) {
  if (item.quality < MAX_QUALITY)
    item.quality = item.quality + 1;
  if (item.sellIn < 1)
    // 0 → 1로 변경
    if (item.quality < MAX_QUALITY)
      item.quality = item.quality + 1;
}
void GildedRose::updateBackstagePass(Item &item) {
  if (item.quality < MAX_QUALITY)
    item.quality = item.quality + 1;
  if (item.sellIn < 11 && item.quality < MAX_QUALITY)
    item.quality = item.quality + 1;
  if (item.sellIn < 6 && item.quality < MAX_QUALITY)
    item.quality = item.quality + 1;
  if (item.sellIn < 1)
    item.quality = 0;
}
void GildedRose::updateSulfuras(Item &) { /* 변화 없음 */ }
void GildedRose::updateNormalItem(Item &item) {
  if (item.quality > MIN_QUALITY)
    item.quality = item.quality - 1;
  if (item.sellIn < 1 && item.quality > MIN_QUALITY)
    item.quality = item.quality - 1;
}
void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS)
    item.sellIn--;
}
//   for (size_t i = 0; i < items.size(); i++) {
//     Item &item = items[i];
//     if (item.name == AGED_BRIE) {
//       if (item.quality < MAX_QUALITY)
//         item.quality++;
//       if (item.sellIn < 1)
//         // 0 → 1로 변경
//         if (item.quality < MAX_QUALITY)
//           item.quality++;
//     } else if (item.name == BACKSTAGE_PASS) {
//       if (item.quality < MAX_QUALITY) {
//         item.quality = item.quality + 1;
//       }

//       if (item.sellIn < 11) {
//         if (item.quality < MAX_QUALITY) {
//           item.quality = item.quality + 1;
//         }
//       }
//       if (item.sellIn < 6) {
//         if (item.quality < MAX_QUALITY) {
//           item.quality = item.quality + 1;
//         }
//       }
//       if (item.sellIn < 1) {
//         item.quality = 0;
//       }
//     } else if (item.name == SULFURAS) {
//       // 아무것도 안 함
//     } else {
//       if (item.quality > MIN_QUALITY)
//         item.quality--;
//       if (item.sellIn < 1)
//         if (item.quality > 0)
//           item.quality--;
//     }
//     if (item.name != SULFURAS)
//       item.sellIn--;
//   }
