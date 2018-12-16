%% <PRE>
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Maple Table Support
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\ProvidesPackage{mapletab}[1998/09/19 -- Maple Spreadsheets]
\typeout{Defining Maple Spreadsheet Environemnts}
\typeout{Maple Spreadsheet and Table Support}
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
%    different from mapletab and a file name other than mapletab.sty
%    is used.
%
% 2. You acknowledge this copyright notice and this file as the source.
%
% 3. The new file is redistributed under the same terms and conditions 
%    as indicated here, including a version of this notice modified to
%    refer to the new file and/or package name.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Maple Spreadsheets and Tables Package
%%%
%%%  Maple displays its spreadsheets as a tabular arrays of cells.
%%%
%%% This is essentially a tabular environment
%%% with some pre and post processing. 
%%% The format is:
%%% \begin{maplespreadsheet}[spreadsheet01.tab]{|c|c|}
%%% \hline
%%% entry 1 &
%%% entry 2 &
%%% \mapleinline{inert}{2d}{1;}{xx} \\
%%% \hline
%%% entry 2,21 &
%%% entry 2,3  &
%%% \mapleinline{{1;} \\
%%% \hline
%%%\end{maplespreadsheet}
%%%
%%%  The cells are in typeset in text mode. The mathematics is
%%%  usually provided in \mapleinline in order to provide both the
%%%  Maple and the LaTeX encoding of the expression.
%%%
%%%  Maple's \multiline macro is used in the latex portion
%%%  of the \mapleinline macro in order to effect line wrapping.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newif\ifMapleTableCell%  % used to change vertical offset of a
			  % mapleinline inside table
\newdimen\multilinesep%
\multilinesep=1ex%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% maplespreadsheet - tabular environment whose entries are encoded
%%%	using mapleinline.
%%%
%%%	\begin{maplespreadsheet}[filename]{format}
%%%	...
%%%	\end{maplespreadsheet}
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\maplespreadsheet{%
\@ifnextchar [{\@maplespreadsheet}{\@maplespreadsheet[]}
}
\def\@maplespreadsheet[#1]#2{%
\par\MapleTableCelltrue\begin{center}\begin{tabular}{#2}}%
\def\endmaplespreadsheet{%
\end{tabular}%
\end{center}%
\MapleTableCellfalse%
}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% maplemultiline - Used for stacking lines of maple input and output
%%%
%%% An implementation of mapleinline based on vbox and hbox.
%%% The tokens are parsed and dropped into hboxes which are in turn
%%% collected into a vbox.
%%%
%%% Each line of the multiline is followed by some extra space
%%% No extra space is required at the top as the multiline is
%%% used inside of \mapleinline, which provide's its own space
%%%
%%% \maplemultiline{x^2 + y^2\\(x - y)^{100}\\}
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Tokens and utilities used by \maplemultiinline
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newbox\@linebuffer\setbox\@linebuffer=\vbox{\relax}
\newbox\@currentline\setbox\@currentline=\hbox{\relax}
\newdimen\@linebufferwidth  %%% DEBUG %%%
\newtoks\EndMultiLine%
\@ifundefined{@skiponetoken}{\gdef\@skiponetoken#1{\relax}}{\relax}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \maplemultiline -
%%% for use primarily inside of table cells.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\maplemultiline#1{%
{\def\\{\relax}%
\processcell#1\noexpand\\\EndMultiLine%
}%
%%% \@linebufferwidth=\wd\@linebuffer%%% DEBUG %%%
%\fbox{\@dumplinebuffer}%         %%% DEBUG %%%
\@dumplinebuffer%
}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \processcell -
%%% start processing the contents of the cell
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\processcell{\futurelet\@nexttoken\processline}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \processline
%%% - invokes \processfullline unless at the
%%% end of the cell. (skips over \cr's to
%%% avoid spacing problems)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\processline{%
\ifx\@nexttoken\EndMultiLine%
\let\next\@skiponetoken%  % at end of cell.
\else%
\let\next\processfullline%% grab next line 
\fi%
% \cr's cause spacing problems.
\ifx\@nexttoken\cr\@skiponetoken\let\next\processline\fi%
\next%
}%
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \processfullline -
%%% - Adds currentline (as identified by \\)
%%% to the vbox \@linebuffer
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\processfullline #1\\{%
\global\setbox\@currentline=\hbox{$#1$}
\@extendlinebuffer{\@currentline}%
\processcell}
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \@extendlinebuffer - a stack of lines
%%% add currentline to end of \@linebuffer
%%%
%%% #1 is assumed to be a box. (hbox) and
%%% is reset at the end.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@extendlinebuffer#1{%
\ifvoid#1\else% 	% skip over blank lines
\ifvoid\@linebuffer%	% first line ?
\global\setbox\@linebuffer=\vtop{\copy#1}%
\else%			% add new lines
\global\setbox\@linebuffer=\vtop{%
\unvcopy\@linebuffer%
\copy#1%
\vskip\multilinesep%
}%
\fi%
\fi%
\global\setbox#1=\hbox{}% reset currentline
}
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% \@dumplinebuffer -
%%% place @linebuffer on the output stream as
%%% a vbox.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@dumplinebuffer{%
\setbox\@linebuffer=\vtop{%
\hbox{\vrule height3pt depth0pt width 0pt}%
\unvbox\@linebuffer%
\unskip}%  
\box\@linebuffer%  A verticle box ...
\global\setbox\@linebuffer=\vbox{}%
\relax}
\endinput %%% mapletab.def
%%% </PRE>
