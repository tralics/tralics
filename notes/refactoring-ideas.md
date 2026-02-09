# Refactoring ideas

## High impact

### Memory management: 283 `new` vs 3 `delete`

`Xml` objects are allocated with `new` everywhere (txmathboot.cpp: 33, txmath.cpp: 51)
and wrapped in `gsl::not_null<Xml*>`. There's no clear ownership model. Migrating to
`std::unique_ptr<Xml>` or an arena allocator would prevent leaks and clarify lifetimes.

### Monster functions

`txparser.cpp` has a ~1300-line function, `txfp.cpp` has a ~1000-line one,
`Hashtab_boot.cpp` is ~2000 lines of init code.

### ~~EndOfData as control flow~~ ✅ DONE

Eliminated. All 26 `throw EndOfData()` sites converted to bool returns. The
`class EndOfData` has been removed from `types.h`. XmlIO uses a local `Eof` struct.
9 `register_action_plain` registrations upgraded to `register_action`.

## Medium impact

### `#define` macros → `constexpr`

String macros like `#define LANGLE "&#x02329;"` in txmathboot.cpp and case-list macros
in txmath1.cpp/txl3.cpp could become `constexpr string_view` or inline functions.

### `dynamic_cast` chains

SaveAux subclasses in txparser3.cpp use `dynamic_cast` downcasts instead of a
visitor/variant pattern.

### Signed/unsigned boilerplate

~100+ `to_signed()`/`to_unsigned()` calls for array indexing, indicating the index
types are mismatched throughout.

## Quick wins

### Stale TODOs/FIXMEs

~15 stale markers, including a confirmed bug comment in txparser2.cpp
(`"doc says true; FIXME"`).

### C-style casts

Only 7, mostly `long(x)` functional casts in math code.
