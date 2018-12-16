%%  -*- latex -*-
\ProvidesPackage{ra}[2013/07/22 v1.9 Utilities for INRIA's activity report]
\RequirePackage{xkeyval}
% This file is part of Tralics

%% This software is governed by the CeCILL license under French law and
%% abiding by the rules of distribution of free software.  You can  use, 
%% modify and/ or redistribute the software under the terms of the CeCILL
%% license as circulated by CEA, CNRS and INRIA at the following URL
%% "http:%%www.cecill.info". 
%% (See the file COPYING in the main directory for details)
% copyright (C) INRIA/apics/marelle (Jose' Grimm) 2008-2013

%% Note the condition is false by default
\newif\ifra@emptymodule %% true if empty module names allowed
\newif\ifra@catperso %% true if catperso is defined
\newif\ifra@participant %% true if participant is different from participants
\newif\ifra@old %% true if compiling ra before 2006
\newif\ifra@composition %% true if env composition is defined
\newif\ifra@moduleref %% true if moduleref is defined
\newif\ifra@topic %% true if topics can be used
\newif\ifra@topics %% true if topics are effectively used
\newif\ifra@noaff %% true if affiliations in \pers

\newif\ifra@inmodule % true inside a module
\newif\ifra@firstsection\ra@firstsectiontrue % true if in first section
\def\@allmodules{,} % The list of module names
\newcounter{modules}
\newtoks\ra@topics % topics declarations

\let\glo\@glo % needed in Tralics 2.13.3
\let\glossaire\@glossaire
\let\endglossaire\end@glossaire
\DeclareOption{catperso}{\ra@catpersotrue}
\DeclareOption{participant}{\ra@participanttrue}
\DeclareOption{co-publiants}{\ra@copubliantstrue}
\DeclareOption{emptymodule}{\ra@emptymoduletrue}
\DeclareOption{old}{\ra@oldtrue}
\DeclareOption{composition}{\ra@compositiontrue}
\DeclareOption{moduleref}{\ra@modulereftrue}
\DeclareOption{topic}{\ra@topictrue}
\DeclareOption{noaff}{\ra@noafftrue} % look for \ifra@noaff below

\ProcessOptions\relax

%--------------------------------------------------


% Following commands store their arguments somewhere to be used later.
\def\theme#1{\def\ra@theme{#1}}
\def\UR#1{\def\ra@UR{#1}}
\def\isproject#1{\def\ra@isproject{\setkeys{raF}{ptype=#1}}}
\def\projet#1#2#3{\def\ra@proj@a{#1}\def\ra@proj@b{#2}\def\ra@proj@c{#3}}
\let\project\projet


%% Conditionally define the catperso environment
%% There are optional constraints on the argument.
\ifra@catperso
\newenvironment{catperso}[1]{%
  \begin{xmlelement*}{catperso}\let\par\empty
    \edef\@tmp{\tralics@find@config{catperso}}%
    \xbox{head}{\ifx\@tmp\empty#1\else\tralics@get@config{catperso}{#1}\fi}%
    \@addnl}
  {\end{xmlelement*}\@addnl}
\fi

%% environment participants defines \pers to be \persA
\newenvironment{participants}{%
  \begin{xmlelement*}{participants}\let\par\empty\let\pers\persA}
  {\end{xmlelement*}\@addnl}

%% environment co-publiants defines \pays
\newenvironment{co-publiants}{%
  \begin{xmlelement*}{co-publiants}\let\par\empty\let\pays\@pays}
  {\end{xmlelement*}\@addnl}

%% Alternate names are conditionnally defined.
\ifra@participant
\newenvironment{participante}{%
  \begin{xmlelement*}{participante}\let\par\empty\let\pers\persA}
  {\end{xmlelement*}\@addnl}

\newenvironment{participant}{%
  \begin{xmlelement*}{participant}\let\par\empty\let\pers\persA}
  {\end{xmlelement*}\@addnl}

\newenvironment{participantes}{%
  \begin{xmlelement*}{participantes}\let\par\empty\let\pers\persA}
  {\end{xmlelement*}\@addnl}
\else
\let\participante\participants
\let\participant\participants
\let\participantes\participants
\let\endparticipante\endparticipants
\let\endparticipant\endparticipants
\let\endparticipantes\endparticipants
\fi

% #1=list name, #2=key
\def\tralics@use@config#1#2{%
  \edef\@tmp{\tralics@find@config{#1}}%
  \ifx\@tmp\empty#2\else\tralics@get@config{#1}{#2}\fi}


%% 
\def\remove@fl@space@#1#2{%
\expandafter\def\expandafter #1\expandafter {\zap@fl@space{#2}}}

\def\remove@fl@space#1{%
  \expandafter\remove@fl@space@\expandafter#1\expandafter{#1}}

\def\@pays#1#2{\xbox{pays}{\XMLaddatt{court}{#2}\XMLaddatt{long}{#1}}%
\@addnl\@ifnextchar,\@gobble\empty}

% Hack applied to \persA and \persB
\def\pers@hack#1#2#3{%
  \def\t@pnom{#1}%
  \def\t@nom{#2}%
  \def\t@aux{#3}%
  \tralics@fnhack\t@nom\t@aux
  \remove@fl@space\t@aux
  \remove@fl@space\t@pnom
  \remove@fl@space\t@nom}


%% This is \persA, the simple command
\def\persA#1{\@ifnextchar[{\persA@part{#1}}{\persA@nom{#1}}}
\def\persA@part#1[#2]#3{\persA@nom{#1}{#2 #3}}
\def\persA@nom#1#2{\@ifnextchar[{\persA@opt{#1}{#2}}{\persA@opt{#1}{#2}[]}}

\def\persA@opt#1#2[#3]{%
  \pers@hack{#1}{#2}{#3}%
  \@persA{\t@pnom}{\t@nom}{\t@aux}}


\def\@persA#1#2#3{\xbox{pers}{\XMLaddatt{nom}{#2}\XMLaddatt{prenom}{#1}#3}%
  \@addnl\@ifnextchar,\@gobble\empty}


%% This is \persB the complicated command with affiliation
\def\persB#1{\@ifnextchar[{\persB@part{#1}}{\persB@nom{#1}}}
\def\persB@part#1[#2]#3{\persB@nom{#1}{#2 #3}}
\def\persB@nom#1#2{\@ifnextchar[{\persB@rc{#1}{#2}}{\persB@rc{#1}{#2}[]}}
\def\persB@rc#1#2[#3]#4#5{\@ifnextchar[{\persB@aux{#1}{#2}{#3}{#4}{#5}}
  {\persB@aux{#1}{#2}{#3}{#4}{#5}[]}}
\def\persB@aux#1#2#3#4#5[#6]{\@ifnextchar[{\persB@hdr{#1}{#2}{#3}{#4}{#5}{#6}}
  {\persB@hdr{#1}{#2}{#3}{#4}{#5}{#6}[]}}

\def\persB@hdr#1#2#3#4#5#6[#7]{%
  \pers@hack{#1}{#2}{#6}%
  \def\t@rc{#3}\remove@fl@space\t@rc
  \def\t@catpro{#4}\remove@fl@space\t@catpro
  \def\t@orga{#5}\remove@fl@space\t@orga
  \def\t@hdr{#7}\remove@fl@space\t@hdr
  \ifx\t@rc\empty\def\tmp{}\else\def\tmp{[\t@rc]}\fi
  \expandafter\@persB\tmp{\t@pnom}{\t@nom}{\t@catpro}{\t@orga}{\t@aux}{\t@hdr}}

\newcommand\@persB[7][]{%
  % Make sure error token are outside the xbox
  \edef\@tmp{\tralics@find@config{profession}}%
  \edef\t@pro{\ifx\@tmp\empty#4\else\tralics@get@config{profession}{#4}\fi}%
  \edef\@tmp{\tralics@find@config{affiliation}}%
  \edef\t@aff{\ifx\@tmp\empty#5\else\tralics@get@config{affiliation}{#5}\fi}%
  \edef\@tmp{\tralics@find@config{ur}}%
  \ifnum\ra@year>2006 
  \edef\t@rc{\ifx\@tmp\empty#1\else\tralics@get@config{ur}{#1}\fi}%
  \else\let\t@rc\empty\fi
  \xbox{pers}{%
    \unless\ifx\t@rc\empty\XMLaddatt{research-centre}{\t@rc}\fi
    \edef\tmp{#7}\unless\ifx\tmp\empty\XMLaddatt{hdr}{#7}\fi
    \XMLaddatt{profession}{\t@pro}%
    \XMLaddatt{affiliation}{\t@aff}%
    \XMLaddatt{nom}{#3}\XMLaddatt{prenom}{#2}%
    #6}\@addnl\@ifnextchar,\@gobble\empty}



%% This is \persC, like \persB, without affiliation
\def\persC#1{\@ifnextchar[{\persC@part{#1}}{\persC@nom{#1}}}
\def\persC@part#1[#2]#3{\persC@nom{#1}{#2 #3}}
\def\persC@nom#1#2{\@ifnextchar[{\persC@rc{#1}{#2}}{\persC@rc{#1}{#2}[]}}
\def\persC@rc#1#2[#3]#4{\@ifnextchar[{\persC@aux{#1}{#2}{#3}{#4}}
  {\persC@aux{#1}{#2}{#3}{#4}[]}}
\def\persC@aux#1#2#3#4[#5]{\@ifnextchar[{\persC@hdr{#1}{#2}{#3}{#4}{#5}}
  {\persC@hdr{#1}{#2}{#3}{#4}{#5}[]}}

\def\persC@hdr#1#2#3#4#5[#6]{%
  \pers@hack{#1}{#2}{#5}%
  \def\t@rc{#3}\remove@fl@space\t@rc
  \def\t@catpro{#4}\remove@fl@space\t@catpro
  \def\t@hdr{#6}\remove@fl@space\t@hdr
  \ifx\t@rc\empty\def\tmp{}\else\def\tmp{[\t@rc]}\fi
  \expandafter\@persC\tmp{\t@pnom}{\t@nom}{\t@catpro}{\t@aux}{\t@hdr}}

\newcommand\@persC[6][]{%
  % Make sure error token are outside the xbox
  \edef\@tmp{\tralics@find@config{profession}}%
  \edef\t@pro{\ifx\@tmp\empty#4\else\tralics@get@config{profession}{#4}\fi}%
  \edef\@tmp{\tralics@find@config{ur}}%
  \ifnum\ra@year>2006 
  \edef\t@rc{\ifx\@tmp\empty#1\else\tralics@get@config{ur}{#1}\fi}%
  \else\let\t@rc\empty\fi
  \xbox{pers}{%
    \unless\ifx\t@rc\empty\XMLaddatt{research-centre}{\t@rc}\fi
    \edef\tmp{#6}\unless\ifx\tmp\empty\XMLaddatt{hdr}{#6}\fi
    \XMLaddatt{profession}{\t@pro}%
    \XMLaddatt{nom}{#3}\XMLaddatt{prenom}{#2}%
    #5}\@addnl\@ifnextchar,\@gobble\empty}

%% Redefines  \persB to \persC for ra2013 and following.
\ifra@noaff \let\persB\persC \fi


\newenvironment{moreinfo}{\begin{xmlelement*}{moreinfo}}
  {\end{xmlelement*}\@addnl}
  
\newcommand\bestcite[1]{\xbox{best}{\cite{#1}}}

%% obsolete in 2007
\def\declaretopic#1#2{}

\newenvironment{module}{\@start@module}{\tralics@pop@module}

% first optional argument #1=topic ignored; #2=section, #3=aux, #4=title
\newcommand\@start@module[4][]{%
  \ifra@inmodule\PackageError{Raweb}{Nested modules are illegal}{}\fi
  \ra@inmoduletrue
  \edef\@tmp{\tralics@get@config{fullsection}{#2}}%
  \ifx\@tmp\empty\else
    \ifra@firstsection\else \tralics@pop@section\fi
     \global\ra@firstsectionfalse
     \typeout{Translating section #2}%
     \tralics@push@section{#2}
     \ifnum\ra@year>2006 \XMLaddatt{titre}{\@tmp}\fi
  \fi
  \refstepcounter{modules}%
  \edef\foo{\noexpand\in@{,#3,}{\@allmodules}}\foo
  \ifin@ \ClassError{Raweb}{Duplicate module: #3}{}\else
  \xdef\@allmodules{,#3\@allmodules}\fi
  \ifra@emptymodule
  \tralics@push@module{#3}{#4}%
  \else
  \tralics@push@module{#3}{\@ifbempty{#4}{(Sans Titre)}{#4}}%
  \fi
  \ifra@topic\XMLaddatt{html}{module\themodules}\fi
  \ifra@topics\edef\t@topic{\tralics@get@config{section}{#1}}
  \unless\ifx\t@topic\empty\XMLaddatt{topic}{\@nameuse{ra@topicval@\t@topic}}\fi
  \fi}

% We use keyval to check the validity of the project type.
\define@choicekey*+{raF}{ptype}[\val\nr]{non,no,oui,yes,project,project team,team,action}{%
  \edef\ra@ptype {%
  \ifcase \nr false% case  non
  \or false% case no
  \or true%  case oui
  \or true% case yes
  \or true% case project
  \or true% case project team
  \or false% case team
  \or explo% case action
  \fi}}{\PackageError{raweb}{Invalid Team type #1 ignored}}

\def\ra@check@project{%
  \ifx\ra@proj@a\relax \PackageError{Raweb}{Missing \string \project}{}\fi
  \def\tmpA##1{\lowercase{\xdef\tmpA{\detokenize{##1}}}}
  \expandafter\tmpA\expandafter{\ra@proj@a}
  \ifx\tmpA\tmpB \else\PackageError{Raweb}{Invalid Team name \ra@proj@a}{}\fi
  \xbox{projet}{\ifx\ra@proj@b\empty \tmpB\else\ra@proj@b\fi}%
}

\def\rawebstartdocument{%
  \@addnl
  \begin{xmlelement*}{accueil}%
    \edef\tmpB{\ra@jobname}
    \XMLaddatt{html}{\ra@jobname}%
    \def\ra@ptype{erreur}\ra@isproject\XMLaddatt{isproject}{\ra@ptype}\@addnl
    \xbox{theme}{\ifnum\ra@year>2008 Dummy%
      \else\tralics@get@config{theme}{\ra@theme}\fi} \@addnl
    \ra@check@project\@addnl
    \xbox{projetdeveloppe}{\ra@proj@c}\@addnl
    \expandafter\tralics@interpret@rc\expandafter{\ra@UR}\@addnl
    \the\ra@topics
  \end{xmlelement*}\@addnl
}

\let\ra@proj@a\relax
\let\pers\persB
\let\pays\relax

% The documentation says to use these commands, so let's define them
\let\maketitle\relax
\let\loadbiblio\relax
\let\keywords\motscle

%% --------------------------------------------------
\ifra@moduleref
% syntax \moduleref[yr]{p}{s}{a}
\newcommand\moduleref[4][]{%
  \def\@tmp{#1}%
  \unless\ifx\@tmp\empty\edef\@tmpA{\ra@year}\ifx\@tmp\@tmpA\let\@tmp\empty\fi\fi
  \ifx\@tmp\empty
  \@iftempty{#4}{\ref{section:#3}}{\ref{mod:#4}}%
  \else \PackageError{Raweb}{\string \moduleref[#1] is not implemented}{}\fi}

\else
\def\moduleref#1#2#3{\ref{mod:#3}}

\fi

%% --------------------------------------------------
\ifra@old

\let\pers\persA
\fi

%% --------------------------------------------------
\ifra@composition

\let\pers\undefined
\newenvironment{composition}
{\let\pers\persB
  \ifra@inmodule\PackageError{Raweb}{Composition forbidden in Module}{}\fi
  \unless\ifra@firstsection
    \PackageError{Raweb}{Composition must be before first module}{}
    \tralics@pop@section
  \fi
  \edef\@tmp{\tralics@get@config{fullsection}{composition}}%
  \ifx\@tmp\empty\else
  \ifra@firstsection\else \tralics@pop@section\fi
  \global\ra@firstsectionfalse
  \typeout{Translating composition}%
  \tralics@push@section{composition}%
  \XMLaddatt{titre}{\@tmp}\@addnl%
  \fi
}{}


\fi
%% --------------------------------------------------

\def\declaretopic#1#2{%
\xbox{topic}{\XMLaddatt{num}{#1}\xbox{t\_titre}{#2}}\@addnl
}

\ifra@topic
\newcounter{topics}
\def\ra@topicval@default{1}
\let\ra@declaretopic\declaretopic
\def\declaretopic#1#2{%
  \ra@topicstrue
 \stepcounter{topics}%
 \expandafter\ra@declaretopicaux\expandafter{\the\c@topics}{#1}{#2}}

\def\ra@declaretopicaux#1#2#3{%
 \@namedef{ra@topicval@#2}{#1}
 \ra@topics=\expandafter{\the\ra@topics\ra@declaretopic{#1}{#3}}}
\fi

%% --------------------------------------------------

%% This section defines an environment action
%% This is not yet used, but the code is tested and working.


%% in any case
\define@key{raA}{siid}[NONE]{\XMLaddatt{siid}{#1}}
\define@key{raA}{role}{\XMLaddatt{role}{#1}}
%% for project
\define@choicekey{raB}{nature}{visit,intership,sabbatic,explorator}{\XMLaddatt{nature}{#1}}

%% for participant


% for organism
\define@key{raD}{country}[]{\@ifbempty {#1}{}{\XMLaddatt{country}{#1}}}
\define@key{raD}{city}[]{\@ifbempty {#1}{}{\XMLaddatt{city}{#1}}}
\define@key{raD}{department}[]{\@ifbempty {#1}{}{\XMLaddatt{department}{#1}}}
\define@key{raD}{name}{\XMLaddatt{sname}{#1}}

% for collaborator
\define@key{raE}{title}{\XMLaddatt{title}{#1}}
% for funded
\define@key{raG}{type}{\XMLaddatt{type}{#1}}
%\define@key{raE}{funded}{\XMLaddatt{funded}{#1}}

\def\pr@participant#1#2#3{%
  \xbox{participant}{\setkeys{raA,raC}{#3}\XMLaddatt{name}{#2}\XMLaddatt{firstname}{#1}}\@addnl}

\def\pr@collaborator#1#2#3{%
  \xbox{collaborator}{\setkeys{raA,raC,raE}{#3}\XMLaddatt{name}{#2}\XMLaddatt{firstname}{#1}}\@addnl}



% we locally redefine \url,  we check for emptyseness
\let\@url =\url
\def\pr@@url#1{\@iftempty {#1}{}{\xbox{url}{\@url*{#1}}\@addnl}}

\def\pr@organism#1#2#3{%
  \xbox{organism}{\setkeys{raA,raD}{#2}\XMLaddatt{name}{#1}%
  \let\collaborator=\pr@collaborator#3}\@addnl}

\newtoks\pr@participants
\newtoks\pr@organisms
% Adds a token list #2 at the end of a token register #1
\def\gadd@tohook#1#2{\global#1\expandafter{\the#1#2}}
% If  #2 expand to fooo, adds {foo} at the end of a token register #1
\def\gadd@tohooka#1#2{%
  \toks@=\expandafter{\expandafter{\expandafter{#2}}}%
  \expandafter \gadd@tohook \expandafter#1\the\toks@}



\newenvironment{action} [3][]
{ \gdef\pr@type{#2}%
  \gdef\pr@sname{#3}%
  \gdef\pr@siid{\setkeys{raA,raB}{#1}}%
  \gdef\pr@title{}
  \gdef\pr@abstract{}
  \gdef\pr@duration{}
  \gdef\pr@url{}
  \gdef\pr@funded{}
  \global\pr@participants={}
  \global\pr@organisms={}
 \def\title##1{\gdef\pr@title{\XMLaddatt{name}{##1}}}
 \def\abstract##1{\gdef\pr@abstract{##1}}
 \def\duration##1##2{\gdef\pr@duration{\XMLaddatt{end}{##2}{\XMLaddatt{start}{##1}}}}
 \def\url##1{\gdef\pr@url{\pr@@url{##1}}}
 \def\funded##1##2{\xbox{funded}{\XMLaddatt{by}{##1}\setkeys{raA,raG}{##2}}}
 \def\participant##1##2##3{\gadd@tohook\pr@participants{\pr@participant{##1}{##2}{##3}}}
  \newenvironment{partners}{}{}
  \def\organism##1##2{\gadd@tohook\pr@organisms{\pr@organism{##1}{##2}}
   \grabenv\pr@organismaux}
  \def\endorganism{\gadd@tohooka\pr@organisms\pr@organismaux}
\setbox0=\xbox {cc}\bgroup}
{\egroup 
 \xbox{project}{\pr@siid \pr@duration\pr@title
   \XMLaddatt{program}{\pr@type}\XMLaddatt{shortname}{\pr@sname}
   \xbox{abstract}{\pr@abstract}\@addnl\pr@funded
   \pr@url \the\pr@participants\the\pr@organisms}}


\newenvironment{EuropeanInitiatives}
{\begin{xmlelement*}{EuropeanInitiatives}}
{\end{xmlelement*}}



%% --------------------------------------------------

\endinput
