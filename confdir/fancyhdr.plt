%% -*- latex -*-
\ProvidesPackage{fancyhdr}[2011/6/01 v1.0 fancyheadings for tralics]
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% $Id: fancyhdr.plt,v 1.1 2011/08/05 12:20:07 grimm Exp $


\newlength{\fancy@headwidth}
\let\headwidth\fancy@headwidth

\newcommand{\headrulewidth}{0.4pt}
\newcommand{\footrulewidth}{0pt}
\newcommand{\footruleskip}{.3\normalbaselineskip}
\newcommand{\plainheadrulewidth}{0pt}
\newcommand{\plainfootrulewidth}{0pt}

\def\nouppercase#1{{#1}}
\def\headrule{\xmlemptyelt{headrule}}
\def\footrule{\xmlemptyelt{footrule}}
\def\fancypagestyle#1{\xbox{fancypagestyle}{\XMLaddatt{value}{#1}}}