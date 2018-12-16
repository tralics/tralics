% -*- latex -*-
%% <PRE>
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
%    different from mapleutil and a file name other than mapleutil.sty
%    is used.
%
% 2. You acknowledge this copyright notice and this file as the source.
%
% 3. The new file is redistributed under the same terms and conditions 
%    as indicated here, including a version of this notice modified to
%    refer to the new file and/or package name.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Miscelaneous utilities used to define the main Maple environments
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\ProvidesPackage{mapleutil}[1998/09/19 -- Maple Style Utilities]
\typeout{Defining Maple Utility Macros}

\let \removelastskip\relax % tralics
\newskip\normalbaselineskip 

%
% Utilities for changing character codes
%
\def\MakeTab#1{\catcode `#1 = 4 }      		% &
\def\MakeParameter#1{\catcode `#1 = 6 }		% #
\def\MakeSuperScript#1{\catcode `#1 = 7 }	% ^
\def\MakeSubScript#1{\catcode `#1 = 8 }		% _
\def\MakeLetter#1{\catcode `#1 = 11 }
\def\MakeOther#1{\catcode`#1 = 12 }
\def\MakeActive#1{\catcode `#1 = \active\relax }
\def\MakeTabActive{\MakeActive{\^^I}}
\def\MakeEolActive{\MakeActive{\^^M}}
%
\@ifundefined{dospecials}{%
\gdef\dospecials{\do\ \do\\\do\{\do\}\do\$\do\&\do\#\do\^\do\_\do\%\do\~}%
}{\relax}%
\@ifundefined{postdospecials}{%
\gdef\postdospecials{%
\catcode`\\\z@%
\catcode`\{\@ne%
\catcode`\}\tw@%
\catcode`\ 10%
\catcode`\&11}%
}{\relax}%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% In a Maple tty environment every line is treated as a paragraph.
% The behavior at the end of every Maple paragraph (line) is given
% by, thereby inhibiting page breaks in such environments.
\def\@MaplePar{\nopagebreak[3]\par\@@par}%
% \def\@MaplePar{\nopagebreak[3]\EveryParZ\@@par}%
%
\newif\if@IgnoreNewLine
\@IgnoreNewLinetrue
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Characters requiring special treatment
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%-- Spaces
\def\MapleSpace{\ }
\def\@ObeySpaces{\MakeActive{\ }\@@ObeySpaces\relax}
{\MakeActive{\ }\gdef\@@ObeySpaces{\edef {\MapleSpace}}}

%-- BackSlashes

{\catcode`| = 0 |catcode`\\ = 12 |gdef|BackSlash{\}}

%-- Tabs

\newdimen\@MapleTabSize
\def\@ObeyTabs{\MakeTabActive\@@ObeyTabs\relax}
{\MakeTabActive\gdef\@@ObeyTabs{\def^^I{\@MapleTab}}}
\def\@MapleTab{%
  \leavevmode   % ensure that we are in horizontal mode
  \egroup      % Terminate box register 0
  \dimen0=\wd0 % store its length and round up to next stop
  \divide\dimen0 by \@MapleTabSize
  \advance\dimen0 by 1sp
  \multiply \dimen0 by \@MapleTabSize
  \wd0 = \dimen0
  \box0
  \setbox0 = \hbox\bgroup
}

%-- End of Line Character

\def\@ObeyEol{\MakeEolActive\@@ObeyEol\relax}%
{\MakeEolActive\gdef\@@ObeyEol{\let^^M=\@MapleEol}}
\def\@MapleEol{%
  \if@IgnoreNewLine
  \else
    \leavevmode%
    \egroup%
    \box 0%
    \@MaplePar%
  \fi
  \@IgnoreNewLinefalse
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Definitions of Utilities for the Main Maple Environments
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% These Macros are the common elements of the verbatim like environents.
% The spacing above and below the environment should is done in the 
% environment preamble instead of here to enable this to be used in a 
% variety of such environments.

% \newtoks\MapleIOType \MapleIOType = {a}
\edef\@MapleQuiet{q}      % indicates a silent command or result
\edef\@MapleVerbose{a}    % The standard behavior in which
                          % results are generated and displayed.
\edef\@MapleNoPrompt{np}  % specify no prompts in environment
\edef\@MaplePrompt{p}     % specify prompts for this environment
\edef\@MapleNoInput{ni}   % No input appears in output stream.
\edef\@MapleNoOutput{no}  % No output appears in outputstream.

% The next macro is used by the verbatim like environments to
% reset character codes, and change the treatment of new lines, etc.
% Actions  can be made conditional on the value of \@MapleIOType which
% is always set to 'a' , or to 'q' as in the value of the 
% optional parameter used in \begin{mapleinput}[q]

\newtoks\MapleListCorrection  \MapleListCorrection={}
\def\@SetupMapleTty#1{%
\par\@endpefalse   %% to clear the \@endpetrue done in \mapleinput
\begingroup  %% this is the real start of the special environment. 
  %%%  Compute and set tab positions
  \@MapleTabSize = &em
  \multiply\@MapleTabSize by \MapleTab
  %%%  Set this flag to 0 if the \begin{...} is to appear on a
  %%%  separate line of the source file.
  \ifnum #1 = 0 \@IgnoreNewLinetrue\else\@IgnoreNewLinefalse\fi%
  %
  %%% Describe how to handle every paragraph (recall paragraph == line )
  %% \@MapleFont%
  \everypar = {\EveryParZ}%
  \def\EveryParZ{%
    \ifMaplePrompt
    \the\MapleListCorrection
    \the\MaplePromptString
    \MaplePromptString = \MaplePromptSecondary
    \MapleListCorrection = {\hskip 0pt\relax}
    \fi
    \setbox0 = \hbox \bgroup
  }%
%
%%%  Choose the type size and font -  The order of these commands
%%%  may be important.
\@MapleFont%
\let\do\@makeother\dospecials%
\catcode ``=\active%
\frenchspacing%
\@ObeySpaces% 
\@ObeyTabs%
\@ObeyEol%
}

% Exit the special tty environments
\def\@EndMapleTty{%
\endgroup%% matches the \begingroup inside of @SetupMapleTty
}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
% The following Macros start the various Verbatim like environments
% for special Maple regions.  These calls set up the 
% scanning for the environment terminator.
%
% A conditional parameter as in \begin{...}[q] etc. denotes a
% special class of Maple Regions.

\begingroup
  \catcode`| = 0  % becomes the escape character
  \catcode`( = 1  % becomes a begin group
  \catcode`) = 2  % becomes an end group
  \catcode`@ = 11 % becomes a regular character (for macro names)
  \catcode`\{ = 12 % becomes an other character
  \catcode`\} = 12 % becomes a other character
  \catcode`\\ = 12 % becomes a other character
%
%  type a   standard (verbose) usage
%  type q   quiet command
%  type np  no prompt  (imapleinput only at this time...)
%  type ni  no input  
%  type no  no output  
%
% -- Maple Inert Input  
|gdef|@BeginIMapleInput#1\end{imapleinput}(%
|MaplePrompttrue%
#1%
|end(imapleinput))%
%
% -- Inert TTY Output
|gdef|@BeginIMapleTtyout#1\end{imaplettyout}(%
|vskip|AboveMapleSkip%  extra vspace is needed here
|MaplePromptfalse
#1%
|end(imaplettyout))%
%
% -- Fixed Width
|gdef|@BeginFixedWidth#1\end{FixedWidth}(%
|vskip|AboveMapleSkip%  extra vspace is needed here
|MaplePromptfalse
#1%
|end(FixedWidth))%
% -- TTY Input 
|gdef|@BeginMapleInput[#1]#2\end{mapleinput}(%
|edef|@MapleIOType(#1)%
|gdef|MapleIOType(#1)%
|ifx|@MapleIOType|@MapleNoPrompt|MaplePromptfalse|fi%
|ifx|@MapleIOType|@MapleNoInput|edef|@MapleIOType(q)|fi%
|ifx|@MapleIOType|@MapleQuiet%
|typeout(.............Quiet Maple Command #1)%
|removelastskip%
|else%
%|typeout(.............Verbose Maple Command)%
#2%
|fi%
|end(mapleinput))%
%
% -- TTY Output
|gdef|@BeginMapleTtyout[#1]#2\end{maplettyout}(%
%%|typeout(TTYout Environment of type #1)%
|edef|@MapleIOType(#1)%
|gdef|MapleIOType(#1)%
|ifx|@MapleIOType|@MaplePrompt|MaplePromptfalse|fi%
|ifx|@MapleIOType|@MapleNoOutput|edef|@MapleIOType(q)|fi%
|ifx|@MapleIOType|@MapleQuiet%
|typeout(.............Quiet Maple Tty Out)%
|else%
#2%
|fi%
|end(maplettyout))%
%
% -- Suppressed Latex Output
|gdef|@GobbleMapleLatex#1\end{maplelatex}(%
|end(maplelatex))%
|endgroup
%
\@ifundefined{]}{\gdef\[{$}}{\relax}
\@ifundefined{[}{\gdef\]{$}}{\relax}
\endinput
%%% </PRE>
