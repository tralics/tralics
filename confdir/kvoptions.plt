%%%  -*- latex -*-
\ProvidesPackage{kvoptions}[2008/12/23 v1.0  Keyval support for LaTeX options (HO)]


%% $Id: kvoptions.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Original latex code
%% Copyright (C) 2004, 2006, 2007 by
%%    Heiko Oberdiek <oberdiek@uni-freiburg.de>

% Make sure we 

\@ifundefined{setkeys}{\RequirePackage{keyval}\relax}{}

\DeclareOption{debugshow}{}
\DeclareOption{patch}{}% Not yet implemented
\ProcessOptions\relax

%% Two comands defined in C++
\define@key{KVO}{family}{\KVO@family@set{#1}}
\define@key{KVO}{prefix}{\KVO@prefix@set{#1}}
\newcommand*{\SetupKeyvalOptions}{\setkeys{KVO}}

%% For optional argument of \DisableKeyvalOption

\SetupKeyvalOptions{%
  family=KVOdyn,%
  prefix=KVOdyn@%
}
\DeclareBoolOption[true]{global}
\DeclareComplementaryOption{local}{global}
\DeclareStringOption[undef]{action}

\let\KVOdyn@name\relax
\let\KVOdyn@ext F

\define@key{KVOdyn}{class}{%
  \def\KVOdyn@name{#1}%
  \let\KVOdyn@ext C
}
\define@key{KVOdyn}{package}{%
  \def\KVOdyn@name{#1}%
  \let\KVOdyn@ext P
}

\newcommand*{\DisableKeyvalOption}[3][]{%
  \begingroup
    \setkeys{KVOdyn}{#1}%
    \def\x{\endgroup}%
    \@ifundefined{KVO@action@\KVOdyn@action}{%
      \PackageError{kvoptions}{%
        Unknown disable action %
        `\expandafter\strip@prefix\meaning\KVOdyn@action'\MessageBreak
        for option `#3' in keyval family '#2'%
      }\@ehc
    }{%
      \csname KVO@action@\KVOdyn@action\endcsname{#2}{#3}%
    }%
  \x
}

%% Disable -> undef
\def\KVO@action@undef#1#2{%
  \edef\x{\endgroup
    \ifKVOdyn@global\global\fi
    \let
    \expandafter\noexpand\csname KV@#1@#2\endcsname
    \relax
    \ifKVOdyn@global\global\fi
    \let
    \expandafter\noexpand\csname KV@#1@#2@default\endcsname
    \relax
  }%
}

\def\KVO@action@ignore#1#2{%
  \edef\x{\endgroup
    \ifKVOdyn@global\global\fi
    \let
    \expandafter\noexpand\csname KV@#1@#2\endcsname
    \noexpand\@gobble
    \ifKVOdyn@global\global\fi
    \let
    \expandafter\noexpand\csname KV@#1@#2@default\endcsname
    \noexpand\@empty
  }%
}

\def\KVO@action@error{%
  \KVO@do@action{error}%
}
\def\KVO@action@warning{%
  \KVO@do@action{warning}%
}


\def\KVO@do@action#1#2#3{%
  \ifx\KVOdyn@name\relax
    \PackageError{kvoptions}{%
      Action type `#1' needs package/class name\MessageBreak
      for key `#3' in family `#2'%
    }\@ehc
  \else
    \edef\x{\endgroup
      \noexpand\define@key{#2}{#3}[]{%
        \expandafter\noexpand\csname KVO@disable@#1\endcsname
        {\KVOdyn@name}\noexpand\KVOdyn@ext{#3}%
      }%
      \ifKVOdyn@global
        \global\let
        \expandafter\noexpand\csname KV@#2@#3\endcsname
        \expandafter\noexpand\csname KV@#2@#3\endcsname
        \global\let
        \expandafter\noexpand\csname KV@#2@#3@default\endcsname
        \expandafter\noexpand\csname KV@#2@#3@default\endcsname
      \fi
    }%
  \fi
}
\def\KVO@disable@error#1#2#3{%
  \ifx#2 C
    \expandafter\ClassError
  \else
    \expandafter\PackageError
  \fi
  {#1}{%
    Option `#3' is given too late,\MessageBreak
    now the option is ignored%
  }\@ehc
}
\def\KVO@disable@warning#1#2#3{%
  \ifx#2\@clsextension
    \expandafter\ClassWarning
  \else
    \expandafter\PackageWarning
  \fi
  {#1}{%
    Option `#3' is already consumed\MessageBreak
    and has no effect%
  }%
}

\def\DeclareDefaultOption{\DeclareOption*}

\endinput

