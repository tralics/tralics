% -*- latex -*-
% remreset package
%%%%%%%%%%%%%%%%%%
%$Id: remreset.plt,v 2.2 2008/04/01 17:02:06 grimm Exp $

% Copyright 1997 David carlisle
\def\@removefromreset#1#2{{%
  \expandafter\let\csname c@#1\endcsname\@removefromreset
  \def\@elt##1{%
    \expandafter\ifx\csname c@##1\endcsname\@removefromreset
    \else
      \noexpand\@elt{##1}%
    \fi}%
  \expandafter\xdef\csname cl@#2\endcsname{%
    \csname cl@#2\endcsname}}}


