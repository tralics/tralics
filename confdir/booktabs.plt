% -*- latex -*-
\ProvidesPackage{booktabs}[2015/07/08 v1.0 Booktabs for Tralics]

\@ifundefined{@sptoken}{%
  \PackageError{booktabs}{This needs tralics  version 2.15.3 or  later}{} \endinput} {} 

%% cf booktex appendix D
\def\futurenonspacelet#1{\def\@BTcs{#1}%
   \afterassignment\@BTfnslone\let\nexttoken= }
\def\@BTfnslone{\expandafter\futurelet\@BTcs\@BTfnsltwo}
\def\@BTfnsltwo{\expandafter\ifx\@BTcs\@sptoken\let\next=\@BTfnslthree
   \else\let\next=\nexttoken\fi \next}
\def\@BTfnslthree{\afterassignment\@BTfnslone\let\next= }


\newdimen\heavyrulewidth
\newdimen\lightrulewidth
\newdimen\cmidrulewidth
\newdimen\belowrulesep
\newdimen\belowbottomsep
\newdimen\aboverulesep
\newdimen\abovetopsep
\newdimen\defaultaddspace 

\heavyrulewidth=.08em 
\lightrulewidth=.05em 
\cmidrulewidth=.03em 
\belowrulesep=.65ex
\belowbottomsep=0pt
\aboverulesep=.4ex
\abovetopsep=0pt

\def \@BTrule#1#2[#3]{\hlinee{}{#1}{#2}{#3}}
\def\toprule{\@ifnextchar[{\@BTrule \abovetopsep \belowrulesep}{\@BTrule \abovetopsep \belowrulesep[\heavyrulewidth]}}
\def\midrule{\@ifnextchar[{\@BTrule \aboverulesep \belowrulesep}{\@BTrule \aboverulesep \belowrulesep[\lightrulewidth]}}
\def\bottomrule{\@ifnextchar[{\@BTrule \aboverulesep \belowbottomsep}{\@BTrule \aboverulesep \belowbottomsep[\heavyrulewidth]}}
\def\specialrule#1#2#3{\@BTrule{#2}{#3}[#1]}

\def\cmidrule{\@ifnextchar[\@BTcmidrule{\@BTcmidrule[\cmidrulewidth]}}
\def \@BTcmidrule[#1](#2)#3{\hlinee{#3}\aboverulesep \belowrulesep{#1}}

% If the argument is x, this should be \\[x] except after a \\
\def\addlinespace{\@ifnextchar[{\BT@addspace}{\BT@addspace[\defaultaddspace]}}
\def \BT@addspace[#1]{}

\def\morecmidrules{\\}




\endinput
