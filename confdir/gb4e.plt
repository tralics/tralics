%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% $Id: gb4e.plt,v 2.3 2007/05/22 09:46:32 grimm Exp $

% this file may or may not work.



\gdef\gll#1\\ #2\\{% #1 = first line, #2 = second line
\xbox{gll}{%
  \xbox{line1}{#1}
  \xbox{line2}{#2}
}}

\gdef\glll#1\\ #2\\ #3\\{% #1 = first line, #2 = second line, #3 = third
\xbox{glll}{%
  \xbox{line1}{#1}
  \xbox{line2}{#2}
  \xbox{line3}{#3}
}}
\RequirePackage{cgloss4e}



%%%%%%%%%%%%%%%%%%%%%%%%%%%
% gb4e


\def\exewidth#1{\def\@exwidth{#1}} \exewidth{(234)}
\def\exe{\@ifnextchar [{\@exe}{\@exe[\@exwidth]}}
\def\exi#1{\item[#1]\@ifnextchar [{\@exj}{}}

\def\@exe[#1]{\ifnum \@xnumdepth >0%
                 \if@xrec\@exrecwarn\fi%
                 \if@noftnote\@exrecwarn\fi%
                 \@xnumdepth0\@listdepth0\@xrectrue%
                 \save@counters%
              \fi%
                 \advance\@xnumdepth \@ne \@@xsi%
                 \begin{list}{(\thexnumi)}%
                             {\usecounter{xnumi}\@subex{#1}{1em}%
                              \if@noftnote%
                                 \setcounter{xnumi}{\value{exx}}%
                              \else%
                                 \setcounter{xnumi}{\value{fnx}}%
                              \fi}}

\def\endexe{\if@noftnote\setcounter{exx}{\value{xnumi}}%
                   \else\setcounter{fnx}{\value{xnumi}}%
                        \reset@counters\@xrecfalse\fi\end{list}}



\newcounter{xnumi}
\newcounter{xnumii}
\newcounter{exx}
\newcounter{rxnumi}


\def\@exe[#1]{\begin{list}{\thexnumi}{\usecounter{xnumi}\setcounter{xnumi}{\value{exx}}}}
\def\endexe{\setcounter{exx}{\value{xnumi}}\end{list}}

\def\xlist{\begin{list}{\thexnumi.\alph{xnumii}}{\usecounter{xnumii}}}
\def\endxlist{\end{list}}
\def\xlista{\begin{list}{\thexnumi.\alph{xnumii}}{\usecounter{xnumii}}}
\def\endxlista{\end{list}}




\def\ex{\@ifnextchar [{\@ex}{\item}}
\def\@ex[#1]#2{\item\@exj[#1]{#2}}
\def\@exj[#1]#2{\@exjbg{#1} #2 \end{list}}
\def\@exjbg#1{\begin{list}{#1}{\@subex{\@jwidth}{.5ex}}\item}

