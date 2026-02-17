# Tralics Notes (Consolidated)

Last updated: 2026-02-11

## Architecture overview

### Main classes and responsibilities
- MainClass: top-level driver. Parses command line, locates config/tcf files,
  reads config/input LineLists, determines document type/DTD, bootstraps
  resources (bibtex, fonts, LaTeX), starts translation (trans0), and writes
  XML output/logs.
- Parser: core translator and state machine. Holds macro table (Mactab),
  EQTB tables for registers (ints/dims/glue/boxes/tokens/strings), catcodes,
  and scanning/expansion state. It reads tokens, expands macros, parses
  commands/arguments, and calls translate_all/translate03 loops that build XML.
- Stack: XML construction stack and mode tracker (v/h/math/array/bib/etc).
  Owns the current XML tree nodes, manages push/pop frames, ids, and paragraph
  transitions, and provides helpers for adding nodes/attributes.
- Xml: tree node (name + attributes + children) used for output structure.
- Token / CmdChr: Token is the compact TeX token encoding (char vs control
  sequence). CmdChr is the (command, subtype) pair derived from a Token.
- Hashtab: main control-sequence hash. Maps strings to Token ids and holds the
  eqtb of command meanings (Hashtab::the_eqtb). Also registers primitives.
- Mactab: macro table with reference-counted Macro bodies.
- NameMapper (the_names): string interning/remapping for XML element/attribute
  names (supports configuration-driven renaming).
- Symcode: metadata for symcodes (names and optional dispatch/action hooks).

### Phases of processing (simplified)
1) Startup/config:
   - MainClass parses arguments, determines input/config paths, reads config,
     resolves document type/DTD, and initializes encodings.
2) Boot/initialization:
   - Parser bootstraps EQTB defaults, macros, primitives (Hashtab),
     fonts/languages, and package setup (including LaTeX/latex3 helpers).
3) Translation:
   - LineList provides input lines. Parser scans into Token stream, expands
     macros, and executes commands. translate_all loops get_x_token/translate03
     and accumulates text into unprocessed_xml, then flushes to Stack.
4) XML tree assembly:
   - Stack manages paragraph/mode transitions and pushes Xml nodes, attributes,
     and anchors. Parser helpers convert TeX constructs to Xml nodes.
5) Finalization/output:
   - Parser final checks/post-processing (images, labels, math), then MainClass
     writes XML/log output.

### Hash tables / registries
- Hashtab: global control sequence dictionary + eqtb command map.
- Mactab: macro bodies with ref counts (expansion-safe).
- EQTB tables: scoped TeX registers (ints, dimensions, glue, boxes, token lists,
  and strings). EQTB entries store value + level for scoping.
- Stack id_map: sparse id->Xml* lookup for TeX-level commands referencing nodes.

### Enums and identifiers
- enums.h defines symcodes (command codes), subtypes (command variants), internal
  parameter types, math list types and math node types, boundary types, and
  catcodes. These enums drive parsing decisions and dispatch tables.

### Notes
- Most translation logic lives in tx*.cpp (e.g., txtranslate.cpp for the main
  translation loop), with package-specific behavior under src/packages/.

## Build & test reference
- `ninja -C build` runs and includes tests.

## Constraints and pitfalls

### What not to do (we tried it)
- A phase-2 session object with `extern &` globals caused a `std::out_of_range`
  abort during `testerr`.
- Root cause is likely static init order; avoided for now.

### String migration notes (char* -> std::string/string_view)
Hotspots where `String` is used as a nullable pointer or relies on pointer
arithmetic. These cannot be replaced by `std::string` without redesign
(use `std::optional<std::string_view>` or a separate flag).

- /Users/vincent/Prog/tralics/src/ScaledInt.cpp and /Users/vincent/Prog/tralics/src/Glue.cpp
  - arith_ns::start_err(String) treats nullptr as "no threshold".
- /Users/vincent/Prog/tralics/src/CmdChr_name.cpp
  - strip_end(String) returns nullptr and does s + 3.
  - CmdChr::name() (and the token_*_name() family) return nullptr for "no name".
- /Users/vincent/Prog/tralics/src/txmath.cpp
  - math_env_name() is checked for nullptr and uses S + 3.
- /Users/vincent/Prog/tralics/src/txscan.cpp
  - val.special_name() may return nullptr and has a fallback string.

Keep char* at boundaries (CLI/readline and mutable buffers):
- /Users/vincent/Prog/tralics/src/readline.cpp
- /Users/vincent/Prog/tralics/src/tralics.cpp
- /Users/vincent/Prog/tralics/src/MainClass.cpp

## Active refactor plans

### Plan: eliminate the String type alias

#### Problem
String is `using String = const char*` (types.h:7). It is used across the
codebase. The alias is dangerous because `std::string(nullptr)` is UB, and the
codebase already converts String -> std::string in CmdChr::name().

#### Replacement strategy

| old pattern | new type | when |
|---|---|---|
| return type that can be nullptr | std::optional<std::string_view> (or std::optional<std::string> if ownership needed) | indicates absence |
| return type that is always non-null | std::string_view | no optionality needed |
| parameter (never null, typically a literal) | std::string_view | zero-copy, accepts literals |
| member / array of compile-time literals | std::string_view | lightweight, constexpr-friendly |
| local variable | auto / std::string / std::string_view | follow the source |
| pointer arithmetic (s + 3) | .substr(3) | on string or string_view |

#### Remaining phases
- [ ] Phase 4 - function parameters (~73)
  - [ ] Change String parameters to std::string_view across Parser.h, Buffer.h,
        Bibtex.h, MathDataP.h, FpGenList.h, etc.
  - [ ] Inside function bodies, replace pointer arithmetic with .substr(),
        strlen() with .size(), strcmp() with ==.
  - [ ] At CLI/readline boundaries (readline.cpp, tralics.cpp, MainClass.cpp)
        keep const char* and convert as needed.
  - [ ] Build & test after each header.
- [ ] Phase 5 - members, arrays, local variables (~40)
  - [ ] Convert std::array<String, N> lookup tables to
        std::array<std::string_view, N>.
  - [ ] Convert local String variables to auto, std::string_view, or
        std::string depending on context.
  - [ ] Convert struct members (e.g. in Bibtex.h) to std::string_view or
        std::string depending on ownership.
  - [ ] Build & test.
- [ ] Phase 6 - remove the alias
  - [ ] Delete using String = const char *; from types.h.
  - [ ] Full build - fix any stragglers.
  - [ ] Final test run.

#### Risks & notes
- CLI boundaries (readline.cpp, MainClass.cpp) should keep raw const char*.
- CmdChr::name() returns std::optional<std::string> because some branches
  return std::string and callers need owned strings.
- Buffer::see_config_kw() returns std::optional<std::string> because it returns
  a pointer into its own mutable buffer.

### Plan: replace dispatch-only subtypes with string-keyed registry

#### Problem
enums.h is ~3000 lines with many dispatch-only subtypes. Every dispatch code is
maintained in three places: enums.h, Hashtab_boot.cpp, and txtoken1.cpp.

#### Key insight
The codes are just a bridge between identical strings. If we store the string
and generate codes at runtime, we eliminate enums.h entries, name functions,
and the enum maintenance burden.

#### What can be replaced vs what must stay
- Must stay numeric (arithmetic/indexing): catcode offsets, register offsets,
  token offsets, math table locations, or any code used in arithmetic/indexing.
- Can be replaced: codes used only in primitive("name", cmd, code), dispatch
  switch/if, and name mapping.

#### Architecture sketch
- CommandId: type-safe runtime identifier.
- CommandRegistry: single source of truth for name->id and id->name.
- Registration replaces three-place bookkeeping; dispatch compares CommandId
  values or uses an action map.

#### Migration strategy
- [ ] Phase 1 - infrastructure (CommandId/CommandRegistry, build & test)
- [ ] Phase 2 - migrate one command family (e.g. section_cmd)
- [ ] Phase 3 - migrate remaining families incrementally
- [ ] Phase 4 - cleanup (remove token_*_name helpers, shrink enums.h)

#### Risks
- Performance: switches are fast; if-chains on CommandId should be similar.
- Hidden arithmetic uses may block migration for some codes.

## Refactoring ideas (unplanned)

### High impact
- Memory management: Xml objects are allocated with new everywhere and stored as
  raw pointers. Consider std::unique_ptr<Xml> or an arena allocator.
- Monster functions: txparser.cpp (~1300 lines), txfp.cpp (~1000 lines),
  Hashtab_boot.cpp (~2000 lines).

### Medium impact
- #define macros -> constexpr string_view or inline functions.
- dynamic_cast chains in SaveAux subclasses (txparser3.cpp).
- Signed/unsigned boilerplate: widespread to_signed/to_unsigned calls.

### Quick wins
- Remove legacy subsystems (readline support, txra parts).
- Investigate whether titlepage code can be simplified or removed.
- C-style casts: only 7, mostly long(x) functional casts in math code.

## TODO backlog (2026-02-11)
Items below need deeper refactoring or design work; copied from TODO markers in src/.

- [x] src/NameMapper.cpp
  - 241: without Buffer (done 2026-02-15)
  - 246: without Buffer (done 2026-02-15)

- [x] src/txio.cpp
  - 55: nullopt (done 2026-02-15)

- src/BibEntry.cpp
  - 420: [[deprecated]]

- [x] src/MathHelper.cpp
  - 11: elsewhere (done 2026-02-15)

- src/Hashtab_boot.cpp
  - 1387: useless alone?
  - 1388: useless alone?
  - 1401: useless alone?

- [x] src/TexFonts.cpp
  - 13: Replace that with size_t or std::optional<size_t> (done 2026-02-15)

- src/Mactab.cpp
  - 21: shared_ptr or something RAII?

- src/packages/tipa.cpp
  - 13: find a way to remove ipa_cmd from enums.h

- src/txparser3.cpp
  - 55: inline
  - 195: make Glue formattable

- src/txmathboot.cpp
  - 1253: big table of entities and codepoints
  - 1296: useless?
  - 1389: move to a more reasonable and robust place

- src/txmath.cpp
  - 996: move the logic to Symcode
  - 1021: put this into Symcode
  - 1076: put this into Symcode
  - 1088: put this into Symcode
  - 1619: this is weird, store a string instead of a type

- src/txerr.cpp

- src/Dispatcher.cpp
  - 380: wrap the next ones in functions to fit on one line
  - 411: multilingual stuff somewhere

- src/txmath1.cpp
  - 849: why not go through all the machinery here?

- src/MainClass.cpp
  - 101: Manage this centrally
  - 112: this allows weird syntax
  - 151: this should be
  - 346: spdlog etc
  - 358: Link this with verbose (later in startup)
  - 382: this does not feel like it belongs here
  - 682: Erk this is not good
  - 963: make this always true

- src/txtrees.cpp
  - 19: there has to be a system library for that
  - 384: rewrite properly

- src/Bibtex.cpp
  - 26: without the_names?

- src/Buffer.cpp
  - 24: regexp (done 2026-02-17; converted/removed during cleanup)
  - 883: regexp "\\s*[aA][nN][dD]\\s"

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
  - next_label_id: moved into Parser state (done 2026-02-17)
  - 39: this should never happen but it does on linux+clang9

- src/tralics/CitationKey.h
  - 19: merge all these

- src/util.cpp
  - 70: std::optional<std::string>

- src/tralics/LatexPackage.h
  - 5: std::unordered_map? order relevant?

- src/tralics/MainClass.h
  - 50: vector of std::fs::path
  - 64: one type to rule all encodings
  - 65: this should always be UTF-8 (done 2026-02-17; output encoding removed)
  - 66: this should always be UTF-8 (done 2026-02-17; transcript encoding removed)
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
  - 10: This is a bit hackish, but it works (going through long fails).

- src/tralics/util.h
  - 18: std::chrono::year::is_leap

- src/tralics/OneIndex.h
  - 17: unordered_map

- src/tralics/Macro.h
  - 20: remove

- src/tralics/NameMapper.h
  - 8: This is work in progress. We need to be able to change XML output and so

- src/tralics/Buffer.h
  - 58: all those should be const
  - 68: [vb] sic
  - 73: refactor all that
  - 130: skip(const std::string&)

## Next suggested steps (state ownership migration)
- Continue moving remaining GlobalState members into owners:
  - everyjob_string, bad_chars -> likely Parser or MainClass
  - bib tracking (cur_entry_name, cur_entry_line, raw_bib, bib_allow_break)
    -> Bibtex/Bibliography
  - math tables (custom_table, math_chars, my_constant_table) -> math/encoding state
  - label/ref lists (ref_list, defined_labels, removed_labels, omitcite_list)
    -> Parser/Stack
- After more migration, reconsider a session object using accessor functions
  rather than extern & globals.
