Getting started with Tralics
============================

| `The first document <#first-doc>`__
| `Our second document <#second-doc>`__
| `Our third document <#third-doc>`__
| `Our fourth document <#fourth-doc>`__
| `Statistics <#stats>`__
| `A complete example <#last-doc>`__

This file is a primer for Tralics, a LaTeX to XML translator. We explain
how to use it. First, we consider a `trivial <#first-doc>`__ example,
that contains only text, then the same one, `with latex
markup <#second-doc>`__. After that, we give a small example `with a
configuration file <#third-doc>`__ and a last one `with another
configuration file <#fourth-doc>`__. We also explain `near the
end <#stats>`__ the meaning of all those numbers printed by *Tralics* at
the end of the job. Finally we give a complete non trivial document.

Introduction
------------

This document was written in 2006 for Tralics version 2.9; since then
the syntax changed a bit. The following demonstrates some features

.. container:: tty_out

   ::

      grimm@macarthur$ tralics -inputfile=../hello5.xml -o Hello
      This is tralics 2.11.5, a LaTeX to XML translator, running on macarthur
      Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm
      Licensed under the CeCILL Free Software Licensing Agreement
      Fatal error: Cannot open input file ../hello5.tex

If you say tralics hello, the effect is to convert hello.tex into
hello.xml. The program takes some arguments; in the example above, there
are three arguments separated by spaces. An argument that start with a
dash is an option, these are described in `options of the tralics
command <options.html>`__. Some options (like -verbose) take no
argument; some take two arguments, and some take a single argument. The
following forms are equivalent: -o Hello (a space), -o=Hello (no space,
but an equals sign), or -o = Hello (an equals sign, two spaces). You can
also say "-o= Hello" (an equal sign optional spaces), provided that the
shell removes the doubles quotes and presents this to *Tralics* as a
single string.

All options start with a dash, and all arguments that are not options
are input files; unless the -interactivemath option is used, exactly one
input file must be given. If you want to convert a file whose name
starts with a dash, you must use the -inputfile option. The extension
.tex is added to the input file name when missing. You can change the
name of the output file into result.xml via the -o result option. Note
also that if the input file is foo.xml *Tralics* will not attempt to
convert foo.xml.tex into foo.xml.xml; it makes the assumption that you
want foo.xml from foo.tex. This explains the previous error message.

The first document
------------------

The simplest document you can give to *Tralics* has the following form.

.. container:: ltx-source

   ::

      Hello, world!

Assume that the file is called hello.tex and you call *Tralics* on it.
You can say tralics hello -noconfig or tralics -noconfig hello.tex to
compile it, the result will be the same. If you just say tralics hello,
the default configuration file is loaded, but the resulting XML file is
the same.

This is what you will see on the terminal.

.. container:: tty_out

   ::

         maclaurin@grimm$ tralics hello
      1  This is tralics 2.15.4, a LaTeX to XML translator, running on maclaurin
      2  Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      3  Licensed under the CeCILL Free Software Licensing Agreement
      4  Starting translation of file hello.tex.
      5  Configuration file identification: standard $ Revision: 2.24 $
      6  Read configuration file ../confdir/.tralics_rc.
      7  Bib stats: seen 0 entries.
      9  Math stats: formulas 0, kernels 0, trivial 0, \mbox 0, large 0, small 0.
      10 Buffer realloc 1, string 1336, size 11986, merge 0
      11 Macros created 177, deleted 7; hash size 2398; foonotes 0.
      12 Save stack +14 -14.
      13 Attribute list search 1517(1509) found 1077 in 1122 elements (1120 at boot).
      14 Number of ref 0, of used labels 0, of defined labels 0, of ext. ref. 0.
      17 Output written on hello.xml (187 bytes).
      18 No error found.
      19 (For more information, see transcript file hello.log)

Let's try to understand all these lines. Lines 1 to 3 identify the
software and its release number (here 2.15.4) and the name of the
computer. You may have a different version, that produces a different
output. In particular, the third line of the XML result is a comment
line that holds the version number, and since version 2.9, the
compilation date. Thus, the size of XML output can depend on the
version.

Line 4 indicates what *Tralics* will do (before version 2.13, there was
an option telling *Tralics* to check the file for the Raweb, or call
LaTeX). Default is to translate the source file in to XML. Lines 5 and 6
say that some configuration file has been read. If the option -noconfig
is given, you see *No configuration file* instead. For details about
configuration files, see `Configuration files of
Tralics <titlepage.html>`__ .

Everything starting at line 7 is printed after translation is complete.
Lines 7 to 16 will be explained in detail later. In earlier versions,
you could see on line 8: Seen 0 bibliographic entries (this follows
logically from line7); on line 15 Modules with 0, without 0, sections
with 0, without 0 (Modules were once used for the raweb), and on line 16
There was no image (however, if images were used, then *Tralics* says
so). Line 11 says that 2398 multiletter control sequences were in use,
and 177 macros were created; line 13 says that 1120 elements are
constructed at bootstrap time (for instance, ``\empty`` and ``\alpha``
both occupy a slot in the hash table, one is a macro, and the other is
associated to an XML element).

The last three lines indicate that *Tralics* was happy (no errors found)
and generated an XML file. If you call *Tralics* with option -silent,
you will see only the first four lines, and the last three ones. We can
look at the result:

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE unknown SYSTEM 'unknown.dtd'>
      <!-- Translated from latex by tralics 2.9, date: 2006/09/20-->
      <unknown>
      <p>Hello, world!
      </p></unknown>

or

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='UTF-8'?>
      <!DOCTYPE unknown SYSTEM 'unknown.dtd'>
      <!-- Translated from latex by tralics 2.15.4, date: 2015/08/21-->
      <unknown>
      <p>Hello, world!
      </p></unknown>

*Tralics* has generated another file, named hello.log. In this case, the
content is more or less the same as what is printed on the terminal. As
a general rule, everything printed on the terminal is also printed on
the transcript file. In what follows we have marked with -- the lines
that differs (the banner line “This is tralics...” is printed in the
terminal before options are parsed, the transcript file is generated
after its name has been found). We have marked with ++ some lines that
are not printed on the terminal. Three lines are marked \*\*, they
indicate some omitted stuff, namely the statistics (same as above),
*File info* and *Bootstrap info*, these are 46 lines starting with
``\countdef``, 43 with ``\dimendef``, 16 with ``\chardef`` or
``\mathchardef``, and 23 with ``\skipdef``. These lines explain, for
instance, that the chapter counter is the counter number 45.

.. container:: log_out

   ::

      -- Transcript file of tralics 2.15.4 for file hello.tex
         Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      -- Tralics is licensed under the CeCILL Free Software Licensing Agreement
      ++ Start compilation: 2015/08/22 11:14:51
      ++ OS: Apple, machine maclaurin
         Starting translation of file hello.tex.
      ++ Output encoding: UTF8  (idem transcript).
      ++ Left quote is ` right quote is '
      ++ \notrivialmath=1
      ++ ++ Input encoding is 1 (iso-8859-1) for the main file
      ** File info
         Configuration file identification: standard $ Revision: 2.24 $
         Read configuration file ../confdir/.tralics_rc.
      ++ Configuration file has type \documentclass
      ++ No \documentclass in source file
      ++ Using some default type
      ++ dtd is unknown from unknown.dtd (standard mode)
      ++ OK with the configuration file, dealing with the TeX file...
      ++ There is a single line
      ++ Starting translation
      ** Bootstrap info
      ** Statistics

Our second document
-------------------

The second example looks a bit more like a LaTeX document. The file is
called hello1.tex; its content is:

.. container:: ltx-source

   ::

      \documentclass{article}
      \begin{document}
      Hello, world!
      \end{document}

The result of the compilation can be found
`here <%0A%20%20doc-start.html#ex1>`__. Using an older version of
*Tralics* gives:

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE std SYSTEM 'classes.dtd'>
      <!-- Translated from latex by tralics 2.13.0, date: 2008/07/22-->
      <std><p>Hello, world!
      </p>
      </std>

The difference between the two compilations is the third line (it holds
the *Tralics* version and the compilation date) and the first: the
default file encoding is now UTF-8.

The essential difference between hello.xml and hello1.xml is the DTD and
the name of the root element. Here it is ``<std>``; this follows from
the following assumptions: *Tralics* finds a configuration file that,
like the standard one, with revision 2.24, maps “article” to “std”, and
there is no article.tcf file in the search path.

If you compile the file with tralics hello1 -noconfig, the result XML
file will be independent of any configuration or tcf file. Translation
is the same as above. The transcript file starts as follows:

.. container:: log_out

   ::

      1  Transcript file of tralics 2.15.4 for file hello1.tex
         Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      3  Tralics is licensed under the CeCILL Free Software Licensing Agreement
         Start compilation: 2015/08/22 12:26:05
      5  OS: Apple, machine maclaurin
         Starting translation of file hello1.tex.
      7  Output encoding: UTF8  (idem transcript).
         Left quote is ` right quote is '
      9  \notrivialmath=1
         ++ Input encoding is 1 (iso-8859-1) for the main file
      11 No configuration file.
         No type in configuration file
      13 Seen \documentclass article
         Potential type is article
      15 Using some default type
         dtd is std from classes.dtd (standard mode)
      17 OK with the configuration file, dealing with the TeX file...
         There are 4 lines

The start of the transcript file is the same as above, except for the
file name and the compilation date. Line 11 says that no configuration
file is found (in fact, none is searched); line 12 says that no “type”
was found (obviously). In such a case, the “type” defaults to the
current documentclass (this behavior is the same as the standard
configuration file). In the case of hello.tex the main file did not have
a ``\documentclass`` command; here there is one, as shown on line 13. As
a consequence *Tralics* is ready to read the article.tcf (this is the
meaning of “potential”); but this is inhibited by the -noconfig option.
So a default type is used (line 15). This type is std, meaning the the
root element is named ``<std>`` (it would have been ``<unknown>`` in the
absence of a ``\documentclass`` command).

This is the start of transcript file, without the option

.. container:: log_out

   ::

      10 ++ Input encoding is 1 (iso-8859-1) for the main file
      11 ++ file .tralics_rc does not exist.
      12 ++ file ../confdir/.tralics_rc exists.
      13 Configuration file identification: standard $ Revision: 2.24 $
      14 Read configuration file ../confdir/.tralics_rc.
      15 Configuration file has type \documentclass
      16 Seen \documentclass article
      17 Potential type is article
      18 Defined type: std
      19 ++ file article.tcf does not exist.
      20 ++ file ../confdir/article.tcf does not exist.
      21 Alias torture does not match article
      22 Potential type article aliased to std
      23 Using type std
      24 dtd is std from classes.dtd (standard mode)
      25 OK with the configuration file, dealing with the TeX file...

On line 11 and 12, you can see that the configuration file (by default,
it is named .tralics_rc) is search in the current directory then in the
../confdir directory, this can be changed via an option. On line 15, you
can see that the potential type is, as above, the current document
class, thus “article”, and the configuration file defines only “std”
(with some aliases). On lines 19 and 20 you can see where the
article.tcf file is searched; if such a file is found it is interpreted.
Lines 20 and 21 show that “article” is aliases to “std” which becomes
the “type”. Finally, line 25 says that the DTD is “std”.

.. container:: log_out

   ::

      158 ++ file hello1.ult does not exist.
          ++ file article.clt does not exist.
      160 ++ file ../confdir/article.clt exists.
          ++ Made @ a letter
      162 ++ Opened file ../confdir/article.clt; it has 34 lines
          Document class: article 2011/05/09 v1.2 Article document class for Tralics
      164 ++ file std.clt does not exist.
          ++ file ../confdir/std.clt exists.
      166 ++ Made @ a letter
          ++ Opened file ../confdir/std.clt; it has 91 lines
      168 File: std 2008/10/23 v1.1 Standard LaTeX document class, for Tralics
          ++ End of file ../confdir/std.clt
      170 ++ cur_file_pos restored to 1
          {Options to execute->letterpaper,10pt,oneside,onecolumn,final}
      172 {Options to execute->}
          ++ End of file ../confdir/article.clt
      174 ++ cur_file_pos restored to 0
          ++ End of virtual file.
      176 ++ cur_file_pos restored to 0

Lines 28 to 157 in the transcript file show register allocation, they
are omitted here. You can see a lots of lines starting with ++. They are
printed by the I/O manager. Whenever *Tralics* tries to open a file, it
print a line in the transcript file. For instance, you can see where the
configuration file is searched. You can also see that the @ character is
made a letter while reading some files; and you can see that
“cur_file_pos” is restored (this variable is used by the class/package
mechanism). You can see the action at the end of some files (the virtual
file contains the documenthook token list).

After the bootstrap phase, *Tralics* executes the instructions that come
from the configuration file, then those of the main file. In the case of
a LaTeX document (i.e., if a document class is found, as indicated on
line 16), the file hello1.ult is loaded, when present; line 158 show
that this file is search only in the current directory.

When the ``\documentclass`` command is seen, the class file is loaded.
As you can see, *Tralics* uses article.clt instead of article.cls. Since
the standard classes (book, report and article) share some options, the
common code is in std.clt. Instead of foo.sty, a file named foo.plt is
used for packages. What you can see in the transcript file is: the
second argument of ``\ProvidesClass``, which is also printed on the
terminal (line 163), and of ``\ProvidesFile`` (line 168), the default
options (argument of ``\ExecuteOptions``, line 171) and the actual
options (optional argument of ``\documentclass``, empty in this example,
line 172). Lines 177 to 186, omitted here, are similar to those shown
above.

Our third document
------------------

Assume that we have a file, named hello.tcf that contains the following
lines.

.. container:: ltx-source

   ::

      ## This is an example of a configuration file for tralics
      ## Copyright 2006 Inria/apics, Jose' Grimm
      ## $Id: hello.tcf,v 1.1 2006/07/17 09:09:06 grimm Exp $
      ## tralics ident rc=hello.tcf $Revision: 1.1 $


      DocType = Article classes.dtd
      DocAttrib =Foo \World
      DocAttrib =A \specialyear
      DocAttrib =B  \tralics
      DocAttrib =C  \today
      BeginCommands
        \def\World{world}
        \def\today{\the\year/\the\month/\the\day}
      End

The file defines, in order, the doctype to use in the XML file, four
attributes to the document element, and two commands ``\World``, and
``\today``. The ``\tralics`` pseudo-command produces
``Tralics version 2.9`` (your mileage may vary), and the
``\specialyear`` pseudo-command returns the value of the option -year
(for more info, see `Tralics options <options.html#rayear>`__ and
`configuration file <titlepage.html#specialyear>`__).

Consider the following source file, named hello2.tex:

.. container:: ltx-source

   ::

      \newcommand\hello{\uppercase {h}ello}
      \documentclass{article}
      \begin{document}
      \hello, \World!
      \end{document}

Compile with tralics hello2.tex -config=hello.tcf -oe8. As mentioned
above, the -oe8 option tells *Tralics* to use UTF-8 encoding for the XML
file, this now the default, but was not for version 2.13.0. We assume
that the hello.tcf file is found `(according to these
rules) <titlepage.html#potential-type>`__. The translation is the
following (for some strange reason the day is shown as 21 and 22).

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='UTF-8'?>
      <!DOCTYPE Article SYSTEM 'classes.dtd'>
      <!-- Translated from latex by tralics 2.13.0, date: 2008/07/22-->
      <Article  C='2008/7/21' Foo='world' B='Tralics version 2.13.0' A='2008'>
      <p>Hello, world!
      </p>
      </Article>

If we use the following invocation tralics hello2 -config=hello -verbose
the transcript file contains much more lines. We give here some of them.

.. container:: log_out

   ::

         1 Transcript file of tralics 2.15.4 for file hello2.tex
         2 Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
         3 Tralics is licensed under the CeCILL Free Software Licensing Agreement
         4 Start compilation: 2015/08/23 11:45:38
        11 Trying config file from user specs: hello
        12 ++ file ../confdir/hello.tcf exists.
        13 Configuration file identification: hello.tcf $ Revision: 1.1 $ 
        14 Read configuration file ../confdir/hello.tcf.
        15 Using tcf type hello
        16 dtd is Article from classes.dtd (standard mode)
        17 OK with the configuration file, dealing with the TeX file...
        18 There are 8 lines
        19 Starting translation

What you can see is where *Tralics* is searching for the configuration
file, and what it found, in particular the DTD.

.. container:: log_out

   ::

       664 [73] %% End bootstrap commands for latex
       665 ++ Input stack empty at end of file
       666 [1] \newcommand\hello{\uppercase {h}ello}
       671 [13]   \def\World{world}
       675 [14]   \def\today{\the\year/\the\month/\the\day}
       679 [17] \addattributestodocument{Foo}{\World}
       686 \World ->world
       690 [17] \addattributestodocument{C}{\today}
       697 \today ->\the \year /\the \month /\the \day 
       702 {\the}
       703 {\the \month}
       704 \the->8.
       706 {\the}
       707 {\the \day}
       708 \the->23.
       712 [1] \InputIfFileExists*+{hello2.ult}{}{}
       718 ++ file hello2.ult does not exist.
       719 [2] \documentclass{article}
       720 {\documentclass}
       721 [3] \begin{document}

We have shown on lines 686, 697, 702 to 708, the trace of evaluation of
TeX command; you can consult this `document <doc-start.html#trans>`__
for explanations. Line 704 shows that the month is an ordinary number,
and the ``\today`` command has to be rewritten if its expansion is
assumed to give 2015/08/23. Line 708 shows that the day is correct.

Lines 664, 666, 671 are formed of a number in square brackets and some
text; the number is the current line number and the text is the content
of the line to evaluate. On line 664, you see the last line of the
bootstrap code, on line 666 the first line of the main file, on line 719
the second line of the main file. This line contains the
``\documentclass`` command that may be used to find the type (in this
case the type is given in the configuration file). The lines inserted
from the configuration file are inserted before this one (671, 675, 679,
690, 712), or at the start, in case where no document class is given.
Lines 679 and 690 correspond to the two “DocAttrib” lines of the
configuration file that could not have been evaluated earlier. As you
can see on line 712, the hello2.ult file is inserted after the code from
the configuration file, and before the document class. Line 721 says
that the line following the ``\documentclass`` has been read, because
this command has an optional argument.

.. container:: log_out

   ::

      1056 ++ Input stack -- 1 ../confdir/article.clt
      1057 {\begin}
      1058 {\begin document}
      1061 +stack: ending environment document; resuming document.
      1063 +stack: level set to 1
      1064 ++ Input stack ++ 1 (AtBeginDocument hook)
      1065 [1] \let\do\noexpand\ignorespaces
      1069 atbegindocumenthook= \let \AtBeginDocument \@notprerr 
            \let \do \noexpand \ignorespaces 

Lines 722 to 1056 correspond to the loading of the class file. Line 1056
says that we are finished with article.clt and we continue with the main
file. Recall line 721; the first token is ``\begin``, it is not an
opening bracket, thus not the start of an optional argument, hence is
executed now. This command reads an argument, the name of the
environment, pushes a new stack level, and executes the command
associated to the start of the environment, here ``\document``. There is
a hack here, see lines 1061 and 1063: the stack is popped. Everything
happens outside an environment; however *Tralics* still pretends that it
is inside “document”. (Normally, at the end of the environment, all
commands that were modified are restored; however, at the end of the
document, these commands become useless, so there is no need to save and
restore). Line 1064 explains that the ``\AtBeginDocument`` hook is about
to be evaluated. The next line shows some additional code. Line 1069
shows all tokens to be evaluated; in order: the ``\AtBeginDocument``
(empty in this example), the redefinitions implies by ``\@onlypreamble``
(the ``\AtBeginDocument``) and the line shown above.

.. container:: log_out

   ::

      1088 [5] \end{document}
      1089 {\end}
      1092 {\end document}
      1093 +stack: level + 2 for environment entered on line 5
      1094 {\enddocument}
      1095 {\endallinput}
      1096 atenddocumenthook: \real-enddocument \endenv
      1097 {\real-enddocument}
      1098 Pop (module) 1: document_v p_v}
      1100 {\endenv}
      1103 +stack: ending environment document; resuming document.
      1104 +stack: level - 2 for environment from line 5
      1105 ++ Input stack empty at end of file
      1106 Bib stats: seen 0 entries.

We omit the trace of the document hook and line 4 of the main document.
What comes now is ``\end{document}``. The effect is first to execute
``\enddocument`` (line 1094) and then ``\endenv`` (this is an internal
command that pops the stack, its effect can be seen on lines 1100, 1103,
1104). However, the stack should be empty, so we artificially push a new
element on the stack (line 1093). Since this is the end of the job, all
remaining text is ignored (lines 1095 and 1105). Since version 2.15.4,
all text after ``\end{document}``, on the same line is also ignored. All
tokens gathered by the ``\AtEndDocument`` command are inserted in the
current input stream in order to be evaluated (there are none in this
example). Line 1096 shows what remains to be executed. The purpose of
the first command is to close all chapters, sections, paragraphs, etc,
so that, if an index has be generated, it is not inside a paragraph.

Note: when line 1106 is printed the job is not complete, as *Tralics*
the bibliography is not yet handled. Our example file has no
bibliography, so nothing special happens.

Our fourth document
-------------------

We assume now that we have a configuration file hello3.tcf containing
this:

.. container:: ltx-source

   ::

      ## This is an example of a configuration file for tralics
      ## Copyright 2006 Inria/apics, Jose' Grimm
      ## $Id: hello3.tcf,v 2.3 2006/07/24 12:09:34 grimm Exp $
      ## tralics ident rc=helloconf3!

      BeginCommands
        \def\World{world}
      End

      BeginTitlePage
       \maketitle <Title> "" ""
      End

      DocType = Article classes.dtd
      att_language = "language"
      xml_stylesheet = "my_stylesheet.css"
      xml_stylesheettype = "text/xsl"

      BeginCommands
      \newcommand\hello{\uppercase {h}ello}
      End

      BeginTitlePage
       \abstract <abstract> "No abstract given" 
       \author <author> "No author given"
      End

A standard configuration file would consist of a single block
BeginCommands ... End and a single block BeginTitlePage ... End, instead
of two blocks. Nevertheless, you have the right to split your commands
as shown here. This configuration file defines five commands. There is
no restriction on ``\hello`` and ``\World``, but the ``\maketitle``
command can be used only once. Moreover, the ``\abstract`` and
``\author`` commands have to be used before the ``\maketitle`` command,
they have a default value.

Consider a source file hello3.tex that contains the following lines

.. container:: ltx-source

   ::

      % -*- latex -*- encoding: utf-8
      % tralics configuration file 'hello3'
      % The previous line magically defines the commands used in this file
      \documentclass{book}
      \begin{document}
      \author{José Grimm}
      \maketitle \normalsize
      \hello, \World!
      \end{document}

The second line of the document tells *Tralics* to use hello3.tcf as
configuration file rather than the default. Since the document class is
``book``, the main element of the XML output has ``part='true'`` in its
attribute list. It has also ``language='english'``, because the default
language is language number 0, namely english and the configuration file
provides the attribute name to use (value of att_language). Finally,
there is a style sheet declaration.

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='UTF-8'?>
      <?xml-stylesheet href="my_stylesheet.css" type="text/xsl"?>
      <!DOCTYPE Article SYSTEM 'classes.dtd'>
      <!-- Translated from latex by tralics 2.15.4, date: 2015/08/24-->
      <Article language='english' part='true'>
      <Title><abstract>No abstract given</abstract>
      <author>José Grimm</author>
      </Title>
      <p>Hello, world!
      </p>
      </Article>

Line 10 of the transcript of the second document says: “Input encoding
is 1 (iso-8859-1) for the main file”. In fact, *Tralics* maintains a
table of transcriptions, zero is for UTF-8, 1 for iso-8859-1, the file
inputenc.plt defines ansinew, applemac, etc. A command of the form
``\usepackage[latin9]{inputenc}`` tells *Tralics* to use the encoding
associated to latin9, after the ``\begin{document}``. This means that
the current file can have more than one encoding; at any point in the
file, you can say\ ``\input@encoding=1`` in order to switch to latin1.
This feature is only available for TeX files: all other files
(configuration files, package file, bibliographic files, etc) have a
fixed encoding. The default encoding is given by an option (-utf8,
-latin1, default is latin1). This can be overwritten, if the document
contains a line of the form ``encoding: utf-8`` near its beginning.

Assume that your file contains a line with
``\show ©A\show éabB\show ó©©©C``, this is latin1 encoded, and let's
pretend it is UTF8. You will get some error messages, see below. At the
end of the run, *Tralics* shows the number of lines converted, the
number of lines that caused trouble, and the number of error. For each
wrong line, the transcript file will contain something like \\show
\\251A\show \\351abB\show \\363\251\251\251C (non-ASCII character are
printed in base 8).

.. container:: tty_out

   ::

      UTF-8 parsing error (line 55, file txtb.tex, first byte)
      UTF-8 parsing error (line 55, file txtb.tex, continuation byte)
      UTF-8 parsing error (line 55, file txtb.tex, continuation byte)
      UTF-8 parsing overflow (char U+E9A69, line 55, file txtb.tex)
      the letter A.
      the letter B.
      the letter C.
      Input conversion errors: 1 line, 4 chars.
      Input conversion: 6 lines converted.

Statistics
----------

The *Tralics* distribution contains a lot of test files, some of them
should compile without error, o

.. container:: tty_out

   ::

       1 This is tralics 2.13.0, a LaTeX to XML translator, running on medee
       2 Copyright INRIA/MIAOU/APICS 2002-2008, Jos\'e Grimm

       3 Licensed under the CeCILL Free Software Licensing Agreement
       4 Starting translation of file torture.tex.
       5 Configuration file identification: standard $ Revision: 2.24 $
       6 Read configuration file /user/grimm/home/cvs/tralics/confdir/.tralics_rc.
       7 Configuration file identification: torture.tcf $ Revision: 1.5 $
       8 Read tcf file for type: ../confdir/torture.tcf
       9 Document class: article 2006/08/19 v1.0 article document class for Tralics
      10 File `taux2.tex' already exists on the system.
      11 Not generating it from this source
      12 Translating section command div0: W.
      13 \show: 0
      14 Translating section command div0: A.
      15 Translating section command div0: B.
      15 Translating section command div0: C.
      17 Translating section command div0: A.
      18 Translating section command div0: B.
      19 Translating section command div0: C.
      20 Warning: junk in table
      21 detected on line 3613 of file torture.tex.
      22 END OF FILE
      23 Bib stats: seen 5(2) entries
      24 Seen 3 bibliographic entries
      25 Math stats: formulas 751, kernels 167, trivial 5, \mbox 19, large 1, small 59.
      ?? List stats: short 0, inc 33, alloc 90238.
      27 Buffer realloc 28, string 12981, size 247367, merge 232
      28 Macros created 1986, deleted 1463; hash size 3062; foonotes 5.
      29 Save stack +5189 -5189.
      30 Attribute list search 6965(1484) found 2667 in 6419 elements (1117 at boot).
      31 Number of ref 31, of used labels 23, of defined labels 31, of ext. ref. 25.
      ?? Modules with 0, without 0, sections with 2, without 29
      33 Input conversion: 49 lines converted.
      34 There were 9 images.
      35 Following images not defined: x, y, Logo-INRIA-couleur, ../../a_b:c, x_, figure1a, figure1b, figure1c.
      36 Output written on torture.xml (164989 bytes).
      37 No error found.
      38 (For more information, see transcript file torture.log)

Line 4, 5 and 6 show that the standard configuration file has been read,
lines 7 and 8 that torture.tcf has been included because of the aliasing
mechanism of the standard configuration file.

Lines 10 and 11 show the output of a ``filecontents`` environment.

Line 12, and 14 to 29 show progress: each time a toplevel section is
translated, its name is printed.

Line 13 is a test of the ``\show`` command.

Line 20-21 is printed when a non-tabular appears in a table.

Line 22 is a result of ``\typeout`` command, placed at the end of the
document: It tests that every line until the last one have been read.

The different statistics have to be interpreted as follows

-  Line 23 says that 5 citations in the document, two of them defined
   with the ``\bibitem`` command, thus three unsolved that are looked
   for in the bibliography data files. Line 24 says that 3 bibliographic
   entries have been found in the data base.
-  Line 25 indicates some math statistics. In fact, 751 math formulas
   are constructed. They were 167 cases in which a non-trivial kernel
   was used (case where an index or an exponent has to be constructed).
   There were 5 *trivial* formulas, such as ``$^{i\grave {e}me}$``,
   ``$23$``, ``$x$`` that were translate in a special manner (Say
   tralics -notrivialmath if you do not like this behavior.) There were
   ten cases where something like ``\mbox`` appeared in a math formula.
   In fact, *Tralics* has created three ``<mtext>`` elements. There was
   1 large and 59 small objects: these are explained elsewhere. It has
   to do with how the command ``\big``, and similar ones are translated.
-  Line ?? says that you used a total of 90238 list cells and the global
   free list was incremented 33 times, decremented 0 times. The figures
   for the testfp file are 953556 cells, 755 increments, and 33
   decrements. Whenever a cell is needed, one is taken from the free
   list; if the free list is empty, 100 cells are allocated; when a cell
   becomes useless, it is added to the free list; when the size of the
   free list exceeds 1000, the whole free list is recycled. In version
   2.13, memory allocation changed, list cells are no more counted.
-  Line 27 gives buffer statistics. Initially buffers can contain 512
   characters. Only once has the size to be increased. All these buffers
   used to create 12981 strings, with a total of 247367 bytes.
   If you say something like ``{foo}{bar}``, when *Tralics* sees the
   first closing brace, it creates an XML element (generally a ``<p>``
   element) with the string foo, and when it sees the second closing
   brace it creates a second element with the string bar (this is
   because strange things can happen when a closing brace is seen). More
   than often, these two elements can be merged. This happened 232
   times.
-  Line 28 says that *Tralics* has created 1986 macros and deleted 1463
   ones. For the fptest, these numbers are much larger, namely 7703
   creations and 5401 deletions; this is a big number, this is because
   lots of tests are done, and each test uses a local macro. Moreover
   3062 slots are used in the hash table by multiletter control
   sequences (it is 2225 at start of document). This number has to be
   compared with the 925 commands of PlainTeX and the 3216 of standard
   LaTeX. Finally, five footnotes and no index are defined in the
   document.
-  Line 29 says that *Tralics* has seen 5189 open braces (or
   ``\begingroup`` command, or environment) that made it increase the
   save stack pointer. Good news is that the same number of closing
   braces has been seen.
-  Line 30 says that *Tralics* has created 6419 elements. 1117 elements
   were created by the bootstrap phase (essentially for the math
   formulas). There is a special hash table for elements and attributes.
   1484 strings were added to this table by the bootstrap phase, and a
   total of 2667 at the end of the run. There were however 6965 access
   to this table.
-  Line 31 says that *Tralics* has defined 31 labels (a label is
   associated to each section, footnote, item in a list, table and
   figure environments, and some math formulas). Of these 31 labels, 23
   were used via a ``\ref`` command (some labels were used more than
   once, since the number of ``\ref`` commnds was 31). Moreover, 25
   references to an external document, via ``\href.``
-  Line ?? explains that there are some modules and some sections with
   (and without) some information useful only for the RAweb. This
   information is not printed anymore.
-  Line 33 says that 49 lines containing non-ASCII characters were
   converted to UTF-8.
-  Lines 34 says that 9 images were included via the
   ``\includegraphics`` commands, only one of them exists. In fact, the
   file torture.img contains the following

   .. container:: log_out

      ::

         # images info, 1=ps, 2=eps, 4=epsi, 8=epsf, 16=pdf, 32=png, 64=gif
         see_image("x",0,33);
         see_image("y",0,2);
         see_image("Logo-INRIA-couleur",0,8);
         see_image("../../tralics/Test/a_b",1,1);
         see_image("../../a_b:c",0,1);
         see_image("x_",0,1);
         see_image("figure1a",0,2);
         see_image("figure1b",0,2);
         see_image("figure1c",0,1);

   The second number (e.g., 33) is the number of times the file is
   included, and the first is the sum of the types, for instance 17=1+16
   means postscript and pdf. Here only a_b is found in PostScript
   format.
-  Line 36 says that *Tralics* has printed a given number of bytes in
   the resulting XML file. This good news.
-  Line 37 says that no error was detected. This is even better news.

A complete example
------------------

We give here an example of a full document. This uses the RR package,
that defines the commands ``\RRetitle``, ``\RRauthor`` ``\RRabstract``
(this make a non-trivial title page), the fancyvrb package (there is a
\`pre=pre' somewhere in the text, the associated action is in the
package) and natbib, for the citations.

The document was translated using the following invocation: tralics txtc
-noentnames -nostraightquotes -nozerowidthspace -trivialmath. We then
converted the XML in HTML, it is given `here <txtc.html>`__.

The file has been slightly modified: the first line is the correct
invocation to tell emacs and *Tralics* that it is latin1 encoded. The
URLs shown here are the ones to use. Starting with version 2.10.7, the
line marked COMPATIBILITY PROBLEM finishes the verbatim environment; the
one that follows has to be used.

.. note::

   This is file ``txtc.tex`` in the tests.

.. container:: ltx-source

   ::

      \documentclass{article}   % -*- coding: iso-8859-1 -*-
      \usepackage{fancyvrb}
      \usepackage{natbib}
      \usepackage{RR}

      \RRetitle{A sample file for Tralics}
      \RRauthor{José Grimm}
      \RRabstract{This document shows some commands of \textit{Tralics}.
      We use it also to show that characters are converted into the right encoding
      in a lot of situations, including commands, titles, indices etc.
      The \textit{XML} result is translated via \textsl{XSLT} into \textbf{HTML} 
      and available  on the web
      \url{http://www-sop.inria.fr/marelle/tralics/txtc.html}.
      Source document can be found at
      \url{http://www-sop.inria.fr/marelle/tralics/doc-step.html}.} 

      \keyword{Latex, XML, HTML, UTF8, Hàn}
      \begin{document}


      % This is à còmment

      \tableofcontents
      \section{Who is Hàn}
      If you call tralics with options -te1a ou -te8a, the terminal should show 
      \verb=^^e0=\index{verb}\footnote{Index here} for the section title; 
      if you say -te1, there is a single byte, if you say
      -te8, there are two bytes. If you say -e1a or -e8a, the XML file should
      contain \verb=&#E0;=, in the case -oe1 ou -oe8, the XML file contains the
      characters shown on the terminal.

      \def\gobble#1{} %% Used later


      The following lines try to demonstrate that Tralics handles 16bit characters.
      An error will be signaled because the argument is out of range; but the
      character with hex value 312 should be valid; the command defined here
      by csname has two characters in its name, it must be followed by an
      exclamation point (a space is allowed between the command an the exclamation
      point).
      \expandafter\def\csname féé\endcsname!{123}
      \expandafter\def\csname f^^^^0123\endcsname!{312}
      \catcode`\é 11 \catcode"123=11 \catcode65536=11
      \féé !! \f^^e9^^^^00e9 !! \f^^^^0123 !!

      This is standard verbatim: \verb+a _bç+, \verb*=a _bç=, \verbèa _bçè,
      \verb-\verb+ { } $$-, \verb +x+ . Think about this last example.
      We index here a word\index{vérb}. Location is just before period.
      This is a verbatim environment
      \begin{verbatim}
      {\let\rm\bf \bf totoé}
      <!--this is a comment -->
      &Dollar; not &Equals; &Euro;
        \end{verbatim}
      % See comment below
      \begin{rawxml}
      {\let\rm\bf \bf totoé}
      <!--this is a comment -->
      &Dollar; not &Equals; &Euro;
      \end{rawxml}

      Note. A verbatim environment neutralises meaning of some commands.
      The last line of the verbatim environment should start with an ampersand 
      character; since this is a special character in XML, it is represented as
      \verb=&amp;= or \verb=&#x26;=. Lines can be numbered; spaces can be replaced
      by non-breaking ones; lines can use special fonts; paragraphs can be
      no-indented, etc.\index{verb@verb}% same as \index{verb}

      On the other hand, a rawxml environment is left unchanged. Remember however
      that end-of-line characters and spaces are removed from the end of the line; a
      new line character is added at the end of the line. If you remove the comment
      between the two environments, replacing it by an empty line, then the second 
      environment will be in vertical mode. Otherwise, the end of the verbatim
      environment inserts a \verb=\noindent=, and the environment that follows is in
      horizontal mode. As a consequence, there will be a P element on the first
      line of the raw xml; moreover, since the final space in a paragraph is
      removed, you will find the end-P element at the end of the line.%
      \index{vérb@verb}% this a new index entry

      The translation of the environment contains e-acute (its representation
      depends on the output encoding), three ampersand characters, a less than sign,
      a greater than sign. The second line is a valid XML comment, the third line is
      well-formed XML (it contains three entities, so that the XML is valid only if
      the DTD defines these entities); it is very easy to produce invalid and
      ill-formed XML.\index{vérb@vérb}% this one alreedy seen

      The xmllatex command is to be used with care. It can produce
      \xmllatex{Hàn Th&\#x1ebf; Thành}{unused}\footnote{Hàn is the author of
      pdftex}. The second argument is meant to be translated by \LaTeX, it is
      ignored by Tralics. Instead of \verb=\xmllatex{foo}{bar}=, define a command,
      use it in the text, and overwrite it in a ult file (user configuratin file).%
      \index{vérb@vérb|bf}% Note that encap is ignored

      In the current version, you can say \'{\^e} because the double-accent 
      mechanism is implemented, or ^^^^1ebf, this is a character, as valid as the
      other ones. This is possible and dangerous too \xmllatex{<TeX/>}{tex}.%
      \index{vérb!vèrb} %subitem in index

      A verbatim test. We put some stuff in English and French before, in order
      to show how it is translated differently. The end of the environment can
      contain spaces (see example above), but nothing else.
      \language=0
      test ligatures: <<>>``''-- et --- !
      \language=1
      test ligatures: <<>>``''-- et --- !
      \numberedverbatim
      \begin{verbatim}
      test : !@#$%^&*()_$
      test : {\foo\} et zxcvbnm,./
      \end{verbatim}
      \begin{verbatim}
      test ZXVBNM<>? ~
      test \verb+\verb-xx-+
      test ligatures: <<>>``''-- et --- !
       \end{verbatim} Not this one COMPATIBILITY PROBLEM
       \end+{verbatim} Not this one %% + added in version 2.11 WORKS
      \end{notverbatim}
      \end{verbatim}

      \gobble{
      \end{verbatim}
      }

      \unnumberedverbatim
      Verbatim without line numbers. 
      \begin{verbatim}
      test : !@#$%^&*()_$
      test : {\foo\} et zxcvbnm,./
      test ZXVBNM<>? ~
      test \verb+\verb-xx-+
      test ligatures: <<>>``''-- et --- !
      test BL : \\738! et \\838!.
      \end{verbatim}
      The BL test is funny; why should it fail? a long long time ago, before
      it was called Tralics, our translator was written in Perl, and such a line 
      was illegal; the math was converted by Omega, see \cite{place99}. We cite also
      \citeyear{mKay}, and \citefullauthor{mathml2}.

      We now test the verbatim extensions. We start with Verbatim
      and demonstrate the counting possibilities
      \begin{Verbatim}                   [numbers=true]
      test line 1a
      test line 1b
      \end{Verbatim}
      and without numbers
      \begin{Verbatim}
      [numbers=true]test line 2a
      [numbers=true]test line 2b
      \end{Verbatim}
      \begin{Verbatim} %
      [numbers=true] this text is ignored
      The environment has an optional argument; spaces but no newlines are allowed
      between brace and bracket; what follows the argument on the line is ignored
      \end{Verbatim}

      We put here the first character of the line in italics
      \def\verbatimfont#1{{\it #1}}
      \def\verbatimnumberfont{\large}
      \count3=4
      \begin{Verbatim}[counter=3]
      5 we use here counter number 3
      6 for counting lines
      \end{Verbatim}
      Define our Verbatim hook now.
      \expandafter\def\csname Verbatim@hook\endcsname{pre=pre,style=latex}
      \begin{Verbatim}[counter=03]
      7 we use here counter number 03 (the same)
      8 but the HTML output differs a lot.
      \end{Verbatim}
      \newcounter{vbcounter}
      \setcounter{vbcounter}{\count3}
      \begin{Verbatim}[counter=vbcounter]
      9 we use here counter named vbcounter
      10 initialised to the value of the previous counter
      \end{Verbatim}
      \begin{Verbatim}[counter=vbcounter]
      11 yet another verbatim line (ok with é^^e9?)
      \end{Verbatim}

      \let\verbatimfont\tt
      \def\verbatimnumberfont#1{\xbox{vbnumber}{#1}}

      \DefineVerbatimEnvironment{xverbatim}{Verbatim}{pre=pre,style=latex}
      \begin{xverbatim}[numbers=left]
      note that, if no counter is specified, it is FancyVerbLine
      \end{xverbatim}
      \begin{xverbatim}[numbers=left,firstnumber=last,style=log]
      and that the first line is numbered one by default.
      Of course, options given on the line have precedence over options
      inherited from the definition.
      \end{xverbatim}

      \newenvironment{centré}{\centering}{}
      \begin{centré}
      In French, centré means centered.
      \end{centré}

      \DefineShortVerb{\|}
      \SaveVerb{DU}|$_|\def\DU{\UseVerb{DU}} %$
      \section{Short  Verb, as in \DU}
      \let\verbatimfont\sffamily
      Test of |\DefineShortVerb| and |\UndefineShortVerb|. Normally
      the bar is used, but 16bit characters are possible. Example, with itrema:
      \DefineShortVerb{\ï}
      |toto| ïxï |+x-| ï|t|ï,
      \UndefineShortVerb{\ï}
      and without:
      |toto| ïxï |+x-| ï|t|ï
      Spaces: like this |+ +| or that \fvset{showspaces=true}|+ +|
      Verbatimfoo: \verb|+ foo +*foo*foo*|.

      \def\verbatimfont#1{{#1}}
      \def\verbprefix#1{A#1A}
      \def\verbatimprefix#1{B#1B}

      \SaveVerb{Ç}|}|\def\FE{\UseVerb{Ç}}
      \DefineShortVerb{\+}
      \SaveVerb{VE}+|+\def\VE{\UseVerb{VE}}
      \UndefineShortVerb{\+}
      \UndefineShortVerb{\|}

      Test of useverb \UseVerb{Ç}, \FE,\VE, \DU.
      \begin{verbatim}
      We have changed the font, and added a prefix
      Spaces are special
      \end{verbatim}

      Switch to English, for colons in URLs \language=0

      \bibliography{tralics}

      \end{document}
