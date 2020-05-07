All commands, alphabetic order; letter W
========================================

This page contains the description of `\\wd <#cmd-wd>`__,
`\\wedge <#cmd-wedge>`__, `\\whiledo <#cmd-whiledo>`__,
`\\@whiledim <#cmd-whiledim>`__, `\\@whilenum <#cmd-whilenum>`__,
`\\@whilesw <#cmd-whilesw>`__, `\\widehat <#cmd-widehat>`__,
`\\widetilde <#cmd-widetilde>`__, `\\@width <#cmd-width>`__,
`\\widowpenalties <#cmd-widowpenalties>`__,
`\\widowpenalty <#cmd-widowpenalty>`__, `\\widthof <#cmd-widthof>`__,
`\\wlog <#cmd-wlog>`__, `\\wp <#cmd-wp>`__, `\\wr <#cmd-wr>`__,
`wrapfigure, wraptable <#env-wrapfigure>`__, `\\write <#cmd-write>`__.

--------------

.. _cmd-wd:

\\wd
----

A command like ``\wd0=2\wd1`` says to replace the width of the box
number 0 by twice the width of box number one. See
`scanint <doc-s.html#fct-scanint>`__ for how integers are read (here the
integer is zero, it should be a number between 0 and 255 in TeX, 1023 in
*Tralics*). See `scandimen <doc-s.html#fct-scandimen>`__ for how
dimensions are read (here ``2\wd1`` is the dimension).

Changing the dimension of the box does not change its content: if you
say ``\hbox to 0pt{...}``, you will get an overflow hbox, but if you say
``\setbox0= \hbox{...}\wd0=0pt \box0``, you will get a box that is
neither overfull nor underfull, the result is of width zero. This is
more or less the same as ``\hbox to 0pt{...\hss}``.

*Tralics* does not use nor remember box dimensions. In the example above
``2\wd1`` evaluates to 0pt, and modifying ``\wd0`` does not modify the
box.

.. _cmd-wedge:

\\wedge (math symbol)
---------------------

The ``\wedge`` command is valid only in math mode. It generates a binary
operator: ``<mo>&wedge;</mo>`` (Unicode U+2227, ∧). See description of
the ```\pm`` <doc-p.html#cmd-pm>`__ command.

.. _cmd-whiledo:

\\whiledo
---------

The ``\whiledo`` command is provided by the ifthenelse package. It takes
two arguments: a test and a body. The test is evaluated via
```\ifthenelse`` <doc-i.html#cmd-ifthenelse>`__. It if is true, the body
is evaluated, and the test is done again.

The original LaTeX definition uses the ``\@whilesw`` command, and a
special boolean ``\if@whiledo``. It is shown here.

.. container:: ltx-source

   ::

      \makeatletter
      \newif\if@whiledo
      \long\def\whiledo#1#2{%
        \ifthenelse{#1}%
          {\@whiledotrue
           \@whilesw\if@whiledo\fi
             {#2%
              \ifthenelse{#1}\@whiledotrue\@whiledofalse}}%
          {}%
        }

Here is an example of code

.. container:: ltx-source

   ::

      \newcounter{ca}\newcounter{cb}
      \newcommand{\printgcd}[2]{%
        \setcounter{ca}{#1}\setcounter{cb}{#2}%
        Gcd(#1,#2) = 
        \whiledo{\not\(\value{ca}=\value{cb}\)}%
          {\ifthenelse{\value{ca}>\value{cb}}%
            {\addtocounter{ca}{-\value{cb}}}%
            {\addtocounter{cb}{-\value{ca}}}%
          gcd(\arabic{ca},\arabic{cb}) = }%
        \arabic{ca}.}
      \printgcd{144}{272}

The translation is the following. |gcd|

This is another way for the same command. The first time we enter
``\@whilesw``, the test is true. Here we print the value at the start of
the loop instead of the end.

.. container:: ltx-source

   ::

      \newif\iftest
      \newcommand{\printgcd}[2]{%
        \setcounter{ca}{#1}\setcounter{cb}{#2}%
        \testtrue
        \@whilesw\iftest gcd(\arabic{ca},\arabic{cb}) = \fi{%
         \ifnum\value{ca}=\value{cb}\testfalse\else
          \ifnum\value{ca}>\value{cb}%
            \addtocounter{ca}{-\value{cb}}\else
            \addtocounter{cb}{-\value{ca}}\fi
      \fi}%
        \arabic{ca}.}
      \printgcd{144}{272}

Here we use ``\@whilenum``. The code is: while C is less than D, then
say A=C, B=D-C. In any case, put in C the smallest of A, B, and in D the
largest. The big trouble is that ``\@whilenum`` compares two numbers,
and the test is less-than, greater-than and equal-than. There is no
different-from. You just cannot say: \`while A is different from B do'.

.. container:: ltx-source

   ::

      \newcounter{cc}\newcounter{cd}
      \def\assigncounter{%
       \ifnum\value{ca}<\value{cb}%
        \setcounter{cc}{\value{ca}}%
        \setcounter{cd}{\value{cb}}%
       \else 
        \setcounter{cc}{\value{cb}}%
        \setcounter{cd}{\value{ca}}%
        \fi}

      \def\subtractcounter{%
       \setcounter{cb}{\value{cd}}%
       \setcounter{ca}{\value{cc}}%
       \addtocounter{cb}{-\value{cc}}}%
      \let\printgcd\relax
      \newcommand{\printgcd}[2]{%
        \setcounter{ca}{#1}\setcounter{cb}{#2}%
        \assigncounter
        \@whilenum \value{cc}<\value{cd} Gcd(\arabic{cc},\arabic{cd}) = \do{%
           \subtractcounter\assigncounter}%
        \arabic{ca}.}
      \printgcd{144}{272}

This is the same code, but we use dimensions. Note how dimensions are
converted to integers and vice-versa.

.. container:: ltx-source

   ::

      \newlength\cC\newlength\cD
      \def\assignlength{%
       \ifnum\value{ca}<\value{cb}%
        \setlength\cC{\value{ca}sp}%
        \setlength\cD{\value{cb}sp}%
       \else 
        \setlength\cC{\value{cb}sp}%
        \setlength\cD{\value{ca}sp}%
        \fi}

      \def\subtractlength{%
       \setcounter{cb}{\cD}%
       \setcounter{ca}{\cC}%
       \addtocounter{cb}{-\cC}}%
      \let\printgcd\relax
      \newcommand{\printgcd}[2]{%
        \setcounter{ca}{#1}\setcounter{cb}{#2}%
        \assignlength
        \@whiledim \cC<\cD Xgcd(\arabic{ca},\arabic{cb}) = \do{%
           \subtractlength\assignlength}%
        \arabic{ca}.}
      \printgcd{144}{272}

.. _cmd-whilesw:

\\@whilesw
----------

You can say ``\@whilesw test \fi{body}``. Here ``test`` is an incomplete
conditional, and the ``\fi`` token must be explicit. The body is
executed as long as the test is true. See above
```\whiledo`` <doc-w.html#cmd-whiledo>`__ above. This command is defined
in the LaTeX kernel, although not described in the books.

.. _cmd-whilenum:

\\@whilenum
-----------

You can say ``\@whilenum test etc \do{body}``. Here ``test`` is an
incomplete conditional, of the form number-op-number (where op is one of
< > or =) and the ``\do`` token must be explicit. The body is executed
as long as the test is true. See above
```\whiledo`` <doc-w.html#cmd-whiledo>`__ above. This command is defined
in the LaTeX kernel, although not described in the books.

.. _cmd-whiledim:

\\@whiledim
-----------

You can say ``\@whiledim test etc\do{body}``. Here ``test`` is an
incomplete conditional, of the form dimen-op-dimen (where op is one of <
> or =) and the ``\do`` token must be explicit. The body is executed as
long as the test is true. See above
```\whiledo`` <doc-w.html#cmd-whiledo>`__ above. This command is defined
in the LaTeX kernel, although not described in the books.

.. _cmd-widetilde:

\\widetilde (math accent)
-------------------------

The ``\widetilde`` command puts a tilde accent over a kernel. It is
(currently in *Tralics*) equivalent to ``\tilde``. It works only in math
mode. See also the ```\acute`` <doc-a.html#cmd-acute>`__ command.

.. _cmd-widehat:

\\widehat (math accent)
-----------------------

The ``\widehat`` command puts a hat accent over a kernel. It is
(currently in Tralics) equivalent to ``\hat``. It works only in math
mode. See also the ```\acute`` <doc-a.html#cmd-acute>`__ command.

.. _cmd-widowpenalties:

\\widowpenalties (internal integer)
-----------------------------------

This command is an eTeX extension to *Tralics*, that reads or writes a
slot in a dynamically allocated table. The syntax is the following. In a
set context, an equals is read, followed by an integer *n*. If the
integer is positive, then *n* integer values are read and stored,
otherwise the table is cleared. In a get context, an integer *n* is
read, and the result is an integer; if *n* is negative, this is zero, if
*n* is zero it is the length of the table, if *n* is positive it is the
value found in the table (or the last value if *n* is too big). (See
`scanint <doc-s.html#fct-scanint>`__ for details of argument scanning).

The sequence ``\widowpenalties=3 101 102 103`` reallocates the table
associated to widow penalties, and puts 101, 102 and 103 in it. The
sequence ``\the\displaywidowpenalties 0`` typesets the value at position
0 in the table, namely 101. The table is not used by *Tralics*. In the
code that follows, the macro ``\bad`` is not called.

.. container:: ltx-source

   ::

      \interlinepenalties=3 1 2 3
      \clubpenalties=3 11 12 13
      \widowpenalties=3 101 102 103
      \displaywidowpenalties=3 1001 1002 1003
      \widowpenalties= -1
      \edef\foo{%
      \the\interlinepenalties 1
      \the\clubpenalties\interlinepenalties2
      \the\displaywidowpenalties -1
      \the\displaywidowpenalties 0
      \the\displaywidowpenalties 4
      \the\widowpenalties 0}
      \def\xfoo{1120310030}
      \ifx\foo\xfoo\else\bad\fi

.. _cmd-widowpenalty:

\\widowpenalty (internal integer)
---------------------------------

When you say ``\widowpenalty=100``, then TeX will use 100 as penalty for
a lonely line at the top of a page (line not followed by an equation)
when it computes optimal page breaks. Unused by *Tralics*. (See
`scanint <doc-s.html#fct-scanint>`__ for details of argument scanning).

.. _cmd-width:

\\@width
--------

This command expands to ``width``. Do not modify it.

.. _cmd-widthof:

\\widthof
---------

This command is defined by the \`calc' package but not implemented in
*Tralics*.

.. _cmd-wlog:

\\wlog
------

The command ``\wlog`` behaves like ```\write-1`` <#cmd-write>`__. This
means that the command reads a list of tokens, and prints it (expanded)
to the transcript file.

.. _cmd-wp:

\\wp (math symbol)
------------------

The ``\wp`` command is valid only in math mode. It generates a
miscellaneous symbol: ``<mi>&wp;</mi>`` (Unicode U+2118, ℘). See
description of the ```\ldots`` <doc-l.html#cmd-ldots>`__ command.

.. _cmd-wr:

\\wr (math symbol)
------------------

The ``\wr`` command is valid only in math mode. It generates a binary
operator: ``<mo>&wr;</mo>``, (Unicode U+2240, ≀). See description of the
```\pm`` <doc-p.html#cmd-pm>`__ command.

.. _env-wrapfigure:

wrapfigure, wraptable (environment)
-----------------------------------

These two environments are extensions of ``figure`` and ``table``.

.. container:: ltx-source

   ::

      \begin{wrapfigure}[12]{r}[34pt]{5cm} FIG \end{wrapfigure}
      \begin{wraptable}[12]{r}[34pt]{5cm} TABLE \end{wraptable}
      \begin{wrapfigure}{r}{5cm} FIG \end{wrapfigure}
      \begin{wraptable}{r}{5cm} TABLE \end{wraptable}

.. container:: xml_out

   ::

      <figure id-text='2' id='uid48' width='5cm' overhang='34pt' place='r' narrow='12'>
        <p rend='center'>FIG</p>
      </figure>
      <table rend='display' id-text='1' id='uid49' width='5cm' overhang='34pt' place='r' narrow='12'>
        <unexpected>TABLE</unexpected>
      </table>
      <figure id-text='3' id='uid50' width='5cm' place='r'>
        <p rend='center'>FIG</p>
      </figure>
      <table rend='display' id-text='2' id='uid51' width='5cm' place='r'>
        <unexpected>TABLE</unexpected>
      </table>

.. _cmd-write:

\\write
-------

After ``\write`` comes a number and a general text (see the
`scanint <doc-s.html#fct-scanint>`__ command for details of parsing) the
number and `scantoks <doc-s.html#fct-scantoks>`__ for the other
argument). The effect is to write the second argument in the file
specified by the first argument. See
```\openout`` <doc-o.html#cmd-openout>`__ for further information).

.. |gcd| image:: /img/img_79.png
