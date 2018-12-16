%%%%%  -*- latex -*-

%% $Id: forloop.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
% Original code by nsetzer (C) LGPL


\ProvidesPackage{forloop}  [2008/01/10 v1.0 For Loops for LaTeX]
\RequirePackage{ifthen}



\newcommand{\forloop}[5][1]{%
  \setcounter{#2}{#3}%
  \ifthenelse{#4}
  {#5\addtocounter{#2}{#1}\forloop[#1]{#2}{\value{#2}}{#4}{#5}}
  {}}

% Original code 
% \newcommand{\forLoop}[5][1]{%
%   \setcounter{#4}{#2}%
%   \ifthenelse{ \value{#4}<#3 }
%   {#5\addtocounter{#4}{#1}\forLoop[#1]{\value{#4}}{#3}{#4}{#5}}
%   {\ifthenelse{\value{#4}=#3}
%     {#5}
%     {}}}

\newcommand{\forLoop}[5][1]{%
  \setcounter{#4}{#2}%
  \ifthenelse{\not  \value{#4}>#3 }
  {#5\addtocounter{#4}{#1}\forLoop[#1]{\value{#4}}{#3}{#4}{#5}}
  {}}


\endinput
