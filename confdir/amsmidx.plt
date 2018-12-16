%%%  -*- latex -*-
\ProvidesPackage{amsmidx}[2007/12/12 v1.0 multiple indices]


%% $Id: amsmidx.plt,v 2.1 2007/12/12 17:13:26 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\endinput

These command should be used for creating multple indices
\renewcommand{\makeindex}[1]{filename}
\renewcommand{\index}[2]{filename=#1, object=#2} 
\renewcommand{\printindex}[1]{filename=#1}
\newcommand{\Printindex}[2]{filename=#1 indexname=#2}
\newcommand{\indexcomment}[1]{text to be added later via \indexchap}
\renewcommand{\indexchap}[1]{#1 = some header}
\endinput
