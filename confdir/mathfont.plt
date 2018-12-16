% -*- latex -*-
\ProvidesPackage{mathfont}[2015/06/24 v1.1 Tralics Math font implementation]

% Version 1.1 assumes the char tables builtin 


\def\DeclareSymbolFont#1#2#3#4#5{%
 \edef\@tmp{\ifcsname #3-encoding\endcsname
     \expandafter\noexpand\csname#3-encoding\endcsname
   \else \ifcsname #2-encoding\endcsname
     \expandafter\noexpand\csname#2-encoding\endcsname
     \else \expandafter\noexpand\csname T1-encoding\endcsname \fi\fi}
 \wlog{Defining new mathalphabet #1=\expandafter\string\@tmp}
 \edef \@tmp{\let\expandafter\noexpand\csname#1-mathfont\endcsname
   \expandafter\noexpand\@tmp}\@tmp}

\def\mathalpha{}
\def\DeclareMathSymbol#1#2#3#4{\ifcsname#3-mathfont\endcsname
  \edef#1{#2\@nameuse{#3-mathfont}{#4}}\else \error{Unknown math alphabet #3}\fi}


\endinput