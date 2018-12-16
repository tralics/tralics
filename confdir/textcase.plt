%%% -*- latex -*-
\ProvidesPackage{textcase}[2007/11/08 v1.0 Text only upper/lower case changing]
%% $Id: textcase.plt,v 2.1 2007/11/22 08:37:08 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Latex code copyright 1998 David Carlisle

\let\MakeTextUppercase\@@MakeTextUppercase
\let\MakeTextLowercase\@@MakeTextLowercase
\let\NoCaseChange\@firstofone
\def\@NoCaseChange#1{\noexpand\NoCaseChange{\noexpand#1}}
\def\@tc@cite#1#{\toks@{\noexpand\cite#1}\@tc@citex}
\def\@tc@citex#1{\NoCaseChange{\the\toks@{#1}}}
\def\@tc@label#1{\NoCaseChange{\label{#1}}}
\def\@tc@ref#1{\NoCaseChange{\ref{#1}}}
\def\@tc@ensuremath#1{\NoCaseChange{\ensuremath{#1}}}

\DeclareOption{overload} {%
\let\MakeUppercase\@@MakeTextUppercase
\let\MakeLowercase\@@MakeTextLowercase
}

\ProcessOptions\relax
\endinput

