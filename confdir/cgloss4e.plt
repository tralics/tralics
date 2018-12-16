% -*- latex -*-
\ProvidesPackage{cgloss4e}[2006/08/24 v1.0 cgloss4e for Tralics]
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software  license 
%% (See the file COPYING in the main directory for details)
%% $Id: cgloss4e.plt,v 2.3 2007/05/22 09:46:32 grimm Exp $



%% No options

%% These commands allow single spacing in \gll or \glll
%% even when the current style says doublespacing
\def\singlegloss{}
\def\nosinglegloss{}

%% These commands are written in C++, allow use simple name
\let\gll\cgloss@gll
\let\glll\cgloss@glll


% omit the \vskip
\def\glt{\par\noindent}
\let\trans\glt
\def\glend{} % obsolete
\providecommand\eachwordone{\rm}
\providecommand\eachwordtwo{\rm}
\providecommand\eachwordthree{\rm}
