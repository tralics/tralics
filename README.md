![C/C++ CI](https://github.com/UMS-Mathdoc/tralics/workflows/C/C++%20CI/badge.svg)

This file is work in progress, below is all the documentation spread around the
upstream distribution.

# Previous info collected from the source code

## Original Copyright file from the upstream distribution

Copyright José Grimm, Inria, projet APICS. 2004, 2005, 2006, 2007, 2008
Email: Jose.Grimm@sophia.inria.fr
Web page: http://www.inria.fr/apics/tralics

(French version of the Copyright notice can be found after the English one)
(French and English versions of licence are provided in the files
Licence_CeCILL_V2-en.txt and Licence_CeCILL_V2-fr.txt)


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

------------------------------------------------------------------------------

Copyright José Grimm, Inria, projet APICS. 2004, 2005, 2006, 2007, 2008
Email: Jose.Grimm@sophia.inria.fr
Page web: http://www.inria.fr/apics/tralics

Ce logiciel est un programme informatique servant à traduire
des documents du format LaTeX vers le langage XML.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.




## Original COPYING file from the upstream distribution

Tralics is a software that comes with 1) the sources files, 
2) some test files, 3) some utilitary files, 4) the documentation.

The distribution includes 1, 2 and 3. Some utilitary files are
copies, or modifications of files found on the WEB, with their own
copyright notice (see the README file for the list of these files).

The source files are Copyright by Inria, see notice below.

The test files are some TeX files that can be translated by tralics.
You can use or modify them as you wish. 
If you distribute a modified version of Tralics, make sure that
the script alltests runs.


If you modify the raweb configuration file, confdir/ra.tcf,
or a file of the form ra2007.tcf, 
you should modify or remove the line containing `tralics ident',
as explained in the file.

In case of problem contact : tralics@sophia.inria.fr
or consult the Web page: http://www.inria.fr/apics/tralics

-------------------------------------------------------------------
Copyright notice for Tralics:

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

## Original README file from the upstream distribution

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
   http://www.cecill.info
See the files COPYING and Copyright for additional details.
Plain text version of the license are in the files
  Licence_CeCILL_V2-en.txt  Licence_CeCILL_V2-fr.txt


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



In case of problem contact : tralics@sophia.inria.fr
or consult the Web page: http://www.inria.fr/apics/tralics


The remaining of the archive is formed of miscellaneous files, that can be 
used to convert the XML produced by Tralics into HTML, Pdf, or whatever.



July 2008.

## Original LISEZMOI file from the upstream distribution

// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
// sur le site "http://www.cecill.info".
// (voir fichier Licence_CeCILL_V2-fr.txt)
// Pour plus de détails : voir la fin du fichier.

L'archive contient six répertoires et quelques fichiers.
Les répertoires sont xml, Test, confdir, icons et src.
Pour avoir un exécutable, aller dans le répertoire src, et taper make.
(ce qui suit est une copie du fichier README en anglais)

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

August 2006.


-------------------

Ce logiciel est un programme informatique servant à traduire
des documents du format LaTeX vers le langage XML.

Ce logiciel est régi par la licence CeCILL soumise au droit français et
respectant les principes de diffusion des logiciels libres. Vous pouvez
utiliser, modifier et/ou redistribuer ce programme sous les conditions
de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA 
sur le site "http://www.cecill.info".

En contrepartie de l'accessibilité au code source et des droits de copie,
de modification et de redistribution accordés par cette licence, il n'est
offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
seule une responsabilité restreinte pèse sur l'auteur du programme,  le
titulaire des droits patrimoniaux et les concédants successifs.

A cet égard  l'attention de l'utilisateur est attirée sur les risques
associés au chargement,  à l'utilisation,  à la modification et/ou au
développement et à la reproduction du logiciel par l'utilisateur étant 
donné sa spécificité de logiciel libre, qui peut le rendre complexe à 
manipuler et qui le réserve donc à des développeurs et des professionnels
avertis possédant  des  connaissances  informatiques approfondies.  Les
utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
logiciel à leurs besoins dans des conditions permettant d'assurer la
sécurité de leurs systèmes et ou de leurs données et, plus généralement, 
à l'utiliser et l'exploiter dans les mêmes conditions de sécurité. 

Le fait que vous puissiez accéder à cet en-tête signifie que vous avez 
pris connaissance de la licence CeCILL, et que vous en avez accepté les
termes.

## README file from the /src folder

Copyright notice.

   This software is governed by the CeCILL license under French law and
   abiding by the rules of distribution of free software.  You can  use, 
   modify and/ or redistribute the software under the terms of the CeCILL
   license as circulated by CEA, CNRS and INRIA at the following URL
   "http://www.cecill.info". 
   (See the file COPYING in the main directory for details)

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

---------------------------------------------------------------------

Copyright Jose' Grimm, Inria, projet APICS. 2004, 2005, 2006
Email: Jose.Grimm@sophia.inria.fr
Web page: http://www.inria.fr/apics/tralics

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
