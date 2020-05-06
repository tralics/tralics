===============================================
All packages known by Tralics, alphabetic order
===============================================

| This page contains the description of the following packages
  `12many <#p12many>`__, `abstract <#abstract>`__, `ae <#ae>`__,
  `aecompl <#aecompl>`__, `aeguill <#aeguill>`__, `alltt <#alltt>`__,
  `amsbsy <#amsbsy>`__, `amscd <#amscd>`__, `amsfonts <#amsfonts>`__,
  `amsgen <#amsgen>`__, `amsmath <#amsmath>`__, `amsopn <#amsopn>`__,
  `amssymb <#amssymb>`__, `amstext <#amstext>`__, `amsthm <#amsthm>`__,
  `amsxtra <#amsxtra>`__, `array <#array>`__, `bbding <#bbding>`__,
  `braket <#braket>`__, `calc <#calc>`__, `cgloss4e <#cgloss4e>`__,
  `checkend <#checkend>`__, `color <#color>`__, `comma <#comma>`__,
  `comment <#comment>`__, `coollist <#cool>`__, `coolstr <#cool>`__,
  `curves <#curves>`__, `datenumber <#datenumber>`__,
  `decimal <#decimal>`__, `delarray <#delarray>`__,
  `epigraph <#epigraph>`__, `esdiff <#esdiff>`__, `etex <#etex>`__,
  `eucal <#eucal>`__, `eufrak <#eufrak>`__, `fancyvrb <#fancyvrb>`__,
  `fix-cm <#fix-cm>`__, `fixfoot <#fixfoot>`__, `fixme <#fixme>`__,
  `fixltx2e <#fixltx2e>`__, `flafter <#flafter>`__, `float <#float>`__,
  `fp <#fp>`__, `fontenc <#fontenc>`__, `forloop <#forloop>`__,
  `gb4e <#gb4e>`__, `graphics, graphicx <#graphicx>`__,
  `graphpap <#graphpap>`__, `html <#html>`__, `ifpdf <#ifpdf>`__,
  `ifthen <#ifthen>`__, `index <#index>`__, `inputenc <#inputenc>`__,
  `keyval <#keyval>`__, `kvoptions <#voptionsl>`__,
  `latexsym <#latexsym>`__, `lipsum <#lipsum>`__,
  `makeidx <#makeidx>`__, `maple2e <#maple>`__, `mapleenv <#maple>`__,
  `mapleplots <#maple>`__, `maplestyle <#maple>`__,
  `mapletab <#maple>`__, `maple2util <#maple>`__,
  `minimal <#minimal>`__, `mml <#mml>`__, `natbib <#natbib>`__,
  `newlfont <#newlfont>`__, `nopageno <#nopageno>`__,
  `oldlfont <#oldlfont>`__, `overword <#overword>`__,
  `remreset <#remreset>`__, `ra <#ra>`__, `RR <#RR>`__,
  `shortvrb <#shortvrb>`__, `showidx <#showidx>`__, `soul <#soul>`__,
  `suffix <#suffix>`__, `syntonly <#syntonly>`__,
  `textcase <#textcase>`__, `textcomp <#textcomp>`__, `tipa,
  tipx <#tipa>`__ , `tloop <#tloop>`__, `theorem <#theorem>`__,
  `tracefnt <#tracefnt>`__, `tralics-iso <#tralics-iso>`__,
  `upref <#upref>`__, `xkeyval <#xkeyval>`__,
| and classes `article <#article>`__, `book <#book>`__,
  `report <#report>`__, `std <#std>`__,
| Click on DL in order to get the source.

--------------

.. _p12many:

12many
------

This package by Ulrich Schwarz, allows to type set intervals like 1..n
in different ways. The funny style ``laue`` is only partially
implemented. Exemple. You configure as
``\setOTMstyle[var=\iota, naturals=\mathbb{N},where=:] {set}`` and use
it as ``$\nto{12}{233}$``. Translation is
:math:`\left\{ \iota \in {\mathbb{N}}:12 \leq \iota \leq 233 \right\}`

.. _abstract:

abstract
--------

This package defines the ``abstract`` and ``onecolabstract``
environments, that translate into an XML element whose name is stored in
``\abstractname``. Options ``original``, ``addtotoc``, ``number``
``runin`` are currently ignored.

.. _ae:

ae
--

Dummy package, with one option ``slides``, that selects the Almost
European Computer Modern font in LaTeX.

.. _aecompl:

aecompl
-------

This package provides some additional character from the cmr font to the
ae font. Does nothing in Tralics.

.. _aeguill:

aeguill
-------

Dummy package, with options ``ec, cm, cyr, pl, lm``, whose purpose is to
add French guillements to the AE fonts, taken from the font given as
option.

.. _alltt:

alltt
-----

This package defines the environment shown in the example below. It is
like verbatim, but braces an backslashes have their default meaning.

.. container:: ltx-source

   ::

      \begin{alltt}
       $x=&#^_%~
       \textit{x=&#^_}$%
      \end{alltt}

.. _amsbsy:

amsbsy
------

The package provides two commands\ ``\boldsymbol`` and ``\pmb``, that
typeset their argument normally. Maybe, one day, a bold version will be
used.

.. _amscd:

amscd
-----

The package defines the CD environment, see example below. The
environment makes the atsign character active, it reads three identical
characters: less than, greater than for horizontal arrows, A, V for
vertical arrows, or a single one (vertical bar, equals sign, dot, or
``\vert``). Each atsign construct gives a double cell in the table.

.. container:: ltx-source

   ::

      \DeclareMathOperator\add{add}
      \DeclareMathOperator\cf {cf}
      \DeclareMathOperator\cov{cov}
      \DeclareMathOperator\non{non}
      \DeclareMathOperator\End{End}
      \[ \begin{CD}
           \cov (L) @>>> \non (K) @>>> \cf (K)  \\
             @VVV          @AAA          @AAA   \\
           \add (L) @>>> \add (K) @>>> \cov (K) \\
         \end{CD} \]

.. container:: ltx-source

   ::

      \[ \begin{CD}
           S^{W_\Lambda}\otimes T @>j>>      T       \\
            @VVV                        @VV{\End P}V \\
          (S \otimes T)/I         @=   (Z\otimes T)/J
         \end{CD} \]

.. _amsfonts:

amsfonts
--------

This package has an option psamsfonts, but does nothing.

.. _amsgen:

amsgen
------

This package is included in a lot of other ams packages. It defines some
short cuts ``\@xp``, ``\DN@``, ``\FN@`` ``\nx@``, and a command
``\@ifnotempty``.

.. _amsmath:

amsmath
-------

This package provides the following options: leqno, reqno, fleqn,
centertags, tbtags, intlimits, nointlimits, sumlimits, nosumlimits,
namelimits, nonamelimits, centertags, tbtags, cmex. The first five
options have as effect to add an attribute to the document. Other ones
have no effect.

Using this package defines a lot of unimplemented symbols. For instance
``\bbslash`` is a command taken from the LaTeX companion that should
provide a blackboard slash character. Some useful math commands that are
not in the kernel are given by this file.

.. _amsopn:

amsopn
------

This package provides the following options: namelimits and
nonamelimits. All commands defined by the LateX style file are in the
*Tralics* kernel.

.. _amssymb:

amssymb
-------

This package has an option amsfonts, but does nothing.

.. _amstext:

amstext
-------

This package does nothing.

.. _amsthm:

amsthm
------

This package does nothing.

.. _amsxtra:

amsxtra
-------

This package provides ``\sphat``, ``\spcheck``, ``\sptilde``,
``\spdot``, ``\spddot``, ``\spdddot``, ``\spbreve``, ``\spbreve``, as
well as the unimplemented command ``\accentedsymbol``.

array
-----

This is virtual package: this means that the following example works out
of the box.

.. container:: ltx-source

   ::

      \newcolumntype{L} {>{\large\bfseries 2}l <{y}|}
      \newcolumntype{+} {>{B}l <{D}|}
      \newcolumntype{_}{rlc<{x}}
      \newcolumntype{x}{>{b}c<{a}}
      \begin{tabular*}{10pc}[b]{lrc}
      \hline
      a&b&c\\[2pt]
      \multicolumn{1}{l}{A}&B&C\\\hline
      \end{tabular*}

.. _article:

article
-------

Standard article class.

.. _bbding:

bbding
------

This package defines some names for some Ding characters. For instance
``\PhoneHandset`` for ✆, ``\FiveStarCenterOpen`` for ✫,
``\FiveStarOpenDotted`` for ✬, and so on.

.. _book:

book
----

Standard article class.

.. _braket:

braket
------

This package provides ``\bra``, ``\ket``, ``\braket``, ``\set``, and a
large version (with capital initial letters). In the example that
follows, ``\left`` and ``\right`` delimiters are used, the vertical bars
are large in the case of the braket, but only the first one in the case
of the set.

.. container:: ltx-source

   ::

      $ \Braket{ \phi | \frac{\partial^2}{\partial t^2} | \psi } $
      $   \Set{ x\in\mathbf{R}^2 | 0<{|x|}<5 } $

calc
----

The calc package is built-in. The following example shows some
computations. In each case, the numbers on the left and right of the
equals signs are the same.

.. container:: ltx-source

   ::

      \newcounter{Ac}
      \setcounter{Ac}{(1+2)*(3+4)-20}\theAc=1
      \addtocounter{Ac}{(1*2)+(3*-4)+(34/7)}\theAc=-5

      \newlength{\Bc}
      \setlength{\Bc}{(1cm+2cm)*(3+4)-200mm} \the\Bc=28.4526pt
      \setlength\Bc{\the\Bc*\ratio{25.4pt}{722.7pt}} \the\Bc=0.99985pt
      \Bc=1in \setlength\Bc{\the\Bc * 100 / 7227} \the\Bc=0.99998pt
      \Bc=1in \setlength\Bc{\the\Bc * \real{ 0.01383700013837}} \the\Bc=1.00018pt
      \Bc=1cm \setlength\Bc{\the\Bc / \real{28.452755}} \the\Bc=0.99985pt
      \Bc=1cm \setlength\Bc{\the\Bc * \ratio{254pt}{7227pt}} \the\Bc=0.99985pt
      \Bc=1in \setlength\Bc{\the\Bc / \ratio{7227pt}{100pt}} \the\Bc=1.00018pt
      \Bc=1IN \setlength\Bc{\the\Bc / \ratio{7227PT}{100pT}} \the\Bc=1.00018pt  

.. _cgloss4e:

cgloss4e
--------

Experimental package. All the code is in reality in the kernel

.. _checkend:

checkend
--------

If some closing braces and the like are missing, *Tralics* will print a
line of the form *Save stack +40 -23* at the end of the job. A new item
is pushed whenever a variable must be restored (in particular the name
of an environment), and a new item is pushed for each boundary. Thus,
the previous indication does not tell how many errors are in the
document. The checkend package adds a special command (provided by
Tralics 2.11.1) to the ``\end{document}`` hook that pops the save stack,
and prints an error message whenever a boundary is found. It could be
like this.

.. container:: tty_out

   ::

      Error signaled at line 34 of file txt15.tex:
      Non-closed environment `aaa' started at line 28.
      Non-closed environment `ccc' started at line 27.
      Non-closed brace started at line 26.

.. _color:

color
-----

Support for colors in Tralics. Options debugshow, usenames, monochrome
are ignored. There are some driver options (dvips, xdvi, etc), all
ignored. Options dvipsnames or nodvipsnames tell whether some color like
Red are defined. Default depends on the driver.

.. _comma:

comma
-----

This package provides a command ``\commaform`` that typesetsthe content
of a counter with a comma very three digits; in fact it uses the content
of ``\commaformtoken``. The internal command ``\@commaform`` takes as
argument a number (or a reference to counter register). Example

.. container:: ltx-source

   ::

      \makeatletter\let\Commaform\@commaform\makeatother
      \countdef\CT0

      \setcounter{page}{12345}\commaform{page}
      \setcounter{page}{123456}\commaform{page}
      \setcounter{page}{1234567}\commaform{page}
      \CT=12345678 \Commaform\CT
      \def\commaformtoken{ }
      \Commaform{1234567890123456789012345678901234567890}

Translation

.. container:: xml_out

   ::

      12,345
      123,456
      1,234,567
      12,345,678
      1 234 567 890 123 456 789 012 345 678 901 234 567 890

.. _comment:

comment
-------

Implements ``\includecomment`` and ``\excludecomment`` as provided by
the html package.

.. _cool:

coollist
--------

coolstr
-------

These packages provide some functionalities for strings and lists

.. container:: ltx-source

   ::

      \ifstrleneq{abc}{3}{OK}{\error}
      \ifstrleneq{abc}{4}{\error}{OK}
      \strlen{abc} % typesets 3 
      \strlenstore{abc}{chapter} % stores 3 in the chapter counter
      \strchar{abcd}{3} % typesets third char of string, namely c 
      \substr{abcdefgh}{2}{3}  % typesets cde 
      \isdecimal{-2.345}{T} % sets boolean T to true 
      \isdecimal{2.345e6}{T} % sets boolean T to false 
      \isnumeric{2.345e6}{T} % sets boolean T to true 
      \isint{2.345e6}{T} % sets boolean T to false 
      \isint{-2345}{T} % sets boolean T to true 

      \listlen{1,2,3,4,5,6}% typesets 6 
      \listlenstore{chapter}{1,2,3,4}% stores 3 in the chapter counter
      \listval{11,22,33,44}{3}% typesets 33 
      \liststore{1,2,3,4}{temp}% fills \tempi;\tempii;\tempiii;\tempiv. 

.. _curves:

curves
------

The package defines commands like ``\curve``, ``\tagcurve`` etc.

.. _datenumber:

datenumber
----------

Provides functionalities on dates. You can set a date, for instance the
first of March 2008 by ``\setdatenumber{2008}{3}{1}``, manipulate it, so
that ``\prevdate\datedate`` gives 29th February 2008. The default
language is selected by a package option, but you can change it, for
instance ``\dateselectlanguage {USenglish} \datedate`` gives February
29, 2008.

.. _decimal:

decimal
-------

This package makes a dot active in math mode, transted as ``\cdot``. Use
``\.`` as a normal dot. Example
``$\left(  1.25 >0\.\right. \hbox{text\.s.}$``.

.. _delarray:

delarray
--------

The package defines the environment used in the example below. In LaTeX,
it redefines the array environment, but this is currently impossible in
Tralics.

.. container:: ltx-source

   ::

      $
      \begin{delarray}({cc})
      a&b\\c&d
      \end{delarray}
      $

.. _epigraph:

epigraph
--------

This package implements the ``\epigraph`` command and the ``epigraphs``
environment.

.. container:: ltx-source

   ::

      \epigraph{Example is the school of mankind, and they will learn at no other}
      {\textit{Letters on a Regicide Peace}\\ \textsc{Edmond Burke}}
      \begin{epigraphs}
      \qitem{A text}{An author}
      \qitem{A second text}{An author}
      \end{epigraphs}

Translation

.. container:: xml_out

   ::

      <epigraph>
        <epitext rend='flushleft'>
          Example is the school of mankind, and they will learn at no other
        </epitext>
        <episource rend='flushright'>
          <hi rend='it'>Letters on a Regicide Peace</hi>
          <newline/> 
         <hi rend='sc'>Edmond Burke</hi>
       </episource>
      </epigraph>
      <epigraphs>
        <epitext rend='flushleft'>A text</epitext>
        <episource rend='flushright'>An author</episource>
        <epitext rend='flushleft'>A second text</epitext>
        <episource rend='flushright'>An author</episource>
      </epigraphs>

.. _esdiff:

esdiff
------

This package lets you say ``$\diff{f}{x}$``, ``$\diff*[2]{g}{y}{0}}$``
and ``$\diffp{f}{{x}{y^2}}$``.

.. _etex:

etex
----

The etex package is a basic interface to some primitives provided by
eTeX. It provides some commands like ``\globcount``, ``\loccount``,
``\globcountblk``, and ``\loccountblk`` that are not yet implemented in
*Tralics*, and options ``localalloclog`` ``localallocnolog``,
``localallocshow`` that can be used for debugging purposes. The option
``noalloc`` is specific to *Tralics*, it inhibits the creation of these
commands that you cannot use. The package also redefines the way how
registers are allocated (not done in *Tralics*, since ``\newdimen`` and
the like can use 1024 registers.

The package provides ``\loggingall`` that is the same as
``\tracingall``, but without printing anything on the terminal, and
``\tracingnone`` that removes tracing; note that these three commands
change the counters associated to tracing: macros, output, commands,
restores, assigns, math, but nothing else (not used by *Tralics*). The
package provides a ``\eTeX`` command, and symbolic names for grouptypes,
interactionmodes, nodetypes, and iftypes provided that the option is
selected.

In the example that follows, the last call to ``\F`` is likely to
produce an error message, because *Tralics* handles all boxes alike.

.. container:: ltx-source

   ::

      %\usepackage[grouptypes,iftypes]{etex}
      \def\F#1{\ifnum\currentgrouptype=#1\else\typeout{Bug \string#1}\fi}
      \F\bottomleveltype{\F\simplegrouptype\begingroup\F\semisimplegrouptype
      \hbox{\F\hboxgrouptype}\endgroup}

      \def\G#1{\edef\x{\the\currentiftype}\ifnum\x=#1\else\typeout{Bug \string#1}\fi}
      \if00\G\charif\fi\ifx00\G\xif\fi \ifnum0=0 \G\numif\fi

.. _eucal:

eucal
-----

This package defines options psamsfonts, mathcal and mathsrc. It defines
``\EuScript`` as a synonym to ``\mathcal``.

.. _eufrak:

eufrak
------

This package defines option psamsfonts. It defines ``\EuFrak`` as a
synonym to ``\mathfrak``.

.. _fancyvrb:

fancyvrb
--------

This package provides ``\DefineVerbatimEnvironment`` and some keywords
used for the *Tralics* documentation. The real code is defined in the
kernel.

.. _fix-cm:

fix-cm
------

Dummy package.

.. _fixfoot:

fixfoot
-------

Provides command ``\DeclareFixedFootnote``.

.. _fixme:

fixme
-----

Provides commands like ``\fixme``.

.. _fixltx2e:

fixltx2e
--------

Dummy package.

.. _flafter:

flafter
-------

Dummy package.

.. _float:

float
-----

This package provides the ``\newfloat`` command.

.. _fp:

fp
--

All operations defined by the fp package are implemented in C++. The two
options ``nomessages`` and ``debug`` do nothing. In the example below
``\foo`` and ``\xfoo`` contain -84, assuming that ``\mthree`` holds -3,
and ``\yfoo`` has the value indicated.

.. container:: ltx-source

   ::

      \FPupn\foo{ 3 abs mthree abs 3 sgn 10 * mthree sgn 100 * + + +}\testeq\foo{-84.}
      \FPeval\xfoo{abs(3) + abs(-3) + (sgn(3)* 10) + (sgn(-3) * 100)}\testeq\xfoo{-84.
      \def\yfoo{1}
      \def\xbar#1 #2{\FPmul\yfoo{\yfoo}{#1}%
         \let\next\xbar\ifx#2\relax\let\next\relax\fi\next#2}
      \xbar 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 \relax
      %\yfoo is 121645100408832000.

.. _fontenc:

fontenc
-------

Dummy package.

.. _forloop:

forloop
-------

Provides ``\forloop``.

.. _gb4e:

gb4e
----

Experimental package. Loads cgloss4e

.. _graphicx:

graphics
--------

The ``\includegraphics`` command is built-in in *Tralics* with the
semantics of the graphicx package. A few related commands
(``\resizebox``, ``\reflectbox``, ``\resizebox``) are defined by the
graphics package. The three commands ``\graphicspath``,
``\DeclareGraphicsExtensions`` and ``\DeclareGraphicsRule`` are also
defined but their arguments are ignored. The graphicx package loads
keyval if needed, it defines ``\rotatebox`` and allows declarations as
``\setkeys{Gin}{width=20pt}``.

.. _graphpap:

graphpap
--------

Dummy package.

.. _html:

html
----

This package provides some commands like ``\htmladdfnormallink`` that
are variants of ``\href``, environments like ``htmlonly`` that behave as
comments, and a lot of other functions that do nothing.

.. _ifpdf:

ifpdf
-----

Provides a boolean ``\iffpdf`` whose value is false.

.. _ifthen:

ifthen
------

The ``\ifthenelse`` command is builtin. This package provides a variant
``\Ifthenelse`` that redefines ``\value`` in a strange way (the same as
the LaTeX code).

.. container:: ltx-source

   ::

      \newcommand{\QU}[2]{%
      \ifthenelse {
        \(\equal{#1}{ENG} \and \equal{#2}{yes} \)
           \or
        \(\equal{#1}{FRE} \and \equal{#2}{oui} \)
      }{``OK''}{``not OK''}}
      You agree \QU{ENG}{yes} or don't \QU{ENG}{no}. 
      D'accord \QU{FRE}{oui} ou pas \QU{FRE}{non}. 

.. _index:

index
-----

This package provides a multiple index mechanism. You can declare a new
index via ``\newindex``, add an attribute to the index via
``\addattributetoindex``, and use the commands ``\printindex`` or
``\index`` with an optional argument.

.. container:: ltx-source

   ::

      \newindex{A}{}{}{Second Index}
      \addattributetoindex[A]{head}{Second Index}
      \printindex[A]
      Text\index[A]{Foo} 
      and more text\index[A]{FOO!Bar}

.. _inputenv:

inputenc
--------

This package recognises options utf8, latin1, ascii, ansinew, applemac,
cp1250, cp1252, cp1257, cp437, cp437de, cp865, cp850, cp852, cp858,
decmulti, latin10, latin2, latin3, latin4, latin5, latin9, macce, next.
It defines ``\IeC``, and a command ``\inputencoding`` a command that
takes as argument an encoding name defined as an option, and uses this
as enconding for the current file.

.. _keyval:

keyval
------

This package implements a system of defining and using sets of
parameters, which are set using the syntax ``key=value``. It requires at
lest version 2.11.1, since the ``\define@key`` command is written in
C++. It takes 3 arguments (plus an optional one), see example below. The
parser of ``\setkeys`` is also builtin. It splits a sequence of tokens
at commas, and for each subsequence, splits into a key=val pair. Spaces
are removed around the key and the value, as well as a pair of braces,
as shown be the example. The package has an option that says wether an
undefined key is an error (default) or just a warning.

.. container:: ltx-source

   ::

      \makeatletter
      \define@key{kv}{foo}{\Foo{#1}} %% foo=x calls \Foo{x}
      \define@key{kv}{bar}[dbar]{\Bar{#1}} %% bar,bar=y calls \Bar with dbar or y.
      \makeatother 

      \def\Foo#1{{seen#1in Foo}}
      \def\Bar#1{seen#1in Bar}
      \setkeys{kv}{foo = 1, bar = 2, bar}
      \setkeys{kv}{ foo = {\it x }, foo = {{\it y} }, }

Translation

.. container:: xml_out

   ::

      <p>seen1in Fooseen2in Barseendbarin Bar
      seen<hi rend='it'>x in Foo</hi>seen<hi rend='it'>y</hi> in Foo</p>

.. _kvoptions:

kvoptions
---------

This package defines ``\DeclareBoolOption``,
``\DeclareComplementaryOption``, ``\DeclareStringOption``,
``\DeclareVoidOptions`` that can be used in a package. There is also a
command ``\DisableKeyvalOption`` that allows you to disable an option.

.. _latexsym:

latexsym
--------

Dummy package.

.. _lipsum:

lipsum
------

This package provides a command that can be used as ``\lipsum``,
``\lipsum[10-14]``, or ``\lipsum[12]``, and produces some paragraphs
from a dummy text. The command ``\setlipsumdefault`` can be used to set
the range if no argument is given (default is 1-7), valid range is 1 to
150.

.. _makeidx:

makeidx
-------

This package defines ``\see``, ``\seealso``, ``\seename``, and
``\alsoname``.

.. _maple:

maple2e, mapleenv, mapleplots, maplestyle, mapletab, mapleutil
--------------------------------------------------------------

A bunch of experimental packages that can be used to typeset code
generated by maple

.. _minimal:

minimal
-------

This is a dummy class.

.. _mml:

mml
---

This package provides a lot of commands, for instance ``\mmlIntegers``
that produces <integers> it can be used in math mode, for content markup
MathML.

.. _natbib:

natbib
------

Provides features of natbib.sty

.. container:: ltx-source

   ::

      \citeyear{xml} \citeauthor{xml} \citefullauthor{xml}
      \cite{xml}\cite{xml}

.. _newlfont:

newlfont
--------

Provides redefinition of commands like ``\rm``.

.. _nopageno:

nopageno
--------

This adds the attribute pair ``page-numbering='false'`` to the root
element.

.. _oldlfont:

oldlfont
--------

Dummy package.

.. _overword:

overword
--------

This package provides two macros which can be used as building blocks
for the parsing of text. It forms part of the Calendar package, but is
written as a discrete module that may be used in other packages as well.
An example of use is given at the end of the package.

.. _ra:

ra
--

Files needed for typeseting Inria's Activity Report. Since 2007, there
is a class file for each year, named ``ra2007``, etc. It has no option.
This class loads the ``ra`` package, with some options, depending on the
year. The package defines ``\persA``, ``\persB``, see documentation, and
make ``\pers`` equivalent to ``\persB``, unless option ``old`` is given,
case where ``\persA`` is used. The option ``catperso`` defines a
catperso environment. Four environments ``participantXXX`` are defined
(see `documentation <doc-p.html#env-participant>`__) where XXX is empty,
or ``s`` or ``e`` or ``es``. By default, they are identical, producing a
``participants`` element; using option ``participant`` changes this: the
name of the XML element is the name of the environment.

.. _report:

report
------

Standard article class.

.. _remreset:

remreset
--------

If you say ``\newcounter{C}[A]`` then counter C is reset whenever
counter A is stepped. The package provides the construction
``\@removefromreset{C}{A}``, that removes C from the list of counters to
be reset. Note the invsere construction ``\@addtoreset{C}{A}`` is
implemented by default.

.. _RR:

RR
--

The package needed for typeseting Inria's Research Reports

.. _shortvrb:

shortvrb
--------

Defines ``\MakeShortVerb`` and ``\DeleteShortVerb\`` in terms of
primitives ``\DefineShortVerb`` and ``\UndefineShortVerb``.

.. _showidx:

showidx
-------

Dummy package.

soul
----

Some commands from this package are built-in.

.. _std:

std
---

This is not a class/package, but a file shared between different classes

.. _suffix:

suffix
------

Package by D. Kastrup. It allows to construct macros with suffixes; in
the example that follows, we have a ``\snarf`` command, and extend it
twice. We use it, put the content in a box, and print the box. We then
show the command, so as to see how it is redefined. The last two lines
contain two macros that can be used to get the name of the commands that
do the real job.

.. container:: ltx-source

   ::

      \def\snarf#1{A#1}
      \WithSuffix\long\def\snarf[#1]{B#1}
      \WithSuffix\long\def\snarf(#1){C#1}
      \setbox0\hbox{\snarf{1}\snarf[2]\snarf(3)\snarf=4}
      \showbox0
      \show\snarf
      \expandafter\show\csname\SuffixName\snarf[\endcsname
      \expandafter\show\csname\NoSuffixName\snarf\endcsname

This is what *Tralics* prints on the terminal. Note the funny names.

.. container:: tty_out

   ::

      A1B2C3A=4=
      \snarf=\protected macro: ->\WSF@suffixcheck \snarf .
      \WSF:\snarf the character [=\long macro: #1]->B#1.
      \WSF:\snarf=macro: #1->A#1.

.. _syntonly:

syntonly
--------

Dummy package.

.. _textcase:

textcase
--------

This package implements ``\MakeTextUppercase`` and
``\MakeTexLowercase``, as well as ``\NoCaseChange``. In the example that
follows, the first line should be translated as ``foo Bar $Xy$``. The
package has a single option: overload, when used, it redefines
``\MakeUppercase`` and ``\MakeLowercase``. Without the option, the
second math formula is in uppercase, with it, it is lowercase.

.. container:: ltx-source

   ::

      \MakeTextLowercase{Foo \NoCaseChange{Bar} $Xy$ }
      \MakeUppercase{Foo $Xy$ }

.. _textcomp:

textcomp
--------

This package declares options: full, almostfull, euro, safe, force, and
warn. It does defines some commans like ``\textsevenoldstyle`` to
translate into ``<char name='sevenoldstyle'/>``. There is no Unicode
characters for these TeX characters.

.. _tipa:

tipa
----

The tipa package provides some characters and accents used for IPA
(International Phonetic Alphabet). The tipx package contains additional
commands. In LaTeX, two font encodings T3 and TS3 are associated to
these package, and ``\textipa{SO""}`` produces the same as
``\textesh\textopeno\textsecstress``. This mechanism is not yet
implemented. See `HERE <doc-a.html#accents>`__ for some examples.
Options extra, tone, T1, noenc, do nothing. Option safe is used by
default. If you want unsafe mode, use the command ``\tipaunsafemode``.

.. _tloop:

tloop
-----

This package provides the function ``\tloop`` with two arguments, range
and code. The range is fully expanded, should be of the form a-b; The
internal counter ``\tloop@ctr`` ranges from a to b inclusive (at least
one iteration is performed). The loop in the following example produces
``3=iii 4=iv 5=v``.

.. container:: ltx-source

   ::

      \count0=3 \count1=5 
      \def\range{\the\count0-\the\count1}
      \makeatletter
      \tloop\range{\the\tloop@ctr=\romannumeral\tloop@ctr\space}
      \tloop{1-3}{{\Tloop{1-3}{\the\tloop@ctr\the\Tloop@ctr\space }}}

The package provides also ``\Tloop`` that is similar; this allows you to
put loops into loops. The example that follows creates a 3x3 table with
ij at row i column j.

.. container:: ltx-source

   ::

      \def\x{}
      \def\addtox#1{\xdef\x{\x#1}}
      \tloop{1-3}{\ifnum\tloop@ctr>1\addtox{\\}\fi
                 {\Tloop{1-3}{\ifnum\Tloop@ctr>1\addtox{&}\fi
                  \addtox{\the\tloop@ctr\the\Tloop@ctr}}}}
      \begin{tabular}{ccc}\x\end{tabular}

.. _theorem:

theorem
-------

This is a virtual package. The example that follows requires no package
at all.

.. container:: ltx-source

   ::

      \theorembodyfont{\sl}
      \theoremstyle{break}
      \newtheorem{Cor}{Corollary}
      \setcounter{section}{17}
      \newtheorem{Exa}{Example}[section]
      \begin{Cor}
       This is a sentence typeset in the theorem environment \texttt{Cor}.
      \end{Cor}
      \begin{Exa}
       This is a sentence typeset in the theorem environment \texttt{Exa}.
      \end{Exa}

.. _titlepage:

titlepage
---------

This package implements some commands that can be used for a thesis
title page.

.. _tracefnt:

tracefnt
--------

This package declares options: errorshow, warningshow, infoshow,
debugshow, loading, infoshow, and pausing. It does nothing.

.. _tralics-iso:

tralics-iso
-----------

This package defines a lot of commands like ``\mathslarr`` that should
be used in math mode; translation is something like &#x2190; This
character is called \`Leftwards arrow' in the Unicode documentation and
\`short left arrow' in the iso9573-13, hence the command name. Your
browser may show the character as ←.

.. _upref:

upref
-----

Dummy package.

.. _xkeyval:

xkeyval
-------

Dummy package.
