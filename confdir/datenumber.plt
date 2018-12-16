%%% -*- latex -*-
\ProvidesPackage{datenumber} [2008/03/25 1.0 Dates]

% $Id: datenumber.plt,v 2.2 2011/05/02 08:52:34 grimm Exp $
%% Copyright (C) 2001 J"org-Michael Schr"oder.
%% 
%% IMPORTANT NOTICE:
%% 
%% This program may be distributed and/or modified under the
%% conditions of the LaTeX Project Public License, either version 1.2
%% of this license or (at your option) any later version.
%% The latest version of this license is in
%% http://www.latex-project.org/lppl.txt
%% and version 1.2 or later is part of all distributions of LaTeX
%% version 1999/12/01 or later.

%% Options are language names
\DeclareOption{french}{\@datelanguagefrench}
\DeclareOption{spanish}{\@datelanguagespanish}
\DeclareOption{german}{\@datelanguagegerman}
\DeclareOption{ngerman}{\@datelanguagegerman}
\DeclareOption{english}{\@datelanguageenglish}
\DeclareOption{USenglish}{\@datelanguageUSenglish}

%% Each option FOO defines \@datelanguageFOO
%% that defines 3 macros, for the day, the month, the date

% Maybe using \let here would be more efficient
\def\@datelanguagefrench{%
  \def\@dayname{\@daynamefrench}%
  \def\@monthname{\@monthnamefrench}%
  \def\@datedate{\@datedatefrench}%
}

\def\@datelanguagespanish{%
  \def\@dayname{\@daynamespanish}%
  \def\@monthname{\@monthnamespanish}%
  \def\@datedate{\@datedatespanish}%
}

\def\@datelanguagegerman{%
  \def\@dayname{\@daynamegerman}%
  \def\@monthname{\@monthnamegerman}%
  \def\@datedate{\@datedategerman}%
}

\def\@datelanguageenglish{%
  \def\@dayname{\@daynameenglish}%
  \def\@monthname{\@monthnameenglish}%
  \def\@datedate{\@datedateenglish}%
}

\def\@datelanguageUSenglish{%
  \def\@dayname{\@daynameenglish}%
  \def\@monthname{\@monthnameenglish}%
  \def\@datedate{\@datedateUSenglish}%
}

% Make English the default
\@datelanguageenglish


% \@datedateLANG typesets current date
\def\@datedatespanish{\thedateday~de \datemonthname\ de~\the\year}
\def\@datedatefrench{\thedateday \ifnum1=\thedateday \/\ier\fi
    \space \datemonthname\space \number\year}
\def\@datedategerman{\thedateday.~\datemonthname\space\thedateyear}
\def\@datedateUSenglish{\datemonthname\space\thedateday, \thedateyear}

\def\@datedateenglish{\thedateday \ifcase\thedateday \or
  st\or nd\or rd\or th\or th\or th\or th\or th\or th\or th\or
  th\or th\or th\or th\or th\or th\or th\or th\or th\or th\or
  st\or nd\or rd\or th\or th\or th\or th\or th\or th\or th\or
  st\fi~\datemonthname \space\thedateyear}


%% Process options now
\ProcessOptions \relax

%% 
\def\setmonthname#1{%
  \def\datemonthname{\@monthname{#1}}}

\def\setdayname#1{%
  \def\datedayname{\@dayname{#1}}}


\def\dateselectlanguage#1{%
  \csname @datelanguage#1\endcsname
  \setdayname{\thedatedayname}%
  \setmonthname{\thedatemonth}%
}

\def\datedate{\@datedate}%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% This counter countains the epoch
%% Only dates after the epoch are valid; setting it too small
%% is a waste of time, too high may be wrong
\newcount\@startyear

%% sets the epoch and recomputes today date
\def\setstartyear#1{%
  \ifnum#1<1
    \errmessage{Package datenumber: startyear < 1}%
  \fi
  \@startyear=#1\relax
  \setdatetoday
}

%% This command returns the epoch
\def\thestartyear{\number\@startyear}

%% Four counters 
\newcounter{datenumber}
\newcounter{dateyear}
\newcounter{datemonth}
\newcounter{dateday}
\newcounter{datedayname}

%% day number is 1 for Monday etc
\def\setdaynamebynumber#1{%
  \@tempcnta=#1\relax
  \ifnum\@startyear=1800\advance \@tempcnta by 1\fi
  \ifnum\@startyear=1900\advance \@tempcnta by 6\fi
  \ifnum\@startyear=2000\advance \@tempcnta by 4\fi
  {%
    \count0 = \@tempcnta
    \divide\count0 by 7
    \multiply\count0 by 7
    \advance\@tempcnta by -\count0 %% between 0 and 6
    \advance\@tempcnta by 1
    \setcounter{datedayname}{\@tempcnta}% changes globally
  }%
  \setdayname{\thedatedayname}%
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Args: date year month day 
%% First argument must be a counter name, other arguments must be
%% integers
\def\set@datenumber#1#2#3#4{%
  \dates@checkdate{#2}{#3}{#4}%
  \ifnum\@startyear<1
    \errmessage{Package datenumber: startyear < 1}%
  \fi
  \ifnum#2<\@startyear
    \errmessage{Package datenumber: datenumber < startyear (#2 < \@startyear)}%
  \fi
  \dates@countdays{#1}{\@startyear}{#2}{#3}{#4}}

%% Args: date year month day 
%% First argument is a number
%% other arguments must be counter names
\def\set@datebynumber#1#2#3#4{%
  \ifnum\@startyear<1
    \errmessage{Package datenumber: startyear < 1}%
  \fi
  \ifnum#1<1
    \errmessage{Package datenumber: datenumber < 1}%
  \fi
  \dates@datebynumber \@startyear{#1}{#2}{#3}{#4}}

%% User function. computes everything from year month day
\def\setdatenumber#1#2#3{%
  \set@datenumber{datenumber}{#1}{#2}{#3}% fills datenumber
  \setcounter{dateyear}{#1}%
  \setcounter{datemonth}{#2}%
  \setcounter{dateday}{#3}%
  \setdaynamebynumber{\thedatenumber}%
  \setmonthname{\thedatemonth}}


\let\setdate\setdatenumber % an alias

%% sets date of today
\def\setdatetoday{%
  \setdatenumber{\the\year}{\the\month}{\the\day}}



%% User function. computes everything from number of says since epoch
\def\setdatebynumber#1{%
  \set@datebynumber{#1}{dateyear}{datemonth}{dateday}%
  \setcounter{datenumber}{#1}%
  \setdaynamebynumber{\thedatenumber}%
  \setmonthname{\thedatemonth}%
}

%% Computes the date of tomorrow or yesterday
\def\nextdate{%
 \dates@nextdate{dateyear}{datemonth}{dateday}%
 \setdate{\thedateyear}{\thedatemonth}{\thedateday}}

\def\prevdate{%
 \dates@prevdate{dateyear}{datemonth}{dateday}%
 \setdate{\thedateyear}{\thedatemonth}{\thedateday}}




\setstartyear{1800}

\endinput
%%

