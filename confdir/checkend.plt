%%%  -*- latex -*-
\ProvidesPackage{checkend}[2007/12/14 v1.0 Checks for end environments]
%% After an idea of bezos

%% $Id: checkend.plt,v 2.1 2007/12/19 08:38:31 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2007
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)
%% N.B. Do not use \pop@stack differently from this

\AtEndDocument{\pop@stack}
\endinput
