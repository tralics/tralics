% -*- latex -*-
\ProvidesPackage{amscd}[2007/03/24 v1.0 Commutative Diagrams]
\RequirePackage{amsgen}

%\atdef@ A#1{x} define a command \b@A, where b is a space character
\def\atdef@#1{\expandafter\def\csname\space @\string#1\endcsname}
\def\at@use#1{\csname\space @\string#1\endcsname}%

%% @))) is the same as @>>>
\begingroup \catcode`\~=\active \lccode`\~=`\@
\lowercase{%
  \global\atdef@)#1)#2){~>#1>#2>}
  \global\atdef@(#1(#2({~<#1<#2<}
  \gdef\CDat{\let ~=\at@use}
}% end lowercase
\endgroup



%% True in CD environment
\newif\ifCD@

\def\internalecdatsignvalue{\at@use}
\begingroup \catcode`\@=\active

\endgroup


\newenvironment{CD}{\catcode`@=13\CDat\begin{matrix}}{\end{matrix}}

\def\@phantom#1{\mathbox{mphantom}{\scriptstyle #1}}

\atdef@>#1>#2>{&\xrightarrow[#2]{#1}&}
\atdef@<#1<#2<{&\xleftarrow[#2]{#1}&}

\atdef@ A#1A#2A{\cd@arrow{#1}{#2}{\uparrow}&&}
\atdef@ V#1V#2V{\cd@arrow{#1}{#2}{\downarrow}&&}

\def\cd@arrow@none#1{#1}
\def\cd@arrow@right#1#2{\@phantom{#1}#2{\scriptstyle#1}}
\def\cd@arrow@left#1#2{{\scriptstyle#1}#2\@phantom{#1}}
\def\cd@arrow@both#1#2#3{\@phantom{#2}{\scriptstyle#1}#3{\scriptstyle#2}\@phantom{#1}}

\def\cd@arrow#1#2#3{
  \@ifbempty{#1}{\@ifbempty{#2}{\cd@arrow@none#3}{\cd@arrow@right{#2}#3}}
    {\@ifbempty{#2}{\cd@arrow@left{#1}#3}{\cd@arrow@both{#1}{#2}#3}}
}
\atdef@|{\Big\Vert&&}
\atdef@\vert{\Big\Vert&&}
\atdef@.{&&}
\atdef@={&=&}
\endinput
%%
%% End of file `amscd.sty'.




\endinput


