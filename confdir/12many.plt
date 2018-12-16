% -*- latex -*-
% $Id: 12many.plt,v 1.1 2015/11/25 07:51:35 grimm Exp $
\ProvidesPackage{12many}[2015/08/09 12many for Tralics]

%% Original file
%% Copyright (C) 2004,2005 by Ulrich M. Schwarz.

\RequirePackage{calc, keyval}

\def\ldotp{.} % should be defined elsewhere

\newcommand\newOTMstyle{%
  \let\otm@@newcmd\newcommand
  \otm@@fooOTMstyle
}
\newcommand\renewOTMstyle{%
  \let\otm@@newcmd\renewcommand
  \otm@@fooOTMstyle
}
\newcommand*\otm@@fooOTMstyle[2][\relax]{%
  \ifx #1\relax\else
  \@for\parm:=#1\do{%
    \expandafter\otm@@newkvOTMparameter\parm={#2}%
  }%
  \fi
  \expandafter\otm@@newcmd\expandafter*\csname otm@style@#2\endcsname[2]%
}
\def\otm@@newkvOTMparameter#1=#2=#3{%
  \newOTMparameter{#3}{#1}{#2}%
}
\newcommand\newOTMparameter[3]{%
  \expandafter\providecommand\csname otm@#1@#2\endcsname{#3}%
  \define@key{otm@#1}{#2}{\expandafter\renewcommand\csname otm@#1@#2\endcsname{##1}}%
}
\newcommand\setOTMparameter[2]{%
  \expandafter\renewcommand\csname otm@#1@#2\endcsname%
}
\newcommand\getOTMparameter[2][\otm@@currentstyle]{%
  \csname otm@#1@#2\endcsname%
}
\newcommand*\setOTMstyle[2][]{%
  \edef\otm@@currentstyle{#2}% was \protected@edef
  \PackageInfo{12many}{Using style \otm@@currentstyle[#1]}%
  \setkeys{otm@\otm@@currentstyle}{#1}%
  \expandafter\let\expandafter\nto\csname otm@style@\otm@@currentstyle\endcsname%
}
\newcommand\nto{%
  \PackageWarning{12many}{No style selected. Using dots.}%
  \setOTMstyle{dots}%
  \nto%
}
\newcommand\oto{\nto{0}}
\newcommand\ito{\nto{1}}

%% Style "`set"': the "`proper way"': {i\in N: x<=i<=y}
\newOTMstyle[var=i,naturals={\mathrm{N}_0},where=|]{set}{%
  \{\getOTMparameter{var}\in\getOTMparameter{naturals}%
  \getOTMparameter{where} #1\leq\getOTMparameter{var}\leq#2\}%
}
%% Style "`laue"': 1 to n is n with some sort of rharpoon below.
\newcounter{otm@scratch}

% Assumes argument is an integer; error otherwise [JG]
\newOTMstyle{laue}{%
  \ifcase#1\relax
  (\{0\}\otm@laue@setplus\otm@@laue@laue{#2})%
  \or
  \otm@@laue@laue{#2}%
  \else
  \setcounter{otm@scratch}{#1}%
  \addtocounter{otm@scratch}{-1}%
  (\otm@@laue@laue{#2}%
  \otm@laue@setminus\otm@@laue@laue{\arabic{otm@scratch}})%
  \fi
}
\newOTMparameter{laue}{setminus}{\setminus}
\newOTMparameter{laue}{setplus}{\cup}
\newOTMparameter{laue}{ybelow}{0.3ex}
\newOTMparameter{laue}{strokewidth}{0.08ex}
\newOTMparameter{laue}{innersidegap}{0.05em}
\newOTMparameter{laue}{outersidegap}{0.05em}
\newcommand{\otm@@laue@laue}[1]{\mathbox{mrow}[style][laue]{#1}}

%% Style "`dots"': variations of the {1,...,n} theme.

\newOTMstyle[dots={,\ldots,}]{dots}{%
  \{#1\getOTMparameter{dots}#2\}%
}
%% Style "`nude"': variations of the 1..n theme.

\newOTMstyle[dots={\ldotp\ldotp}]{nude}{%
  #1\getOTMparameter{dots}#2%
}

\InputIfFileExists{12many.cfg}{%
  \PackageInfo{12many}{Also using customization file 12many.cfg}
}{\PackageInfo{12many}{No customization file used}}


\DeclareOption*{\setOTMstyle{\CurrentOption}}
\ProcessOptions\relax
\endinput
