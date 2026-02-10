# Tralics

Tralics is a LaTeX to XML translator.

## Build and test

```
cmake -S . -B build -G Ninja
ninja -C build
```

## Run

```
./build/tralics input.tex
```

## Configuration

Default configuration files live in `confdir/`.

The archive contains four directories and some files.


The src directory contains the following files
  Copyright: copy of the copyright notice
  README: explains how to create the Tralics binary
  Makefile  : this is the Makefile
  *.cpp *.h : the sources.
You get a binary version of Tralics by saying `make' in this directory.

The confdir directory contains the following files
   config_empty tralics_rc .tralics_rc
      -> configuration files
   - files of the form foo.tcf,  these are Tralics configuration files
   - files of the form foo.clt,  these are Tralics version of class files
   - files of the form foo.plt,  these are Tralics version of packages
These files are used by Tralics; when you install a new version, you should
copy them in the appropriate place.


The test directory contains some test files
The Modele directory contains translation of the test files.
The test directory contains `alltests',
  this is a shell script that compiles the test files and compares
  them against models.

Since July 2008, directories raweb, xml, styles, icons are distributed
in an alternate file, named tralics-extra-2.12.7.tar.gz
(the version number may vary).


The src directory contains the following files
  Copyright: copy of the copyriht notice
  README: explains how to create the Tralics binary
  Makefile  : this is the Makefile
  *.cpp *.h : the sources.


The confdir directory contains the following files
   test.tcd cedram.tcf eurotex.tcf unused.tcf testa.tcf tpa.tcf hello.tcf
   torture.tcf hello1.tcf hello3.tcf gescap.tcf tp.tcfRR.tcd rabib.tcf
      -> example of tcf files
   config_empty tralics_rc .tralics_rc
      -> configuration files
  ra.tcf : tcf file for the raweb
  mpd.tex mpmath.tex : some tex files


The xml directory contains the following files and directories
  mathml2 (directory)
    -> the mathml2 DTD, version 1.12, date 2003/11/04, downloaded 2004/09/15
     (see the copyright notice in each of these files)

  fotex.sty-patched, fotex-add.sty
    a patched version of fotex.sty (by Sebastian Rahtz)
    (see copyright notice in the file)
    (see also README file in the xml directory)

  raweb3-makecolspec.xsl raweb3-param.xsl raweb3-table.xsl
   Three auxiliary style sheets, from the TEI by Sebastian Rahtz
   adapted to tralics (see the copyright notice in each of these files)

  preview.xsl
   Style sheet for producing small images.

  raweb3fo.xsl rrfo3.xsl
   Two style sheets, used at Inria, given as example.
   Inspired by from the TEI by Sebastian Rahtz.
   (see the copyright notice in each of these files)

  raweb3.dtd
    The Raweb DTD (for year 2003).

  raweb-cfg.sty
    re-implementation of the MathML to dvi converter
    (distributed under the LaTeX Project Public License )

  tralics-iso.sty
    a TeX file that defines lots of mathematical symbols

The icons directory contains some icons to be used for the raweb

The raweb directory contains
  raweb.cls ra2006.cls
      : the classes for the raweb
  rawebnum.bst  rawebalpha.bst rawebxml.bst raweb.bst
      : bst files
  Logo-INRIA-couleur.ps  Logo-INRIA-couleur.pdf LogoRA2006.pdf LogoRA2006.eps
      : Logos

The main directory contains
   ChangeLog : a file that describes what has changed
  README    : this file
  LISEZMOI  : French version of this file
  Copyright : French/English copyright notice for Tralics
  COPYING   : Copyright notice for the whole package
  Licence_CeCILL_V2-en.txt  Licence_CeCILL_V2-fr.txt
    English and french version of the CeCILL License, text version
  rahandler.pl
     Perl script called by tralics in case action is required


The test directory contains

   torture.tex, tormath.tex fptest.tex : the 3 main testfiles
   torture.bib: a bibtex associated to it
   tortureaux.tex taux1.tex taux2.tex:
      test file (for testing `\openin`)
   tpa.tex:  a file for testing the titlepage stuff
   lxdoc.tex  :
      the source of the documentation (now obsolete)
   tralics-euro2003.ps :
      preliminary version of the paper eurotex 20023

You can compile tralics issuing the make command.
A C++ compiler (like gcc version 3 or 4 is required)

You can test the software: go into the test directory,
run tralics on the files torture, fptest, tormath, tormath1
The result should be: No error found.


In case of problem contact : Jose.Grimm@sophia.inria.fr
or consult the Web page: http://www.inria.fr/apics/tralics

Compiling instructions.

Only standard C++ libraries are required. Just say make,
this will provide a tralics binary.
You can uncomment the line 'CPPFLAGS=-g -Wall' in case of trouble.
This Software was compiled on Windows (cygwin gcc 3.4.4)
on linux/fedora (gcc 3.4.4), on MacOS 10.4 (gcc 4.0.0)

The binary contains the default location of the tralics configuration files.
You can modify this in the following way:
a) uncomment the line RAOPT=somevalue
  and set the variable TRALICSDIR (it can be an argument of make)
b) edit the file txpath.h and define TRALICSDIR there.
You can do the same for confdir

(VB) except, TRALICSDIR is never read ...

In case of problem contact: tralics@sophia.inria.fr
or consult the Web page: http://www.inria.fr//tralics
Report all bugs at the address mentioned above.

## README file from the /Test folder

Copyright Jos� Grimm, Inria, projet APICS. 2004
Email: Jose.Grimm@sophia.inria.fr

This software is a computer program whose purpose is to
translate LaTeX documents into XML format.

This software is governed by the CeCILL  license under French law and
abiding by the rules of distribution of free software.  You can  use,
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and,  more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.


## COPYING file from the /confdir folder

The confdir directory contains some files to be used with Tralics.

---------------------------------------------------------------------

If you modify the raweb configuration file, the file named ra.tcf,
you should modify or remove the line containing `tralics ident',
as explained in this  file.
