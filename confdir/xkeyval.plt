%%% -*- latex -*-

%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
\def \@tempa $#1 #2 #3 #4 #5 #6 #7 ${%
  \def \filedate    {#4}%
  \def \fileversion {v#3}%
}
\@tempa $Id: xkeyval.plt,v 2.2 2008/02/25 07:58:28 grimm Exp $
\ProvidesPackage{xkeyval}
  [\filedate \space \fileversion \space  Package option processing (HA)]
\NeedsTeXFormat{LaTeX2e}[1995/12/01]

%% This is the magic line
\csname tralics@boot@keyval\endcsname
%% used in case of warnings
\def\XKV@warn#1{\PackageWarning{xkeyval}{#1}}


%\def\XKV@ifstar#1{\@ifnextcharacter*{\@firstoftwo{#1}}}
%\def\XKV@ifplus#1{\@ifnextcharacter+{\@firstoftwo{#1}}}
\DeclareOptionX*{%
  \PackageWarning{xkeyval}{Unknown option `\CurrentOption'}%
}

\ProcessOptionsX\relax

\endinput

