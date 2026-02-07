# Tralics Refactor Status

Date: 2026-02-07

## What was done

1) Consolidated globals into `GlobalState` and switched call sites to `global_state.<field>`.
   - Commit: `72c96cc` "Consolidate global state"

2) Moved runtime/path state into `MainClass`, and run-state flags/counters into `Parser`.
   - `MainClass`: `conf_path`, `input_path`, `file_name`, `file_list`
   - `Parser`: `seen_document`, `seen_enddocument`, `nb_errs`, `cur_file_line`, `cur_file_name`
   - Updated call sites to use `the_main.*` and `the_parser.*`.
   - Commit: `f6ad486` "Move runtime state into MainClass and Parser"

## Tests

- `ninja -C build` runs and includes tests.
- Warnings only: deprecated `EndOfData` in `XmlIO.cpp`, `txmath.cpp`, `txparser3.cpp`, `txtranslate.cpp`.

## What not to do (we tried it)

- A phase-2 session object with `extern &` globals caused a `std::out_of_range` abort during `testerr`.
- Root cause is likely static init order; avoided for now.

## String migration notes (char* -> std::string/string_view)

Hotspots where `String` is used as a nullable pointer or relies on pointer arithmetic. These cannot be replaced by `std::string` without redesign (use `std::optional<std::string_view>` or a separate flag).

- `/Users/vincent/Prog/tralics/src/ScaledInt.cpp` and `/Users/vincent/Prog/tralics/src/Glue.cpp`
  - `arith_ns::start_err(String)` treats `nullptr` as “no threshold”.
- `/Users/vincent/Prog/tralics/src/CmdChr_name.cpp`
  - `strip_end(String)` returns `nullptr` and does `s + 3`.
  - `CmdChr::name()` (and the `token_*_name()` family) return `nullptr` for “no name”.
- `/Users/vincent/Prog/tralics/src/txmath.cpp`
  - `math_env_name()` is checked for `nullptr` and uses `S + 3`.
- `/Users/vincent/Prog/tralics/src/txscan.cpp`
  - `val.special_name()` may return `nullptr` and has a fallback string.

Keep `char*` at boundaries (CLI/readline and mutable buffers):

- `/Users/vincent/Prog/tralics/src/readline.cpp`
- `/Users/vincent/Prog/tralics/src/tralics.cpp`
- `/Users/vincent/Prog/tralics/src/MainClass.cpp`

## Next suggested steps

- Continue moving remaining `GlobalState` members into owners:
  - `everyjob_string`, `bad_chars` -> likely `Parser` or `MainClass`
  - bib tracking (`cur_entry_name`, `cur_entry_line`, `raw_bib`, `bib_allow_break`) -> `Bibtex`/`Bibliography`
  - math tables (`custom_table`, `math_chars`, `my_constant_table`) -> math/encoding state
  - label/ref lists (`ref_list`, `defined_labels`, `removed_labels`, `omitcite_list`) -> `Parser`/`Stack`

- After more migration, reconsider a session object using accessor functions rather than `extern &` globals.
