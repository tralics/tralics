%%% -*- latex -*-
\ProvidesPackage{latex}[2007/12/22 v1.0 Implements some basic latex cmmands]
%% $Id: latex.plt,v 2.1 2008/02/19 16:21:17 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\defaulthyphenchar=`\-
\interfootnotelinepenalty=100

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\newhelp#1#2{\newtoks#1#1\expandafter{\csname#2\endcsname}}
\def\newinsert#1{...}

\def\magstephalf{1095 }
\def\magstep#1{\ifcase#1 \@m\or 1200\or 1440\or 1728\or 2074\or 2488\fi\relax}
\def\mathhexbox#1#2#3{\mbox{$\m@th \mathchar"#1#2#3$}}
\def\underbar#1{\underline{\sbox\tw@{#1}\dp\tw@\z@\box\tw@}}
\def\hidewidth{\hskip\hideskip}
\def\narrower{%
  \advance\leftskip\parindent
  \advance\rightskip\parindent}
\def\ialign{\everycr{}\tabskip\z@skip\halign} % initialized \halign
\def\oalign#1{\leavevmode\vtop{\baselineskip\z@skip \lineskip.25ex%
  \ialign{##\crcr#1\crcr}}}
\def\ooalign{\lineskiplimit-\maxdimen \oalign}

\def\CheckCommand{\@star@or@long\check@command} %% TODO

\edef\@backslashchar{\expandafter\@gobble\string\\}
\def\protected@edef{} %% todo
\def\protected@xdef{} % todo
\def\unrestored@protected@xdef{}% todo

\gdef\PackageError#1#2#3{}
\gdef\ClassError#1#2#3{}
\def\PackageWarning#1#2{}
\def\PackageWarningNoLine#1#2{}
\def\PackageInfo#1#2{}
\def\ClassWarning#1#2{}
\def\ClassWarningNoLine#1#2{}
\def\ClassInfo#1#2{}
\gdef\@latex@error#1#2{}
\def\@latex@warning#1{}
\def\@latex@warning@no@line#1{}
\def\@latex@info#1{}
\def\@latex@info@no@line#1{}
\let\@warning\@latex@warning
\let\@@warning\@latex@warning@no@line
\let\@latexerr\@latex@error

\DeclareRobustCommand{\GenericInfo}[2]{}%  BOF 
\DeclareRobustCommand{\GenericWarning}[2]{}% BOF
\DeclareRobustCommand{\GenericError}[4]{}% BOF 

\def\@setpar#1{\def\par{#1}\def\@par{#1}}
\def\@par{\let\par\@@par\par}
\def\@restorepar{\def\par{\@par}}

\def\@bsphack{%
  \ifhmode
    \@savsk\lastskip
  \fi}
\def\@esphack{%
  \ifhmode
    \ifdim\@savsk>\z@
      \ignorespaces
    \fi
  \fi}
\def\@Esphack{%
  \ifhmode
    \ifdim\@savsk>\z@

      \@ignoretrue
      \ignorespaces
    \fi
   \fi}

\def\addvspace#1{ ... }%
\def\addpenalty#1{ ... }%


%\nobreakdashes-- is  the same as \setbox0\hbox{--\nobreak}\unhbox0
\DeclareRobustCommand{\nobreakdashes}{...}
\def\enskip{\hskip.5em\relax}
\def\negthinspace{\kern-.16667em }
\long\def \protected@write#1#2#3{%
      \begingroup
       \let\thepage\relax
       #2%
       \let\protect\@unexpandable@protect
       \edef\reserved@a{\write#1{#3}}%
       \reserved@a
      \endgroup
      \if@nobreak\ifvmode\nobreak\fi\fi
}

\def\settoheight{\@settodim\ht}
\def\settodepth {\@settodim\dp}
\def\settowidth {\@settodim\wd}


\def\@uclclist{\oe\OE\o\O\ae\AE \dh\DH\dj\DJ\l\L\ng\NG\ss\SS\th\TH}
\def\LastDeclaredEncoding{}
\DeclareRobustCommand\linespread[1]{}

\let\@@hyph=\-        % Save original primitive definition
\let\@@italiccorr=\/
\def\@unexpandable@protect{\noexpand\protect\noexpand}
\def\set@display@protect{\let\protect\string}
\def\set@typeset@protect{\let\protect\@typeset@protect}
\def\nfss@text#1{{\mbox{#1}}}

\def \nocorrlist {,.}
\let \nocorr \relax
%  \def \nfss@text {\leavevmode\hbox}  BOF

\def\newlabel#1#2{} % may have 5 args
\let\ignorespacesafterend\@ignoretrue

\def\buildrel#1\over#2{\mathrel{\mathop{\kern\z@#2}\limits^{#1}}}

\def\openup{\afterassignment\@penup\dimen@}
\def\@penup{\advance\lineskip\dimen@
  \advance\baselineskip\dimen@
  \advance\lineskiplimit\dimen@}
\def\lefteqn#1{\rlap{$\displaystyle #1$}}
\def\makelabel#1{%
  \@latex@error{Lonely \string\item--perhaps a missing
        list environment}\@ehc}

\def\lrbox#1{%
  \edef\reserved@a{%
    \endgroup
    \setbox#1\hbox{%
      \begingroup\aftergroup}%
        \def\noexpand\@currenvir{\@currenvir}%
        \def\noexpand\@currenvline{\on@line}}%
  \reserved@a
    \@endpefalse
    \color@setgroup
      \ignorespaces}
\def\endlrbox{\unskip\color@endgroup}


%Tabulations not yet done
\def\kill{}
\def\tabbing#1{}
\def\extracolsep#1{\tabskip #1\relax}
\def\vline{\vrule \@width \arrayrulewidth}
\gdef\shortstack{\@ifnextchar[\@shortstack{\@shortstack[c]}}
\def\title#1{\gdef\@title{#1}}
\def\author#1{\gdef\@author{#1}}
\def\date#1{\gdef\@date{#1}}
\def\thanks#1{...}
\def\and{} % Titlepage

\def\addcontentsline#1#2#3{...}%
\long\def\addtocontents#1#2{...}%
\def\contentsline#1{\csname l@#1\endcsname}
\def\@dottedtocline#1#2#3#4#5{...}%
\def\numberline#1{\hb@xt@\@tempdima{#1\hfil}}
\def\footnoterule{}

\def\footnotemark{}% optarg
\def\footnotetext{}%  optatg + mandatory arg

\newbox\@begindvibox
\def \AtBeginDvi #1{%
  \global \setbox \@begindvibox
    \vbox{\unvbox \@begindvibox #1}}


\def\@thehead{\@oddhead} % initialization
\def\@thefoot{\@oddfoot}

\gdef \enlargethispage {
   \@ifstar{\@enlargepage}{\@enlargepage}}
\gdef\@enlargepage#1{}

\newcommand\suppressfloats[1][]{}

\endinput
size=400
size=385
size=365
size=344
size=323
size=307
size=207 