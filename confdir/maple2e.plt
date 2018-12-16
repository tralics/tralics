
\ProvidesPackage{maple2e}[2007/02/16 V1.0
Maple Latex Style Package for Tralics]

%
\newif\ifnoplots\noplotsfalse
\DeclareOption{noplots}{}
\DeclareOption{dvips}{}
\DeclareOption{dvi2ps}{}
\DeclareOption{dvilaser}{}
\DeclareOption{dvipsone}{}
\DeclareOption{dvitops}{}
\DeclareOption{dviwindo}{}
\DeclareOption{emtex}{}
\DeclareOption{pubps}{}
\DeclareOption{textures}{}
\DeclareOption{xdvi}{}
\DeclareOption{dvitps}{}
\DeclareOption{dviwin}{}
\DeclareOption{oztex}{}
\DeclareOption{SWP}{}

% \ExecuteOptions{dvips}  % default's to dvips driver
\ProcessOptions*


\usepackage{mapleenv}   %%% Standard Maple Environments
\usepackage{mapletab}   %%% Maple Macros for tables and spreadsheets
\usepackage{mapleplots} %%% Maple Macros for Plots
\usepackage{maplestyle} %%% Macros supporting Automatic Paragraph Styles
\endinput %%%% The End %%%%%

