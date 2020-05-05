This page describes how to produces the activity report of INRIA. It is
Chapter 3 of `Tralics and the Raweb <raweb.html>`__

.. _run0:

3. Running the Programs for the Raweb
=====================================

You have the following choices:

#. You are not interested in the raweb, and you just say tralics myfile.
   This produces a beautiful XML document.
#. You do not have tralics on your machine: you run latex (bibtex,
   dvips, etc) on your TeX document. The gives a dvi or PS version of
   your document (with \`draft' on each page in the case of the Raweb).
#. You can say tralics -check. This will check the syntax, and produce a
   transcript file, and nothing more.
#. You can say tralics myfile -ps. This is more or less the combination
   of the two previous options. However, the order of the modules might
   be changed. This generates a PostScript file with the Inria Logo on
   it.
#. You can say tralics myfile -myoptions. This produces a beautiful XML
   document, and more. For instance, if you tralics -xmltex, you will
   get a pdf file, provided that all files and scripts are available on
   your machine (normally, everything should be included in the raweb
   package, contact maintainers of this package in case of problem).

Since 2008, all these funny options have been removed, and *Tralics*
just produces the XML file.

3.1 Options of the program
--------------------------

Options of the program are available on a `separate
page <options.html>`__.

3.2 Running latex
-----------------

You can run latex on the file. This works provided that the raweb class
file is available (until 2006). This is the first run

.. container:: tty_out

   ::

      grimm@medee$ latex miaou2003
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou2003.tex
      LaTeX2e <2003/12/01>
      Babel <v3.8c> and hyphenation patterns for english, UKenglish, french, german, 
      ngerman, italian, spanish, dumylang, nohyphenation, loaded.
      (/usr/local/lib/texmf.local/tex/generic/inria/inputs/ra2003.cls
      Document Class: ra2003 2003/06/12 v3,1 Copyright INRIA/MIAOU 2003 (E version)
      (/usr/local/lib/texmf/tex/latex/base/article.cls
      Document Class: article 2004/02/16 v1.4f Standard LaTeX document class
      %%Stuff about packages omitted
      No file miaou2003.aux.
      No file miaou2003.fb.bbl.
      No file miaou2003.fb.aux.
      No file miaou2003.toc.
      No file miaou2003.refer.bbl.
      No file miaou2003.bbl.
      %% Lots of warnings omitted
      LaTeX Warning: There were undefined references.
      LaTeX Warning: Label(s) may have changed. Rerun to get cross-references right.)
      (see the transcript file for additional information)
      Output written on miaou2003.dvi (11 pages, 22628 bytes).
      Transcript written on miaou2003.log.

The start of the run could also be, in 2006, something like

.. container:: tty_out

   ::

      grimm@medee$ latex apics2006
      This is pdfeTeX, Version 3.141592-1.30.3-2.2 (Web2C 7.5.5)
      entering extended mode
      (./apics2006.tex
      LaTeX2e <2003/12/01>
      Babel <v3.8d< and hyphenation patterns for american, french, german, ngerman, 
      bahasa, basque, bulgarian, catalan, croatian, czech, danish, dutch, esperanto,
      estonian, finnish, greek, icelandic, irish, italian, latin, magyar, norsk, 
      polish, portuges, romanian, russian, serbian, slovak, slovene, spanish, 
      swedish, turkish, ukrainian, nohyphenation, loaded.
      (./ra2006.cls
      Document Class: ra2006 2006/07/25 v3,2 
         Copyright INRIA/MIAOU/APICS 2006 Raweb in DRAFT mode
      (/usr/local/teTeX/share/texmf-dist/tex/latex/base/article.cls

Run bibtex on each of the three files. **Note**: the arguments of bibtex
are not completely obvious. The file miaou2003.fb.aux is created by the
footbib package, and the use of the ``\footcite`` command, the file
miaou2003.refer.aux is created by the ``\loadbiblio`` command.

.. container:: tty_out

   ::

      grimm@medee$ bibtex miaou2003
      This is BibTeX, Version 0.99c (Web2C 7.3.1)
      The top-level auxiliary file: miaou2003.aux
      The style file: raweb.bst
      Database file #1: miaou2003.bib
      Warning--empty booktitle in seyfertIMS2003
      (There was 1 warning)
      grimm@medee$ bibtex miaou2003.fb
      This is BibTeX, Version 0.99c (Web2C 7.3.1)
      The top-level auxiliary file: miaou2003.fb.aux
      The style file: rawebalpha.bst
      Database file #1: miaou_foot2003.bib
      grimm@medee$ bibtex miaou2003.refer
      This is BibTeX, Version 0.99c (Web2C 7.3.1)
      The top-level auxiliary file: miaou2003.refer.aux
      The style file: rawebnum.bst
      Database file #1: miaou_refer2003.bib

Run latex a second time

.. container:: tty_out

   ::

      grimm@medee$ latex miaou2003
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou2003.tex
      %% Lots of warnings/messages omitted
      LaTeX Warning: There were undefined references.
      LaTeX Warning: There were multiply-defined labels.)
      (see the transcript file for additional information)
      Output written on miaou2003.dvi (15 pages, 42256 bytes).
      Transcript written on miaou2003.log.

Run latex a third time. This will suppress the \`undefined references'
message.

.. container:: tty_out

   ::

      grimm@medee$ latex miaou2003
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou2003.tex
      %% Lots of messages omitted
      LaTeX Warning: Label `MIAOU@resultats@' multiply defined.
      %% Lots of messages omitted
      LaTeX Warning: There were multiply-defined labels. )
      (see the transcript file for additional information)
      Output written on miaou2003.dvi (15 pages, 42264 bytes).
      Transcript written on miaou2003.log.
      grimm@medee$ dvips  miaou2003 -o
      This is dvips(k) 5.94a Copyright 2003 Radical Eye Software (www.radicaleye.com)
      ' TeX output 2004.07.29:1056' -> miaou2003.ps
      <tex.pro><special.pro>. [1] [2] [3] [4] [5] [6] [7] [8] [9<xml-route.ps>] 
      [10] [11] [12] [13] [14] [15]

The result is the following `miaou2003.ps <miaou2003.ps>`__ . There are
two sections \`New results' (section 6 and 8), because there is a module
in section \`new results' after a module in section \`Contracts'. This
explains why LaTeX sees multiply-defined labels. There is an: Overfull
\\hbox (22.7474pt too wide) in paragraph at lines 19--19. Line 19
contains the ``\maketitle`` command. The overfull box contains the long
name of the Team. It is hard to remove it. Note: this is a draft
version. You should not try to add commands that remove overfull and
underfull boxes, this is completely useless.

3.3 Running Tralics
-------------------

When you run *Tralics*, whatever the arguments, it will print the
following lines. The third line (`Starting XX processing for YYY' may
change).

.. container:: tty_out

   ::

      This is tralics 2.2, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2004, Jos\'e Grimm
      Starting latex processing for miaou2003.
      Configuration file identification: standard $Revision: 2.3 $
      Read configuration file /net/home/grimm/cvs/tralics/.tralics_rc.
      For the environment ending near line 191
      please use `participant' or `participante' for the name,
      as there is only one \pers in the list.
      For the environment ending near line 253:
      please use `participants' or `participantes' for the name,
      as there are 2 \pers in the list.
      There are 63 environments
      Checked 5 keyword env with 19 keywords
      Checked 5 catperso and 9 participant(es) envs with 34 \pers
      There were 2 topics
      Sections (and # of modules): 1(1) 2(1) 3(1) 4(6) 5(5) 6(4) 7(3) 8(4) 9(3).

We already explained the messages about singular/plural in the
``participant`` environment. The preprocessor has seen 63 environments
(5 ``motscle``, 5 ``catperso``, 9 ``participant``, 28 ``module``, and
some others, namely one ``moreinfo,`` one ``glossaire``, one ``array``,
one ``figure``, two ``description``, two ``equation``, and 8
``itemize``). The total number of keywords is 19, the number of
``\pers`` commands is 34. They were two declared topics, and the last
line indicates the number of modules per section.

You can run tralics with option -check. In this case, the program checks
the syntax, prints some information, and that's all.

.. container:: tty_out

   ::

      grimm@medee$ tralics miaou2003 -check
      This is tralics 2.2, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2004, Jos\'e Grimm
      Starting check for miaou2003.
      %% Common output as above omitted

For the 2006 version, this changes a little bit. Note that there are no
artificial topics in the Apics RA.

.. container:: tty_out

   ::

      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting xml processing for apics2006.
      Configuration file identification: standard $ Revision: 2.18 $
      Read configuration file .tralics_rc.
      Configuration file identification: standard-ra $ Revision: 2.2 $
      Read tcf file for type: ../confdir/ra.tcf
      There are 157 environments
      You did not use any topic
      Sections (and # of modules): 1(1) 2(1) 3(2) 4(6) 5(5) 6(15) 7(2) 8(6) 9(3).

3.4 Running Tralics -ps
-----------------------

Let's try tralics -ps miaou2003. The effect of this command is to check
the source file, re-arrange the modules in the right order, and call
LaTeX. The ps option is not supported anymore in the 2008 version.

From the input file miaou2003.tex another source file miaou.tex (whose
content is given `here <raweb2.html>`__) is created. Essentially, it is
the initial file, with the modules in the right order; commands like
``\begin{module}`` are replaced by
``\RAstartmodule{1} {MIAOU} {6} {tralics}{...}`` (the first number is
the topics, the second number is the section). The document class is
``raweb``.

This file is compiled by an external program, launched directly by
*Tralics* some years ago, a Perl script later on. On some systems the
latex command does not recognize the -interaction switch, and you have
to say something like /mypath/mylatex \\nonstopmode\input{thefile}. For
these reasons, the names of the command to execute is not built-in, but
in the configuration file. We assume that the relevant lines are:

.. container:: ltx-source

   ::

        makefo="xsltproc --catalogs -o %A.fo %B %C";
        makehtml = "xsltproc --catalogs  %B %C";
        call_lint = "xmllint --catalogs --valid  --noout %C"
        makepdf = "pdflatex -interaction=nonstopmode %w"
        generatedvi = "latex -interaction=nonstopmode %t"
        generateps = "dvips %t.dvi -o %t.ps"

These lines are copied in the file user_param.pl and the Perl script
rahandler.pl is called on it (there is a command line option that allows
the use of another program). In the example that follows, there are
lines of the form todo_something=x. They correspond to the options of
tralics. Here ps is set to true, this explains that the script must call
latex on the file apics.tex. Here html is set to false, true would mean
that conversion from apics.xml to apics.html is required.

.. container:: ltx-source
   :name: user-param

   ::

      $::makefo='xsltproc --catalogs -o %A.fo %B %C';
      $::makehtml='xsltproc --catalogs  %B %C';
      $::checkxml='xmllint --catalogs --valid  --noout %C';
      $::makepdf='pdflatex -interaction=nonstopmode %w';
      $::makedvi='';
      $::dvitops='';
      $::generate_dvi='latex -interaction=nonstopmode %t';
      $::generate_ps='dvips %t.dvi -o %t.ps';
      $::tralics_dir='/user/grimm/home/cvs/tralics';
      $::ra_year='2006';
      $::no_year='apics';
      $::tex_file='apics';
      $::todo_fo=0;
      $::todo_html=0;
      $::todo_tex=0;
      $::todo_lint=0;
      $::todo_ps=1;
      $::todo_xml=0;
      1;

This is now what you see on the screen.

.. container:: tty_out

   ::

      grimm@medee$ tralics miaou2003 -ps
      This is tralics 2.2, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2004, Jos\'e Grimm
      Starting latex processing for miaou2003.
      %% Common output as above omitted
      /user/grimm/home/bin/rahandler.pl v2.5, (C) 2004 INRIA, José Grimm, projet APICS
      copy-and-exec for miaou
      latex -interaction=nonstopmode miaou
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou.tex
      LaTeX2e <2003/12/01>
      Babel <v3.8c> and hyphenation patterns for english, UKenglish, french, german, 
      ngerman, italian, spanish, dumylang, nohyphenation, loaded.
      (raweb.cls
      Document Class: raweb 2001/06/14 v2,2 Copyright INRIA/MIAOU 2001
      (/usr/local/lib/texmf/tex/latex/base/article.cls
      Document Class: article 2004/02/16 v1.4f Standard LaTeX document class
      %%more latex trace
      No file miaou.aux.
      No file miaou.fb.bbl.
      No file miaou.fb.aux.
      No file miaou.toc.
      %%more latex trace
      No file miaou.refer.bbl.
      No file miaou.bbl.
      [10] (miaou.fb.aux) (miaou.aux)

      LaTeX Warning: There were undefined references.
      LaTeX Warning: Label(s) may have changed. Rerun to get cross-references right.)
      (see the transcript file for additional information)
      Output written on miaou.dvi (11 pages, 19004 bytes).
      Transcript written on miaou.log.
      exit status of latex is 0
      bibtex -min-crossrefs=1000 miaou
      bibtex exit status 0
      bibtex -min-crossrefs=1000 miaou.refer
      bibtex exit status 0
      bibtex -min-crossrefs=1000 miaou.fb
      bibtex exit status 0
      latex -interaction=nonstopmode miaou
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou.tex
      %%more latex trace
      LaTeX Warning: Label(s) may have changed. Rerun to get cross-references right.
      (see the transcript file for additional information)
      Output written on miaou.dvi (15 pages, 38616 bytes).
      Transcript written on miaou.log.
      exit status of latex is 0
      latex -interaction=nonstopmode miaou
      This is TeX, Version 3.14159 (Web2C 7.3.1)
      (miaou.tex
      %%more latex trace
      (miaou.aux) )
      (see the transcript file for additional information)
      Output written on miaou.dvi (15 pages, 38624 bytes).
      Transcript written on miaou.log.
      exit status of latex is 0
      dvips miaou.dvi -o miaou.ps
      This is dvips(k) 5.94a Copyright 2003 Radical Eye Software (www.radicaleye.com)
      ' TeX output 2004.07.29:1125' -> miaou.ps
      <tex.pro><8r.enc><texps.pro><special.pro>. [1<LogoRA2003.eps>
      <Logo-INRIA-couleur.ps>] [2] [1] [2] [3] [4] [5] [6] [7<xml-route.ps>] [8] 
      [9] [10] [11] [12] [13] 
      exit status of dvips is 0
      (Transcript written on miaou.rh.log)
      done

The result is the following `miaou.ps <miaou.ps>`__ .

There is one warning: Underfull \\vbox (badness 10000) has occurred
while \\output is active. Tracing TeX shows the following. The selected
breakpoint at end of page 7 has: t=151.71101 plus 4.88531, g=200.427
(here g is the size of the page, minus the size of the image, and t the
size of the text, the big difference between t and g explains the
underfull box). There is no page break between the title of module 6.2
and the first line of the text. There is possible page break between
this line and the start of 6.3. Parameters are t=201.6997 plus 12.54138
minus 0.94266 and g=200.427. This pagebreak is not chosen because it
would overflow by 0.33pt! Adding a bit of shrinkability to the parskip,
for instance ``\parskip =0pt plus 1pt minus 1pt``, is enough in this
case. But, remember: this is not the final document.

In case of errors, latex may complain, for instance as “Undefined
control sequence”. In this case, you will see something like “exit
status of latex is 256”. A zero status is OK, a non-zero one means an
error. If your code contains ``\def\foobar{0\foobar}\foobar``, you will
overflow TeX's memory. However, if you remove the 0, you will go in an
endless loop. If you kill the process, normally, this will kill the Perl
script, and the latex process. After that, you will have to debug (add
``\tracingall`` somewhere, and look at the transcript file, its name is
miaou.log)

3.5 Running Tralics -xml
------------------------

Let's start with a call of *Tralics* without options. This is what you
will see (you may notice that module statistics are no longer computed)

.. container:: tty_out

   ::

      This is tralics 2.13.0, a LaTeX to XML translator, running on medee
      Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file apics2007.tex.
      Configuration file identification: standard $ Revision: 2.24 $
      Read configuration file /user/grimm/home/cvs/tralics/confdir/.tralics_rc.
      Configuration file identification: standard-ra $ Revision: 2.3 $
      Read tcf file /user/grimm/home/cvs/tralics/confdir/ra2007.tcf
      Document class: ra2007 2008/07/16 v1.1 Activity Report, Inria 2007
      Translating section composition
      Translating section presentation
      Translating section fondements
      Translating section domaine
      Translating section logiciels
      Translating section resultats
      Translating section contrats
      Translating section international
      Translating section diffusion
      Bib stats: seen 91 entries.
      Seen 91 bibliographic entries.
      Math stats: formulas 612, kernels 212, trivial 3, \mbox 6, large 0, small 65.
      Buffer realloc 40, string 18431, size 669087, merge 13
      Macros created 2013, deleted 1766; hash size 2493; foonotes 1.
      Save stack +2635 -2635.
      Attribute list search 11990(1484) found 3831 in 6638 elements (1117 at boot).
      Number of ref 105, of used labels 47, of defined labels 109, of ext. ref. 24.
      Input conversion: 137 lines converted.
      There were 20 images.
      Output written on apics.xml (289437 bytes).
      No error found.
      (For more information, see transcript file apics2007.log)

Let's try apics2006 with version 2.8. We get

.. container:: tty_out

   ::

      grimm@medee$ tralics apics2006     
      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting xml processing for apics2006.
      %% Common output as above omitted
      Translating section composition
      Translating section presentation
      Translating section fondements
      Translating section domaine
      Translating section logiciels
      Translating section resultats
      Translating section contrats
      Translating section international
      Translating section diffusion
      Bib stats: seen 71 entries
      Seen 75 bibliographic entries
      %% Memory usage
      Used 1784 commands
      Math stats: formulas 607, non trivial kernels 321,
         special 7, trivial 141, \mbox 6, large 0, small 142.
      Buffer realloc 32, string 18447, size 704375, merge 13
      Macros created 85, deleted 0.
      Save stack +1828 -1828.
      Attribute list search 8640(1549) found 3514 in 6351 elements (1450 after boot).
      Number of ref 96, of used labels 46, of defined labels 90, of ext. ref. 26.
      Modules with 26, without 15, sections with 9, without 19
      There were 16 images.
      Output written on apics.xml (284415 bytes).
      No error found. %% Very important information here
      (For more information, see transcript file apics2006.log)
      %% Raweb specific commands, via the Perl script
      rahandler.pl v2.12, (C) 2004 2006 INRIA, Jos\'e Grimm, Team Apics
      Postprocessor did nothing

You can see that the Perl script has been called. It is configurated to
call a XSLT processor twice (producing a file miaou.fo, or a bunch of
HTML files), then calling pdflatex twice, then xmllint. In fact nothing
is done.

You could try tralics apics2006 -xmllint. No error is signaled: the file
apics2006.tex that we use to check the tools produces correct output!
You could try tralics miaou2003 -xmllint. The answer is that the
document produced by *Tralics* does not conform the DTD, because the
module starting at `line 201 <raweb1.html#201>`__ contains no text; in
reality, xmllint says the following:

.. container:: tty_out

   ::

      miaou.xml:152: element module: validity error : Element module content does 
      not follow the DTD, expecting (head , (moreinfo | keywords | participant | 
      participants | participante | participantes | anchor)* , ((div2 | div3 | 
      div4)+ | ((cit | p | list | note | formula | table | figure)+ , (div2 | 
      div3 | div4)*))), got (head keywords)
      </keywords></module>
                          ^

If you say tralics miaou2003 -xmltex, you will see the following.
**Note**: you ask TeX to interpret the XML code, this is much slower
anything else. Timings for apics 2006: Tralics 0.1 s, Latex 0.5s, Pdf
via XML 24s.

.. container:: tty_out

   ::

      [2]grimm@medee$ tralics miaou2003 -xmltex
      This is tralics 2.2, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2004, Jos\'e Grimm
      Starting xml processing for miaou2003.
      %% as above
      Output written on miaou.xml (38393 bytes).
      No error found.
      (For more information, see transcript file miaou2003.log)
      /user/grimm/home/bin/rahandler.pl v2.4, (C) 2004 INRIA, José Grimm, projet APICS
      exec-xml for miaou
      %% This converts miaou.xml into miaou.fo
      xsltproc --catalogs -o miaou.fo /net/home/grimm/cvs/raweb/src/rawebfo.xsl miaou.xml
      exit status of makefo is 0
      (not done) xsltproc --catalogs  /net/home/grimm/cvs/raweb/src/rawebhtml.xsl miaou.xml
      %% Call pdflatex on wmiaou
      pdflatex -interaction=nonstopmode wmiaou
      This is pdfTeXk, Version 3.14159-1.10b (Web2C 7.4.5)
       %&-line parsing enabled.
      (./wmiaou.tex{/usr/local/lib/texmf/pdftex/config/pdftex.cfg}
      LaTeX2e <2003/12/01>
      Babel <v3.8c>  and hyphenation patterns for english, UKenglish, french, german, 
      ngerman, italian, spanish, dumylang, nohyphenation, loaded.
      (/usr/local/lib/texmf/tex/xmltex/base/xmltex.tex
      xmltex version: 2002/06/25 v1.9 (Exp)

      Encoding = utf-8 (/usr/local/lib/texmf/tex/xmltex/base/xmltex.cfg)
      (./wmiaou.cfg
      Loading mathml support and raweb extensions
      ) (./miaou.fo (/usr/local/lib/texmf/tex/xmltex/passivetex/fotex.xmt)
      (/usr/local/lib/texmf/tex/latex/base/article.cls
      Document Class: article 2004/02/16 v1.4f Standard LaTeX document class
      %% More latex packages
      (/usr/local/lib/texmf/tex/latex/hyperref/pd1enc.def) (./hyperref.cfg)
      (/usr/local/lib/texmf/tex/latex/hyperref/hpdftex.def) (./fotex.cfg))
      No file wmiaou.aux.
      No file wmiaou.out.
      No file wmiaou.out.
      <Logo-INRIA-couleur.pdf, id=7, 411.5375pt x 106.3975pt>
      <use Logo-INRIA-couleur.pdf> <LogoRA2003.pdf, id=8, 299.1175pt x 341.275pt>
      <use LogoRA2003.pdf>
      LaTeX Warning: Reference `uid1' on page 1 undefined on input line 3.
      %% More latex warnings

      LaTeX Warning: There were undefined references.
      LaTeX Warning: Label(s) may have changed.Rerun to get cross-references right.
       ) ) )
      (see the transcript file for additional information)
      Output written on wmiaou.pdf (14 pages, 137378 bytes).
      Transcript written on wmiaou.log.

      exit status of pdflatex is 0
      %% Second run
      pdflatex -interaction=nonstopmode wmiaou
      This is pdfTeXk, Version 3.14159-1.10b (Web2C 7.4.5)
       %&-line parsing enabled.
      (./wmiaou.tex{/usr/local/lib/texmf/pdftex/config/pdftex.cfg}
      %% some messages omitted
      [2] [1] [2] [3]
      [4 <./xml-route.pdf>] [5] [6] [7] [8] [9] (./wmiaou.aux) ) ) )
      (see the transcript file for additional information)</usr/local/lib/texmf/fonts
      /type1/bluesky/cm/cmtt10.pfb></usr/local/lib/texmf/fonts/type1/bluesky/cm/cmbx1
      0.pfb></usr/local/lib/texmf/fonts/type1/bluesky/cm/cmsy7.pfb></usr/local/lib/te
      xmf/fonts/type1/bluesky/cm/cmr5.pfb></usr/local/lib/texmf/fonts/type1/bluesky/c
      m/cmex10.pfb></usr/local/lib/texmf/fonts/type1/bluesky/symbols/msbm7.pfb></usr/
      local/lib/texmf/fonts/type1/bluesky/cm/cmr7.pfb></usr/local/lib/texmf/fonts/typ
      e1/bluesky/cm/cmmi5.pfb></usr/local/lib/texmf/fonts/type1/bluesky/symbols/msbm1
      0.pfb></usr/local/lib/texmf/fonts/type1/bluesky/cm/cmmi7.pfb></usr/local/lib/te
      xmf/fonts/type1/bluesky/cm/cmr10.pfb></usr/local/lib/texmf/fonts/type1/bluesky/
      cm/cmmi10.pfb></usr/local/lib/texmf/fonts/type1/bluesky/cm/cmsy10.pfb>{/usr/loc
      al/lib/texmf/dvips/base/8r.enc}
      Output written on wmiaou.pdf (14 pages, 152502 bytes).
      Transcript written on wmiaou.log.
      exit status of pdflatex is 0
      (not done) xmllint --catalogs --valid  --noout miaou.xml
      (Transcript written on miaou.rh.log)
      done

The pdf is `here <wmiaou.pdf>`__. It is named wmiaou.pdf, a name that
does not conflict with the names miaou2003.ps and miaou.ps. Running
*Tralics* ten times in a row give the following size for the Pdf file:
152499 152499 152500 152500 152500 152500 152497 152497 152497 152497.
Strange, isn't it?
