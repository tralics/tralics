=============================================
All commands, alphabetic order; unimplemented
=============================================

| This page contains the description of the following unimplemented
  commands `\\accent <#cmd-accent>`__,
  `\\addcontentsline <#cmd-addcontentsline>`__,
  `\\address <#cmd-address>`__,
  `\\addtocontents <#cmd-addtocontents>`__,
  `\\appendix <#cmd-appendix>`__,
  `\\arraystretch <#cmd-arraystretch>`__, `\\author
  \\@author <#cmd-author>`__, `\\displaylines <#cmd-displaylines>`__,
  `\\eqalign <#cmd-eqalign>`__, `\\eqalignno <#cmd-eqalignno>`__,
  `\\futurenonspacelet <#cmd-futurenonspacelet>`__,
  `\\insert <#cmd-insert>`__, `\\leqalignno <#cmd-eqalignno>`__,
  `\\newinsert <#cmd-newinsert>`__, `\\newhelp <#cmd-newhelp>`__,
  `\\newfam <#cmd-newfam>`__, `\\new@mathgroup <#cmd-newfam>`__,
  `\\openup <#cmd-openup>`__, `\\vadjust <#cmd-vadjust>`__,
| and environments `abstract <#env-abstract>`__. [File under
  construction, there are other unimplemented commands]

--------------

abstract (environment)
----------------------

The ``abstract`` environment is not implemented in Tralics. (this
environment is not defined by LaTeX, but by classes like article and
report). However, it is possible to define one that can be used once for
a titlepage. See `Configuration page <titlepage.html>`__.

\\accent
--------

This command is followed by a 8-bit number and by optional assignments.
“Here optional assignments stands for zero or more assignment commands
other than ``\setbox``. If the assignments are not followed by a
character, where character stands for any of the commands just discussed
in the previous paragraph, TeX treats ``\accent`` as if it were
``\char``, except that the space factor is set to 1000. Otherwise, the
character that follows the assignment is accented by the character that
corresponds to the 8-bit number (The purpose of the intervening
assignments is to allow the accenter and the accentee to be in different
fonts.) [...] Finally TeX sets ``\spacefactor=10000``.” So, according to
the TeXbook, the following is legal:
:literal:`\\accent`a\count0=0\par \accent'23\count0=0e`. It produces an
\`a', then a \`é' (provided that the current font uses the
OT1-encoding). In *Tralics* an error is generated and the behavior is
that of like ``\char``. The space factor is left unchanged. See
`scanint <doc-s.html#fct-scanint>`__ for how the argument is scanned.

\\addcontentsline
-----------------

This command is defined by LaTeX as
``\def\addcontentsline#1#2#3{%   \addtocontents{#1}{\protect\contentsline{#2}{#3}{\thepage}}}``
The first argument is the name of a file extension (toc, lof or lot),
the second a symbolic name (for instance, it could be ``part``, in this
case ``\protect \contentsline{#2}`` will evaluate later to ``\l@part``);
the last argument will in general be a command (like ``\numberline``)
that does something with the line number.

In *Tralics*, the value of ``\thepage`` is undefined, this explains why
this command is not implemented.

\\address
---------

This command is not defined by LaTeX, but by classes like letter, and
maybe some classes for articles to journals. However, it is possible to
define such a command that can be used once for a titlepage. See
`Configuration page <titlepage.html>`__.

\\addtocontents
---------------

See ``\addcontentsline`` above. This command takes two arguments, and
prints in a file defined by the first argument, the value of the second.
This value is expanded, except for: ``\label``, \\\ ``\index``,
``\glossary`` (these commands and their arguments are not written),
commands preceeded by ``\protect``, and ``\thepage`` (this command is
expanded later, when the page is shipped out).

\\addvspace
-----------

Unimplemented. See elsewhere.

\\appendix
----------

Lamport says that: ``\appendix`` is a declaration that changes the way
sectional units are numbered. It generates no text, and does not affect
the numbering of parts. Since *Tralics* does not handle section numbers,
this command does nothing really useful.

You may consider: ``\endsec{}\xmlemptyelt{appendix}``, this closes all
sectioning environments and puts ``<appendix>`` on the top of the XML
tree.

\\arraystretch
--------------

\\author
--------

In LaTeX, this command remember its argument in ``\@author``, and the
class may (or may not) use its value via ``\maketitle``; it may also
make it undefined later on.

It is not implemented in *Tralics*, but you can design your own
titlepage format. See `Configuration page <titlepage.html>`__.

.. _cmd-displaylines:

\\displaylines
--------------

This command is defined in TeX (and LaTeX for compatibility). It
corresponds to the LaTeX environments ``multline`` or ``gather``. You
say for instance
``$$\displaylines{line1\cr\line2\cr  ... last line\cr}$$`` Each line is
centered (but you can use ``\hfill``). Exercise 19.16 of the TeXbook
explains how you can fake equation numbers. Unimplemented

.. _cmd-eqalign:

\\eqalign
---------

This is a plain TeX construction. A typical example is
``$$\eqalign{line&1\cr\line&2\cr    last& line\cr}$$``. Its LaTeX
equivalent would be
``\begin{align*}line&1\\line&2\\    last& line\end{align*}``. The
``\eqalign`` construction is allowed anywhere in a display math
environment and corresponds to a ``aligned`` environment in LaTex; see
example below.

.. _cmd-eqalignno:

\\eqalignno, \\leqalignno
-------------------------

These are plain TeX constructions. A typical example is
``$$\eqalignno{line&1\cr\line&2&(a)\cr    last& line&b\cr}$$``. The
effect is the same as ``\eqalign``, as described above, but the second
and third equations have an equation number on the right. In the case of
``\leqalignno``, it would be on the left. The equation number may
overlap the formula. If a formula contains ``\eqalignno`` it should
contain nothing else. You can use ``\noalign{\hbox{and}}`` to separate
equations with text; the LaTeX equivalent of this construction being
``\intertext``. The LaTeX philosophy is to use automatic equation
numbers, so that this construction becomes less interesting, however the
``\tag`` command can be used to change the equation number. Moreover, a
global indicator says that all equation numbers are to be put on the
left (or right).

Example. If you say ``\tagasattribute`` then *Tralics* will put the tags
as attribute for the formula. Since there is one equation, what you get
is a single attribute containing ``a,b``. Note that the use of ``\tag*``
inhibits putting parentheses around the tag. There is a third option,
``\tagatcurpos`` that puts the tag at the current position (with some
space on the left). In this case, parentheses around tags are correct.

.. container:: ltx-source

   ::

       % equivalent of \eqalign
          $$ T:\begin{aligned}L&1\\L&2\\  L&3\end{aligned}$$
      % equivalent of eqalignno
      \begin{align}L&1\notag\\Lamp;2\tag{a}\\  L&3\tag*{b}\end{align}

.. container:: xml_out

   ::

      <formula type='display'>
        <math mode='display' xmlns='http://www.w3.org/1998/Math/MathML'>
          <mrow>
            <mi>T</mi><mo>:</mo>
            <mtable displaystyle='true'>
          <mtr>
            <mtd columnalign='right'><mi>L</mi></mtd>
            <mtd columnalign='left'><mn>1</mn></mtd>
          </mtr>
          <mtr>
            <mtd columnalign='right'><mi>L</mi></mtd>
            <mtd columnalign='left'><mn>2</mn></mtd>
          </mtr>
          <mtr>
            <mtd columnalign='right'><mi>L</mi></mtd>
            <mtd columnalign='left'><mn>3</mn></mtd>
          </mtr>
            </mtable>
          </mrow>
        </math>
      </formula>
      <formula id-text='mid1' id='uid1' textype='align' type='display'>
        <math mode='display' xmlns='http://www.w3.org/1998/Math/MathML'>
          <mtable displaystyle='true'>
            <mtr>
          <mtd columnalign='right'><mi>L</mi></mtd>
          <mtd columnalign='left'><mn>1</mn></mtd>
            </mtr>
            <mtr>
          <mtd columnalign='right'><mi>L</mi></mtd>
          <mtd columnalign='left'><mn>2</mn></mtd>
            </mtr>
            <mtr>
          <mtd columnalign='right'><mi>L</mi></mtd>
          <mtd columnalign='left'><mrow><mn>3</mn>
              <mspace width='2.em'/><mi mathvariant='normal'>a</mi><mo>,
              </mo><mi mathvariant='normal'>b</mi></mrow></mtd>
            </mtr>
          </mtable>
        </math>
      </formula>

.. _cmd-futurenonspacelet:

\\futurenonspacelet
-------------------

This macro is explained in the TeXbook, appendix D; it is provided by
the booktabs package.

.. _cmd-insert2:

\\insert
--------

This command scan a 8-bit integer; it must not be 255 (as this is a
register index, any value less than 1024 is allowed by *Tralics*). After
that comes filler, then a {, then vertical mode material and finally }
(see `scantoks <doc-s.html#fct-scantoks>`__ for the syntax). “The {
causes TeX to enter internal vertical mode and a new level of grouping.
When the matching } is sensed, the vertical list is put into an
insertion item that is appended to the current list using [...] that
were current in the group just ended. This insertion item leads
ultimately to a page insertion only if it appears in TeX's main vertical
list, so it will have to ‘migrate’ there if it starts out in a
horizontal list or a math list. TeX also exercises the page builder,
after an ``\insert`` has been appended in vertical mode”. (Citation from
the `TeXbook <index.html#texbook>`__, Chapter 24).

The insertion item is formed of the list constructed above, and some
parameters, stored in registers indexed by the 8-bit integer. What
happens with the content of the box is complicated (the previous
paragraphs indicated that it could simply disappear). In fact, this is
used to implement footnotes in plain TeX, and floating figures in LaTeX
(each pending float uses a different insertion number).

.. _newfam:

\\newfam \\new@mathgroup
------------------------

The command ``\newfam\x`` allocates a new counter for use with ``\fam``.
Since math families are not implemented, the allocator is not
implemented either in *Tralics*. In LaTeX, a family is a ``\mathgroup``
and the allocator is ``\new@mathgroup``.

\\newinsert
-----------

The command ``\newinsert\x`` allocates a new slot for use with
``\insert``; if the allocated value is for instance 17, then
``\count17``, ``\dimen17``, ``\skip17`` and ``\box17`` are reserved for
this insertion. Since insertions are not implemented, the allocator is
not implemented either in *Tralics*.

\\newhelp
---------

The result of ``\newhelp\foo{my message}`` is to allocate a new token
list register, say 12, make ``\foo`` equivalent to ``\toks12`` and put
in this token register the command whose name is ``my message``. You can
say ``\errhelp=\foo`` since ``\foo`` has the right type. In case of
error, the token register is printed. According to Knuth this saves
space (well, in some sense). Unimplemented.

.. _cmd-openup:

\\openup
--------

In plain TeX, ``\displaylines`` and similar commands start with
``\openup1\jot``. This puts some extra space between the formula and the
text that precedes.

.. _cmd-vadjust2:

\\vadjust
---------

This command is similar to ```\insert`` <#cmd-insert>`__ (except that it
does not read an insertion number). “The constructed vertical list goes
into an adjustment item that is appended to the current list. However,
``\vadjust`` is not allowed in vertical mode. When an adjustment item
migrates from a horizontal list to the vertical list, the vertical list
inside the adjustment item is ‘unwrapped’ and put directly into the
enclosing list” (Citation from the `TeXbook <index.html#texbook>`__,
Chapter 24).

In *Tralics*, there is no conversion from an horizontal list into a
vertical list, so that this command provokes an error. Note that second
error may occur while scanning the brace. (see
`scantoks <doc-s.html#fct-scantoks>`__ for the syntax).
