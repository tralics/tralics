%%%  -*- latex -*-
\ProvidesPackage{etex}[2007/12/22 v1.1 eTeX basic definition package ]


%% $Id: etex.plt,v 2.2 2008/01/09 17:13:48 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Start of comments from the original style file

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% A basic interface to some etex primitives, closely modeled on
%% etex.src and etexdefs.lib provided by the core etex team.

%% The etex.src `module' system is not copied here, the standard
%% LaTeX package option mechanism is used instead,
%% however the package options match the module names.
%% (Currently grouptypes, interactionmodes, nodetypes, iftypes.)
%% The individual type names are different too: We use, e.g.,
%%
%% `\bottomleveltype' and `\simplegrouptype' instead of
%% `\grouptypes{bottomlevel}' and `\grouptypes{simple}'.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Other Comments...

%% The names of the `interactionmodes' are not too good.
%% In particular \scroll and \batch are likely to clash with existing
%% uses. These names have been changed into \batchinteractionmode,
%% \scrollinteractionmode etc.
%% Similarly, the names of the `groupetypes' have been changed, in
%% particular \mathgroup would conflict with the LaTeX kernel.

%% \etex logo could have the same trick as \LaTeXe to pick up a bold
%% epsilon when needed. (Not done here, I hate wasting tokens on logos.)
%% This version does have a \m@th not in the original.

%% The \globcountvector, \loccountvector, etc. allocation macros are
%% not (yet) implemented.

%% Currently if run on a standard TeX, the package generates an error.
%% Perhaps it should instead load some code to try to fake
%% the new etex primitives in that case???
%% Likewise, the package generates an error when used with e-TeX V 1

%% The etex.src language mechanism is not copied here. That facility
%% does not use any of the etex features. LaTeX should be customised
%% using the same hyphen.cfg mechanism as for a format built with a
%% standard TeX.

%% David Carlisle

%% Upgraded for e-TeX V 2.0
%% Peter Breitenlohner

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% end of comments from the original syle file

\DeclareOption{grouptypes}{\catcode`\G=9}
\DeclareOption{interactionmodes}{\catcode`\I=9}
\DeclareOption{nodetypes}{\catcode`\N=9}
\DeclareOption{iftypes}{\catcode`\C=9}
\DeclareOption{localalloclog}{}
\DeclareOption{localallocnolog}{}
\DeclareOption{localallocshow}{}
\newif\if@noalloc
\DeclareOption{noalloc}{\@noalloctrue}

\def\eTeX{$\varepsilon$-\TeX}

% \def\tracingall{}  no need to redefine in Tralics
%\let\loggingall\tracingall %  In the kernel now

%% This clears only values used by Tralics
\def\tracingnone{%
  \tracingmacros\z@
  \tracingoutput\z@
  \tracingcommands\z@
  \tracingrestores\z@
  \tracingassigns\z@
  \tracingmath\z@}


\catcode`\G=14
\catcode`\I=14
\catcode`\N=14
\catcode`\C=14

\ProcessOptions

%% Declare names for `grouptypes'

G \chardef \bottomleveltype       =  0 % for the outside world
G \chardef \simplegrouptype       =  1 % for local structure only
G \chardef \hboxgrouptype         =  2 % for `\hbox{}'
G \chardef \adjustedhboxgrouptype =  3 % for `\hbox{}' in vertical mode
G \chardef \vboxgrouptype         =  4 % for `\vbox{}'
G \chardef \vtopgrouptype         =  5 % for `\vtop{}'
G \chardef \aligngrouptype        =  6 % for `\halign{}', `\valign{}'
G \chardef \noaligngrouptype      =  7 % for `\noalign{}'
G \chardef \outputgrouptype       =  8 % for output routine
G \chardef \mathgrouptype         =  9 % for, e.g, `^{}'
G \chardef \discgrouptype         = 10 % for `\discretionary{}{}{}'
G \chardef \insertgrouptype       = 11 % for `\insert{}', `\vadjust{}'
G \chardef \vcentergrouptype      = 12 % for `\vcenter{}'
G \chardef \mathchoicegrouptype   = 13 % for `\mathchoice{}{}{}{}'
G \chardef \semisimplegrouptype   = 14 % for `\begingroup...\endgroup'
G \chardef \mathshiftgrouptype    = 15 % for `$...$'
G \chardef \mathleftgrouptype     = 16 % for `\left...\right'

%% Declare names for `interactionmodes'

I \chardef \batchinteractionmode     = 0 % omits all stops and omits terminal output
I \chardef \nonstopinteractionmode   = 1 % omits all stops
I \chardef \scrollinteractionmode    = 2 % omits error stops
I \chardef \errorstopinteractionmode = 3 % stops at every opportunity to interact

%% Declare names for `nodetypes'

N \chardef \charnode     =  0 % character nodes
N \chardef \hlistnode    =  1 % hlist nodes
N \chardef \vlistnode    =  2 % vlist nodes
N \chardef \rulenode     =  3 % rule nodes
N \chardef \insnode      =  4 % insertion nodes
N \chardef \marknode     =  5 % a mark node
N \chardef \adjustnode   =  6 % an adjust node
N \chardef \ligaturenode =  7 % a ligature node
N \chardef \discnode     =  8 % a discretionary node
N \chardef \whatsitnode  =  9 % special extension nodes
N \chardef \mathnode     = 10 % a math node
N \chardef \gluenode     = 11 % node that points to a glue specification
N \chardef \kernnode     = 12 % a kern node
N \chardef \penaltynode  = 13 % a penalty node
N \chardef \unsetnode    = 14 % an unset node
N \chardef \mathsnodes   = 15 % nodes that occur only in maths mode

%% Declare names for `iftypes'

C \chardef \charif     =  1 % \if
C \chardef \catif      =  2 % \ifcat
C \chardef \numif      =  3 % \ifnum
C \chardef \dimif      =  4 % \ifdim
C \chardef \oddif      =  5 % \ifodd
C \chardef \vmodeif    =  6 % \ifvmode
C \chardef \hmodeif    =  7 % \ifhmode
C \chardef \mmodeif    =  8 % \ifmmode
C \chardef \innerif    =  9 % \ifinner
C \chardef \voidif     = 10 % \ifvoid
C \chardef \hboxif     = 11 % \ifhbox
C \chardef \vboxif     = 12 % \ifvbox
C \chardef \xif        = 13 % \ifx
C \chardef \eofif      = 14 % \ifeof
C \chardef \trueif     = 15 % \iftrue
C \chardef \falseif    = 16 % \iffalse
C \chardef \caseif     = 17 % \ifcase
C \chardef \definedif  = 18 % \ifdefined
C \chardef \csnameif   = 19 % \ifcsname
C \chardef \fontcharif = 20 % \iffontchar

\catcode`\G=11
\catcode`\I=11
\catcode`\N=11
\catcode`\C=11


\if@noalloc\endinput\fi
%% Register allocation
%% The number of registers in Tralics is 1024 and all builtin commands like
%% \newdimen will allocate from 10 to 1023

%% The functions below are easy to implement; but they use 20 count registers
\def\et@xglob#1#2#3#4{\error{Commands like \string\globcount\space not yet implemented}}
\def\et@xloc#1#2#3#4{\error{Commands like \string\loccount\space not yet implemented}}
\def\et@xgblk#1#2#3#4{\error{Commands like \string\globcountblk\space not yet implemented}}
\def\et@xlblk#1#2#3#4{\error{Commands like \string\loccountblk\space not yet implemented}}

% \def \et@xglob #1#2#3#4% <offset>, <type>, <method>, <register>
% \def \et@xloc #1#2#3#4% <offset>, <type>, <method>, <register>

\def \globcount  {\et@xglob 0\count  \countdef}
\def \loccount   {\et@xloc  0\count  \countdef}
\def \globdimen  {\et@xglob 1\dimen  \dimendef}
\def \locdimen   {\et@xloc  1\dimen  \dimendef}
\def \globskip   {\et@xglob 2\skip   \skipdef}
\def \locskip    {\et@xloc  2\skip   \skipdef}
\def \globmuskip {\et@xglob 3\muskip \muskipdef}
\def \locmuskip  {\et@xloc  3\muskip \muskipdef}
\def \globbox    {\et@xglob 4\box    \mathchardef}
\def \locbox     {\et@xloc  4\box    \mathchardef}
\def \globtoks   {\et@xglob 5\toks   \toksdef}
\def \loctoks    {\et@xloc  5\toks   \toksdef}
\def \globmarks  {\et@xglob 6\marks  \mathchardef}
\def \locmarks   {\et@xloc  6\marks  \mathchardef}

\def\globcountblk  {\et@xgblk 0\count  }
\def\loccountblk   {\et@xlblk 0\count  }
\def\globdimenblk  {\et@xgblk 1\dimen  }
\def\locdimenblk   {\et@xlblk 1\dimen  }
\def\globskipblk   {\et@xgblk 2\skip   }
\def\locskipblk    {\et@xlblk 2\skip   }
\def\globmuskipblk {\et@xgblk 3\muskip }
\def\locmuskipblk  {\et@xlblk 3\muskip }
\def\globboxblk    {\et@xgblk 4\box    }
\def\locboxblk     {\et@xlblk 4\box    }
\def\globtoksblk   {\et@xgblk 5\toks   }
\def\loctoksblk    {\et@xlblk 5\toks   }
\def\globmarksblk  {\et@xgblk 6\marks  }
\def\locmarksblk   {\et@xlblk 6\marks  }


\endinput