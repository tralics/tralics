%%%  -*- latex -*-
\ProvidesPackage{subfigure}[2008/01/23 v1.0 Subfigure package]


%% $Id: subfigure.plt,v 2.3 2008/04/21 16:36:28 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% If this quantity is zero, postprocesser converts \subfigure
%% into arrays; otherwise they are left unchanged.
\ifx\tralics@use@subfigure\undefined
\else
\tralics@use@subfigure=1
\fi

\endinput
