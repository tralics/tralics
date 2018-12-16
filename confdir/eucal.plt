% -*- latex -*-
\ProvidesPackage{eucal}[2007/11/21 v1.2 Euler Script font]
\DeclareOption{psamsfonts}{}
\DeclareOption{mathcal}{}
\DeclareOption{mathscr}{\let\mathscr\mathcal}
\ProcessOptions\relax

\let\EuScript\mathcal

\endinput


