# Gilded Rose 테스트 계획 보고서

## 1. 작성 목적

본 보고서는 Gilded Rose C++17 프로젝트의 테스트 계획 수립 결과를 정리한다.

현재 프로젝트는 `GildedRose::updateQuality()`에 핵심 비즈니스 규칙이 집중되어 있으며, 아이템 이름 문자열에 따라 `sellIn`과 `quality` 변화 규칙이 달라진다. 따라서 리팩토링이나 신규 `"Conjured"` 요구사항 구현 전에 Google Test 기반의 회귀 테스트와 경계값 테스트를 먼저 확보하는 것이 중요하다.

## 2. 분석 및 계획 기준

테스트 계획은 다음 자료를 기준으로 작성했다.

- `GildedRoseRequirements.txt`
- `docs/requirements_analysis.md`
- `cpp/include/GildedRose.h`
- `cpp/include/Item.h`
- `cpp/src/GildedRose.cpp`
- `cpp/test/GildedRoseTest.cpp`

프로젝트의 주요 기술 스택은 다음과 같다.

- C++17
- CMake
- Google Test
- gcov/lcov

## 3. 현재 테스트 상태

현재 `cpp/test/GildedRoseTest.cpp`에는 기본 예제 수준의 테스트만 존재한다. 이 테스트는 실제 요구사항을 검증하기보다 `"fixme"` 값을 기대하는 형태이므로, 도메인 규칙 보호 수단으로는 부족하다.

따라서 테스트 계획에서는 기존 테스트를 `TEST_F` 기반의 의미 있는 단위 테스트로 재구성하고, 아이템 타입별 규칙과 경계값을 명시적으로 검증하도록 했다.

## 4. 테스트 설계 방향

테스트는 `GildedRose::updateQuality()`의 하루 단위 상태 변화를 중심으로 설계한다.

핵심 방향은 다음과 같다.

- `TEST_F` fixture를 사용해 공통 아이템명과 업데이트 헬퍼를 제공한다.
- Given-When-Then 구조로 테스트 의도를 명확히 한다.
- `sellIn`과 `quality`가 함께 바뀌는 경우 두 값을 모두 검증한다.
- 리팩토링 전에는 현재 동작을 특성화 테스트로 고정한다.
- 신규 Conjured 요구사항은 실패하는 테스트로 먼저 표현한 뒤 구현한다.

## 5. 테스트 우선순위

### P0: 핵심 불변식

가장 먼저 검증해야 하는 규칙은 품질 범위와 전설 아이템 예외다.

- Sulfuras를 제외한 모든 아이템의 `quality`는 0 미만이 되면 안 된다.
- 증가형 아이템의 `quality`는 50을 초과하면 안 된다.
- Sulfuras는 `sellIn`과 `quality`가 모두 변하지 않아야 한다.
- Backstage passes는 콘서트 이후 `quality`가 0이 되어야 한다.

### P1: 아이템 타입별 규칙

Normal, Aged Brie, Backstage passes, Sulfuras, Conjured 각각에 대해 만료 전과 만료 후 동작을 검증한다.

- Normal: 만료 전 `quality -1`, 만료 후 `quality -2`
- Aged Brie: 만료 전 `quality +1`, 만료 후 `quality +2`
- Backstage passes: 남은 일수 구간에 따라 `+1`, `+2`, `+3`, 콘서트 이후 `0`
- Sulfuras: 항상 값 불변
- Conjured: 만료 전 `quality -2`, 만료 후 `quality -4`

### P2: 문자열 식별 및 오분류 방지

현재 구조에서는 아이템 타입을 `name` 문자열로 판별하므로 문자열 오타와 부분 문자열 매칭 리스크가 있다.

특히 `"Conjured Mana Cake"`와 `"Not Conjured"`를 함께 테스트해 Conjured 판별 기준을 명확히 해야 한다.

## 6. 주요 경계값

테스트 계획에서는 다음 경계값을 우선 검증 대상으로 정의했다.

- `quality == 0`: 감소형 아이템이 음수가 되지 않는지 확인
- `quality == 1`: 감소량이 2 이상일 때 0으로 보정되는지 확인
- `quality == 49`: 증가량이 2 이상일 때 50을 초과하지 않는지 확인
- `quality == 50`: 증가형 아이템이 상한을 유지하는지 확인
- `sellIn == 0`: update 후 만료 경계 규칙이 적용되는지 확인
- `sellIn == -1`: 이미 만료된 상태의 규칙이 적용되는지 확인

이 경계값은 Normal, Aged Brie, Backstage passes, Sulfuras, Conjured에 각각 필요한 조합으로 적용한다.

## 7. 예외 및 특이 케이스

테스트 계획에는 다음 특이 케이스를 포함했다.

- 빈 `std::vector<Item>`에 대해 `updateQuality()`를 호출해도 예외 없이 종료되어야 한다.
- 여러 아이템이 같은 벡터에 있어도 각 아이템 규칙이 독립적으로 적용되어야 한다.
- Sulfuras는 일반 품질 상한 50의 예외로 `quality == 80`을 유지해야 한다.
- Backstage passes는 품질 증가 중에도 50을 초과하지 않아야 하며, 콘서트 이후에는 0이 되어야 한다.
- Conjured는 감소량이 크므로 `quality` 하한 보정 테스트가 반드시 필요하다.

## 8. 커버리지 목표

권장 커버리지 목표는 다음과 같다.

- 라인 커버리지: 90% 이상
- 브랜치 커버리지: 85% 이상
- 함수 커버리지: 100%
- 핵심 비즈니스 규칙 커버리지: 100%

현재 `updateQuality()`는 중첩 조건문이 많기 때문에 라인 커버리지만으로는 충분하지 않다. lcov의 branch coverage를 함께 확인해 아이템 타입별 분기와 경계 조건이 누락되지 않았는지 검증해야 한다.

## 9. gcov/lcov 측정 전략

커버리지는 GCC 또는 gcov 호환 Clang 환경에서 측정하는 것을 권장한다.

기본 흐름은 다음과 같다.

1. `--coverage -O0 -g` 옵션으로 coverage 빌드를 생성한다.
2. `ctest`로 Google Test 전체를 실행한다.
3. `lcov`로 커버리지 데이터를 수집한다.
4. 외부 의존성, Google Test, 테스트 파일을 제외한 뒤 리포트를 생성한다.
5. 누락된 라인과 브랜치가 실제 비즈니스 규칙이면 테스트를 추가한다.

Windows/MSVC 환경에서는 gcov/lcov 사용이 제한될 수 있으므로, MinGW, WSL, Clang/GCC 기반 CI 환경 또는 OpenCppCoverage 같은 대안을 검토한다.

## 10. 기대 효과

이번 테스트 계획을 적용하면 다음 효과를 기대할 수 있다.

- 레거시 동작을 테스트로 고정해 안전한 리팩토링 기반을 마련할 수 있다.
- Conjured 신규 요구사항을 기존 규칙과 충돌 없이 구현할 수 있다.
- 품질 상한/하한, 만료 경계, 전설 아이템 예외 같은 결함 발생 지점을 조기에 검출할 수 있다.
- 커버리지 수치를 통해 테스트 누락 영역을 객관적으로 확인할 수 있다.

## 11. 산출물

- 테스트 계획서: `docs/test_plan.md`
- 테스트 계획 보고서: `Report/04_TestPlan_보고서.md`
