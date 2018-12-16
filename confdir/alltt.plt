%%% -*- latex -*-
\ProvidesPackage{alltt}[2006/10/05 v1.0 defines alltt environment]
%% $Id: alltt.plt,v 2.4 2007/05/22 09:46:32 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\def\alltt@dospecials{\do\ \do\$\do\&\do\#\do\^\do\_\do\%\do\~}


\def\alltt@setup{%
\let\do\@makeother
\alltt@dospecials
\@vobeyspaces
\obeylines\par\tt
}

\newenvironment{alltt}{\alltt@setup}{\par}

\endinput

