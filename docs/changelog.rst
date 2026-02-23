ChangeLog for the Tralics software
==================================

2026-02-23, Version 3.0.0
-------------------------

Many changes for this release, mostly none of them affecting the output (too
many to list here). Of note:
- Switch to CMake for better compatibility
- Use typographically correct quotation marks in the output
- Change the confdir search algorithm, enable kpathsea lookup for it
- Switch to spdlog for logging
- Only support UTF8 output

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

2008-07-20, Version 2.13.x
--------------------------

-  [2008-07-20, version 2.13.0]
   Fixed ``\end{...}`` if in env that finishes the math.
   Removed SansTitre etc.
   Made ``\ra@jobname`` work in any case.
   Removed printing of Option -year= ignored.
   Added a fifth small size.
   Added ``\@xsetfontsize`` and ``\@xgetfontsize``.
   [2008-09-16] Implemented amsart class.
   Added an ``id-text`` attribute to most anchors.
   Implemented ``\refstepcounter``.
   Implemented ``\notag`` and ``\incr@eqnum``.
   Changed ``\thechapter`` etc in standard classes.
   Replaced ``\stepcounter`` by ``\refstepcounter`` in ra.plt.
   Defined the command ``\AmS``, as well as the ten analogous of
   ``\Hat`` in the amsmath package.
   Created a soul package, moved 4 commands into it.
   Added commands of type ``\labelenumi``.
   Added commands of type ``\p@enumii``.
   Implemented ``\newtheorem*``.
   Hack the case where a theorem starts with a display formula.
   Implemented ``\hdotsfor`` as a line of dots.
   Change which equations are numbered.
   Allow ``\char\``` in text in math.
   Changed ``\stepcounter`` and ``\@killcounter``.
   Fixed a bug in ``\XMLlastid``.
   Added ``\everybibitem``.
   Added ``\XMLgetatt``.
   Re-implemented ``attlisttable``.
   Removed the ``AttListTable`` structure, uses vector instead.
-  [2008-10-03, Version 2.13.1]
   Removed all hacks concerning year field for the raweb.
   [2008-10-16] Added special case of
   ``\ChangeElementName{theorem}{xx}`` when second argument is empty or
   starts with space.
   Fixed a bug: the anchor is always the theorem element, if name is
   empty, then attribute list is copied to the ``<p>`` that follows.
   Changed figure filename handling: ``./`` no more removed.
   Implemented translation of optional argument of divisions and
   ``\caption``. Value put in a ``<alt_head>`` may be changed in config
   via alt_section or alt_caption.
   Fixed translation of ``\bindnasrepma`` from U+260B to U+214B.
   Fixed some math envs (e.g. ``align``), by providing a non-empty
   id-text value.
   Added option -no_float_hack.
   Obsoleted options nobibyearerror and nobibyearmodify.
   Removed parametrisation of raweb topics.
   Use other counters for ``itemize`` than for ``enumerate``.
   Moved the ``\glo`` command and ``glossaire`` env in in ra.plt.
   Fixed translation of ``\quad`` and others in nonmathml mode in
   ``\mbox``.
   Added some Win32 conditionals in ``readline.C``.
   Feature: optional argument in ``enumerate`` does not change the type
   to description.
   Added a label attribute to items in ``enumerate`` env.
   Added ``enumi@hook`` (Brian West).
   Moved ``\labelitemXXX`` from std.clt to the kernel.
   Added a default label, just in case.
   Removed evaluation of font commands in nomathml mode (Mukesh Kumar).
   Added command name of ``\hbox`` in the tree for nomathml mode (Mukesh
   Kumar).
   Removed interpretation of ``\hbox`` in nomathml mode.
   Removed the bibtex warning: You should use {\'e} rather than \\'{e}.
-  [2008-10-27, Version 2.13.3]
   Fixed a bug in ``wrapfigure``.
   Implemented package epigraph.
   Added missing line number in error message for missing close brace.
   Allow expansion of arguments of ``\begin``, ``\end``.
   Improved error handling of ``\def\xx#1{}\begin{\xx}``.
   Changed ``\hspace{2.cm}`` in math mode, result always a dimension in
   pt, works even after ``\mathcode\`.="8000``.
   Implemented package esdiff.
   Implemented full graphics package.
   Implemented optional arg of ``\rotatebox``.
   Implemented colors.
   Implemented commands like ``\textsevenoldstyle``.
   Fixed a bug in function ``read_from_file`` (core dump on gcc-3.3.3) .
   Added a ``verse`` environment
   Fixed ``\@latex@error`` and others.
   Moved ``moreinfo`` in a style file.
-  [2008-11-13, Version 2.13.4]
   Allow ``\fbox{\[ x\]}``.
   Allow ``\fbox{\begin{tabular}{c} x & y  \end{tabular}}``.
   At most four digits are read for the year.
-  [2008-11-17, Version 2.13.5]
   Added ``\XMLref``.
   Fixed underscores and hat: these characters are allowed if
   InLoadHandler. OK in ``\XMLaddatt``.
   Removed runaway argument error in use of ``\@reevaluate``.
   Fixed ``\bmod`` (Bouche).
-  [2009-10-05, Version 2.13.6]
   Added ``\XMLref``.
   Added kvoptions, float package.
   Changed ``\ClassError`` to put something in the XML.
   Bug correction detected by compiler. (``Hashtab`` ctor; cmd
   ``ifdefinable``; cmd ``ltfont``).
   Added parentheses as suggested by compiler.
-  **[2009-10-14]**
   Corrected printing of command names like ``&`` in error elements.
   Removed theme if year > 2008 for the RA.
   Thus, file testerr tested with year=2008.
   Added test files for RA2009.
   Added list of fields/entries not to use in the Biblio.
   Changed warning for Lille.
   [2010-03-31] Added a hack for primes in math mode.
   Some math-mode names can be changed.
   Corrected a bug in printing of very large characters.
-  [2010-10-25, Version 2.13.7]
   Input file encoding algorithm improved.
   Corrrected ``new ColSpec::ColSpec (...)`` (thanks to Daniel Simon).

2008-04-21, Version 2.12.x
--------------------------

-  [2008-04-21, version 2.12]
   [2008/04/26] Fixed the bug :literal:`\\mathcode`x="8000$\mbox{x}$`.
   In nomathml mode, ``\relax`` not printed any more.
   Fixed a bug in natbib.plt (Wagner) when ``\cite`` is redefined.
   Added end-of-paragraph before ``\end{thebibliography}``.
-  **[2008-05-18, version 2.12.1]**
   Fixme. Font changes in tables are incorrect...
   Implemented ``\natcite``.
   Changed ``\bibitem``, calls ``\ignorespaces``, produces a bibitem
   element.
   Bug fix: declaration xml_pack_font_att = "true" is honored.
   Changed ``\mathversion{bold}$\mathnormal x$`` to produce bold.
   Implemented tipa package.
   Bug fix ``$\hbox{{\ensuremath{x}y}}$`` is OK .
   Implemented bold math Greek letter (Wagner).
-  **[2008-05-29, version 2.12.2]**
   Newline is OK in ``\zap@space``.
   Changed ``\:`` to medmuskip.
   Use ``<unexpected>`` for junk in table (Wagner).
   Allow any kind of space in text in math (Wagner).
-  **[2008-06-06, version 2.12.3]**
   Implemented ``\formulaattribute`` in no-mathml mode (Vishwakarma).
   Added a ``textype`` attribute for all ``<texmath>`` objects.
   Special commands like ``\tralics@prompt`` added.
   Added raw_bib as command line option.
   Translation of ``\begin{table} \begin{tabular}`` is a tabular.
   (Wagner).
   Fixed default type for masterthesis.
   The ``\calc`` command is mode independent (Vishwakarma).
-  [2008-06-20, version 2.12.4]
   Moved declaration of ``uint`` from ``txvars.h`` to ``tralics.h``
   (Leopard).
   Removed a bibtex error if no author/editor.
   Expansion of ``\u z`` is ``\@unicode@composite {z^^^^0306}`` (T.
   Bouche).
   Commands like ``\bar`` do not look anymore for ``\relax`` (T.
   Bouche).
-  [2008-07-04, version 2.12.6]
   [2008-07-08] Default value of ``\pers`` is ``\persB``.
   Removed restrictions on participants, catperso, etc.
   Distribution comes in two files: ``tralics-src`` and
   ``tralics-extra``.
   Removed use of file ../xml/tralics.sty in test files.
   Removed function ``only_for_module`` and associated statistics.
   Made Tralics less verbose for image info.
   Renamed ``default_year`` to ``ra_year``, associated to ``\ra@year``
   Added two source files ``txra.C`` and ``txconfig.C``.
   Complete redesign of the RA stuff (Tex code in ra.plt ra2008.clt).
   Removed the value field in the ``ParamDataSlot``
   Removed side-effects in ``check_ns::check_section``.
   Biblio inserts ``\csname @href\endcsname`` in case ``\href`` is
   redefined.
   No ``\href`` added if the URL field has ``\rrrt``
   Commented out content of file ``txcheck.C``
   Removed all special hacks for the RA, todo_xml etc.
   Changed ``\XMLaddatt`` : argument fully translated. Special chars
   maybe handled differently.

2007-11-29, Version 2.11.x
--------------------------

-  **[2007-11-29, version 2.11]**
   Changed type of Char 160 to active, as nobreakspace.
   Re-edited ChangeLog.
   [2007-12-05] Removed ``cur_cs``.
   Single ``check_outer_validity`` function.
   Bug fix: invalid token better handled.
   Added the html package; moved some functions there.
   Allow optional star after ``\tableofcontents``.
   Allow underscore in environment names.
-  **[2007-12-06, version 2.11.1]**
   Implemented keyval package and a lot of others.
   Added ``\tralics@split`` and ``\define@key``.
   [fixme] Double sharp in ``\@ifnextchar{}{}``.
   Made utf8x an alias for utf8 in inputenc.
   Added command ``\pop@stack``.
   Added commands ``\@temptokena`` and friends.
   Changed order of assignments in ``\tracingall``.
   Virtual end-of-item added to minipage.
-  **[2007-12-19, version 2.11.2]**
   Split the file ``txparser.C`` in two parts.
   Added ``\@onelevel@sanitize``.
   **[2007-12-22]**
   Renamed ``\filbreak`` (removing one l).
   Added ``\loggingall`` as synonym to ``\tracingall``.
   Moved html tests from file torture to testhtml.
   Implemented ``\listfiles``.
   **[2007-12-26]** Option xml_font_pack_att added in configuration
   files.
   Added commands like ``\fontencoding``, ``\usefont`` ``\selectfont``.
   Added ``\ignorespaces`` after ``\put`` and ``\multiput``.
   New file ``txtrees.h`` added.
   Multiple index added.
   Split file ``tralics.C`` in two parts, ``txtoken2.C`` holds the
   hashtab Ctor.
   Added newline or spaces when printing ``\message``.
   Added ``\@addtoreset``.
   Made ``^^L`` same as ``\outer\par``.
   Fixed ``\noexpand^^L``.
   Added a function ``T_next_arg_local();``.
   Re-implemented allocation mechanism using LaTeX method.
   Made ``\@ifundefined`` expandable.
   Split ``txvars.h`` in two parts, adding ``txcmd.h``.
   Enhanced ``hash_table::is_defined``, sets cur token.
   Added ``get_mac_value``.
   **[2008-01-23, version 2.11.3]**
   Postprocessing of subfigure depends on ``\tralics@use@subfigure``
   (Wagner).
   Corrected silly bug in ``stack::isfloat``.
   Changed status of ``\linewidth`` and ``\columnwidth``.
   **[2008-01-31]** Allow underscore in Team names for the Raweb
   (Durollet).
   Changed the way how the tralic_rc file is found if confdir is given.
-  [2008-02-05, version 2.11.4]
   Command name added to XML result element ``<error>`` in case of
   error.
   [2008-02-07] Bug fix; Brace delimiters were wrong [Umesh
   Vishwakarma].
   Changed the test in ``txpath.h``, using APPLE instead of ppc.
   Added enumeration for special cases in ``parse_args``.
   Made ``foo.xml.tex`` an invalid input file name
   Made ``foo.xml`` a synonym to ``foo.tex`` as input name
   Added options input_file, output_file, log_file.
   Changed basic IO functions.
   Bug fix in ``str_toks11``.
   Space and newline are identical in ``\ifx``.
   Implemented the xkeyval package. Options of ``\usepackage`` may
   contain commands.
-  **[2008-02-18, version 2.11.5]** Reimplemented packages and classes.
   Re-implemented ``filecontents`` environment.
   Capitalised some class names; Xml replaced by Xmlp
   Changed return value of ``get_macro`` to a reference.
   Removed the name of Buffer.
   Renamed ``my_stack`` to ``the_stack``.
   In ``txtitlepage.C``, less global variables.
-  [2008-02-22, version 2.11.6]
   Replaced ``sym_codes`` by ``symcodes``.
   Implemented ``\PackageError`` and the like.
   Implemented ``\ChangeElementName``.
   Setting of ``makefo`` in ``ShellActions::assign`` considered obsolete
   and moved at end of function.
   Fixed the case where options not declared with ``\DeclareOptionX``
   have an equal sign.
   Fixed the case where ``\documentclass`` options have braces when
   scanning doctype.
   Commands like ``\@car`` are non-long.
   Added ``\CheckCommand``.
   Renamed ``parser::define`` as ``parser::mac_define``.
   Package checkend adds an Info item to the XML file.
   Bug fix, command ``\@cons`` changes its arg globally.
   Changed some macros that read argument lists like ``next_opt_arg``.
   Outer check added for ``\@addtoreset`` and other commands that call
   ``\csname`` (ifundefined).
   Removed paragraph hacks for keywords.
   Implemented ``\ifvoid``.
   Fixed a silly bug: arithmetic overflow limits were off by a factor 8;
   (wrong constants).
   You can say ``\hbox{$$x$$}``.
   Made ``\scantokens`` work in interactive mode.
   Moved ``\newblock`` into std.clt.
   Moved some commands to amsmath package.
   Consider carriage-return as unprintable, hence prints as ^^M.
   Changed ``is_big`` so that char U+FFFF is valid on input. (neither
   U+FFFE nor U+FFFF are in the XML output).
   Replaced LaTeX Warning by Tralics Warning.
   Added special marker for Warning, so that ``\@gobble`` works.
   Made ``\stop`` work in interactive mode.
   Fixed a bug in ``add_buffer_to_list``, TL was badly pushed/popped.
   Variable ``name_for_error`` locally reset in expand and translate.
   Error element contains ``name_for_error``, without final space.
   Endline char is ``\r`` in verbatim.
   Error recovery mechanism changed.
   Added a file ``txparser3.C``.
   Mode of ``thebibliography`` changed.
   Implemented ``@nomathml=-2`` mode.
-  [2008-04-04, version 2.11.7]
   Change chapter ids from uidXXX to cidXXX.
   Removed ``stop_on_error`` flag for parse_error.
   No message of the form ``image foo not found`` if foo.png exists.
   Fixed a bug in ``\@reevaluate\foo\bar\gee``.
   Created a virtual file named .tex.
   You can say ``\input{}``.
   Result of ``\read`` is always brace balanced.
   Fixed a bug where ``\thickmuskip==\muskip0``.
   Changed overflow signalling mechanism.
   Removed the global variable ``radix``.
   Moved some global variables into ``main_ns`` namespace.
   Removed variable ``Parser::silent``.
   Replaced all occurences of ``skip_group`` by ``mac_arg``.
-  [2008-04-15, version 2.11.8]
   Removed function ``no_ovf_add``.
   Other arithmetic checks added.
   Removed mu to glue and glue to mu functions.
   Inlined ``cv_mu_to_glue`` and ``cv_glue_to_mu``.
   Changed type of integer in ``SthInternal`` to ``ScaledInteger``.
   Made arith operations members of ``ScaledInteger``.
   Fixed a bug in scale, when division is exact.
   Added restrictions to environment ``catperso``.
   DTD can be given on the command line.
   No directory search for ult file.
   Optional plus in ``\IfFileExists`` and ``\InputIfFileExists``.
   Option param takes one or two arguments.
   No creation of ``foo_.bbl`` if useless.
   Ignore check option in RA2008.
   Removed the fatal error if more than one of check/ps/xml are given.
   Shell variables TRALICSDIR and RAWEBDIR no more looked at.

2007-05-02, Version 2.10.x
--------------------------

-  **[2007-05-02, version 2.10]**
   Added options leftquote and rightquote. Indicate how to translate
   character left and right quote in non-verbatim text mode.
   Feature: if option usequotes is selected, doubles quotes give << or
   >> in French language.
   Feature: Double left quote, and double right quote translated into ”
   and “ (U+201D, U+201C).
   [2007-05-04] Feature: First token of a math expression removed if it
   is ``\relax``.
   [2007-05-17] Feature: More characters allowed in math mode.
   Changed translation of ``\upsilon`` and ``\Upsilon`` is to
   ``&upsilon;`` and ``&Upsilon;``.
   Space added after commands in the output of ``\showthe``,
   ``\showtokens``.
   [2007-06-01] ``\protected`` commands no longer considered as
   ``\outer``.
   Corrected implementation of ``\marks``.
   Made ``\mathbbm`` an alias to ``\mathbm``.
   In math mode :literal:`\\char`\&` produces ``&amp;`` (and not ``&``).
-  **[2007-07-06, version 2.10.1]**
   Feature: *Tralics* counts the number of HDR.
   (Raweb) Feature: It is required that you say if there are HDRs in the
   Raweb.
   (Raweb) Syntax change allows ``\UR{Sophia, Rocquencourt}``.
   (Raweb) Added ``\ResearchCentreList`` as alias to ``\UR``.
   (Raweb) Semantic change: Optional argument \`Location' added between
   \`Lastname' and before \`Catpro' in ``\pers`` in ``catperso``.
   Mandatory for multi-located teams.
   (Raweb) New Location arg can have a default value.
   (Raweb) Feature: File apics2007.ult not loaded when compiling the ra.
   (Raweb) Feature: When *Tralics* generates a tex file for the RA
   (option -ps), it uses latin1 as encoding.
   (Raweb) Feature: Team and section titles printed using terminal
   encoding.
   Bug fix: Tralics should works again for RA 2005 (bibliography).
-  [2007-07-16, version 2.10.2]
   (Raweb) Feature: ``\TeamHasHdr`` is optional. (request by
   Vercoustre).
   (Raweb) Feature. Removed ``all``, ``none`` as optional argument for
   Location argument in ``\pers``.
-  [version 2.10.3]
   New implementation for ra2007. Details follow. Nothing has changed if
   producing postscript, or testing the thing.
   File hyperref.cfg no more constructed because data not remembered.
   (Raweb) Feature: You cannot say ``\UR{\RCSophia}``.
   (Raweb) Bug fix: Commands from config file not inserted when
   producing PS.
   Feature: tralics --help: Order of options changed.
   Redefinition of ``\not`` in amsmath.plt removed.
   Feature: Html attribute no more computed for modules in RA2007.
   Feature: No restriction on module names (only uniqueness required).
   Bug fix: ``buffer::slash_separated`` does ``advance()`` in case of
   success.
   Feature: In the config file, themes can be given using uppercase
   letters.
   Changed error signaling for the raweb in some cases.
-  [version 2.10.4, 2007-07-23]
   Bug fix: No error in case ``\global\relax\def\foo{}`` (following a
   remark by Charpentier).
   Added ``fullsection_vals`` as replacement for ``section_vals``.
   Change: section titles defined by config file are put in the XML.
   Changed DTD to raweb7.dtd.
   Allow multiline variables in a config file.
   Added automatical call to ``\rawebstartdocument``.
   Change: Command ``\url`` adds ``<allowbreak>``.
   [version 2.10.5, 2007-08-02]
   Feature: Variable ``hacknotitle`` set to false by ``run_simple_ra``.
   Feature: Implicit group added whenever ``T_arg1`` is called.
   Function ``T_arg1*`` inlined (title was not printed on the tty).
   Removed option hacknotitle from the command list.
-  [version 2.10.6, 2007-09-27]
   Changed exit status of readline to 1.
   Change: No ``<allowbreak>`` added at the end of URL.
   Re-implemented ``\ensuremath`` as a macro (Req. Connexions).
   Bug fix. Added some code to raweb-uni.sty that redefines color (code
   commented out in pdftex.def for version 1.40).
   Added a -V, -verbose-doc option.
   Change trace of ``\let\oe\OE``.
   Re-implemented ``\MakeUppercase``, ``\MakeLowercase``. Case change
   commands allowed in Math mode. (connexions)
   Bug fix in lc/uc pair for ``\NG``.
   Feature allows ``$\mbox{(\ref{x})}=\eqref{x}$``.
   Bug fix: Command ``\ensuremath``, wrong placement of ``\relax``.
-  [version 2.10.7, 2007-11-03]
   (txmain) Bug fix. Year flag better checked.
   (txmain) Feature: Main file argument of Tralics can contain slashes.
   Bug fix. End verbatim algorithm changed.
   Bug fix. Better trace in case of too many errors.
   Bug fix. Labels were lost in nomathl mode.
   New feature. textcase package implemented
   [2007-11-09](txbib) Silly bug in ``find_lower_case``. (Wagner)
   (txbib) Feature: something like ``@book(foo,...}`` works.
-  [version 2.10.8, 2007-11-12]
   Feature: On the fly encoding implemented.
   Feature: Changed ``\protect`` to ``\relax``.
   (txmath) Bugfix. ``M_cv1`` handles ``\le`` the same as ``=``.
   (txmath) Feature: implemented the fencing algorithm.
   Bug fix. Wagner's math examples produce symmetric operators.
   New Command ``\tracingmath`` controls verbosity of the LBR algorithm.
   Feature: Math trace holds ^^J instead of newline.
   Added ``\@curmathfont``.
-  **[version 2.10.9, 2007-11-21]**
   Removed general purpose tokens from math trace.
   Implemented special active math characters.
   Implemented Package braket.
   Error recovery enhanced in math mode.
   (txcheck) Bug fix, if multi-line data has to be parsed.
   Bug fix. ``flush_buffer`` added at start of ``\citation`` (MP
   Durollet).

2006-10-26, Version 2.9.x
-------------------------

-  **[2006-10-26, version 2.9]**
   Corrected implementation of ``\chardef`` in math; added ``\chardef``.
   Feature: Ignore ``\vskip2cm`` and ``\vspace{2cm}`` in math mode.
   Feature: No space after ``\ifdim``, in the transcript of +scandimen
   for \\ifdim >12.0pt.
   Bug fix: Correct unit in the transcript of ``\mkern3mu``.
   Added Bibtex extensions.
   Bug fix: Tilda added if empty verbatim line.
-  **[2006-11-14, version 2.9.1]**
   Bug fix: It is possible to insert underscore in file names defined by
   ``\InputIfFileExists`` (T. Bouche).
   Bug fix: confusion between masterthesis and mastersthesis in
   ``make_names`` (T. Bouche).
   Bug fix: Switch 'IUCLC' removed in function ``set_termio``, because
   it does not work on Mac and testing whether we are on a mac is
   non-trivial. (Pb signaled by David Pichardie).
-  **[2006-11-28, version 2.9.2]**
   Feature: OS type is printed on the log file, works correctly on
   macintel (N. Roussel).
-  **[2006-12-10, version 2.9.3]**
   (txbib) Bug fix: second argument of ``make_new_entry`` was ignored
   (Theo Papadopoulo).
   Feature: In the raweb case, apics_all2006.bib is read (if it exists)
   instead of apics2006.bib, apics_refer2006.bib, apics_foot2006.bib.
   Bug fix: Additional fields are also copied in crossref.
   Feature ``foot_prefix`` is now useless in biblio.
   [2007-01-04] (txclasses) Bug fix, babel options were empty.
   [2007-01-14] Bug fix: Spaces normalized in ``\index``.
   [2007-01-17] Bug fix: Underscore chars allowed in team name (Laurent
   Pierron).
   [2007-01-29] Added math commands ``\triangleq\Cap\Cup``.
   Changed ``\allowbreak``.
   Added ``\m@th``.
   [2007-02-11] Added Configuration mfenced_separator_val="foo"
   Added option -(no)math_variant.
   You can say: tralics -param foo bar.
   Feature: There are 15 math fonts, corresponding to the 13 MathML
   variants. (plus normal and upright). In nomathml mode, they are
   hacked.
   Change: the name in a ``math`` is a C++ string instead of char*; it
   holds the first argument of a ``\mathbox``.
   Added ``\mathci \mathcn \mathcsymbol``.
   Change: you can say ``\mathmi[foo][bar]{gee}``, translates to
   ``<mi foo='bar'>gee</foo>``.
   Feature: Parses :literal:`\\char`\$` the same as ``\chardef`` in math
   mode.
   Feature: A single ``<mn>`` element is created for a sequence of
   digits
   [2007-02-16] Feature: Space character allowed in an environment name.
   Feature: Font size commands allowed in ``$x\mbox{\small$y$}z$``.
   Bug fix: char U+0327 is not special in math mode, since it's not
   ASCII. Use ``\times``.
   Feature: Translation of ``\right.`` is empty delimiter unless
   -compatibility mode.
   Bug fix of ``\let\A B $\A$`` dumps no core any more.
   Math mode change: ``\vskip`` converted into ``\vspace``, although
   this is useless.
   Change: No space in translation of ``A \\[2mm] B``.
   Bug fix. In math mode ``\end{foo}`` inserts a closing brace only if
   the begin inserted an opening brace (i.e., not if the begin was
   created outside math mode). Bug signaled by Thierry Bouche.
-  [2007-02-18, version 2.9.4]
   Improved the test when opening a file.
   Option bad_minus added
   Bug fix: In the case ``$1^23$`` the superscript is 2 (T. Bouche).
   Bug fix in ``math_p::find_paren2``; a group is big if it contains a
   big element. Gives better placement of ``<mrow>``.
   Added ``\mathfontproperty``.
   Added ``\colon`` as ``<mo lspace='0'>:</mo>``.
   Changed ``\def\root#1\of{\@root}``.
   Corrected translation of ``\widehat`` (invalid character code).
   Added ``\operatornamewithlimits``.
   Added ``\qopname``.
   Added: you can say ``$\left/ !\right\backslash$``.
   Changed implementation of ``\mathop...\limits``.
   Bug fix: Translation of ``$\big(xy$`` was empty.
   Bug fix: Translation of ``$\big(x\big\}$`` has mfenced, because
   ``\{\}`` converted to ``\lbrace\rbrace``.
   Bug fix: less-than and greater-than behave like ``\langle, \rangle``.
   Feature: Exchanged execution order of cv3 and cv4; this gives big
   delimiters in ``$\bigl(\sum \bigr )^2$``.
   Bug fix: Math style computation corrected: Exercise 17.15 gives
   correct answer.
   Bug fix: Boolean ``\ifinner`` gives correct answer in math mode.
   Bug fix: Command ``\pmod`` produces inserts 18mu or 8mu of space
   depending on the mode (display or not).
   Bug fix: Translation of ``\iff`` the same as ``\Leftrightarrow``.
   Changed translation of ``$\rm x\bf 2$``.
   Added ``\@Vert``, that is the same character as ``\parallel``, using
   this as delimiter. All delimiters work now.
   Change: ``\overbrace\underbrace`` produce a large operator that can
   have limits.
   Fixed translation of ``$\mathmo{<=}$``.
   Added ``\phantom``.
   Change: Automatic position of mrows: not in case open big close, but
   in case Big open close.
   Added:
   ``\ImaginaryI,\ExponentialE,\DifferentialD, \InvisibleTimes,     \ApplyFunction,\InvisibleComma``.
   Added ``\cellattribute\rowattribute``.
   Bug fix: Multline environment: all equations are in display style,
   first and last are correctly handled.
   Added ``\tag`` and variants.
   Added environment ``gather``; commands ``\xleftarrow \xrightarrow``
   optional argument added.
   Added
   ``\lvert \lVert \rvert \Rvert \cfrac \accentset     \undertilde \overleftrightarrow \underleftrightarrow   \arrowvert \bracevert \Arrowvert``;
   and a lot of other commands also.
   Bug fix: Xmatrix was wrong for X=b, B or V!.
   Implemented commutative diagrams.
   Added ``\scanupdown``; needed for ``\sideset``.
   Feature: phi and varphi swapped.
   Bug fix: Width of normal space in math mode changed from 6pt to 4pt.
   Changed definition of prime character.
   Changed definition of ``\asymp`` , is now asympeq, U+224D.
   Allow font changes in text in math.
   Allow commands like ``\hfill`` for horizontal placement of fractions
   or arrays.
   Phantom and smash hard-coded.
   Sideset command hard-coded.
-  [2007/04/28] ``\relax`` commands retained in the math tree.
   Mathfonts are groups now.
   Added option nozerowidthelt.
   Commands ``\begingroup \endgroup`` allowed in math mode and used for
   font switch.

2006-08-01, Version 2.8.x
-------------------------

-  **[2006-08-01, version 2.8]**
   Change in readline: ``killbuf`` is a string, ``history`` a vector of
   string
   Static functions are now in a namespace.
-  [2006-08-06, version 2.8.1]
   Moved interpretation of config data in ``tralics.C``.
   Added a file ``txaccent.C`` that handles all accents.
   Change: expansion of ``\a'`` is ``\'``.
   Change: use ``\tracingmacros``, for showing expansion of accents.
   Change double accent implementation. You can say ``\a'{\a^e}``; error
   messages could be Error in accent, command = \\'\^, Cannot put this
   accent on letter i.
   Change: If interactive, read config only if given as parameter.
   Changed again the algo for finding config file. Default value can be
   set in the Makefile. Otherwise in a new file ``txpath.h``.
   Changed the file mktar. Now the tar contains a directory.
-  **[2006-08-08, version 2.8.2]**
   Change. You can say ``$\S$``, not ``$\textsection$`` (both are valid
   in future versions).
   Changed expansion of ``\o\ae\aa\th\dh``, and uppercase version, to
   characters. They are allowed in math mode.
   Added ``\caps \hl \st \so \ul``. Use same method as
   ``\textsuperscript``.
   Commands like ``\ier`` do not produce an entity anymore. Rather than,
   they are like ``\textsuperscript{er}\xspace``.
   Added commands ``\list, \endlist, \usecounter``.
-  **[2006-08-09, version 2.8.3]**
   Added ``\@item``, like ``\item``, but replaces label by an attribute.
   Changed at-sign char to be letter while reading config file.
   Config file allows ``\let\item\@item``.
   Bug fix for ``\xmlelt{}{-}``.
   Added concept of xx.clt, xx.plt, xx.ult.
   Added an optional star after
   ``\input, \include, \InputIfFileExists``.
   Changed ``\usepackage{foo,bar}`` to be the same as
   ``\usepackage{foo}`` and ``\usepackage{bar}``.
   Bug fix: ``correct_type`` is now implicit for macros.
   Bug fix: Inserted missing ``flush_buffer`` in ``cst2_cmd``, like
   ``\ieme``.
-  **[2006-08-15, version 2.8.4]**
   Change: Transcript file says date= 2006/01/02 03:04:05.
   Implemented ``filecontents`` environment.
   Bug fix ``\InputIfFileExists`` has three args.
   Feature: file foo.ult loaded only if document class exists.
   Added new file ``txclass.C``.
   Bug fix ``\newenvironment{foo}[2]{}{#1}`` now invalid.
   Feature: All calls to ``file_exists`` are traced.
   Name change: ``Buffer, att_list``: renamed ``add`` to ``push_back``;
   ``buf  << bf_reset`` is the same as ``buf.reset()``.
   Feature: Star after ``\chapter`` etc is handled. Default star in
   book, outside main matter
   Added ``\toplevelsection``.
   You can say: tralics apics2006 -config=../confdir/ra.tcf (bug
   signaled by M.P. Durollet)
   Added count register ``\notrivialmath``.
   implemented index mechanism.
   Bug fix: Array preamble accepts rlc of category other.
   Bug fix: Verbatim numbering OK now.
   Bug fix: Array preamble accepts a char declared ShortVerb.
   Option -nostraightquotes added.
   Bug fix: Implicit ``\noindent`` after ``\item[]``.
   Feature: Attlist number 3, 4, 5 reserved for index, TOC, biblio.
   Added ``\thanks`` as an alias to ``\footnote``.
-  **[2006-09-04, version 2.8.5]**
   Change: command ``\textasciicircum`` produces char U+2303.
   Change: Option -notrivial-math is the default, option trivial-math
   added.
   Bug fix: Tables in a tabular OK now. There will be an attribute
   ``rend``, with value display or tabular, instead of none.
   Added ``\anchor``
   Bug fix: Command ``\@arabic`` ok.
   Feature: Section titles in the transcript file.
   Bug fix: Leaders are OK now.
-  **[2006-09-09, version 2.8.6]**
   Feature: Three values possible for the second argument of
   ``read_a_file``.
   Feature: Main file not immediately converted, but later.
   Change: All files are converted into UFT8 internal.
   Change: The number 256 appears in a single include file.
   Change: The number 257 replaced by ``special_relax``.
   Change: The number 255 appears no more in a C file.
   Changed retval of ``cmd_chr::char_val`` to integer from character.
   Use ``relax_code`` instead of ``subtypes(256)``.
   Changed ``Str_hash`` realloc step size to 10000 instead of 1000.
   Replaced constant 100 by ``xml_offset`` in ``txmath.C``.
   Change: ``^^^^abcd`` does not put the character back in the buffer,
   but in ``unread_char``, saved by ``push_input_stack``.
   Hash table size increased plus info in case of abort.
   Feature: argument of ``\newif`` must start with if.
   Replaced calls to ``hash_find`` by ``locate``.
   Changed retval of ``get_a_new_line`` to a vector of characters,
   instead of a buffer. This is saved by ``push_input_stack``.
   Feature: Value of ``\endlinechar`` can be zero.
   Feature: Compilation date now in XML file.
   Feature: Meaning of ``log_or_tty`` changed.
   Hack: Version number changed to 2.9 for the examples.
   Change: Removed leadingpart, leadingsection from documentclass
   options. Use ``\toplevelsection`` command instead.
   Removed ``\cite@type`` ; is now in natbib.plt; ``\cite@@type`` added
   as hook.
   Bibtex scanner: ``author="\{\"x"`` is interpreted as a Latex string
   with three tokens. Result is always balanced.
   bibliography: ``all_entries`` is a vector of pointers. Makes life
   easier.
   Removed interactive_bib option.
   Changed four hat parsing again. Input buffer is converted to Unicode
   characters in a vector.
   More calls to ``scan_left_brace_and_bi``, which has been redefined.
   Feature: Option confdir=foo is OK as 3 shell tokens.
   Change: Use of == in tpa assignments
   Feature: img file not created if no images exists.
   Bug fix: second ``\ignorespaces`` added in ``\item``.
   Bug fix: ``\index``, @ can be letter or other.
   Feature: all symbols of latexsym are now implemented.
   Added ``\wlog``.
   Bug fix: Relax scanning: ``\frac{foo}\x`` is ok, if ``\x`` is a user
   defined command.
   Added ``\pageref``.
   Change: ``candimen`` replaces 18mu by 10pt.
   Removed ``\zerospace``.
   Changed ``\enspace``.
   Buf fix: Print # not ## in case of error in ``insert_token0``.
-  [2006-09-09, version unchanged]
   Implemented some e-TeX extensions; Change tracing for
   ``\tracingassigns``.
   Reimplemented ``conditions`` as vector.
   Changed current font to ``tenrm``.

2006-04-01, Version 2.7
-----------------------

-  [**2006-04-02, version 2.7**]
   Bibtex modifications. There is a field that contains the current year
   as an integer. 0 means missing, -1 means unparsable. If 2 chars
   given, then 06 is replaced by 2006, 96 by 1996.
   An error is signaled in a refer; if the year is 0, -1, cur-year,
   greater than cur-year; the entry is moved into the year section.
   Some braces are removed when sorting.
   Change: Finding profile on mac changed.
-  [**2006-06-12, version 2.7a**]
   Fixed ``\unskip``.
   Added ``\mathfrak``.
   Added ``\@addml``.
   Added ``\operatorname \DeclareMathOperator``.
-  [**2006-06-14, version 2.7b**]
   Changed ``\keywords`` to undefined by default. It is too dangerous to
   use the environment as a command.
   Added ``\choose \shoveleft \shoveright``.
   Added environments ``multline aligned``.
   Added ``\@firstoftwo`` and ``\@secondoftwo`` (in LaTeX syntax),
   ``\@ifbempty`` ``\@iftempty`` (in C++).
-  [**2006-06-14, version 2.7p4**]
   Trying to print characters in ``no_mode`` is no more a fatal error;
   the mode is changed to ``argument_mode``.
   Bug fix: if foo is empty, ``\input{foo}``, *Tralics* crashes.
   Bug in push/pop input stack fixed; ``cur_input_stack`` is now a
   vector. Double pop avoided by checking the length. Name of
   file/virtual file added.
   If you say -noentnames, there are no more entity names, math mode
   also. Otherwise, generate ``&straightepsilon;`` and ``&varepsilon;``
   instead of ``&#x3F5;`` and ``&#x3B5;``.
   Easy part of bootstrap code is now written in C++.
   Reevaluate uses ``lines.split_string``, comments are ok now.
-  [**2006-07-05, version 2.7p6, 2006-07-14, last patch for v2.7**]
   Added lots of functions in file ``txcommands.C``, so that each case
   in the big switch has less the 5 lines.
   Changed Raweb structure: removed some tests in ``txcheck.C``.
   Added ``\@setmode``.
   Changed production of keywords in the hyperref.cfg file.
   Changed the semantics of the ``\pers`` command.
   Changed biblio for the raweb.
   (raweb) Config file: ra is no more the first type. Clean up.
   (raweb) New argument for *Tralics* distinguish_refer_in_rabib
   Concept of tcf file added; all config files converted to tcf
   Changed directory structure of *Tralics*.
   UTF8 output is possible now.
   Removed all 8bit characters in configurations files.
   Removed all 8bit characters in the C++ sources.
   Added ra.tcf, tralics_rc has now a single type.
   Cleanup in ``txcommands.C``. A big table is used instead of a
   function for ``cst1_cmd``.
   ``\guillemotlet`` and ``\guillemotright`` produce a single character
   in this table.
-  [**2006-07-23**]
   Following files added:
   ``txio.C   tximage.h, txscaled.h, txid.h, txeqtb.h, txfp.h, txio.h, txstats.h,    txpost.h, txcond.h, txatt.h, txhash.h, txxml.h, txtokenlist.h, txarray.h``
   ``tralics.h`` contains no class definitions any more.
   Removed one global variable in ``txarray``.
   ``att_list`` is essentially a vector.
   Simplified logic of ``T_par1``.
   Converted ``line_ptr`` in a list.
   Renamed Language as att_language in config file.
   [raweb] Removed default for theme_vals, affiliation_vals,
   profession_vals, section_vals, ur_vals. Fatal error if missing.
   (raweb) Function ``sans_titre`` hacked.
   Option -hacknotitle added.
   rahandler: uses tralicsdir instead of rawebdir.
   Everything that was in the the old rawebdir now copied in the Tralics
   dir.
   Option distinguish_refer_in_rabib is true by default.
   (raweb) Implemented ``\refercite`` for the two latex classes.
   Removed ``temp_list`` data structure.
   Re-implemented ``token_list`` as a normal list.
   Re-implemented math lists also.
   Added classes ``math_f`` and ``math_q``; removed three calls to
   'abort'.
   Added two files ``txmath1.C`` and ``txmath1.h``.
   Use ``#xab;`` notation for math chars.
   Added function ``buffer::add_int16ent``.
   Added function ``buffer::real_utf8``. The only function that produces
   chars.
   Bug fix: Translation of ``$\hbox{é<}$`` is now OK.
   Bug fix: in nomathml mode, ``$\hbox{$x$}$`` is ok.
   Added ``end_all_input_code`` command code. Now ``\end{document}`` is
   the real end.
   Fixed tralics foo -confdir=../confdir.
   No delay for the ``\addattributestodocument`` for DocAttrib.

2005-08-10, Version 2.6
-----------------------

-  [**2005-08-09, version 2.6**]
-  [**2005-09-05, version 2.6pl1**]
   Wrong topics.
   Bug fix: Transcript file truncated in case ``\mathchardef\Xc=3``
   (signaled by Sebastian Öblom).
-  [**version 2.6pl2**]
   Bug fix: ``\cline`` did not work correctly, because a carriage return
   was added between cells, and it was expected that only cells appear
   in a row.
-  [**version 2.6pl3**]
   Bug fix: math_type of ``\models`` was wrong (misplaced close paren).
-  [**version 2.6pl4**]
   Bug fix: Path of config file wrongly computed. (signaled by J.P.
   Talpin).
-  [**version 2.6pl5**]
   Added a test that the file name is lower case only in raweb mode.
   [2006-01-13] Bug fix: CSS says that 3.cm is not a valid dimension.
   Thus the figure env produces 3.0cm
   [2006-01-25] New: ``\#`` allowed in Math mode.
-  [**2006-01-30, version 2.6pl7** ]
   [txbib] Empty crossref ignored. Better debug otherwise.
   Empty glossaire is an error.

2005-07-11, Version 2.5
-----------------------

-  [**2005-07-11, version 2.5 (pl1)**]
   Bug fix: ``\bibitem[foo]{bar}``.
   Change: Bibtex is booted twice now.
   Change: ``\begin{thebibliography}`` enters vmode.
   Change: Empty part and junior value not printed for ``\bpers``.
   Change: The ``\cititem`` command leaves the mode unchanged.
   Change: ``\bpers`` gives only 1 error: should occur in bibliographic
   mode only.
   Change: *Tralics* is less verbose in silent mode.
   Bug fix: ``"\par`` works now in German.
   Configuration file identification dump changed.
   Change: ``\FPseed`` is an integer reference.
-  [**2005/07/25**]
   Math environment always traced.
   Fixed ``\mathop{\rm sin}``.
   Fixed ``$\ttfamily xy$``.
   Config file in source.
   Changed algorithm for finding configuration file.
   Fixed ``\indent\noindent``, is a no-op.
   Changed value of ``\quad`` outside math and of ``~`` inside math.
   Added some commands.
-  [**version 2.5pl7**]
   Changed paragraph hacking.
   (raweb) Configuration changed: composition\* says: no topics in
   compo.
   An error is signaled in case of scanint overflow (!!).
   Added ``\nocentering``.
   Added escape_char in cases like : ``\let\foo\bar \show\foo``.
   Countdef and Co are traced.

2004-11-30, Version 2.4
-----------------------

-  [**2004-11-30, version 2.4**]
   Bug fix: Underscore characters OK in ``\psfig{file=...}``.
   Outer tokens are forbidden in a lot of situations.
   Added ``\verbprefix`` and ``\verbatimprefix``
   Changed translation of ``$\epsilon \varepsilon$`` to
   ``&#x3F5; &#x3B5;`` (bug signaled by Fabrice Rastello).
-  [**version 2.4pl1**]
   No space between parenthesis and ``\footcite``.
   Calls to ``\newcount`` always logged.
-  [**2004-12-06, version 2.4 (pl2)**]
   Bug fix. ``\the\catcode\the\count@``.
   Macros are printed as \\Bar=macro: #1#->#1.
   [2004-12-21] Bug fix: Active ampersand produces ampersand (Gregoire
   Malandain).
   Changed printing of latex macros with optional argument as
   \\fooiv=opt \\long macro: bar#2#3->Seen#1#2#3.
-  [**2005-01-22, version 2.4 (pl4)**]
   Bug fix: ``\leavevmode`` added before ``\cite`` (M.P. Durollet).
   raweb-cfg.sty: Handling of array modified.
   Bug fix in ``\multicolumn``.
   Three ``\aftergroup`` commands added for implementing thinlines and
   thicklines.
   Fixed a bug; confusion between ``np_line_thickness`` and
   ``np_linethickness``.
   Added ``\XMLaddatt``.
   Bug Fix. In math mode, ``\hspace{...}`` did not work with newline as
   space because ``the_toks()`` uses category code 12. Newline now
   replaced by space. [D. Talay]
   Biblio strings read via ``fetch_name``...
   New biblio implemented.
-  [**2005-02-10, version 2.4 (pl5)**]
   xmltex: ``<mrow>`` implemented as a group.
   Translation ``${x_y}_z$``, the result contains a ``<mrow>``.

2004, fall, Version 2.3
-----------------------

-  [**version 2.3**]
   [raweb] Removed abstract and body environment.
-  [**2004-10-30 version 2.3 pl2**]
   Line number indicated in case of bad end verbatim.
   Space allowed before ``\end{verbatim}``.
   Added ``\mathattribute``.
-  [**version 2.3pl3**]
   [raweb] You can say ``\bibliography{miaou2003.refer+refer}``.
   Removed the message unexpandable command in expand? \\cl@section.
-  [**version 2.3pl5**]
   Bug fix. After ``\def\cs AB#1{}``, the log file contains a space
   after the CS name.
   Bug fix for ``\long\outer\def``.
   Change: You can use environments with one letter.
   Bug fix: ``scan_opt_arg`` removed braces twice.
   Bug fix: ``\par`` tokens are now forbidden in non-``\long`` commands
   (they are a allowed in system defined commands).
   Implementation of ``\long\outer`` changed.
-  [**version 2.3pl6**]
   Fixed a bug with ``last_att_list`` and ``realloc`` (bug signaled by
   Emmanuel Thomé).
   Fixed a bug for option -find_words.
   Signal error in case ``\def\foo{#0}``.

2004, summer, version 2.2
-------------------------

-  [**version 2.2**]
   Trace changed. You will see {end-group character }}.
-  [**version 2.2(pl1)**]
   In case of errors, whole usage not always shown.
   [raweb] Changed handling of ``\maketitle`` and ``\loadbiblio``.
   Option -debug does the right thing.
   Added utf8 mode for input.
   Bug fix in option -noundefmac.
   Corrected bug in ``T_twoints``.
-  [**version 2.2(pl3)**]
   Moved ``mu_glue`` in the glue table.
   Improved tracing for registers.
   Improved tracing for ``\divide``.
-  [**2004-09-08, version 2.2pl4**]
   Bug in ``\footnote{\url{~}}`` corrected.
   Changed ``\href``, added ``\Href``.
   ``\mathmn\mathmo \mathmi`` added.
-  [**version 2.2pl6**]
   Instead of rahandler.pl, you can use any other software.
   Avoids creating a transcript file for a non-existing input.
   Made ``math::convert_this_to_string()`` public.
-  [**version 2.2pl7**]
   Replaced the Copyright notice by CeCILL.

2004-05-14, version 2.1
-----------------------

-  [**version 2.1a, 2004-05-14**]
   Bug fix ``{\def\relax{0}\edef\foo{\ifnum0=0\fi}}``.
   Details added.
   Change: ``undef_cmd`` is now ``max_command +1``, and
   ``\if\undef\undef\fi`` complains.
   Bug fixed in ``\romannumeral``.
   Bug fixed in ``\ifmmode``.
   Typo in mstyle.
-  [**2004-05-28**]
   [raweb] Removed check of ``\moduleref`` in ``txcheck.C``.
   [raweb] Removed check of ``\input`` in ``txcheck.C``.
   [raweb] Commands ``\loadbiblio`` and ``\maketitle`` are made
   ``\relax`` by txcheck, and are no more removed.
   The name of ``\caption`` in a figure can be parameterized by
   xml_scaption_name.
-  [**2004-05-01, version 2.1c**]
   There is a perl script, called rahandler.pl that calls latex, and
   other external processes.
   Bug fix: ``math::mk_space`` generates a proper object.
   Change: ``mc_table`` is a list of XML objects.
   All math elements are proper math objects.
   And no buffer needed for math boot.
   File ``tralics_rc`` is now under CVS, in the tralics dir.
   Underscore allowed in bibliography name.
   There is a ``\ignorespaces`` after ``\begin{document}``.
   Made example 8-2-28 work.
   [raweb] Removed the ``\typeprojet`` command.
   [raweb] Removed the ``\debugfalse`` command.
   Implemented ``\Box\dddot\ddot\mathcircle``.
   Different names in config file for ``figure`` and ``graphic``.
   Corrected ``\dfrac``.
   Implemented ``\bmod \mod \pmod \pod``.
   Corrected ``\textbullet``.
   New feature. Fonts changes generate attributes, or elements.
   ``\sc`` and ``\fsc`` use the same font.
-  [**2004-07-08**]
   Bug fix. ``\newcommand\bar{}`` defines the command, even after error.
   Bug fix. Adding two positive dimensions gives always a positive
   result (even in case of overflow).
   Added ``\date``.
   Added ``\dag\ddag``.
   Replaced entity ``&ZeroWidthSpace;`` by ``&#x200B;``.
   Removed function ``my_abort()``.
-  [**2004-07-14**]
   Fixed ``\FPeval``.
   Added ``\parbox``.
   Removed useless stuff from ``txmain.C``.

2004-02-27, Version 2.0
------------------------

-  **[2004-02-27]**
   Implemented mathstyle.
   Bug fix. ``\genfrac`` chooses the right style.
   Added ``\penalty``.
   Bug fix: All tokens are printed OK now.
   Added a -silent option.
   Split ``txmath.C`` into two parts.
   **[2004-03-01]** Renamed ``Main`` and ``Project`` classes.
   UR lists can be configured.
   Removed ``array_info::~array_info()``, because of bugs.
   Bug fix: ``\string~`` is OK.
   Added ``\mkern \mskip``.
-  **[2004-03-09]**
   [Raweb] Sections are no more hard coded.
   Added ``\lastbox``.
   Added file ``txstring.h``; data structure ``istring`` added.
   [Raweb] Removed hacks for the presentation section.
   Removed ``math_dimen_attrib``.
   Corrected hash code of null string.
   Added a class ``user_param``.
   Changed buffer size to 256.
-  [**2004-03-08**]
   The default is not to construct the word list.
   Reordered name-positions in alphabetic order.
   Bug fix in ``token::tex_is_digit``.
   Silly bug in ``\count@=222\ifodd\count@ 1 \else 2\fi``.
   Added ``\hrule\vrule``.
   Change: ``\begin{document}`` is more restricted.
   Fixed ``\MakeUppercase{\i\j}``.
   Moved ``handling_ra`` in ``user_param``.
   Added a ``cstu_cmd`` code, in order to reduce number of ``cst1_cmd``.
   Added some Unicode characters between U+0180 and U+0233, those
   between U+1E00 and U+1EFF.
-  [**2004-03-16**]
   Bug fix in ``eqnarray`` column specs.
   Change. ``\char256`` is now printed as &#x100;.
   Added ``\lgroup\rgroup\lmoustache\rmoustache``.
   Fix. ``\setcounter{foo}{-1}\alph{foo}`` says something more sensible
   than Counter too large.
   Bug fix. ``parser::T_twoints`` calls scanint, not scan_dimen.
   Change. ``\xmlemptyelt`` does not start a paragraph.
   Removed things like ``alph_code+10``.
   Change in ``latex_ctr``: fully expandable (includes ``\arabic``,
   ``\@alph``, ...).
   Implemented ``\frame \dashbox``.
   Bug fix. ``\unnumberedverbatim`` has no effect on ``Verbatim``.
   Added ``\unhbox`` etc.
-  [**2004-03-16**]
   Bug correction in ``scan_int_digs`` : In the case of ``\box{123}``
   only one error is signaled.
   Added ``\showlists``.
   Bug fix: ``\showthe`` and ``\showbox`` print to transcript and tty.
   Added a name to every buffer (for debug). Reduced the default size of
   a buffer to 128.
   Added ``\OR\AND\NOT\isundefined`` in if-then-else.
   Removed one of the splitter buffers.
   Added ``\newtoks``.
-  [**2004-04-05**]
   Removed ``in_biblio_key``.
   Removed ``noparindent``.
   Added some ``\every...`` commands.
   Bug fix: ``\everydisplay\everymath`` dumped core.
   Removed ``finish_string``, buffer is always 0-terminated.
-  [**2004-04-05**]
   Modified the ``xml`` class. Is a pointer, with a vector.
   Change. ``parser::get_x_token`` returns a bool.
   Added function ``parser::T_newline``.
   Always check before adding an attribute to a list.
   Removed Parameter #1 of ``scan_something_internal`` .
   Bug fix ``\dimen0=\wd1`` is ok.
   Added option ``q`` in titlepage.
   Bug fix. ``\newtheorem*{problem}{Problem}`` is OK now.
   Titlepage hook added.
   Bug fix ``\let\foo#\def\A\foo1\foo2{\foo1}``.
   You may see: \\renewenvironment: cannot define \\xxx.
   You cannot say ``\newcounter{AA}\newcounter{AA}``.

2003-09, Version 1.9
--------------------

-  [**Version 1.9b** date unknown]
   New includes ``txparser.h txtoken.h txfonts.h txlogger.h``.
   ``#include<stdio.h>`` is now in ``tralics.h``.
   ``lex_type`` in now unused.
   ``old_catcodes`` moved into ``Parser``.
   ``verbatim_chars`` moved into ``Parser``.
   ``token_to_list`` is now useless.
   ``Token::val`` is private.
   ``Token::Token(int)`` is now explicit.
   Added a new option that avoids year errors in the bibliography.
   Add a new class ``log_or_tty``.
   If the option -verbose is not used, then (not done) is not printed.
   ``\show\foo`` gives: \\foo=macro: #1\bar ->x{} (à la TeX without
   \\n).
   ``\show`` upgraded.
   ``\show`` prints also in the log file.
   Correction of: There was no image image.
   No ``<biblio/>`` if biblio is empty.
   The name of the ``.bib`` file is in the ``_bbl``.
   Better error message in the case of ``\a{}`` and ``\a{toto}``.
   ``\meaning`` works as in TeX.
   Correction of a bug in ``\SaveVerb \UseVerb``.
-  [**Version 1.9c**, date unknown]
   ``\font\foo=cmr10 at 30pt \show\foo`` works.
   Don't start in RA mode if standard or no ``\documentclass``.
   Corrected a bug in ``see_tex_ext`` (forgotten ``finish_string``).
   String stats: the sum of ``String+string`` is printed.
   *Tralics* stops after 1000 errors.
   Removed ``Main::initialised``.
   Replacement of ``bauteurs`` and ``bediteur`` by ``bauthors`` and
   ``beditors``.
   If I do ``\bibitem``, I do not add it to the rest.
   Some functions in ``txbib.h`` are private.
   ``version_string`` moved to file ``txmain.C``.
-  [**Version 1.9d, 2003-09-20**]
   Added option -configfile.
   [fp_num::div]: n can be negative.
   Removed some commas and semi-colons so as to remove warnings on sun
   like
   "txbuffer.C", line 1265: Error: Cannot use const char\* to initialize
   char*.
   "txbuffer.C", line 524: Warning: k hides the same name in an outer
   scope.
   ``Buffer::convert`` replaced by ``Buffer::convert_to_str() const``.
   Removed implicit conversion from ``Buffer`` to ``String`` then
   ``string``.
   ``Buffer::the_characters()`` renamed to ``Buffer::c_str()``.
   Removed all calls to ``sprintf``.
   ``Buffer::see_something`` argument is a ``const String[]``, without
   backslash.
   Missing comma added in ``bad_commands1``.
   bug correction: missing ``finish_string`` in ``Buffer::see_year``.
   bug correction: missing ``finish_string`` in
   ``bibentry::numeric_label``.
-  [**Version 1.9e, 2003-10-06**]
   Removed ``Buffer::insert_braced()`` (same as ``add_braced``).
   Message \\begin catperso (ligne 27) ... \\end xcatperso replaced by
   \\begin{catperso} (line 27) ... \\end{xcatperso}.
   ``read_a_file`` and ``read_a_file_spec``: same function.
   ``module::sec_string`` is useless.
   Removed:
   ``rc_space, convert_dim_to_space, a_dim_to_pt, convert_dim``.
   Removed tests for ``\def, \let, \newif`` etc.
   Instead of: They were 1 Sanstitre handled there is now: There was 1
   NoTitle handled.
   [**2003-10-10**]
   [bibtexerr]: seeing a } changes the error type.
   Added a field ``file_name`` in ``line_ptr``.
   Added a line number in the error messages of bibtex.
   Dynamic memory in ``txmath.C``.
   ``Parser::math_argument`` replaced by ``before_mac_arg()``; watch for
   differences as in ``$a\hbox{}b$``.
   [**2003-10-15**]
   Bibtex error messages start with Error and end with a period.
   ``\c{C}`` is ok.
   In the case of error: too many commas and empty name, the string is
   printed.
   Errors detected while reading a bibtex file are better handled.
   The log starts with: Trace of tralics XX for file YY (XX= version
   number).
   [array_info]: fields are private.
   The ``attributes`` table is dynamic. The size was 10000 (5993 for
   Sosso).
   Correction of bug (idem).
   Supression of XML stats if tralics -ps.
   Bug in ``do_indexing``.
   tralics -check executes ``do_indexing``, prints nothing.
   tralics -check does not create files \_modules, \_delcbis etc.
   Added some const in the files.
-  [**version 1.9i, 2003-10-16**]
   Added constant ``level_zero`` and ``level_one``.
   Anonymous ``enum`` changed to ``name_positions``.
   Functions ``push_level`` and ``pop_level`` take another argument.
   Strange errors for
   ``$$\begin{array}{cc}\mbox{\begin{tabular}{r|cc}...``.
   Renamed ``Parser::thebuf`` to ``Parser::unprocessed_xml``.
   For ``Stack::push1``, second arg is a ``name_positions``, first arg
   is optional.
   For ``Stack::newid0``, arg is a ``name_position``.
   For ``Parser::T_arg1``, ``Parser::T_arg1_opt``: second arg is a
   ``name_positions``.
   Added ``Parser::nT_next_arg()``, ``Parser::nT_next_optarg()``.
   In ``txpost.C``: removed code dealing with ``mylabel``.
   Renamed ``st_pos_foo`` into ``sp_foo``.
   Removed ``Stack:add_att_to_last(string,string)``.
   Renamed ``Parser::buffer`` to ``Parser::input_buffer``.
   Re-implemented ``str_hash::find`` .
   [**2003-10-22**]
   Added a new file ``txfonts.C``.
   Function ``Parser::get_token`` returns a boolean: true if invalid.
   Added some tests...
   Functions made private in ``txmath.h``.
   Idem for ``xmlaction``.
   Bug correction: ``\typeprojet`` : ``\localisation`` is obsolete.
   ``Buffer::get_wptr()`` replaced by ``Buffer::length()``.
   Special hack for macros like ``\empty``.
   Some equivalents are defined at level one.
   [**2003-10-23**]
   Field ``math::name`` defaults to empty string, not null pointer
   (avoids core dump in ``scan_math_aux``).
   Bug in ``math::handle_mbox`` fixed.
   Instead of: Label multiply defined apprentissage2 we have : Label
   'apprentissage2' multiply defined (first use line 625).
   Instead of: undefined label track-arche we have : undefined label
   \`track-arche' (first use at line 748).
   No more Strange keywords cha\^{\i}nes.
   Removed final dot in keywords.
   This is a valid keyword: ``c{\oe}ur``.
   Implemented ``\oldstylenums`` like ``\textsuperscript`` and
   ``\textsubscript``.
   Implemented ``\linebreak`` and ``\nolinebreak`` as no-op with
   optional argument.
   *Tralics* counts lines properly for Unix, Mac and Windows.
   If you say ``EDITOR = {A. Drouin & G. Eude & J.Robert}`` you get an
   error of the form bad syntax in author or editor name.
-  [**version 1.9j, 2003-10-24**]
   Added special hack for empty macros everywhere.
   Better error message for ``\addtolength{toto}{2cm}``.
   Same for ``\addtolength{\toto}{2cm}``.
   This is an error now:
   ``\setlength{2}{4cm},        \addtolength{x}{3cm}``.
   Corrected error message for ``\DefineShortVerb{a}``.
   Command ``\ifeof`` works now.
   ``{\escapechar=-1 \string\foo}`` gives ``foo``.
   Implemented ``\openout``, ``\write`` and ``\closeout``.
   *Tralics* signals unterminated ``\if`` conditions.
   This is ok: ``\iffalse \FPiflt{1}{2} \fi \toto \fi``.
   Evaluating ``$\undef$`` generates an understandable message.
   Evaluating ``$a^b^c$`` gives now : double superscript.
-  [**version 1.9k, 2003-10-30**]
   Added a missing ``flush_buffer()`` in ``\bibitem``.
   Command ``\newline`` invalid in table.
   Function ``T_bibitem`` was completely wrong...
   Attribute ``rowspan`` replaced by ``columnspan`` in math tables.
   Added a use_all_sizes option.
   [**2003-10-30**]
   Split file ``txparser.C`` into two parts. ``txscan.C`` is a new file.
   There is an indication in the log file whenever *Tralics*
   opens/closes a file.
   Commands ``\input`` and ``\endinput`` are fully expanded.
   Calls ``\input{foo}`` and ``\input foo`` are OK.
   Implicit call of ``save_the_state`` for ``\input`` (needed for
   ``\input A\input B``).
   Functions ``\openin`` and ``\closein`` are always traced, but only
   sometimes on the tty.
   Call :literal:`\\catcode `\A=9` works now.
   Function ``get_token`` completely rewritten.
   [print_mods]: ``this_line`` is never zero.
   Added a ``Stats`` object that collects a lot of statistics.
-  [**Version 1.9l, 2003-11-14**]
   Replaced ``scan_four_bit_ints`` by 3 functions.
   Implemented DOI (Digital Object Identifier).
   Added a -utf8, -latin1 option.
   ``\the\par`` generates the same error as in TeX.
   Use anonymous namespace instead of static variable.
   Removed ``xmlstats``...
   Added more Ctor to ``xml_action``.
   [**2003-12-10**]
   Made ``clines``, ``math_p_aux``, ``math_elt``, ``token_elt``,
   ``cond_aux`` subclasses of other classes.
   Corrected a bug for multi ``\UR``.
-  [**Version 1.9m, 2003-12-10**]
   ``\>`` has now the value ``\medmuskip``.
   ``\, \> \: \!``: same values as in MathML TR.
   ``$\bigl[\frac{1}{a^2}\bigr[$`` works.
   ``$\ensuremath{\cal} X$`` yields \\\ ``cal``.
   ``\RAsection`` calls ``leave_h_mode``.
   Corrected bug ``\begin{module}...text\end{module}``.
   Added ``\underleftarrow \underrightarrow``.
   ``\linebreak, \nolinebreak \pagebreak \nopagebreak`` -> ok.
   ``\urlfont`` and ``url_font`` in ``.tralics_rc``.
   ``xmlelement`` environment changed.
   Bug correction in ``\AtBeginDocument``.
   No spaces from bootstrap code.
-  [**version 1.9n, 2003-12-16**]
   Quotes :literal:`\``foo''` -> gives the word foo.
   You can use ``\begin{split}``.
   Added ``\tfrac`` as alternative to ``\frac`` and ``\dfrac``.
   Added ``\genfrac#1#2#3#4#5``.
   Added ``\DeclareRobustCommand``.
   Script placement of ``\lim_{x=0}`` and ``\sin^2(x)`` not the same.
   Use of a ``mstyle`` element, according to MathML standards.
   Bug in ``\underset``.
-  [**version 1.9o, 2003-12-22**]
   Character ``~`` is now active, expands to ~ of catcode 12.
   If \\\ ``def\foo#1 #2{}``, then a newline is a separator between
   ``#1`` and ``#2``.
   Added ``\bye``, same as ``\endinput``.
-  [**version 1.9p, 2004-01-09**]
   Some corrections on long arrows.
   Scope: ``\paragraph{\bf foo}`` : only foo is in \\\ ``bf``.
-  [**version 1.9q, 2004-01-09**]
   Added a space after the label of subfigure.
   Replaced type ``Token`` by ``token``.
   Replaced type ``Stack`` by ``stack``, and ``stack`` by ``my_stack``.
   Replaced type ``Modes`` by ``mode``.
   Replaced type ``Parser`` by ``parser``.
   Added classes ``eqtb_dim``, ``save_aux_dim``, and ``eqtb_dim_table``.
   Changed Copyright miaou/apics.
   Made nested environments work.
   Added ``\let\verbatimfont\tt\let\verbatimnumberfont\small``.
   Changed ``\unitlength`` to an internal dimension.
   Added ``\textheight=427pt``, ``\textwidth=570pt``
   Make ampersand in url work: they are like ``\char9738`` (signaled by
   Alain Giraud).
   Added ``remove_last_space()`` for ``\multicolumn``.
   Changed: Braces in math argument.
-  [**2004-01-15**]
   Changed ``\pers`` to use ``\@pers``.
   Change: ``\pers{a}{b}[c ]`` trailing space removed.
   Added ``\tralicsversion``.
   Bug: ``scan_int`` at end of token list pushes back an invalid token.
   Change ``\setcounter{bar}{17}`` gives Unknown counter \`bar'.
   Added a feature that counts cr-lf as two characters for Windows. So
   that the number of bytes printed is what claimed.
   Added trace of ``\loop``.
   Seeing a closing brace does not call ``flush_buffer()``; call done
   only by ``save_aux_font::unsave``.
   Bug in ``trace_if`` fixed. Fct ``if_serial`` moved from ``parser``
   class to ``condition``.
-  [**2004-01-22**]
   Changed ``array_info``, it holds two vectors : u and v.
   Renamed class ``Buffer`` as ``buffer``.
   Added support for German.
   Change: ``flush_buffer`` does not create a useless XML element.
   Made code of ``\par`` and Co clearer.
   Entities: *Tralics* may output &#xA0; instead of &nbsp;.
   This prints OK: ``\expandafter\show\csname^^I^^85^^97\endcsname``.
   Bug: :literal:`\\catcode`\^^I` does not change the catcode of space.
   First cell in table does not start with space.
   Bug: ``\dj\DJ`` is OK now.
-  [**2004-01-24**]
   Commands added for ``\=t\=T\=h\=H`` etc.
   Correction to: \`Seeing a closing brace does not call
   ``flush_buffer()``;' needed also in by ``save_aux_boxend::unsave``.
   Changed the type of math chars between 192 and 256.
   Corrected translation of ``\'i\"E``.
   Added some characters.
   Added ``\texteuro``.
   Change: ``\glo`` does not enter no_mode unless it was in this mode.
   Better debug in case of abort() because of no-mode.
   Added ``\newdimen\itemsep`` in the preamble.
-  [**version 1.9t, 2004-01-30**]
   Bug fixed in ``check_for_alias_type``.
   Bug fixed: ``\url{#}`` is ok, because # is active, bound to \\# (MPD
   for compsys).
   Bug in ``evaluate_now`` fixed.
   Titlepage works as explained in the Doc.
-  [**2004-02-02**]
   Bug fixed in ``\endinput``.
   Added option -notrivialmath.
   Titlepage hacked a bit. Made things more general.
   Added a semi-colon after ``see_image``.
   Language attribute name in main element is defined in the config
   file.
   Added ``\include``.
   Fixed: ``\end{module}`` restores the mode (v,h etc).
   Recoded the ``minipage`` environment.
   Bug fix: ``minipage`` sets locally centering to 0.
   New feature. Commands ``\makebox`` ``\framebox`` : pos argument
   handled.
   Bug fix: Underscore char in file name are allowed.
   Bug fix: ``\usepackage[canadien,german,austrian]{babel}``.
   Added variable ``in_load_handler`` for the problem of underscore in
   file names.
   Bug fix: ``\fbox{\includegraphics{../../tralics/TEST/a_b.eps}}``.
   Added variable ``cur_file_name``. Used for errors.
   Bug in ``\centering`` fixed.
   Added ``\scalebox{}{}``.
   Implemented ``\tabular*``.
   In the interactive case, ``\stop`` switches to non-interactive.
   Added option -shell-escape.
   Added Zapf Dingbats.
   Added various symbols from textcomp:
   ``\textcelsius \textwon  \textnaira \textlira \textflorin \textdong    \textperthousand \textpertenthousand \trextreferencemark \textdagget     \textdaggerdbl \textgravedbl \textacutedbl \textasciigrave \textasciiacutex   \textlquill \textrquill \textohm \textmho \textnumero \textcircledP    \textrecipe  \textservicemark \textestimated \textbullet \textinterrobang``.
-  [**2004-02-10**]
   Added a new file ``txcommands.C``.
   Added a new data structure: ``cmd_chr``.
   No more ~ of catcode 12.
   Removed some useless code.
   Re-implemented the stack.
   Change: ``att_list::add`` : does nothing if value is zero.
   Spaces are not logged in vertical mode.
   Removed ``xml::xml(string,int,bool)``.
   Removed ``buffer::array_spec2``.
   Removed ``att_list::array_spec0``.
   Added ``\newcolumntype``.
   Change: ``\cline`` allowed only after ``\\``.
   Re-implemented array. ``>{\large\bfseries 2}l <{y}|`` is now Ok. Thus
   ``\cr`` is added somewhere. Even at end of table.
   Removed useless ``xml::finish_tabular()``.
-  [**2004-02-14**]
   Reimplemented ``\multicolumn``.
   Bug fix with ``isframe("cell")``.
   Added parameterisation code. Corrected bugs.
   Added ``\let\par\@empty`` in a table, particip and catperso.
   Added a new file: ``txarray.C``.
   Spaces at end of cell removed.
   Bug fix: ``\includegraphics[height=.3\textheight]{...}``.
   Removed ``parser::pack``. Some const added.
   Removed ``no_for_eof`` hack.
   Ordered ``txparser.h`` in alphabetic order, and put everything
   private if possible.
   Added option footnotehack.
   Fix. ``stack::add_nl()`` adds a newline only if there is none.
   Replaced ``buffer::remove_last_nl()`` by ``stack::remove_last_space``
   used instead. This removes all spaces before a ``</p>``.
   Optional argument of ``\makebox`` is implemented.
   Bug fix in ``scan_glue``.
-  [**2004-02-19**]
   Changed default compilation mode. it is non-ra. dft defaults to 3.
   The list of themes is no more hard-coded.
   Bug fix ``$\hspace*{}1cm$``.
   Bug fix. ``\vskip\vskip`` call scan_glue 3 times. Thus
   ``find_special_scaled`` is useless, removed.
   Added ``\bigskip``, same as ``\vskip12pt``.
   Added ``\ignorespaces``.
   Bug fix: ``\vdots`` outside math.
   Marks added.
   Added ``\arc\bigcircle\closecurve\curve\scaleputtagcurve``.
   Added ``token_list_to_att``.
   Bug fix ``\begin{minipage}{\dimension}...``.
-  [**2004-02-24**]
   Implemented ``\newtheorem``.
   Removed special hack for verbatim parsing.
   Added ^^^^ABCD support.
   Changed the token organisation.
   Moved SH out of the stack.

2003, Version 1.8
-----------------

-  [date unknown] Bug in ``mem_alloc`` of ``titlepage::check_size``.
   Removed error message command code=17 in case of undefined
   command/environment.
   More explicit warnings.
   More realistic implementation of ``\write-1``.
   Added space in message restoringinteger value.
   ``\end{document}`` calls ``\endinput``.
   Changed the log.
   Only one ``push_par``.
   Added a function ``ok_for_label``.
   Removed ``label_arg``, it is useless now.
   Removed a fatal error.
   Added ``\AtBeginDocument \AtEndDocument``.
   Renamed ``xxx_val_type`` as ``it_xxx``.
   Renamed ``xxx_del`` as ``del_xxx``.
   Renamed ``xxx_mode`` as ``mode_xxx``.
   All command names finish with ``_cmd.``.
   Removal of global variables : ``cur_level``, ``unexpected_seen_hi``,
   ``global_in_url``.
   Added ``\AddAttToLast*``.
-  [**2003-06-18**]
   Added support for Topics.
   Replaced amac by miaou.
   Changed a little the banner.
-  **[Version 1.8b, 2003-06-18]**
   Functions ``mk_acc``, ``fetch_accent`` put in class ``Parser``.
   These functions are private.
   Changed the names of the section.
   Raweb sections names now in cls file.
   Details. Renamed ``hashtab::boot()`` as ``hashtab::hashtab()``.
   Some functions are private.
   Some functions are const.
   Added a class for macros, named ``mactab``.
   Replaced some ``ostring`` by ``string``.
   Removed ``hashtab::new_math``.
   Removed ``math_table`` from ``hashtab``.
   Added ``ctoken::ctoken(stoken)``.
   Removed function ``Parser::the_catcode``.
   Added class ``save_catcode``.
   Removed ``error_stack`` and ``tmp_error_stack``, and changed the
   error mechanism.
   Everything printed on the tty is also printed on the log file.
   Removed function ``fiche_ur``.
   Removed command ``\localisation``.
   Removed ``nb_sec2_mods``.
   Replaced ``nouveau module`` by ``new module``.
   Value of pdftitle is now Activity report.
   Removed references to ``lg_file``.
   [checked_stat] Some data are private.
   ``modlist`` is now a vector, ``cur_module`` is current module.
   ``\project`` is an alias for ``\projet``.
   Team instead of projet in all messages.
   Removed a goto.
   Variable ``ref_list`` is now a vector.
   ``logger_ctl`` removed (was useless) and printed a useless 0.
   The ``short_name`` attribute of module is machine generated.
   Added ``short_module`` name as new argument to
   ``Parser::T_start_ramodule``.
   [modname] File miaou_contrats_FT becomes miaou_module33.tex.
   Removed class ``module_list_item``.
   ``\begin{module}[]{domaine}{chapeau}{}`` is OK.
   Corrected a bug in ``Buffer::check_moduleref``.
   Variable ``the_projetval`` is now a field of ``main_data``.
   Variable ``the_env`` is now a vector.
   Changed the algo for printing the moreinfo stuff.
   Variable ``themod_cher`` is now a vector.
   Variable ``all_heads`` is a vector.
   Added option -noxmlerror.
   ``hyperref.cfg`` contains Team or Project-Team.
-  [ **Version 1.8f** date unknown]
   Removed the ``Main::signal_error()`` function.
   Removed ``main_data::fatal_err``, added ``bad3``, changed ``bad1``.
   No more ``tmp_int_to_string``.
   Funny bug: :literal:`{\catcode`\é=7 ééab}` was wrong (double bug).
   Removed ``print_token``.
   Added ``void Buffer::add(scaled_int V, glue_spec unit)``.
   Another bug corrected.
   File ``txbib.C`` changed.
   ``cite:gaujal-jcss??`` not transformed to ``cite:gaujal-jcss--``.
   Removed ``Buffer::normalise()``.
   Variable ``job_name`` is in ``main_data``. Printed in case of error.
   Printing of Fatal_error for every fatal message on ``log_and_tty``.
   No more ``concat``.
   Function ``double_hat`` and 2 other ones replaced by a single
   function.
   Call to ``flush_buffer`` in case of error.
   Bug corrected in ``is_verbatim_end``.
   [special_hash]: two ``vector<string>``. It's easier.
   ``\includegraphics[=foo,clip=]{...}`` OK.
   ``\escapechar=-1`` does something sensible.
   Replacement of ``string_table`` by ``vector<string>``.
   Use a vector in ``pack_matcher`` and other bug corrections.
   New class ``arith`` that contains ``radix`` and ``error`` and
   functions.
   Variable ``the_save_stack`` is a vector.
   Overflow in counter is no more fatal.
   Modified ``read_mac_body`` and ``read_delimited`` in order to share .
   Guillemets and space.
   Working on FP.
   Added some missing math symbols.
   [token_list]: new methods ``front``, ``back``, ``push_front``,
   ``push_back``, ``pop_front``, ``pop_back``.
   End-of-line character is now carriage return, not line-feed.
   ``T_arg1`` takes 2 arguments.
   Lots of elements have a name that can be changed.
   Type ``Token`` is now a class.
   Moved ``make_catcodes`` into ``tralics.C``.
   Renamed ``oparen_code`` etc to ``oparen_cmd``.
   Moved all global tokens into the ``hash_table``.
   ``\verb`` is now in translate, not expand.
   ``\par`` is allowed in a caption (like in a footnote).
   ``\usepackage[french,english]{babel}`` is OK.
   Implemented tex fonts...
   Removed all ``stoken`` and ``ctoken``.
   Added: ``\read \openin \closein``.
   ``Parser::get_token()`` instead of ``Parser::get_next()``.
   ``\textfont scriptfont \scriptscriptfont`` ok.
   tralics /toto/titi.tex generates an error, rather than trying to open
   w/toto/titi.tex.
   ``\fnsymbol`` is OK.
   All variables ``st_pos_XXX`` in ``str_hash``.
   Replaced ``stack.get_mode()==mode_v`` by ``stack.in_v_mode()``.
   Spaces are now traced.
   Init messages sent to ``log_and_tty``.
   Figure ``./foo/bar.ps`` transforms into ``foo/bar.ps``.
   Message: \\~{} is the wrong way to put a tilde in an URL.

2002-12, Version 1.7
--------------------

-  [date unknown]
   In bibtex, if the citation is numeric, optional argument in the alpha
   key.
   Partial handling of ``\begin`` inside comments.
   ``\catcode160=10``. Said otherwise, nonbreakspace is considered as a
   normal space.
   Message duplicate field ignored becomes duplicate field \`note'
   ignored, in the case of a note.
   ``\href{toto}{\url{titi}}`` gives two errors unexpected element hi,
   invalid url value. the first message becomes: unexpected font change
   tt. if the second is printed, it will be you should perhaps use
   \\href{\url{x}}{y} instead of \\href{y}{\url{x}}.
   ``editor = "\textsc{toto}"`` in the biblio gives unexpected font
   change sc. maybe you confused Publisher with Editor.
   Bug corrections: ``\c{c}`` OK in a bibname; ``\v{c}`` OK ; ``\a'{e}``
   OK.
   Some new commands:
   ``\sharp \natural \flat \oval \updownarrow \thinspace``.
   Message Module name already seen replaced by Module name \`result'
   already seen.
   ``$_$`` give no memory fault but Math formula should not finish with
   \_
   Calling tralics -all works.
-  **[Version 1.7a, 2002-12-20]**
   ``\htmladdnormallinkfoot{x}{\url{y}}`` works.
   ``\label{foo-bar}`` and ``\label{foo+bar}`` are now distinct.
   Bug correction ``\begin{table}\begin{tabular}...`` is OK.
   Order of bibliographic references is not hard-coded.
   ``\includegraphics{foo:bar}`` is OK...
   Corrected a bug in the date (first month=0).
   Bug in simplemath (``$x$=$y$`` gave y=y).
   Bug in bibtex crossref management (copy of wrong thing).
   ``read_config_file`` : is OK now.
   *Tralics* accepts now empty input files.
   bug in etal....
   bug in ``\epsfig 3pt`` translated to ``3fil``.
   bug in ``\pers{...}{...}[{\sc xx}]`` strange scope.
   missing attribute, in last cell of a tabular.
   added ``\leavevmode`` before ``\begin{tabular}``.
   Problems with tables.
   ``\footcite`` puts a space before.

Version 1.6
-----------

-  [date unknown]
   Implementation of
   ``\@ifundefined, \@ifstar, \@ifnextchar, \typeout``.
   Added option interactivemath. This prints the formulas on the the
   screen. No file is read.
   Instead of a message like Undefined command \\eta; command code = 157
   you get Math only command \\eta. Missing dollar not inserted.
   Definition of ``\begin{sloppypar} ... \end{sloppypar}``.
   In an url, ``\~`` is converted to a ``~``.
   In an url, ``_`` is of catcode 13, and not 11, it is defined to be
   identical to ``\_``. (problem of ``toto\_titi_tata``).
   Definition of ``\epsfbox``.
   A construction like :literal:`$7^{\mbox{\scriptsize \it i\`{e}me}}$`
   works. It is like ``7\ieme``.
   Added ``\P``, it is like ``¶``.
   Added
   ``\line \put \bezier \qbezier \vector     \thicklines \thinlines \dashlike \drawline  \dotteline``.
   ``\begin{minipage}[][][]{} ... \end{minipage}`` is translated.
   Implementation of ``\begin{xmlelement} ... \end{xmlelement}``.
-  **Version 1.6b** [date unknown]
   Implementation of ``\hbox, \vbox, \xbox`` etc.
   Silly bug ``\def\foo\xbox{\foo}{}`` loops without end.
   Correction of bugs in a token list.
   Bug in ``<<~`` : superfluous space.
   Bug in ``\medskip\par\noindent``.
   Added ``\mbox, \fbox, \makebox, \framebox`` with the syntax of
   ``picture``.
   Added the ``picture`` environment.
   Added ``comment`` environnement.
   If the final brace ``\end{module}`` is missing *Tralics* does not say
   : Unexpected end of file but indicates the line containing the
   ``\end``.
   If you say ``\htmladdnormallink{toto}(titi)`` (parentheses instead of
   braces) the error will be: Invalid URL value.
-  **Version 1.6c** [date unknown]
   Bug correction: the second ``\par`` in ``\par toto \par titi`` was
   ignored (synchronisation problem).
   Added ``\let\debugfalse\relax``.
   Instead of putting the biblio in ``toto.bbl``, I put it in ``toto_.bbl``.
   Bug of ``\{`` and ``\}`` in math mode, not completely handled.
   Bug of ``\item[{\bf x}]``; braces disappear.
   Bug of ``\begin{Verbatim} [numbers=true]``; the test of optional
   argument is strange.
   Bug of ``\,`` among others in math mode; there is an unwanted
   ``<mo>``.
   Bug: editeur twice in case of @conference.
   Bug of ``\newcount{toto}[titi]``, the ``\let\cl@toto\@empty`` did
   ignore the reference count.
   Other bug in the biblio.
   ``$$toto$$titi`` inserts a ``\par\noindent``.
   If you say ``\newcommand{\Athapascan0}{ok}`` the error message will
   be Attempt to define: \\Athapascan 0, Error signaled at line 188:
   Only one token allowed in argument list.
   ``\moduleref[1539]{PROJET}{section}{module}`` is an error, because I
   don't know how to translate into English (Everybody knows Marignan
   1515).
-  **Version 1.6d** [date unknown]
   Implementation of ``\dotsc \dotsb \dotsi \dotsm`` it's like
   ``\dots``.
   ``\geqslant, \leqslant`` ok.
   [raweb-cfg.sty] ``\ell`` works fine.
   Added\ ``\let\href\htmladdnormallink``.
   ``\not=`` and ``\not\in`` work.
   [rawebfo.xsl] added ``separateur.objet`` in citation/xref.
   Problem of ``\href{\url{&}}{&}`` This is accepted by the translator,
   but conversion to Pdf fails.
   Environment ``cases`` implemented.
   [raweb-cfg.sty] problem with ``\item`` : bullet non centered if TeX
   decides to start a new page because of the \\penalty of the \\item.
   *Tralics* signals an error if a) you say ``\label{x} \label{x}``, b)
   you say ``\ref{x}`` without the associated ``\label``, c) you say
   ``\ref`` and the label disappears [thanks JB Pomet].
   ``a\over b`` gives ``<mfrac>`` rather than a ``<mover>`` [pb. I.
   Vatton].
   Added namespaces in math elements.
   Order of biblio references changed.
   Implementation of
   ``\input, \endinput, \IfFileExists, \InputIfFileExists``.
   Bug corrections: in ``\small``, nothing happened; in
   ``\setbox3=\hbox{}``, assignment was local;
   ``{\large \hbox{\small x}}`` gave an empty ``<large>`` in the hbox.
   ``\newcommand\foo{\begin{latexonly}...`` calls abort, instead of
   memory fault (project Trec).
   Some ``(Sans titre)`` disappear, other become ``(Sans Titre)``.

Version 1.5
-----------

-  [date unknown]
   ``\hspace`` allowed in maths. For instance ``$I\hspace{-0.1cm}R^n$``;
   Not bad in Pdf, not recognized by amaya.
   The file wmachin.cfg is not created if invalid (following a mail of
   Vincent Lefèvre).
   *Tralics* looks at the shell variable ``TRALICSDIR`` before
   ``RAWEBDIR``.
   *Tralics* copies catalog.src in catalog in the current working dir
   replacing ``source_directory`` by whatever needed (catalog.src is a
   new file, to be added in the package).
   ``\includegraphics{toto.ps}``, ``\includegraphics{a.b.c}`` are
   translated into ``<graphics file='toto'/> <graphics file='a.b.c'/>``.
   Correction of a silly bug for the Sharp Team.
   A second one.
   The error message Non character found in environment name: becomes
   Non-character found in \\includegraphics.
   (For the Sharp Team). Added a new error message that reads Things
   like {\'{}} are a bit strange (they use it for an apostrophe).
   Implementation of ``\uppercase``, \\\ ``lowercase``,
   ``\MakeUppercase``, ``\MakeLowercase``.
   Add/test of the commands
   ``\oe\OE\o\O\ae\AE\dh\DH\dj\DJ\l\L\ng\NG\ss\SS\th\TH`` (``\ng``,
   ``\NG`` do not work, ``\dj`` is strange).
   Problem ``\oe`` does not work in HTML.
   ``\mbox{\sc Vasy}`` is translated as ``{\sc Vasy}``.
   ``\begin{quote} ... \end{quote}`` is OK.
   Idem with ``flushleft`` and ``flushright``.
   ``\raggedleft`` ``\raggedright`` OK. (must modify rawebhtml.xsl and
   rawebfo.xsl).
   ``\begin{wrapfigure}{x}{y} ... \end{wrapfigure}`` is handled as
   ``\begin{figure}[x] ... \end{figure}``. (putting a figure near some
   text works only if somebody checks that the layout is OK).
   Silly bug. You should not see something like Checked 1 keyword env
   with 14 keywords (13 unique) because there are 15 words, all
   different.
   ``\ensuremath{\alpha}`` works.
   ``$\ensuremath{\beta}$`` puts too many braces, but there are other
   functions that do the same. Must correct this one day.
