# Gilded Rose 요구사항 분석 보고서

## 1. 작성 목적

본 보고서는 Gilded Rose C++17 프로젝트의 요구사항을 구현 및 테스트 관점에서 재정리한 내용을 기록한다.

Gilded Rose는 아이템 이름에 따라 `sellIn`과 `quality` 변화 규칙이 달라지는 레거시 코드 카타이다. 특히 `Aged Brie`, `Backstage Pass`, `Sulfuras`, `Conjured`는 일반 아이템과 다른 예외 규칙을 가지므로, 구현 전에 비즈니스 규칙과 경계값을 명확히 정의하는 것이 중요하다.

## 2. 분석 기준

분석은 다음 문서를 기준으로 수행했다.

- `GildedRoseRequirements.txt`
- `README.md`

프로젝트의 구현 환경은 다음과 같다.

- C++17
- CMake
- Google Test
- `std::vector<Item>` 기반 아이템 목록
- `std::string name` 기반 아이템 타입 판별

## 3. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | 구현상 식별 기준 | SellIn 변화 | Quality 변화 | 주요 예외 |
|---|---|---:|---|---|
| Normal | 특수 아이템명이 아닌 경우 | 매일 -1 | 만료 전 -1, 만료 후 -2 | Quality는 0 미만 불가 |
| Aged Brie | `name == "Aged Brie"` | 매일 -1 | 만료 전 +1, 만료 후 +2 | Quality는 50 초과 불가 |
| Backstage Pass | `name == "Backstage passes to a TAFKAL80ETC concert"` | 매일 -1 | 11일 이상 +1, 10일 이하 +2, 5일 이하 +3 | 콘서트 이후 Quality는 0 |
| Sulfuras | `name == "Sulfuras, Hand of Ragnaros"` | 변하지 않음 | 변하지 않음 | Quality 80 유지, 0~50 제한 예외 |
| Conjured | `"Conjured"` 또는 `"Conjured ..."` 계열 | 매일 -1 | 만료 전 -2, 만료 후 -4 | Normal보다 2배 빠르게 감소 |

## 4. C++ 구현 관점의 주의사항

현재 구조에서는 `Item`에 타입 필드가 없으므로 `name` 문자열을 기준으로 아이템 타입을 판별해야 한다. 이 방식은 간단하지만 문자열 오타, 공백, 대소문자, 쉼표 누락에 취약하다.

`std::string ==` 비교는 문자열 전체가 정확히 일치해야 하므로 `"Sulfuras, Hand of Ragnaros"`나 `"Backstage passes to a TAFKAL80ETC concert"`처럼 긴 이름은 상수화하는 것이 좋다.

`std::string::find`를 사용할 때는 부분 문자열 매칭에 주의해야 한다. 예를 들어 `name.find("Conjured") != std::string::npos`는 `"Not Conjured"`도 Conjured로 잘못 분류할 수 있다. Conjured를 접두어 규칙으로 처리한다면 `name.rfind("Conjured", 0) == 0`처럼 시작 위치를 확인하는 방식이 더 안전하다.

또한 `Item` 클래스와 `items` 속성은 요구사항상 수정하지 않아야 한다. 따라서 타입 필드를 추가하기보다는 타입 판별 함수, 문자열 상수, 품질 보정 함수 등을 활용해 기존 제약 안에서 구현하는 것이 적절하다.

## 5. 예외 및 경계값 조건

1. `quality`는 `Sulfuras`를 제외하고 항상 0 이상 50 이하를 유지해야 한다.
2. `quality == 0`인 감소형 아이템은 더 이상 감소하지 않아야 한다.
3. `quality == 49` 또는 `quality == 50`인 증가형 아이템은 증가량이 2 이상이어도 50을 초과하지 않아야 한다.
4. `sellIn == 0`은 만료 경계값이므로 update 이후 만료 후 규칙 적용 여부를 명확히 검증해야 한다.
5. `sellIn == -1`은 이미 만료된 상태이므로 만료 후 규칙을 적용해야 한다.
6. `sellIn`이 -2 이하인 음수여도 만료 후 규칙은 동일하게 적용되어야 한다.
7. `Sulfuras`는 `sellIn`과 `quality`가 모두 변하지 않아야 한다.
8. `Sulfuras`의 `quality`는 80이며, 일반적인 0~50 제한의 예외다.
9. Backstage Pass는 콘서트가 지난 뒤 `quality`가 0이 되어야 한다.
10. Conjured는 감소량이 커서 0 미만으로 내려가기 쉬우므로 하한 보정 테스트가 필요하다.

## 6. Conjured 신규 요구사항

Conjured 아이템은 신규 공급업체 추가로 도입되는 요구사항이며, 핵심 규칙은 Normal 아이템보다 Quality가 2배 빠르게 감소한다는 점이다.

1. 만료 전 Normal 아이템의 감소량은 1이고, Conjured의 감소량은 2다.
2. 만료 후 Normal 아이템의 감소량은 2이고, Conjured의 감소량은 4다.
3. Conjured의 `sellIn`은 일반 아이템처럼 매일 1 감소한다.
4. Conjured도 `quality` 하한 0을 반드시 지켜야 한다.
5. Conjured 판별 기준은 테스트에서 명확히 고정해야 한다.
6. `"Conjured Mana Cake"` 같은 접두어 사례와 `"Not Conjured"` 같은 오탐 사례를 함께 검증하는 것이 좋다.

## 7. Google Test 테스트 시나리오

1. Normal 아이템은 update 후 `sellIn`이 1 감소한다.
2. Normal 아이템은 만료 전 update 후 `quality`가 1 감소한다.
3. Normal 아이템은 `sellIn == 0`일 때 update 후 `quality`가 2 감소한다.
4. Normal 아이템은 `sellIn == -1`일 때 update 후 `quality`가 2 감소한다.
5. Normal 아이템은 `quality == 0`이면 update 후에도 0을 유지한다.
6. Aged Brie는 만료 전 update 후 `quality`가 1 증가한다.
7. Aged Brie는 `sellIn == 0`일 때 update 후 `quality`가 2 증가한다.
8. Aged Brie는 `quality == 50`이면 update 후에도 50을 유지한다.
9. Aged Brie는 `quality == 49`이고 만료 후 증가량이 2여도 50을 초과하지 않는다.
10. Backstage Pass는 `sellIn == 11`이면 update 후 `quality`가 1 증가한다.
11. Backstage Pass는 `sellIn == 10`이면 update 후 `quality`가 2 증가한다.
12. Backstage Pass는 `sellIn == 6`이면 update 후 `quality`가 2 증가한다.
13. Backstage Pass는 `sellIn == 5`이면 update 후 `quality`가 3 증가한다.
14. Backstage Pass는 `sellIn == 0`이면 update 후 `quality`가 0이 된다.
15. Backstage Pass는 `quality == 49`이고 증가량이 2 이상이어도 50을 초과하지 않는다.
16. Sulfuras는 update 후 `sellIn`이 변하지 않는다.
17. Sulfuras는 update 후 `quality`가 80으로 유지된다.
18. Sulfuras는 `sellIn`이 0 또는 음수여도 값이 변하지 않는다.
19. Conjured 아이템은 만료 전 update 후 `quality`가 2 감소한다.
20. Conjured 아이템은 `sellIn == 0`일 때 update 후 `quality`가 4 감소한다.
21. Conjured 아이템은 `sellIn == -1`일 때 update 후 `quality`가 4 감소한다.
22. Conjured 아이템은 감소량이 현재 `quality`보다 커도 0 미만으로 내려가지 않는다.
23. `"Conjured Mana Cake"`를 Conjured로 처리할지 검증한다.
24. `"Not Conjured"`가 Conjured로 잘못 처리되지 않는지 검증한다.
25. 여러 타입의 아이템이 같은 `std::vector<Item>`에 있을 때 각 규칙이 독립적으로 적용된다.
26. 빈 `std::vector<Item>`에 대해 `updateQuality()`를 호출해도 예외 없이 종료된다.
27. `updateQuality()`를 여러 번 호출했을 때 일 단위 규칙이 누적 적용된다.

## 8. 기대 효과

본 요구사항 분석을 통해 구현자는 아이템별 규칙과 경계값을 명확히 이해한 상태에서 코드를 수정할 수 있다. QA 관점에서는 테스트 케이스의 우선순위를 정하고, Conjured 추가로 인해 기존 아이템 동작이 깨지지 않는지 회귀 테스트를 설계할 수 있다.

특히 문자열 기반 분기, `quality` 상하한, `sellIn` 만료 경계, `Sulfuras` 예외는 결함이 발생하기 쉬운 지점이므로 테스트 작성 시 반드시 포함해야 한다.

## 9. 산출물

- 요구사항 분석 문서: `requirements_analysis.md`
- 보고서 파일: `Report/02_RequirementsAnalysis_보고서.md`
