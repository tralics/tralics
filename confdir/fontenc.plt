%%% -*- latex -*-
\ProvidesPackage{fontenc}[2006/10/06 v1.0 Standard Latex]
%% $Id: fontenc.plt,v 2.4 2007/05/22 09:46:32 grimm Exp $

\def\fontencoding#1{\wlog{output encoding option #1 seen}}

\DeclareOption*{\fontencoding{\CurrentOption}}
\ProcessOptions
\endinput
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)