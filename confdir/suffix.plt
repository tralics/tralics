%%%  -*- latex -*-
\ProvidesPackage{suffix}[2007/12/15 v1.0 Variant command support]


%% $Id: suffix.plt,v 2.1 2007/12/19 08:38:31 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% suffix is part of the bigfoot bundle for critical typesetting
%% Copyright 2002, 03, 04, 05, 06  David Kastrup <dak@gnu.org>


\def\WithSuffix{\@temptokena{}\let\WSF@global\relax
  \WSF@sfx}
\long\def\WSF@sfx#1#2{\WSF@append{#1}\def\reserved@a{#2}%
  \afterassignment\WSF@decsuff \WSF@gobblenext}

\def\WSF@append#1{\@temptokena\expandafter{\the\@temptokena#1}}

\def\WSF@gobblenext{\let\reserved@b= }
\long\def\SuffixName#1{WSF:\string#1 \meaning}
\def\NoSuffixName{WSF:\string}
\def\WSF@decsuff{\ifcsname
    \expandafter\NoSuffixName\reserved@a\endcsname
  \else
    \WSF@global\expandafter\let\csname
       \expandafter\NoSuffixName\reserved@a
      \expandafter\endcsname \reserved@a
    \long\def\reserved@c##1{\WSF@global\protected\def
      ##1{\WSF@suffixcheck##1}}%
    \expandafter\reserved@c\reserved@a
  \fi
  \WSF@global
    \the\expandafter\@temptokena\csname
    \expandafter \SuffixName
    \reserved@a\reserved@b\endcsname}
\def\WSF@suffixcheck#1{\begingroup\def\reserved@a{#1}%
  \futurelet\reserved@b\WSF@suffixcheckii}
\def\WSF@suffixcheckii{\ifcsname \expandafter\SuffixName
    \reserved@a\reserved@b\endcsname
      \expandafter
      \WSF@suffixcheckiii
    \else
      \expandafter
      \WSF@suffixcheckiv
    \fi}
\def\WSF@suffixcheckiii{%
  \afterassignment\endgroup
  \expandafter\aftergroup
    \csname \expandafter \SuffixName\reserved@a\reserved@b\endcsname
    \WSF@gobblenext}

\def\WSF@suffixcheckiv{%
    \expandafter\endgroup
    \csname \expandafter\NoSuffixName\reserved@a\endcsname}
\WithSuffix\def\WSF@sfx\long{\WSF@append\long\WSF@sfx}
\WithSuffix\def\WSF@sfx\global{\let\WSF@global\global\WSF@sfx}
\WithSuffix\def\WSF@sfx\protected{\WSF@append\protected\WSF@sfx}
\WithSuffix\def\WSF@sfx\expandafter{\expandafter\WSF@sfx\expandafter}
\WithSuffix\edef\WSF@sfx\gdef{\let\WSF@global\global
  \expandafter\noexpand\csname\NoSuffixName\WSF@sfx\endcsname\def}
\WithSuffix\edef\WSF@sfx\xdef{\let\WSF@global\global
  \expandafter\noexpand\csname\NoSuffixName\WSF@sfx\endcsname\edef}
\endinput
%%

