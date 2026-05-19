# Test plan for C++ project
_Exported on 5/19/2026 at 10:21:32 GMT+9 from Cursor (3.4.20)_

---

**User**

@GildedRose.h @GildedRose.cpp @GildedRoseRequirements.txt @requirements_analysis.md

[P] 시니어 QA 리드입니다.
[C] C++17, Google Test, CMake, (가능하면 gcov/lcov)
[T] 테스트 계획서를 작성해줘.
    - TEST_F 기반 단위 테스트 범위/우선순위
    - 경계값 케이스 목록 (quality 0,1,49,50 / sellIn 0,-1)
    - 예외/특이 케이스 목록
    - 커버리지 목표 (예: 90%+)와 gcov/lcov 측정·개선 전략
[F] Markdown 문서. doc/test_plan.md로 저장


---

**Cursor**

요청하신 범위에 맞춰 기존 요구사항과 Gilded Rose 구현을 먼저 확인한 뒤, 테스트 계획서를 `doc/test_plan.md`에 Markdown으로 작성하겠습니다.

루트의 `requirements_analysis.md`는 현재 존재하지 않고, 같은 내용의 분석 문서는 `docs/requirements_analysis.md`에서 확인했습니다. 이제 실제 헤더/구현 파일 위치를 찾아 구현 구조까지 반영하겠습니다.

현재 구현은 `cpp/include`와 `cpp/src` 아래에 있으며, 기존 Google Test는 최소 스모크 테스트만 있는 상태입니다. 테스트 계획에는 이 레거시 구조와 신규 Conjured 요구사항의 우선순위를 분리해 반영하겠습니다.

작성 전 `doc` 폴더가 없는 것을 확인했으므로, 요청한 경로에 맞춰 폴더를 만든 뒤 계획서를 추가하겠습니다.

이제 계획서 본문을 추가합니다. 내용은 현재 `TEST` 1개 수준의 테스트 상태에서 `TEST_F` 기반 회귀/경계 테스트로 확장하는 실행 계획과, CMake 환경에서의 gcov/lcov 측정 절차를 포함합니다.

`doc/test_plan.md`로 테스트 계획서를 작성했습니다.

포함한 내용:
- `TEST_F` 기반 단위 테스트 구조와 우선순위
- `quality 0, 1, 49, 50` / `sellIn 0, -1` 경계값 케이스
- Conjured, Sulfuras, 문자열 식별 등 예외/특이 케이스
- 90%+ 커버리지 목표와 gcov/lcov 측정 및 개선 전략

`ReadLints` 확인 결과 문서 파일에 linter 오류는 없었습니다.

---

**User**

doc/* 파일을 docs 폴더에 옮긴다음 doc 폴더를 삭제해줘

---

**Cursor**

요청대로 `doc` 아래 문서를 `docs`로 옮기고, 이동 후 빈 `doc` 폴더를 제거하겠습니다. 먼저 대상 파일 충돌 여부를 확인하겠습니다.

`docs/test_plan.md`는 아직 없고, 이동 대상은 `doc/test_plan.md` 하나입니다. 파일 내용을 보존해 `docs`에 추가한 뒤 원본과 빈 폴더를 정리하겠습니다.

이동 명령은 완료됐습니다. 확인 중 `doc` 폴더가 사라진 것은 확인됐고, 대상 파일 경로를 한 번 더 확인하겠습니다.

대상 파일은 `docs/test_plan.md`에 생성됐지만 원본 `doc/test_plan.md`도 남아 있습니다. 원본 파일을 삭제하고 빈 폴더를 다시 제거하겠습니다.

완료했습니다. `doc/test_plan.md`를 `docs/test_plan.md`로 옮겼고, `doc` 폴더는 삭제된 상태를 확인했습니다.

---

**User**

Report 폴더에 보고서를 작성해줘

---

**Cursor**

`Report` 폴더의 기존 보고서 형식과 방금 작성한 테스트 계획서를 확인해서, 같은 톤의 테스트 계획 보고서로 추가하겠습니다.

기존 `Report` 파일들이 번호 기반 보고서명과 요약형 문체를 쓰고 있어, `04_TestPlan_보고서.md`로 테스트 계획 작성 보고서를 추가하겠습니다.

`Report/04_TestPlan_보고서.md`를 작성했습니다.

기존 보고서 형식에 맞춰 테스트 계획의 목적, 현재 테스트 상태, `TEST_F` 설계 방향, 경계값/예외 케이스, 커버리지 목표와 gcov/lcov 전략을 요약했습니다. `ReadLints` 확인 결과 오류는 없습니다.

