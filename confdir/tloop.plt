%%%  -*- latex -*-
\ProvidesPackage{tloop}[2007/12/16 v1.0  Tralics loops]


%% $Id: tloop.plt,v 2.1 2007/12/19 08:38:31 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% \tloop{nb1-nb2}{code}
%% executes code with \tloop@ctr ranging from nb1 to nb2.
%% code is executed at least once;
%% First argument is fully expanded; if no dash given in first argument, then 
%% nb2=0 (code executed once).

\newcount\tloop@ctr
\newcount\tloop@max
\newcount\Tloop@ctr
\newcount\Tloop@max

\def\tloop@get#1-#2;{\def\tloop@A{#1}\def\tloop@B{#2}}
\def\tloop#1#2{%
  \edef\tmp{#1}%
  \expandafter\tloop@get\tmp-;%
  \setbox0\hbox{\global\tloop@ctr=0\tloop@A\relax}%
  \setbox0\hbox{\global\tloop@max=0\tloop@B\relax}%
  \loop #2%
    \ifnum\tloop@ctr<\tloop@max%
    \advance\tloop@ctr1 %
  \repeat
}

\def\Tloop#1#2{%
  \edef\tmp{#1}%
  \expandafter\tloop@get\tmp-;%
  \setbox0\hbox{\global\Tloop@ctr=0\tloop@A\relax}%
  \setbox0\hbox{\global\Tloop@max=0\tloop@B\relax}%
  \loop #2%
    \ifnum\Tloop@ctr<\Tloop@max%
    \advance\Tloop@ctr1 %
  \repeat
}