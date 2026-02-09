# Plan: replace dispatch-only `subtypes` with string-keyed registry

## Problem

`enums.h` is 2949 lines with ~1660 dispatch-only `subtypes` values and
~530K arithmetic/indexing values.  Every dispatch code is defined in three
places:

1. **enums.h** — `part_code`, `number_code`, …
2. **Hashtab_boot.cpp** — `primitive("part", section_cmd, part_code)`
3. **txtoken1.cpp** — `case part_code: return "part"`

This triple-bookkeeping is error-prone and makes the codebase hard to
extend.  The enum values are arbitrary integers — the strings are what
matter.

## Key insight

For dispatch-only codes, the flow is always:

```
input string → hash_table.primitive(string, cmd, CODE) → eqtb stores CmdChr{cmd, CODE}
    → dispatch: switch(chr) { case CODE: … }
    → output: case CODE: return string
```

The CODE is just a bridge between identical strings.  If we store the
string directly and generate the CODE at runtime, we eliminate enums.h
entries, txtoken1.cpp name functions, and the enum maintenance burden.

## What can be replaced vs what must stay

### Must stay numeric (arithmetic/indexing)
- `cat_code_offset` through `other_int_offset` — character table indices
- `*_reg_offset` — register file indices
- `*_t_offset` (`letter_t_offset`, `space_t_offset`, …) — token offsets
- `math_*_loc` — math character table positions
- Any code used in `chr - X`, `chr + X`, `chr < X`, array indexing

### Can be replaced (~1400 values)
- All codes used only in `case X: do_something(); break` dispatch
- All codes used only in `primitive("name", cmd, X_code)` + `case X: return "name"`
- Condition: the code is never part of arithmetic, only compared with `==`

## Architecture

### 1. `CommandId` — a type-safe runtime identifier

```cpp
// Replaces subtypes for dispatch-only codes
class CommandId {
    unsigned value;  // runtime-generated sequential ID
    explicit CommandId(unsigned v) : value(v) {}
public:
    friend class CommandRegistry;
    bool operator==(CommandId o) const { return value == o.value; }
    // for use as hash key
    struct Hash { size_t operator()(CommandId id) const { return id.value; } };
};
```

### 2. `CommandRegistry` — the single source of truth

```cpp
class CommandRegistry {
    struct Entry {
        std::string                input_name;    // "part"
        std::string                output_name;   // "part" (customizable)
        symcodes                   cmd;           // section_cmd
        CommandId                  id;            // runtime-generated
    };

    std::vector<Entry>                                     entries;
    std::unordered_map<std::string_view, CommandId>        by_name;   // "part" → id
    std::unordered_map<unsigned, std::string_view>         id_to_name; // id → "part"

public:
    // Register a new command (replaces primitive + enum + name switch)
    auto register_command(std::string name, symcodes cmd) -> CommandId;

    // Lookup
    auto find(std::string_view name) -> std::optional<CommandId>;
    auto name(CommandId id) -> std::string_view;

    // Customization (gettext-style)
    void set_output_name(CommandId id, std::string custom_name);

    static auto instance() -> CommandRegistry&;
};
```

### 3. Registration replaces three-place bookkeeping

Before (three files):
```cpp
// enums.h
part_code, chapter_code, section_code, ...

// Hashtab_boot.cpp
primitive("part", section_cmd, part_code);
primitive("chapter", section_cmd, chapter_code);

// txtoken1.cpp
case part_code: return "part";
case chapter_code: return "chapter";
```

After (one place):
```cpp
// Hashtab_boot.cpp (or a new boot_commands.cpp)
auto part_id    = registry.register_command("part", section_cmd);
auto chapter_id = registry.register_command("chapter", section_cmd);
```

### 4. Dispatch replaces switch

Before:
```cpp
void Parser::T_paras(subtypes c) {
    switch (c) {
    case part_code: /* handle part */ break;
    case chapter_code: /* handle chapter */ break;
    ...
    }
}
```

After — **two options**:

**Option A: keep switch, but with runtime IDs**
```cpp
// In boot: store IDs in a struct or namespace
static CommandId part_id, chapter_id, ...;

void Parser::T_paras(CommandId c) {
    if (c == part_id) { /* handle part */ }
    else if (c == chapter_id) { /* handle chapter */ }
    ...
}
```

**Option B: action map (like Dispatcher::boot already does)**
```cpp
// In boot:
registry.register_action("part", section_cmd, [](auto&) { /* handle part */ });
registry.register_action("chapter", section_cmd, [](auto&) { /* handle chapter */ });
```

Option B is cleaner but a bigger change.  Option A is a smaller diff
since the dispatch logic stays in the same functions.

### 5. CmdChr evolution

`CmdChr` currently stores `{symcodes cmd, subtypes chr}`.  Two paths:

**Path 1 (minimal):** Keep `subtypes chr` but make `CommandId` implicitly
convertible to/from `subtypes`.  The eqtb table doesn't change.
Arithmetic codes keep their enum values.  Dispatch codes get
runtime-generated values that happen to be unsigned ints.  The enum
constants disappear from enums.h but the numeric type stays.

**Path 2 (cleaner):** Split `chr` into two: a `CommandId` for dispatch
and a `subtypes` for arithmetic codes.  This requires knowing at each
use site which kind it is.  More disruptive.

Path 1 is recommended.

### 6. Name output and customization

```cpp
// Default: returns the registration name
registry.name(part_id)  // → "part"

// Customization (gettext-like)
registry.set_output_name(part_id, "partie");  // French output
registry.name(part_id)  // → "partie"
```

This replaces the `token_*_name()` functions entirely: the registry
is the single source for both input parsing and output names.

## Migration strategy

### Phase 1 — Infrastructure
- [ ] Create `CommandId` class and `CommandRegistry`
- [ ] Add `register_command()` and lookup methods
- [ ] Make `CommandId` convertible to/from `subtypes` (bridge)
- [ ] Build & test (no behavioral change yet)

### Phase 2 — Migrate one command family
- [ ] Pick `section_cmd` as pilot (8 codes: `part_code` through `endsec_code`)
- [ ] Replace enum values with `registry.register_command()` calls
- [ ] Replace `case part_code:` with `if (c == part_id)` or equivalent
- [ ] Remove the 8 enum values from enums.h
- [ ] Remove the `token_section_name()` helper
- [ ] Build & test

### Phase 3 — Migrate remaining families one at a time
- [ ] `convert_cmd` family (~14 codes)
- [ ] `for_cmd` family (~12 codes)
- [ ] `cite_cmd` family (~4 codes)
- [ ] ... continue for all ~100 cmd families
- [ ] Each migration: remove enums, remove name helper, update dispatch

### Phase 4 — Clean up
- [ ] Remove empty `token_*_name()` functions
- [ ] Remove corresponding declarations from CmdChr.h
- [ ] Simplify `CmdChr::name()` — most cases now handled by registry
- [ ] enums.h shrinks to only arithmetic/indexing codes + symcodes

## Identifying which codes can be migrated

A code `X_code` can be migrated if it appears **only** in:
- `case X_code: return "...";` (name mapping)
- `case X_code: /* dispatch */` (action dispatch)
- `primitive("...", some_cmd, X_code)` (registration)
- `== X_code` or `!= X_code` (equality comparison)

A code **cannot** be migrated if it appears in:
- `chr - X_code`, `chr + X_code` (arithmetic)
- `chr < X_code`, `chr > X_code` (range comparison, unless purely for dispatch ordering)
- Array indexing: `table[X_code]`
- Bitfield encoding: `X_code * 4 + Y`

A script can classify all ~1660 codes automatically by grepping for
these patterns.

## Risks

- **Performance**: hash lookup vs switch.  Modern CPUs with branch
  prediction make switches very fast.  For name lookup (debug only),
  this doesn't matter.  For hot dispatch paths, if-chains with
  `CommandId` comparison should be similar — they're just integer `==`.
- **Arithmetic boundary**: some codes that look dispatch-only might
  have hidden arithmetic uses (e.g. `l3_p_code + 4`).  The
  classification script must catch these.
- **Size**: the registry stores strings — a few KB at most for ~1400
  entries.  Negligible.
- **Compilation**: removing 1400 enum values from enums.h will
  dramatically speed up incremental builds (fewer recompiles when
  enums.h changes).

## Example: section_cmd migration diff

```diff
 // enums.h
-    part_code = 0,
-    chapter_code,
-    section_code,
-    subsection_code,
-    subsubsection_code,
-    paragraph_code,
-    subparagraph_code,
-    endsec_code,

 // Hashtab_boot.cpp
-    primitive("part", section_cmd, part_code);
-    primitive("chapter", section_cmd, chapter_code);
+    ids.part    = reg.register_and_make_primitive("part", section_cmd);
+    ids.chapter = reg.register_and_make_primitive("chapter", section_cmd);

 // txtoken1.cpp
-auto CmdChr::token_section_name() const -> std::optional<std::string_view> {
-    switch (chr) {
-    case part_code: return "part";
-    case chapter_code: return "chapter";
-    ...
-    }
-}
+// Deleted — registry handles name lookup

 // CmdChr_name.cpp
-    case section_cmd: return opt(token_section_name());
+    case section_cmd: return std::string(registry.name(chr));

 // txtranslate.cpp
-    if (sectionning_offset == part_code) ...
+    if (sectionning_offset == ids.part) ...
```
