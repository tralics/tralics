% -*- latex -*-
\ProvidesPackage{amsopn}[2007/03/24 v1.0 operator names for Tralics]

% \DeclareRobustCommand{\operatorname}{ ... } Kernel
% \DeclareRobustCommand{\qopname}{ ... } Kernel
%\newcommand{\DeclareMathOperator}{% not yet implemented

\DeclareOption{namelimits}{\let\nmlimits@\displaylimits}
\DeclareOption{nonamelimits}{\let\nmlimits@\nolimits}
\let\nmlimits@\displaylimits

\ExecuteOptions{}
\ProcessOptions\relax

\endinput

% Commands that follow ignored;
% they are now defined in the kernel

\newcommand{\DeclareMathOperator}{%
  \@ifstar{\@declmathop m}{\@declmathop o}}
\long\def\@declmathop#1#2#3{%
  \@ifdefinable{#2}{%
    \DeclareRobustCommand{#2}{\qopname\newmcodes@#1{#3}}}}

 
\@onlypreamble\DeclareMathOperator

\def\operator@font{\mathgroup\symoperators}
\def\operatorfont{\operator@font}
\def\operatornamewithlimits{\operatorname*}
\def\varlim@#1#2{%
  \vtop{\m@th\ialign{##\cr
    \hfil$#1\operator@font lim$\hfil\cr
    \noalign{\nointerlineskip\kern1.5\ex@}#2\cr
    \noalign{\nointerlineskip\kern-\ex@}\cr}}%
}
\def\varinjlim{%
  \mathop{\mathpalette\varlim@{\rightarrowfill@\textstyle}}\nmlimits@
}
\def\varprojlim{%
  \mathop{\mathpalette\varlim@{\leftarrowfill@\textstyle}}\nmlimits@
}

\def\varliminf{\mathop{\mathpalette\varliminf@{}}\nmlimits@}
\def\varliminf@#1{\@@underline{\vrule\@depth.2\ex@\@width\z@
   \hbox{$#1\m@th\operator@font lim$}}}
\def\varlimsup{\mathop{\mathpalette\varlimsup@{}}\nmlimits@}
\def\varlimsup@#1{\@@overline{\hbox{$#1\m@th\operator@font lim$}}}


