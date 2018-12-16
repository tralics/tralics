%%% -*- latex -*-
\ProvidesPackage{listings}[2011/05/02 v1.1 Listings for Tralics]
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2011
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\ifx\setkeys\undefined \RequirePackage{xkeyval} \fi
% \lstset{a=b, c=d} defines \lst@value --
% calling \lst@value later on generates \setkeys{lst}{a=b,c=d}
\def\lstset#1{\def\lst@value{\setkeys{lst}{#1}}}



% lst's keys add corresponding attributes to current element
\define@key{lst}{language}{\XMLaddatt{language}{#1}}
\define@key{lst}{basicstyle}{\XMLaddatt{basicstyle}{#1}}
\define@key{lst}{keywordstyle}{\XMLaddatt{keywordstyle}{#1}}
\define@key{lst}{tabsize}{\XMLaddatt{tabsize}{#1}}
\define@key{lst}{framexleftmargin}{\XMLaddatt{framexleftmargin}{#1}}
\define@key{lst}{frame}{\XMLaddatt{frame}{#1}}
\define@key{lst}{rulesepcolor}{\XMLaddatt{rulesepcolor}{#1}}
\define@key{lst}{numbers}{\XMLaddatt{numbers}{#1}}
\define@key{lst}{breaklines}{\XMLaddatt{breaklines}{#1}}


% lstlisting is a verbatim environment with a preamble named lst
\ifx \lst@verbatim\undefined 
\DefineVerbatimEnvironment{lstlisting}{Verbatim}{pre=ls, style=lst}% old behaviour
\else
\DefineVerbatimEnvironment{lstlisting}{lst@verbatim}{}
\fi


% Redefiniion of some commands, to be used insed a group
\def\lst@patch{%
    \def\color##1{##1}%
    \def\normalsize{normalsize}%
    \def\bfseries{bfseries}}
\def\lst@auxkeys{\def\lst{lst}\expandafter\setkeys\expandafter\lst\expandafter{\lstlisting@hook}}
\def\FV@style@lst{\lst@patch \lst@value\lst@auxkeys}

% the lst preamble prepends the verbatim with the XML tag <lstlisting>,
% then adds the attributes coming from the lstset command;
% \color and \normalsize are redefined locally
\def\FV@pre@lst{\begin{xmlelement*}{listing}}

% the lst postamble appends the end tag to the verbatim
\def\FV@post@lst{\end{xmlelement*}}


% \listingfile includes an external file as a listing
% to acheive that, we read the file line by line
% the result is put in an <xmlelement> tag
\newread\listingfile
\def\lstinputlisting#1{%
  \begin{xmlelement*}{listing}{%
    \lst@patch \lst@value
    \openin\listingfile=#1
    \endlinechar=`\^^J
    \loop\unless\ifeof\listingfile
      \readline\listingfile to \listingline
      \listingline
    \repeat
    \closein\listingfile
  }
  \end{xmlelement*}
}



