# Tralics

Tralics is a LaTeX to XML translator.

## Quick start

Build and run the default test target:

```
cmake -S . -B build -G Ninja
ninja -C build
```

Run the translator:

```
./build/tralics input.tex
```

## Homebrew (Tap)

A Homebrew formula is provided at `Formula/tralics.rb` for use in a tap
repository.

For a tap repository (for example `tralics/homebrew-tap`), users can install
with:

```
brew tap tralics/tap
brew install tralics
```

## Configuration

Default configuration files live in `confdir/`. Common file types include:

- `*.tcf`: Tralics configuration files
- `*.clt`: Tralics versions of class files
- `*.plt`: Tralics versions of package files
- `config_empty`, `tralics_rc`, `.tralics_rc`: baseline configuration files

## Tests

The default build target runs tests. Additional test inputs and fixtures live in
`test/`, including `test/alltests` for the legacy regression runner.

## Repository layout

- `src/`: core translator sources
- `confdir/`: configuration and package/class overrides
- `test/`: test inputs and regression data
- `docs/`: documentation sources
- `notes/`: developer notes (including architecture)
- `external/`, `extra/`: bundled external data and extras
- `build/`, `build-asan/`: build directories (generated)

## Architecture overview

Main classes and responsibilities:

- `MainClass`: top-level driver. Parses arguments, locates config and tcf files,
  reads inputs, determines document type/DTD, bootstraps resources, starts the
  translation, and writes XML output/logs.
- `Parser`: core translator and state machine. Holds macro tables, registers,
  catcodes, and scanning/expansion state. Executes translate loops that build XML.
- `Stack`: XML construction stack and mode tracker. Owns the current XML tree
  nodes, manages paragraph/mode transitions, and provides helpers for attributes.
- `Xml`: tree node (name + attributes + children) used for output structure.
- `Token` / `CmdChr`: TeX token encoding and derived (command, subtype) pairs.
- `Hashtab`: control-sequence hash that maps names to tokens and command meanings.
- `Mactab`: macro table with reference-counted macro bodies.
- `NameMapper` (`the_names`): string interning/remapping for XML names.
- `Symcode`: metadata for command codes and optional dispatch hooks.

Processing phases (simplified):

1. Startup/config: `MainClass` parses args, resolves paths, reads config, and
   initializes encodings.
2. Boot/initialization: `Parser` bootstraps defaults, primitives, macros, fonts,
   languages, and package helpers.
3. Translation: input lines become tokens; macros expand; commands execute; text
   accumulates into XML buffers and is flushed to the `Stack`.
4. XML assembly: `Stack` manages mode transitions and builds the XML tree.
5. Finalization/output: post-processing then XML/log output is written.

Registries and tables:

- EQTB tables: scoped TeX registers (ints, dimensions, glue, boxes, token lists,
  and strings), storing values with scoping levels.
- `Stack::id_map`: sparse id-to-XML lookup for references.

## License

Tralics is licensed under the CeCILL license. See `COPYING` and `COPYRIGHT` for
details.
