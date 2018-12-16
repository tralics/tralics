%%%  -*- latex -*-
\ProvidesPackage{graphicx}[2008/11/06 v1.0  Enhanced Graphics in Tralics]


%% $Id: graphicx.plt,v 2.1 2008/11/13 14:13:38 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\DeclareOption{unknownkeysallowed} {}
\DeclareOption*{\PassOptionsToPackage\CurrentOption{graphics}}
\ProcessOptions
\RequirePackage{graphics}

\@ifundefined{setkeys}{\RequirePackage{keyval}}{}

\define@key{Grot}{origin}[c]{\XMLaddatt{origin}{#1}}
\define@key{Grot}{x}{\setlength\dimen@{#1}{\XMLaddatt{x}{\the\dimen@}}}
\define@key{Grot}{y}{\setlength\dimen@{#1}{\XMLaddatt{y}{\the\dimen@}}}
\define@key{Grot}{units}{\XMLaddatt{units}{#1}}

\let\@orig@rotatebox\rotatebox
\def\rotatebox{\@ifnextchar[\Grot@box@kv\@orig@rotatebox}


\def\Grot@box@kv[#1]#2#3{%
  \@orig@rotatebox{#2}{\setkeys{Grot}{#1}#3}}%

\endinput
