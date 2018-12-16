%% -*- latex -*-
\ProvidesPackage{fancyvrb}[2006/08/26 v1.0 fancyverb for tralics]
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% $Id: fancyvrb.plt,v 2.5 2007/12/12 17:13:26 grimm Exp $

\def\DefineVerbatimEnvironment#1#2#3{%
\expandafter \def\csname#1@hook\endcsname{#3}%
\expandafter\let\csname#1\expandafter\endcsname\csname#2\endcsname
\expandafter\let\csname end#1\expandafter\endcsname\csname end#2\endcsname}

\ifx\define@key\undefined %% provided by the keyval package, builtin in V11.1
\def\define@key#1#2{%
  \@ifnextchar[{\KV@def{#1}{#2}}{\@namedef{KV@#1@#2}##1}}
\def\KV@def#1#2[#3]{%
  \@namedef{KV@#1@#2@default\expandafter}\expandafter
    {\csname KV@#1@#2\endcsname{#3}}%
  \@namedef{KV@#1@#2}##1}
\fi

\def\FV@pre@pre{\begin{xmlelement*}{pre}}
\def\FV@post@pre{\end{xmlelement*}}
\def\FV@pre@verbatim{\begin{xmlelement*}{verbatim}}
\def\FV@post@verbatim{\end{xmlelement*}}
\def\FV@style@log{\XMLaddatt{class}{log-code}}
\def\FV@style@xml{\XMLaddatt{class}{xml-code}}
\def\FV@style@latex{\XMLaddatt{class}{latex-code}}

\newlength \topsep

