%%  -*- latex -*-

% This file is part of Tralics
%% Copyright (C) INRIA/Apics (Jose' Grimm) 2007
%% $Id: overword.plt,v 2.1 2008/02/19 16:21:17 grimm Exp $
%% Copyright (C) 1996, 1997 Frank Bennett, Jr. for the original tex file

\ProvidesPackage{overword}[2007/12/29 v1.0  overword package for Tralics]


\def\:{\let\@sptoken= } \:  % this makes \@sptoken a space token

\long\def\@ifoverword#1#2#3#4{%
  \let\reserved@a=#1%
  \def\reserved@d{#4}\def\reserved@b{#2}\def\reserved@c{#3}%
  \global\let\@overword\relax%
  \futurelet\@let@token\@ifowd}

\long\def\@ifowd{%
  \ifx\@let@token\@sptoken%
    \gdef\@preoverwordspace{ }%
    \let\reserved@e\@xifowd%
  \else%
    \ifx\@let@token\end%
      \let\reserved@e\reserved@d%
    \else%
      \ifx\@let@token\par%
        \let\reserved@e\@xifpargobble%
      \else%
        \long\def\reserved@e##1 {\long\gdef\@overword{##1}\@@ifowd}%
      \fi%
    \fi%
  \fi%
  \reserved@e}

\long\def\@xifpargobble#1{\futurelet\@let@token\@ifowd}
\def\:{\@xifowd} \expandafter\def\: {\futurelet\@let@token\@ifowd}
\def\@@ifowd{\futurelet\@let@token\@ifnparnch}
\long\def\@ifnextnparchar#1#2#3#4{%
  \let\reserved@a=#1%
  \def\reserved@b{#2}\def\reserved@c{#3}\def\reserved@d{#4}%
  \futurelet\@let@token\@ifnparnch}
\long\def\@ifnparnch{%
  \ifx\@let@token\@sptoken%
    \let\reserved@e\@xifnparnch%
  \else%
    \ifx\@let@token\end%
      \let\reserved@e\reserved@d%
    \else%
      \ifx\@let@token\par%
        \let\reserved@e\@xifpargobblench%
      \else%
        \ifx\@let@token\reserved@a%
          \let\reserved@e\reserved@b%
        \else%
          \let\reserved@e\reserved@c%
        \fi%
      \fi%
    \fi%
  \fi%
  \reserved@e}
\long\def\@xifpargobblench#1{\futurelet\@let@token\@ifnparnch}
\def\:{\@xifnparnch}
\expandafter\def\: {\futurelet\@let@token\@ifnparnch}
\endinput

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Start of testfile
\documentclass{minimal}
\show\:
\usepackage{overword}

\begin{document}

\show\:
\makeatletter
\show\@sptoken
\message{foo}\message{bar}


\newtoks\registerone
\newtoks\registertwo
\def\list{}

\def\stashonright{%
  \registerone=\expandafter{\@overword}
  \registertwo=\expandafter{\list}
  \edef\list{%
    \noexpand\\{\the\registerone}\the\registertwo}}

\def\showlist{%
  \def\\##1{\message{##1}}
  \message{\space\space -> }
  \list
  \message{^^J}}

\def\readlistagain{%
  \def\\##1{ ##1 }
  \edef\listtext{\list}
\message{\listtext}
  \expandafter\store\listtext\enditall}

\def\storeandshow{%
  \@ifoverword\enditall
   {\message{Ran into \string\enditall!^^J}
    \stashonright
    \expandafter\showlist\@gobble}
   {\stashonright
    \storeandshow}
   {\message{Ran into \string\end!^^J}
    \stashonright
    \showlist}}

\message{^^J----------------------------------------------------------------------}
\message{This file demonstrates the operation of the Overword package for LaTeX}
\message{Ordinarily, TeX reads and inteprets printable characters one by one.}
\message{With this package, it is possible to read input word by word.  This}
\message{capability makes it possible to design user interfaces which are more}
\message{intuitive to users.  The Calendar bundle is one example of such an}
\message{interface.  Below, a phrase is printed to the terminal twice, once in}
\message{reverse word order, and once in re-reversed (normal) word order.  Each}
\message{message is triggered by a control sequence found just beyond the last}
\message{word read --- \string\enditall in the first instance, \string\end\space in the second.}
\message{This file produces no printed output.}
\message{^^J----------------------------------------------------------------------}



\storeandshow This is a demonstration of the overword package \enditall

\let\listclone\list
\def\list{}
\def\\#1{ #1 }
\edef\listtext{\listclone}
\expandafter\storeandshow\listtext


\end{document}

%%% Local Variables: 
%%% mode: latex
%%% TeX-master: t
%%% End: 
