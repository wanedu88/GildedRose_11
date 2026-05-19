# Gilded Rose 테스트 계획서

## 1. 목적 및 범위

본 문서는 C++17 기반 Gilded Rose 레거시 코드의 비즈니스 규칙을 Google Test로 검증하기 위한 테스트 계획이다. 주요 목표는 기존 동작을 안정적으로 특성화하고, 신규 `"Conjured"` 아이템 요구사항을 안전하게 추가할 수 있도록 `TEST_F` 기반 단위 테스트와 경계값 테스트를 체계화하는 것이다.

대상 코드는 다음과 같다.

- `cpp/include/GildedRose.h`
- `cpp/include/Item.h`
- `cpp/src/GildedRose.cpp`
- `GildedRoseRequirements.txt`
- `docs/requirements_analysis.md`

`Item`의 public 구조(`name`, `sellIn`, `quality`)는 요구사항상 변경하지 않는다.

## 2. 테스트 전략

### 2.1 테스트 수준

- 단위 테스트: `GildedRose::updateQuality()`의 하루 단위 상태 변경을 검증한다.
- 특성화 테스트: 리팩터링 전 현재 레거시 동작을 고정한다.
- 회귀 테스트: Normal, Aged Brie, Backstage passes, Sulfuras, Conjured 규칙이 변경되지 않는지 검증한다.
- 다중 일자 테스트: 여러 번 `updateQuality()`를 호출했을 때 일 단위 규칙이 누적 적용되는지 검증한다.

### 2.2 테스트 작성 원칙

- Google Test의 `TEST_F`를 사용해 공통 fixture를 구성한다.
- 테스트명은 구현 세부사항이 아니라 비즈니스 동작을 설명한다.
- Given-When-Then 구조를 유지한다.
- 상태 변화가 `sellIn`과 `quality`에 모두 영향을 주는 경우 두 값을 모두 검증한다.
- 각 테스트는 독립적이고 결정적으로 실행되어야 한다.
- 리팩터링 전에는 먼저 실패/성공하는 테스트로 현재 동작과 목표 동작을 분리한다.

## 3. TEST_F 기반 단위 테스트 구조

권장 fixture는 다음 역할을 가진다.

- 공통 아이템명 상수 제공
- 단일 아이템 업데이트 헬퍼 제공
- 여러 아이템 벡터 업데이트 헬퍼 제공
- `sellIn`, `quality`, `name` 검증 헬퍼 제공

예상 구조:

```cpp
class GildedRoseUpdateQualityTest : public ::testing::Test {
protected:
    static constexpr const char* kAgedBrie = "Aged Brie";
    static constexpr const char* kBackstagePass =
        "Backstage passes to a TAFKAL80ETC concert";
    static constexpr const char* kSulfuras = "Sulfuras, Hand of Ragnaros";
    static constexpr const char* kConjured = "Conjured Mana Cake";

    Item updateOne(const std::string& name, int sellIn, int quality);
};
```

## 4. 테스트 범위 및 우선순위

### P0: 핵심 도메인 불변식

가장 먼저 작성해야 하는 테스트다. 실패 시 품질 하한/상한 또는 전설 아이템 규칙이 깨질 수 있다.

- Sulfuras를 제외한 모든 아이템은 하루 후 `sellIn`이 1 감소한다.
- 감소형 아이템의 `quality`는 0 미만으로 내려가지 않는다.
- 증가형 아이템의 `quality`는 50을 초과하지 않는다.
- Sulfuras는 `sellIn`과 `quality`가 모두 변하지 않는다.
- Backstage passes는 콘서트 이후 `quality`가 0이 된다.

### P1: 아이템 타입별 정상/만료 동작

각 아이템 타입의 대표 규칙을 고정한다.

- Normal: 만료 전 `quality -1`, 만료 후 `quality -2`
- Aged Brie: 만료 전 `quality +1`, 만료 후 `quality +2`
- Backstage passes: `sellIn > 10`이면 `+1`, `10 >= sellIn > 5`이면 `+2`, `5 >= sellIn >= 0`이면 `+3`, 만료 후 `0`
- Conjured: 만료 전 `quality -2`, 만료 후 `quality -4`
- Sulfuras: 모든 경계에서 값 불변

### P2: 문자열 식별 및 특이 입력

아이템 타입이 `name` 문자열로 판별되므로 오분류 위험을 검증한다.

- `"Conjured Mana Cake"`가 Conjured로 처리되는지 검증한다.
- `"Not Conjured"`가 Conjured로 잘못 처리되지 않는지 검증한다.
- `"Sulfuras, Hand of Ragnaros"`의 정확한 이름만 전설 아이템으로 처리되는지 검토한다.
- Backstage passes의 긴 이름 오타를 방지하기 위해 테스트와 구현에서 상수를 공유하거나 중복을 최소화한다.

### P3: 통합성에 가까운 단위 케이스

여러 아이템이 같은 벡터에 있을 때 독립적으로 갱신되는지 확인한다.

- Normal, Aged Brie, Backstage passes, Sulfuras, Conjured를 하나의 벡터에 넣고 1일 업데이트 결과를 검증한다.
- 빈 `std::vector<Item>`에 대해 `updateQuality()`를 호출해도 예외 없이 종료되는지 검증한다.
- 2일 이상 연속 업데이트 시 경계 통과 결과가 올바른지 검증한다.

## 5. 경계값 케이스 목록

요청 경계값인 `quality = 0, 1, 49, 50`과 `sellIn = 0, -1`을 우선 조합한다. 모든 조합을 기계적으로 곱집합으로 작성하기보다, 비즈니스 규칙이 달라지는 지점 위주로 테스트한다.

### 5.1 Normal

| sellIn | quality | 기대 결과 |
|---:|---:|---|
| 0 | 0 | `sellIn = -1`, `quality = 0` 유지 |
| 0 | 1 | `sellIn = -1`, 만료 후 감소로 `quality = 0` |
| 0 | 49 | `sellIn = -1`, 만료 후 감소로 `quality = 47` |
| -1 | 1 | `sellIn = -2`, 이미 만료 상태이므로 `quality = 0` |

### 5.2 Aged Brie

| sellIn | quality | 기대 결과 |
|---:|---:|---|
| 0 | 49 | `sellIn = -1`, 만료 후 증가량 2가 적용되지만 `quality = 50` |
| 0 | 50 | `sellIn = -1`, `quality = 50` 유지 |
| -1 | 49 | `sellIn = -2`, `quality = 50` |
| -1 | 50 | `sellIn = -2`, `quality = 50` 유지 |

### 5.3 Backstage Passes

| sellIn | quality | 기대 결과 |
|---:|---:|---|
| 10 | 49 | `sellIn = 9`, 증가량 2가 적용되지만 `quality = 50` |
| 5 | 49 | `sellIn = 4`, 증가량 3이 적용되지만 `quality = 50` |
| 0 | 49 | `sellIn = -1`, 콘서트 이후 `quality = 0` |
| -1 | 50 | `sellIn = -2`, 이미 만료 상태이므로 `quality = 0` |

추가 대표 경계:

- `sellIn = 11`: 증가량 1 구간
- `sellIn = 10`: 증가량 2 구간 시작
- `sellIn = 6`: 증가량 2 구간
- `sellIn = 5`: 증가량 3 구간 시작
- `sellIn = 1`: 콘서트 직전 증가량 3 구간

### 5.4 Sulfuras

| sellIn | quality | 기대 결과 |
|---:|---:|---|
| 0 | 80 | `sellIn = 0`, `quality = 80` 유지 |
| -1 | 80 | `sellIn = -1`, `quality = 80` 유지 |

Sulfuras는 일반 `quality <= 50` 상한의 예외다.

### 5.5 Conjured

| sellIn | quality | 기대 결과 |
|---:|---:|---|
| 0 | 1 | `sellIn = -1`, 만료 후 감소량 4가 적용되지만 `quality = 0` |
| 0 | 49 | `sellIn = -1`, `quality = 45` |
| -1 | 1 | `sellIn = -2`, 이미 만료 상태이므로 `quality = 0` |
| -1 | 50 | `sellIn = -2`, `quality = 46` |

추가로 `sellIn > 0`, `quality = 1`인 경우 만료 전 감소량 2가 적용되어도 `quality = 0`으로 보정되는지 확인한다.

## 6. 예외 및 특이 케이스 목록

- 빈 아이템 목록: `items`가 비어 있어도 `updateQuality()`는 예외 없이 종료되어야 한다.
- 여러 아이템 동시 업데이트: 한 아이템의 갱신이 다른 아이템에 영향을 주지 않아야 한다.
- 이름이 유사한 아이템: `"Not Conjured"`처럼 `"Conjured"`를 포함하지만 접두어가 아닌 이름을 오분류하지 않아야 한다.
- 정확히 일치하지 않는 Sulfuras 이름: 오타가 있는 경우 전설 아이템으로 처리하지 않는 현재 문자열 기반 정책을 명확히 한다.
- `quality` 초기값이 50인 증가형 아이템: 증가량이 1보다 커도 50을 초과하지 않아야 한다.
- `quality` 초기값이 0 또는 1인 감소형 아이템: 감소량이 2 이상이어도 0 미만으로 내려가지 않아야 한다.
- `sellIn` 초기값이 0인 아이템: 하루 업데이트 후 `sellIn = -1`이 되며 만료 경계 규칙이 적용되는지 확인한다.
- `sellIn` 초기값이 -1인 아이템: 이미 만료된 상태로 간주해 만료 후 규칙을 적용한다.
- 다중 업데이트: `sellIn`이 양수에서 0, 음수로 넘어갈 때 규칙 전환이 올바른지 검증한다.

## 7. 권장 테스트 케이스 목록

### Normal

- `NormalItem_DecreasesSellInByOne`
- `NormalItem_DecreasesQualityByOneBeforeSellDate`
- `NormalItem_DecreasesQualityTwiceAsFastOnSellDate`
- `NormalItem_DecreasesQualityTwiceAsFastAfterSellDate`
- `NormalItem_NeverHasNegativeQuality`

### Aged Brie

- `AgedBrie_IncreasesQualityBeforeSellDate`
- `AgedBrie_IncreasesQualityTwiceAsFastOnSellDate`
- `AgedBrie_IncreasesQualityTwiceAsFastAfterSellDate`
- `AgedBrie_NeverExceedsQualityFifty`

### Backstage Passes

- `BackstagePass_IncreasesQualityByOneWhenSellInIsGreaterThanTen`
- `BackstagePass_IncreasesQualityByTwoWhenSellInIsTenOrLess`
- `BackstagePass_IncreasesQualityByThreeWhenSellInIsFiveOrLess`
- `BackstagePass_DropsQualityToZeroOnSellDate`
- `BackstagePass_DropsQualityToZeroAfterConcert`
- `BackstagePass_NeverExceedsQualityFiftyBeforeConcert`

### Sulfuras

- `Sulfuras_DoesNotChangeBeforeSellDate`
- `Sulfuras_DoesNotChangeOnSellDate`
- `Sulfuras_DoesNotChangeAfterSellDate`
- `Sulfuras_AllowsQualityEighty`

### Conjured

- `Conjured_DecreasesQualityByTwoBeforeSellDate`
- `Conjured_DecreasesQualityByFourOnSellDate`
- `Conjured_DecreasesQualityByFourAfterSellDate`
- `Conjured_NeverHasNegativeQuality`
- `Conjured_UsesPrefixClassificationForConjuredItems`
- `Conjured_DoesNotMatchNonPrefixNames`

### Collection and Regression

- `UpdateQuality_HandlesEmptyItemList`
- `UpdateQuality_UpdatesMultipleItemTypesIndependently`
- `UpdateQuality_AppliesRulesAcrossMultipleDays`

## 8. 커버리지 목표

권장 목표는 다음과 같다.

- 라인 커버리지: 90% 이상
- 브랜치 커버리지: 85% 이상
- 함수 커버리지: 100%
- 핵심 비즈니스 규칙 커버리지: 100%

단순 수치보다 중요한 기준은 모든 아이템 타입과 경계 규칙이 테스트로 표현되어 있는지다. 특히 현재 `GildedRose::updateQuality()`는 중첩 조건문이 많으므로 라인 커버리지 90%를 달성해도 브랜치 누락이 생길 수 있다. 따라서 lcov의 branch coverage를 함께 확인한다.

## 9. gcov/lcov 측정 전략

### 9.1 GCC 또는 Clang gcov 계열 빌드

gcov/lcov는 GCC 또는 gcov 호환 Clang 환경에서 사용하는 것을 권장한다. Windows에서 MSVC를 사용하는 경우 gcov/lcov 대신 OpenCppCoverage 같은 대안을 검토해야 한다.

권장 CMake 설정 예시는 다음과 같다.

```bash
cmake -S cpp -B build-coverage -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"
cmake --build build-coverage
ctest --test-dir build-coverage --output-on-failure
```

### 9.2 lcov 리포트 생성

```bash
lcov --capture --directory build-coverage --output-file coverage.info
lcov --remove coverage.info '/usr/*' '*/_deps/*' '*/test/*' --output-file coverage.filtered.info
genhtml coverage.filtered.info --output-directory coverage-report
```

측정 대상은 우선 `cpp/src/GildedRose.cpp`와 `cpp/include`의 도메인 코드로 제한한다. Google Test, FetchContent로 내려받은 외부 의존성, 테스트 파일은 커버리지 목표에서 제외한다.

### 9.3 개선 루프

1. P0 테스트를 먼저 작성하고 실패 원인을 확인한다.
2. 기존 레거시 동작과 요구사항이 다른 지점을 분리한다.
3. P1 아이템별 정상/만료 테스트를 추가한다.
4. lcov로 라인/브랜치 누락을 확인한다.
5. 누락된 분기가 실제 비즈니스 의미가 있으면 테스트를 추가한다.
6. 의미 없는 방어 분기나 중복 분기는 테스트를 억지로 추가하지 말고 리팩터링 후보로 기록한다.
7. Conjured 요구사항 테스트를 추가하고 구현 후 회귀 테스트 전체를 실행한다.

## 10. 진입 및 완료 기준

### 진입 기준

- 현재 빌드가 CMake로 재현 가능해야 한다.
- Google Test 실행 명령이 문서화되어 있어야 한다.
- 기존 실패 테스트가 있다면 실패 원인이 알려져 있어야 한다.

### 완료 기준

- P0, P1 테스트가 모두 통과한다.
- Conjured 요구사항 테스트가 모두 통과한다.
- `Item`의 public 구조가 변경되지 않았다.
- `sellIn` 음수 허용과 `quality` 범위 규칙이 검증되어 있다.
- 라인 커버리지 90% 이상, 브랜치 커버리지 85% 이상을 달성하거나 미달 사유가 문서화되어 있다.
- 커버리지 리포트에서 외부 의존성과 테스트 코드는 제외되어 있다.

## 11. 리스크 및 대응

- 현재 구현에는 Conjured 전용 분기가 없으므로 Conjured 테스트는 초기에는 실패할 가능성이 높다. 이는 신규 요구사항 구현을 위한 의도된 실패로 관리한다.
- 문자열 기반 타입 판별은 오타와 부분 문자열 매칭 리스크가 크다. 테스트에서 대표 이름과 오분류 케이스를 함께 고정한다.
- 중첩 조건문 구조는 브랜치 커버리지 누락을 만들기 쉽다. 리팩터링 전 충분한 특성화 테스트를 확보한다.
- Windows/MSVC 환경에서는 gcov/lcov 사용이 제한될 수 있다. MinGW, WSL, 또는 Clang/GCC 기반 CI 환경에서 커버리지를 측정하는 방안을 병행한다.
