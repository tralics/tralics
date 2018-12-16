%%%  -*- latex -*-
\ProvidesPackage{color}[2008/11/06 v1.0  Basic Colors ]

%% $Id: color.plt,v 2.1 2008/11/13 14:13:38 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

\DeclareOption{debugshow}{}
\DeclareOption{dvips}{\def\c@lor@namefile{}}
\DeclareOption{xdvi}{\def\c@lor@namefile{}}
\DeclareOption{dvipdf}{}
\DeclareOption{dvipdfm}{}
\DeclareOption{dvipdfmx}{}
\DeclareOption{pdftex}{}
\DeclareOption{xetex}{}
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

\DeclareOption{dvipsnames}{\def\c@lor@namefile{}}
\DeclareOption{nodvipsnames}{\let\c@lor@namefile\relax}
\let\c@lor@namefile\relax

%% This is the default, make implementation easier
\DeclareOption{usenames}{}
\DeclareOption{monochrome}{}
\ProcessOptions\relax

\def\textcolor#1#{\@textcolor{#1}}
\def\@textcolor#1#2#3{\leavevmode{\color#1{#2}#3}}


%%% Define now some colors
\def\c@lor@named{named}
\def\DefineNamedColor#1#2#3#4{%
  \def\tmp{#1}
  \ifx\tmp\c@lor@named\definecolor{#2}{#3}{#4}%
  \else\error{Bad color model #1}\fi}


\definecolor{black}{rgb}{0,0,0}
\definecolor{white}{rgb}{1,1,1}
\definecolor{red}{rgb}{1,0,0}
\definecolor{green}{rgb}{0,1,0}
\definecolor{blue}{rgb}{0,0,1}
\definecolor{cyan}{cmyk}{1,0,0,0}
\definecolor{magenta}{cmyk}{0,1,0,0}
\definecolor{yellow}{cmyk}{0,0,1,0}

\ifx\c@lor@namefile\relax\else
\DefineNamedColor{named}{GreenYellow}   {cmyk}{0.15,0,0.69,0}
\DefineNamedColor{named}{Yellow}        {cmyk}{0,0,1,0}
\DefineNamedColor{named}{Goldenrod}     {cmyk}{0,0.10,0.84,0}
\DefineNamedColor{named}{Dandelion}     {cmyk}{0,0.29,0.84,0}
\DefineNamedColor{named}{Apricot}       {cmyk}{0,0.32,0.52,0}
\DefineNamedColor{named}{Peach}         {cmyk}{0,0.50,0.70,0}
\DefineNamedColor{named}{Melon}         {cmyk}{0,0.46,0.50,0}
\DefineNamedColor{named}{YellowOrange}  {cmyk}{0,0.42,1,0}
\DefineNamedColor{named}{Orange}        {cmyk}{0,0.61,0.87,0}
\DefineNamedColor{named}{BurntOrange}   {cmyk}{0,0.51,1,0}
\DefineNamedColor{named}{Bittersweet}   {cmyk}{0,0.75,1,0.24}
\DefineNamedColor{named}{RedOrange}     {cmyk}{0,0.77,0.87,0}
\DefineNamedColor{named}{Mahogany}      {cmyk}{0,0.85,0.87,0.35}
\DefineNamedColor{named}{Maroon}        {cmyk}{0,0.87,0.68,0.32}
\DefineNamedColor{named}{BrickRed}      {cmyk}{0,0.89,0.94,0.28}
\DefineNamedColor{named}{Red}           {cmyk}{0,1,1,0}
\DefineNamedColor{named}{OrangeRed}     {cmyk}{0,1,0.50,0}
\DefineNamedColor{named}{RubineRed}     {cmyk}{0,1,0.13,0}
\DefineNamedColor{named}{WildStrawberry}{cmyk}{0,0.96,0.39,0}
\DefineNamedColor{named}{Salmon}        {cmyk}{0,0.53,0.38,0}
\DefineNamedColor{named}{CarnationPink} {cmyk}{0,0.63,0,0}
\DefineNamedColor{named}{Magenta}       {cmyk}{0,1,0,0}
\DefineNamedColor{named}{VioletRed}     {cmyk}{0,0.81,0,0}
\DefineNamedColor{named}{Rhodamine}     {cmyk}{0,0.82,0,0}
\DefineNamedColor{named}{Mulberry}      {cmyk}{0.34,0.90,0,0.02}
\DefineNamedColor{named}{RedViolet}     {cmyk}{0.07,0.90,0,0.34}
\DefineNamedColor{named}{Fuchsia}       {cmyk}{0.47,0.91,0,0.08}
\DefineNamedColor{named}{Lavender}      {cmyk}{0,0.48,0,0}
\DefineNamedColor{named}{Thistle}       {cmyk}{0.12,0.59,0,0}
\DefineNamedColor{named}{Orchid}        {cmyk}{0.32,0.64,0,0}
\DefineNamedColor{named}{DarkOrchid}    {cmyk}{0.40,0.80,0.20,0}
\DefineNamedColor{named}{Purple}        {cmyk}{0.45,0.86,0,0}
\DefineNamedColor{named}{Plum}          {cmyk}{0.50,1,0,0}
\DefineNamedColor{named}{Violet}        {cmyk}{0.79,0.88,0,0}
\DefineNamedColor{named}{RoyalPurple}   {cmyk}{0.75,0.90,0,0}
\DefineNamedColor{named}{BlueViolet}    {cmyk}{0.86,0.91,0,0.04}
\DefineNamedColor{named}{Periwinkle}    {cmyk}{0.57,0.55,0,0}
\DefineNamedColor{named}{CadetBlue}     {cmyk}{0.62,0.57,0.23,0}
\DefineNamedColor{named}{CornflowerBlue}{cmyk}{0.65,0.13,0,0}
\DefineNamedColor{named}{MidnightBlue}  {cmyk}{0.98,0.13,0,0.43}
\DefineNamedColor{named}{NavyBlue}      {cmyk}{0.94,0.54,0,0}
\DefineNamedColor{named}{RoyalBlue}     {cmyk}{1,0.50,0,0}
\DefineNamedColor{named}{Blue}          {cmyk}{1,1,0,0}
\DefineNamedColor{named}{Cerulean}      {cmyk}{0.94,0.11,0,0}
\DefineNamedColor{named}{Cyan}          {cmyk}{1,0,0,0}
\DefineNamedColor{named}{ProcessBlue}   {cmyk}{0.96,0,0,0}
\DefineNamedColor{named}{SkyBlue}       {cmyk}{0.62,0,0.12,0}
\DefineNamedColor{named}{Turquoise}     {cmyk}{0.85,0,0.20,0}
\DefineNamedColor{named}{TealBlue}      {cmyk}{0.86,0,0.34,0.02}
\DefineNamedColor{named}{Aquamarine}    {cmyk}{0.82,0,0.30,0}
\DefineNamedColor{named}{BlueGreen}     {cmyk}{0.85,0,0.33,0}
\DefineNamedColor{named}{Emerald}       {cmyk}{1,0,0.50,0}
\DefineNamedColor{named}{JungleGreen}   {cmyk}{0.99,0,0.52,0}
\DefineNamedColor{named}{SeaGreen}      {cmyk}{0.69,0,0.50,0}
\DefineNamedColor{named}{Green}         {cmyk}{1,0,1,0}
\DefineNamedColor{named}{ForestGreen}   {cmyk}{0.91,0,0.88,0.12}
\DefineNamedColor{named}{PineGreen}     {cmyk}{0.92,0,0.59,0.25}
\DefineNamedColor{named}{LimeGreen}     {cmyk}{0.50,0,1,0}
\DefineNamedColor{named}{YellowGreen}   {cmyk}{0.44,0,0.74,0}
\DefineNamedColor{named}{SpringGreen}   {cmyk}{0.26,0,0.76,0}
\DefineNamedColor{named}{OliveGreen}    {cmyk}{0.64,0,0.95,0.40}
\DefineNamedColor{named}{RawSienna}     {cmyk}{0,0.72,1,0.45}
\DefineNamedColor{named}{Sepia}         {cmyk}{0,0.83,1,0.70}
\DefineNamedColor{named}{Brown}         {cmyk}{0,0.81,1,0.60}
\DefineNamedColor{named}{Tan}           {cmyk}{0.14,0.42,0.56,0}
\DefineNamedColor{named}{Gray}          {cmyk}{0,0,0,0.50}
\DefineNamedColor{named}{Black}         {cmyk}{0,0,0,1}
\DefineNamedColor{named}{White}         {cmyk}{0,0,0,0}
\fi

\endinput
