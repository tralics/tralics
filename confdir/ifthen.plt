%%% -*- latex -*-
\ProvidesPackage{ifthen}[2006/10/05 v1.0 standard latex ifthen package]
%% $Id: ifthen.plt,v 2.6 2007/11/23 17:00:32 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


\let\@original@ifthen\ifthenelse
\def\@fixifthen{\def\value##1{\the\csname c@##1\endcsname}} 
\newif\if@ifthenpatch

\def\Ifthenelse#1{%
 \bgroup
   \@fixifthen
   \@original@ifthen{#1}{\global\@ifthenpatchtrue}{\global\@ifthenpatchfalse}%
 \egroup%
 \@original@ifthen{\boolean{@ifthenpatch}}}
\endinput

%The package is implemented in C++


