# Cursor AI project rules
_Exported on 5/19/2026 at 09:42:46 GMT+9 from Cursor (3.4.20)_

---

**User**

[P] 당신은 레거시 코드 QA/리팩토링을 돕는 시니어 C++ 엔지니어입니다.
[C] Gilded Rose C++17 프로젝트에서 Cursor AI가 항상 따라야 하는 규칙을
    프로젝트 루트의 .cursorrules로 작성하려고 합니다.
[T] 아래 요구를 만족하는 .cursorrules 내용을 작성해줘.
    - 기술 스택: C++17 + CMake + Google Test (+ gcov/lcov)
    - 절대 규칙: Item 구조체 수정 금지, quality 0~50 (Sulfuras 제외), sellIn 음수 허용
    - 테스트 규칙: Given-When-Then, TEST_F, 경계값(quality 0/50, sellIn 0/-1) 포함
    - 리팩토링 규칙: 테스트 Green 상태에서만 진행, 매직 넘버 상수화 권장
[F] .cursorrules 파일에 그대로 붙여넣을 수 있는 텍스트


---

**Cursor**

요청하신 `.cursorrules` 작성 형식과 Cursor 규칙 파일 관례를 먼저 확인한 뒤, 그대로 붙여넣을 수 있는 한국어 규칙 텍스트로 정리하겠습니다.

아래 내용을 프로젝트 루트의 `.cursorrules`에 그대로 붙여넣으면 됩니다.

```text
# Gilded Rose C++17 Project Rules

You are a senior C++ engineer helping with QA, characterization testing, and safe refactoring of the Gilded Rose legacy codebase.

## Tech Stack

- Use C++17.
- Use CMake as the build system.
- Use Google Test for automated tests.
- Use gcov/lcov when measuring test coverage.
- Prefer simple, idiomatic C++ over clever abstractions.

## Absolute Domain Rules

- Never modify the `Item` struct.
- Do not change the public shape of `Item`: `name`, `sellIn`, and `quality` must remain compatible with existing behavior.
- `quality` must always stay between `0` and `50`, except for `Sulfuras`.
- `Sulfuras` quality does not change and may remain above `50`.
- `sellIn` is allowed to become negative.
- Do not clamp `sellIn` to `0`.
- Preserve existing Gilded Rose behavior unless a test explicitly defines a new desired behavior.

## Testing Rules

- Before refactoring legacy behavior, add or preserve characterization tests.
- Tests should follow Given-When-Then structure.
- Prefer `TEST_F` with clear fixtures for related item behavior.
- Test names must describe the business behavior, not implementation details.
- Include boundary value tests for:
  - `quality == 0`
  - `quality == 50`
  - `sellIn == 0`
  - `sellIn == -1`
- Cover normal items, Aged Brie, Sulfuras, Backstage passes, and Conjured items when applicable.
- Tests should assert both `quality` and `sellIn` when behavior depends on both.
- Keep tests deterministic and independent.

## Refactoring Rules

- Refactor only when all tests are Green.
- After each meaningful refactoring step, run the relevant tests.
- Do not combine behavior changes with refactoring.
- Prefer small, reversible refactoring steps.
- Replace magic numbers with named constants when it improves readability.
- Avoid speculative abstractions.
- Keep business rules explicit and easy to verify against tests.
- Preserve public APIs unless the user explicitly asks to change them.

## Code Style

- Write readable C++17.
- Prefer `const` where appropriate.
- Prefer named helper functions only when they clarify business rules.
- Avoid unnecessary inheritance or complex design patterns.
- Keep conditionals understandable for legacy maintenance.
- Comments should explain business intent or non-obvious legacy behavior, not restate the code.

## Workflow

- Start by understanding existing tests and behavior.
- Add missing boundary tests before changing production code.
- Run the test suite after edits.
- If coverage is requested, use gcov/lcov and report uncovered business cases.
- Never hide failing tests; explain the failure and fix the cause.
```

---

**User**

 .cursorrules 생성해줘

---

**Cursor**

프로젝트 루트에 `.cursorrules` 파일을 만들겠습니다. 먼저 기존 파일이 있는지 확인한 뒤, 없으면 방금 정리한 규칙 내용으로 생성하겠습니다.

기존 `.cursorrules`는 없어서 새 파일로 추가합니다. 요청하신 Gilded Rose C++17 QA/리팩토링 규칙을 그대로 넣겠습니다.

`.cursorrules` 파일을 프로젝트 루트에 생성했습니다.

경로: `C:\DEV\GildedRose_11\.cursorrules`

---

**User**

Report 폴더에 보고서를 작성해줘

---

**Cursor**

보고서의 주제가 명시되지 않았으니, 방금 생성한 `.cursorrules`의 목적과 핵심 규칙을 정리한 프로젝트 규칙 보고서로 작성하겠습니다. 먼저 `Report` 폴더 존재 여부와 현재 규칙 파일 내용을 확인하겠습니다.

`Report` 아래에는 아직 파일이 없어서 보고서 폴더를 만들고, `.cursorrules` 작성 결과를 설명하는 Markdown 보고서를 추가하겠습니다.

이제 `Report` 폴더에 규칙 작성 배경, 주요 내용, 적용 기대효과를 담은 보고서 파일을 생성합니다.

`Report` 폴더에 보고서를 작성했습니다.

생성된 파일: `Report/cursorrules_report.md`

보고서에는 `.cursorrules` 작성 목적, 기술 스택, 도메인 절대 규칙, 테스트 규칙, 리팩토링 규칙, 기대 효과를 정리했습니다. Lint 오류도 없습니다.

