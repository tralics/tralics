%% -*- latex -*-
%% This is file `natbib.plt', a simplified version of natbib.sty for tralics
%% Copyright 2006 INRIA Jose Grimm

%% Copyright 1993-2003 Patrick W Daly, for the natbib.sty file 
\ProvidesPackage{natbib}[2008/05/18 v1.3 ]
% $Id: natbib.plt,v 2.3 2008/05/19 06:42:15 grimm Exp $

\let\@tralics@cite\cite

\ifx\natcite\undefined

\def\cite@type#1{\@ifnextchar[{\cite@typex{#1}}{\cite@typey{#1}}}
\def\cite@typey#1#2{{\def\cite@@type{#1}\@tralics@cite{#2}}}
\def\cite@typex#1[#2]{\@ifnextchar[{\cite@typexx{#1}{#2}}{\cite@typeyy{#1}{#2}}}
\def\cite@typeyy#1#2#3{{\def\cite@@type{#1}\@tralics@cite[#2]{#3}}}
\def\cite@typexx#1#2[#3]#4{{\def\cite@@type{#1}\@tralics@cite[#2 and #3]{#4}}}

\else
\def\cite@type#1{\natcite[#1]}
\fi


% Defining the citation style of a given bib style:
 % Use \bibpunct (in the preamble only) with 6 mandatory arguments:
 %    1. opening bracket for citation
 %    2. closing bracket
 %    3. citation separator (for multiple citations in one \cite)
 %    4. the letter n for numerical styles, s for superscripts
 %        else anything for author-year
 %    5. punctuation between authors and date
 %    6. punctuation between years (or numbers) when common authors missing
 % One optional argument is the character coming before post-notes. It
 %   appears in square braces before all other arguments. May be left off.
 % Example (and default) \bibpunct[, ]{(}{)}{;}{a}{,}{,}

% These are the variables needed
\newif\ifNAT@numbers \NAT@numbersfalse
\newif\ifNAT@super \NAT@superfalse

%% These 4 commands are define in the Tralics kernel
\providecommand\NAT@open{(} 
\providecommand\NAT@close{)}
\providecommand\NAT@cmt{, }
\providecommand\NAT@sep{;}
\newcommand\NAT@aysep{,}
\newcommand\NAT@yrsep{,}


%% Tralics action in case a style is given
\def\NAT@insertstyle{
  \XMLaddatt[3]{natopen}{\NAT@open}
  \XMLaddatt[3]{natclose}{\NAT@close}
  \XMLaddatt[3]{natsep}{\NAT@sep}
  \XMLaddatt[3]{nataysep}{\NAT@aysep}
  \XMLaddatt[3]{natyrsep}{\NAT@yrsep}
  \XMLaddatt[3]{natcmt}{\NAT@cmt}
  \XMLaddatt[3]{natpos}{\ifNAT@numbers n\else\ifNAT@super s\else a\fi\fi}
}

%% The user command
\newcommand\bibpunct[7][, ]{%
  \gdef\NAT@open{#2}%
  \gdef\NAT@close{#3}%
  \gdef\NAT@sep{#4}%
  \global\NAT@numbersfalse
  \ifx #5n\global\NAT@numberstrue
  \else
  \ifx #5s\global\NAT@numberstrue\global\NAT@supertrue
  \fi\fi
  \gdef\NAT@aysep{#6}%
  \gdef\NAT@yrsep{#7}%
  \gdef\NAT@cmt{#1}%
  \global\let\bibstyle\@gobble
}

 % Define citation punctuation for some author-year styles
 % One may add and delete at this point
 % Or put additions into local configuration file natbib.cfg
\newcommand\bibstyle@chicago{\bibpunct{(}{)}{;}{a}{,}{,}}
\newcommand\bibstyle@named{\bibpunct{[}{]}{;}{a}{,}{,}}
\newcommand\bibstyle@agu{\bibpunct{[}{]}{;}{a}{,}{,~}}%Amer. Geophys. Union
\newcommand\bibstyle@egs{\bibpunct{(}{)}{;}{a}{,}{,}}%Eur. Geophys. Soc.
\newcommand\bibstyle@agsm{\bibpunct{(}{)}{,}{a}{}{,}\gdef\harvardand{\&}}
\newcommand\bibstyle@kluwer{\bibpunct{(}{)}{,}{a}{}{,}\gdef\harvardand{\&}}
\newcommand\bibstyle@dcu{\bibpunct{(}{)}{;}{a}{;}{,}\gdef\harvardand{and}}
\newcommand\bibstyle@aa{\bibpunct{(}{)}{;}{a}{}{,}} %Astronomy & Astrophysics
\newcommand\bibstyle@pass{\bibpunct{(}{)}{;}{a}{,}{,}}%Planet. & Space Sci
\newcommand\bibstyle@anngeo{\bibpunct{(}{)}{;}{a}{,}{,}}%Annales Geophysicae
\newcommand\bibstyle@nlinproc{\bibpunct{(}{)}{;}{a}{,}{,}}%Nonlin.Proc.Geophys.
 % Define citation punctuation for some numerical styles
\newcommand\bibstyle@cospar{\bibpunct{/}{/}{,}{n}{}{}%
     \gdef\NAT@biblabelnum##1{##1.}}
\newcommand\bibstyle@esa{\bibpunct{(Ref.~}{)}{,}{n}{}{}%
     \gdef\NAT@biblabelnum##1{##1.\hspace{1em}}}
\newcommand\bibstyle@nature{\bibpunct{}{}{,}{s}{}{\textsuperscript{,}}%
     \gdef\NAT@biblabelnum##1{##1.}}
 % The standard LaTeX styles
\newcommand\bibstyle@plain{\bibpunct{[}{]}{,}{n}{}{,}}
\let\bibstyle@alpha=\bibstyle@plain
\let\bibstyle@abbrv=\bibstyle@plain
\let\bibstyle@unsrt=\bibstyle@plain
% The author-year modifications of the standard styles
\newcommand\bibstyle@plainnat{\bibpunct{[}{]}{,}{a}{,}{,}}
\let\bibstyle@abbrvnat=\bibstyle@plainnat
\let\bibstyle@unsrtnat=\bibstyle@plainnat


%%% -------------------------------------------------------------

\let\bibstyle=\@gobble %% standard latex

\newif\ifNAT@openbib \NAT@openbibfalse
\newif\ifNAT@longnames\NAT@longnamesfalse

\DeclareOption{numbers}{\NAT@numberstrue
   \ExecuteOptions{square,comma,nobibstyle}}
\DeclareOption{super}{\NAT@supertrue\NAT@numberstrue
   \renewcommand\NAT@open{}\renewcommand\NAT@close{}
   \ExecuteOptions{nobibstyle}}
\DeclareOption{authoryear}{\NAT@numbersfalse
   \ExecuteOptions{round,colon,bibstyle}}
\DeclareOption{round}{%
  \renewcommand\NAT@open{(} \renewcommand\NAT@close{)}
  \ExecuteOptions{nobibstyle}}
\DeclareOption{square}{%
  \renewcommand\NAT@open{[} \renewcommand\NAT@close{]}
  \ExecuteOptions{nobibstyle}}
\DeclareOption{angle}{%
  \renewcommand\NAT@open{$<$} \renewcommand\NAT@close{$>$}
  \ExecuteOptions{nobibstyle}}
\DeclareOption{curly}{%
  \renewcommand\NAT@open{\{} \renewcommand\NAT@close{\}}
  \ExecuteOptions{nobibstyle}}
\DeclareOption{comma}{\renewcommand\NAT@sep{,}
  \ExecuteOptions{nobibstyle}}
\DeclareOption{colon}{\renewcommand\NAT@sep{;}
   \ExecuteOptions{nobibstyle}}
\DeclareOption{nobibstyle}{\let\bibstyle=\@gobble}
\DeclareOption{bibstyle}{\let\bibstyle=\@citestyle}
\DeclareOption{openbib}{\NAT@openbibtrue}
\DeclareOption{sectionbib}{\def\NAT@sectionbib{on}}
\DeclareOption{sort}{\def\NAT@sort{1}}
\DeclareOption{sort&compress}{\def\NAT@sort{2}}
\DeclareOption{longnamesfirst}{\NAT@longnamestrue}
\DeclareOption{nonamebreak}{\def\NAT@nmfmt#1{\mbox{\NAT@up#1}}}

\def\NAT@sort{0}
\newcommand\citetext[1]{\NAT@open#1\NAT@close}

\def\NAT@nmfmt#1{{\NAT@up#1}}
\renewcommand\bibstyle[1]{\@ifundefined{bibstyle@#1}{\relax}
     {\csname bibstyle@#1\endcsname}}
\AtBeginDocument{\global\let\bibstyle=\@gobble}
\let\@citestyle\bibstyle
\newcommand\citestyle[1]{\@citestyle{#1}\let\bibstyle\@gobble}
\@onlypreamble{\citestyle}\@onlypreamble{\@citestyle}
\@onlypreamble{\bibpunct}
\ProcessOptions


\def\citeyear{\cite@type{year}}
\def\citeyearpar{\cite@type{yearpar}}
\def\citeauthor{\@ifstar{\cite@type{fullauthor}}{\cite@type{author}}}
\def\citefullauthor{\cite@type{fullauthor}}

\def\citep{\@ifstar{\cite@type{citepstar}}{\cite@type{citep}}}
\def\citealt{\@ifstar{\cite@type{citealtstar}}{\cite@type{citealt}}}
\def\citealp{\@ifstar{\cite@type{citealpstar}}{\cite@type{citealp}}}

\def\Citet{\@ifstar{\cite@type{Citetstar}}{\cite@type{Citet}}}
\def\Citep{\@ifstar{\cite@type{Citepstar}}{\cite@type{Citep}}}
\def\Citealt{\@ifstar{\cite@type{Citealtstar}}{\cite@type{Citealt}}}
\def\Citealp{\@ifstar{\cite@type{Citealpstar}}{\cite@type{Citealp}}}
\def\Citeauthor{\@ifstar{\cite@type{Fullauthor}}{\cite@type{Author}}}

\InputIfFileExists{natbib.cfg}
       {\typeout{Local config file natbib.cfg used}}{}

\NAT@insertstyle
\endinput


%%
 % This is either to be made up manually, or to be generated by an
 % appropriate .bst file with BibTeX.
 %                            Author-year mode     ||   Numerical mode
 % Then, \citet{key}  ==>>  Jones et al. (1990)    ||   Jones et al. [21]
 %       \citep{key}  ==>> (Jones et al., 1990)    ||   [21]
 % Multiple citations as normal:
 % \citep{key1,key2}  ==>> (Jones et al., 1990; Smith, 1989) || [21,24]
 %                           or  (Jones et al., 1990, 1991)  || [21,24]
 %                           or  (Jones et al., 1990a,b)     || [21,24]
 % \cite{key} is the equivalent of \citet{key} in author-year mode
 %                         and  of \citep{key} in numerical mode
 % Full author lists may be forced with \citet* or \citep*, e.g.
 %       \citep*{key}      ==>> (Jones, Baker, and Williams, 1990)
 % Optional notes as:
 %   \citep[chap. 2]{key}    ==>> (Jones et al., 1990, chap. 2)
 %   \citep[e.g.,][]{key}    ==>> (e.g., Jones et al., 1990)
 %   \citep[see][pg. 34]{key}==>> (see Jones et al., 1990, pg. 34)
 %  (Note: in standard LaTeX, only one note is allowed, after the ref.
 %   Here, one note is like the standard, two make pre- and post-notes.)
 %   \citealt{key}          ==>> Jones et al. 1990
 %   \citealt*{key}         ==>> Jones, Baker, and Williams 1990
 %   \citealp{key}          ==>> Jones et al., 1990
 %   \citealp*{key}         ==>> Jones, Baker, and Williams, 1990
 % Additional citation possibilities (both author-year and numerical modes)
 %   \citeauthor{key}       ==>> Jones et al.
 %   \citeauthor*{key}      ==>> Jones, Baker, and Williams
 %   \citeyear{key}         ==>> 1990
 %   \citeyearpar{key}      ==>> (1990)
 %   \citetext{priv. comm.} ==>> (priv. comm.)
 % Note: full author lists depends on whether the bib style supports them;
 %       if not, the abbreviated list is printed even when full requested.
 %
 % For names like della Robbia at the start of a sentence, use
 %   \Citet{dRob98}         ==>> Della Robbia (1998)
 %   \Citep{dRob98}         ==>> (Della Robbia, 1998)
 %   \Citeauthor{dRob98}    ==>> Della Robbia
 %
 %
 % Citation aliasing is achieved with
 %   \defcitealias{key}{text}
 %   \citetalias{key}  ==>> text
 %   \citepalias{key}  ==>> (text)
 %
 
 %
 % To make this automatic for a given bib style, named newbib, say, make
 % a local configuration file, natbib.cfg, with the definition
 %   \newcommand{\bibstyle@newbib}{\bibpunct...}
 % Then the \bibliographystyle{newbib} will cause \bibstyle@newbib to
 % be called on THE NEXT LATEX RUN (via the aux file).
 %
 % Such preprogrammed definitions may be invoked in the text (preamble only)
 %  by calling \citestyle{newbib}. This is only useful if the style specified
 %  differs from that in \bibliographystyle.
 %
 % With \citeindextrue and \citeindexfalse, one can control whether the
 % \cite commands make an automatic entry of the citation in the .idx
 % indexing file. For this, \makeindex must also be given in the preamble.
 %
 % LaTeX2e Options: (for selecting punctuation)
 %   round  -  round parentheses are used (default)
 %   square -  square brackets are used   [option]
 %   curly  -  curly braces are used      {option}
 %   angle  -  angle brackets are used    <option>
 %   colon  -  multiple citations separated by colon (default)
 %   comma  -  separated by comma
 %   authoryear - selects author-year citations (default)
 %   numbers-  selects numerical citations
 %   super  -  numerical citations as superscripts
 %   sort   -  sorts multiple citations according to order in ref. list
 %   sort&compress   -  like sort, but also compresses numerical citations
 %   longnamesfirst  -  makes first citation full author list
 %   sectionbib - puts bibliography in a \section* instead of \chapter*
 % Punctuation so selected dominates over any predefined ones.
 % LaTeX2e options are called as, e.g.
 %        \usepackage[square,comma]{natbib}
 % LaTeX the source file natbib.dtx to obtain more details
 % or the file natnotes.tex for a brief reference sheet.
 %-----------------------------------------------------------


\newcommand\shortcites[1]{%
  \@bsphack\@for\@citeb:=#1\do
  {\edef\@citeb{\expandafter\@firstofone\@citeb}%
   \global\@namedef{bv@\@citeb\@extra@b@citeb}{}}\@esphack}

\newcommand\defcitealias[2]{%
   \@ifundefined{al@#1\@extra@b@citeb}{}
   {\PackageWarning{natbib}{Overwriting existing alias for citation #1}}
   \@namedef{al@#1\@extra@b@citeb}{#2}}
\DeclareRobustCommand\citetalias{\begingroup
   \NAT@swafalse\def\NAT@ctype{3}\NAT@parfalse\NAT@citetp}
\DeclareRobustCommand\citepalias{\begingroup
   \NAT@swatrue\def\NAT@ctype{3}\NAT@partrue\NAT@citetp}

