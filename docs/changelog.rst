ChangeLog for the Tralics software
==================================

Work in progress
----------------

Bookkeeping with no functional change
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  Lots of refactoring of the C++ code using ``clang-tidy`` and
   ``clang-format``, making use of some C++17 features (too many changes
   to list, many of them automated)
-  Merging of the various documentation files, reorganization of the
   directory structure to make it more readable
-  Transition to CMake, and automatic run of the tests as part of the
   build
-  Remove a few unused function parameters and some dead code

Changes affecting the output
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

-  [2020-02-12] import a
   `patch <https://github.com/softcover/tralics/commit/5164df1cdf978da0e8eb28d38b6e8fd2baf8f47b>`__
   from the SoftCover fork which includes missing refs in links for
   reference (only changes the output if there is an error)
-  [2020-04-21] switch to ``spdlog`` for logging, this changes the
   format and the repartition of the contents, hopefully in a reasonable
   direction

2015-11-25, Version 2.15.4
--------------------------

-  [2016-01-04] Bug correction in environment ``subequation``
-  [2017-05-26] Added command ``\omitcite``

2015-08-06, Version 2.15.3
--------------------------

- Corrected error message in ``\newcounter`` overflow.
- The Raweb requires no more a ``theme_vals`` entry in the config file.
- Made ``\@firstofone`` expandable.
- Corrected a bug in ``\ra@year`` in non-ra mode.
- What comes after ``\end{document}`` is ignored.
- The last line in the XML file contains only the end of the document element.
- Removed useless code of module check.
- An external program is called for the bibliography only if ``write18`` is enabled.
- Single error signaled in case of ``\newcommand{\foo\bar}{}``.
- Corrected ``\string^^^^1234``.
- Corrected ``\expandafter\string\csname\endcsname``.
- Handles correctly the case ``\escapechar=0``.
- Accepts ``\@gobble{\par}``.
- For many command that read an optional argument, paragraphs are forbidden.
- In case of ``\def\foo#1 #2{}\foo1 2``, both spaces can be replaced by a new line.
- Command ``\newcount`` never allocates register 255.
- Count registers no more used for allocation commands like ``\newcount``.
- Internal: made ``\outer`` tests faster than ``\protected``
- Corrected bug in ``\marks``.
- Simplified ``\message``.
- Corrected error handling in ``\def\foo#1#2{#1}\foo\bad\par`` the second argument gives a runaway; this stops argument scanning and inhibits evaluation of body.
- Indicates a corrects size for virtual files.
- No seg fault for ``\newif~`` or ``\newif\~``.
- Width of minipage read as dimen, not glue; no error anymore in ``\begin{minipage}{2pt plus}abc\end{minipage}``.
- Corrected behavior of newline in detokenize: ``\detokenize{^^J}`` gives a newline character of category code 12.
- Changed ``\meaning\sixt@@n\meaning\@M ;`` yields now ``\char"10\mathchar"2710``.
- Corrected ``\newmuglue``.
- No more element ``<error n='invalid.' l='570' c='no module'/>`` if same raweb sections are empty.
- General cleanup.
- Improved trace for restoring values; corrected printing of mu glue.
- Implemented subequations environment.
- Redesigned math array handling.
- Removed ``array*`` environment.
- Corrected numbering of multline environment.
- Made ``\nonumber`` a synonym of ``\notag``.
- Added option ``multi_math_label`` and counter ``\@multi@math@label``.
- No column span attribute for ``\multicolumn{a}{b}{c)`` when ``a=1``.
- One error per extra ``\label`` in math mode.
- Implemented ``\anchorlabel`` in math mode.
- Array preamble ``{>{$} c<{$}}`` works.
- Implemented lots of command for ``LaTeX3``.

2015-01-22, Version 2.15.2
--------------------------

-  corrected a bug in printing the encoding of the transcript file
   (David Binderman).
-  If the save stack holds a font command, this is popped at the end of
   the process (it might be pushed by a global ``\normalfont``).
-  The types of the objects remaining on the save stack is printed on
   the transcript file at the end of the process.
-  [2015-06-24] Support for math symbols added.
-  [2015-07-14] Changed implementation of ``\cline``.
-  Added ``\@sptoken`` to the kernel.
-  New command ``\hlinee`` as extension of ``\hline \cline``; this allow
   to specify the thickness of a rule and its surrounding space in a
   table.
-  Implemented the booktabs package
-  Corrected french translation of ``\aparaitre``.
-  Corrected ``\textasciibreve``, ``\textasciicaron``.
-  Corrected a nasty bug that inhibits tags in align environment (or any
   usee defined environment that expands to ``@align``).
-  Implemented ``\XMLcurrentcellid`` ``\XMLcurrentrowid``
   ``\XMLcurrentarrayid``.
-  Removed the obsolete NoTitle hack.
-  Corrected the postprocessing, removing loops in unused XML elements.
-  Corrected ``\vcenter``.
-  Changed ``\xmlemptyelt``.
-  Added a bunch of commands that make access to the XML tree
   (``\XML@fetch``, ``\XML@parent``, ``\XML@insert`` etc (for details,
   see documentation of one of these commands).
-  Added the possibility to use the result of ``\readxml``.
-  No error in ``$\muskip0=\lastskip$``.
-  An error is signaled in ``$a\above-ptb$``.
-  No error is signaled in ``$a\overwithdelims\relax()b$``.
-  There is always a rend attribute in figure, table.
-  No more loss of attributes in
   ``\begin{table} \begin{tabular*}{17pt}[b] {c} y \end{tabular*}\end{table}``.

2013-07-22, Version 2.15.1
--------------------------

-  Removed some unused variables in the code (Julien Schueller).
-  There are no more affiliations in the RA2013 (Marie-Pierre Durollet).
-  Replaced 2007 by current year in The configuration file for the RA is
   ``ra2007.tcf`` or ``ra.tcf``.
-  [2014-03-31] A module cannot start in a paragraph; implicit ``\par``
   added (Marie-Pierre Durollet).

2012-08-31, Version 2.15.0
--------------------------

-  [2013-01-21] Number of pages always inserted in the bibliography.

2012-07-06, Version 2.14.5
--------------------------

-  Added a possibility to specify an XML style sheet.
-  No terminator needed after a display math formula in interactive mode
   (Allen Downey).
-  Sections in raweb mode may be mandatory.

2011-11-07, Version 2.14.4
--------------------------

-  Removed final ``<allowbreak/>`` in url [2012-01-19, V Lefevre].
-  IO encoding easier to understand in the transcript file.
-  Added an option ``-dualmath``, and ``-nomathml`` reverted to old
   meaning.
-  Changed printing of some characters.
-  Extended the four-hat notation to five hats.
-  Implemented a starred version ``\url*``.
-  Added the ``\grabenv`` command.
-  Default output encoding is UTF8.
-  Translation of ``\phi`` is ``phiv`` or ``&#x3D5;`` i.e., ϕ,
   translation of ``\varphi`` also changed.
-  Translation of ``\longmapsfrom`` ``\Longmapsfrom`` ``\longmapsto``
   ``\Longmapsto`` is now ``&#x27FB;`` ``&#x27FD;`` ``&#x27FC;``
   ``&#x27FE;`` giving ⟻ ⟽ ⟼ ⟾ (Heiko Schroeder).

2011-05-02, Version 2.14.3
--------------------------

-  Fixed the font/size/color in ``\footnote``.
-  Allows ``\ChangeElementName{use\_font\_elt}{true}``.
-  Implemented ``\appendix`` in classes.
-  Commands like ``\newpage`` implicitly call ``\par``.
-  Added ``\nouppercase`` and ``\fancypagestyle`` commands in
   ``fancyhdr.plt``
-  Implemented ``\listoftables`` and ``\listoffigures``.
-  Command ``\tableofcontents`` finishes a paragraph and inserts the
   value of ``tocdepth`` counter.
-  Corrected error message in case of bad ``hbox``. Produces a
   ``<merror>`` element.

2011-01-24, Version 2.14.1
--------------------------

-  [bibtex] Added the serial number to the ``bibtex`` sort key .
-  [txmath] Corrected tracing of ``$\hspace{2mm}$``.
-  Improved printing of math tokens in case of fonts.
-  Corrected translation of ``\left`` in ``nomath-2`` mode.
-  Added ``\negmedspace`` ``\negthickspace``.
-  Corrected error message for ``\sin\int`` outside math.
-  Corrected ``\left\updownarrow`` in ``nomathml`` mode.
-  [``amsmath.plt``] Changed definition of ``xxalignat`` and co.
   Corrected the no-mathml translation.
-  No label generated for starred version of align env.
-  Implemented dual mathmode in case ``\@nomathl=-3``.
-  Argument of ``\bibliography`` may contain a trailing bib extension
   (Eric Bischoff).
-  [2011/04/11] Added ``\tableattribute`` env.
-  No error in ``missing_flush`` if buffer contains only of spaces.
-  No error in cases like ``$\mbox{\hspace {2mm}}$``.
-  Implemented ``\bordermatrix``.
-  Improved some error messages.

Previous versions
-----------------

There is an old changelog ``ChangeLog0.html`` that I couldn’t locate at
the moment; previous releases: 

- 2008-07-20, Version 2.13
- 2008-04-21, Version 2.12 
- 2007-11-29, Version 2.11 
- 2007-05-02, Version 2.10 
- 2006-10-26, Version 2.9
- 2006-08-01, Version 2.8 
