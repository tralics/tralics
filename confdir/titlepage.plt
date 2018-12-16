\ProvidesPackage{titlepage}[2007/01/10 v1.1 Thesis TitlePage]
% Copyright Inria/Apics (Jos\'e Grimm) 2006-2007
%% $Id: titlepage.plt,v 2.3 2008/04/01 17:02:06 grimm Exp $
\newcommand\thesetitre[1]{\xbox{thesetitre}{#1}}
\newcommand\membre[4][]{%
\xbox{membre}{#1\XMLaddatt{type}{#2}\XMLaddatt{prenom}{#3}\XMLaddatt{nom}{#4}}}
\newenvironment{jury}{\begin{xmlelement*}{jury}}{\end{xmlelement*}}

\newenvironment{metadata}{\begin{xmlelement*}{metadata}}{\end{xmlelement*}}

\newcommand\lieuthese[1]{\xbox{lieuthese}{#1}}
\newcommand\titrefrancais[1]{\xbox{titrefrancais}{#1}}
\newcommand\typethese[1]{\xbox{typethese}{#1}}
\newcommand\lieuthesesuite[1]{\xbox{lieuthesesuite}{#1}}
\newcommand\datesoumission[1]{\xbox{datesoumission}{#1}}
\newcommand\directeur[2]{\xbox{directeur}{#1 #2}}
\newcommand\doctorant[2]{\xbox{doctorant}{#1 #2}}

\newcommand{\dedicace}[1]
{\xbox{dedicace}{#1}}
