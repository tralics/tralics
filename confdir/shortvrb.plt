%%% -*- latex -*-
\ProvidesPackage{shortvrb}[2006/10/06 v1.0 Standard latex]
%% $Id: shortvrb.plt,v 2.3 2007/05/22 09:46:32 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\def\MakeShortVerb{\@ifstar{}{}\DefineShortVerb}
\let\DeleteShortVerb\UndefineShortVerb

\endinput
