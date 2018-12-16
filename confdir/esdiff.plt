%%%  -*- latex -*-
%% Package for the notation of derivatives.
%% Eddie Saudrais, 08/09/1999.
\ProvidesPackage{esdiff}[2008/11/04 v1.0 Derivatives]


%% $Id: esdiff.plt,v 2.1 2008/11/13 14:13:38 grimm Exp $
%% TRALICS, copyright (C) INRIA/apics (Jose' Grimm) 2008
%% Licensed under the CeCILL free software license 
%% (See the file COPYING in the main directory for details)

%% Changes JG; The kern is much too large for Firefox
\def\ES@kern{\mkern-7mu}
\def\ES@kern{}

\newcommand*{\ES@taille}[1]{\displaystyle{#1}}
\newcommand*{\ES@difint}{\mkern 0mu}
\newcommand*{\ES@derpint}{\mkern 0mu}
\newcommand*{\ES@croisint}{\mkern 0mu}
\newcommand*{\ES@dop}{\mathrm{d}}
\DeclareOption{display}{\renewcommand*{\ES@taille}[1]{\displaystyle{#1}}}
\DeclareOption{text}{\renewcommand*{\ES@taille}[1]{\textstyle{#1}}}
\DeclareOption{roman}{\renewcommand*{\ES@dop}{\ensuremath{\mathrm{d}}}}
\DeclareOption{italic}{\renewcommand*{\ES@dop}{\ensuremath{d}}}
\DeclareOption{thin}{\renewcommand*{\ES@difint}{\ensuremath{\,}}}
\DeclareOption{med}{\renewcommand*{\ES@difint}{\ensuremath{\;}}}
\DeclareOption{big}{\renewcommand*{\ES@difint}{\ensuremath{\:}}}
\DeclareOption{thinp}{\renewcommand*{\ES@derpint}{\ensuremath{\,}}}
\DeclareOption{medp}{\renewcommand*{\ES@derpint}{\ensuremath{\;}}}
\DeclareOption{bigp}{\renewcommand*{\ES@derpint}{\ensuremath{\:}}}
\DeclareOption{thinc}{\renewcommand*{\ES@croisint}{\ensuremath{\,}}}
\DeclareOption{medc}{\renewcommand*{\ES@croisint}{\ensuremath{\;}}}
\DeclareOption{bigc}{\renewcommand*{\ES@croisint}{\ensuremath{\:}}}
\ProcessOptions\relax
\newcommand*{\diff}{}
\def\diff{\@ifstar{\ES@diffstar}{\ES@diffnostar}}
\def\ES@diffstar{\@ifnextchar[{\ES@diffstar@i}{\ES@diffstar@ii}}
\def\ES@diffstar@i[#1]#2#3#4{%
\mathchoice{%
\left(\frac{\ES@dop^{#1}\ES@difint#2}%
{\ES@dop\ES@difint#3^{#1}}\right)_{\ES@kern#4}}%
{\ES@taille{\left(\frac{\ES@dop^{#1}\ES@difint#2}%
{\ES@dop\ES@difint#3^{#1}}\right)_{\ES@kern#4}}}%
{\scriptstyle{\left(\frac{\ES@dop^{#1}\ES@difint#2}%
{\ES@dop\ES@difint#3^{#1}}\right)_{\ES@kern#4}}}%
{\scriptstyle{\left(\frac{\ES@dop^{#1}\ES@difint#2}%
{\ES@dop\ES@difint#3^{#1}}\right)_{\ES@kern#4}}}}
\def\ES@diffstar@ii#1#2#3{%
\mathchoice{\left(\frac{\ES@dop\ES@difint#1}%
{\ES@dop\ES@difint#2}\right)_{\ES@kern#3}}%
{\ES@taille{\left(\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}%
\right)_{\ES@kern#3}}}{\scriptstyle{\left(%
\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}\right)%
_{\ES@kern#3}}}%
{\scriptstyle{\left(\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}\right)%
_{\ES@kern#3}}}}
\def\ES@diffnostar{\@ifnextchar[{\ES@diffnostar@i}{\ES@diffnostar@ii}}
\def\ES@diffnostar@i[#1]#2#3{\mathchoice{\frac{\ES@dop^{#1}\ES@difint#2}%
{\ES@dop\ES@difint#3^{#1}}}%
{\ES@taille{\frac{\ES@dop^{#1}\ES@difint#2}{\ES@dop\ES@difint#3^{#1}}}}%
{\scriptstyle{\frac{\ES@dop^{#1}\ES@difint#2}{\ES@dop\ES@difint#3^{#1}}}}%
{\scriptstyle{\frac{\ES@dop^{#1}\ES@difint#2}{\ES@dop\ES@difint#3^{#1}}}}}
\def\ES@diffnostar@ii#1#2{\mathchoice%
{\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}}%
{\ES@taille{\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}}}%
{\scriptstyle{\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}}}%
{\scriptstyle{\frac{\ES@dop\ES@difint#1}{\ES@dop\ES@difint#2}}}}
\newcommand*{\diffp}{}
\def\diffp{\@ifstar{\ES@diffpstar}{\ES@diffpnostar}}
\def\ES@diffpstar{\@ifnextchar[{\ES@diffpstar@i}{\ES@diffpstar@ii}}
\def\ES@diffpstar@i[#1]#2#3#4{%
\mathchoice{\left(\frac{\partial^{#1}%
\ES@derpint#2}{\partial\ER@derpint#3^{#1}}\right)_{\ES@kern#4}}%
{\ES@taille{\left(\frac{\partial^{#1}\ES@derpint#2}{\partial\ES@derpint#3^{#1}}%
\right)_{\ES@kern#4}}}%
{\scriptstyle{\left(\frac{\partial^{#1}\ES@derpint#2}%
{\partial\ES@derpint#3^{#1}}%
\right)_{\ES@kern#4}}}%
{\scriptstyle{\left(\frac{\partial^{#1}\ES@derpint#2}%
{\partial\ES@derpint#3^{#1}}%
\right)_{\ES@kern#4}}}}
\def\ES@diffpstar@ii#1#2#3{\mathchoice{\left({\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}\right)_{\ES@kern#3}}%
            {\ES@taille{\left({\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}\right)_{\ES@kern#3}}}%
            {\scriptstyle{\left({\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}\right)_{\ES@kern#3}}}%
            {\scriptstyle{\left({\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}\right)_{\ES@kern#3}}}}
\def\ES@diffpnostar{\@ifnextchar[{\ES@diffpnostar@i}{\ES@diffpnostar@ii}}
\def\ES@diffpnostar@i[#1]#2#3{%
\mathchoice{\frac{\partial^{#1}\ES@derpint#2}%
{\partial\ES@derpint#3^{#1}}}%
{\ES@taille{\frac{\partial^{#1}\ES@derpint#2}{\partial\ES@derpint#3^{#1}}}}%
{\scriptstyle{\frac{\partial^{#1}\ES@derpint#2}{\partial\ES@derpint#3^{#1}}}}%
{\scriptstyle{\frac{\partial^{#1}\ES@derpint#2}{\partial\ES@derpint#3^{#1}}}}}
\def\ES@diffpnostar@ii#1#2{\mathchoice{\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}%
            {\ES@taille{\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}}%
            {\scriptstyle{\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}}%
            {\scriptstyle{\begingroup
            \toks0={}\count0=0
            \ES@degree #2\ES@degree
            \frac{\partial\ifnum\count0>1^{\the\count0 }\fi\ES@derpint#1}%
            {\the\toks0}%
            \endgroup}}}
\def\ES@degree #1{\ifx #1\ES@degree \expandafter\ES@stopd
                      \else \expandafter\ES@addd \fi #1^1$#1\ES@addd}
\def\ES@stopd #1\ES@addd{}
\def\ES@addd #1^#2#3$#4\ES@addd{\advance\count0 #2
                          \toks0=\expandafter{\the\toks0%
                           {\partial\ES@derpint #4}%
                          \ES@croisint}\ES@degree}
\endinput
%%
%% End of file `esdiff.sty'.
