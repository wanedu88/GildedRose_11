# Gilded Rose 코드 품질 분석 보고서

## 분석 대상

- 파일: `cpp/src/GildedRose.cpp`, `cpp/include/GildedRose.h`
- 핵심 함수: `GildedRose::updateQuality()`
- 관점: SOLID, Code Smell, C++17 리팩토링 가능성

## 문제점 분석

| 문제점 | 위반 원칙/스멜 | 영향 | 개선 방향 | 우선순위 |
|---|---|---|---|---:|
| `updateQuality()`가 모든 아이템 타입의 규칙, 경계값 보정, `sellIn` 감소, 특수 예외 처리를 한 함수에서 모두 수행한다. | SRP 위반, Long Method | 변경 이유가 여러 개로 섞인다. 신규 아이템 추가, 품질 범위 변경, 만료 규칙 변경이 모두 같은 함수 수정을 요구한다. | 아이템 타입 판별, `sellIn` 갱신, `quality` 증감, 경계값 보정을 별도 함수 또는 전략 객체로 분리한다. | 1 |
| `"Aged Brie"`, `"Backstage passes to a TAFKAL80ETC concert"`, `"Sulfuras, Hand of Ragnaros"` 문자열 비교가 조건문 곳곳에 직접 노출되어 있다. | OCP 위반, Primitive Obsession, Duplicated Code | 새 아이템 타입을 추가할 때 기존 조건문을 계속 수정해야 한다. 문자열 오타가 컴파일 타임에 잡히지 않고, 분기 누락 가능성이 높다. | `constexpr std::string_view` 이름 상수와 `enum class ItemType` 분류 함수를 도입한다. 이후 타입별 처리 전략으로 연결한다. | 1 |
| 품질 하한 `0`, 상한 `50`, Backstage 임계값 `11`, `6`, 일일 변화량 `1` 등이 의미 없이 숫자로 직접 사용된다. | Magic Number | 도메인 규칙의 의미가 코드에서 드러나지 않는다. `sellIn < 11`이 실제로는 "10일 이하" 규칙임을 읽는 사람이 해석해야 한다. | `constexpr int MinQuality = 0`, `MaxQuality = 50`, `BackstageDoubleIncreaseThreshold = 10`, `BackstageTripleIncreaseThreshold = 5`, `DailySellInDelta = 1`처럼 도메인 언어로 상수화한다. 조건식은 `sellIn <= threshold` 형태로 맞춘다. | 2 |
| `quality < 50`, `quality > 0` 검사가 여러 곳에 반복되고, 증가/감소 연산도 중복된다. | Duplicated Code, Shotgun Surgery | 품질 경계 규칙이 바뀌면 여러 위치를 동시에 고쳐야 한다. 증가량이 2 이상일 때 상한을 넘지 않도록 보장하는 의도가 흩어진다. | `increaseQuality(Item&, int amount)`, `decreaseQuality(Item&, int amount)` 또는 `std::clamp` 기반 `adjustQuality()` 헬퍼로 경계값 보정을 한 곳에 모은다. | 2 |
| 중첩 `if`가 깊고 부정 조건이 연속된다. 예: 일반 아이템 여부를 `Aged Brie`와 `Backstage`가 아닌 경우로 판단하고, 내부에서 다시 `Sulfuras`를 제외한다. | 조건문 복잡도, Long Method, Negative Condition | 읽는 순서와 실제 비즈니스 규칙의 순서가 다르다. 특수 아이템 예외가 뒤늦게 섞여 버그가 숨어들기 쉽다. | `if (isSulfuras(item)) return;` 같은 조기 분기 또는 타입별 `switch`/전략 디스패치로 분기 깊이를 낮춘다. | 2 |
| `items[i]` 인덱싱이 함수 전체에 반복된다. | Readability Smell, Low-level Iteration | 같은 객체 접근이 장황하고, 규칙보다 컨테이너 접근 방식이 더 눈에 띈다. | C++17 범위 기반 반복문 `for (auto& item : items)`로 변경한다. | 3 |
| Backstage 규칙이 단계별 증가로 암묵적으로 표현된다. 기본 `+1` 후 `sellIn < 11`이면 추가 `+1`, `sellIn < 6`이면 추가 `+1`을 더한다. | Implicit Logic, Magic Number | 최종 증가량 규칙 `+1/+2/+3/0`이 코드에서 직접 보이지 않는다. 임계값 경계 해석 실수가 생기기 쉽다. | `backstageIncreaseFor(sellIn)` 함수를 두고 `sellIn <= 0` 이후 드롭 규칙과 `<= 5`, `<= 10` 증가량을 명시한다. 또는 테이블 기반 임계값 매핑을 사용한다. | 3 |
| 만료 후 규칙이 첫 번째 품질 처리 이후 별도 블록에서 다시 적용되어 흐름이 두 단계로 분산된다. | Temporal Coupling, Duplicated Conditional Logic | `sellIn` 감소 전후 중 어느 값을 기준으로 하는지 추적해야 한다. 신규 규칙 추가 시 만료 전/후 처리 위치를 잘못 선택할 위험이 있다. | 하루 업데이트 순서를 명시적으로 모델링한다. 예: 타입별 품질 갱신량 계산, `sellIn` 감소, 만료 후 보정이 필요한 타입의 정책 적용 순서를 함수명으로 드러낸다. | 3 |
| `Sulfuras` 예외 처리가 품질 감소 방지와 `sellIn` 감소 방지 위치에 각각 흩어져 있다. | SRP 위반, Duplicated Special Case | 전설 아이템의 "변하지 않는다"는 핵심 규칙이 한 곳에서 보장되지 않는다. | `SulfurasStrategy::update()`를 no-op으로 만들거나, 타입 분기 초기에 아무 작업 없이 반환한다. | 2 |
| `GildedRose` 클래스가 `std::vector<Item>& items`를 public으로 노출한다. | Encapsulation Violation | 외부 코드가 업데이트 규칙을 우회해 아이템 목록과 상태를 직접 변경할 수 있다. 테스트 편의성과 레거시 요구사항이 아니라면 불변식 유지가 어렵다. | 기존 kata 제약을 확인한 뒤 가능하면 private으로 내리고 필요한 조회 인터페이스를 제공한다. kata 제약상 공개 유지가 필요하면 리팩토링 범위 밖으로 둔다. | 5 |

## C++17 개선 방향 요약

1. **상수화부터 적용한다.** 아이템명과 도메인 숫자를 `constexpr std::string_view`, `constexpr int`로 정의하면 코드 의미가 즉시 좋아지고 테스트에도 같은 상수를 공유할 수 있다.
2. **타입 판별을 한 곳으로 모은다.** `enum class ItemType { Normal, AgedBrie, BackstagePass, Sulfuras, Conjured }`와 `classify(const Item&)`를 두면 문자열 비교가 `updateQuality()`에서 사라진다.
3. **품질 조정 헬퍼를 만든다.** `std::clamp`를 사용한 `adjustQuality(item, delta)` 또는 증가/감소 전용 함수를 두면 `0..50` 경계 규칙을 한 곳에서 보장할 수 있다. 단, `Sulfuras`의 품질 `80` 예외는 별도 정책으로 다룬다.
4. **전략 패턴을 우선 고려한다.** 아이템별 업데이트 규칙이 서로 다르므로 `NormalPolicy`, `AgedBriePolicy`, `BackstagePolicy`, `SulfurasPolicy`, `ConjuredPolicy`처럼 정책 객체 또는 함수 객체를 두면 OCP 위반을 줄일 수 있다.
5. **테이블 기반 방식은 Backstage 같은 구간 규칙에 적합하다.** 예를 들어 `{threshold: 5, increase: 3}`, `{threshold: 10, increase: 2}`, `{default: 1}`처럼 증가량 계산을 데이터로 표현할 수 있다.
6. **`std::variant`는 내부 도메인 모델을 새로 만들 수 있을 때 적합하다.** 현재 `Item` 구조를 변경하지 않는 kata 제약이 있다면 `variant`를 직접 저장하기보다, `Item`을 분류한 뒤 내부 처리 단계에서만 타입 안전한 정책 선택에 제한적으로 사용하는 편이 현실적이다.

## 리팩토링 우선순위

1. **아이템 타입 판별과 특수 문자열 상수화**: OCP 위반의 중심이며, 신규 요구사항인 Conjured 추가 시 가장 먼저 충돌하는 지점이다.
2. **품질 경계값 및 증감 헬퍼 추출**: 중복 제거 효과가 크고, `quality` 불변식을 한 곳에서 보장할 수 있다.
3. **`updateQuality()` 분기 구조 평탄화**: 조기 반환, 타입별 함수, `switch`를 통해 Long Method와 조건문 복잡도를 낮춘다.
4. **전략/정책 기반 구조 도입**: 아이템별 규칙을 독립 모듈로 분리해 SRP와 OCP를 동시에 개선한다. 테스트도 정책 단위로 세분화할 수 있다.
5. **캡슐화 개선 검토**: `items` public 노출은 구조적 문제지만 kata 제약 또는 기존 테스트와 충돌할 수 있으므로 후순위로 다룬다.
