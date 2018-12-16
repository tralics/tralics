%%%  -*- latex -*-
\ProvidesPackage{float}[2009/01/03 v1.0 Float enhancements (AL)]

%% $Id: float.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Original latex file
%% copyright (c) 1991-2000 Anselm Lingnau
%% (lingnau@tm.informatik.uni-frankfurt.de)
%% 

\newcommand\float@error[1]{\PackageError{float}{%
  Unknown float style `#1'}{}}

\let\@caption\relax
\let\float@caption\relax

\newcommand\fs@plain{\XMLaddatt{style}{plain}}
\newcommand\fs@plaintop{\XMLaddatt{style}{plaintop}}
\newcommand\fs@ruled{\XMLaddatt{style}{ruled}}
\newcommand\fs@boxed{\XMLaddatt{style}{boxed}}

\newcounter{float@type}

\newtoks\float@exts
\newtoks\@float@everytoks


\newcommand\floatstyle[1]{\@ifundefined{fs@#1}%
  {\float@error{#1}}%
  {\def\float@style{#1}}}

% default style
\floatstyle{plain}

\newcommand\restylefloat{\@ifstar\float@restyle@\float@restyle}

\newcommand\float@restyle@[1]{\float@restyle{#1}%
  \expandafter\let\csname @float@c@#1\endcsname=\@caption}

\newcommand\float@restyle[1]{%
  \expandafter\edef\csname fst@#1\endcsname{\expandafter\noexpand\csname
    fs@\float@style\endcsname}%
  \@namedef{#1\expandafter}{\@float{#1}}%
  \@namedef{#1*\expandafter}{\@dblfloat{#1}}%
  \expandafter\let\csname end#1\endcsname\float@end
  \expandafter\let\csname end#1*\endcsname\float@dblend
  \expandafter\let\csname @float@c@#1\endcsname=\float@caption
  \@ifundefined{@float@every@#1}{%
    \expandafter\newtoks\csname @float@every@#1\endcsname}{}%
  \@nameuse{@float@every@#1}={}}

\newcommand\floatname[2]{\@namedef{fname@#1}{#2}}

\newcommand\floatplacement[2]{\@namedef{fps@#1}{#2}}

\newcommand\floatevery[2]{\csname @float@every@#1\endcsname={#2}}

\newcommand*{\listof}[2]{%
  \xbox{listof}{\XMLaddatt{type}{#1}#2}}

\newcommand\newfloat[3]{\@namedef{ext@#1}{#3}
  \let\float@do=\relax
  \xdef\@tempa{\noexpand\float@exts{\the\float@exts \float@do{#3}}}%
  \@tempa
  \floatplacement{#1}{#2}%
  \@ifundefined{fname@#1}{\floatname{#1}{#1}}{}
  %% Changed from original code 
  \addtocounter{float@type}{1}
  \expandafter\edef\csname ftype@#1\endcsname{\the\value{float@type}}%
  \restylefloat{#1}%
  \expandafter\edef\csname fnum@#1\endcsname%
    {\expandafter\noexpand\csname fname@#1\endcsname{}
       \expandafter\noexpand\csname the#1\endcsname}
  \@ifnextchar[{\float@newx{#1}}%
    {\@ifundefined{c@#1}{\newcounter{#1}\@namedef{the#1}{\arabic{#1}}}%
      {}}}

% Case where a counter is given
\def\float@newx#1[#2]{\@ifundefined{c@#1}{\newcounter{#1}[#2]%
    \expandafter\edef\csname the#1\endcsname{%
      \expandafter\noexpand\csname
        the#2\endcsname.\noexpand\arabic{#1}}}%
    {\PackageWarning{float}{Can't redefine counter variable for #1.}}}

\newfloat{figure}{tbp}{lof}
\newfloat{table}{tbp}{loft}
\float@exts={}
\endinput
