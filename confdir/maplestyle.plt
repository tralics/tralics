%% -*- latex -*-

\ProvidesPackage{maplestyle}[1998/09/19 -- Maple Environments -- mapleenv.sty ]
\RequirePackage{mapleutil}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Copyright (c) 1994--1998 by Waterloo Maple Inc., all rights reserved.
%
% Permission is granted to use this package without charge. It may
% be redistributed providing that it is distributed as is
% and without charge.  You may modify these macros for your own use,
% and redistribute them, subject to the following conditions.
%
% 1. The package name used for any modified version of this file is 
%    different from maplestyle and a file name other than maplestyle.sty
%    is used.
%
% 2. You acknowledge this copyright notice and this file as the source.
%
% 3. The new file is redistributed under the same terms and conditions 
%    as indicated here, including a version of this notice modified to
%    refer to the new file and/or package name.
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%--------------------------------------------------------------------
% - Routine for Automatically defining Undefined Character Styles
%--------------------------------------------------------------------
\newif\if@def\@deffalse
\def\DefineCharStyle#1{
  \@ifundefined{#1}{\@deftrue}{\@deffalse}
  \if@def
    \typeout{Defining a new character style: #1}
    \expandafter\def\csname #1\endcsname##1{\relax {##1}}
  \fi}
%
%--------------------------------------------------------------------
% - Routine for Handling Undefined Paragraph Styles  
%
% These are handled as environments.  Blank spaces are allowed in 
% the environment names.
%--------------------------------------------------------------------
\def\DefineParaStyle#1{
\@ifundefined{#1}{\@deftrue}{\@deffalse}
\if@def
  \typeout{Defining new paragraph style: #1}
  \expandafter\newcommand \csname #1\endcsname[0]{\relax}%
  \expandafter\long\expandafter\def\csname end#1\endcsname{\relax}
\fi
}

%--------------------------------------------------------------------
% - Routine for Handling Undefined Character Styles
%
% This macro allows for macro names having spaces in them.
% Usage:
%
%     \QTR{macro name}{argument 1}
%
%--------------------------------------------------------------------
\@ifundefined{QTR}{%    
  \gdef\QTR#1#2{{\csname#1\endcsname{#2}}}}{\relax} % 
\@ifundefined{TEXTsymbol}{%
  \gdef\TEXTsymbol#1{\mbox{$#1$}}}{\relax}%

%--------------------------------------------------------------------
% - The Known (Pre-defined!) Maple Paragraph Styles
%---------------------------------------------------------------------
% \newenvironment{maplegroup}{}{}



%
\newenvironment{Heading 1}%
{\par\noindent\sffamily\Large\bfseries\upshape}%
{\par}
\newenvironment{Heading 2}%
{\par\noindent\sffamily\large\bfseries\upshape}%
{\par}
\newenvironment{Heading 3}%
{\par\noindent\sffamily\large\bfseries\itshape}%
{\par}
\newenvironment{Heading 4}%
{\par\noindent\sffamily\normalsize\bfseries\itshape}%
{\par}
%
\newenvironment{Normal}{\normalsize\rmfamily\mdseries}{}
\newenvironment{Text Output}{\normalsize\ttfamily\mdseries}{}
\newenvironment{Title}{\begin{center}\rmfamily\LARGE\upshape\ignorespaces}%
{\end{center}}
\newenvironment{Warning}{\ttfamily}{}
%--------------------------------------------------------------------
% -- Fixed Width Environment is like a verbatim Version
%--------------------------------------------------------------------
%
\newbox\maplettybox%
\setbox\maplettybox=\hbox{}
%
\expandafter\def\csname FixedWidth\endcsname{
  \removelastskip%
  \nopagebreak[3]% 
  \MaplePromptfalse       % No prompt is required in this environment.
  \@SetupMapleTty{1}   % The 1 indicates "respect first new line"
  \@BeginFixedWidth  % np indicates omit prompt
}
%
\expandafter\def\csname endFixedWidth\endcsname{%
  \@EndMapleTty%
  \removelastskip
  \vskip\BelowMapleSkip%
  \pagebreak[3]%
  \@endpetrue
  \ignorespaces
}%
%
%------------------------------------------------------------------------------
% -- Regular Known Character styles
%
%        If spaces occur in name then define using \expandafter
%        as seen below, and invoke as QTR{name}{options}
%        Otherwise, define and use as normal (i.e., using \def)
%
%------------------------------------------------------------------------------
\expandafter\def\csname 2D Comment\endcsname#1{{\rmfamily\upshape #1}}
\expandafter\def\csname 2D Input\endcsname#1{{\rmfamily\upshape #1}}
\expandafter\def\csname 2D Math\endcsname#1{{\rmfamily\upshape #1}}
\expandafter\def\csname 2D Output\endcsname#1{{\rmfamily\upshape #1}}
\expandafter\def\csname 2D Math\endcsname#1{{\rmfamily\upshape #1}}
\expandafter\def\csname Help Heading\endcsname#1{{\large\bfseries\sffamily\upshape #1}}
\expandafter\def\csname Help Normal\endcsname#1{{\rmfamily\itshape #1}}
\expandafter\def\csname LaTeX\endcsname#1{\relax #1\relax }
\expandafter\def\csname 2D Comment\endcsname#1{{\rmfamily\itshape #1}}
\expandafter\def\csname Maple Input\endcsname#1{{\ttfamily\upshape #1}}
\expandafter\def\csname Output Labels\endcsname#1{{\ttfamily\upshape #1}}
\expandafter\def\csname Plot Title\endcsname#1{{\large\sffamily\upshape #1}}
\expandafter\def\csname Plot Text\endcsname#1{{\sffamily\upshape #1}}
\expandafter\def\csname Popup\endcsname#1{{\bfseries\sffamily\upshape #1}}
\expandafter\def\csname Hyperlink\endcsname#1{{\bfseries\rmfamily\itshape #1}}
%--------------------------------------------------------------------
% Hyperlink support (printing characteristics only)
%--------------------------------------------------------------------
\def\labelLink#1#2{\Hyperlink{#2}}
\def\docLink#1#2{\Hyperlink{#2}}
\endinput %%% maplestyle.def
%%% </PRE>
