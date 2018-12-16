%%% -*- latex -*-
\ProvidesPackage{newlfont}[2006/10/06 v1.0 newlfont]
%% $Id: newlfont.plt,v 2.3 2007/05/22 09:46:32 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%    This file contains the code for the \texttt{newlfont} package
%    which provides defines commands like |\rm| to behave as with
%    NFSS1, i.e., to change one font attribute without resetting the
%    other attributes.

\let\rm\rmfamily
\let\sf\sffamily
\let\tt\ttfamily
\let\bf\bfseries
\let\sl\slshape
\let\sc\scshape
\let\it\itshape
\let\mediumseries\mdseries
\let\normalshape\upshape
\let\pcal\@undefined
\let\cal\mathcal
\let\pmit\@undefined
\let\mit\mathnormal

\endinput
%
