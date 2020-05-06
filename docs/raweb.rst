.. _tralics:

Tralics and the Raweb
=====================

| Table of Contents
| `1. Tralics and the Raweb <#tralics1>`__
| `2. Overview of the system <#overview>`__
|         `2.1 Requirements of the raweb <#requi>`__
|         `2.2 Trivial examples <#triv-exp>`__
|         `2.3 Main document <#main-doc>`__
|         `2.4 Modules <#modules>`__
|         `2.5 Module Interface <#modules-i>`__
|         `2.6 First Module <#f-module>`__
|             `2.6.1 Original syntax <#fmodule3>`__
|             `2.6.2 The 2006 semantics <#fmodule6>`__
|             `2.6.3 The 2007 changes <#fmodule7>`__
|             `2.6.4 The 2008 changes <#fmodule8>`__
|         `2.7 References <#references>`__
|         `2.8 Bibliography <#rabib>`__
|             `2.8.1 Raweb Case <#rabib1>`__
|             `2.8.2 General Case <#rabib2>`__
|             `2.8.3 Extended Syntax <#rabib3>`__
|             `2.8.4 More hacks <#rabib4>`__
|             `2.8.5 Reducing entropy <#rabib5>`__
| `3 Running the programs <#run0>`__
|         `3.1 Options of the program <options.html>`__
|         `3.2 Running Latex <raweb4.html#run1>`__
|         `3.3 Running Tralics <raweb4.html#run2>`__
|         `3.4 Running Tralics -ps <raweb4.html#run3>`__
|         `3.5 Running Tralics -xml <raweb4.html#run4>`__
| `4 The auxiliary perl script <raweb5.html#script0>`__
|         `4.1 Postscript mode <raweb5.html#script1>`__
|         `4.2 XML mode <raweb5.html#script2>`__
|         `4.3 Evaluation of parameters <raweb5.html#percent>`__
| `5 The fotex.cfg file <raweb6.html#fotex>`__
| `6 The raweb-cfg.sty file <raweb6.html#racfg>`__
|         `6.2 Ampersands in URL <raweb6.html#ra-amp>`__
|         `6.3 MathML <raweb6.html#ra-mathml>`__
|         `6.4 Tables in MathML <raweb6.html#mathml-table>`__
|         `6.5 Pictures <raweb6.html#pictures>`__
|         `6.6 Tables <raweb6.html#fo-table>`__
|         `6.7 Getting labels right <raweb6.html#fo-label>`__
|         `6.8 The title page <raweb6.html#fo-titlepage>`__
|         `6.9 Math and images <raweb6.html#fo-emath>`__

--------------

.. _tralics1:

1.1 A short history
-------------------

This file explains the link between *Tralics* and the Raweb.

A short history of the Raweb may be found on the Inria internal web site
`Inria internal
website <http://www.inria.fr/interne/disc/apropos/chantiers/raweb-xml/histoire.html>`__.
The question concerns Inria's Annual Activity Report, also known as
“Rapport d'activité”, or “Annexe technique” to the RA or “annexes
scientifiques” to the RA. This is a document, written by the research
teams, at the end of the year N (October, November), and published in
March of year N+1.

Until the 1993 edition (published in 1994), only a paper version
existed. A LaTeX model was used since 1987, designed by Jacques André
then Martin Jourdan. See the following paper, `Une expérience
d'utilisation de LaTeX : le Rapport d'activité de
l'INRIA <http://cahiers.gutenberg.eu.org/cg-bin/article/CG_1988___0_17_0.pdf>`__
by Louarn in the first Cahiers Gutenberg.

In 1993, contacts were made with the Grif S.A. society, for the design
of a SGML DTD and a LaTeX-to-SGML converter. As a result, Philippe
Louarn was able to put on the web the `RA (year
1994) <http://www.inria.fr/rapportsactivite/RA94/RA94.html>`__ in its
HTML version. But this was judged too complicated, and for several
years, the HTML was directly produced from the LaTeX source, using
latex2html (example:
`miaou95 <http://raweb.inria.fr/rapportsactivite/RA95/miaou/miaou.html>`__).

In 1996, a working group (conducted by Albert Benveniste) gave new
specifications: independent modules, grouped into ten sections, etc. A
technical group was created, whose objective was to find a company that
could sell a software (maybe using XML as intermediate language). None
was found, but the design of modules (in LaTeX syntax) was well-defined
by Laurent Pierron and José Grimm. For the Ra98, a Perl script did some
preprocessing, splitting the LaTeX source into modules (one module per
HTML page) (example:
`miaou98 <http://raweb.inria.fr/rapportsactivite/RA98/miaou/miaou.html>`__).

In 1999, the Scientific Annexes to Inria's Annual Report were renamed
RAWEB, to emphasize the role played by the Web (it is available as a
CD-Rom, but no more printed by Inria).

In 2001, the Perl scripts mentioned above evolved into a LaTeX-to-XML
converter (some ideas were borrowed from latex2html, which is also a
Perl script). The main trouble was conversion from XML to Pdf, and we
used tools from the TeX community (by S. Rahtz and D. Carlisle) and
PdfLaTeX. On the other hand, the images in the HTML files were converted
by LaTeX. Additional software (indexes, etc.) was written/used by
Marie-Pierre Durollet.

This gave a complicated object: a Perl script, that converts a LaTeX
file into XML (using Omega as a subprocess for the math) followed by
another Perl script that extracts the math, calls latex, then dvips,
then pstoimg (a Perl script that calls ppmquant, pncrop, and so forth,
whose job is to produce a png image for the math formula), and finally
an XSLT processor for the effective conversion. (example:
`miaou2001 <http://raweb.inria.fr/rapportsactivite/RA2001/miaou/miaou.html>`__).

1.2 Birth of Tralics
--------------------

The big Perl script was rewritten as a C++ translator, renamed
*Tralics*, and got (for version 1.6) a first IDDN number in Dec. 2002.
This software was still able to produce a LaTeX preview of the Raweb:
The source is read, the syntax is tested, a LaTeX file is written for
each module, latex and bibtex are called, the resulting dvi is converted
to PostScript. But the same *Tralics* can be used in a different way:
the source is read, the syntax is tested, an XML file is created, an
XSLT processor is called to generate the XSL/FO, and PdfLaTeX generates
the Pdf (you can also generate an HTML version). (example:
`miaou2003 <http://raweb.inria.fr/rapportsactivite/RA2003/miaou/uid0.html>`__).

Note that in 2003, there were a few people writing their RA directly in
XML. As a consequence, the new *Tralics*, to be be used in 2004, does
only the bare minimum: it converts the LaTeX into XML. There is a Perl
script that does everything else (calling external programs like latex,
xsltproc, etc). A non-obvious point concerns conversion of math and
images into HTML. Currently, a Perl script extracts the math, uses LaTex
to produce a dvi file, which in converted to PostScript then png format.

Since 2005, the XML version of the Raweb is available on the web; its
DTD is different from what *Tralics* produces; the research report
`inria-0000077 <http://hal.inria.fr/inria-00000777>`__ describes both
DTDs and the XSL style sheet that does the conversion.

Since 2008, a raweb file is like another file. The pre-processor (with
all its fatal errors) has been removed, as well as the postprocessor.
All commands, specific to the raweb like ``\project`` are now defined in
a style file (to be used via a class file), they may use some primitives
that extract data from a configuration file.

.. _overview:

2. Overview of the system
=========================

.. _requi:

2.1 Requirements of the Raweb
-----------------------------

We give `here <raweb1.html>`__ an example of a real file. It is an
abridged version of the `Report of the MIAOU
project. <http://www.inria.fr/rapportsactivite/RA2003/miaou/uid0.html>`__
The input to *Tralics* is: one TeX file (that can input other TeX
files), one, two or three bibliography files, and some image files.
These three bibliography files can be merged into a single one; and the
filecontents environment might be used to insert the bibliography in the
TeX source.

In the example, we have three bibliography files:
`miaou2003.bib <raweb1.html#miaou2003bib>`__,
`miaou_foot2003.bib <raweb1.html#miaoufootbib>`__, and
`miaou_refer2003.bib <raweb1.html#miaoureferbib>`__ and one
`image <xml-route.png>`__ (The image is extracted from a power-point
presentation, converted to PostScript, then Pdf; the image shown here is
the png version of the PostScript file obtained via convert; you can see
`this on the web <xml-route-ra.png>`__, and notice a loss of quality.)
The image explains how *Tralics* is connected to the other processes.

When translating a file, *Tralics* can be in Raweb mode or not.
Differences between Raweb and non-raweb modes are tiny: they concern
essentially the bibliography (there are some tests that check if an item
is in the correct bibliography file). In Raweb mode, the input file
should be foo2004.tex, where foo matches the name of team, and the
document class should be ra2004 (replace 2004 by the current year).
Non-ascii characters, as well as uppercase letters are forbidden.

.. _triv-exp:

2.2 Trivial examples
--------------------

**Example 1.** An important point is that *Tralics* compiles only TeX
files, i.e. whose names end with .tex. If no extension is given, one is
added. The .xml extension is replaced by .tex.

.. container:: tty_out

   ::

      grimm@medee$ tralics my_file.xml
      This is tralics 2.13.0, a LaTeX to XML translator, running on medee
      Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Fatal error: Cannot open input file my_file.tex

**Example 2.** Earlier versions of *Tralics* refused to translate files
that are not in the current directory.

.. container:: tty_out

   ::

      grimm@medee$ tralics /floppy/foo etc
      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Fatal error
      Absolute or relative pathnames not allowed: /floppy/foo 

**Example 2bis.** Since version 2.10.7 *Tralics* accepts to translate
files that are not in the current directory. By default, the XML result
and transcript files are put in the same directory as the input, but
this can be changed.

.. container:: tty_out

   ::

      maclaurin@grimm$  tralics ../Test/hello -outputdir=/tmp -logfile=x -silent
      This is tralics 2.15.4, a LaTeX to XML translator, running on maclaurin
      Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file ../Test/hello.tex.
      Output written on /tmp/hello.xml (187 bytes).
      No error found.
      (For more information, see transcript file /tmp/x.log)

**Example 3.** In the examples 3 to 6, we use the -silent switch to
reduce verbosity, and we translate a trivial TeX file. Here we
demontrate the -dir switch. This option has currrently no effect
anymore.

.. container:: tty_out

   ::

      maclaurin@grimm$ tralics hello -dir none -silent
      This is tralics 2.15.4, a LaTeX to XML translator, running on maclaurin
      Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file hello.tex.
      Output written on hello.xml (187 bytes).
      No error found.
      (For more information, see transcript file hello.log)

**Example 4.** Here we say that we do not want a configuration file.

.. container:: tty_out

   ::

      grimm@medee$ tralics -noconfig hello -silent
      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting xml processing for hello.
      No configuration file.
      Output written on hello.xml (171 bytes).
      No error found.
      (For more information, see transcript file hello.log)

**Example 5.** In version 2.8, you could set the shell variable
``TRALICSDIR`` to a directory containing the configuration file. This
has no effect any more, and the translation is the same as in example 3.

.. container:: tty_out

   ::

      grimm@medee$ TRALICSDIR=.. tralics hello -silent
      This is tralics 2.13.0, a LaTeX to XML translator, running on medee
      ...
      (For more information, see transcript file hello.log)

**Example 6.** Here the configuration file is found via default rules.
Translation is the same as in example 3.

.. container:: tty_out

   ::

      grimm@medee$ tralics hello -silent
      ...
      (For more information, see transcript file hello.log)

In all these examples, the TeX file contains a single line
``Hello, world!``. The translation is

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE unknown SYSTEM 'unknown.dtd'>
      <!-- translated from latex by tralics 2.8-->
      <unknown>
      <p>Hello, world!
      </p></unknown>

There are some differences between version 2.8 and the current version.
For instance the compilation date has been added in the XML, thus making
it a bit larger (192 bytes instead of 171). The default encoding is now
UTF-8, this makes the file smaller; finally, the length of the version
number impacts the length of the XML result. The sentence “starting xml
processing for hello” has been replaced by “Starting translation of file
hello.tex”. If the -silent option is given, information about
configuration file is not printed on the terminal.

**Raweb mode** is entered if the *type* is ``ra``. The *type* can be
specified in the configuration file (the .tralics_rc file; we assume
that either no configurationn file, or the standard configuration one
will be used). If you call *Tralics* with option type=foo, then the
*type* will be ``foo``. Otherwise, if you call *Tralics* with option
config=bar2005, and if bar2005.tcf exists, the *type* will be ``bar``.
Otherwise, if the file has ``\documentclass{gee2003}``, and gee2003.tcf
or gee.tcf exists, then the *type* will be ``gee``; otherwise some
default type will be used. The tcf file will be loaded, if given or
deduced.

Let's assume that we have a file containing two lines, the first one is
``\documentclass{ra2003}``, the second one is ``Hello,     world!`` as
above. Assume that the file has three names xhello.tex, hello2003.tex
and hello2006.tex. All these files will be translated in Raweb mode. No
XML file will be produced unless the four digits that appear in the
document class are also in the file name, this is the case only for
hello2003.

**Example 7.** We translate here file xhello.tex. Since the file name
has no digits in it, you will get an error of the form *Input file name
must be team name followed by 2008*. With version 2.8, the -verbose
switch produces a great number of lines, explaining what happens
(current version is less verbose).

.. container:: tty_out

   ::

      grimm@medee$ tralics xhello -verbose
      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting xml processing for xhello.
      Configuration file identification: standard $ Revision: 2.24 $
      Read configuration file /user/grimm/home/cvs/tralics/.tralics_rc.
      Configuration file has type \documentclass
      Seen \documentclass ra2003
      Potential type is ra2003
      ...
      Configuration file identification: standard-ra $ Revision: 2.1 $
      Read tcf file for type: ../confdir/ra.tcf
      Using type ra
      ...
      dtd is raweb from raweb3.dtd (mode RAWEB2006)
      Fatal error: Please add 2006 in the name of the input file
      Fatal_error for xhello
      Say tralics --help to get some help

**Example 8.** You can use the option -year=2008; this sets a variable
to be 2008; in Raweb mode, this can produce an error of the form *Option
-year=2008 incompatible with year in source file*, is ignored otherwise.
Even without this switch, the file will not compile, because of a
mismatch between the year of the file, and that of the document class.

.. container:: tty_out

   ::

      grimm@medee$ tralics hello2006
      This is tralics 2.13.0, a LaTeX to XML translator, running on medee
      Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file hello2006.tex.
      Configuration file identification: standard $ Revision: 2.24 $
      Read configuration file ../confdir/.tralics_rc.
      Configuration file identification: standard-ra $ Revision: 2.4 $
      Read tcf file ../confdir/ra.tcf
      Illegal document class ra2003 should be ra2006

**Example 9.** Final test: we translate the file hello2003. *Tralics*
version 2.8 uses a preprocessor that complains in case of unbalanced
environments.

.. container:: tty_out

   ::

      [2]grimm@medee$ tralics hello2003
      This is tralics 2.8, a LaTeX to XML translator
      Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting xml processing for hello2003.
      Configuration file identification: standard $ Revision: 2.24 $
      Read configuration file /user/grimm/home/cvs/tralics/.tralics_rc.
      Configuration file identification: standard-ra $ Revision: 2.1 $
      Read tcf file for type: ../confdir/ra.tcf
      Error signaled at line 3:
      Missing \end{document}.
      Fatal_error for hello2003

**Example 10.** In version 2.10.7, the preprocessor is not called if the
year is 2007 or greater (this is indicated by the line *Simplified RA*).
Thus, we can copy hello2003 into hello2007, replacing 2003 by 2007 is
the document class. Compilation succeeds.

.. container:: tty_out

   ::

      grimm@macarthur$ tralics hello2007 -silent
      This is tralics 2.10.7, a LaTeX to XML translator, running on macarthur
      Copyright INRIA/MIAOU/APICS 2002-2007, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file hello2007.tex.
      Read tcf file ../confdir/ra2007.tcf
      Simplified RA
      Translation done 
      Output written on hello.xml (249 bytes).
      No error found.
      (For more information, see transcript file hello2007.log)
      rahandler.pl v2.15, (C) 2004 2006 INRIA, Jos\'e Grimm, Team Apics
      Postprocessor did nothing

**Example 11.** In version 2.13, both the preprocessor and
postprocessors are removed. Note the following differences between
examples 10 and 11. The line *Simplified RA* has been removed, as well
as the two lines dealing with the postprocessor. There is a file
ra2007.tcf but no file ra2003.tcf (because ra.tcf can be used instead).
Finally, the class file ra2007.clt was added to the system after example
10 was run.

.. container:: tty_out

   ::

      grimm@medee$ tralics hello2003
      This is tralics 2.13.0, a LaTeX to XML translator, running on medee
      Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Starting translation of file hello2003.tex.
      Read tcf file ../confdir/ra.tcf
      Document class: ra2003 2008/07/12 v1.0 Activity Report, Inria 2003
      Output written on hello.xml (249 bytes).
      No error found.
      (For more information, see transcript file hello2003.log)

In Raweb mode, the magic line ``\AtBeginDocument{\rawebstartdocument}``
is executed. The effect of this command is to translate the metadata
(name of the team, etc); it is defined by the class file. Our little
file compiles because there is no ``\begin{document}``. Translation is

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE raweb SYSTEM 'raweb7.dtd'>
      <!-- Translated from latex by tralics 2.10.7, date: 2007/11/03-->
      <raweb language='english' creator='Tralics version 2.10.7' year='2007'>
      <p>Hello, world!</p>
      </raweb>

*Tralics* version 2.15.4 however complains with *No module in section
composition* and four other errors; this is because the configuration
file imposes these sections to be non-empty (they are marked “special”
in the configuration file). On the other hand, there is no problem with
hello2007. Note: the year field in ``raweb`` element is the same as the
document class, and not the compilation date.

.. _main-doc:

2.3 Main document
-----------------

We make here the following assumption: the document is named
foo2003.tex, the document class is ra2003 (same year), and a
configuration file specifying type RA is seen. In this case, RA mode is
entered. Before 2007, there was a preprocessor, that did some checks,
rearranged the code, etc. In what follows, we explain what happens with
the example of 2003 and what is new in 2007 or 2008.

A first difference in 2007 is that you will not see *fatal error,
missing \\project* anymore (a normal error will be signaled instead when
translating the start of the document). A well-formed configuration file
is required. If you get a message like the following, please contact the
raweb team.

.. container:: tty_out

   ::

      Read tcf file ra2007.tcf
      The configuration file for the RA is ra2007.tcf or ra.tcf
      It must define a value for the parameter ur_vals
      See transcript file apics2007.log for details
      No xml file generated
      Fatal_error for apics2007

.. _conffile:

2.3.1 The Configuration File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The configuration file for the raweb is named ra.tcf (generic code) or
ra2008.tcf (this one is specific to the year 2008). Its content is
similar to what is described `here <doc-step.html#fourth-doc>`__. The
generic ra.tcf configuration file has a block of commands, the specific
files have none (all commands specific to the raweb are now in the
ra.plt file, included by the ``\documentclass`` command). The file
starts with

.. container:: ltx-source
   :name: confspec8

   ::

        DocType = raweb raweb7.dtd
        DocAttrib = year \specialyear
        DocAttrib = creator \tralics

This has as effect to define the preamble of the XML result file (see
translation of example 10 above). There are lines of the following form:

.. container:: ltx-source

   ::

        bibtex_fields = "x-editorial-board" 
        bibtex_fields = "+x-international-audience" 
        bibtex_fields = "+x-proceedings" 
        bibtex_fields = "+x-invited-conference"
        bibtex_fields = "+x-scientific-popularization" 
        bibtex_fields = "+x-pays"
        bibtex_fields = "+url-hal" 
        bibtex_extensions = "patent standard"
        bibtex_extensions = "+hdrthesis" 

The left hand side may be bibtex_extensions or bibtex_fields, the right
hand side is a space-separated list of items (you can insert line breaks
as in the case of fullsection_vals below). The list may start with a
plus sign; this means append to the previous value (default is overwrite
it). The meaning of these lines is the following: when *Tralics*
interprets the bibliography, it recognizes (for every entry type) the
fields x-editorial-board, x-international-audience, etc, including
url-hal; it also recognizes the entry types patent, standard and
hdrthesis. An element in the list may be preceded by a minus sign; in
which case, it will be ignored (ignoring entry types may provoke
undefined references). Assume for instance that the bibliography
contains

.. container:: ltx-source

   ::

      @phdthesis{TEILR09,
      title = {TEI},
      author = {Laurent Romary},
      school = {Humboldt-Universitat zu Berlin},
      url =  "http://www.hu-berlin.de/",
      year = 2015, 
      x-editorial-board = "yes",
      x-international-audience = "yes",
      x-proceedings = "no",
      x-invited-conference = "yes",
      x-scientific-popularization = "yes",
      x-pays = {xx,yy}  ,
      x-other = "ignored" ,
      url-hal ="http://hal.inria.fr/",
      }
      @patent{Algo09,
      author = {Gaëtan Bisson and Răzvan Bărbulescu},
      Title = {Algorithmes unicode pour les lettres Ààéçœ et Ñ},
      year = 2015,
      x-pays= {CH,HK}
      }

Assume moreover that the school field has to be omitted. The translation
is now

.. container:: xml_out

   ::

      <citation from='year' key='7' id='bid15' userid='cite:TEILR09' type='phdthesis'>
      <bauteurs><bpers prenom='L.' nom='Romary' prenomcomplet='Laurent'/></bauteurs>
      <btitle>TEI</btitle>
      <btype>Ph. D. Thesis</btype>
      <xref url='http://www.hu-berlin.de/'>http://<allowbreak/>www.<allowbreak/>hu-berlin.
        <allowbreak/>de/</xref><x-editorial-board>yes</x-editorial-board>
      <x-international-audience>yes</x-international-audience>
      <x-proceedings>no</x-proceedings>
      <x-invited-conference>yes</x-invited-conference>
      <x-scientific-popularization>yes</x-scientific-popularization>
      <x-pays>xx,yy</x-pays>
      <url-hal>http://hal.inria.fr/</url-hal>
      </citation>
      <citation from='year' key='15' id='bid2' userid='cite:Algo09' type='patent'>
      <bauteurs><bpers prenom='G.' nom='Bisson' prenomcomplet='Gaëtan'/>
        <bpers prenom='R.' nom='Bărbulescu' prenomcomplet='Răzvan'/></bauteurs>
      <btitle>Algorithmes unicode pour les lettres Ààéçœ et Ñ</btitle>
      <x-pays>CH,HK</x-pays>
      </citation>

Finally, the ra2008.tcf file contains also the following lines that
defines five lists.

.. container:: ltx-source
   :name: confspec

   ::

        section_vals = "composition*/presentation*/fondements/domaine/logiciels/"
        section_vals = "+resultats/contrats*/international*/diffusion*/"
        theme_vals = "Com Cog Num Sym Bio"
        ur_vals = "Rocquencourt//Sophia/Sophia Antipolis/Rennes//Lorraine//";
        ur_vals = "+RhoneAlpes/Rh\^one-Alpes/Futurs//"
        affiliation_vals ="Inria//Cnrs//University//ForeignUniversity//"
        affiliation_vals ="+Public//Other//"
        profession_vals = "Scientist//Assistant//Technical//PHD//"
        profession_vals = "+PostDoc//StudentIntern//Other//"

The quantity theme_vals is a list of words, separated by spaces,
converted to lower case. The quantity section_vals is a list of words,
separated by slash characters (with possibly a star at the end; these
are called “special”). For all other variables of the form
something_vals, it is a list of pairs, name and value, separated by
slashes. If the value is empty, the name is used instead. An initial
plus sign means continuation. The final slash can be omitted; an initial
slash is ignored.

Since 2007, the English title associated to a (French) section name has
been moved to the configuration file; it is given by fullsection_vals
(that should be used instead of section_vals; with this syntax, there
are no “special” sections anymore). As the following demonstrates, you
can put a string on more than one line, provided that the last character
on the line is a backslash.

.. container:: ltx-source
   :name: confspec1

   ::

        fullsection_vals = "/composition/Team/presentation/Overall Objectives/\
           fondements/Scientific Foundations/domaine/Application Domains/\
           logiciels/Software/resultats/New Results/\
           contrats/Contracts and Grants with Industry/\
           international/Other Grants and Activities/diffusion/Dissemination"
        xml_rasection = "section"

The translation of a section, for instance composition, is
``<composition>``. If the configuration file defines xml_rasection, this
will be the name of the element; the value extracted from
fullsection_vals becomes an attribute; example
``<section titre='Team'>``. Note that the start and end of a section is
determined by the modules it contains.

The transcript file contains some lines, showing how *Tralics*
interprets the configuration file. You might see the following lines.

.. container:: log_out

   ::

      theme_vals=Com Cog Num Sym Bio
      Section: composition -> Team
      Section: presentation -> Overall Objectives
      Section: fondements -> Scientific Foundations
      Section: domaine -> Application Domains
      Section: logiciels -> Software
      Section: resultats -> New Results
      Section: contrats -> Contracts and Grants with Industry
      Section: international -> Other Grants and Activities
      Section: diffusion -> Dissemination
      UR: Rocquencourt -> Rocquencourt
      UR: Sophia -> Sophia Antipolis
      UR: Rennes -> Rennes
      UR: Lorraine -> Lorraine
      +UR: RhoneAlpes -> Rh\^one-Alpes
      UR: Futurs -> Futurs

2.3.2 Interpreting the Configuration File
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*Tralics* has a command
```\tralics@get@config{X}{Y}`` <doc-t.html#cmd-tralicsfindconfig>`__
that returns a value Z, according to the following rules.

-  It is an error if X is ``foo``, and if the configuration file does
   not specifies foo_vals, or if Y is not in the list.
-  The value associated to Y in the list is returned.
-  If X is ``theme``, then Y is converted to lower case; this will be Z.
-  If you say
   ```\tralics@interpret@rc{foo,bar}`` <doc-t.html#cmd-tralicsinterpretrc>`__,
   this checks that all arguments are in the list ur_vals and marks them
   as possible locations. If a single item is given, it will be the
   default location.
-  If X is ``ur``, then Z is Y, if Y is a possible location. An initial
   plus sign is discarded: if Y is correct, it becomes the default
   location. If Y is empty, the default location will be used. An error
   is signaled otherwise.
-  If X is ``fullsection``, let k be the current section number, and K
   the position of Y in the section list (this could be section_vals or
   fullsection_vals). If Y is empty, an error is signaled if there is no
   current section, otherwise K is k. An error is signaled if Y is not
   in the list; an error is signaled if K is less than k ; an error also
   is signaled if K=k and Y is ``composition`` (details are given
   later). After that, K becomes the current section number. The value Z
   is empty if K=k, the value associated to Y otherwise.
-  If X is ``section``, then if the section number *k* (defined as
   above) is “special”, then Z is empty. Otherwise Z is Y (or
   ``default`` if Y is empty).

2.3.3 The main document
~~~~~~~~~~~~~~~~~~~~~~~

The main Raweb document has the following structure

.. container:: ltx-source

   ::

      \documentclass{ra2003}         %001
      <Header>
      \begin{document}               %016
      \maketitle                     %019
      <Modules>
      \loadbiblio                    %488
      \end{document}                 %489

Here, and in the examples that follow, blue numbers are the line numbers
of the corresponding lines in the example file. Things like <Modules>
are explained below. The ``\documentclass`` should be ``ra2006`` for the
year 2006.

**Note.** The two commands ``\maketitle`` and ``\loadbiblio`` are now
obsolete.

2.3.4 The header
~~~~~~~~~~~~~~~~

The header of the file is everything before ``\begin{document}``. It
should not produce any text. There is a free part and a required one,
they can be intermixed. The free part contains definitions and
``\usepackage`` commands. We recommend to always use ``\newcommand``
instead of ``\def`` (do not follow the example). You should *not* load
packages like ``french`` or ``babel``, because the current language of
the Raweb is English. For instance:

.. container:: ltx-source

   ::

      \usepackage{amsfonts}          %002
      \usepackage{amsmath}
      \def\CC{{\mathbb C}}           %009
      \newcommand{\etc}{etc}
      \def\corresp{manager}

The required part of the header contains lines like the following

.. container:: ltx-source

   ::

      \projet{MIAOU}{Miaou}{Mathématiques et 
           Informatique de l'Automatique et de l'Optimisation pour 
        l'Utilisateur}               %006
      \theme{4a}                     %004
      \isproject{YES} % \isproject{OUI} works also %005
      \UR{\URSophia}                 %007

See `Changes in 2007 <#fmodule7>`__ and `Changes in 2008 <#fmodule8>`__
for some changes introduced for the RA2007 or RA2008. There are no fatal
errors any more.

2.3.5 The Team Name and Properties
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``\isproject`` command take one argument that should be true if the
team is a Project-Team (according to local terminology). The current
version of the ra.plt package uses the xkeyval mechanism in order to
allow variants (équipes, actions transversales, whatever, see
instructions).

The ``\project`` command (it can be spelled as ``\projet``) takes three
arguments Uname, Sname and Lname. Here Sname is the short name of the
Team, and Lname its long name. The short name could be as complicated as
πr\ :sup:`2`. The long name is a bit more explicit and should be in
English.

The first argument Uname should match the file name, miaou2003.tex. The
code that follows uses ```\ra@jobname`` <doc-r.html#cmd-rajobname>`__,
this holds ``miaou``. It is like ``jobname``, except that the directory
part, and the year has been stripped; in particular, all characters have
category code 12 (except for space), so ``detokenize`` has to be used.
In earlier versions, Uname had to be all uppercase.

.. container:: ltx-source

   ::

        \edef\tmpB{\ra@jobname}
        \def\tmpA##1{\lowercase{\xdef\tmpA{\detokenize{##1}}}}
        \expandafter\tmpA\expandafter{\ra@proj@a}
        \ifx\tmpA\tmpB \else\PackageError{Raweb}{Invalid Team name \ra@proj@a}{}\fi

2.3.6 The Theme
~~~~~~~~~~~~~~~

The ``\theme`` command takes as argument a Theme (Research Teams are
grouped into domains, split into themes). Between 1996 and 2003,
research themes were identified by a digit, between 1 and 4, followed by
a letter; giving ``4a`` as argument has as effect to insert in the HTML
a link to the `theme
4 <http://www.inria.fr/recherche/equipes/listes/theme_4.en.html>`__
(this link is probably broken now). In 2004, the list of domains/themes
changed, and checking become more complex. You can see errors of the
form *Invalid theme 4a, Valid themes are com cog num sym bio*, where the
list comes from the configuration file (converted to lower case). Here
COM stands for “Communicating systems”, one of its themes being
“Distributed systems and software architecture”). In 2009, the list
changed again: *apics* (successor of *miaou*) is in domain
“Mathématiques appliquées, calcul et simulation” (in English “Applied
Mathematics, Computation and Simulation”), and theme “Modeling,
Optimization, and Control of Dynamic Systems”. This information, as well
as a link to the correct URL, is now inserted into the HTML or Pdf
version of the RA by an xsl stylesheet, according to the value of the
team name. As a consequence, the argument of the ``\theme`` command is
now ignored.

2.3.7 The Localisation
~~~~~~~~~~~~~~~~~~~~~~

The ``\UR`` command takes as argument a list of Research Units. The
default configuration file shown above gives the list of valid names. In
2007 they are ``Rocquencourt``, ``Sophia``, ``Rennes``, ``Lorraine``,
``RhoneAlpes`` and ``Futurs``. Since then ``Futurs`` has been replaced
by ``Bordeaux``, ``Lille`` and ``Saclay``. Before 2007, you had to say
``\UR{\URSophia}`` (for a team in a single UR) or
``\UR{\URFuturs\URSophia}`` for a team in two URs. This is still
accepted, but the recommended syntax is now ``\UR{Sophia}`` (for a team
in a single UR) or ``\UR{Futurs,Sophia}`` where the comma could be
replaced by a space. The list is interpreted by
```\tralics@interpret@rc`` <doc-t.html#cmd-tralicsinterpretrc>`__ that
can signal an error of the form:

.. container:: tty_out

   ::

      Error signaled at line 22 of file exemple2006.tex:
      Illegal localisation value: Saclay
      Use one or more of: Rocquencourt Sophia Rennes Lorraine RhoneAlpes Futurs.

2.3.8 The Topics
~~~~~~~~~~~~~~~~

The last part of the header is the Topic declaration. Example

.. container:: ltx-source

   ::

      \declaretopic{abc}{Topic abc}  %013
      \declaretopic{def}{Topic def}

Topics are defined if the ra.plt package is called with option
``topic``. Otherwise the two lines above are ignored as well as the
optional argument of the module environment. Topics have been declared
obsolete in 2007, so they exist only for compatibility reason, and no
error is signaled anymore. Each topic has a name and a title. The name
can be an optional argument of ``module`` (see the example, lines
`79 <raweb1.html#079>`__, `207 <raweb1.html#207>`__
`212 <raweb1.html#212>`__ and `322 <raweb1.html#322>`__.) Topics are
handled via a call to ``\tralics@get@config{section}{XX}``. See for
instance the reports of `isa
2003 <http://www.inria.fr/rapportsactivite/RA2003/isa2003/isa_tf.html>`__,
`isa
2004 <http://www.inria.fr/rapportsactivite/RA2004/isa2004/isa_tf.html>`__
or `odyssee
2005 <http://www.inria.fr/rapportsactivite/RA2005/odyssee/odyssee_tf.html>`__.

2.3.9 Preprocessor Limitations
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We list here some errors signaled by the preprocessor; this concerns
only the RA2006 and before.

Lines containing ``\theme`` etc, should not contain anything else after
the arguments. Replacing the % line 5 by a \_ will give this message

.. container:: tty_out

   ::

      Error signaled at line 5:
      Characters ignored at end of line
      current line is  _ \isproject{OUI} works also
      error is at position 1 at character _ (ascii 95).
      Fatal_error for miaou2003

You cannot say ``\def\foo{YES}\isproject\foo``, because the program
looks for explicit braces. Example:

.. container:: tty_out

   ::

      Error signaled at line 5:
      Characters ignored before open brace
      error signaled while scanning isproject
      current line is \foo % \isproject{OUI} works also
      error is at position 0 at character \ (ascii 92).
      Fatal_error for miaou2003

Before 2006, *Tralics* created file, named \_hyperref.cfg, containing
all the keywords in alphabetic order. You could see errors of the form

.. container:: tty_out

   ::

      Error at line 107
      Strange keywords 

      meromorphic approximation, frequency-domain identification,
      extremal problems {$(\max,+)$}

The ``participants`` environment should contain only ``\pers{}{}[]``
commands, separated by commas.

.. container:: tty_out

   ::

      Error signaled at line 253:
      Syntax error near `and \pers{xx}{yy}'

      \pers{Jean-Paul}{Marmorat} and \pers{xx}{yy}.
      Fatal_error for miaou2003

or

.. container:: tty_out

   ::

      Error signaled at line 253:
      Superfluous comma

      \pers{Jean-Paul}{Marmorat},, \pers{xx}{yy}
      \pers{Martine}{Olivi}[\corresp]
      .
      Fatal_error for miaou2003

The ``\pers`` command can be used in a ``participants`` or ``catperso``
environment. In one case, you must use commas as separators, in the
other case, you cannot. In the current version, the comma after
``\pers`` is optional, so that you won't see

.. container:: tty_out

   ::

      Error signaled at line 253:
      There must be a comma between two \pers

      \pers{Jean-Paul}{Marmorat}\pers{xx}{yy}
      \pers{Martine}{Olivi}[\corresp]
      .
      Fatal_error for miaou2003

nor

.. container:: tty_out

   ::

      Error signaled at line 44:
      Syntax error near `, \pers{x}{y}'
      {Ph. D. Students}
      \pers{David}{Avanessoff}[Fellow, INRIA], \pers{x}{y}.
      Fatal_error for miaou2003

The distiction between environments participants and participants has
been removed, there is no more

.. container:: tty_out

   ::

      For the environment ending near line 191
      please use `participant' or `participante' for the name,
      as there is only one \pers in the list.
      For the environment ending near line 253:
      please use `participants' or `participantes' for the name,
      as there are 2 \pers in the list.

The following error message was introduced in 2007, after removal of the
preprocessor. You will not see anymore in 2008.

.. container:: tty_out

   ::

      Error signaled at line 3095 of file apics2007.tex:
      Text found in a mode where no text is allowed.
      Only \pers commands should appear in a participants environment

2.4 Modules
-----------

A module is an environment, with some arguments: an optional topic
reference (see above), and three required arguments (section, name,
title). It is followed by an optional interface (formed of contributors,
keywords, glossary, and moreinfo) and text. Since a module is
essentially a ``\subsection``, the text should contain only
``\subsubsection`` and ``\paragraph`` commands. Modules cannot be
nested. The first module is special, we shall explain it later. The
second module looks like this

.. container:: ltx-source

   ::

      \begin{module}{presentation}{presentation}{} %047
      \begin{moreinfo}                             %048
      The project was terminated June the 30th, 2003. 
      A proposal for a new project named APICS has been submitted to the steering
      committee of Inria Sophia Antipolis.
      \end{moreinfo}
      ...
      \end{module}                                 %076

It should be a small description of the main research objectives of the
Team. You can use the ``moreinfo`` environment for emphasizes (in this
example, to indicate that this is the last RA for the miaou Team).
Historical note: Miaou has been effectively dissolved on 2003-31-12, was
replaced by Apics on 2004-01-01, which became a Project on 2005-01-01.

The standard configuration file `(see above) <#confspec>`__ defines nine
sections. The Raweb DTD associates to each section an English name,
shown in parenthesis here. Since 2007, the names have been moved to the
configuration file `(see above) <#confspec1>`__

#. composition (Team) example `line 22 <raweb1.html#022>`__
   This section gives the list of the members of the Team; see below.
#. presentation (Overall Objectives), example `line
   47 <raweb1.html#047>`__
   General presentation of the team, and overall objectives.
#. fondements (Scientific Foundations), example `line
   79 <raweb1.html#079>`__.
   This should explain the (scientific) tools and methodology used by
   the Team.
#. domaine (Application Domains), `example line
   183 <raweb1.html#183>`__, `line 187 <raweb1.html#187>`__, `line
   201 <raweb1.html#201>`__, `line 207 <raweb1.html#207>`__, `line
   212 <raweb1.html#212>`__, `line 217 <raweb1.html#217>`__.
   This is a very important section. You should use the following
   official keywords: telecommunications, multimedia, biology, health,
   process engineering, transportation systems, environment.
#. logiciels (Software), `example line 231 <raweb1.html#231>`__, `line
   241 <raweb1.html#241>`__, `line 249 <raweb1.html#249>`__, `line
   262 <raweb1.html#262>`__, `line 273 <raweb1.html#273>`__
   Each piece of software should be described here. You should use
   ``[correspondant]`` as moreinfo field for one of the team members.
#. resultats (New Results), example `line 286 <raweb1.html#286>`__,
   `line 314 <raweb1.html#314>`__, `line 318 <raweb1.html#318>`__, `line
   359 <raweb1.html#359>`__.
   This section explains the new results found by the Team.
#. contrats (Contracts and Grants with Industry), example `line
   322 <raweb1.html#322>`__, `line 338 <raweb1.html#338>`__, `line
   346 <raweb1.html#346>`__.
   You should explain here the work funded by Industry.
#. international (Other Grants and Activities), example `line
   366 <raweb1.html#366>`__, `line 380 <raweb1.html#380>`__, `line
   401 <raweb1.html#401>`__, `line 407 <raweb1.html#407>`__.
   In this section, you describe contacts with other Teams (in France,
   Europe, and rest of the Word).
#. diffusion (Dissemination), example `line 433 <raweb1.html#433>`__,
   `line 468 <raweb1.html#468>`__, `line 474 <raweb1.html#474>`__.
   Here you describe Teaching activities, etc.

The first argument of a module, the section, is handled by calling
``\tralics@get@config{fullsection}{#1}`` whose behavior is described
`above <#fullsection>`__. If this defines a new section, the previous
section (if any) is popped via
```\tralics@pop@section`` <doc-t.html#cmd-tralicspopsection>`__ and a
new section is created via
```\tralics@push@section{#1}`` <doc-t.html#cmd-tralicspushsection>`__.

The section may be empty, case where no new section is created. If there
is no current section, you will see an error of the form *No default
section*. Normally, “composition” is the first module (see below) and
the first section. So, if the first module has an empty section and is
followed by the composition, you will see a second error *Composition
must be before first module* and a third *Only one module accepted in
composition*. If a module with empty section follows the composition,
you will only see the last of these three message.

If the section is not in the list of valid sections defined by the
configuration file, you will get an error of the form *Invalid section
Domaine* followed by *Valid sections are composition presentation
fondements domaine logiciels resultats contrats international diffusion*
(it could be followed by the errors listed above).

**Note** You can notice that the module starting at `line
359 <raweb1.html#359>`__ is in the section “resultats”, but it comes
after the modules of the section “contrats”. This is allowed by the
preprocessor : modules are re-ordered. This feature is removed in 2007.
You will see the error *Bad section fondements after diffusion* followed
by *Order of sections is composition presentation fondements domaine
logiciels resultats contrats partenariat diffusion* (it could be
followed by a second error).

The second item in a module, name, is a symbolic name, that uniquely
defines the module; it must be unique, or else an error of the form
*Class Raweb Error: Duplicate module: dif-anim* is signaled. Since a
label (for instance ``mod:presentation``, is created and anchored to the
module, there will be another error *Label 'mod:dif-anim' multiply
defined (first use line 667 file exemple2006.tex)*. Trying to use
special commands (for instance math formulas) can produce errors of the
form *unexpected element formula*.

Finally a module has a title. In the example, three modules have an
empty title: `line 22 <raweb1.html#022>`__, `line
47 <raweb1.html#047>`__, and `line 183 <raweb1.html#183>`__. This is
somehow weird, so, unless ra.plt is loaded with option ``emptymodule``,
an empty title is replaced by ``(Sans Titre)``. Before 2008, a
complicated algorithm was used by the post-processor to deal with the
case of empty modules titles. In the current version, an error is
signaled and a default title is used.

.. _modules-i:

2.5 Module Interface
--------------------

A typical module should contain an interface and a body. The interface
is formed of contributors, keywords, glossary, and moreinfo. The
original idea was that body is defined by a ``body`` environment. This
has been simplified: it can be any text, including subsections with
their own interface. In a section like “Overall objectives”, you do not
cite contributors (because you talk about the whole Team), and in a
section like “Dissemination”, you do not cite them either (in fact,
names appear in the body of the module, for instance `line
436 <raweb1.html#436>`__ and `line 470 <raweb1.html#470>`__). Example

.. container:: ltx-source

   ::

      \begin{module}{}{calcul formel}{Software aspects of computer algebra}
        \begin{participants}
          \pers{Jean}[de]{La Fontaine}[1621-1695]
          \pers{Cecil Blount}{De Mille}
        \end{participants}
        \begin{motscle}
          mot1, mot2, mot3
        \end{motscle}
        \begin{glossaire}
          \glo{trick}{the thing which makes go the thingummy}
          \glo{BabelFish}{the translator of the previous sentence}
        \end{glossaire}
        \begin{moreinfo}
          This work was sponsored by all the sponsors
        \end{moreinfo}
        This is the body of the module.
      \end{module}

The moreinfo is technically a ``moreinfo`` environment. It is formed of
a short paragraph (of the form: “this work done in collaboration with
X”).

The keywords are indicated by the ``motscle`` environment; examples are
given at `line 104 <raweb1.html#104>`__, `line 136 <raweb1.html#136>`__,
`line 193 <raweb1.html#193>`__, `line 202 <raweb1.html#202>`__, and
`line 225 <raweb1.html#225>`__. The environment contains a comma
separated list of keywords. The last example shows how to put commas
into a keyword. Keywords have become obsolete. Later on, the powers that
be have reintroduced a global list of keywords, with a different syntax.

A glossary is a ``glossaire`` environment in which you can put ``\glo``
commands; they take two arguments: a word and its description. There is
a dummy example `line 475 <raweb1.html#104>`__. In the current version,
there is no possibility to make a reference to the items of the
glossary.

A contributors list is given by a ``participants`` environment. Since
2008, this produces a ``<participants>`` element. For compatibility
reasons with the initial (French) version, you can use ``participant``,
``participante`` or ``participantes``. If the ra package is called with
option ``participant``, then the name of the element is the name of the
environment. Inside the environment, empty lines are ignored and
``\pers`` is defined to be ``\persA``. Examples are `line
094 <raweb1.html#094>`__, `line 126 <raweb1.html#126>`__, `line
189 <raweb1.html#189>`__, `line 219 <raweb1.html#219>`__, `line
233 <raweb1.html#233>`__, `line 243 <raweb1.html#243>`__, `line
250 <raweb1.html#250>`__, `line 265 <raweb1.html#265>`__, and `line
274 <raweb1.html#274>`__.

.. _f-module:

2.6 First Module
----------------

.. _fmodule3:

2.6.1 Original syntax
~~~~~~~~~~~~~~~~~~~~~

The first section contains a single module, for instance, the one
starting at `line 22 <raweb1.html#022>`__. Its name is useless. It
should contain only ``catperso`` environments (but this is not checked).
Examples are `line 24 <raweb1.html#024>`__, `line
28 <raweb1.html#028>`__, `line 32 <raweb1.html#032>`__, `line
36 <raweb1.html#036>`__, and `line 40 <raweb1.html#040>`__. Each such
environment has a title, and contains a list of names, like the
``participants`` environment, without commas.

The syntax of the ``\pers`` command is the following: first name,
particle, last name and moreinfo. An example is the following:
\\pers{Jean}[de]{La Fontaine}[1621-1695]. Arguments in braces an
mandatory, arguments in brackets are optional. We recommend the syntax
``\pers{Jean}{de la   Fontaine}``, since *Tralics* does nothing with the
particle apart prefixing it to the last name. In the first module, the
moreinfo may indicate that some contributor has a grant from Inria, or
whatever, and for non permanent people, their arrival, and departure
date (as in `line 43 <raweb1.html#043>`__); this could also indicate a
collaboration of another organism, for instance like in `line
97 <raweb1.html#097>`__;

.. _fmodule6:

2.6.2 The 2006 semantics
~~~~~~~~~~~~~~~~~~~~~~~~

In 2006, the syntax of ``\pers`` changed. The ``\persA`` command is the
one described above, the syntax of ``\persB`` is first name, particle,
last name (same meaning as above), CR, profession, affiliation,
moreinfo, HDR. The last two arguments are optional; a non-empty value of
HDR means that the person has a “Thèse d'état” or a “Habilitation à
diriger les Recherches”. The CR argument was introduced in 2007, and is
described in section 2.6.3.

The profession and affiliation arguments must be used after the last
name. The value is restricted to what is defined by the `configuration
file <#confspec>`__. Example

.. container:: ltx-source

   ::

      \begin{catperso}{Ph. D. Students}
      \pers{Jean}{Dupond}{Scientist}{Inria}
      \pers{Jean}{Dupont}{Assistant}{Cnrs}[][yes]
      \pers{Jean}{Durand}{Technical}{University}[][]
      \pers{Jean}{Dumat}{StudentIntern}{Other}[bla bla ][no]
      \pers{Jean}{Durant}{PHD}{ForeignUniversity}[with a T]
      \pers{Jean}{Dumas}{PostDoc}{Public}[with a S][scheduled november 2007]
      \pers{Jean}{Dumont}{ Other }{Other}[bla bla ][no]
      \end{catperso}

.. _fmodule7:

2.6.3 The 2007 changes
~~~~~~~~~~~~~~~~~~~~~~

Topics have become obsolescent. This means that *Tralics* recognises the
syntax, but ignores all values. A new feature has been added: the start
of the document optionally contains a line of the form
``\TeamHasHdr{yes}``. A true value means: at least one staff member has
a Hdr (see discussion about the ``\pers`` command). This feature has
been removed in 2008.

The following three lines are equivalent; they say that the team is
located in both Rocquencourt and Sophia. Recall that “UR” stands for
Unité de Recherche (Research Unit), and this is nowadays called Research
Center.

.. container:: ltx-source

   ::

      \UR{\URRocquencourt\URSophia }
      \UR{Rocquencourt,Sophia }
      \ResearchCentreList{Rocquencourt,Sophia}

Since 2007 you must indicate, for every staff member, its localisation
(Rocquencourt or Sophia, in the example above). This is done by adding
an optional argument to the ``\pers`` command. Of course, the optional
argument is required only if the RC list has more than one element.
Example

.. container:: ltx-source

   ::

      \begin{catperso}{Team Leader}
      \pers{Laurent}{Baratchart}[Sophia]{Chercheur}{INRIA}[DR Inria][Habilite]
      \end{catperso}

In the example that follows, the equals sign says that Sophia becomes
the default value (so that there is no need to repeat it).

.. container:: ltx-source

   ::

      \begin{catperso}{Reserch scientists (external)}
        \pers{Jean}{Louchet}[=Sophia]{Chercheur}{INRIA}[CR]
        \pers{Jean-Marie}{Rocchisani}{Chercheur}{UnivFr}[Université Paris XIII]
      \end{catperso}

.. _fmodule8:

2.6.4 The 2008 changes
~~~~~~~~~~~~~~~~~~~~~~

Before 2008, the first section consisted in a module consisting of a
sequence of ``catperso`` environments. Such an environment takes one
argument, a category of people. As the examples show, the values are
very diverse (compare ``Head of project team`` with ``Team leader``), so
there was an attempt to limit the values to those defined in the
configuration file, before the replacement by a unique environment
``composition``, outside any module.

Remember that the ``\pers`` command comes in two versions ``\persA``
(simple) and ``\persB`` (complicated). These are commands are defined in
the ra.plt package file, included from the class file (for instance
ra2009.clt) with some options. You should use the ``\pers`` command only
in two cases: inside a ``participant`` environment (and then it is
``\persA``), or in the first module. If this one is defined by
``composition`` (2008 and later), then ``\pers`` is made undefined
outside ``participant`` and ``composition``. It is ``\persA`` if the
package is loaded with option ``old`` (2005 and before) and ``\persB``
otherwise (2006 and later). Example for 2008:

.. container:: ltx-source

   ::

      \begin{composition}
        \pers{Christine}{Eisenbeis}{Chercheur}{INRIA}[Team Leader (CR) Inria][Habilite]
        \pers{Nathalie}{Gaudechoux}{Assistant}{INRIA}[Secretary (SAR) Inria]
        \pers{François}{Thomasset}{Chercheur}{INRIA}[Research Director (DR) Inria]
        \pers{Jean}{Louchet}{Chercheur}{AutreEtablissementPublic}[Ing. en chef Armement (CR)]
        \pers{Jean-Marie}{Rocchisani}{Chercheur}{UnivFr}[Université Paris XIII]
        \pers{Moussa}{Lo}{Visiteur}{UnivEtrangere}[AUF Grant/ Gaston Berger University, 
          Saint-Louis, Senegal, from March 1st till August 31 2008]
      \end{composition}

2.6.5 The 2013 changes
~~~~~~~~~~~~~~~~~~~~~~

The affiliation part of the ``\pers`` command has been removed (and the
command is defined as ``\persC``). Example (with a CR part).

.. container:: ltx-source

   ::

      \begin{composition}
        \pers{Tim}{Berners-Lee}[=Grenoble]{Chercheur}[Team leader, Senior Researcher Inria][Habilite]
        \pers{Ada}{Lovelace}{Assistant}[shared with another team]
        \pers{Richard}{Stallman}[Saclay]{Chercheur}[Senior Researcher Inria]
        \pers{Linus}{Torvalds}{Chercheur}[Ing. en chef Armement]
        \pers{Marc}{Andreessen}{Enseignant}[Professor, Université Paris 13]
        \pers{Donald E.}{Knuth}{Visiteur}[AUF Grant/ Gaston Berger University, Saint-Louis, Senegal, from March 1st till August 31]
        \pers{François}{Gernelle}{Technique}[IGHCA, Unicode team]
      \end{composition}

2.7 References
--------------

Our example contains `line 92 <raweb1.html#092>`__ a ``\label`` that is
referenced at `line 329 <raweb1.html#329>`__. This label is associated
to a section (starting at line `line 90 <raweb1.html#090>`__). You can
also associate a label with an equation (`line 141 <raweb1.html#141>`__)
or a figure (`line 304 <raweb1.html#304>`__) (note that the label has
nothing to do with the ``\caption`` that follows; references to the
figure will be correct after translation, but incorrect if LaTeX is
called directly).

You can reference a module via the ``\moduleref[y]{p}{s}{name}``. Since
2006, the optional argument has been removed, and the command is
equivalent to ``\ref{mod:name}`` (if the last argument is non-empty, to
``\ref{section:s}`` otherwise). The original idea was to refer to the
module “name” in section “s” of the activity report of team “p” for year
“y” (this worked in principle for the HTML version of the RA as the name
of the HTML page depended only on this quantities; it never worked in
the PDF version). So the year has to be the current year, the team has
to be the current team and the section could be deduced from the module
name, whence the current implementation. There is an example `line
330 <raweb1.html#330>`__ and some more at `line 410 <raweb1.html#410>`__
(if the module name is empty, the link points to the section).

You can use the ``\href`` command for external references, like in `line
258 <raweb1.html#258>`__, `line 280 <raweb1.html#280>`__, `line
384 <raweb1.html#384>`__, `line 394 <raweb1.html#394>`__, `line
398 <raweb1.html#398>`__, `line 481 <raweb1.html#481>`__.

In the bibliography, you can add a ``url`` field, like `this
one <raweb1.html#bib-Avan-Bar-Pom03rr>`__. We provide the ``\rrrt``
command: its argument is the number of a Inria Research report. Example
`here <raweb1.html#bib-Bara-Chy-Pom03>`__.

It is also possible to give the ``doi``: in `this
example <raweb1.html#bib-lswprep>`__, indicating the ``doi=`` is the
same as the ``xxurl``, without the xx.

.. _rabib:

2.8 Bibliography
----------------

.. _rabib1:

2.8.1 Bibliography: the raweb case
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are three bibliography files for the raweb: ``miaou2003.bib``,
``miaou_foot2003.bib``, and ``miaou_refer2003.bib``.

A reference from the main file miaou2003.bib appears in the bibliography
if it is cited via ``\cite`` (there is an example `line
122 <raweb1.html#122>`__) or via ``\nocite`` (example `line
246 <raweb1.html#246>`__) or if the ``\nocite{*}`` has been issued
(example `line 20 <raweb1.html#020>`__). The example file contains an
instance of each of the standard bibtex entries, with all the required
and optional fields, named std1 to std13, with two famous authors. The
main bibliography should contain only references of the Team in the
current year. Since 2013, this file is automatically generated.

Entries from miaou_refer2003.bib can be referred to via ``\cite`` or
``\refercite``, there is an implicit ``\nocite{*}``, so that the whole
database is included. The miaou_refer2003.bib should contain the ten
most important published papers (or books) of the team.

Reference to other bibliography entries (not from the team, not from the
current year, or not one of the most important publications of the team)
can be made, provided that the entry is in miaou_foot2003.bib, and you
use the command ``\footcite`` (there is an example `line
122 <raweb1.html#122>`__).

.. _rabib2:

2.8.2 Bibliography: the general case
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Normally, you say ``\cite{foo,bar}``, and this makes two references to
some bibliographic entries, defined by ``\bibitem{foo}`` and
``\bibitem{bar}``. Very often, an external program, bibtex, is used to
compute the list of these ``\bibitem`` and their content, using the
argument of the ``\bibliography`` command as the list of the database
files to look at. The output of bibtex is put in the file miaou2003.bbl,
it contains essentially one ``thebibliography`` environment (more than
one in the raweb case), and lots of ``\bibitem{foo}`` commands. The
content of these is defined by the current bibliography style, the value
of the argument of the ``\bibliographystyle`` command.

The default behavior of *Tralics* is not to call bibtex, but to compute
and translate an intermediate file, say miaou2003_.bbl (note the
underscore), that is equivalent to the miaou2003.bbl file produced by
bibtex. It contains no ``thebibliography`` environment, but a lot of
``\citation`` commands (that are in reality environments, since they are
explicitly terminated by an ``\endcitation`` command). This file is
translated (in case of errors while translating the bibliography, line
numbers refer to this file). An example is given under
```\citation`` <doc-c.html#cmd-citation>`__.

The result is independent of any bibliographystyle. It contains commands
of the form ``\cititem{btitle}{title of the book}``. If the command
``\cititem-btitle`` is defined, it will be called with
``title of the book`` as argument. It can also contain commands as
``\bauteurs`` and ``\beditors``, that take as argument a list of
``\bpers`` commands. All these commands can be redefined, they are
independent of commands used elsewhere.

The bibliography is translated at the end of the run, when the list of
all citations is found. The result is a ``<biblio>`` element (the name
can be changed, via the configuration file, using something like
xml_biblio = "Biblio"). This element is inserted in the XML tree at the
end. If you use the ``\bibliography`` command, the first occurrence of
it defines the location (bibtex complains if more than one
``\bibliography`` is issued, but *Tralics* is happy with more than one).
You can also use ``\insertbibliohere``, this command has precedence over
everything else.

Assume that the configuration file contains a line with bibtex_fields =
"firstpage lastpage". The effect of this line is to tell *Tralics* to
read also the fields ``firstpage`` and ``lastpage`` and to include them
in the bbl file as ``\cititem{firstpage}{50}``. If the configuration
file contains also bibtex_extension = "foo bar" then entry types ``foo``
and ``bar`` are recognised, together with the standard types.

.. _rabib3:

2.8.3 Bibliography: extended syntax
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The mechanism explained in §2.8.1 above works by extending the syntax of
the ``\cite`` command. It takes three arguments type (optional), prenote
(optional) and keys (a comma separated list of keys) for instance
``\cite[foo][bar]{gee1, gee2}``. If only one optional argument is given,
it is prenote. The translation is a sequence of
``<cit><ref>   </ref></cit>`` elements, one for each key.

The three commands ``\footcite``, ``\yearcite`` and ``\refercite`` do
not read a type but assume it is, respectively, ``foot``, ``year`` and
``refer``. In the case of ``\footcite``, the first ``<cite>`` is
preceded by the value of ``\footcitepre`` (that does nothing by
default). See also `\\yearcite <doc-y.html#cmd-yearcite>`__,
`\\refercite <doc-r.html#cmd-refercite>`__ and
`\\footcite <doc-f.html#cmd-footcite>`__. The commands ``\yearcite`` and
``\refercite`` were introduced in version 2.8 of *Tralics*, while
``\footcite`` is inspired from the footcite package by Eric Domenjoud.
The command ``\nocite`` is similar, but it takes no prenote. Its
translation is empty. A special key is ``*``; in this case a global
variable is set (meaning that the whole bibliography data base should be
included). The command ``\natcite`` command takes three optional
arguments nat-type, prenote, postnote. Translation is one ``<Cit>``
element, containing one ``<ref>`` per key.

If type is ``year``, it will be replaced by an empty value; if type is
``refer`` and DRY (see below) is false, it will be replaced as well. An
auxiliary command is then applied to each key. For instance
``\cite[year][aa]{r1,r2}`` is converted into
``\cite@one {}{r1}{aa}\citepunct \cite@one {}{r2}{}`` (the first empty
argument is the converted type type), and ``\footcite[bar]{gee1, gee2}``
is converted into
``\footcitepre \cite@one {foot}{gee1}{bar}\footcitesep \cite@one   {foot}{gee2}{}``
(the first command, by default does nothing). Finally,
``\natcite[aa][bb][cc]{r1,r2}`` is converted to
``\NAT@open bb \cite@simple {r1}\NAT@sep \cite@simple {r2}\NAT@cmt   cc\NAT@close \endnatcite``;
this uses some commands defined by the natbib.plt package. Translation
is
``<Cit type='aa'>(bb <ref   target='bid0'/>;<ref target='bid1'/>,   cc)</Cit>``.
The purpose of the ``\endnat`` token is to finish the ``<Cit>`` element.
Translation of ``\cite{xx}`` is ``<cit><ref target='bid8'/></cit>``.

Let's explain the commands ``\cite@simp{key}`` and
``\cite@one{type}{val}{key}``. They first generate a cite key, formed of
the type, the string “cite:” and the key, for instance
``footcite:gee2``. A unique identifier (for instance ``bid1``) is
associated to the key. If the identifier does not exists, the reference
is marked to be solved; it is an error if ``\cite`` is called after
*Tralics* has started translating the bibliography (for instance, you
cannot use the command in a bibliography file). In the case of
``\nocite`` no further action is taken; otherwise a reference is
created, of the form ``<ref target='bid1'/>``. In the case of
``\cite@simp``, this element is inserted in the XML tree, and no further
action is taken. Otherwise, the translation of the second argument
becomes the content of the element. A new paragraph is started if
necessary, and a ``<cit>`` element is inserted in the current XMl tree;
its value is the ``<ref>`` element. It may have three attribute:
``rend='type'`` (where the value of the attribute is the type),
``type='w'`` (in case ``\def\cite@@type{w}`` has been executed), and
``prenote='xx'`` (in case ``\def\cite@prenote{xx}`` has been executed).

One task of *Tralics* is to solve the citations, this means, for every
cite key, for instance ``footcite:gee2`` with unique id ``bid1``, there
is a unique element whose id attribute is bid1. There are different
ways, see below. After the whole document is translated, *Tralics*
checks if there are unsolved citations. If so, it translates the
bibliography. After that, for unsolved citations, an error is signaled
of the form *Error signaled while handling entry footcite:gee2 undefined
reference*. The ``\bibliography`` command specifies the list of data
files to consider. In the case of ``torture+foot`` (assuming there is no
file torture+foo.bib) the file torture.bib is read, and
``footcite:gee2`` is solved by a reference whose key is gee2. In the
case of the raweb, for instance for miaou2006, there are three
bibliography files miaou2006.bib (should contain all references of the
year 2006 of the team), miaou_refer2006.bib (contains major publications
of the team in recent years) and miaou_foot2006.bib (for everything
else). These file solve entries of type year, refer and foot
respectively. It may happen that *Tralics* moves an entry from one
category to the other, withe a message of the type *entry moved from
refer to year* for different reasons.

There is a special hack, DRY (for distinguish-refer-year) introduced in
version 2.8. If *Tralics* is called with the switch
-distinguish_refer_in_rabib=false, then DRY mode is set to false (see
details in `Options of the program <options.html#drir>`__). It is true
by default. If DRY is true, you can put the same entry (with the same
key, and the same value) in both files miaou2006.bib and
miaou_refer2006.bib. This is however not recommended.

.. _rabib4:

2.8.4 Bibliography: more hacks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are different ways to put solve an entry. For instance
``\xbox {foo} {\XMLsolvecite [] [foot] {toto} {text}}`` (see
`\\XMLsolvecite <doc-x.html#cmd-XMLsolvecite>`__) solves the entry
associated to ``\footcite{toto}``. You can also use
``\bibitem[xx]{toto]`` (see `\\bibitem <doc-b.html#cmd-bibitem>`__),
that solves the entry with a ``<citation>`` element. You can ask
*Tralics* to generate the bibliography, or use an external program that
get all data from one or more bibliography databases. Note: you must use
the -shell-escape argument of *Tralics* in order to execute an external
program.

*Tralics* interprets the argument of the ``\bibliographystyle`` command
as follows. If it is ``bibtex:foo``, this means that ``bibtex testb`` is
executed; if it is ``program:cat``, this means that the program
``cat testb.aux`` will be executed. In all other cases, it defines the
style to be used (default is plain). This command can be used more then
once: first to define the style, second to define the command; note that
``bibtex:foo`` sets the style to foo, and ``bibtex:`` uses the defined
style. We assume here that testb is the name of the document to
translate. Example

.. container:: ltx-source

   ::

       
      \documentclass{article}
      \begin{document}
      \AtEndDocument{\bibitem{unused}Hey}
      \bibliography{torture}
      \bibliographystyle{bibtex:}
      \cite{poussin,cruz,many,strange,unused}
      \end{document}

If an external program is given, a minimal auxiliary file is created,
named, testb.aux, containing

.. container:: ltx-source

   ::

       
      \citation{poussin}
      \citation{cruz}
      \citation{many}
      \citation{strange}
      \bibstyle{plain}
      \bibdata{torture}

It is assumed that the external program generates a file named
testb.bbl; this is obviously not the case for ``cat testb.aux``. This
file is loaded (if possible) and executed. It is assumed that
translating this file will solve all entries, but no further checks are
done.

If no external program is given, *Tralics* will read the database files,
and construct a bbl file (named testb_.bbl) and executes it. Given the
following command
``\newcommand \usebib[2] {\bibliography{#1#2, #1_foot#2+foot,   #1_refer#2+refer}}``,
you can say ``\usebib{miaou}{2003}``, if you want to mimic what
*Tralics* does for the RA2003. (note: only suffixes year, foot and refer
are considered; this could be generalized).

.. _rabib5:

2.8.5 Bibliography: reducing entropy
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A new feature was introduced in 2006, it allows the use of a single data
base instead of three different ones. This means that you can provide
miaou_all2006.bib instead of three files. This feature is incompatible
with the idea that one of the three files is automatically generated. An
entry in the file has type year, foot or refer, if it is cited by
``\yearcite``, ``\footcite`` or ``\refercite`` or ``\nocite`` with the
corresponding flag.

When *Tralics* reads the file miaou_all2006.bib, and sees an entry, say
\`Knuth', it considers it as solving ``\cite{Knuth}``,
``\refercite{Knuth}`` or ``\footcite{Knuth}``. In general, a single
alternative is chosen, but as explained above, it is allowed to use both
``\cite{Knuth}`` and ``\refercite{Knuth}``.

What happens if you say ``\nocite{*}``? One possibility would be to
insert the whole data base thrice (as foot, refer and year), a second
possibility would be to insert it once, but not as year, neither as
refer. Hence, the only logical solution: the behavior is the same as
before version 2.9.3. If you say ``\nocite{*}``, all entries from
miaou2006.bib are included, by defaut all entries from
miaou_refer2006.bib are included. As a consequence, an entry from
miaou_foot2006.bib or miaou_all2006.bib is included only if explicitly
cited. This can be done via ``\nocite[foot]{Knuth}`` or
``\nocite[refer]{Knuth}`` if the reference should have type \`foot' or
\`refer'.

Important note. Assume that you say ``\cite{Knuth}`` and
``\footcite{knuth}``, but no other variants like KNUTH of KNuth, etc. It
is possible to have two files miaou2006.bib and miaou_foot2006.bib with
the entry \`Knuth'. When the second data base is read, then \`Knuth'
will solve \`knuth' (assuming that the data base does not contain any
variants like KNUTH, etc.) However, if you merge these two files in a
single one, you will be in trouble: \`Knuth' will solve
``\cite{Knuth}``, and no tentative is made for variants; in this case
the data base must contain \`knuth' that solves ``\footcite{knuth}``.

.. _run0:

3. Running the programs
=======================

Options to the *Tralics* program can be found on a `separate
page <options.html>`__. Some of them have been removed in 2008, there
description can be found `here <raweb4.html>`__, where we explain the
different strategies that could be used to produce the Raweb before
2008.

.. _script0:

4. The auxiliary perl script
============================

Before 2008, an auxiliary Perl script described `here <raweb5.html>`__
was used for the Raweb.

.. _fotex:

5. The fotex.cfg file
=====================

.. _racfg:

6. The raweb-cfg.sty file
=========================

The pdf files of the activity report are produced via a Makefile as
follows. The TeX source is converted by *Tralics* into some XML file; an
XML processor converts this into another XML file, conforming to the new
DTD (whatever this means). Another XML processor converts the result
into XSL/FO format, which is interpreted by TeX via the tools of S.
Rahtz and D. Carlisle. Some patches to these files are required, they
are described `here <raweb6.html>`__.
