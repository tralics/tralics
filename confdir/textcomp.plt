%%%  -*- latex -*-
\ProvidesPackage{textcomp}[2008/11/09 v1.1  textcomp]

%% $Id: textcomp.plt,v 2.6 2015/07/24 13:31:02 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007, 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
\DeclareOption{full}{}
\DeclareOption{almostfull}{}
\DeclareOption{euro}{}
\DeclareOption{safe}{}
\DeclareOption{force}{}
\DeclareOption{warn}{}
\ExecuteOptions{almostfull}
\ProcessOptions\relax


\def\X#1{\expandafter\def\csname text#1\endcsname{\xbox{char}{\XMLaddatt{name}{#1}}}}

\X{compwordmark}\X{blank}\X{born}\X{capitalcompwordmark}
\X{centoldstyle}\X{compwordmark}\X{copyleft}\X{dblhyphen}\X{died}
\X{discount}\X{divorced}\X{dollaroldstyle}\X{gurani}\X{interrobangdown}
\X{lbrackdbl}\X{leaf}\X{married}\X{quotestraightbase}\X{quotestraightdblbase}
\X{rbrackdbl}\X{tildelow}\X{sevenoldstyle}
\X{zerooldstyle}\X{twooldstyle}\X{threeoldstyle}\X{sixoldstyle}
\X{oneoldstyle}\X{nineoldstyle}\X{eightoldstyle}\X{fiveoldstyle}\X{fouroldstyle}
\let\X\undefined
\endinput
