%%% -*- latex -*-
\ProvidesPackage{index}[2007/12/29 v1.0 Multiple Index]

%% $Id: index.plt,v 2.1 2008/02/19 16:21:17 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


\def\newindex{\@ifnextchar[{\x@newindex}{\x@newindex[thepage]}}
\def\x@newindex[#1]{\@ifstar {\y@newindex{#1}} {\y@newindex{#1}}}
\def\y@newindex#1#2#3#4#5{\@newindex{#2}{#5}}

\let\renewindex\newindex
\def\disableindex#1{\error{Command \string\disableindex\space not implemented}}

\providecommand{\seename}{see}
\providecommand*{\see}[2]{\emph{\seename} #1}

\endinput
