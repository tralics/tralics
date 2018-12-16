%% -*- latex -*-
%% $Id: decimal.plt,v 2.1 2008/04/29 16:46:43 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Expansion of \. was \mbox{.}, replaced by \char`\.

%% Original copyright notice
%%  (c) copyright 2003 Apostolos Syropoulos and Dick Nickalls
%%  This program can be redistributed and/or modified under the terms
%%  of the LaTeX Project Public License Distributed from CTAN


\ProvidesFile{decimal.sty}  [2003/07/12 v1.0 Package `decimal.sty']
\let\peri@d\.
\def\.{\ifmmode\char`\.\else\expandafter\peri@d\fi}
%\DeclareMathSymbol{\cd@t}{\mathord}{symbols}{"01}
\mathcode`\.="8000
{\catcode`\.=\active%
 \gdef.{\cdot} }
\endinput
%%

