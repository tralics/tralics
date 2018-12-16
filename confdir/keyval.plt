%% -*- latex -*-
\ProvidesPackage{keyval}
                 [2008/02/23 v1.2 key=value parser for Tralics (DPC)]

%% $Id: keyval.plt,v 2.4 2009/01/05 10:35:31 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% \define@key and \tralics@split are coded in C++
%% \defin@key{foo}{bar}{ge#1e} is the same as \def\KV@foo@bar#1{ge#1e}
%% \defin@key{foo}{bar}[ok]{ge#1e} is the same as 
%%   \def\KV@foo@bar@default{\KV@foo@bar{ok}} \def\KV@foo@bar#1{ge#1e}

%% \tralics@split{L}\A\B{u=v,w} expands into
%% \A{Lu}{v}\B{Lw}, 

\def\Gin@prefix{Gin}
\def\setkeys#1{\def\tmp{#1}\ifx\tmp\Gin@prefix\let\tmp\Gin@set\else
\def\tmp{\tralics@split{KV@#1@}\KV@normal\KV@default}\fi\tmp}
\def\Gin@set#1{\def\Gin@keys{#1,}}


\def\KV@normal#1#2{%
  \expandafter\let\expandafter\@tempc \csname#1\endcsname 
  \ifx\@tempc\relax \KV@errx{#1 undefined}%
  \else \@tempc{#2}\fi
}

\def\KV@default#1{%
    \expandafter\let\expandafter\@tempb \csname#1@default\endcsname
    \ifx\@tempb\relax \KV@err{No value specified for #1}% always err
    \else \@tempb\relax\fi
}

\DeclareOption{unknownkeysallowed}{%
  \def\KV@errx#1{\PackageWarning{keyval}{#1}}}

\DeclareOption{unknownkeyserror}{%
  \def\KV@errx#1{\PackageError{keyval}{#1}}}

\ExecuteOptions{unknownkeyserror}

\let\KV@err\KV@errx

\ProcessOptions



\endinput
