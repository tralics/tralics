%%% -*- latex -*-
\ProvidesPackage{coolstr}[2008/04/24 v1.0  COntent Oriented LaTeX Strings]
%% based on a package by nsetzer
%% This file may be distributed and/or modified under the
%% conditions of the Limited General Product License


%% $Id: coolstr.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% 
\NeedsTeXFormat{LaTeX2e}[1999/12/01]
\RequirePackage{ifthen}

\newcommand{\setstrEnd}[1]{} % useless
\newcount\COOL@str@index
\newcount\COOL@str@start
\newcount\COOL@str@end

%%%%%%% testing types of numbers etc
\newboolean{COOL@decimalfound}
\newboolean{COOL@decimal}

%% Set a user boolean from \ifCOOL@decimal
\def\COOL@set@bool#1{%
  \ifCOOL@decimal\setboolean{#1}{true}%
  \else \setboolean{#1}{false}\fi}


%% Read all chars until finding \@nil; increment \@tempcntb
%% Emit #1 if counter has the same value as \@tempcnta
\def\COOL@strgobble#1{%
  \ifx\@nil#1% done
  \else
  \advance\@tempcntb by 1
  \ifnum \@tempcntb=\@tempcnta #1\fi
  \expandafter\COOL@strgobble\fi
}

%% Read all chars until finding \@nil; increment \@tempcntb
%% Emit #1 if counter is between \COOL@str@start and \COOL@str@end
\def\COOL@substrgobbler#1{%
  \ifx\@nil#1\else
  \advance\@tempcntb by 1
  \ifnum\@tempcntb < \COOL@str@start
  \else
    \ifnum\@tempcntb > \COOL@str@end
     \else #1\fi\fi
  \expandafter\COOL@substrgobbler\fi
}

%% Read all chars until finding \@nil; increment \@tempcntb
%% Set boolean to true if position is \@tempcnta and char is #1
\def\COOL@strcomparegobble#1#2{%
 \ifx#2\@nil
 \else
    \advance\@tempcntb by1
    \ifnum\@tempcnta=\@tempcntb 
      \ifx #1#2\@tempswatrue\fi
    \fi
    \expandafter\COOL@strcomparegobble\expandafter#1\fi
}

%% Emit char at position #2 in #1 if index positive; set size in \@tempcntb
\newcommand{\strchar}[2]{%
  \@tempcntb=\z@
  \@tempcnta=#2\relax
  \COOL@strgobble#1\@nil}

%% set size in \@tempcntb
\newcommand{\strlen}[1]{%
  \strchar{#1}{0}%
  \number\@tempcntb}

%% set size of #1 in #2
\newcommand{\strlenstore}[2]{%
  \strchar{#1}{0}%
  \setcounter{#2}{\@tempcntb}}

%%\ifstrchareq{str}{pos}[char}{true-code}{false-code}
%% Executes one of true-code false-code depending on whether character
%% at position pos if str is char.
\newcommand{\ifstrchareq}[3]{
 \@tempcntb=\z@
 \@tempcnta=#2
 \@tempswafalse
 \COOL@strcomparegobble#3#1\@nil
 \if@tempswa
     \expandafter\@firstoftwo
  \else
    \expandafter\@secondoftwo
  \fi}

%%\ifstrleneq{str}{len}{true-code}{false-code}
%% Executes one of true-code false-code depending on whether size of str is len
\newcommand{\ifstrleneq}[2]{%
  \strchar{#1}{0}%
  \ifnum\@tempcntb=#2\relax
   \expandafter\@firstoftwo
  \else
    \expandafter\@secondoftwo
  \fi}

%% let A=\@tempcntA, B=\@tempcntb; we set #1 into A, then COOL@str@index
%% B+A+1 if A<0 and to A to A >0
%% is size of string, we set starting index to #1 or B+#1
\def\subtr@checkstart#1{%
  \@tempcnta=#1\relax
  \ifnum\@tempcnta<\z@
    \ifnum\@tempcnta<-\@tempcntb
      \COOL@str@index=0 % too big
    \else
      \COOL@str@index=\@tempcntb
      \advance\COOL@str@index \@tempcnta
      \advance\COOL@str@index 1
    \fi
  \else 
    \ifnum\@tempcnta>\@tempcntb
       \COOL@str@index=\z@ %too big
    \else \COOL@str@index\@tempcnta\fi
  \fi
}

%% Argument is the number of characters
%% This sets \COOL@str@start and \COOL@str@end 
\def\subtr@checksize#1{%
  \def\@tempa{beg}%
  \def\@tempb{#1}%
  %% if len=beg, use [1,I]
  \ifx\@tempa\@tempb
    \COOL@str@start=1
    \COOL@str@end=\COOL@str@index
    \else
      \def\@tempa{end}%
       % len=end use [I,max]
      \ifx\@tempa\@tempb
      \COOL@str@start=\COOL@str@index
      \COOL@str@end=\@tempcntb
      \else \ifnum #1<\z@
        %% size negative [I+size+1,I]
        \COOL@str@start=\COOL@str@index
        \advance\COOL@str@start #1%
        \advance\COOL@str@start 1 
        \ifnum\COOL@str@start > 0\else\COOL@str@start=1\fi
        \COOL@str@end=\COOL@str@index
      \else \ifnum #1 > \z@
         %% normal case [I, I+len-1]
          \COOL@str@start=\COOL@str@index 
          \COOL@str@end=\COOL@str@index
          \advance\COOL@str@end #1\relax
          \advance\COOL@str@end by -1
          \ifnum\COOL@str@end > \@tempcntb \COOL@str@end\@tempcntb \fi
        \else
          \COOL@str@index=\z@
        \fi\fi\fi\fi}

%% Emits a particur substring of #1 (starting at #2 with #3 characters).
\newcommand{\substr}[3]{%
  \strchar{#1}{0}%
  \subtr@checkstart{#2}%
  \subtr@checksize{#3}%
  \ifnum \COOL@str@index>\z@ \@afterfi
    \@tempcntb=0
    \COOL@substrgobbler#1\@nil%
  \fi
}



%% Checking numbers 
%% Skip initial sign
\def\COOL@decimalgobbler#1{%
  \ifx\@nil#1\let\next\COOL@decimalfalse
  \else \ifnum`#1=`+ \let\next\COOL@decimalgobblerA
  \else \ifnum`#1=`- \let\next\COOL@decimalgobblerA
  \else \def\next{\COOL@decimalgobblerA#1}\fi\fi\fi
  \next
}

%% We have read initial sign, must check for a single dot
\def\COOL@decimalgobblerA#1{%
   \ifx\@nil#1\let\next\COOL@decimalfalse
   \else \ifnum`#1=`. \let\next\COOL@decimalgobblerB
   \else \def\next{\COOL@decimalgobblerC#1}\fi\fi\next}

%% We have see optional sign and dot
\def\COOL@decimalgobblerB#1{%
  \ifx\@nil#1\let\next\COOL@decimalfalse 
  \else \COOL@decimalfoundtrue\let\next\COOL@decimalgobblerC\fi\next}

%% Generic 
\def\COOL@decimalgobblerC#1{%
  \ifx\@nil#1%
  \else 
   \@tempswafalse % false char ok
   \ifnum`#1 < `0 \@tempswatrue \fi
   \ifnum `#1 > `9 \@tempswatrue \fi
   \ifnum `#1 = `. 
      \ifCOOL@decimalfound \else \COOL@decimalfoundtrue\@tempswafalse \fi
   \fi
   \if@tempswa\COOL@decimalfalse\fi
   \expandafter\COOL@decimalgobblerC\fi}

%% Sets \COOL@decimal if #1 is a decimal number
\def\COOL@isdecimal#1{%
  \COOL@decimalfoundfalse
  \COOL@decimaltrue
  \expandafter\COOL@decimalgobbler#1\@nil}

%% Split AeB or AEB in two parts
% \COOL@num@magnitude and \COOL@num@exponent
\def\COOL@split@number#1{%
  \COOL@eparser#1e\@nil%
  \ifx\empty\COOL@num@exponent\empty
    \COOL@Eparser#1E\@nil%
    \ifx\empty\COOL@num@exponent\empty
      \def\COOL@num@exponent{0}%
    \else 
      \expandafter\COOL@Ecorrector\COOL@num@exponent%
    \fi
  \else
    \expandafter\COOL@ecorrector\COOL@num@exponent%
  \fi
}

\def\COOL@eparser#1e#2\@nil{%
  \def\COOL@num@magnitude{#1}%
  \def\COOL@num@exponent{#2}%
}
\def\COOL@ecorrector#1e{%
  \def\COOL@num@exponent{#1}%
}
\def\COOL@Eparser#1E#2\@nil{%
  \def\COOL@num@magnitude{#1}%
  \def\COOL@num@exponent{#2}%
}
\def\COOL@Ecorrector#1E{%
  \def\COOL@num@exponent{#1}%
}


%% Set \COOL@decimalfalse if arg not int
%% First check initial sign
\def\COOL@intgobblerA#1{%
  \ifx\@nil#1\let\next\COOL@decimalfalse
  \else \ifnum`#1=`+ \let\next\COOL@intgobblerB
  \else \ifnum`#1=`- \let\next\COOL@intgobblerB
  \else \def\next{\COOL@intgobblerB#1}\fi\fi\fi
  \next
}

%% Check if the number is empty
\def\COOL@intgobblerB#1{%
  \ifx\@nil#1\let\next\COOL@decimalfalse \fi
  \COOL@intgobbler#1}

\def\COOL@intgobbler#1{%
  \ifx\@nil#1%
  \else 
   \ifcat#11\else\COOL@decimalfalse\fi
   \@tempswafalse % false char ok
   \ifnum`#1 < `0 \@tempswatrue \fi
   \ifnum `#1 > `9 \@tempswatrue \fi
   \if@tempswa\COOL@decimalfalse\fi
   \expandafter\COOL@intgobbler\fi}




%% Sets a user boolean #2 if #1 is a decimal number
\newcommand{\isdecimal}[2]{%
  \COOL@isdecimal{#1}%
  \COOL@set@bool{#2}}

%% Sets a user boolean #2 if #1 is integer
\newcommand{\isint}[2]{%
  \COOL@decimaltrue
  \expandafter\COOL@intgobblerA#1\@nil%
  \COOL@set@bool{#2}}

%% Sets a user boolean #2 if #1 is a number
\newcommand{\isnumeric}[2]{%
  \COOL@split@number{#1}%
  \COOL@isdecimal{\COOL@num@magnitude}%
  \ifCOOL@decimal\expandafter\COOL@intgobblerA\COOL@num@exponent\@nil\fi
  \COOL@set@bool{#2}}

\endinput
