%%% -*- latex -*-
\ProvidesPackage{braket}[2007/11/08 v1.0 Dirac bra-ket notation]
%% $Id: braket.plt,v 2.2 2008/01/31 17:41:54 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Original Latex file by Donald Arseneau
%% Needs tralics 2.10.9, because of the \mathcode

\def\bra#1{\mathinner{\langle{#1}|}}
\def\ket#1{\mathinner{|{#1}\rangle}}
\def\braket#1{\mathinner{\langle{#1}\rangle}}
\def\Bra#1{\left\langle#1\right|}
\def\Ket#1{\left|#1\right\rangle}

{\catcode `|=13
  \gdef\Braket#1{\begingroup
    \ifx\SavedDoubleVert\relax \let\SavedDoubleVert\|\let\|\BraDoubleVert \fi
    \mathcode`\|32768\let|\BraVert
    \left\langle{#1}\right\rangle\endgroup}    
  \gdef\set#1{\mathinner
    {\lbrace\,{\mathcode`\|32768\let|\midvert #1}\,\rbrace}}  
  \gdef\Set#1{\left\{%
      \ifx\SavedDoubleVert\relax \let\SavedDoubleVert\|\fi
      \:{\let\|\SetDoubleVert
        \mathcode`\|32768\let|\SetVert
        #1}\:\right\}}
}


%% % turn || into \|
\gdef\BraVert{\@ifnextchar|{\|\@gobble}  {\egroup\,\mid@vertical\,\bgroup}}
\def\SetVert{\@ifnextchar|{\|\@gobble}   {\egroup\;\mid@vertical\;\bgroup}}

\def\BraDoubleVert{\egroup\,\mid@dblvertical\,\bgroup}
\def\SetDoubleVert{\egroup\;\mid@dblvertical\;\bgroup}
\let\SavedDoubleVert\relax

\let\mid@vertical\vert
\let\mid@dblvertical\Vert

\def\midvert{\egroup\mid\bgroup}
\endinput
%
%   \Braket{ \phi | \frac{\partial^2}{\partial t^2} | \psi }
%   \Set{ x\in\mathbf{R}^2 | 0<{|x|}<5 }
%
