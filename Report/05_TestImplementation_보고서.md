# Gilded Rose 테스트 구현 보고서

## 1. 작성 목적

본 보고서는 Gilded Rose C++17 프로젝트의 Google Test 기반 단위 테스트 구현 결과를 정리한다.

이전 테스트 계획에서 정의한 아이템 타입별 규칙, 경계값, 예외 조건을 `cpp/test/GildedRoseTest.cpp`에 `TEST_F` 형태로 구현하고, `cmake --build build` 및 `ctest` 실행 결과가 Green인지 확인했다.

## 2. 구현 대상

- 테스트 파일: `cpp/test/GildedRoseTest.cpp`
- 구현 파일: `cpp/src/GildedRose.cpp`
- 헤더 파일: `cpp/include/GildedRose.h`, `cpp/include/Item.h`
- 요구사항 기준: `docs/requirements_analysis.md`
- 테스트 프레임워크: Google Test
- 빌드 도구: CMake, Ninja, MinGW GCC

## 3. 테스트 구현 요약

기존 `GildedRoseTest.cpp`의 기본 샘플 테스트를 제거하고, `GildedRoseTest` fixture 기반 테스트로 재작성했다.

테스트는 모든 케이스에서 Given-When-Then 주석 구조를 사용했으며, `updateQuality()` 호출 후 `sellIn`과 `quality`를 `EXPECT_EQ` 또는 `ASSERT_EQ`로 함께 검증하도록 구성했다.

구현된 테스트 수는 총 27개다.

| 아이템 타입 | 테스트 수 | 주요 검증 내용 |
|---|---:|---|
| Normal | 5 | 만료 전 감소, `sellIn == 0`, `sellIn == -1`, `quality == 0` 하한 |
| Aged Brie | 5 | 만료 전 증가, 만료 후 증가량, `quality == 49/50` 상한 |
| Backstage Pass | 6 | `sellIn` 구간별 증가량, 콘서트 당일/이후 0 처리, 품질 상한 |
| Sulfuras | 5 | `sellIn` 불변, `quality == 80` 유지, 50 상한 예외 |
| Conjured | 6 | 만료 전 `-2`, 만료 후 `-4`, 품질 하한, 접두어 판별 |

## 4. 경계값 반영 결과

요구된 주요 경계값은 다음 테스트에 반영했다.

- `quality == 0`: Normal 감소형 아이템의 하한 유지 검증
- `quality == 1`: Normal 및 Conjured 감소량 적용 후 0 미만 방지 검증
- `quality == 49`: Aged Brie와 Backstage Pass 증가 후 50 초과 방지 검증
- `quality == 50`: Aged Brie 상한 유지 검증
- `sellIn == 0`: Normal, Aged Brie, Backstage Pass, Sulfuras, Conjured의 경계 규칙 검증
- `sellIn == -1`: 이미 만료된 상태의 타입별 규칙 검증

## 5. Conjured 구현 보강

기존 `GildedRose.cpp`에는 Conjured 아이템 전용 규칙이 없었으므로, 테스트를 Green으로 만들기 위해 구현도 함께 보강했다.

적용한 기준은 다음과 같다.

- `"Conjured"`로 시작하는 아이템을 Conjured 타입으로 판별한다.
- `"Conjured Mana Cake"`는 Conjured로 처리한다.
- `"Not Conjured"`는 Conjured로 오분류하지 않는다.
- 만료 전 `quality`는 2 감소한다.
- `sellIn <= 0` 상태에서는 `quality`가 4 감소한다.
- 감소 후 `quality`는 0 미만으로 내려가지 않는다.

또한 품질 보정 중복을 줄이기 위해 `increaseQuality()`와 `decreaseQuality()` 헬퍼를 추가하고, Sulfuras는 조기 분기로 `sellIn`과 `quality`를 보존하도록 정리했다.

## 6. 테스트 실행 결과

최종 검증은 `cpp` 디렉터리에서 다음 명령으로 수행했다.

```powershell
cmake --build build
ctest --test-dir build --output-on-failure
```

실행 결과는 다음과 같다.

- 전체 테스트: 27개
- 성공: 27개
- 실패: 0개
- 결과: 100% Green

추가로 `ReadLints`로 수정 파일의 진단을 확인했으며, `cpp/src/GildedRose.cpp`와 `cpp/test/GildedRoseTest.cpp` 모두 linter 오류가 없었다.

## 7. 품질 관점 평가

이번 테스트 구현으로 핵심 도메인 규칙이 타입별로 명시적으로 검증되며, 향후 리팩토링 시 회귀 결함을 빠르게 탐지할 수 있는 기반이 마련되었다.

특히 다음 리스크를 테스트로 보호한다.

- 만료 경계에서 감소량 또는 증가량이 잘못 적용되는 문제
- `quality`가 0 미만 또는 50 초과로 벗어나는 문제
- Sulfuras가 일반 아이템처럼 변경되는 문제
- Backstage Pass가 콘서트 이후 0으로 떨어지지 않는 문제
- Conjured가 Normal 아이템처럼 처리되거나 부분 문자열로 오분류되는 문제

## 8. 산출물

- 테스트 구현: `cpp/test/GildedRoseTest.cpp`
- 규칙 구현 보강: `cpp/src/GildedRose.cpp`
- 테스트 구현 보고서: `Report/05_TestImplementation_보고서.md`
