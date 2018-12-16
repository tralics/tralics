%% -*- latex -*-
\ProvidesPackage{comma}
      [2007/12/29 v1.0 Insert commas every three digits (DPC)]

%% $Id: comma.plt,v 2.1 2008/02/19 16:21:17 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% Original Copyright LPPL, by
%%% David Carlisle david@dcarlisle.demon.co.uk
%%%

% #1 is the name of a LaTeX counter.
\def\commaform#1{%
  \expandafter\@commaform\csname c@#1\endcsname}

\def\commaformtoken{,}

\def\@commaform#1{%
  \expandafter\@commaaux
    \expandafter{\expandafter}%
    \number\@comma@ux#1%
    \relax\@empty\@empty}
\def\@comma@ux#1{#1 }


\def\@commaaux#1#2#3#4{%
  \ifx\relax#2%
    \addcomma#1\relax
  \else
    \ifx\relax#3%
      \addcomma\@empty\@empty#1#2\relax
    \else
      \ifx\relax#4%
        \addcomma\@empty#1#2#3\relax
      \else
         \@commaauxafterfi{#1#2#3#4}%
      \fi
    \fi
  \fi}


\def\@commaauxafterfi#1\fi\fi\fi{%
  \fi\fi\fi\@commaaux{#1}}

\def\addcomma#1#2#3#4{%
  #1#2#3%
  \if#4\relax
  \else
    \commaformtoken
    \expandafter\addcomma\expandafter#4%
  \fi}
 
\endinput