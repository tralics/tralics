%%%  -*- latex -*-
\ProvidesPackage{fixfoot}[2009/01/101 v1.0 fixed-text footnotes]

%% $Id: fixfoot.plt,v 2.1 2009/01/15 18:10:58 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


% Adaptation of a package by 
% Author: Robin Fairbairns <rf10@cam.ac.uk>

\DeclareOption{debug}{}
\ProcessOptions\relax
%
\newcommand\DeclareFixedFootnote{%
  \@ifstar{\@declare@fftn\xspace}%
          {\@declare@fftn\relax }}

\newcommand\@declare@fftn[3]{%
  \@ifdefinable#2{\@declare@@fftn{#1}{#2}{#3}}}

\newcounter{fixfoot}
\def\ff@newnote#1{}

\newcommand\@declare@@fftn[3]{%
  \stepcounter{fixfoot}%
  \edef\x{\AtEndDocument{\xbox{fixfoot}{\XMLaddatt{id}{fid\the\value{fixfoot}}%
      \unexpanded{#3}}\@addnl}}%
  \x
  \edef#2{\xbox{footref}{\XMLaddatt{target}{fid\the\value{fixfoot}}}\noexpand#1}}

%
\endinput
