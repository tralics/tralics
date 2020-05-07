==================================
Tralics: All commands, non-letters
==================================

This page contains the description characters of `catcode
0 <#catcode0>`__, `catcode 1 <#catcode1>`__, `catcode 2 <#catcode2>`__,
`catcode 3 <#catcode3>`__, `catcode 4 <#catcode4>`__, `catcode
5 <#catcode5>`__, `catcode 6 <#catcode6>`__, `catcode 7 <#catcode7>`__,
`catcode 8 <#catcode8>`__, `catcode 9 <#catcode9>`__, `catcode
10 <#catcode10>`__, `catcode 11 <#catcode11>`__, `catcode
12 <#catcode12>`__, `catcode 13 <#catcode13>`__, `catcode
14 <#catcode14>`__, `catcode 15 <#catcode15>`__, `catcode
16 <#catcode16>`__.

It contains also the description of `~ <#tilde>`__,
`\\~ <#cmd-tilde>`__, `\\$ <#cmd-dollar>`__, `\\% <#cmd-percent>`__,
`& <#cmd-amp>`__, `\\& <#cmd-amp>`__, `\\! <#cmd-exclam>`__,
`\\, <#cmd-comma>`__, `\\> <#cmd-greater>`__, `\\; <#cmd-semicolon>`__,
`\\{ <#cmd-openbrace>`__, `\\} <#cmd-closebrace>`__,
`\\( <#cmd-openparen>`__, `\\) <#cmd-closeparen>`__,
`\\\_ <#cmd-underscore>`__, `\\# <#cmd-sharp>`__, `\\\| <#cmd-bar>`__,
`\\\\ <#symbol-backslash>`__, `\| <#bar>`__, `[ \\[ <#openbra>`__, `]
\\] <#closebra>`__, `( <#cmd-openparen>`__, `) <#cmd-closeparen>`__,
`< <#ltsign>`__, `> <#gtsign>`__, `' <#quote>`__, `\\' <#cmd-quote>`__,
`\\" <#cmd-doublequote>`__, `\\= <#cmd-equal>`__, `\\. <#cmd-dot>`__,
`\\^ <#cmd-hat>`__, `\\\` <#cmd-grave>`__, `\\ <#cmd-space>`__ , `-
\\- <#cmd-minus>`__, `\\+ <#cmd-plus>`__, `\* <#cmd-star>`__,
`\\@ <#cmd-atsign>`__, `\\/ <#cmd-italiccor>`__.

--------------

The following definitions will be used here:

.. container:: ltx-source

   ::

      \def\testeq#1#2{\def\tmp{#2}\ifx#1\tmp\else \toks0={#1wantd: ->#2.}
      \typeout{\the\toks0}\show #1\uerror\fi}
      \def\xtesteq#1#2{\xdef\tmp{#2}\ifx#1\tmp\else \toks0={#1wantd: ->}
      \typeout{\the\toks0 #2.}\show #1\uerror\fi}

This can be used as

.. container:: ltx-source

   ::

      \def\foo{something} \testeq\foo{Something else}

and will produce no XML, but an error and two lines of message, like

.. container:: tty_out

   ::

      \foo wantd: ->Something else.
      \foo=macro: ->something.
      Error signaled at line 352:
      Undefined command \uerror.

Note the following points. Both ``\typeout`` and ``\show`` print the
value on the tty as well as the transcript file. The token list of
``\typeout`` is fully expanded, and for this reason, we put in a token
list the quantity ``#1wantd: ->#2.`` and inhibit expansion via ``\the``.

The symbols, by catcode
=======================

Unless stated otherwise, a category code (in short a catcode) is an
integer between 0 and 15. It is a property of input characters, used by
the scanner to convert the input stream into a sequence of tokens. The
character with ASCII code 37 is the character %, and is normally of
category 14, thus behaves like a start-of-comment. You can insert such a
character in the XML output via the sequence ``\char37`` (However
``\char60`` produces ``&lt;``, see the
```\char`` <doc-c.html#cmd-char>`__ command). In most of the cases, a
character of catcode *c* is read as a command with command code *c*.

*Tralics* uses the same parsing rules as TeX, with the exception of
characters of category 16 (see below), and the fact that the character
range is not limited to 8bit characters but to 16 bits. A construction
like ``^^^^^1d7e0`` provides a character without category code, see `the
documentation on characters <doc-chars.html>`__.

Catcode 0
---------

The only character with catcode 0 is the backslash. This character is
used to construct command names. The number 0 is not a command code. The
command name is created as follows: if the next character is not of
category 11, the name consists of the single character, and *Tralics*
goes into state *S* if the character is a space, state *M* otherwise; if
the character is of category 11, the name consists of all characters
category 11 starting with the current one, and *Tralics* goes into state
*S*. The first unused character will be read again later, and a category
code assigned to it. Normally, spaces after command names are ignored
because the current state *S* and the category code of the space
character is 10. But the command can change the category code of the
character that follows.

The following example shows how to change the ``\catcode`` of the letter
x, so that x can be used like a backslash. In the second definition,
there is no space between the digit zero and the letter x. This letter
is scanned, found to be a non digit, and pushed back in the main token
list; at this moment, the catcode is still unchanged, ``xfoo`` is
considered as a string of four letters, and not a command.

.. container:: ltx-source

   ::

         {\def\foo{\gdef\bar{OK}}  \catcode`x=0 xfoo}
         \testeq\bar{OK}
         {\def\foo{\gdef\bar{notOK}}  \catcode`x=0xfoo}
         \testeq\bar{OK}

The next example shows that the token after a control sequence like
``\foo`` can be of catcode letter (because when the character is read
again, its catcode is analysed again).

.. container:: ltx-source

   ::

      {\def\bar#1{\egroup\show#1}
      \def\foo{\bgroup\catcode32=11\catcode`\%=11 \bar}\foo \foo%\foo=\foo$\foo#}

In the *Tralics* output below, \`the letter' means a character of
category 11, \`the character' a character of category 12.

.. container:: tty_out

   ::

      the letter  .
      the letter %.
      the character =.
      math shift character $.
      macro parameter character #.

Catcode 1
---------

Initially, the only character of catcode 1 is the open brace. See below.

Catcode 2
---------

Initially, the only character of catcode 2 is the closing brace.

The following example shows that you can use other characters. In
*Tralics*, characters of catcode 1 and 2 serve two purposes: for
grouping, so that modifications are local to a group, and for delimiting
arguments.

.. container:: ltx-source

   ::

      {\catcode`A1\catcode`B2 
      \def\fooA2B\testeq\fooA2B
      \def\barA\bgroup\def\fooA3B\egroupB\bar% \bar modifies \foo locally
      \testeq\foo{2}}

Note the following trick: ``\uppercase \relax\bgroup}``. After
``\uppercase`` and commands like that, you can have an arbitrary
sequence of spaces and ``\relax`` commands. The argument is delimited on
the left by an implicit left brace, on the right by an explicit right
brace.

Catcode 3
---------

Initially, the only character with catcode 3 is the dollar sign. It is
used to enter and exit math and display math mode. A construct like
:literal:`{\catcode `x=3 \catcode`y=3 xy \sinxy}` is the same as
``\[\sin\]``.

Catcode 4
---------

The only character with catcode 4 is the alignment tab character &. (see
description of `arrays <doc-array.html>`__).

Catcode 5
---------

The only character with catcode 5 is the end-of-line character (carriage
return, ASCII code 13). When TeX sees such a character, it throws away
the remaining of the line. If TeX is in state *N*, the result is a
``\par`` token; if TeX is in state *M*, the result is a newline token of
catcode 10, and otherwise, the character is ignored. For *Tralics*, the
newline token has value 10 (line-feed), and not 32 (space) as in TeX. As
a result, in most cases, newline characters remain in the XML result,
whenever they are equivalent to space (the purpose is to make the output
more readable). Note that *Tralics* is in state *N* whenever it reads
the first character of a line. The number 5 is not a command code.

Whenever *Tralics* sees a new line, it inserts the character defined by
the ``\endlinechar`` command. This character is by default the end of
line character, see ```\endlinechar`` <doc-e.html#cmd-endlinechar>`__.

Catcode 6
---------

The only character with catcode 6 is the sharp sign #. This character is
used as parameter delimiter or parameter reference in macro definitions.
It is also used in TeX table preambles (but not in LaTeX not *Tralics*).
In the definition of ``\xbar`` below, the quantity ``#1`` refers to the
first argument of ``\Ma``, ``##1`` refers to the first argument of
``\Mb`` and ``####1`` could be used to refer to the first argument of
``\xbar``. As you can see, the sharp character can be replaced by any
character of catcode 6. In order to put in a command a character of
category code 6, it suffices to precede it by any character of catcode
6. The body of the ``\xfoo`` command is ``#A#A``, but the printer shows
it as ``##AA##AA``. Example

.. container:: ltx-source

   ::

      {\catcode`A6 \def\fooA1A2{\xdef\bar{A2A1}}\foo23
      \testeq\bar{32}}
      \def\Ma#1{\def\Mb ##1{\xdef\xbar{#1##1}}}\Ma a\Mb b
      \testeq\xbar{ab}
      \def\foo{###AA#AA}\def\fooB{##AA##AA} \def\fooC{####AAAA}
      \ifx\foo\fooB\else\bad\fi \ifx\foo\fooC\bad\fi}

Note. There are some subtle differences between TeX and *Tralics*.
Assume that X has category code 6, T has category 1 and that you define
``\def\foo xX1yT#1}``. If you ask TeX to print the value, you will see
\\foo=macro: xX1y->X1 while *Tralics* says \\foo=macro: x#1y->#1. The
reason is the following: TeX stores the macro as a single list of
tokens, replacing the start of the body (here T) by a special marker,
and omitting the final brace. This explains why T is not printed. The
body of the macro holds a reference to the first argument so
``\def\xfoo xX1y{X1}`` produces the same result, but
``\def\yfoo x#1y{#1}`` gives a different result. In case of
``\def\foo X1#2{#1}``, the character used in the body is the last found
in the argument list (here a sharp sign). On the other hand, *Tralics*
stores somewhere the list of characters that are before the first
argument (here x), and the delimitors for the arguments (here y for the
first argument). In particular ``\foo``, ``\xfoo`` and ``\yfoo`` use the
same representation.

As a consequence, comparing macros via ``\ifx`` may produce different
results; the same holds for ``\meaning``. Consider now
``\def\foo X1XTX1}``; TeX prints \\foo=macro: X1T->X1T while *Tralics*
prints \\foo=macro: #1T->#1. Here ``\foo`` is a macro with one argument
delimited by T (of category code 1), and this character is reinserted
after the expansion. *Tralics* does not show which character it
reinserts; in fact it insert an open brace. One could use ``\futurelet``
in order to see the difference.

Finally consider ``\newcommand\foo[2][truc]{X1X2}``. You will see
\\xfoo=macro: ->\@protected@testopt \\xfoo \\\xfoo {truc} in the case of
TeX (this means that TeX created an auxiliary command, whose value is
\\\xfoo=\long macro:[#1]#2->#1#2) or \\xfoo=opt \\long macro:
truc#2->#1#2 (this means that *Tralics* did not create other other
commands).

Catcode 7
---------

The only character with catcode 7 is the hat. This character is used in
math mode for superscripts. It is also used in the double hat construct:
if a character of catcode 7 appears twice in a row, like in ``^^13`` and
``^^ab``, and is followed by two digits in base 16, it is as if the
character with this code had been given (here, code 19, and 171); note
that only lowercase letters are allowed here. In the case where a
character of catcode 7 appears twice in a row, and is followed by a 7bit
character of code *c* (like ``^^Z`` or ``^^A`` or ``^^{``), it is as if
TeX had seen a character of code *c-64* or *c+64* (the one which is
between 0 and 128). In the example, the numbers are 26, 1 and 59. The
catcode of this character is examined again, for instance ``^^5e`` is
the hat character, of catcode 7.

Example.

.. container:: ltx-source

   ::

      {1^^{^^ab2^^5e^ab3^^5e^5e^ab4\def\Abc{ok}\def\bAc{OK}\^^41bc\b^^41c}
      {\catcode `\é=7 ééab $xé2$ %next line should produce M
      éé
      $1^è=^^^AééT$}  %line contains hat, hat, control-A
      \def\msg{A message.^^J}

This is the XML translation

.. container:: xml_out

   ::

      <p>1;«&nbsp;2«&nbsp;3«&nbsp;4okOK
      «&nbsp;<formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      <msup><mi>x</mi> <mn>2</mn> </msup></math></formula>
       M<formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      <mrow><msup><mn>1</mn> <mo>è</mo> </msup><mo>=</mo><mi>A</mi>
      <mo>&#20;</mo></mrow></math></formula> </p>

Note: The line that contains the two é characters translates as capital
M, because the last character on the line is the newline character,
control-M (even though on Unix, you would expect control-J). The
``\msg`` command contains as last token a newline character (control-J
of catcode 12), and not an end-of-line character of catcode 5. The
character control-T, represented by &#20; seems to be illegal in XML.
Without it, the preview is |some chars|.

If you say ``^^^^ABCD``, the result is a character whose value is
defined by the value ABCD (each letter must be a digit, or a lower case
letter between A and F). Such a construct is equivalent to
``\char &ABCD``, but it is one token, and spaces are not ignored after
it. (You can also use five hats, see `the documentation on
characters <doc-chars.html>`__). Example

.. container:: ltx-source

   ::

      \def\foo#1#2#3{#1=#2=#3=}
      \foo^^^^0153^^^^0152^^^^0178
       ^^^^017b^^8?

.. container:: xml_out

   ::

        
      &#339;=&#338;=&#376;=
      &#379;x?

Preview: |^^^^ example|

Catcode 8
---------

The only character with catcode 8 is the underscore character. It is
used for subscripts in math mode. See the
```\sp`` <doc-s.html#cmd-sp>`__ command for an example of use.

Outside math mode, you will get an error. For instance, if you say

.. container:: ltx-source

   ::

      {\catcode`x7 \catcode`y=8 a^b_c xy\sp\sb}

then *Tralics* will complain (but not in the same fashion as TeX).

.. container:: tty_out

   ::

      Error signaled at line 377:
      Missing dollar not inserted, token ignored: {Character ^ of catcode 7}.
      Error signaled at line 377:
      Missing dollar not inserted, token ignored: {Character _ of catcode 8}.
      Error signaled at line 377:
      Missing dollar not inserted, token ignored: {Character x of catcode 7}.
      Error signaled at line 377:
      Missing dollar not inserted, token ignored: {Character y of catcode 8}.
      Error signaled at line 377:
      Missing dollar not inserted, token ignored: \sp.
      Error signaled at line 377:
      Missing dollar not inserted, token ignored: \sb.

Catcode 9
---------

Characters of code 9 are ignored. Initially, no character has this
category code.

Catcode 10
----------

A character of catcode 10 acts like a space. If TeX sees a character of
catcode 10, the action depends on the current state. If the state is *N*
or *S*, the character is ignored. Otherwise, TeX is in state *M* and
changes to state *S*, and the result is a space token (character 32,
category 10). Space, tabulation are of catcode 10.

Spaces are in general ignored at start of line, because TeX is in state
*M*. In verbatim mode, the catcode of the space is changed, and thus
spaces remain.

Catcode 11
----------

Characters of catcode letter can be used to make multiletter control
sequences (without using ``\csname``). Only ASCII letter (between a and
z, or between A and Z) are by default of catcode 11.

Catcode 12
----------

Characters of catcode 12 cannot be used to make multiletter control
sequences. All characters not liste elsewhere are of catcode 12
(especially, all 8-bit characters).

Catcode 13
----------

Characters of category 13 are active. They can be used only if a
definition is associated. In *Tralics* only the tilde character is of
13, but the three characters ``_#&`` have a definition (the translation
is the character). Note that, in PlainTeX, the tilde character expands
to ``\penalty \@M \ `` (there is a space at the end of the command) and
in LaTeX to ``\nobreakspace{}``, which is the same with a
``\leavevmode`` in front, in *Tralics*, the expansion is simply
``\nobreakspace``.

Catcode 14
----------

Characters of catcode 14 act like an start-of-comment character. The
only character with catcode 14 is the percent character.

Catcode 15
----------

Characters of catcode 15 are invalid. There is no invalid character in
*Tralics*.

Catcode 16
----------

There is no character of catcode 16 in TeX . In *Tralics*, this code is
reserved for verbatim-like characters, defined by ``\DefineShortVerb``.
These characters act is if they were preceded by ``\verb``. Note that
the star character is not exceptional. You can use ``\fvset``, if you
want to change the translation of a space.

Example:

.. container:: ltx-source

   ::

      \DefineShortVerb{\|}
      Test of |\DefineShortVerb| and |\UndefineShortVerb|.
      \DefineShortVerb{\+}
      test 1 |toto| +x+ |+x-| +|t|+
      \UndefineShortVerb{\+}
      test 2 |toto| +x+ |+x-| +|t|+
      espace: |+ +|\fvset{showspaces=true}|+ +|\fvset{showspaces=false}|+ +|.
      \DefineShortVerb{\*}
      Verbatimfoo: *+ foo +*\verb+*foo*+\verb*+foo*+
      Verbatimfoo: \verb|+ foo +*foo*foo*|.

The XML output is the following

.. container:: xml_out

   ::

      <p>Test of <hi rend='tt'>\DefineShortVerb</hi> and <hi rend='tt'>\UndefineShortVerb</hi>.

      test 1 <hi rend='tt'>toto</hi> <hi rend='tt'>x</hi> <hi rend='tt'>+x-&#x200B;</hi>
        <hi rend='tt'>|t|</hi>

      test 2 <hi rend='tt'>toto</hi> +x+ <hi rend='tt'>+x-&#x200B;</hi> +<hi rend='tt'>t</hi>+
      espace: <hi rend='tt'>+&nbsp;+</hi><hi rend='tt'>+&blank;+</hi><hi rend='tt'>+&nbsp;+</hi>.

      Verbatimfoo: <hi rend='tt'>+&nbsp;foo&nbsp;+</hi><hi rend='tt'>*foo*</hi><hi rend='tt'>foo*</hi>
      Verbatimfoo: <hi rend='tt'>+&nbsp;foo&nbsp;+*foo*foo*</hi>.
      </p>

We can continue the example as follows. We show how to use ``\SaveVerb``
and ``\UseVerb``.

.. container:: ltx-source

   ::

      \SaveVerb{FE}|}|\def\FE{\UseVerb{FE}}
      \DefineShortVerb{\+}
      \SaveVerb{VE}+|+\def\VE{\UseVerb{VE}}
      \SaveVerb{DU}|$_|\def\DU{\UseVerb{DU}} %$
      \UndefineShortVerb{\+}
      \UndefineShortVerb{\|}
      \UndefineShortVerb{\*}
      Test \FE,\VE, \DU.

.. container:: xml_out

   ::

      <p>Test <hi rend='tt'>}</hi>,<hi rend='tt'>|</hi>, <hi rend='tt'>$_</hi>.
      </p>

Preview: |example of ShortVerb|

Mono-letter commands
====================

~
-

*(tralics version 1 description: The ``~`` character is a normal
character, but translates into a non-breaking space. Of course, the
result is a tilde in verbatim mode, and a mathematical space in math
mode.)* Example:

.. container:: ltx-source

   ::

      Test tilde:~\verb=~=$a~b$.
      \href{\url{a~b\~c}}{some url}

The XML translation is

.. container:: xml_out

   ::

      Test tilde:&nbsp;<hi rend='tt'>~</hi><formula type='inline'>
      <math xmlns='http://www.w3.org/1998/Math/MathML'>
      <mrow><mi>a</mi><mspace width='1em'/><mi>b</mi></mrow></math></formula>.
      <xref url='some url'>a~b~c</xref>

Note how ``~`` and ``\~`` are handled by the ``\url`` command.

In the current version of *Tralics*, the tilde character is active and
defined as ``\def~{\nobreakspace}``. The only purpose of the change is
to make the following example work (it is file xii by David Carlisle).

.. container:: ltx-source

   ::

      \let~\catcode~`76~`A13~`F1~`j00~`P2jdefA71F~`7113jdefPALLF
      PA''FwPA;;FPAZZFLaLPA//71F71iPAHHFLPAzzFenPASSFthP;A$$FevP
      A@@FfPARR717273F737271P;ADDFRgniPAWW71FPATTFvePA**FstRsamP
      AGGFRruoPAqq71.72.F717271PAYY7172F727171PA??Fi*LmPA&&71jfi
      Fjfi71PAVVFjbigskipRPWGAUU71727374 75,76Fjpar71727375Djifx
      :76jelse&U76jfiPLAKK7172F71l7271PAXX71FVLnOSeL71SLRyadR@oL
      RrhC?yLRurtKFeLPFovPgaTLtReRomL;PABB71 72,73:Fjif.73.jelse
      B73:jfiXF71PU71 72,73:PWs;AMM71F71diPAJJFRdriPAQQFRsreLPAI
      I71Fo71dPA!!FRgiePBt'el@ lTLqdrYmu.Q.,Ke;vz vzLqpip.Q.,tz;
      ;Lql.IrsZ.eap,qn.i. i.eLlMaesLdRcna,;!;h htLqm.MRasZ.ilk,%
      s$;z zLqs'.ansZ.Ymi,/sx ;LYegseZRyal,@i;@ TLRlogdLrDsW,@;G
      LcYlaDLbJsW,SWXJW ree @rzchLhzsW,;WERcesInW qt.'oL.Rtrul;e
      doTsW,Wk;Rri@stW aHAHHFndZPpqar.tridgeLinZpe.LtYer.W,:jbye

.. _section-1:

\\~
---

The ``\~`` command allows you to put a tilde accent on a letter (see
also the ```\tilde`` <doc-t.html#cmd-tilde>`__ command). The
possibilities are given here:

.. container:: ltx-source

   ::

      \~A \~a \~{\^A} \~{\^a} \~{\u A} \~{\u a} \~E \~e \~{\^E} \~{\^e} 
       \~I \~i \~N \~n \~O \~o \~{\=O} \~{\=o} \~{\'O} \~{\'o} \~{\"O}
      \~{\"o}  \~{\^O} \~{\^o} \~{\H O} \~{\H o}  \~U \~u \~{\'U} \~{\'u} \~{\H U}
      \~{\H u} \~V \~v  \~Y \~ y

the result is

.. container:: xml_out

   ::

      Ã ã &#7850; &#7851; &#7860; &#7861; &#7868; &#7869; &#7876; &#7877;
      &#296; &#297; Ñ ñ Õ õ &#556; &#557; &#7756; &#7757; &#7758;
      &#7759; &#7894; &#7895; &#7904; &#7905; &#360; &#361; &#7800; &#7801; &#7918;
      &#7919; &#7804; &#7805; &#7928; &#7929;

Preview |tilde accent|

.. _section-2:

\\$
---

The ``\$`` command is valid in math mode and text mode. It generates a
dollar sign (``<mi>$</mi>`` in math mode). See also the description of
the ```\qquad`` <doc-q.html#cmd-qquad>`__ command. Remember that the
dollar sign by itself (using default category codes) starts or finishes
a math formula.

.. _section-3:

\\%
---

The ``\%`` command is valid in math mode and text mode. It generates a
percent sign ( ``<mo>%</mo>`` in math mode). See also the description of
the ```\qquad`` <doc-q.html#cmd-qquad>`__ command. Remember that the
percent sign by itself (using default category codes) starts a comment.

.. _section-4:

\\&
---

The ``\&`` command is valid in math mode and text mode. It generates a
ampersand sign ``&amp;`` (or ``<mo>&amp;</mo>`` in math mode). See
description of the ```\qquad`` <doc-q.html#cmd-qquad>`__ command.
Remember that the ``&`` character is valid only inside arrays as a cell
delimiter (see description of `arrays <doc-array.html>`__).

.. _section-5:

\\!
---

The ``\!`` command is valid in math mode and text mode. It generates a
negative space of -3/18em in math mode ``<mspace width='-0.166667em'/>``
and nothing in text mode. See description of the
```\qquad`` <doc-q.html#cmd-qquad>`__ command.

\\, (backslash comma)
---------------------

The ``\,`` command is valid in math mode and text mode. It generates a
space of 3/18em in math mode, ``<mspace width='0.166667em'/>``, and a
``&nbsp;`` otherwise (this is ``&#xA0;``, as defined in isonum.ent).

See description of the ```\qquad`` <doc-q.html#cmd-qquad>`__ command and
```\AA`` <doc-a.html#cmd-Caa>`__ command.

.. _section-6:

\\>
---

The ``\>`` command is valid in math mode. It generates a space of
4/18em. See description of the ```\qquad`` <doc-q.html#cmd-qquad>`__
command.

.. _section-7:

\\;
---

The ``\;`` command is valid in math mode. It generates a space of
5/18em. See description of the ```\qquad`` <doc-q.html#cmd-qquad>`__
command.

.. _section-8:

\\{
---

The ``\{`` command is valid in math mode and text mode. It generates an
open brace, for instance ``<mo>{</mo>`` in math mode. See description of
the ```\qquad`` <doc-q.html#cmd-qquad>`__ command, and
```\AA`` <doc-a.html#cmd-Caa>`__ command. It can be used as a math
delimiter, see description of the ```\vert`` <doc-v.html#cmd-vert>`__
command.

Note that a single brace (without backslash), assuming default catcodes,
opens a group.

.. _section-9:

\\}
---

The ``\}`` command is valid in math mode and text mode. It generates an
close brace, for instance ``<mo>}</mo>`` in math mode. See description
of the ```\qquad`` <doc-q.html#cmd-qquad>`__ command, and
```\AA`` <doc-a.html#cmd-Caa>`__ command. It can be used as a math
delimiter, see description of the ```\vert`` <doc-v.html#cmd-vert>`__
command.

Note that a single brace (withut backslash) (assuming default catcodes)
closes a group.

\\+ (backslash plus)
--------------------

This command is undefined in *Tralics*; it is part of the unimplemented
``\settabs`` mechanism

\\\_ (backslash underscore)
---------------------------

The ``\_`` command is valid in math mode and text mode. It generates an
underscore, in math mode it is ``<mo>_</mo>`` See description of the
```\qquad`` <doc-q.html#cmd-qquad>`__ command and
```\AA`` <doc-a.html#cmd-Caa>`__ command.

With defaults catcodes, a simple underscore character is valid only in
math mode, and starts a subscript.

\\- (backslash minus)
---------------------

The LaTeX kernel contains ``\def\-{\discretionary{-}{}{}}``. Since
hyphenation is not implemented in *Tralics*, ``\-`` produces no result.

The command is also redefined by the tabbing environment, which is not
yet implemented.

Note that hyphens are ligatures:

.. container:: ltx-source

   ::

      a - b -- c --- d

.. container:: xml_out

   ::

      <p>a - b &ndash; c &mdash; d</p>

preview |dashes|

\| (vertical bar)
-----------------

The ``|`` command it is equivalent to ``\vert`` as a math delimiter. See
description of the ```\vert`` <doc-v.html#cmd-vert>`__ command.

\\\| (backslash vertical bar)
-----------------------------

The ``\|`` command is valid only in math mode. It is equivalent to
``\Vert``. It produces the character U+2225, ∥. See description of the
```\vert`` <doc-v.html#cmd-vert>`__ command.

.. _section-10:

\\#
---

The character # has category 6, is described `here <catcode6>`__ , and
cannot be use to produce a sharp sign, you must used ``\#`` instead. It
is valid in text and in math mode; you can also use
```\sharp`` <doc-s.html#cmd-sharp>`__, that produces the musical sign
character U+266F, ♯. See also description of the
```\AA`` <doc-a.html#cmd-Caa>`__ command.

\\\\ (backslash backslash)
--------------------------

The ``\\`` command has three meanings, depending on whether it is in
text, in a text table, or in math table. It can be followed by a star
(which is ignored) and an optional argument in brackets, which is a
dimension. Inside a table, the ``\\`` indicates the end of the current
row, and the optional argument specifies additional space between rows.
This argument is currently ignored in math mode. Note that ``\\``
finished the current cell, the current row, and starts a new row and a
new cell. If this is the last cell in the table, and if it is empty,
then the row is removed. Said otherwise, a ``\\`` is ignored at the end
of a table; it is however needed before a final ``\hline`` if you want
an horizontal rule at the end of the table.

Outside a table, a ``\\`` specifies the end of a paragraph, and the
start of a new one, which is not indented. The optional argument
indicates vertical space to be added (see also the
```\vskip`` <doc-v.html#cmd-vskip>`__ command). In a title, the command
with its arguments is replaced by ``\@headercr``, a command that
produces a space by default. We give here an example of ``\\`` outside a
table.

.. container:: ltx-source

   ::

      text A \\* text B \\[3mm] text C
      \expandafter\def\csname @headercr\endcsname{; }
      \section{A\\[2mm]B}
      \begin{center}
      line one\\
      this is the second line
      \end{center}

This is the XML translation. The ``<p>`` element has a ``noindent``
attribute only if it has no ``rend`` attribute.

.. container:: xml_out

   ::

      <p>text A</p>
      <p noindent='true'>text B</p>
      <p noindent='true' spacebefore='8.53581'>text C</p>
      <div0 id='uid1'><head>A; B</head>
      <p rend='center'>line one</p>
      <p rend='center'>this is the second line</p>
      </div0>

Preview. |example of bbbb in text|

This is an example of math table.

.. container:: ltx-source

   ::

      \begin{equation}
      \begin{array}{lcl}
      \dot{x} & = & Ax+g(x,u)\\[2mm]
       y & = & Cx \\
       \multicolumn{3}{l}{x\in R^n} 
      \end{array}
      \end{equation}

This is the XML translation.

.. container:: xml_out

   ::

      <formula type='display'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mtable>
         <mtr>
          <mtd columnalign='left'><mover accent='true'><mi>x</mi> <mo>&dot;</mo></mover></mtd>
          <mtd><mo>=</mo></mtd>
          <mtd columnalign='left'>
           <mrow><mi>A</mi><mi>x</mi><mo>+</mo><mi>g</mi><mo>(</mo><mi>x</mi>
             <mo>,</mo><mi>u</mi><mo>)</mo></mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='left'><mi>y</mi></mtd>
          <mtd><mo>=</mo></mtd>
          <mtd columnalign='left'><mrow><mi>C</mi><mi>x</mi></mrow></mtd>
         </mtr>
         <mtr>
          <mtd columnalign='left' columnspan='3'><mrow><mi>x</mi><mo>&Element;</mo>
              <msup><mi>R</mi> <mi>n</mi> </msup></mrow>
          </mtd>
         </mtr>
        </mtable>
       </math>
      </formula>

This is a preview. Note that the ``\multicolumn`` is ignored in the
rendering. This has been corrected. |example of bbbb in a math table|

Other example.

.. container:: ltx-source

   ::

      \begin{tabular}{|ll|rr|cc|}
      \hline a&b&c&d&e&f\\
      aaa&bbb&ccc&ddd&eee&fff\\
      \hline
      A&\multicolumn{3}{l|}{BCD}&E&F\\
      \multicolumn{2}{|l}{ab}&c&d&e&f\\
      \cline{1-3}\cline{6-6}
      aaa&bbb&ccc&ddd&eee&fff\\\hline
      \end{tabular}

This is the XML translation.

.. container:: xml_out

   ::

      <table rend='inline'>
       <row top-border='true'>
        <cell halign='left' left-border='true'>a</cell>
        <cell halign='left' right-border='true'>b</cell>
        <cell halign='right'>c</cell>
        <cell halign='right' right-border='true'>d</cell>
        <cell halign='center'>e</cell>
        <cell halign='center' right-border='true'>f</cell>
       </row>
       <row>
        <cell halign='left' left-border='true'>aaa</cell>
        <cell halign='left' right-border='true'>bbb</cell>
        <cell halign='right'>ccc</cell>
        <cell halign='right' right-border='true'>ddd</cell>
        <cell halign='center'>eee</cell>
        <cell halign='center' right-border='true'>fff</cell>
       </row>
       <row top-border='true'>
        <cell halign='left' left-border='true'>A</cell>
        <cell cols='3' halign='left' right-border='true'>BCD</cell>
        <cell halign='center'>E</cell>
        <cell halign='center' right-border='true'>F</cell>
       </row>
       <row>
        <cell bottom-border='true' cols='2' halign='left' left-border='true'>ab</cell>
        <cell bottom-border='true' halign='right'>c</cell>
        <cell halign='right' right-border='true'>d</cell>
        <cell halign='center'>e</cell>
        <cell bottom-border='true' halign='center' right-border='true'>f</cell>
       </row>
       <row bottom-border='true'>
        <cell halign='left' left-border='true'>aaa</cell>
        <cell halign='left' right-border='true'>bbb</cell>
        <cell halign='right'>ccc</cell>
        <cell halign='right' right-border='true'>ddd</cell>
        <cell halign='center'>eee</cell>
        <cell halign='center' right-border='true'>fff</cell>
       </row>
      </table>

The rendering of the ``tabular`` is not good: first, the width of the
table is the width of the page (changed to 15cm in this example), and
the width of each column is defined by the number of characters in it
(in this example, you do not see a difference, but replacing ``a`` by
``$a$`` would be catastrophic).

You should look at the `page on arrays <doc-array.html>`__. It contains
an example similar to this one. You can see that we changed the
algorithm: a ``\hline`` implies a ``bottom-border`` on the previous row.

|example of bbbb in a normal table|

.. _section-11:

[, \\[
------

The ``[`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command.

The expansion of ``\[`` is ``$$``. It means: \`begin display math' but
*Tralics* does no check.

.. _section-12:

], \\]
------

The ``]`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command.

The expansion of ``\]`` is ``$$``. It means: \`end display math' but
*Tralics* does no check. Hence ``$a\]b$`` and ``\]x\[`` are valid
expressions.

.. _section-13:

(, \\(
------

The ``(`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command.

The expansion of ``\(`` is ``$``. It means: \`start inline math' but
*Tralics* does no check. This command is also used as an opening
delimiter by the `ifthen package <doc-i.html#ifthenelse>`__.

.. _section-14:

), \\)
------

The ``)`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command.

The expansion of ``\)`` is ``$``. It means: \`end inline math' but
*Tralics* does no check. In particular ``$$ a\(\)`` is valid; note also
that ``\(\)`` is an empty math formula. This command is also used as a
closing delimiter by the `ifthen package <doc-i.html#ifthenelse>`__.

< (less than sign)
------------------

The ``<`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command. Otherwise, it
translates like a normal character, but is always printed as ``&lt;`` in
the XML output. There is one exception: the
```rawxml`` <doc-r.html#env-rawxml>`__ environment prints the convent
verbatim, and the ```\xmllatex`` <doc-x.html#cmd-xmllatex>`__ command
print

> (greater than sign)
---------------------

The ``>`` command can be used as a math delimiter. See description of
the ```\vert`` <doc-v.html#cmd-vert>`__ command.

' (single quote, apostrophe, straight quote)
--------------------------------------------

This character behaves normally in text mode; it has a special meaning
when *Tralics* reads a number (see
`scanint <doc-s.html#fct-scanint>`__); it has a special meaning in math
mode. Plain TeX defines an active apostrophe character as follows

.. container:: ltx-source

   ::

      {\catcode`\'=\active \gdef'{^\bgroup\prim@s}}
      \def\prim@s{\prime\futurelet\next\pr@m@s}
      \def\pr@m@s{\ifx'\next\let\nxt\pr@@@s \else\ifx^\next\let\nxt\pr@@@t
        \else\let\nxt\egroup\fi\fi \nxt}
      \def\pr@@@s#1{\prim@s} \def\pr@@@t#1#2{#2\egroup}

The definition of LaTeX is similar, with ``\expandafter`` instead of
``\nxt``. In *Tralics*, these lines of TeX code are replaced by some
lines of C++. The effect can be seen on the following example.

.. container:: ltx-source

   ::

      {\tracingall $x' x'' x''' x'''' u_2' v'^3_4$}

The transcript file will hold

.. container:: log_out

   ::

      {math shift}
      +stack: level + 3
      ...
      Math: $x^{\prime } x^{\prime \prime } x^{\prime \prime \prime } 
      x^{\prime \prime \prime \prime } u_2^{\prime } v^{\prime 3}_4$

We have removed a bunch of lines of the form \`+stack: level', because
each opening and closing brace changes the current level (by the way, it
is the dollar sign that increases the level first). The XML translation
is the following.

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mrow>
         <msup><mi>x</mi> <mo>&prime;</mo></msup>
         <msup><mi>x</mi> <mrow><mo>&prime;</mo><mo>&prime;</mo></mrow> </msup>
         <msup><mi>x</mi><mrow><mo>&prime;</mo><mo>&prime;</mo><mo>&prime;</mo></mrow></msup>
         <msup><mi>x</mi> <mrow><mo>&prime;</mo><mo>&prime;</mo><mo>&prime;</mo>
           <mo>&prime;</mo></mrow></msup>
         <msubsup><mi>u</mi> <mn>2</mn> <mo>&prime;</mo></msubsup>
         <msubsup><mi>v</mi> <mn>4</mn><mrow><mo>&prime;</mo><mn>3</mn></mrow> </msubsup>
        </mrow>
       </math>
      </formula>

Preview |primes in math mode|

\\' (backslash left quote)
--------------------------

The single quote character behaves normally, but has a special meaning
when *Tralics* reads a number (see
`scanint <doc-s.html#fct-scanint>`__). The ``\'`` command puts an acute
accent over some letters. Do not confuse with ``\acute``, which is a
math-only command. Example

.. container:: ltx-source

   ::

      \'A \'a \'{\AA} \'{\aa} \'{\AE} \'{\ae} \'{\^A} \'{\^a} \'{\u A} \'{\u a} 
      \'C \'c \'{\c C} \'{\c c} \'E \'e  \'{\=E}
      \'{\=e} \'{\^E} \'{\^e} \'G \'g \'I \'i \\\'{\"I} \'{\"i} \'K \'k \'L \'l \'M 
      \'m \'N \'n \'O \'o \'{\O} \'{\o} \'{\~O} \'{\~o} \'{\=O} \'{\=o} \'{\^O} 
      \'{\^o} \'{\H O} \'{\H o}\\ \'P \'p \'R \'r \' S \'s
      \'{\.S} \'{\.s} \'U \'u \'{\"U} \'{\"u} \'{\~U} \'{\~u} \'{\H U} \'{\H u} 
      \'W \'w \'Y \'y \'Z \'z 

gives

.. container:: xml_out

   ::

      Á á &#506; &#507; &#508; &#509; &#7844; &#7845; &#7854; &#7855;
      &#262; &#263; &#7688; &#7689; É é &#7702;
      &#7703; &#7870; &#7871; &#500; &#501; Í í
      &#7726; &#7727; &#7728; &#7729; &#313; &#314; &#7742;
      &#7743; &#323; &#324; Ó ó &#510; &#511; &#7756; &#7757; &#7762; &#7763; &#7888;
      &#7889; &#7898; &#7899;
      &#7764; &#7765; &#340; &#341; &#346; &#347;
      &#7780; &#7781; Ú ú &#471; &#472; &#7800; &#7801; &#7912; &#7913;
      &#7810; &#7811; Ý ý &#377; &#378;

Preview |acute accent|

\\" (backslash double quote)
----------------------------

The double quote character behaves normally, but has a special meaning
when *Tralics* reads a number (see
`scanint <doc-s.html#fct-scanint>`__). The ``\"`` command puts a umlaut
or diaeresis accent over some letters. Example:

.. container:: ltx-source

   ::

      \"A \"a \"{\=A} \"{\=a} \"E \"e \"H \"h \"I \"i \"{\'I} \"{\'i} \"O \"o
      \"{\=O} \"{\=o} \"{\~O} \"{\~o} \"t\\ \"U \"u \"{\=U} \"{\=u} \"{\`U} \"{\`u}
      \"{\'U} \"{\'u} \"{\v U} \"{\v u} \"W \"w \"X \"x \"Y \"y

gives

.. container:: xml_out

   ::

      Ä ä &#478; &#479; Ë ë &#7718; &#7719; Ï ï &#7726; &#7727; Ö ö
      &#554; &#555; &#7758; &#7759; &#7831;
      Ü ü &#7802; &#7803; &#475; &#476;
      &#471; &#472; &#473; &#474; &#7812; &#7813; &#7820; &#7821; &#376; ÿ

Preview |dieresis accent|

\\= (backslash equals)
----------------------

The ``\=`` command generates a macro accent, similar to the ``\bar``
(that works in math mode only). The translation of

.. container:: ltx-source

   ::

      \=A \=a \={\"A} \={\"a} \={\.A} \={\.a} \=\AE \=\ae \=E \= e \={\'E} \={\'e}
      \={\`E} \={\`e} \=G \=g \=H \=h\\
      \=I \=i \={\d L} \={\d l} \= O \=o \={\k O} \={\k o} \={\"O} \={\"o} \={\~O}
      \={\~o} \={\.O} \={\.o} \={\`O} \={\`o} \={\'O} \={\'o}\\ \={\d R} \={\d r}
       \=T \=t \=U \=u \={\"U} \={\"u} \=Y \=y

is

.. container:: xml_out

   ::

      &#256; &#257; &#478; &#479; &#480; &#481; &#482;&#483;&#274; &#275; &#7702; &#7703;
      &#7700; &#7701; &#7712; &#7713; &#294; &#295;
      &#298; &#299; &#7736; &#7737; &#332; &#333; &#492; &#493; &#554; &#555; &#556;
      &#557; &#560; &#561; &#7760; &#7761; &#7762; &#7763; &#7772; &#7773;
      &#358; &#359; &#362; &#363; &#7802; &#7803; &#562; &#563;

Preview |macro accent|

\\. (backslash dot)
-------------------

The ``\.`` command generates dot accent, similar to the ``\dot`` (that
works in math mode only). The translation of

.. container:: ltx-source

   ::

      \.A \.a \.{\=A} \.{\=a} \.B \.b \.C \.c \.D \.d \.E \.e \.F \.f \.G \.g \.H
      \.h \.I \.L \.l \.M \.m \.N \.n \.O \.o \.{\=O} \.{\=o} \.P \.p \.R \.r \.S
      \.s \.{\d S} \.{\d s} \.{\v S} \.{\v s} \.{\'S} \.{\'s} \.T \.t \.W \.w \.X
      \.x \.Y \.y \.Z \.z

is

.. container:: xml_out

   ::

      &#550; &#551; &#480; &#481; &#7682; &#7683; &#266; &#267; &#7690; &#7691; &#278; &#279;
      &#7710; &#7711; &#288; &#289; &#7714; &#7715; &#304; &#319; &#320; &#7744; &#7745; &#7748;
      &#7749; &#558; &#559; &#560; &#561; &#7766; &#7767; &#7768; &#7769; &#7776; &#7777;
      &#7784; &#7785; &#7782; &#7783; &#7780; &#7781; &#7786; &#7787; &#7814; &#7815; &#7818;
      &#7819; &#7822; &#7823; &#379; &#380;

Preview |dot accent|

\\^ (backslash hat)
-------------------

The ``\^`` command generates a circonflex accent, similar to the
``\hat`` command (that works in math mode only). The translation of

.. container:: ltx-source

   ::

      \^A \^a \^{\'A} \^{\'a} \^{\`A} \^{\`a} \^{\h A} \^{\h a} \^{\~A} \^{\~a} 
      \^{\d A} \^{\d a} \^C \^c \^E \^e \^{\'E} \^{\'e} \^{\`E} \^{\`e} \^{\h E}
      \^{\h e} \^{\~E} \^{\~e} \^{\d E} \^{\d e} \^G \^g \^H \^h \^I \^i \^J \^j 
      \^O \^o \^{\'O} \^{\'o}  \^{\`O} \^{\`o}  \^{\h O} \^{\h o}  \^{\~O} \^{\~o}  
      \^{\d O} \^{\d o}  \^S \^ s \^U \^u \^W \^w \^Y \^y \^Z \^z

is

.. container:: xml_out

   ::

      Â â &#7844; &#7845; &#7846; &#7847; &#7848; &#7849; &#7850; &#7851;
      &#7852; &#7853; &#264; &#265;
      Ê ê &#7870; &#7871; &#7872; &#7873; &#7874;
      &#7875; &#7876; &#7877; &#7878; &#7879; &#284; &#285; &#292; &#293; Î î &#308; &#309;
      Ô ô &#7888; &#7889; &#7890; &#7891; &#7892; &#7893; &#7894; &#7895;
      &#7896; &#7897; &#348; &#349; Û û &#372; &#373; &#374; &#375; &#7824; &#7825;

Preview |circonflex accent|

\\\` (backslash backquote)
--------------------------

The backquote character behaves normally, but has a special meaning when
*Tralics* reads a number (see `scanint <doc-s.html#fct-scanint>`__). The
:literal:`\\\`` command generates a grave accent, similar to the
``\grave`` command (that works in math mode only). The translation of

.. container:: ltx-source

   ::

      \`A \`a \`{\^A} \`{\^a} \`{\u A} \`{\u a} \`E \`e \`{\=E} \`{\=e} \`{\^E}
      \`{\^e}  \`I \`i \`N \`n \\\`O \`o \`{\=O} \`{\=o} \`{\^O} \`{\^o} 
      \`{\H O} \`{\H o} \`U \`u \`{\"U} \`{\"u} \`{\H U} \`{\H u} \`W \`w \`Y\`y 

is

.. container:: xml_out

   ::

      À à &#7846; &#7847; &#7856; &#7857; È è &#7700; &#7701; &#7872;
      &#7873; Ì ì &#504; &#505;
      Ò ò &#7760; &#7761; &#7890; &#7891;
      &#7900; &#7901; Ù ù &#475; &#476; &#7914; &#7915; &#7808; &#7809; &#7922;&#7923;

Preview |grave accent|

\\ (backslash space)
--------------------

The ``\`` command adds a space character to the XML tree. The command
name is formed of a backslash followed by a space, a tabulation, a
line-feed, or a carriage return. Since these characters are of catcode
10 (space), spaces after them is ignored. At the start of chapter 25 of
the `TeXbook <index.html#texbook>`__, Knuth explains that the result of
this command is as if a space had been given in a context where the
space factor is 1000 (*Tralics* ignores ``\spacefactor`` and other
factors that modify the space factor). In math mode, the translation is
``<mspace width='4pt'/>``.

Note that ``\space`` produces a single space.

\\/ (backslash slash)
---------------------

The ``\/`` command is assumed to insert a kern corresponding to the
italic correction, if the last item on the horizontal list is a
character or a ligature, or a kern of width zero in math mode. However,
*Tralics* ignores such subtleties, so that this command does nothing.

.. _section-15:

\\\*
----

This command is valid in math mode only, the translation is a
``<mo>&InvisibleTimes;</mo>``.

\\@ (backslash atsign)
----------------------

The LaTeX kernel contains ``\def\@{\spacefactor\@m}``. However,
*Tralics* ignores currently space factors, so that this command does
nothing. If the at-sign character is of category \`letter', then
``\@ne`` is a single command; but if it is of category \`other' (normal
case), it is ``\@`` followed by some characters, and this often gives an
error in LaTeX, since the ``\spacefactor`` command cannot be used
everywhere.

The description of ``\@makeactive`` is found near that of
``\makeatletter``; there are som exceptions to this rule: ``\@ne`` is
described as if it were ``\one``.

.. |some chars| image:: /img/img_13.png
.. |^^^^ example| image:: /img/img_80.png
.. |example of ShortVerb| image:: /img/img_19.png
.. |tilde accent| image:: /img/img_20.png
.. |dashes| image:: /img/img-85.png
.. |example of bbbb in text| image:: /img/img_32.png
.. |example of bbbb in a math table| image:: /img/img_31.png
.. |example of bbbb in a normal table| image:: /img/img_33.png
.. |primes in math mode| image:: /img/img_14.png
.. |acute accent| image:: /img/img_91.png
.. |dieresis accent| image:: /img/img_16.png
.. |macro accent| image:: /img/img_18.png
.. |dot accent| image:: /img/img_17.png
.. |circonflex accent| image:: /img/img_21.png
.. |grave accent| image:: /img/img_22.png
