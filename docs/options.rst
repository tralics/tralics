====================
Command-line options
====================

*Tralics* knows about many options, that start with a single or double
hyphen. Some options take an argument, that is placed just after the
option, the syntax option=argument, without spaces is allowed. You can
also use spaces (option, space, equals, space, argument). Some options
are shown with an underscore, it is optional. They are the following, in
alphabetic order:

-confdir=\ *prefix*
   This is a prefix for an alternate location of tcf, ult, plt, and clt
   files. More than one prefix can be given, all will be used (latest
   first). Since version 2.11.4, the configuration file .tralics_rc is
   searched in this list, if not found in the current directory.

-compatibility
   Keeps compatibility with older versions.

-config=\ *FILE*
   Use FILE instead of default configuration file. This can be an
   absolute or relative pathname.

-config_file=\ *FILE*
   Same as -config=FILE.

-default_class=\ *CLASS*
   Assume that you compile a file that has ``\documentclass{foo}``; in
   this case, *Tralics* tries to read the file foo.clt. No error is
   signaled if the file cannot be opened. If you call *Tralics* with
   option -default_class=bar, then bar.clt is loaded in the case foo.clt
   does not exists; the command ``\CurrentClass`` will hold the name of
   the document class (here “foo”), so that bar.clt knows how it was
   called.

-distinguish_refer_in_rabib=true/false
   This indicates whether, for the raweb, the “refer” bibliography
   database should be considered a normal file, or a subfile for the
   year database. Values other than yes, no, true, false, are ignored.
   Default is yes. The configuration may also specify
   distinguish_refer_in_rabib=xx. In case of conflit, the parameter is
   used. Values other than yes, no, true and false are ignored. Details
   are explained in section `2.8 Bibliography <raweb.html#rabib>`__.

-doctype=\ *A-B*
   This specifies the DOCTYPE of the resulting XML document. Here A is
   the name of the root element, B is the name of the DTD file. The two
   quantities A and B are separated by a dash or a space (in this case,
   quotes are needed, so that "A B" read as a single argument).

-dualmath
   The LaTeX source of math formulas is added to the MathML expression.
   This is the same as using ``\@nomathml=-3`` in the document (see
   `documentation <doc-n.html#cmd-nomathml>`__)

-entnames=yes/no
   This tells *Tralics* to put ``&alpha;`` or ``&#x3B1;`` in the XML
   output. If entity names are used, the resulting XML file can be used
   only if the DTD explains how they are defined. Since version 2.9,
   this option applies only to math characters, that are described in
   the MathMM DTD.

-etex
   This option enables e-TeX extensions. Options are enabled by default.

-find_words
   The list of all words of the XML tree will be printed on the file
   words, after translation is complete.

-help
   The list of main options are printed, and the program stops.

-input_data =\ *VALUE*
   In this case *Tralics* translates the content of the argument VALUE
   and prints the translation of all math formulas it sees (and nothing
   else). No XML file is generated. No error is printed. The transcript
   file is texput.log.

-input_dir =\ *DIR*
   Specifies where source files are to be found. This is a colon
   separated list of directories; current directory is defined by an
   empty slot, or a single dot. It will be added to the end of the list,
   unless present.

-input_path = *DIR*
   Same as input-dir=DIR

-input_file =\ *FILE*
   This tells *Tralics* to compile file FILE (.tex suffix added if
   needed)

-interactivemath, -i
   Special interactive mode, in which characters are read from the
   terminal, instead a file, and math formulas are printed on the
   screen. Input encoding is UTF-8 (it will be latin1 if option -latin1
   comes after option -i).

-latin1
   This option means that input files are by default encoded in
   iso-8859-1 mode (if the first line of the file contains
   ``-*-  coding: utf-8`` it is read in UTF-8 mode).

-leftquote=\ *xxx*
   Defines translation of left quote (or backquote); see below under
   rightquote.

-log_file =\ *FILE*
   This tells *Tralics* to put all messages and warnings in FILE
   (extension .log added if not given); the file is locate in the
   directory specified by output_dir. Defaut value is the name of the
   input file.

-math_variant
   If you say -math_variant then ``$\mathfrak B$`` translates into
   ``<mi mathvariant='fraktur' >B</mi>``. If you say -no_math_variant
   then ``$\mathcal A$`` translates into ``<mi>&#x1D49C;</mi>`` or
   ``<mi>&Ascr;</mi>``. Before version 2.9.4, the result depended on the
   font. The default is to use Unicode characters rather than
   attributes.

-multi_math_label
   This option sets the counter ``\multi@math@label`` to 1. By default
   the counter is zero, and a math formula has at most one equation
   number, so at most one label, anchored to the ``<formula>`` element.
   If the counter is 1, then a math formula , for instance defined by
   the ``split`` environment, has an equation number per row, and one
   label is allowed for each ``<mrow>`` element. New in version 2.15.4

-noconfig
   No configuration file will be used.

-noentnames
   This is the same as entnames=no. This tells *Tralics* to put
   ``&#x3B1;`` rather than ``&alpha;`` in the XML output. See option
   -entnames above.

-noetex
   This option disables e-TeX extensions. Options are enabled by
   default.

-no_float_hack
   This option disables the postprocessor for tables and figures.

-nomathml
   Math formulas are output à la TeX, rather than using MathML. This is
   the same as using ``\@nomathml=-1`` in the document. (see
   `documentation <doc-n.html#cmd-nomathml>`__)

-no_math_variant
   This is the converse of -math_variant described above.

-no_multi_math_label
   This is the converse of -multi_math_label described above.

-notrivialmath
   Inhibits trivial math hacks; see option trivialmath below. Since
   version 2.8.5 the default is trivialmath=1.

-noundefmac
   If you issue this option, all commands will be magically defined
   after the first error message saying that the command is undefined.

-nostraightquotes
   If you issue this option, the apostrophe translates into character
   U+B4, as ``\textasciiacute``. However, the normal value is used in
   verbatim mode, when reading a file name, in an URL, or in a construct
   like ``\char'``.

-noxmlerror
   This option inhibits creation of ``<error/>`` elements in case of
   error. These elements can be useful for debugging, since they
   indicate the current position in the tree; on the other hand, they
   could generate a second error, of the form: *expected element error*.

-nozerowidthelt
   In order to inhibit ligatures when converting XML to Pdf some action
   is required: a special character (zero width space) is added. This
   character appears sometimes as a normal-width-space. As a
   consequence, the ``<zws/>`` is used (and the style sheet that
   converts XML to HTML can ignore this element); If this switch is
   used, character entity ``#x200B;`` is used instead of an element.

-nozerowidthspace
   Translation of ``\verb=--=`` is
   ``<hi rend='xml_in'>-&#x200B;-&#x200B;</hi>``. The ZeroWidthSpace
   character (or some element, see above) is added so that, in case the
   XML file is interpreted by a software like TeX, no en-dash ligature
   is created. This behavior is inhibited by the switch.

-oe8, -oe1, -oe8a, -oe1a
   This specifies output encoding, one of UTF8 or latin1. Interpretation
   of the XML file is independent of the encoding. If the letter a is
   given, then all non-7 bits characters are printed as character
   references. Thus, the only difference between option e8a and oe1a is
   the XML header line. Defaut encoding is UTF8.

-oldphi
   This is a compatiblity option, which gives a wrong translation for
   ``\phi \varphi``.

-output_dir =\ *DIR*
   Specifies the directory in which output files are to be stored (this
   concerns the main XML file, the transcript file, and other files). By
   default, this is the directory containing the input file.

-output_file =\ *FILE*
   This tells *Tralics* to put the result in FILE (extension .xml added
   if not given). [In the case of the raweb, this option is ignored]

-param=\ *FOO=BAR*
   The effect is the same as if FOO="BAR" were in the configuration
   file, after all other values. Syntax is a bit special: both equal
   signs can be replaced by a space.

-raw-bib
   If this option is givem, *Tralics* considers all bibtex entry types
   alike. Otherwise, it may ignore irrelevant fields (for instance,
   editor for a phdthesis).

-rightquote= *xxx*
   Defines translation of right quote (or apostrophe). Characters
   \`(left quote) and ' (right quote) must have category code 12 (other)
   otherwise some constructions fail. In verbatim mode, (as well as in
   Urls) these characters behave normally; if doubled, translation is
   U+201C and U+201D (“ and ”). Otherwise you can change. If you say
   -leftquote=2018 and -rightquote=2019 then characters U+2018 and
   U+2019 are used (‘ and ’). Only base16 digits are allowed; the value
   should be a number between 1 and 2^16 (otherwise default value is
   used).

-shell-escape
   If you issue this command, ``\write18{pwd}`` will execute the \`pwd'
   command.

-silent
   This makes *Tralics* less verbose (prints the title of a \`div0'
   instead of \`div0' or \`div1'; prints less statistics; omits image
   info).

-te8, -te1, -te8a, -te1a
   This specifies encoding used in transcript files. In the case of te8
   or te8a, characters are printed using UTF-8 format; in the case of
   te1 ot te1a, characters are printed using latin1 encoding. Characters
   are printed using the ^^^^abcd notation in case: the value if greater
   than 255, and one of -te8a, -te1a is given, or the character is not
   in proper range (32-126 plus 160-155) and te1 is given, the character
   is smaller then 32. Note: horizontal tabulation, line-feed and
   carriage return do not use the ^^ notation. Default: same as output
   encoding.

-tpa_status=\ *XX*
   This flags controls what is to be translated if the configuration
   file specifies a titlepage (see `description <titlepage.html>`__). If
   the value is \`all', then the whole document is translated; if the
   value is \`title', only the titlepage is translated; if the value is
   \`config', action depends on the configuration file (details can be
   found `here <titlepage.html#titlepage-action>`__). Otherwise,
   translation stops in case of an error, continues otherwise. Only the
   first character of the value is tested. Capital letters are allowed.

-trivialmath=\ *N*
   This sets the counter ``notrivialmath`` to N. If N is 7, *Tralics*
   replaces ``$12^{th}$`` by ``12\textsuperscript{th}``, and handles
   formulas like ``$x$`` as \`trivial math' formulas. Translation is
   ``<formula type='inline'>  <simplemath>x</simplemath> </formula>``
   instead of a more complicated MathML expression. No MathML expression
   is created in the case of ``$\alpha$`` or ``$_{foo}$``. Default value
   of the counter is 1.

-type=\ *FOO*
   Uses FOO instead of the \\documentclass value for the document type.
   If you say tralics -type ra hello, this will read the ra.tcf file,
   enter Raweb mode and fail; on the other hand, if miaou2003.tex is a
   raweb file, tralics -type ra2007 miaou2003 may fail because the
   configuration file for year 2007 may be incompatible with that of
   2003.

-usequotes
   This option converts double quotes into a pair of single quotes,
   either left quotes, or right quotes.

-utf8
   This option means that input files are by default encoded in UTF-8 mode (if
   the first line of the file contains ``-*- coding: iso-8859-1`` it is read in
   latin1 mode).

-utf8output
   Same as option -oe8 -te8.

-verbose, -v
   This makes *Tralics* more verbose. In particular, you will see how
   *Tralics* determines the DTD. It executes ``\tracingall`` (in the
   example of miaou, this prints 9233 lines instead of 142 in the
   transcript file).

-verbose-doc, -V
   The effect of this switch is to execute ``\tracingall`` when
   ``\begin{document}`` is seen; this is like the ``verbose`` switch,
   but you won't see all details of classes and packages.

-version
   Version number is printed; program stops.

-year=\ *XXXX*
   This option sets an internal count register, accessible via the
   command ```ra@year`` <doc-r.html#cmd-rayear>`__. The default value is
   the current year, with an offset: (for this purpose year 2005 starts
   at May, 1st, and terminates at the last of April 2006). You cannot
   use the option when compiling the Raweb, and the year is extracted
   from the file name.

--------------

The following options were once used for the Raweb; they have been
removed in version 2.13.

-all
   Is the same as -xml -xmlfo -xmlhtml -xmltex -xmllint.

-check
   This checks the syntax, but produces nothing (no XML, no LaTeX). Does
   nothing outside Raweb mode.

-dir=\ *RADIR*
   This explains that RADIR is the directory containing lots of stuff
   for the Raweb mode. In particular, it contains a subdirectory confdir
   with the configuration file. If this option is not used, the value of
   the shell variable ``TRALICSDIR`` will be used instead.

-external_prog=\ *XXX*
   In this case XXX will be used instead of rahandler.pl as interpreter
   for the raweb actions defined by the xmlXXX switches.

-hacknotitle
   If you give this option, *Tralics* may replace ``\section{}`` by
   ``\section{Introduction}``. This is implied in raweb mode until 2006.
   The option was withdrawn in 2007: using it has no effect on the
   translation.

-interactivebib
   Withdrawn option.

-nobibyearerror
   Useful in Raweb mode only. No error is signaled if you cite a paper
   of year X in year Y when X and Y are not the same.

-nobibyearmodify
   Useful in Raweb mode only. No modification done if an entry is in the
   refer section instead of being in the year section. Details are
   explained in section `2.8 Bibliography <raweb.html#rabib>`__.

-ps
   This checks the syntax, creates a TeX file instead of XML, and calls
   an external program to convert the TeX into PostScript. Does nothing
   outside Raweb mode.

-radebug
   This option tells the Raweb checker to continue checking after the
   first error instead of stopping immediately; note that the translator
   is not called if the Raweb checker sees an error.

-xml
   This option asks *Tralics* to convert the TeX source into an XML
   result. Is incompatible with options -check or -ps. Is selected, when
   compiling the Raweb, if neither -ps nor -check is given.

-xmlall
   Is the same as -xml -xmlfo -xmlhtml -xmltex -xmllint.

-xmlfo
   This asks for the creation, via an external program, of a xsl-fo
   file. It implies the -xml option. Is meaningful only in Raweb mode,
   where an external program is launched.

-xmlhtml
   This asks for the creation, via an external program, of a set of HTML
   files. It implies the -xml option. Is meaningful only in Raweb mode,
   where an external program is launched.

-xmllint
   This checks, via an external program, the validity of the XML against
   the DTD. It implies the -xml option. Is meaningful only in Raweb
   mode, where an external program is launched.

-xmltex
   This asks for the conversion, via an external program, of the xsl-fo
   file to a Pdf file. Implies -xml and -xmlfo. Is meaningful only in
   Raweb mode, where an external program is launched.
