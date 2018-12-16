%%% -*- latex -*-
\ProvidesPackage{fixltx2e}[2006/10/06 v1.0 standard latex]
%% $Id: fixltx2e.plt,v 2.3 2007/05/22 09:46:32 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2006, 2007
%% Licensed under the CeCILL free software  license 
%% (See the file COPYING in the main directory for details)


%%% This package corrects some bugs

%% 2-col: 1-col fig can come before earlier 2-col fig
%%    -> floats in Tralics do not float
%% \@ discards spaces when moving
%%  -> \@ is empty in tralics
%% \setlength{\dimen0}{10pt}
%%  -> currently, this is an error, because \setlength wants a single token.
%% \addpenalty ruins flush-bottom
%%  -> \addpenalty not yet implemented
%% \fnsymbol should use text symbols if possible
%%   -> tralics never enters math in this case
%% No hyphenation in first word after float environment
%%  -> \@espback \@Esphack redefined. Currently unknown in Tralics
%%  wrong headline for twocolumn
%% Allowing \texttt{\textbackslash emph} to produce small caps

\endinput
