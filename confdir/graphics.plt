%%%  -*- latex -*-
\ProvidesPackage{graphics}[2008/11/06 v1.0  Graphics in Tralics]
%% $Id: graphics.plt,v 2.1 2008/11/13 14:13:38 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)


\DeclareOption{draft}{}
\DeclareOption{final}{}
\DeclareOption{hiresbb}{}
\DeclareOption{demo}{}
\DeclareOption{debugshow}{}
\DeclareOption{hiderotate}{}
\DeclareOption{hidescale}{}

\DeclareOption{dvips}{}
\DeclareOption{xdvi}{}
\DeclareOption{dvipdf}{}
\DeclareOption{dvipdfm}{}
\DeclareOption{dvipdfmx}{}
\DeclareOption{xetex}{}
\DeclareOption{pdftex}{}
\DeclareOption{dvipsone}{}
\DeclareOption{dviwindo}{}
\DeclareOption{emtex}{}
\DeclareOption{dviwin}{}
\DeclareOption{oztex}{}
\DeclareOption{textures}{}
\DeclareOption{pctexps}{}
\DeclareOption{pctexwin}{}
\DeclareOption{pctexhp}{}
\DeclareOption{pctex32}{}
\DeclareOption{truetex}{}
\DeclareOption{tcidvi}{}
\DeclareOption{vtex}{}

\ProcessOptions
\def\reflectbox{\scalebox{-1}[1]}
\def\graphicspath#1{}
\def\DeclareGraphicsExtensions#1{}
\def\DeclareGraphicsRule#1#2#3#4{}

\def\resizebox{%
  \leavevmode
  \@ifstar{\Gscale@@box{total}}{\Gscale@@box{}}}

\def\Gin@exclamation{!}
\def\Gscale@@box#1#2#3#4{%
  \xbox{resizebox}{%
    \let\@tempa\Gin@exclamation
    \def\height{0pt}\def\width{0pt}\def\totalheight{0pt}%
    \expandafter\def\expandafter\@tempb\expandafter{\string#2}%
    \expandafter\def\expandafter\@tempc\expandafter{\string#3}%
    \unless\ifx\@tempb\@tempa
    \setlength\@tempdima{#2}\XMLaddatt{width}{\the\@tempdima}%
    \fi
    \unless\ifx\@tempc\@tempa
    \setlength\@tempdima{#3}\XMLaddatt{#1height}{\the\@tempdima}%
    \fi
    #4}}

\endinput

