#include "GildedRose.h"
#include "AgedBrieItem.h"
#include "BackstagePassItem.h"
#include "FoodBerverageItem.h"
#include "GildedRoseItem.h"
#include "NormalItem.h"
#include "SulfurasItem.h"


#include "Constans.h"

#include <memory>

std::unique_ptr<GildedRoseItem> createItem(Item &item) {
  if (item.name == AGED_BRIE)
    return std::make_unique<AgedBrieItem>(item);
  else if (item.name == BACKSTAGE_PASS)
    return std::make_unique<BackstagePassItem>(item);
  else if (item.name == SULFURAS)
    return std::make_unique<SulfurasItem>(item);
  else if (item.name.find("[F&B]") != std::string::npos)
    return std::make_unique<FoodBeverageItem>(item);
  // ← 추가!
  else
    return std::make_unique<NormalItem>(item);
}

GildedRose::GildedRose(std::vector<Item> &items) : items(items) {}
void GildedRose::updateQuality() {
  for (auto &item : items) {
    auto gildedItem = createItem(item); // Factory
    gildedItem->updateQuality();
    updateSellIn(item);
  }
}
void GildedRose::updateSellIn(Item &item) {
  if (item.name != SULFURAS)
    item.sellIn--;
}