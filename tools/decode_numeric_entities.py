#!/usr/bin/env python3
"""
Decode numeric XML entities in a file:
  - decimal: &#1234;
  - hex:     &#x1F4A9; / &#X1F4A9;

Other entities (like &amp;) are left untouched.
"""

from __future__ import annotations

import argparse
import pathlib
import re
import sys


ENTITY_RE = re.compile(r"&#(?:x([0-9A-Fa-f]+)|([0-9]+));")
XML_DECL_RE = re.compile(r"(<\?xml\s+version=['\"][^'\"]+['\"]\s+encoding=['\"])([^'\"]+)(['\"]\s*\?>)", re.IGNORECASE)


def decode_numeric_entities(text: str) -> str:
    def repl(match: re.Match[str]) -> str:
        hex_part = match.group(1)
        dec_part = match.group(2)
        try:
            value = int(hex_part, 16) if hex_part is not None else int(dec_part, 10)
            return chr(value)
        except (TypeError, ValueError, OverflowError):
            return match.group(0)

    return ENTITY_RE.sub(repl, text)


def force_utf8_decl(text: str) -> str:
    return XML_DECL_RE.sub(r"\1UTF-8\3", text, count=1)


def main() -> int:
    parser = argparse.ArgumentParser(description="Decode numeric XML entities to UTF-8 characters.")
    parser.add_argument("input", type=pathlib.Path, help="Input XML/text file")
    parser.add_argument(
        "-o",
        "--output",
        type=pathlib.Path,
        help="Output file (default: stdout)",
    )
    args = parser.parse_args()

    try:
        src = args.input.read_text(encoding="utf-8")
    except UnicodeDecodeError:
        src = args.input.read_text(encoding="latin-1")

    out = decode_numeric_entities(src)
    out = force_utf8_decl(out)

    if args.output is None:
        sys.stdout.write(out)
    else:
        args.output.write_text(out, encoding="utf-8")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
