# Plan: eliminate the `String` type alias

## Problem

`String` is `using String = const char*` (types.h:7).  It is used ~230
times across ~50 source files.  The alias is dangerous because
`std::string(nullptr)` is UB, and the codebase already converts
`String` → `std::string` in `CmdChr::name()`.

## Replacement strategy

| old pattern | new type | when |
|---|---|---|
| return type that can be `nullptr` | `std::optional<std::string_view>` (or `std::optional<std::string>` if ownership needed) | indicates absence |
| return type that is always non-null | `std::string_view` | no optionality needed |
| parameter (never null, typically a literal) | `std::string_view` | zero-copy, accepts literals |
| member / array of compile-time literals | `std::string_view` | lightweight, constexpr-friendly |
| local variable | `auto` / `std::string` / `std::string_view` | follow the source |
| pointer arithmetic (`s + 3`) | `.substr(3)` | on string or string_view |

## Phased workplan

### Phase 1 — CmdChr name helpers ✅

Changed ~84 private `token_*_name()` / `l3_*_name()` helpers in CmdChr.h
from `-> String` to `-> std::optional<std::string_view>`.  Changed
`CmdChr::name()` to `-> std::optional<std::string>` with an `opt()` bridge.
Changed `special_name()` to `-> std::optional<std::string_view>`.  Updated
~12 call sites using `.value_or("")`.

Files: CmdChr.h, CmdChr_name.cpp, txtoken1.cpp, txfp.cpp, txmath.cpp,
txmath1.cpp, txparser3.cpp, Condition.cpp, txscan.cpp, txtranslate.cpp

### Phase 2 — other nullable return-type functions ✅

- `tralics_ns::math_env_name()` → `std::optional<std::string_view>` (globals.h, txmath.cpp)
- `Buffer::see_config_kw()` → `std::optional<std::string>` (Buffer.h, txtitlepage.cpp, LineList.cpp)
- Simplified CmdChr_name.cpp to use clean `strip_end()` / `opt()` calls

### Phase 3 — always-non-null return-type functions ✅

Changed 7 functions to `-> std::string_view`: `mode_to_string()`,
`boolean()`, `skip_or_continue()`, `to_string(save_type)`,
`gbl_or_assign()`, `dump_slot()`, `save_string_name()`.
Changed `pack_or_class()` inline in LatexPackage.h.
Removed dead declarations: `Token::to_string()`, `FontInfo::show_font()`.
Updated `trace_if()` parameters from `String` to `std::string_view`.

Files: Stack.cpp, txparser.cpp, txparser3.cpp, util.cpp, NewArray.cpp,
NewArray.h, util.h, LatexPackage.h, Token.h, FontInfo.h

### Phase 4 — function parameters (~73)

- [ ] 4a. Change `String` parameters to `std::string_view` across Parser.h,
      Buffer.h, Bibtex.h, MathDataP.h, FpGenList.h, etc.
      Do one header at a time to keep builds working.
- [ ] 4b. Inside function bodies, replace pointer arithmetic with
      `.substr()`, `strlen()` with `.size()`, `strcmp()` with `==`.
- [ ] 4c. At CLI/readline boundaries (readline.cpp, tralics.cpp,
      MainClass.cpp) keep `const char*` and convert as needed.
- [ ] 4d. Build & test after each header.

### Phase 5 — members, arrays, local variables (~40)

- [ ] 5a. Convert `std::array<String, N>` lookup tables to
      `std::array<std::string_view, N>`.
- [ ] 5b. Convert local `String` variables to `auto`, `std::string_view`,
      or `std::string` depending on context.
- [ ] 5c. Convert struct members (e.g. in Bibtex.h) to `std::string_view`
      or `std::string` depending on ownership.
- [ ] 5d. Build & test.

### Phase 6 — remove the alias

- [ ] 6a. Delete `using String = const char *;` from `types.h`.
- [ ] 6b. Full build — fix any stragglers.
- [ ] 6c. Final test run.

## Risks & notes

- **CLI boundaries** (readline.cpp, MainClass.cpp) should keep raw
  `const char*` — don't force `String` removal there.
- Each phase is independently shippable and testable.
- `CmdChr::name()` returns `std::optional<std::string>` (not `string_view`)
  because some branches call helpers returning `std::string` and callers
  need owned strings.
- `Buffer::see_config_kw()` returns `std::optional<std::string>` (not
  `string_view`) because it returns a pointer into its own mutable buffer.
- The `opt()` function in CmdChr_name.cpp bridges
  `optional<string_view>` → `optional<string>`.
