%%% -*- latex -*-
\ProvidesPackage{curves} [2008/03/25 1.0 Curves for LaTeX picture environment]

% For the LaTeX version: Copyright (C) 1991-2000 Ian Maclaine-cross

%% $Id: curves.plt,v 2.1 2008/04/01 17:02:06 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\newlength\csdiameter
\newlength\curvelength
\newlength\overhang
\newif\ifcurvewarn \curvewarntrue

\DeclareOption{dvips}{}
\DeclareOption{emtex}{}
\DeclareOption{xdvi}{}
\DeclareOption{WML}{}

\ProcessOptions\relax

\let\arc\curves@arc
\let\bezier\curves@bezier
\let\bigcircle\curves@bigcircle
\let\closecurve\curves@closecurve
\let\curve\curves@curve
\let\tagcurve\curves@tagcurve
\let\scaleput\curves@scaleput


\newcommand\diskpitchstretch{\@ne}
\newcommand\patternresolution{\@ne}

\newcommand\curvesymbol[1]{\PackageWarning{curves}{Command
    \string\curvesymbol\space not implemented}}
\newcommand\curvedashes[2][\unitlength]{\PackageWarning{curves}{Command
    \string\curvedashes\space not implemented}}

\endinput

