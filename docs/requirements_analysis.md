# Gilded Rose 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 아이템 타입 | C++ 구현상 식별 기준 | SellIn 변화 | Quality 변화 | 예외 및 경계 |
|---|---|---:|---|---|
| Normal | 특수 아이템명에 해당하지 않는 일반 아이템 | 매일 -1 | 기본적으로 매일 -1, SellIn 만료 후 매일 -2 | Quality는 0 미만 불가, 50 초과 불가 |
| Aged Brie | `name == "Aged Brie"` | 매일 -1 | 기본적으로 매일 +1, SellIn 만료 후 매일 +2 | Quality는 50 초과 불가 |
| Backstage Pass | `name == "Backstage passes to a TAFKAL80ETC concert"` | 매일 -1 | SellIn > 10: +1, 10 >= SellIn > 5: +2, 5 >= SellIn >= 0: +3 | 콘서트 이후 Quality는 0, 증가 중에도 50 초과 불가 |
| Sulfuras | `name == "Sulfuras, Hand of Ragnaros"` | 변하지 않음 | 변하지 않음 | Quality 80 유지, 일반 Quality 0~50 제한의 예외 |
| Conjured | 요구사항상 `"Conjured"` 아이템 또는 `"Conjured ..."` 계열 아이템 | 매일 -1 | Normal보다 2배 빠르게 감소. 만료 전 -2, 만료 후 -4 | Quality는 0 미만 불가, Sulfuras 예외와 혼동 금지 |

## 2. 문자열 비교/분기 시 주의점

1. 현재 구조에서는 `Item`에 별도 타입 필드가 없으므로 `std::string name` 값으로 아이템 타입을 판별해야 한다.
2. `std::string`의 `==` 비교는 대소문자, 공백, 쉼표, 철자가 모두 정확히 일치해야 한다.
3. `"Sulfuras, Hand of Ragnaros"`는 쉼표와 공백이 포함되어 있으므로 오타가 있으면 legendary 아이템이 아닌 Normal 아이템처럼 처리될 수 있다.
4. `"Backstage passes to a TAFKAL80ETC concert"`는 긴 문자열이므로 상수화하지 않으면 테스트와 구현 사이에 오타가 생기기 쉽다.
5. `name.find("Conjured") != std::string::npos`는 문자열 어디든 `"Conjured"`가 포함되면 참이므로 `"Not Conjured"` 같은 이름도 잘못 분류할 수 있다.
6. Conjured를 접두어 카테고리로 해석한다면 `name.rfind("Conjured", 0) == 0`처럼 문자열 시작 위치를 확인하는 방식이 더 안전하다.
7. Conjured를 정확한 단일 이름으로 해석한다면 `name == "Conjured"`를 사용해야 한다.
8. 요구사항과 테스트에서 Conjured 판별 기준을 먼저 고정해야 한다. 예를 들어 `"Conjured Mana Cake"`를 Conjured로 볼지 여부를 명확히 해야 한다.
9. 문자열 리터럴이 여러 곳에 흩어지면 변경과 검증이 어려우므로 구현에서는 아이템명 상수 또는 타입 판별 함수를 두는 편이 좋다.
10. `Item` 클래스와 `items` 속성은 요구사항상 수정 금지 대상이므로, 타입 필드를 추가하는 방식은 피해야 한다.
11. 분기 순서상 Sulfuras는 Quality와 SellIn 모두 변하지 않아야 하므로 다른 감소/증가 로직보다 먼저 예외 처리하는 것이 안전하다.
12. Conjured는 감소형 아이템이지만 Normal과 감소량이 다르므로 Normal 기본 분기에 섞여 누락되지 않도록 별도 테스트가 필요하다.

## 3. 예외/경계값 조건

1. 모든 일반 아이템은 하루가 지나면 SellIn이 1 감소한다.
2. Sulfuras를 제외한 모든 아이템은 하루가 지나면 SellIn이 1 감소한다.
3. Normal 아이템은 SellIn 만료 전 Quality가 1 감소한다.
4. Normal 아이템은 SellIn 만료 후 Quality가 2 감소한다.
5. Conjured 아이템은 SellIn 만료 전 Quality가 2 감소한다.
6. Conjured 아이템은 SellIn 만료 후 Quality가 4 감소한다.
7. 감소형 아이템의 Quality는 0 미만으로 내려가면 안 된다.
8. Aged Brie는 SellIn 만료 전 Quality가 1 증가한다.
9. Aged Brie는 SellIn 만료 후 Quality가 2 증가한다.
10. Backstage Pass는 SellIn이 10보다 클 때 Quality가 1 증가한다.
11. Backstage Pass는 SellIn이 10 이하이고 5보다 클 때 Quality가 2 증가한다.
12. Backstage Pass는 SellIn이 5 이하이고 아직 콘서트 전 또는 당일이면 Quality가 3 증가한다.
13. Backstage Pass는 콘서트가 지난 뒤 Quality가 0이 된다.
14. 증가형 아이템의 Quality는 50을 초과하면 안 된다.
15. Sulfuras는 SellIn이 감소하지 않는다.
16. Sulfuras는 Quality가 감소하거나 증가하지 않는다.
17. Sulfuras의 Quality는 80이며, 일반적인 Quality 0~50 제한의 예외다.
18. SellIn이 0인 아이템은 update 이후 SellIn이 -1이 되며, 해당 일자 처리에서 만료 경계 조건을 주의해야 한다.
19. SellIn이 -1인 아이템은 이미 만료된 상태이므로 만료 후 규칙을 적용해야 한다.
20. SellIn이 -2 이하인 음수 아이템도 동일하게 만료 후 규칙을 적용해야 한다.
21. Quality가 0인 감소형 아이템은 추가 감소해도 0으로 유지되어야 한다.
22. Quality가 49 또는 50인 증가형 아이템은 증가량이 1보다 크더라도 50을 넘지 않아야 한다.

## 4. Conjured 신규 요구사항 명세

1. Conjured 아이템은 Normal 아이템보다 Quality가 2배 빠르게 감소한다.
2. Normal 아이템의 기본 감소량이 1이므로 Conjured의 기본 감소량은 2다.
3. Normal 아이템은 SellIn 만료 후 감소량이 2가 되므로 Conjured는 SellIn 만료 후 감소량이 4가 된다.
4. Conjured의 SellIn은 Sulfuras와 달리 매일 1 감소한다.
5. Conjured는 감소형 아이템이므로 Quality 하한 0을 반드시 지켜야 한다.
6. Conjured는 Quality를 증가시키지 않으므로 Quality 상한 50을 직접 초과시킬 가능성은 낮지만, 전체 도메인 규칙상 0~50 범위 안에서 취급해야 한다.
7. Conjured는 Sulfuras, Aged Brie, Backstage Pass와 독립된 아이템 타입으로 테스트해야 한다.
8. Conjured 판별 기준은 구현 전에 확정해야 한다. QA 관점에서는 `"Conjured Mana Cake"`처럼 접두어가 붙은 대표 케이스를 포함하는 테스트가 필요하다.
9. `std::string::find`로 Conjured를 판별하는 경우 의도하지 않은 부분 문자열 매칭을 테스트해야 한다.
10. 접두어 판별을 채택한다면 `"Conjured Mana Cake"`는 Conjured로 처리되고 `"Not Conjured"`는 Conjured가 아니어야 한다.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 update 후 SellIn이 1 감소한다.
2. Normal 아이템은 SellIn 만료 전 update 후 Quality가 1 감소한다.
3. Normal 아이템은 SellIn이 0일 때 update 후 만료 규칙으로 Quality가 2 감소한다.
4. Normal 아이템은 SellIn이 -1일 때 update 후 Quality가 2 감소한다.
5. Normal 아이템은 Quality가 0이면 update 후에도 0을 유지한다.
6. Aged Brie는 SellIn 만료 전 update 후 Quality가 1 증가한다.
7. Aged Brie는 SellIn이 0일 때 update 후 Quality가 2 증가한다.
8. Aged Brie는 SellIn이 -1일 때 update 후 Quality가 2 증가한다.
9. Aged Brie는 Quality가 50이면 update 후에도 50을 유지한다.
10. Aged Brie는 Quality가 49이고 만료 후 증가량이 2여도 50을 초과하지 않는다.
11. Backstage Pass는 SellIn이 11이면 update 후 Quality가 1 증가한다.
12. Backstage Pass는 SellIn이 10이면 update 후 Quality가 2 증가한다.
13. Backstage Pass는 SellIn이 6이면 update 후 Quality가 2 증가한다.
14. Backstage Pass는 SellIn이 5이면 update 후 Quality가 3 증가한다.
15. Backstage Pass는 SellIn이 1이면 update 후 Quality가 3 증가한다.
16. Backstage Pass는 SellIn이 0이면 update 후 Quality가 0이 된다.
17. Backstage Pass는 SellIn이 -1이면 update 후 Quality가 0이 된다.
18. Backstage Pass는 Quality가 49이고 증가량이 2 이상이어도 50을 초과하지 않는다.
19. Sulfuras는 update 후 SellIn이 변하지 않는다.
20. Sulfuras는 update 후 Quality가 80으로 유지된다.
21. Sulfuras는 SellIn이 0이어도 update 후 SellIn과 Quality가 변하지 않는다.
22. Sulfuras는 SellIn이 음수여도 update 후 SellIn과 Quality가 변하지 않는다.
23. Conjured 아이템은 SellIn 만료 전 update 후 Quality가 2 감소한다.
24. Conjured 아이템은 SellIn이 0일 때 update 후 Quality가 4 감소한다.
25. Conjured 아이템은 SellIn이 -1일 때 update 후 Quality가 4 감소한다.
26. Conjured 아이템은 Quality가 1이면 update 후 Quality가 0 미만으로 내려가지 않는다.
27. Conjured 아이템은 Quality가 3이고 만료 후 감소량이 4여도 Quality가 0으로 보정된다.
28. `"Conjured Mana Cake"`를 Conjured로 처리할지 검증한다.
29. `"Not Conjured"`가 Conjured로 잘못 처리되지 않는지 검증한다.
30. Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured가 같은 벡터에 있을 때 각 아이템 규칙이 독립적으로 적용된다.
31. 빈 `std::vector<Item>`에 대해 `updateQuality()`를 호출해도 예외 없이 종료된다.
32. 여러 번 `updateQuality()`를 호출했을 때 일 단위 규칙이 누적 적용된다.
