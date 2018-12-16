% -*- latex -*-
\ProvidesPackage{mapleenv}[2007/02/16 v1.0
 Maple Environments  for Tralics]
\ProvidesPackage{mapleenv}[1998/09/19 -- Maple Environments -- mapleenv.sty ]
\typeout{Defining Standard Maple Environemnts}
\RequirePackage{mapleutil}

%% Original copyright notice
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
%    different from mapleenv and the file name used is different from
%    mapleenv.sty
%
% 2. You acknowledge this copyright notice and this file as the source.
%
% 3. The new file is redistributed under the same terms and conditions 
%    as indicated here, including a version of this notice modified to
%    refer to the new file and/or package name.
%

\DefineVerbatimEnvironment{maplettyout}{Verbatim}
{pre=pre,style=maplettyout}

\def\FV@pre@pre{\begin{xmlelement*}{pre}}
\def\FV@post@pre{\end{xmlelement*}}
\def\FV@style@maplettyout{\XMLaddatt{class}{maplettyout}}




\newskip\AboveMapleSkip		% space above the maple environments.
\newskip\BelowMapleSkip		% space below the maple environments.
\newdimen\LeftMapleSkip 	% indentation for the maple commands.
\newskip\AboveMaplePlot		% spacing above and below maple plots.
\newskip\BelowMaplePlot
\newdimen\MaplePlotHeight 	% size of the maple plot.
\newdimen\MaplePlotWidth 
\newdimen\MaplePlotHeight@Fudge	% dimensions of the Maple plot - needed
\newdimen\MaplePlotWidth@Fudge	% to compensate for extra space around plot
\newdimen\MapleSepLineWidth 
\newif\ifMapleFirstLine		% flag to control printing of first CR
\newif\ifMaplePrompt		% flag to control prompting
\newcount\MapleTab	
\newtoks\MaplePromptString 
\newtoks\MaplePromptSecondary 

%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Set the Default Values  
%
\AboveMapleSkip  = 0.5ex plus 3 pt minus 0 pt
\BelowMapleSkip  = 0.5\AboveMapleSkip
\LeftMapleSkip   = 1.5em		  
\AboveMaplePlot  = 2\AboveMapleSkip
\BelowMaplePlot  = \AboveMapleSkip
\MaplePlotWidth  = 0.900\textwidth
\MaplePlotHeight = 0.651\textwidth
%%% \edef\MaplePlotAngle{270}	% assume Maple plots are in landscape mode
\edef\MaplePlotAngle{0}		% assume Maple plots are in portrait mode
%
% separator line parameters (obsolete)
%
\newdimen\MapleSepLineHeight \MapleSepLineHeight = 0.01in
\let\MapleSepLineWidth\linewidth  % use \let for automatic scaling
\MapleSepLineHeight = 1pt
%
% Font characteristics and other controls 
% for inside Maple's special verbatim environments.
%
\def\MapleSize{\small}
\def\MapleFont{\tt}
\def\@MapleFont{\MapleFont\MapleSize\@noligs}  % used in mapletty envs.
\def\MapleLatexSize{\normalsize} % change this to change maplelatex size
\MapleFirstLinefalse  % Ignore first \cr after \begin{mapleinput}  
\MaplePrompttrue      % Include the prompt string for each input line.
\MapleTab = 8
\MaplePromptString = {\raise 1pt \hbox{$\scriptstyle>$\space}}
\MaplePromptSecondary = \MaplePromptString % prompts  on every line
% \MaplePromptSecondary = {\space\space} % no prompts after first line
%
\newif\ifMapleTableCell%  % used to change vertical offset of a
			  % mapleinline inside table


%%\input{mapleenv.def}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%
% -- imapleinput % Removed JG

% -- imaplettyout % Removed JG
%
% -- imaplelatex % Removed JG

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% -- mapleinput
%
\newlength\newleftskip%
\newbox\mapleinputbox%

\newenvironment{mapleinput}{%
\tracingall
\ignorespaces
  \def\@gobblesemicolons##1{##1}
  \global\setbox\mapleinputbox=\xbox{maplein}\bgroup%
  \everypar{\the\MaplePromptString\MaplePromptString=\MaplePromptSecondary\ }
}{%
  \egroup% end of vbox
  \par
  \vskip\AboveMapleSkip%
  \box\mapleinputbox
  \vskip\BelowMapleSkip
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% -- maplelatex 
%
% This is essentially a wrapper around a display math environment
% It gobbles its content if the [q] or [no] options are in
% effect.
%
\newbox\maplebox
\def\maplelatex{\@ifnextchar [{\@maplelatex}{\@maplelatex[a]}}
%
\def\@maplelatex[#1]{%
  \gdef\MapleIOType{#1}%% for \ifx test%
  \global\setbox\maplebox=\xbox{mapleout}\bgroup%
  \MakeSuperScript{^}%  Ensure these have normal interpretation
  \MakeSubScript{_}%
}%
% 
\def\endmaplelatex{\egroup%
  \copy\maplebox
  \everypar{}%
  \par%
}%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 
%
% -- maplesepline
%
\newcommand{\maplesepline}{\vskip \parskip%
\hrule\@height\MapleSepLineHeight\@width\MapleSepLineWidth%
\vskip \parskip\relax}
%
% -- imaplesepline ( the inert form )
%
\let\imaplesepline\maplesepline
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% -- \mapleIndent
%
% - Special Indentation Macro used to format procedures
%
\newdimen\Tab
\Tab=3ex
\def\mapleIndent#1{\hbox{\hskip#1\Tab\relax}}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  Special environments for in-line maple commands.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  \begin{maplegroup}				- maple exchange group
%  ...
%  ... and \mexpr{sin(x^2)} ...			- inline maple code
%  ...
%  \mapleinline{Inert}{TwoD}{int(sin(x^2),x);}{%	- for maple filter
%  {mapletex}{$\int{ .... }$}		        - for latex
%  ...
%  ...
%  \mapleresult					- location of inline results
%  \begin{maplelatex}				- actual displayed results
%  ...
%  \end{maplelatex}
%  \end{maplegroup}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
% -- \mexpr 
%
% 	\mexpr{int(sin(x^2),x)} --- \mapleexpr{int(sin(x^2),x)}
%
% To make { and } visible in this verbatim environment, use \{ and \}
% Also, to make \ visible, use \\. 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
\def\mexpr{\setbox\@inlinebox=\hbox\bgroup\ttfamily\dosetup\let\\\BackSlash\@domexpr}
%
\def\mapleexpr{\bgroup\ttfamily\dosetup\let\\\BackSlash\@domexpr}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%  \@dosetup	      sets up the character codes, etc. for verbatim
%  \@domexpr          completes the following inline math
%  \@gobblefirst      absorbs the inline math (not displaying it)
%  \@gobblesecond     absorbs the second following argument.
%  \@gobblesemicolons      absorbs semi-colons inside of the 1d mode.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\dosetup{\let\do\@makeother\dospecials\postdospecials}
%
%\def\@domexpr#1{#1\egroup\fbox{\box\@inlinebox}}
\def\@domexpr#1{#1\egroup\box\@inlinebox}
%
\def\@gobblefirst#1{%
  \relax\egroup%
  \box\@inlinebox%
  \setbox\@inlinebox=\hbox{}}
%
\def\@gobblesecond#1#2{%
  #1\egroup\ignorespaces%
  \box\@inlinebox%
  \setbox\@inlinebox=\hbox{}}
% 
\def\@gobblesemicolons{\MakeActive{;}\@@gobblesemicolons}
% 
{\MakeActive{;}\gdef\@@gobblesemicolons{\edef;{\relax}}}

%%% end of mexpr
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
% -- \mapleinline
%
%	Essentially a four argument macro used to control the
%	display of inline material generated by Maple.
%
%	\mapleinline %
%	<status> 	% "active", or "inert"
%	<display mode>  % 1d or 2d
%	<verbatim source> % raw maple code as per \mexpr
%	<latex 2-D source> % latex representation
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% \mapleinline{inert}{2d}{int(sin(x^2),x)}{ % used to generate display
% $\int{\sin(x^2),dx}$}	                    % updated by the filter
%
% Could use \@bsphack ... \@esphack for absorbing unwanted space not 
% currently used, but spacing inside this macro definition is 
% very important.
%
\edef\OneD{1d}\edef\TwoD{2d}% # tokens to identify modes
\newif\ifDisplay\Displayfalse
\newbox\@inlinebox\setbox\@inlinebox=\hbox{}%
\@ifundefined{@skiponetoken}{\gdef\@skiponetoken#1{\relax}}{\relax}%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 2-D output
% final output macro for mapleinline.
% include spacing above and below to ensure there is
% whitespace around the object when it occurs in
% a spreadsheet cell.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@doTwoD#1{\egroup%
%\@ifnextchar\cr{\@skiponetoken}{\relax}%
\@@doTwoD}%
%
\newdimen\cellwidth%
\def\@@doTwoD#1{%
  \ignorespaces%
  \setbox\@inlinebox=\hbox{#1}%
  \ifMapleTableCell%  		  Used in maplespreadsheet
  BAD
  \else%				  Used inline ...
  \box\@inlinebox\egroup%
  \fi%
}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% 1-D output
% Final output macro for mapleinline for 1-D output.
% The raw tokens must be dumped into the output stream so
% we use unhbox.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@doOneD#1{\MakeOther{\ } #1\egroup\unvbox\@inlinebox\relax\@mgobbleone}
\def\@mgobbleone#1{\relax\egroup}
%

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\mapleinline#1#2{%
% ignore first argument (inert or active)		
  \def\[{$}\def\]{$}%		% needed for math mode.
  \edef\DisplayType{#2}%		% 1d or 2d
  \ifx\DisplayType\TwoD
    \let\Display\@doTwoD%	% display the 2-D representation
    \global\Displaytrue
  \else
    \let\Display\@doOneD	% display the 1-D representation
    \global\Displayfalse
  \fi
  \bgroup
  \global\setbox\@inlinebox=\xbox{Linebox}\bgroup
  \obeylines
  \everypar{\the\MaplePromptString\MaplePromptString=\MaplePromptSecondary\ }%
\leavevmode
  \ttfamily\dosetup\obeyspaces\let\\\BackSlash\Display
}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  % \fbox{\box\@inlinebox}% %%% DEBUG %%%

%
% -- \begin{maplegroup} ... \end{maplegroup}
% -- \mapleresult
%
%	A environment used to group commands and result together into
%	one unit.  Inside, these are separated by \mapleresult.
%	The maple filter automatically deletes the part of a maplegroup
%	which follows \mapleresult and replaces it with new results.
%	
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%\newenvironment{maplegroup}{}{}
\def\maplegroup{\relax}
\def\endmaplegroup{\relax}
\let\mapleresult\relax   
\endinput %%% mapleenv.sty
%%% </PRE>
