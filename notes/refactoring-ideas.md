# Refactoring ideas

## High impact

### Memory management: 283 `new` vs 3 `delete`

`Xml` objects are allocated with `new` everywhere (txmathboot.cpp: 33, txmath.cpp: 51)
and stored as raw pointers. There's no clear ownership model. Migrating to
`std::unique_ptr<Xml>` or an arena allocator would prevent leaks and clarify lifetimes.

All `Xml` values are now heap-allocated (no value semantics remain). Move
ctor/assignment are deleted. This is a good starting point for introducing
`unique_ptr` or an arena.

### Monster functions

`txparser.cpp` has a ~1300-line function, `txfp.cpp` has a ~1000-line one,
`Hashtab_boot.cpp` is ~2000 lines of init code.

### ~~EndOfData as control flow~~ ✅ DONE

Eliminated. All 26 `throw EndOfData()` sites converted to bool returns. The
`class EndOfData` has been removed from `types.h`. XmlIO uses a local `Eof` struct.
9 `register_action_plain` registrations upgraded to `register_action`.

### ~~Xid indirection layer~~ ✅ DONE

Eliminated. `Xml::id` is now `size_t` (was `Xid {size_t value; Xml* xml}`).
All attribute operations go through `Xml*` directly (`->add_att()`, `->att`, etc.).
`Xid.h` and `Xid.cpp` deleted. `AttList::lookup` has const and non-const overloads.
`Stack::id_map` provides integer→`Xml*` lookup for TeX-level `\XMLaddatt` commands.

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

### Remove legacy subsystems

- Remove readline support.
- Remove txra and other ra-related parts.
- Investigate whether titlepage code can be simplified or removed.

### Stale TODOs/FIXMEs

~15 stale markers, including a confirmed bug comment in txparser2.cpp
(`"doc says true; FIXME"`).

### C-style casts

Only 7, mostly `long(x)` functional casts in math code.

### TODO backlog (2026-02-11)

Items below need deeper refactoring or design work; copied from TODO markers in src/.

- src/NameMapper.cpp
  - 241: without Buffer
  - 246: without Buffer

- src/txaccent.cpp
  - 5: The first 256 characters of Unicode are identical to Latin-1. So in

- src/LineList.cpp
  - 39: regexp?
  - 418: take a std::filesystem::path

- src/txio.cpp
  - 55: nullopt
  - 183: fs::path
  - 250: the next three function are kind of misleadingly named

- src/txparser.cpp
  - 1429: pass the array instead
  - 3674: pass the std::array instead

- src/BibEntry.cpp
  - 420: [[deprecated]]

- src/MathHelper.cpp
  - 11: elsewhere

- src/Hashtab_boot.cpp
  - 1387: useless alone?
  - 1388: useless alone?
  - 1401: useless alone?

- src/TexFonts.cpp
  - 13: Replace that with size_t or std::optional<size_t>

- src/Mactab.cpp
  - 21: shared_ptr or something RAII?

- src/txscan.cpp
  - 66: is that just std::filesystem::exists, or is the side-effect necessary?
  - 822: use optional better
  - 853: use optional better
  - 887: should ch be unsigned?
  - 1501: pass the array instead
  - 1506: std::optional<size_t>

- src/packages/tipa.cpp
  - 13: find a way to remove ipa_cmd from enums.h

- src/txparser3.cpp
  - 55: inline
  - 195: make Glue formattable

- src/txmathboot.cpp
  - 22: Why a global function?
  - 1253: big table of entities and codepoints
  - 1296: useless?
  - 1389: move to a more reasonable and robust place

- src/txmath.cpp
  - 343: why a separate global method?
  - 577: useless?
  - 996: move the logic to Symcode
  - 1021: put this into Symcode
  - 1076: put this into Symcode
  - 1088: put this into Symcode
  - 1619: this is weird, store a string instead of a type

- src/txerr.cpp
  - 18: without Buffer

- src/txtranslate.cpp
  - 1256: this is ugly
  - 1257: this is ugly
  - 1360: That is ugly
  - 1361: That is ugly
  - 1395: what is hanl?

- src/Dispatcher.cpp
  - 380: wrap the next ones in functions to fit on one line
  - 411: multilingual stuff somewhere

- src/txmath1.cpp
  - 849: why not go through all the machinery here?

- src/TokenList.cpp
  - 9: res.append()

- src/MainClass.cpp
  - 101: Manage this centrally
  - 112: this allows weird syntax
  - 151: this should be
  - 346: spdlog etc
  - 358: Link this with verbose (later in startup)
  - 382: this does not feel like it belongs here
  - 682: Erk this is not good
  - 732: fs::path
  - 963: make this always true

- src/txtrees.cpp
  - 19: there has to be a system library for that
  - 384: rewrite properly

- src/Bibtex.cpp
  - 26: without the_names?

- src/txcommands.cpp
  - 316: in the softcover fork, the next line becomes

- src/Buffer.cpp
  - 24: regexp
  - 883: regexp "\s*[aA][nN][dD]\s"

- src/txl3.cpp
  - 16
  - 539: if(check_declaration) check both args of
  - 556: if(check_declaration) check name is not undef_or_relax
  - 582: if(check_declaration) check all 3 arg are not undef_or_relax
  - 612: if(check_declaration) check name not undef_or_relax
  - 779: is offset always >=0?

- src/txtitlepage.cpp
  - 227: RA remove

- src/globals.cpp
  - 39: this should never happen but it does on linux+clang9

- src/tralics/CitationKey.h
  - 19: merge all these

- src/util.cpp
  - 43: similar to split_at_colon
  - 70: std::optional<std::string>

- src/tralics/LatexPackage.h
  - 5: std::unordered_map? order relevant?

- src/tralics/MainClass.h
  - 50: vector of std::fs::path
  - 64: one type to rule all encodings
  - 65: this should always be UTF-8
  - 66: this should always be UTF-8
  - 101: remove?
  - 109: static in MainClass.cpp

- src/Stack.cpp
  - 368: empty string or something

- src/XmlIO.cpp
  - 423: make codepoint formattable

- src/tralics/MathDataP.h
  - 80: unique instance, should we use static stuff?

- src/Xml.cpp
  - 272: errbuf??
  - 692: without scbuf
  - 882: std::optional<size_t>
  - 887: without Buffer

- src/tralics/ColSpec.h
  - 20: weird name

- src/tralics/WordList.h
  - 5: where is that used? TODO: chained list???

- src/txclasses.cpp
  - 77: make TokenList formattable
  - 249: std::filesystem
  - 530: fs::path
  - 1135: useless?

- src/txparser2.cpp
  - 15: c++ standard class for date?

- src/tralics/TitlePageFullLine.h
  - 4: do

- src/tralics/BibEntry.h
  - 17: derive from CitationKey?

- src/tralics/SaveAux.h
  - 8: this needs some overhaul...

- src/tralics/ConfigData.h
  - 16: make static?

- src/tralics/LineList.h
  - 5: use a vector instead

- src/tralics/TokenList.h
  - 15: rename

- src/tralics/NewArray.h
  - 32: always the_parser, remove

- src/tralics/AllIndex.h
  - 5: unordered_map perhaps

- src/tralics/Parser.h
  - 25: this should be broken up
  - 134: remove
  - 901: specific methods used in packages, belong there
  - 902: static methods that would fit better elsewhere

- src/tralics/Symcode.h
  - 17: add a map like name_sub

- src/tralics/TexFonts.h
  - 8: unsigned parameter

- src/tralics/Xml.h
  - 9: value semantics

- src/tralics/globals.h
  - 13: next are global functions, should we do something with them?
  - 52: return a pair?

- src/tralics/Dispatcher.h
  - 41: this method is too big, but easier to split up than translate03 was, do that

- src/tralics/ScaledInt.h
  - 10: This is a bit hackish, but it works (going through `long` fails).

- src/tralics/util.h
  - 18: std::chrono::year::is_leap

- src/tralics/OneIndex.h
  - 17: unordered_map

- src/tralics/AttList.h
  - 9: use map API

- src/tralics/Macro.h
  - 20: remove

- src/tralics/NameMapper.h
  - 8: This is work in progress. We need to be able to change XML output and so

- src/tralics/Buffer.h
  - 58: all those should be const
  - 68: [vb] sic
  - 73: refactor all that
  - 130: skip(const std::string&)
