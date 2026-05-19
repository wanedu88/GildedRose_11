# Gilded Rose C++ requirements analysis
_Exported on 5/19/2026 at 09:51:15 GMT+9 from Cursor (3.4.20)_

---

**User**

@GildedRoseRequirements.txt @README.md 
[P] 시니어 C++ QA 엔지니어 관점에서
[C] Gilded Rose C++17 프로젝트 (CMake, Google Test)
[T] 요구사항을 C++ 구현 관점에서 재정리해줘.
    1) 아이템 타입별 비즈니스 규칙 표
       - Normal / Aged Brie / Backstage Pass / Sulfuras / Conjured
    2) 문자열 비교/분기 시 주의점 (std::string find/== 등)
    3) 예외/경계값 조건 (quality 0~50, sellIn 0/-1/음수, Sulfuras 예외)
    4) Conjured 신규 요구사항 (감소율 2배) 명세
    5) Google Test 기준 테스트 시나리오 목록 (번호)
[F] Markdown (표 + 번호 목록). docs 폴더를 만들고 requirements_analysis.md로 저장


---

**Cursor**

요구사항 파일과 README를 먼저 확인해서, 구현 관점의 규칙과 테스트 포인트를 현재 프로젝트 내용에 맞춰 정리하겠습니다. Ask mode라 파일 생성은 할 수 없지만, 바로 저장할 수 있는 Markdown 내용을 제공하겠습니다.

문서상 요구사항은 확인했습니다. C++ 구현 관점의 문자열 분기와 Google Test 시나리오를 더 정확히 잡기 위해 실제 헤더/구현/테스트 구조도 읽어보겠습니다.

Ask mode라 `docs/requirements_analysis.md` 파일 생성은 하지 못했습니다. 아래 내용을 그대로 저장하면 됩니다.

```markdown
# Gilded Rose 요구사항 분석

## 1. 아이템 타입별 비즈니스 규칙

| 타입 | 식별 기준 | SellIn 변화 | Quality 변화 | 경계/예외 |
|---|---|---:|---|---|
| Normal | 특수 아이템명이 아닌 일반 아이템 | 매일 -1 | 매일 -1, SellIn 만료 후 -2 | Quality는 0 미만 불가 |
| Aged Brie | `"Aged Brie"` | 매일 -1 | 매일 +1, SellIn 만료 후 +2 | Quality는 50 초과 불가 |
| Backstage Pass | `"Backstage passes to a TAFKAL80ETC concert"` | 매일 -1 | SellIn > 10: +1, 10 이하: +2, 5 이하: +3 | 콘서트 이후 Quality는 0, 50 초과 불가 |
| Sulfuras | `"Sulfuras, Hand of Ragnaros"` | 변하지 않음 | 변하지 않음 | Quality는 80으로 유지, 0~50 제한 예외 |
| Conjured | `"Conjured"` 계열 아이템 | 매일 -1 | Normal보다 2배 빠르게 감소 | Quality는 0 미만 불가 |

## 2. 문자열 비교/분기 시 주의점

1. 기존 C++ 구현은 `Item::name` 문자열로 아이템 타입을 구분한다.
2. `std::string ==` 비교는 대소문자, 공백, 쉼표까지 모두 정확히 일치해야 한다.
3. `"Sulfuras, Hand of Ragnaros"`처럼 쉼표와 공백이 포함된 이름은 오타가 있으면 일반 아이템으로 처리될 수 있다.
4. Backstage Pass의 전체 이름은 `"Backstage passes to a TAFKAL80ETC concert"`이므로 짧은 `"Backstage passes"` 비교와 혼용하지 않는 것이 좋다.
5. Conjured를 `name.find("Conjured") != std::string::npos`로 판별하면 `"Not Conjured"` 같은 이름도 잘못 매칭될 수 있다.
6. Conjured를 접두어 규칙으로 볼 경우 `name.rfind("Conjured", 0) == 0`처럼 문자열 시작 위치를 확인하는 방식이 더 명확하다.
7. 요구사항이 정확히 `"Conjured"`라는 단일 이름인지, `"Conjured Mana Cake"`처럼 접두어 카테고리인지 테스트 명세에서 고정해야 한다.
8. 문자열 분기가 늘어나면 중첩 `if`가 복잡해지므로 타입 판별 함수나 전략 분리도 고려할 수 있다.
9. 단, `Item` 클래스와 `items` 속성은 요구사항상 수정하지 않아야 한다.
10. 새로운 아이템 타입 추가 시 기존 특수 아이템 조건보다 먼저 또는 명확히 분리해서 분기해야 한다.

## 3. 예외/경계값 조건

1. 일반 아이템의 Quality는 매일 1 감소한다.
2. 일반 아이템의 SellIn이 지난 뒤에는 Quality가 하루에 2 감소한다.
3. Quality는 0 미만으로 내려가면 안 된다.
4. Quality는 50을 초과하면 안 된다.
5. Sulfuras는 Quality 80을 유지하며 0~50 제한의 예외다.
6. Sulfuras는 SellIn도 감소하지 않는다.
7. SellIn이 0인 아이템은 update 후 SellIn이 -1이 되며, 만료 후 규칙이 적용되어야 한다.
8. SellIn이 -1 또는 그보다 작은 음수인 아이템은 이미 만료된 상태로 처리한다.
9. Aged Brie는 만료 전 +1, 만료 후 +2 증가하지만 50을 넘지 않는다.
10. Backstage Pass는 SellIn 10 이하에서 +2, 5 이하에서 +3 증가한다.
11. Backstage Pass는 SellIn이 지난 후 Quality가 즉시 0이 된다.
12. Quality가 0인 감소형 아이템은 더 이상 감소하지 않는다.
13. Quality가 49 또는 50인 증가형 아이템은 50을 초과하지 않도록 보정해야 한다.
14. Conjured도 감소형 아이템이므로 Quality 0 하한을 반드시 지켜야 한다.

## 4. Conjured 신규 요구사항

1. Conjured 아이템은 일반 아이템보다 Quality가 2배 빠르게 감소한다.
2. 만료 전 Conjured는 하루에 Quality가 2 감소한다.
3. SellIn이 지난 Conjured는 일반 만료 아이템 감소율의 2배로 하루에 Quality가 4 감소한다.
4. Conjured의 SellIn은 일반 아이템처럼 매일 1 감소한다.
5. Conjured의 Quality는 0 미만으로 내려가지 않는다.
6. Conjured는 Quality 50 상한을 직접 증가시키는 타입은 아니지만, 전체 불변식상 0~50 범위 내에 있어야 한다.
7. Conjured 판별 기준은 테스트에서 명확히 해야 한다. 예: `"Conjured Mana Cake"`를 Conjured로 인정할지 여부.

## 5. Google Test 기준 테스트 시나리오 목록

1. Normal 아이템은 하루 후 SellIn이 1 감소한다.
2. Normal 아이템은 하루 후 Quality가 1 감소한다.
3. Normal 아이템은 SellIn 만료 후 Quality가 2 감소한다.
4. Normal 아이템의 Quality는 0 미만으로 내려가지 않는다.
5. Aged Brie는 하루 후 Quality가 1 증가한다.
6. Aged Brie는 SellIn 만료 후 Quality가 2 증가한다.
7. Aged Brie의 Quality는 50을 초과하지 않는다.
8. Backstage Pass는 SellIn 11 이상에서 Quality가 1 증가한다.
9. Backstage Pass는 SellIn 10에서 Quality가 2 증가한다.
10. Backstage Pass는 SellIn 5에서 Quality가 3 증가한다.
11. Backstage Pass의 Quality는 50을 초과하지 않는다.
12. Backstage Pass는 콘서트 이후 Quality가 0이 된다.
13. Sulfuras는 하루 후 SellIn이 변하지 않는다.
14. Sulfuras는 하루 후 Quality가 80으로 유지된다.
15. Conjured는 만료 전 Quality가 2 감소한다.
16. Conjured는 만료 후 Quality가 4 감소한다.
17. Conjured의 Quality는 0 미만으로 내려가지 않는다.
18. 여러 아이템을 한 번에 업데이트해도 각 아이템 규칙이 독립적으로 적용된다.
19. SellIn이 0인 아이템은 update 후 만료 상태 규칙이 반영된다.
20. SellIn이 이미 음수인 아이템은 만료 후 규칙으로 처리된다.
```

참고로 현재 구현은 `cpp/src/GildedRose.cpp`에서 문자열 완전 일치 기반으로 분기하며, 아직 Conjured 처리는 별도로 없습니다.

---

**User**

@GildedRoseRequirements.txt @README.md

[P] 시니어 C++ QA 엔지니어 관점에서
[C] Gilded Rose C++17 프로젝트 (CMake, Google Test)
[T] 요구사항을 C++ 구현 관점에서 재정리해줘.
    1) 아이템 타입별 비즈니스 규칙 표
       - Normal / Aged Brie / Backstage Pass / Sulfuras / Conjured
    2) 문자열 비교/분기 시 주의점 (std::string find/== 등)
    3) 예외/경계값 조건 (quality 0~50, sellIn 0/-1/음수, Sulfuras 예외)
    4) Conjured 신규 요구사항 (감소율 2배) 명세
    5) Google Test 기준 테스트 시나리오 목록 (번호)
[F] Markdown (표 + 번호 목록). requirements_analysis.md로 저장


---

**Cursor**

Agent mode로 전환되었으니 요청하신 Markdown 문서를 실제 파일로 저장하겠습니다. 먼저 같은 이름의 파일이 이미 있는지 확인한 뒤, 프로젝트 요구사항과 README 기준으로 `requirements_analysis.md`를 작성하겠습니다.

동일 파일은 없어서 새로 생성하겠습니다. 내용은 QA 관점에서 구현 규칙, 문자열 분기 리스크, 경계값, Conjured 명세, Google Test 시나리오 순서로 구성하겠습니다.

문서를 생성했습니다. 마지막으로 생성된 Markdown 파일에 IDE 진단이 있는지 확인하겠습니다.

`requirements_analysis.md`로 저장했습니다.

내용에는 요청하신 5개 항목을 모두 포함했습니다: 아이템 타입별 규칙 표, `std::string` 비교/분기 주의점, Quality/SellIn 경계값, Conjured 감소율 2배 명세, Google Test 기준 테스트 시나리오 번호 목록.

검증: `requirements_analysis.md`에 대한 IDE 진단은 없습니다.

---

**User**

Report 폴더에 보고서 써줘

---

**Cursor**

Report 폴더의 기존 문서 형식에 맞춰, 앞서 만든 요구사항 분석 내용을 보고서 형태로 정리해 저장하겠습니다. 먼저 폴더 구성과 기존 보고서 스타일을 확인하겠습니다.

기존 보고서는 목적, 기술 스택, 핵심 규칙, 기대 효과, 산출물 흐름으로 작성되어 있습니다. 같은 톤으로 `Report/02_RequirementsAnalysis_보고서.md`를 새로 만들겠습니다.

보고서 파일을 생성했습니다. Markdown 진단 문제가 없는지 확인하고 마무리하겠습니다.

`Report/02_RequirementsAnalysis_보고서.md`를 생성했습니다.

기존 `Report/01_CursorRules_설계보고서.md`와 비슷한 보고서 형식으로 작성했고, 요구사항 분석표, C++ 문자열 분기 주의점, 경계값, Conjured 명세, Google Test 시나리오를 포함했습니다.

검증: 새 보고서 파일에 IDE 진단은 없습니다.

