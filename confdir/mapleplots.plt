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
%    different from mapleplots and a  file name other than mapleplots.sty
%    is used.
%
% 2. You acknowledge this copyright notice and this file as the source.
%
% 3. The new file is redistributed under the same terms and conditions 
%    as indicated here, including a version of this notice modified to
%    refer to the new file and/or package name.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Various Plot Configurations
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\ProvidesPackage{mapleplots}[1998/09/19 -- Maple Plotting Macros]
\typeout{Defining Maple Plot Environemnts}
% 
% -- noplots  -- No Driver Available ?
%
\def\mapleplotname#1{
	\typeout{ ...using Plot file #1}%
	\vskip.5\MaplePlotHeight
        \centerline{\Large \bfseries Plot:  #1}
        \vskip.5\MaplePlotHeight
}
%
% -- epsfig.sty (used by dvips, etc)
%
\def\epsfigmapleplot#1{
	\parindent=0ex%
	\MaplePlotWidth  = 0.900\textwidth%
	\MaplePlotHeight = 0.651\textwidth%
	\MaplePlotHeight@Fudge=0.90\MaplePlotHeight%
	\MaplePlotWidth@Fudge =0.90\MaplePlotWidth%
	\ifvmode \removelastskip \else \par \parindent=0ex\removelastskip\fi
	%% \par
        \vskip \AboveMaplePlot
	%
	%  Plot sizes must be fudged because Maple .eps plots
	%  generated using Maple's built-in PostScript plot driver
	%  are 6.5 x 9 inches landscape, but included in portrait
	%  mode to a much smaller scale.
	%
	\IfFileExists{#1}{%
		\typeout{ ...including Plot #1 at size
		\the\MaplePlotWidth\space by \the\MaplePlotHeight}%
		\hbox{%
		    \relax
		    {}\hskip \LeftMapleSkip
		    \epsfig{figure=#1,%
			height=\MaplePlotHeight@Fudge,% scale before rotation
			width=\MaplePlotWidth@Fudge,%
			angle=\MaplePlotAngle}%
		}% end of hbox
	}% 
	{%
		\typeout{ ...Missing Plot File  #1}
		\noindent	% So \mapleplot can be used inside of
				% the LaTeX center environment.
		\vbox{\mapleplotname{#1} }%
	}%
	\par\removelastskip
        \vskip\BelowMaplePlot
	% \vskip-2\BelowMaplePlot{\relax}
	%%% \vspace*{-.3\MaplePlotHeight} 
	% \relax\noindent\ignorespaces
}% end of epsfigmapleplot def

%
% -- Oztex postscript driver
%
% Rotation is not supported.  so use Maple's commands to rotate.
% This can be done using the Maple command 
%
%	interface(plotoptions=`portrait,height=200,width=300`); 
%
% This is the same as mapleespfig, but with no rotation expected.
%
\def\oztexmapleplot#1{
	\ifvmode \removelastskip \else \par \fi
	\vskip \AboveMaplePlot
	%
	%  Plot sizes must be fudged because Maple .eps plots
	%  generated using Maple's built-in PostScript plot driver
	%  are 7.5 x 10 inches, but include an additional 0.5 inch
	%  white border within the PostScript BoundingBox.
	%
	\MaplePlotHeight@Fudge=1.133333\MaplePlotHeight
	\MaplePlotWidth@Fudge =1.100000\MaplePlotWidth
	\vskip -0.066667\MaplePlotHeight		%Fudge
	\noindent	% So \mapleplot can be used inside of
			% the LaTeX center environment.
	\IfFileExists{#1}{%
		\typeout{ ...including Plot #1 at size
		\the\MaplePlotWidth\space by \the\MaplePlotHeight}%
		\hbox{%
		    \hskip -0.05\MaplePlotWidth		%Fudge
		    \epsfig{figure=#1,%
			height=\MaplePlotWidth@Fudge,% scale before rotation
			width=\MaplePlotHeight@Fudge}%
%%oztex%% 			angle=\MaplePlotAngle}%
		    \hskip -0.05\MaplePlotWidth		%Fudge
		}%
	}% end of first case
	{% second case 
	    %\typeout{ ...Missing Plot File  #1}
	    { %
		\vskip.5\MaplePlotHeight%
		\centerline{\Large \bfseries Plot File = #1}
		\vskip.5\MaplePlotHeight %
		}% end of vbox
	}% end of second case
	\par
	\vskip -0.066667\MaplePlotHeight		%Fudge
	\vskip\BelowMaplePlot
	\vskip -\parskip\noindent\ignorespaces
} % end of oztexmapleplot

%%%%%%%%%%%%%%%%%%%%%%%%%
% Install the Driver.
%%%%%%%%%%%%%%%%%%%%%%%%%
\@ifundefined{Driver}{%
      \ifnoplots
	     \let\mapleplot\mapleplotname
	     \let\imapleplot\mapleplot
      \else
      \IfFileExists{epsfig.sty}{% Default to dvips
             \typeout{ ...Defaults to "dvips" Driver }
             \usepackage[dvips]{epsfig}
             \let\mapleplot\epsfigmapleplot
             \let\imapleplot\mapleplot
             \edef\Driver{dvips}}{%  no driver 
	     % generate vertical space containing file name
	     \let\mapleplot\mapleplotname
	     \let\imapleplot\mapleplot
	}
      \fi
}{\usepackage[\Driver]{epsfig}}
%
\def\maplesmartplot#1#2{\mapleplotname{#1}}
%
\endinput % End of File
%%% </PRE>
