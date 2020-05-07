Examples of transcripts
=======================

First example file for the titlepage command

.. container:: log_out

   ::

      Trace of tralics 1.9t for file tpa.tex
      Start compilation: 29/1/2004 11:42:47
      Starting xml processing for tpa.
      <!-- S1 -->
      Trying config file from user specs:../config_tpa
      Read configuration file ../config_tpa.
      Configuration file has type \documentclass
      Seen \documentclass TPA
      <!-- S2 -->
      Potential type is TPA
      Defined type: default
      Defined type: TPA
      Defined type: unused
      Defined type: eurotex
      Using type TPA
      dtd is tpa from tpa.dtd (standard mode)
      Ok with the config file, dealing with the TeX file...
      <!-- S3 -->
      There are 38 lines
      Starting translation
      [1] %% Begin bootstrap commands for latex
      <!-- skip over bootstrap -->
      [21] %% End bootstrap commands for latex
      Defining \InsertTitlepage as \TitlePageCmd 0
         main <titlepage att1='foo' att2='foo2' a1='b1' a2='b2' -- from-tpa='ok' from_type='OK'/>
      Defining \AInsertTitlepage as \TitlePageCmd 1
         main <titlepage att1='foo' att2='foo2'  -- />
      Defining \title as \TitlePageCmd 2
         usual <title-element tea='tea-val'/>
      Defining \utitle as \TitlePageCmd 3
         usual <utitle-element tea='utea-val'/>
      Defining \author as \TitlePageCmd 4
         list <autL al = 'alval' bl = 'blval'/> and <aut auta='autaval'/>
      Defining \uauthor as \TitlePageCmd 5
         list <uautL al='alval' bl='blval'/> and <aut auta='autaval'/>
      Defining \Eabstract as \TitlePageCmd 6
         usual <abstract ab='AB1'/> (flags +par +env)
      Defining \eabstract as \TitlePageCmd 7
         usual <abstract ab='AB2'/> (flags +env)
      <!-- skip over 22 commands like these-->
      Defining \cmda as \TitlePageCmd 30
         usual <cmdA/> (flags +A)
      Defining \cmdb as \TitlePageCmd 31
         usual <cmdB/> (flags +B)
      [1] \cmdb{\cmdBval}<!--another insert-->
      ++ End of virtual file.
      Defining \cmdc as \TitlePageCmd 32
         usual <cmdC/> (flags +C)
      [1] \def\cmdAval{CMDA}
      {\def}
      {\def \cmdAval ->CMDA}
      Character sequence:  .
      [2] \def\cmdBval{\invalid}
      {\def}
      {\def \cmdBval ->\invalid }
      Character sequence:  .
      [3] \def\cmdCval{\invalid}
      {\def}
      {\def \cmdCval ->\invalid }
      Character sequence:  .
      [4] \documentclass{TPA}
      {\documentclass}
      {Push argument 1}
      Character sequence: TPA.
      {Text:TPA}
      {Pop 1: document_v argument_v}
      [1] \cmda{\cmdAval}<!-- Insert 1 -->
      {\titlepage 30}
      {\titlepage 30=\cmda}
      {Push cmdA 1}
      \cmdAval->CMDA
      Character sequence: CMDA.
      {Text:CMDA}
      {Pop 1: document_v cmdA_t}
      Character sequence:  .
      [88]   \def\recurse{\recurse\recurse}<!-- Insert 2 -->
      {\def}
      {\def \recurse ->\recurse \recurse }
      Character sequence:  .
      [5] \def\cmdAval{\invalid}<!-- A destroyed here -->
      {\def}
      {\def \cmdAval ->\invalid }
      Character sequence:  .
      [6] \def\Title{Titlepage customisation in Tralics}
      {\def}
      {\def \Title ->Titlepage customisation in Tralics}
      Character sequence:  .
      [7] 
      {\par}
      [8] 
      {\par}
      [9] \title{\Title}
      {\titlepage 2}
      {\titlepage 2=\title}
      {Push title-element 1}
      \Title->Titlepage customisation in Tralics
      Character sequence: Titlepage customisation in Tralics.
      {Text:Titlepage customisation in Tralics}
      {Pop 1: document_v title-element_t}
      Character sequence:  .
      [10] \author{Jos� Grimm}
      <!-- SKIP -->
      [20] \begin{Eabstract}
      {\begin}
      {\begin Eabstract}
      +stack: level + 2 for environment
      {\titlepage 6}
      {\titlepage 6=\Eabstract}
      [21] This is an abstract with \par some paragraphs in it\par ok ?
      [22] \end{Eabstract}
      {Push abstract 1}
      {Push p 2}
      Character sequence:  This is an abstract with .
      {\par}
      {Text:
      This is an abstract with }
      {Pop 2: document_v abstract_v p_v}
      {Push p 2}
      Character sequence: some paragraphs in it.
      {\par}
      {Text:some paragraphs in it}
      {Pop 2: document_v abstract_v p_v}
      {Push p 2}
      Character sequence: ok ? .
      {Text:ok ?
      }
      {Pop 2: document_v abstract_v p_v}
      {Pop 1: document_v abstract_v}
      {\end}
      {\end Eabstract}
      {\relax}
      {\endgroup (for env)}
      +stack: ending environment Eabstract; resuming document.
      +stack: level - 2 for environment
      <!-- end env... -->
      Character sequence:  .
      [23] 
      <!-- skip -->
      [31] \begin{document}
      {\begin}
      {\begin document}
      +stack: level + 2 for environment
      {\document}
      +stack: ending environment document; resuming document.
      +stack: level - 2 for environment
      +stack: level set to 1
      atbegindocumenthook= \cmdb {\cmdBval }<!-- document hook -->
      \par \let \AtBeginDocument \@notprerr 
      <!-- end of document hook -->
      {\titlepage 31}
      {\titlepage 31=\cmdb}
      {Push cmdB 1}
      \cmdBval->CMDB
      Character sequence: CMDB.
      {Text:CMDB}
      {Pop 1: document_v cmdB_t}
      <!-- skip -->
      [35] \InsertTitlepage
      {\titlepage 0}
      {\titlepage 0=\InsertTitlepage}
      Error signaled at line 35:
      No value given for command \cmdp.
      [1] \cmdc{\cmdCval}<!-- \cmdc is evaluated here -->
      ++ End of virtual file.
      {\titlepage 32}
      {\titlepage 32=\cmdc}
      <!-- SKIP -->
      [37] \end{document}
      {\end}
      {\end document}
      +stack: level + 2 for environment
      {\enddocument}
      {\endinput}
      {\enddocument}
      {Pop (module) 1: document_v p_v}
      {\endgroup (for env)}
      +stack: ending environment document; resuming document.
      +stack: level - 2 for environment
      Character sequence:  .
      {Text:
      }
      Seen 0 bibliographic entries
      <!-- SKIP -->

Second example file for the titlepage command.

.. container:: log_out

   ::

        1 Transcript file of tralics 2.8 for file bo.tex
        2 Copyright INRIA/MIAOU/APICS 2002-2006, Jos\'e Grimm
        3 Tralics is licensed under the CeCILL Free Software Licensing Agreement
        4 Start compilation: 1/8/2006 10:58:10
        5 Starting xml processing for bo.
        6 Configuration file identification: standard $ Revision: 2.24 $
        7 Read configuration file /user/grimm/home/cvs/tralics/.tralics_rc.
        8 Configuration file has type \documentclass
        9 Seen \documentclass cedram
       10 Potential type is cedram
       11 Defined type: std
       12 Configuration file identification: cedram.tcf $ Revision: 1.1 $
       13 Read tcf file for type: cedram.tcf
       14 Using type cedram
       15 dtd is cedram from cedram.dtd (standard mode)
       16 Ok with the config file, dealing with the TeX file...
       17 There are 47 lines
       18 Starting translation
       19 {\countdef \m@ne=\count10}
       20 {\countdef \count@=\count255}
       21 {\countdef \c@page=\count0}
       22 {\dimendef \dimen@=\dimen0}
       23 {\dimendef \epsfxsize=\dimen11}
       24 {\dimendef \epsfysize=\dimen12}
       25 {\chardef \@ne=\char1}
       26 {\chardef \tw@=\char2}
       27 {\chardef \active=\char13}
       28 {\dimendef \z@=\dimen13}
       29 {\dimendef \oddsidemargin=\dimen14}
       30 {\dimendef \evensidemargin=\dimen15}
       31 {\dimendef \leftmargin=\dimen16}
       32 {\dimendef \rightmargin=\dimen17}
       33 {\dimendef \leftmargini=\dimen18}
       34 {\dimendef \leftmarginii=\dimen19}
       35 {\dimendef \leftmarginiii=\dimen20}
       36 {\dimendef \leftmarginiv=\dimen21}
       37 {\dimendef \leftmarginv=\dimen22}
       38 {\dimendef \leftmarginvi=\dimen23}
       39 {\dimendef \itemindent=\dimen24}
       40 {\dimendef \labelwidth=\dimen25}
       41 {\dimendef \fboxsep=\dimen26}
       42 {\dimendef \fboxrule=\dimen27}
       43 {\skipdef \itemsep=\skip11}
       44 {\skipdef \labelsep=\skip12}
       45 {\skipdef \parsep=\skip13}
       46 {\skipdef \fill=\skip14}
       47 {\countdef \c@bottomnumber=\count11}
       48 [1] %% Begin bootstrap commands for latex
       49 [2] \def\lq{`}\def\rq{'}\def\lbrack{[}\def\rbrack{]}%
       50 {\def}
       51 {\def \lq ->`}
       52 {\def}
       53 {\def \rq ->'}
       54 {\def}
       55 {\def \lbrack ->[}
       56 {\def}
       57 {\def \rbrack ->]}
       58 [3] \z@=0pt\m@ne=-1 \fboxsep = 3pt %
       59 {\dimen13}
       60 +scanint for \z@->0
       61 +scandimen for \z@ ->0.0pt
       62 {\count10}
       63 +scanint for \m@ne->-1
       64 {\dimen26}
       65 +scanint for \fboxsep->3
       66 +scandimen for \fboxsep ->3.0pt
       67 [4] \c@page=1 \fill = 0pt plus 1fill
       68 {\count0}
       69 +scanint for \c@page->1
       70 {\skip14}
       71 +scanint for \fill->0
       72 +scandimen for \fill ->0.0pt
       73 +scanint for \fill->1
       74 [5] \def\sloppypar{\par\sloppy}\def\endsloppypar{\par}%
       75 +scandimen for \fill ->1.0pt
       76 {scanglue 0.0pt plus 1.0fill}
       77 {\def}
       78 {\def \sloppypar ->\par \sloppy }
       79 {\def}
       80 {\def \endsloppypar ->\par }
       81 [6] {\catcode`\_=13\global\let_\_}%
       82 {begin-group character {}
       83 +stack: level + 2 for brace
       84 {\catcode}
       85 +scanint for \catcode->95
       86 +scanint for \catcode->13
       87 {\global}
       88 {\global\let}
       89 {\let _ \_}
       90 {end-group character }}
       91 +stack: restoring integer value 8 for \catcode95
       92 +stack: level - 2 for brace
       93 [7] {\catcode`\#=13\global\let#\#}%
       94 {begin-group character {}
       95 +stack: level + 2 for brace
       96 {\catcode}
       97 +scanint for \catcode->35
       98 +scanint for \catcode->13
       99 {\global}
      100 {\global\let}
      101 {\let # \#}
      102 {end-group character }}
      103 +stack: restoring integer value 6 for \catcode35
      104 +stack: level - 2 for brace
      105 [8] {\catcode`\&=13\global\def&{\char`&}}%
      106 {begin-group character {}
      107 +stack: level + 2 for brace
      108 {\catcode}
      109 +scanint for \catcode->38
      110 +scanint for \catcode->13
      111 {\global}
      112 {\global\def}
      113 {\def & ->\char `&}
      114 {end-group character }}
      115 +stack: restoring integer value 4 for \catcode38
      116 +stack: level - 2 for brace
      117 [9] \catcode`\~=13\def~{\nobreakspace}%
      118 {\catcode}
      119 +scanint for \catcode->126
      120 +scanint for \catcode->13
      121 {\def}
      122 {\def ~ ->\nobreakspace }
      123 [10] \def\null{\hbox{}}%
      124 {\def}
      125 {\def \null ->\hbox {}}
      126 [11] \def\urlfont{}%
      127 {\def}
      128 {\def \urlfont ->}
      129 [12] \newenvironment{cases}{\left\{\begin{array}{ll}}{\end{array}\right.}%
      130 {\newenvironment}
      131 {\newenvironment cases}
      132 [13] \def\binom{\genfrac()\z@{}}\def\dbinom{\genfrac(){0pt}0}\def\tbinom{\genfrac(){0pt}1}%
      133 {\def}
      134 {\def \binom ->\genfrac ()\z@ {}}
      135 {\def}
      136 {\def \dbinom ->\genfrac (){0pt}0}
      137 {\def}
      138 {\def \tbinom ->\genfrac (){0pt}1}
      139 [14] \def\space{ }
      140 {\def}
      141 {\def \space -> }
      142 [15] \def\thinspace{ }%
      143 {\def}
      144 {\def \thinspace -> }
      145 [16] \long\def \@gobble #1{}\long\def \@gobbletwo #1#2{}
      146 {\long}
      147 {\long\def}
      148 {\def \@gobble #1->}
      149 {\long}
      150 {\long\def}
      151 {\def \@gobbletwo #1#2->}
      152 [17] \def\markboth#1#2{\gdef\@themark{{#1}}\mark{{#1}{#2}}}
      153 {\def}
      154 {\def \markboth #1#2->\gdef \@themark {{#1}}\mark {{#1}{#2}}}
      155 [18] \def\markright#1{\expandafter\markboth\@themark{#1}}
      156 {\def}
      157 {\def \markright #1->\expandafter \markboth \@themark {#1}}
      158 [19] \def\xscale{1.0}\def\yscale{1.0}\def\xscaley{0.0}\def\yscalex{0.0}
      159 {\def}
      160 {\def \xscale ->1.0}
      161 {\def}
      162 {\def \yscale ->1.0}
      163 {\def}
      164 {\def \xscaley ->0.0}
      165 {\def}
      166 {\def \yscalex ->0.0}
      167 [20] \def\@height{height} \def\@depth{depth} \def\@width{width}
      168 {\def}
      169 {\def \@height ->height}
      170 {\def}
      171 {\def \@depth ->depth}
      172 {\def}
      173 {\def \@width ->width}
      174 [21] \def\@minus{minus}\def\@plus{plus}\def\hb@xt@{\hbox to}
      175 {\def}
      176 {\def \@minus ->minus}
      177 {\def}
      178 {\def \@plus ->plus}
      179 {\def}
      180 {\def \hb@xt@ ->\hbox to}
      181 [22] \def\stretch#1{\z@ \@plus #1fill\relax}
      182 {\def}
      183 {\def \stretch #1->\z@ \@plus #1fill\relax }
      184 [23] \theoremstyle{plain}\theoremheaderfont{\bfseries}
      185 {\theoremstyle}
      186 {\theoremheaderfont}
      187 [24] \def\@namedef#1{\expandafter\def\csname #1\endcsname}
      188 {\def}
      189 {\def \@namedef #1->\expandafter \def \csname #1\endcsname }
      190 [25] \def\@nameuse#1{\csname #1\endcsname}
      191 {\def}
      192 {\def \@nameuse #1->\csname #1\endcsname }
      193 [26] \def\I{I}\def\J{J}
      194 {\def}
      195 {\def \I ->I}
      196 {\def}
      197 {\def \J ->J}
      198 [27] \def\cl@page{}
      199 {\def}
      200 {\def \cl@page ->}
      201 [28] \def\thepage{\@arabic\c@page}
      202 {\def}
      203 {\def \thepage ->\@arabic \c@page }
      204 [29] \def\@roman#1{\romannumeral#1}\def\@Roman#1{\Romannumeral#1}
      205 {\def}
      206 {\def \@roman #1->\romannumeral #1}
      207 {\def}
      208 {\def \@Roman #1->\Romannumeral #1}
      209 [30] \def\mod#1{~\@mod\;#1}
      210 {\def}
      211 {\def \mod #1->~\@mod \;#1}
      212 [31] \def\bmod#1{\;\@mod\;#1}
      213 {\def}
      214 {\def \bmod #1->\;\@mod \;#1}
      215 [32] \def\pmod#1{\pod{\@mod\; #1}}
      216 {\def}
      217 {\def \pmod #1->\pod {\@mod \; #1}}
      218 [33] \def\pod#1{~(#1)}
      219 {\def}
      220 {\def \pod #1->~(#1)}
      221 [34] \newcommand\bottomfraction{.3}
      222 {\newcommand}
      223 {\newcommand \bottomfraction}
      224 [35] \newcount\c@topnumber\newcommand\topfraction{.7}
      225 {\newcount}
      226 {\countdef \c@topnumber=\count12}
      227 {\newcommand}
      228 {\newcommand \topfraction}
      229 [36] \def\date#1{\xmlelt{date}{#1}}
      230 {\def}
      231 {\def \date #1->\xmlelt {date}{#1}}
      232 [37] \newcounter{enumi}\newcounter{enumii}\newcounter{enumiii}\newcounter{enumiv}
      233 {\newcounter}
      234 {\countdef \c@enumi=\count13}
      235 {\newcounter}
      236 {\countdef \c@enumii=\count14}
      237 {\newcounter}
      238 {\countdef \c@enumiii=\count15}
      239 {\newcounter}
      240 {\countdef \c@enumiv=\count16}
      241 [38] \newcommand\labelitemi{\textbullet}
      242 {\newcommand}
      243 {\newcommand \labelitemi}
      244 [39] \newcommand\labelitemii{\normalfont\bfseries \textendash}
      245 {\newcommand}
      246 {\newcommand \labelitemii}
      247 [40] \newcommand\labelitemiii{\textasteriskcentered}
      248 {\newcommand}
      249 {\newcommand \labelitemiii}
      250 [41] \newcommand\labelitemiv{\textperiodcentered}
      251 {\newcommand}
      252 {\newcommand \labelitemiv}
      253 [42] \newcounter {part}\newcounter {chapter}
      254 {\newcounter}
      255 {\countdef \c@part=\count17}
      256 {\newcounter}
      257 {\countdef \c@chapter=\count18}
      258 [43] \newcounter {section}[chapter]\newcounter{subsection}[section]
      259 {\newcounter}
      260 {\countdef \c@section=\count19}
      261 {newcounter_opt}
      262 {newcounter_opt->\cl@chapter}
      263 \cl@chapter ->
      264 {\newcounter}
      265 {\countdef \c@subsection=\count20}
      266 {newcounter_opt}
      267 {newcounter_opt->\cl@section}
      268 \cl@section ->
      269 [44] \newcounter {subsubsection}[subsection]
      270 {\newcounter}
      271 {\countdef \c@subsubsection=\count21}
      272 {newcounter_opt}
      273 {newcounter_opt->\cl@subsection}
      274 \cl@subsection ->
      275 [45] \newcounter {paragraph}[subsubsection]
      276 {\newcounter}
      277 {\countdef \c@paragraph=\count22}
      278 {newcounter_opt}
      279 {newcounter_opt->\cl@subsubsection}
      280 \cl@subsubsection ->
      281 [46] \newcounter {subparagraph}[paragraph]
      282 {\newcounter}
      283 {\countdef \c@subparagraph=\count23}
      284 {newcounter_opt}
      285 {newcounter_opt->\cl@paragraph}
      286 \cl@paragraph ->
      287 [47] \def\refname{Bibliography}\def\footcitesep{, }\def\citepunct{, }
      288 {\def}
      289 {\def \refname ->Bibliography}
      290 {\def}
      291 {\def \footcitesep ->, }
      292 {\def}
      293 {\def \citepunct ->, }
      294 [48] \newcommand\newblock{\hskip .11em plus.33em minus.07em}
      295 {\newcommand}
      296 {\newcommand \newblock}
      297 [49] \def\choose{\atopwithdelims()}
      298 {\def}
      299 {\def \choose ->\atopwithdelims ()}
      300 [50] \def\shoveleft{\multicolumn{1}{l}}
      301 {\def}
      302 {\def \shoveleft ->\multicolumn {1}{l}}
      303 [51] \def\shoveright{\multicolumn{1}{r}}
      304 {\def}
      305 {\def \shoveright ->\multicolumn {1}{r}}
      306 [52] \long\def\@firstoftwo#1#2{#1}\long\def\@secondoftwo#1#2{#2}
      307 {\long}
      308 {\long\def}
      309 {\def \@firstoftwo #1#2->#1}
      310 {\long}
      311 {\long\def}
      312 {\def \@secondoftwo #1#2->#2}
      313 [53] 
      314 {\par}
      315 [54] %% End bootstrap commands for latex
      316 ++ Input stack empty at end of file
      317 Defining \maketitle as \TitlePageCmd 0
      318    main <article  -- 'only title page' 'translate also bibliography'/>
      319 Inserting the command \getproduction
      320 Inserting the command \getid
      321 Defining \daterecieved as \TitlePageCmd 3
      322    usual <date_reception/>
      323 Defining \dateaccepted as \TitlePageCmd 4
      324    usual <date_acceptation/>
      325 Inserting the string <auteur>
      326 Defining \firstname as \TitlePageCmd 6
      327    usual <prenom/>
      328 Defining \middlename as \TitlePageCmd 7
      329    usual <middlename/>
      330 Defining \lastname as \TitlePageCmd 8
      331    usual <nom/>
      332 Defining \address as \TitlePageCmd 9
      333    usual <adresse/> (flags -par)
      334 Defining \email as \TitlePageCmd 10
      335    usual <mel/>
      336 Inserting the string </auteur>
      337 Defining \Ctitle as \TitlePageCmd 12
      338    usual <titre xml:lang='fr'/>
      339 Defining \Dtitle as \TitlePageCmd 13
      340    usual <TeXtitre xml:lang='fr'/>
      341 Defining \Atitle as \TitlePageCmd 14
      342    usual <titre xml:lang='en'/>
      343 Defining \Btitle as \TitlePageCmd 15
      344    usual <TeXtitre xml:lang='en'/>
      345 Defining \currentlanguage as \TitlePageCmd 16
      346    usual <langue/>
      347 Defining \Aabstract as \TitlePageCmd 17
      348    usual <resume xml:lang='en' /> (flags +env)
      349 Defining \Babstract as \TitlePageCmd 18
      350    usual <TEXresume xml:lang='en' /> (flags +env)
      351 Defining \Cabstract as \TitlePageCmd 19
      352    usual <resume xml:lang='fr' /> (flags +env)
      353 Defining \Dabstract as \TitlePageCmd 20
      354    usual <TEXresume xml:lang='fr' /> (flags +env)
      355 Defining \altkeywords as \TitlePageCmd 21
      356    usual <motcle xml:lang='fr'/>
      357 Defining \keywords as \TitlePageCmd 22
      358    usual <motcle xml:lang='en' />
      359 Defining \subjclass as \TitlePageCmd 23
      360    usual <msc/>
      361 [39]   \let\author\relax
      362 {\let}
      363 {\let \author \relax}
      364 [40]   \def\getproduction{\xbox{production}{
      365 {\def}
      366 [41]    \xbox{fichier\_tex}{\jobname}
      367 [42]    \xbox{fichier\_bib}{\jobname}
      368 [43]    \xbox{date\_prod}{\the\year-\the\month-\the\day}}}
      369 {\def \getproduction ->\xbox {production}{
      370 \xbox {fichier\_tex}{\jobname }
      371 \xbox {fichier\_bib}{\jobname }
      372 \xbox {date\_prod}{\the \year -\the \month -\the \day }}}
      373 [44]   \makeatletter
      374 {\makeatletter}
      375 [45]   \def\title{\@reevaluate\Atitle\Btitle}
      376 {\def}
      377 {\def \title ->\@reevaluate \Atitle \Btitle }
      378 [46]   \def\abstract{\@reevaluate*{Aabstract}{Babstract}}
      379 {\def}
      380 {\def \abstract ->\@reevaluate *{Aabstract}{Babstract}}
      381 [47]   \def\altabstract{\@reevaluate*{Cabstract}{Dabstract}}
      382 {\def}
      383 {\def \altabstract ->\@reevaluate *{Cabstract}{Dabstract}}
      384 [48]   \def\Btitle@helper{\@nomathml=-1 }
      385 {\def}
      386 {\def \Btitle@helper ->\@nomathml =-1 }
      387 [49]   \def\Dtitle@helper{\@nomathml=-1 }
      388 {\def}
      389 {\def \Dtitle@helper ->\@nomathml =-1 }
      390 [50]   \def\Babstract@helper{\@nomathml=-1 }
      391 {\def}
      392 {\def \Babstract@helper ->\@nomathml =-1 }
      393 [51]   \def\Dabstract@helper{\@nomathml=-1 }
      394 {\def}
      395 {\def \Dabstract@helper ->\@nomathml =-1 }
      396 [52]   \def\alttitle{\@reevaluate\Ctitle\Dtitle}
      397 {\def}
      398 {\def \alttitle ->\@reevaluate \Ctitle \Dtitle }
      399 [53]    % for the biblio
      400 [54]    \renewcommand\bpers[5][\undefined]{%
      401 {\renewcommand}
      402 {\newcommand \bpers}
      403 [55]    \xbox{nom}{#4}%
      404 [56]  \ifx#1\undefined\xbox{prenom}{#2}\else\xbox{prenom}{#1}\xbox{initiale}{#2}\fi
      405 [57]  \xbox{particule}{#3}%
      406 [58]  \def\tmp{#1\undefined}\ifx\tmp\undefined\else
      407 [59]  \xbox{junior}{#5}\fi}
      408 [60]   \def\lastname@hook#1{\textit{#1}}
      409 {\def}
      410 {\def \lastname@hook #1->\textit {#1}}
      411 [61]   \expandafter\def\csname cititem-btitle\endcsname{%
      412 {\expandafter \def \csname}
      413 {\csname}
      414 {\csname->\cititem-btitle}
      415 {\def}
      416 [62]     \bgroup\catcode `\$=12 \@nomathml=1 \citembtitle}
      417 {\def \cititem-btitle ->\bgroup \catcode `\$=12 \@nomathml =1 \citembtitle }
      418 [63]      \def\citembtitle#1{\xbox{title}{#1}\egroup\@addnl}
      419 {\def}
      420 {\def \citembtitle #1->\xbox {title}{#1}\egroup \@addnl }
      421 [65]   \let\nfrac\dfrac
      422 {\let}
      423 {\let \nfrac \dfrac}
      424 [66]   \let\pfrac\dfrac
      425 {\let}
      426 {\let \pfrac \dfrac}
      427 [67]   \newtheorem{theorem}{Theorem}
      428 {\newtheorem}
      429 {\newtheorem theorem}
      430 [68]   \newtheorem{proof}{Proof}
      431 {\let \endtheorem \@endtheorem}
      432 {\let \thetheorem \relax}
      433 {\def \theorem ->\@begintheorem {Theorem}\thetheorem {}{theorem}{theorem}{plain}}
      434 {\countdef \c@theorem=\count24}
      435 {\def \thetheorem ->\arabic {theorem}}
      436 {\relax}
      437 {\newtheorem}
      438 {\newtheorem proof}
      439 [69]   \newtheorem{lemma}{Lemma}
      440 {\let \endproof \@endtheorem}
      441 {\let \theproof \relax}
      442 {\def \proof ->\@begintheorem {Proof}\theproof {}{proof}{proof}{plain}}
      443 {\countdef \c@proof=\count25}
      444 {\def \theproof ->\arabic {proof}}
      445 {\relax}
      446 {\newtheorem}
      447 {\newtheorem lemma}
      448 [70]   \newtheorem{rema*}{Remark}
      449 {\let \endlemma \@endtheorem}
      450 {\let \thelemma \relax}
      451 {\def \lemma ->\@begintheorem {Lemma}\thelemma {}{lemma}{lemma}{plain}}
      452 {\countdef \c@lemma=\count26}
      453 {\def \thelemma ->\arabic {lemma}}
      454 {\relax}
      455 {\newtheorem}
      456 {\newtheorem rema*}
      457 [71]   \newtheorem{remas*}{Remarks}
      458 {\let \endrema* \@endtheorem}
      459 {\let \therema* \relax}
      460 {\def \rema* ->\@begintheorem {Remark}\therema* {}{rema*}{rema*}{plain}}
      461 {\countdef \c@rema*=\count27}
      462 {\def \therema* ->\arabic {rema*}}
      463 {\relax}
      464 {\newtheorem}
      465 {\newtheorem remas*}
      466 [72]   \newtheorem{defi}{Definition}
      467 {\let \endremas* \@endtheorem}
      468 {\let \theremas* \relax}
      469 {\def \remas* ->\@begintheorem {Remarks}\theremas* {}{remas*}{remas*}{plain}}
      470 {\countdef \c@remas*=\count28}
      471 {\def \theremas* ->\arabic {remas*}}
      472 {\relax}
      473 {\newtheorem}
      474 {\newtheorem defi}
      475 [73]   \let \tn\text
      476 {\let \enddefi \@endtheorem}
      477 {\let \thedefi \relax}
      478 {\def \defi ->\@begintheorem {Definition}\thedefi {}{defi}{defi}{plain}}
      479 {\countdef \c@defi=\count29}
      480 {\def \thedefi ->\arabic {defi}}
      481 {\relax}
      482 {\let}
      483 {\let \tn \text}
      484 [74]   \let\appendix\relax
      485 {\let}
      486 {\let \appendix \relax}
      487 [75]   \let\notag\relax
      488 {\let}
      489 {\let \notag \relax}
      490 [76]   \def\sideset#1#2{}
      491 {\def}
      492 {\def \sideset #1#2->}
      493 [77]   \makeatother
      494 {\makeatother}
      495 [1] \documentclass[AIF]{cedram}
      496 {\documentclass}
      497 {Push argument 1}
      498 Character sequence: AIF.
      499 {Text:AIF}
      500 {Pop 1: document_v argument_v}
      501 {Push argument 1}
      502 Character sequence: cedram.
      503 {Text:cedram}
      504 {Pop 1: document_v argument_v}
      505 [2] 
      506 {\par}
      507 [3] \begin{document}
      508 {\begin}
      509 {\begin document}
      510 +stack: level + 2 for environment
      511 {\document}
      512 +stack: ending environment document; resuming document.
      513 +stack: level - 2 for environment
      514 +stack: level set to 1
      515 ++ Input stack ++ 1 (AtBeginDocument hook)
      516 [1] \let\do\noexpand\ignorespaces
      517 ++ End of virtual file.
      518 ++ Input stack -- 1 (AtBeginDocument hook)
      519 atbegindocumenthook= \let \AtBeginDocument \@notprerr \let \do \noexpand \ignorespaces 
      520 {\let}
      521 {\let \AtBeginDocument \@notprerr}
      522 {\let}
      523 {\let \do \noexpand}
      524 {\ignorespaces}
      525 [4] 
      526 {\par}
      527 [5] \title{Fourier coefficients for simple $L^\infty$ functions}
      528 \title ->\@reevaluate \Atitle \Btitle 
      529 {\@reevaluate}
      530 {Reeval: \Atitle {Fourier coefficients for simple $L^\infty$ functions}%
      531 \Btitle {Fourier coefficients for simple $L^\infty$ functions}%
      532 }
      533 ++ Input stack ++ 1 (reevaluate)
      534 [1] \Atitle {Fourier coefficients for simple $L^\infty$ functions}%
      535 {(Unknown)}
      536 {\titlepage 14}
      537 {\titlepage 14=\Atitle}
      538 {Push titre 1}
      539 +stack: level = 1 for titlepage argument
      540 Character sequence: Fourier coefficients for simple .
      541 {math shift character $}
      542 {Text:Fourier coefficients for simple }
      543 +stack: level + 2 for math
      544 +stack: level - 2 for math
      545 Math: $L^\infty $
      546 Character sequence:  functions.
      547 {end-group character }}
      548 +stack: level - 2 for titlepage argument
      549 {Text: functions}
      550 {Pop 1: document_v titre_t}
      551 [2] \Btitle {Fourier coefficients for simple $L^\infty$ functions}%
      552 {(Unknown)}
      553 {\titlepage 15}
      554 {\titlepage 15=\Btitle}
      555 {Push TeXtitre 1}
      556 +stack: level = 1 for titlepage argument
      557 +stack: level + 2 (spec)
      558 \Btitle@helper ->\@nomathml =-1 
      559 {\@nomathml}
      560 +scanint for \@nomathml->-1
      561 Character sequence: Fourier coefficients for simple .
      562 {math shift character $}
      563 {Text:Fourier coefficients for simple }
      564 +stack: level + 3 for math
      565 +stack: level - 3 for math
      566 Math: $L^\infty $
      567 Character sequence:  functions.
      568 {end-group character }}
      569 +stack: restoring integer value 0 for \@nomathml
      570 +stack: level - 3 for titlepage argument
      571 {Text: functions}
      572 +stack: level - 2 (spec)
      573 {Pop 1: document_v TeXtitre_t}
      574 ++ End of virtual file.
      575 ++ Input stack -- 1 (reevaluate)
      576 [6] \alttitle{Coefficients Fourier pour fonctions  $L^\infty$ simples}
      577 \alttitle ->\@reevaluate \Ctitle \Dtitle 
      578 {\@reevaluate}
      579 {Reeval: \Ctitle {Coefficients Fourier pour fonctions  $L^\infty$ simples}%
      580 \Dtitle {Coefficients Fourier pour fonctions  $L^\infty$ simples}%
      581 }
      582 ++ Input stack ++ 1 (reevaluate)
      583 [1] \Ctitle {Coefficients Fourier pour fonctions  $L^\infty$ simples}%
      584 {(Unknown)}
      585 {\titlepage 12}
      586 {\titlepage 12=\Ctitle}
      587 {Push titre 1}
      588 +stack: level = 1 for titlepage argument
      589 Character sequence: Coefficients Fourier pour fonctions .
      590 {math shift character $}
      591 {Text:Coefficients Fourier pour fonctions }
      592 +stack: level + 2 for math
      593 +stack: level - 2 for math
      594 Math: $L^\infty $
      595 Character sequence:  simples.
      596 {end-group character }}
      597 +stack: level - 2 for titlepage argument
      598 {Text: simples}
      599 {Pop 1: document_v titre_t}
      600 [2] \Dtitle {Coefficients Fourier pour fonctions  $L^\infty$ simples}%
      601 {(Unknown)}
      602 {\titlepage 13}
      603 {\titlepage 13=\Dtitle}
      604 {Push TeXtitre 1}
      605 +stack: level = 1 for titlepage argument
      606 +stack: level + 2 (spec)
      607 \Dtitle@helper ->\@nomathml =-1 
      608 {\@nomathml}
      609 +scanint for \@nomathml->-1
      610 Character sequence: Coefficients Fourier pour fonctions .
      611 {math shift character $}
      612 {Text:Coefficients Fourier pour fonctions }
      613 +stack: level + 3 for math
      614 +stack: level - 3 for math
      615 Math: $L^\infty $
      616 Character sequence:  simples.
      617 {end-group character }}
      618 +stack: restoring integer value 0 for \@nomathml
      619 +stack: level - 3 for titlepage argument
      620 {Text: simples}
      621 +stack: level - 2 (spec)
      622 {Pop 1: document_v TeXtitre_t}
      623 ++ End of virtual file.
      624 ++ Input stack -- 1 (reevaluate)
      625 [7] \author{\firstname{Donald} \middlename{E.} \lastname{Knuth}}
      626 {\relax}
      627 {begin-group character {}
      628 +stack: level + 2 for brace
      629 {(Unknown)}
      630 {\titlepage 6}
      631 {\titlepage 6=\firstname}
      632 {Push prenom 1}
      633 +stack: level = 2 for titlepage argument
      634 Character sequence: Donald.
      635 {end-group character }}
      636 +stack: level - 3 for titlepage argument
      637 {Text:Donald}
      638 {Pop 1: document_v prenom_t}
      639 {(Unknown)}
      640 {\titlepage 7}
      641 {\titlepage 7=\middlename}
      642 {Push middlename 1}
      643 +stack: level = 2 for titlepage argument
      644 Character sequence: E..
      645 {end-group character }}
      646 +stack: level - 3 for titlepage argument
      647 {Text:E.}
      648 {Pop 1: document_v middlename_t}
      649 {(Unknown)}
      650 {\titlepage 8}
      651 {\titlepage 8=\lastname}
      652 {Push nom 1}
      653 +stack: level = 2 for titlepage argument
      654 \lastname@hook #1->\textit {#1}
      655 #1<-Knuth
      656 {\textit}
      657 +stack: level + 3 for brace
      658 {font change \itshape}
      659 Character sequence: Knuth.
      660 {end-group character }}
      661 {Text:Knuth}
      662 {font restore }
      663 +stack: level - 3 for brace
      664 {end-group character }}
      665 +stack: level - 3 for titlepage argument
      666 {Pop 1: document_v nom_t}
      667 {end-group character }}
      668 +stack: level - 2 for brace
      669 [8] 
      670 {\par}
      671 [9] \address{\TeX\ Users Group \\P.O. Box 869\\
      672 {(Unknown)}
      673 {\titlepage 9}
      674 {\titlepage 9=\address}
      675 {Push adresse 1}
      676 +stack: level = 1 for titlepage argument
      677 {\TeX}
      678 {\ }
      679 Character sequence: Users Group .
      680 {\\}
      681 {Text:<TeX/> Users Group }
      682 Character sequence:  P.O. Box 869.
      683 {\\}
      684 {Text: P.O. Box 869}
      685 [10] Santa Barbara, CA 93102-0869 USA}
      686 Character sequence:  Santa Barbara, CA 93102-0869 USA.
      687 {end-group character }}
      688 +stack: level - 2 for titlepage argument
      689 {Text: Santa Barbara, CA 93102-0869 USA}
      690 {Pop 1: document_v adresse_t}
      691 [11] 
      692 {\par}
      693 [12] \email{d.e.knuth@somewhere.on.the.net}
      694 {(Unknown)}
      695 {\titlepage 10}
      696 {\titlepage 10=\email}
      697 {Push mel 1}
      698 +stack: level = 1 for titlepage argument
      699 Character sequence: d.e.knuth@somewhere.on.the.net.
      700 {end-group character }}
      701 +stack: level - 2 for titlepage argument
      702 {Text:d.e.knuth@somewhere.on.the.net}
      703 {Pop 1: document_v mel_t}
      704 [13] 
      705 {\par}
      706 [14] \subjclass{11M26,  11M36, 11S40}
      707 {(Unknown)}
      708 {\titlepage 23}
      709 {\titlepage 23=\subjclass}
      710 {Push msc 1}
      711 +stack: level = 1 for titlepage argument
      712 Character sequence: 11M26, 11M36, 11S40.
      713 {end-group character }}
      714 +stack: level - 2 for titlepage argument
      715 {Text:11M26, 11M36, 11S40}
      716 {Pop 1: document_v msc_t}
      717 [15] 
      718 {\par}
      719 [16] \keywords{simple $L^\infty$ functions, lambda function}
      720 {(Unknown)}
      721 {\titlepage 22}
      722 {\titlepage 22=\keywords}
      723 {Push motcle 1}
      724 +stack: level = 1 for titlepage argument
      725 Character sequence: simple .
      726 {math shift character $}
      727 {Text:simple }
      728 +stack: level + 2 for math
      729 +stack: level - 2 for math
      730 Math: $L^\infty $
      731 Character sequence:  functions, lambda function.
      732 {end-group character }}
      733 +stack: level - 2 for titlepage argument
      734 {Text: functions, lambda function}
      735 {Pop 1: document_v motcle_t}
      736 [17] 
      737 {\par}
      738 [18] \altkeywords{fonctions  $L^\infty$ simples, fonction lambda}
      739 {(Unknown)}
      740 {\titlepage 21}
      741 {\titlepage 21=\altkeywords}
      742 {Push motcle 1}
      743 +stack: level = 1 for titlepage argument
      744 Character sequence: fonctions .
      745 {math shift character $}
      746 {Text:fonctions }
      747 +stack: level + 2 for math
      748 +stack: level - 2 for math
      749 Math: $L^\infty $
      750 Character sequence:  simples, fonction lambda.
      751 {end-group character }}
      752 +stack: level - 2 for titlepage argument
      753 {Text: simples, fonction lambda}
      754 {Pop 1: document_v motcle_t}
      755 [19] 
      756 {\par}
      757 [20] \daterecieved{2004-06-14}%{14 juin 2004}
      758 {(Unknown)}
      759 {\titlepage 3}
      760 {\titlepage 3=\daterecieved}
      761 {Push date_reception 1}
      762 +stack: level = 1 for titlepage argument
      763 Character sequence: 2004-06-14.
      764 {end-group character }}
      765 +stack: level - 2 for titlepage argument
      766 {Text:2004-06-14}
      767 {Pop 1: document_v date_reception_t}
      768 [21] \dateaccepted{2004-12-09}%{9 d�cembre 2004}
      769 {(Unknown)}
      770 {\titlepage 4}
      771 {\titlepage 4=\dateaccepted}
      772 {Push date_acceptation 1}
      773 +stack: level = 1 for titlepage argument
      774 Character sequence: 2004-12-09.
      775 {end-group character }}
      776 +stack: level - 2 for titlepage argument
      777 {Text:2004-12-09}
      778 {Pop 1: document_v date_acceptation_t}
      779 [22] 
      780 {\par}
      781 [23] \begin{abstract}
      782 {\begin}
      783 {\begin abstract}
      784 +stack: level + 2 for environment
      785 \abstract ->\@reevaluate *{Aabstract}{Babstract}
      786 {\@reevaluate}
      787 [24]   This is an abstract with a beautiful inline formula % Comment!
      788 [25]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      789 [26]    O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      790 [27] \end{abstract}
      791 +stack: ending environment abstract; resuming document.
      792 +stack: level - 2 for environment
      793 {Reeval: \begin{Aabstract}This is an abstract with a beautiful inline formula % Comment!
      794   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      795    O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      796 \end{Aabstract}%
      797 \begin{Babstract}This is an abstract with a beautiful inline formula % Comment!
      798   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      799    O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      800 \end{Babstract}%
      801 }
      802 ++ Input stack ++ 1 (reevaluate)
      803 [1] \begin{Aabstract}This is an abstract with a beautiful inline formula % Comment!
      804 {\begin}
      805 {\begin Aabstract}
      806 +stack: level + 2 for environment
      807 {(Unknown)}
      808 {\titlepage 17}
      809 {\titlepage 17=\Aabstract}
      810 {Push resume 1}
      811 +stack: level = 2 for titlepage argument
      812 Character sequence: This is an abstract with a beautiful inline formula .
      813 [2]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      814 {math shift character $}
      815 {Text:This is an abstract with a beautiful inline formula }
      816 +stack: level + 3 for math
      817 +stack: level + 4 for math
      818 +stack: level - 4 for math
      819 +stack: level + 4 for math
      820 +stack: level - 4 for math
      821 [3]    O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      822 +stack: level + 4 for math
      823 +stack: level - 4 for math
      824 +stack: level + 4 for math
      825 +stack: level - 4 for math
      826 +stack: level - 3 for math
      827 Math: $\lambda _n(\pi ) = \frac {N}{2} n \log n + C_1(\pi ) n +
      828 O(\sqrt {n}\log {n})$
      829 Realloc xml math table to 20
      830 Realloc xml math table to 40
      831 Character sequence: , where .
      832 {math shift character $}
      833 {Text:, where }
      834 +stack: level + 3 for math
      835 +stack: level - 3 for math
      836 Math: $C_1(\pi )$
      837 Character sequence:  is a real-valued constant. .
      838 [4] \end{Aabstract}%
      839 {\end}
      840 {Text: is a real-valued constant.
      841 }
      842 {\end Aabstract}
      843 +stack: level - 2 for titlepage argument
      844 +stack: ending environment Aabstract; resuming document.
      845 +stack: level - 1 for environment
      846 {Pop 1: document_v resume_t}
      847 [5] \begin{Babstract}This is an abstract with a beautiful inline formula % Comment!
      848 {\begin}
      849 {\begin Babstract}
      850 +stack: level + 2 for environment
      851 {(Unknown)}
      852 {\titlepage 18}
      853 {\titlepage 18=\Babstract}
      854 {Push TEXresume 1}
      855 +stack: level = 2 for titlepage argument
      856 \Babstract@helper ->\@nomathml =-1 
      857 {\@nomathml}
      858 +scanint for \@nomathml->-1
      859 Character sequence: This is an abstract with a beautiful inline formula .
      860 [6]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      861 {math shift character $}
      862 {Text:This is an abstract with a beautiful inline formula }
      863 +stack: level + 3 for math
      864 +stack: level + 4 for math
      865 +stack: level - 4 for math
      866 +stack: level + 4 for math
      867 +stack: level - 4 for math
      868 [7]    O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      869 +stack: level + 4 for math
      870 +stack: level - 4 for math
      871 +stack: level + 4 for math
      872 +stack: level - 4 for math
      873 +stack: level - 3 for math
      874 Math: $\lambda _n(\pi ) = \frac {N}{2} n \log n + C_1(\pi ) n +
      875 O(\sqrt {n}\log {n})$
      876 Character sequence: , where .
      877 {math shift character $}
      878 {Text:, where }
      879 +stack: level + 3 for math
      880 +stack: level - 3 for math
      881 Math: $C_1(\pi )$
      882 Character sequence:  is a real-valued constant. .
      883 [8] \end{Babstract}%
      884 {\end}
      885 {Text: is a real-valued constant.
      886 }
      887 {\end Babstract}
      888 +stack: restoring integer value 0 for \@nomathml
      889 +stack: level - 2 for titlepage argument
      890 +stack: ending environment Babstract; resuming document.
      891 +stack: level - 1 for environment
      892 {Pop 1: document_v TEXresume_t}
      893 ++ End of virtual file.
      894 ++ Input stack -- 1 (reevaluate)
      895 [28] 
      896 {\par}
      897 [29] 
      898 {\par}
      899 [30] \begin{altabstract}
      900 {\begin}
      901 {\begin altabstract}
      902 +stack: level + 2 for environment
      903 \altabstract ->\@reevaluate *{Cabstract}{Dabstract}
      904 {\@reevaluate}
      905 [31]   Mon r�sum� avec ma formule
      906 [32]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      907 [33]    O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      908 [34] \end{altabstract}
      909 +stack: ending environment altabstract; resuming document.
      910 +stack: level - 2 for environment
      911 {Reeval: \begin{Cabstract}Mon r�sum� avec ma formule
      912   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      913    O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      914 \end{Cabstract}%
      915 \begin{Dabstract}Mon r�sum� avec ma formule
      916   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      917    O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      918 \end{Dabstract}%
      919 }
      920 ++ Input stack ++ 1 (reevaluate)
      921 [1] \begin{Cabstract}Mon r�sum� avec ma formule
      922 {\begin}
      923 {\begin Cabstract}
      924 +stack: level + 2 for environment
      925 {(Unknown)}
      926 {\titlepage 19}
      927 {\titlepage 19=\Cabstract}
      928 {Push resume 1}
      929 +stack: level = 2 for titlepage argument
      930 Character sequence: Mon r�sum� avec ma formule .
      931 [2]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      932 {math shift character $}
      933 {Text:Mon r�sum� avec ma formule
      934 }
      935 +stack: level + 3 for math
      936 +stack: level + 4 for math
      937 +stack: level - 4 for math
      938 +stack: level + 4 for math
      939 +stack: level - 4 for math
      940 [3]    O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      941 +stack: level + 4 for math
      942 +stack: level - 4 for math
      943 +stack: level + 4 for math
      944 +stack: level - 4 for math
      945 +stack: level - 3 for math
      946 Math: $\lambda _n(\pi ) = \frac {N}{2} n \log n + C_1(\pi ) n +
      947 O(\sqrt {n}\log {n})$
      948 Character sequence: , o� .
      949 {math shift character $}
      950 {Text:, o� }
      951 +stack: level + 3 for math
      952 +stack: level - 3 for math
      953 Math: $C_1(\pi )$
      954 Character sequence:  est une constante r�elle. .
      955 [4] \end{Cabstract}%
      956 {\end}
      957 {Text: est une constante r�elle.
      958 }
      959 {\end Cabstract}
      960 +stack: level - 2 for titlepage argument
      961 +stack: ending environment Cabstract; resuming document.
      962 +stack: level - 1 for environment
      963 {Pop 1: document_v resume_t}
      964 [5] \begin{Dabstract}Mon r�sum� avec ma formule
      965 {\begin}
      966 {\begin Dabstract}
      967 +stack: level + 2 for environment
      968 {(Unknown)}
      969 {\titlepage 20}
      970 {\titlepage 20=\Dabstract}
      971 {Push TEXresume 1}
      972 +stack: level = 2 for titlepage argument
      973 \Dabstract@helper ->\@nomathml =-1 
      974 {\@nomathml}
      975 +scanint for \@nomathml->-1
      976 Character sequence: Mon r�sum� avec ma formule .
      977 [6]   $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n +
      978 {math shift character $}
      979 {Text:Mon r�sum� avec ma formule
      980 }
      981 +stack: level + 3 for math
      982 +stack: level + 4 for math
      983 +stack: level - 4 for math
      984 +stack: level + 4 for math
      985 +stack: level - 4 for math
      986 [7]    O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      987 +stack: level + 4 for math
      988 +stack: level - 4 for math
      989 +stack: level + 4 for math
      990 +stack: level - 4 for math
      991 +stack: level - 3 for math
      992 Math: $\lambda _n(\pi ) = \frac {N}{2} n \log n + C_1(\pi ) n +
      993 O(\sqrt {n}\log {n})$
      994 Character sequence: , o� .
      995 {math shift character $}
      996 {Text:, o� }
      997 +stack: level + 3 for math
      998 +stack: level - 3 for math
      999 Math: $C_1(\pi )$
      1000 Character sequence:  est une constante r�elle. .
      1001 [8] \end{Dabstract}%
      1002 {\end}
      1003 {Text: est une constante r�elle.
      1004 }
      1005 {\end Dabstract}
      1006 +stack: restoring integer value 0 for \@nomathml
      1007 +stack: level - 2 for titlepage argument
      1008 +stack: ending environment Dabstract; resuming document.
      1009 +stack: level - 1 for environment
      1010 {Pop 1: document_v TEXresume_t}
      1011 ++ End of virtual file.
      1012 ++ Input stack -- 1 (reevaluate)
      1013 [35] 
      1014 {\par}
      1015 [36] \maketitle
      1016 {(Unknown)}
      1017 {\titlepage 0}
      1018 {\titlepage 0=\maketitle}
      1019 {Push getproduction 1}
      1020 {getproduction->\getproduction}
      1021 \getproduction ->\xbox {production}{
      1022 \xbox {fichier\_tex}{\jobname }
      1023 \xbox {fichier\_bib}{\jobname }
      1024 \xbox {date\_prod}{\the \year -\the \month -\the \day }}
      1025 {\xbox}
      1026 {Push argument 2}
      1027 Character sequence: production.
      1028 {Text:production}
      1029 {Pop 2: document_v getproduction_t argument_t}
      1030 {Constructing a box named production}
      1031 +stack: level + 2 for brace
      1032 {Push hbox 2}
      1033 Character sequence:  .
      1034 {\xbox}
      1035 {Text:
      1036 }
      1037 {Push argument 3}
      1038 Character sequence: fichier.
      1039 {\_}
      1040 Character sequence: tex.
      1041 {Text:fichier_tex}
      1042 {Pop 3: document_v getproduction_t hbox_t argument_t}
      1043 {Constructing a box named fichier_tex}
      1044 +stack: level + 3 for brace
      1045 {Push hbox 3}
      1046 Character sequence: bo.
      1047 {end-group character }}
      1048 +stack: finish a box of type 513
      1049 {Text:bo}
      1050 {Pop 3: document_v getproduction_t hbox_t hbox_t}
      1051 +stack: level - 3 for brace
      1052 Character sequence:  .
      1053 {\xbox}
      1054 {Text:
      1055 }
      1056 {Push argument 3}
      1057 Character sequence: fichier.
      1058 {\_}
      1059 Character sequence: bib.
      1060 {Text:fichier_bib}
      1061 {Pop 3: document_v getproduction_t hbox_t argument_t}
      1062 {Constructing a box named fichier_bib}
      1063 +stack: level + 3 for brace
      1064 {Push hbox 3}
      1065 Character sequence: bo.
      1066 {end-group character }}
      1067 +stack: finish a box of type 513
      1068 {Text:bo}
      1069 {Pop 3: document_v getproduction_t hbox_t hbox_t}
      1070 +stack: level - 3 for brace
      1071 Character sequence:  .
      1072 {\xbox}
      1073 {Text:
      1074 }
      1075 {Push argument 3}
      1076 Character sequence: date.
      1077 {\_}
      1078 Character sequence: prod.
      1079 {Text:date_prod}
      1080 {Pop 3: document_v getproduction_t hbox_t argument_t}
      1081 {Constructing a box named date_prod}
      1082 +stack: level + 3 for brace
      1083 {Push hbox 3}
      1084 {\the}
      1085 {\the \year}
      1086 \the->2006.
      1087 Character sequence: 2006-.
      1088 {\the}
      1089 {\the \month}
      1090 \the->8.
      1091 Character sequence: 8-.
      1092 {\the}
      1093 {\the \day}
      1094 \the->1.
      1095 Character sequence: 1.
      1096 {end-group character }}
      1097 +stack: finish a box of type 513
      1098 {Text:2006-8-1}
      1099 {Pop 3: document_v getproduction_t hbox_t hbox_t}
      1100 +stack: level - 3 for brace
      1101 {end-group character }}
      1102 +stack: finish a box of type 513
      1103 {Pop 2: document_v getproduction_t hbox_t}
      1104 +stack: level - 2 for brace
      1105 {Pop 1: document_v getproduction_t}
      1106 {Push getid 1}
      1107 {getid->\getid}
      1108 {\relax}
      1109 {Pop 1: document_v getid_t}
      1110 {Push p 1}
      1111 Translation terminated after title page
      1112 {\endallinput}
      1113 ++ Input stack empty at end of file
      1114 Bib stats: seen 0 entries
      1115 Seen 3 bibliographic entries
      1116 {Push argument 2}
      1117 {Push biblio 3}
      1118 [1] % reading source bo.bib
      1119 [2] %
      1120 [3] \citation{Bar03}{cite:Ba03}{bid0}{year}{article}
      1121 {\citation}
      1122 {Push argument 4}
      1123 Character sequence: Bar03.
      1124 {Text:Bar03}
      1125 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1126 {Push argument 4}
      1127 Character sequence: cite:Ba03.
      1128 {Text:cite:Ba03}
      1129 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1130 {Push argument 4}
      1131 Character sequence: bid0.
      1132 {Text:bid0}
      1133 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1134 {Push argument 4}
      1135 Character sequence: year.
      1136 {Text:year}
      1137 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1138 {Push argument 4}
      1139 Character sequence: article.
      1140 {Text:article}
      1141 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1142 {Push citation 4}
      1143 [4] \bauthors{\bpers[E. W.]{E. W.}{}{Barnes}{}}
      1144 {\bauthors}
      1145 {Push bauteurs 5}
      1146 \bpers \undefined #2#3#4#5->\xbox {nom}{#4}\ifx #1\undefined \xbox {prenom}{#2}\else \xbox {prenom}{#1}\xbox {initiale}{#2}\fi \xbox {particule}{#3}\def \tmp {#1\undefined }\ifx \tmp \undefined \else \xbox {junior}{#5}\fi 
      1147 #1<-E. W.
      1148 #2<-E. W.
      1149 #3<-
      1150 #4<-Barnes
      1151 #5<-
      1152 {\xbox}
      1153 {Push argument 6}
      1154 Character sequence: nom.
      1155 {Text:nom}
      1156 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1157 {Constructing a box named nom}
      1158 +stack: level + 2 for brace
      1159 {Push hbox 6}
      1160 Character sequence: Barnes.
      1161 {end-group character }}
      1162 +stack: finish a box of type 513
      1163 {Text:Barnes}
      1164 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1165 +stack: level - 2 for brace
      1166 +\ifx1
      1167 +iftest1 false
      1168 +\else1
      1169 {\xbox}
      1170 {Push argument 6}
      1171 Character sequence: prenom.
      1172 {Text:prenom}
      1173 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1174 {Constructing a box named prenom}
      1175 +stack: level + 2 for brace
      1176 {Push hbox 6}
      1177 Character sequence: E. W..
      1178 {end-group character }}
      1179 +stack: finish a box of type 513
      1180 {Text:E. W.}
      1181 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1182 +stack: level - 2 for brace
      1183 {\xbox}
      1184 {Push argument 6}
      1185 Character sequence: initiale.
      1186 {Text:initiale}
      1187 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1188 {Constructing a box named initiale}
      1189 +stack: level + 2 for brace
      1190 {Push hbox 6}
      1191 Character sequence: E. W..
      1192 {end-group character }}
      1193 +stack: finish a box of type 513
      1194 {Text:E. W.}
      1195 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1196 +stack: level - 2 for brace
      1197 +\fi1
      1198 {\xbox}
      1199 {Push argument 6}
      1200 Character sequence: particule.
      1201 {Text:particule}
      1202 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1203 {Constructing a box named particule}
      1204 +stack: level + 2 for brace
      1205 {Push hbox 6}
      1206 {end-group character }}
      1207 +stack: finish a box of type 513
      1208 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1209 +stack: level - 2 for brace
      1210 {\def}
      1211 {\def \tmp ->E. W.\undefined }
      1212 +\ifx2
      1213 +iftest2 false
      1214 +\else2
      1215 {\xbox}
      1216 {Push argument 6}
      1217 Character sequence: junior.
      1218 {Text:junior}
      1219 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1220 {Constructing a box named junior}
      1221 +stack: level + 2 for brace
      1222 {Push hbox 6}
      1223 {end-group character }}
      1224 +stack: finish a box of type 513
      1225 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1226 +stack: level - 2 for brace
      1227 +\fi2
      1228 {Pop 5: document_v p_v argument_h biblio_t citation_t bauteurs_b}
      1229 Character sequence:  .
      1230 [5] \cititem{btitle}{On the expression of {E}uler's constant as a definite integral}
      1231 {\cititem}
      1232 \cititem-btitle ->\bgroup \catcode `\$=12 \@nomathml =1 \citembtitle 
      1233 {begin-group character {}
      1234 +stack: level + 2 for brace
      1235 {\catcode}
      1236 +scanint for \catcode->36
      1237 +scanint for \catcode->12
      1238 {\@nomathml}
      1239 +scanint for \@nomathml->1
      1240 \citembtitle #1->\xbox {title}{#1}\egroup \@addnl 
      1241 #1<-On the expression of {E}uler's constant as a definite integral
      1242 {\xbox}
      1243 {Push argument 5}
      1244 Character sequence: title.
      1245 {Text:title}
      1246 {Pop 5: document_v p_v argument_h biblio_t citation_t argument_b}
      1247 {Constructing a box named title}
      1248 +stack: level + 3 for brace
      1249 {Push hbox 5}
      1250 Character sequence: On the expression of .
      1251 {begin-group character {}
      1252 +stack: level + 4 for brace
      1253 Character sequence: E.
      1254 {end-group character }}
      1255 +stack: level - 4 for brace
      1256 Character sequence: uler's constant as a definite integral.
      1257 {end-group character }}
      1258 +stack: finish a box of type 513
      1259 {Text:On the expression of Euler's constant as a definite integral}
      1260 {Pop 5: document_v p_v argument_h biblio_t citation_t hbox_b}
      1261 +stack: level - 3 for brace
      1262 {end-group character }}
      1263 +stack: restoring integer value 0 for \@nomathml
      1264 +stack: restoring integer value 3 for \catcode36
      1265 +stack: level - 2 for brace
      1266 {\@addnl}
      1267 Character sequence:  .
      1268 [6] \cititem{bjournal}{Messenger of Math.}
      1269 {\cititem}
      1270 {Push bjournal 5}
      1271 Character sequence: Messenger of Math..
      1272 {Text:Messenger of Math.}
      1273 {Pop 5: document_v p_v argument_h biblio_t citation_t bjournal_t}
      1274 Character sequence:  .
      1275 [7] \cititem{bvolume}{33}
      1276 {\cititem}
      1277 {Push bvolume 5}
      1278 Character sequence: 33.
      1279 {Text:33}
      1280 {Pop 5: document_v p_v argument_h biblio_t citation_t bvolume_t}
      1281 Character sequence:  .
      1282 [8] \cititem{byear}{1903}
      1283 {\cititem}
      1284 {Push byear 5}
      1285 Character sequence: 1903.
      1286 {Text:1903}
      1287 {Pop 5: document_v p_v argument_h biblio_t citation_t byear_t}
      1288 Character sequence:  .
      1289 [9] \cititem{bpages}{59--61}
      1290 {\cititem}
      1291 {Push bpages 5}
      1292 Character sequence: 59-61.
      1293 {Text:59&ndash;61}
      1294 {Pop 5: document_v p_v argument_h biblio_t citation_t bpages_t}
      1295 Character sequence:  .
      1296 [10] \endcitation
      1297 {\endcitation}
      1298 {Pop 4: document_v p_v argument_h biblio_t citation_t}
      1299 [11] %
      1300 [12] \citation{Bom00}{cite:Bo99}{bid2}{year}{article}
      1301 {\citation}
      1302 {Push argument 4}
      1303 Character sequence: Bom00.
      1304 {Text:Bom00}
      1305 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1306 {Push argument 4}
      1307 Character sequence: cite:Bo99.
      1308 {Text:cite:Bo99}
      1309 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1310 {Push argument 4}
      1311 Character sequence: bid2.
      1312 {Text:bid2}
      1313 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1314 {Push argument 4}
      1315 Character sequence: year.
      1316 {Text:year}
      1317 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1318 {Push argument 4}
      1319 Character sequence: article.
      1320 {Text:article}
      1321 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1322 {Push citation 4}
      1323 [13] \bauthors{\bpers[E.]{E.}{}{Bombieri}{}}
      1324 {\bauthors}
      1325 {Push bauteurs 5}
      1326 \bpers \undefined #2#3#4#5->\xbox {nom}{#4}\ifx #1\undefined \xbox {prenom}{#2}\else \xbox {prenom}{#1}\xbox {initiale}{#2}\fi \xbox {particule}{#3}\def \tmp {#1\undefined }\ifx \tmp \undefined \else \xbox {junior}{#5}\fi 
      1327 #1<-E.
      1328 #2<-E.
      1329 #3<-
      1330 #4<-Bombieri
      1331 #5<-
      1332 {\xbox}
      1333 {Push argument 6}
      1334 Character sequence: nom.
      1335 {Text:nom}
      1336 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1337 {Constructing a box named nom}
      1338 +stack: level + 2 for brace
      1339 {Push hbox 6}
      1340 Character sequence: Bombieri.
      1341 {end-group character }}
      1342 +stack: finish a box of type 513
      1343 {Text:Bombieri}
      1344 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1345 +stack: level - 2 for brace
      1346 +\ifx3
      1347 +iftest3 false
      1348 +\else3
      1349 {\xbox}
      1350 {Push argument 6}
      1351 Character sequence: prenom.
      1352 {Text:prenom}
      1353 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1354 {Constructing a box named prenom}
      1355 +stack: level + 2 for brace
      1356 {Push hbox 6}
      1357 Character sequence: E..
      1358 {end-group character }}
      1359 +stack: finish a box of type 513
      1360 {Text:E.}
      1361 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1362 +stack: level - 2 for brace
      1363 {\xbox}
      1364 {Push argument 6}
      1365 Character sequence: initiale.
      1366 {Text:initiale}
      1367 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1368 {Constructing a box named initiale}
      1369 +stack: level + 2 for brace
      1370 {Push hbox 6}
      1371 Character sequence: E..
      1372 {end-group character }}
      1373 +stack: finish a box of type 513
      1374 {Text:E.}
      1375 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1376 +stack: level - 2 for brace
      1377 +\fi3
      1378 {\xbox}
      1379 {Push argument 6}
      1380 Character sequence: particule.
      1381 {Text:particule}
      1382 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1383 {Constructing a box named particule}
      1384 +stack: level + 2 for brace
      1385 {Push hbox 6}
      1386 {end-group character }}
      1387 +stack: finish a box of type 513
      1388 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1389 +stack: level - 2 for brace
      1390 {\def}
      1391 {\def \tmp ->E.\undefined }
      1392 +\ifx4
      1393 +iftest4 false
      1394 +\else4
      1395 {\xbox}
      1396 {Push argument 6}
      1397 Character sequence: junior.
      1398 {Text:junior}
      1399 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1400 {Constructing a box named junior}
      1401 +stack: level + 2 for brace
      1402 {Push hbox 6}
      1403 {end-group character }}
      1404 +stack: finish a box of type 513
      1405 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1406 +stack: level - 2 for brace
      1407 +\fi4
      1408 {Pop 5: document_v p_v argument_h biblio_t citation_t bauteurs_b}
      1409 Character sequence:  .
      1410 [14] \cititem{btitle}{Remarks on {W}eil's quadratic functional in the theory of prime numbers~{I}}
      1411 {\cititem}
      1412 \cititem-btitle ->\bgroup \catcode `\$=12 \@nomathml =1 \citembtitle 
      1413 {begin-group character {}
      1414 +stack: level + 2 for brace
      1415 {\catcode}
      1416 +scanint for \catcode->36
      1417 +scanint for \catcode->12
      1418 {\@nomathml}
      1419 +scanint for \@nomathml->1
      1420 \citembtitle #1->\xbox {title}{#1}\egroup \@addnl 
      1421 #1<-Remarks on {W}eil's quadratic functional in the theory of prime numbers~{I}
      1422 {\xbox}
      1423 {Push argument 5}
      1424 Character sequence: title.
      1425 {Text:title}
      1426 {Pop 5: document_v p_v argument_h biblio_t citation_t argument_b}
      1427 {Constructing a box named title}
      1428 +stack: level + 3 for brace
      1429 {Push hbox 5}
      1430 Character sequence: Remarks on .
      1431 {begin-group character {}
      1432 +stack: level + 4 for brace
      1433 Character sequence: W.
      1434 {end-group character }}
      1435 +stack: level - 4 for brace
      1436 Character sequence: eil's quadratic functional in the theory of prime numbers.
      1437 ~ ->\nobreakspace 
      1438 {\nobreakspace}
      1439 {begin-group character {}
      1440 +stack: level + 4 for brace
      1441 Character sequence: I.
      1442 {end-group character }}
      1443 +stack: level - 4 for brace
      1444 {end-group character }}
      1445 +stack: finish a box of type 513
      1446 {Text:Remarks on Weil's quadratic functional in the theory of prime numbers&nbsp;I}
      1447 {Pop 5: document_v p_v argument_h biblio_t citation_t hbox_b}
      1448 +stack: level - 3 for brace
      1449 {end-group character }}
      1450 +stack: restoring integer value 0 for \@nomathml
      1451 +stack: restoring integer value 3 for \catcode36
      1452 +stack: level - 2 for brace
      1453 {\@addnl}
      1454 Character sequence:  .
      1455 [15] \cititem{bjournal}{Rend. Mat. Acc. Lincei, Ser.~IX}
      1456 {\cititem}
      1457 {Push bjournal 5}
      1458 Character sequence: Rend. Mat. Acc. Lincei, Ser..
      1459 ~ ->\nobreakspace 
      1460 {\nobreakspace}
      1461 Character sequence: IX.
      1462 {Text:Rend. Mat. Acc. Lincei, Ser.&nbsp;IX}
      1463 {Pop 5: document_v p_v argument_h biblio_t citation_t bjournal_t}
      1464 Character sequence:  .
      1465 [16] \cititem{bvolume}{11}
      1466 {\cititem}
      1467 {Push bvolume 5}
      1468 Character sequence: 11.
      1469 {Text:11}
      1470 {Pop 5: document_v p_v argument_h biblio_t citation_t bvolume_t}
      1471 Character sequence:  .
      1472 [17] \cititem{byear}{2000}
      1473 {\cititem}
      1474 {Push byear 5}
      1475 Character sequence: 2000.
      1476 {Text:2000}
      1477 {Pop 5: document_v p_v argument_h biblio_t citation_t byear_t}
      1478 Character sequence:  .
      1479 [18] \cititem{bpages}{183--233}
      1480 {\cititem}
      1481 {Push bpages 5}
      1482 Character sequence: 183-233.
      1483 {Text:183&ndash;233}
      1484 {Pop 5: document_v p_v argument_h biblio_t citation_t bpages_t}
      1485 Character sequence:  .
      1486 [19] \endcitation
      1487 {\endcitation}
      1488 {Pop 4: document_v p_v argument_h biblio_t citation_t}
      1489 [20] %
      1490 [21] \citation{BPY01}{cite:BPY01}{bid1}{year}{article}
      1491 {\citation}
      1492 {Push argument 4}
      1493 Character sequence: BPY01.
      1494 {Text:BPY01}
      1495 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1496 {Push argument 4}
      1497 Character sequence: cite:BPY01.
      1498 {Text:cite:BPY01}
      1499 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1500 {Push argument 4}
      1501 Character sequence: bid1.
      1502 {Text:bid1}
      1503 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1504 {Push argument 4}
      1505 Character sequence: year.
      1506 {Text:year}
      1507 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1508 {Push argument 4}
      1509 Character sequence: article.
      1510 {Text:article}
      1511 {Pop 4: document_v p_v argument_h biblio_t argument_t}
      1512 {Push citation 4}
      1513 [22] \bauthors{\bpers[P.]{P.}{}{Biane}{}\bpers[J.]{J.}{}{Pitman}{}\bpers[M.]{M.}{}{Yor}{}}
      1514 {\bauthors}
      1515 {Push bauteurs 5}
      1516 \bpers \undefined #2#3#4#5->\xbox {nom}{#4}\ifx #1\undefined \xbox {prenom}{#2}\else \xbox {prenom}{#1}\xbox {initiale}{#2}\fi \xbox {particule}{#3}\def \tmp {#1\undefined }\ifx \tmp \undefined \else \xbox {junior}{#5}\fi 
      1517 #1<-P.
      1518 #2<-P.
      1519 #3<-
      1520 #4<-Biane
      1521 #5<-
      1522 {\xbox}
      1523 {Push argument 6}
      1524 Character sequence: nom.
      1525 {Text:nom}
      1526 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1527 {Constructing a box named nom}
      1528 +stack: level + 2 for brace
      1529 {Push hbox 6}
      1530 Character sequence: Biane.
      1531 {end-group character }}
      1532 +stack: finish a box of type 513
      1533 {Text:Biane}
      1534 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1535 +stack: level - 2 for brace
      1536 +\ifx5
      1537 +iftest5 false
      1538 +\else5
      1539 {\xbox}
      1540 {Push argument 6}
      1541 Character sequence: prenom.
      1542 {Text:prenom}
      1543 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1544 {Constructing a box named prenom}
      1545 +stack: level + 2 for brace
      1546 {Push hbox 6}
      1547 Character sequence: P..
      1548 {end-group character }}
      1549 +stack: finish a box of type 513
      1550 {Text:P.}
      1551 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1552 +stack: level - 2 for brace
      1553 {\xbox}
      1554 {Push argument 6}
      1555 Character sequence: initiale.
      1556 {Text:initiale}
      1557 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1558 {Constructing a box named initiale}
      1559 +stack: level + 2 for brace
      1560 {Push hbox 6}
      1561 Character sequence: P..
      1562 {end-group character }}
      1563 +stack: finish a box of type 513
      1564 {Text:P.}
      1565 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1566 +stack: level - 2 for brace
      1567 +\fi5
      1568 {\xbox}
      1569 {Push argument 6}
      1570 Character sequence: particule.
      1571 {Text:particule}
      1572 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1573 {Constructing a box named particule}
      1574 +stack: level + 2 for brace
      1575 {Push hbox 6}
      1576 {end-group character }}
      1577 +stack: finish a box of type 513
      1578 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1579 +stack: level - 2 for brace
      1580 {\def}
      1581 {\def \tmp ->P.\undefined }
      1582 +\ifx6
      1583 +iftest6 false
      1584 +\else6
      1585 {\xbox}
      1586 {Push argument 6}
      1587 Character sequence: junior.
      1588 {Text:junior}
      1589 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1590 {Constructing a box named junior}
      1591 +stack: level + 2 for brace
      1592 {Push hbox 6}
      1593 {end-group character }}
      1594 +stack: finish a box of type 513
      1595 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1596 +stack: level - 2 for brace
      1597 +\fi6
      1598 \bpers \undefined #2#3#4#5->\xbox {nom}{#4}\ifx #1\undefined \xbox {prenom}{#2}\else \xbox {prenom}{#1}\xbox {initiale}{#2}\fi \xbox {particule}{#3}\def \tmp {#1\undefined }\ifx \tmp \undefined \else \xbox {junior}{#5}\fi 
      1599 #1<-J.
      1600 #2<-J.
      1601 #3<-
      1602 #4<-Pitman
      1603 #5<-
      1604 {\xbox}
      1605 {Push argument 6}
      1606 Character sequence: nom.
      1607 {Text:nom}
      1608 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1609 {Constructing a box named nom}
      1610 +stack: level + 2 for brace
      1611 {Push hbox 6}
      1612 Character sequence: Pitman.
      1613 {end-group character }}
      1614 +stack: finish a box of type 513
      1615 {Text:Pitman}
      1616 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1617 +stack: level - 2 for brace
      1618 +\ifx7
      1619 +iftest7 false
      1620 +\else7
      1621 {\xbox}
      1622 {Push argument 6}
      1623 Character sequence: prenom.
      1624 {Text:prenom}
      1625 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1626 {Constructing a box named prenom}
      1627 +stack: level + 2 for brace
      1628 {Push hbox 6}
      1629 Character sequence: J..
      1630 {end-group character }}
      1631 +stack: finish a box of type 513
      1632 {Text:J.}
      1633 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1634 +stack: level - 2 for brace
      1635 {\xbox}
      1636 {Push argument 6}
      1637 Character sequence: initiale.
      1638 {Text:initiale}
      1639 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1640 {Constructing a box named initiale}
      1641 +stack: level + 2 for brace
      1642 {Push hbox 6}
      1643 Character sequence: J..
      1644 {end-group character }}
      1645 +stack: finish a box of type 513
      1646 {Text:J.}
      1647 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1648 +stack: level - 2 for brace
      1649 +\fi7
      1650 {\xbox}
      1651 {Push argument 6}
      1652 Character sequence: particule.
      1653 {Text:particule}
      1654 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1655 {Constructing a box named particule}
      1656 +stack: level + 2 for brace
      1657 {Push hbox 6}
      1658 {end-group character }}
      1659 +stack: finish a box of type 513
      1660 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1661 +stack: level - 2 for brace
      1662 {\def}
      1663 {\def \tmp ->J.\undefined }
      1664 +\ifx8
      1665 +iftest8 false
      1666 +\else8
      1667 {\xbox}
      1668 {Push argument 6}
      1669 Character sequence: junior.
      1670 {Text:junior}
      1671 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1672 {Constructing a box named junior}
      1673 +stack: level + 2 for brace
      1674 {Push hbox 6}
      1675 {end-group character }}
      1676 +stack: finish a box of type 513
      1677 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1678 +stack: level - 2 for brace
      1679 +\fi8
      1680 \bpers \undefined #2#3#4#5->\xbox {nom}{#4}\ifx #1\undefined \xbox {prenom}{#2}\else \xbox {prenom}{#1}\xbox {initiale}{#2}\fi \xbox {particule}{#3}\def \tmp {#1\undefined }\ifx \tmp \undefined \else \xbox {junior}{#5}\fi 
      1681 #1<-M.
      1682 #2<-M.
      1683 #3<-
      1684 #4<-Yor
      1685 #5<-
      1686 {\xbox}
      1687 {Push argument 6}
      1688 Character sequence: nom.
      1689 {Text:nom}
      1690 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1691 {Constructing a box named nom}
      1692 +stack: level + 2 for brace
      1693 {Push hbox 6}
      1694 Character sequence: Yor.
      1695 {end-group character }}
      1696 +stack: finish a box of type 513
      1697 {Text:Yor}
      1698 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1699 +stack: level - 2 for brace
      1700 +\ifx9
      1701 +iftest9 false
      1702 +\else9
      1703 {\xbox}
      1704 {Push argument 6}
      1705 Character sequence: prenom.
      1706 {Text:prenom}
      1707 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1708 {Constructing a box named prenom}
      1709 +stack: level + 2 for brace
      1710 {Push hbox 6}
      1711 Character sequence: M..
      1712 {end-group character }}
      1713 +stack: finish a box of type 513
      1714 {Text:M.}
      1715 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1716 +stack: level - 2 for brace
      1717 {\xbox}
      1718 {Push argument 6}
      1719 Character sequence: initiale.
      1720 {Text:initiale}
      1721 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1722 {Constructing a box named initiale}
      1723 +stack: level + 2 for brace
      1724 {Push hbox 6}
      1725 Character sequence: M..
      1726 {end-group character }}
      1727 +stack: finish a box of type 513
      1728 {Text:M.}
      1729 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1730 +stack: level - 2 for brace
      1731 +\fi9
      1732 {\xbox}
      1733 {Push argument 6}
      1734 Character sequence: particule.
      1735 {Text:particule}
      1736 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1737 {Constructing a box named particule}
      1738 +stack: level + 2 for brace
      1739 {Push hbox 6}
      1740 {end-group character }}
      1741 +stack: finish a box of type 513
      1742 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1743 +stack: level - 2 for brace
      1744 {\def}
      1745 {\def \tmp ->M.\undefined }
      1746 +\ifx10
      1747 +iftest10 false
      1748 +\else10
      1749 {\xbox}
      1750 {Push argument 6}
      1751 Character sequence: junior.
      1752 {Text:junior}
      1753 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b argument_b}
      1754 {Constructing a box named junior}
      1755 +stack: level + 2 for brace
      1756 {Push hbox 6}
      1757 {end-group character }}
      1758 +stack: finish a box of type 513
      1759 {Pop 6: document_v p_v argument_h biblio_t citation_t bauteurs_b hbox_b}
      1760 +stack: level - 2 for brace
      1761 +\fi10
      1762 {Pop 5: document_v p_v argument_h biblio_t citation_t bauteurs_b}
      1763 Character sequence:  .
      1764 [23] \cititem{btitle}{Probability laws related to the {J}acobi $\theta$ and {R}iemann $\zeta$ functions, and {B}rownian excursions}
      1765 {\cititem}
      1766 \cititem-btitle ->\bgroup \catcode `\$=12 \@nomathml =1 \citembtitle 
      1767 {begin-group character {}
      1768 +stack: level + 2 for brace
      1769 {\catcode}
      1770 +scanint for \catcode->36
      1771 +scanint for \catcode->12
      1772 {\@nomathml}
      1773 +scanint for \@nomathml->1
      1774 \citembtitle #1->\xbox {title}{#1}\egroup \@addnl 
      1775 #1<-Probability laws related to the {J}acobi $\theta $ and {R}iemann $\zeta $ functions, and {B}rownian excursions
      1776 {\xbox}
      1777 {Push argument 5}
      1778 Character sequence: title.
      1779 {Text:title}
      1780 {Pop 5: document_v p_v argument_h biblio_t citation_t argument_b}
      1781 {Constructing a box named title}
      1782 +stack: level + 3 for brace
      1783 {Push hbox 5}
      1784 Character sequence: Probability laws related to the .
      1785 {begin-group character {}
      1786 +stack: level + 4 for brace
      1787 Character sequence: J.
      1788 {end-group character }}
      1789 +stack: level - 4 for brace
      1790 Character sequence: acobi $.
      1791 {\theta}
      1792 Character sequence: $ and .
      1793 {begin-group character {}
      1794 +stack: level + 4 for brace
      1795 Character sequence: R.
      1796 {end-group character }}
      1797 +stack: level - 4 for brace
      1798 Character sequence: iemann $.
      1799 {\zeta}
      1800 Character sequence: $ functions, and .
      1801 {begin-group character {}
      1802 +stack: level + 4 for brace
      1803 Character sequence: B.
      1804 {end-group character }}
      1805 +stack: level - 4 for brace
      1806 Character sequence: rownian excursions.
      1807 {end-group character }}
      1808 +stack: finish a box of type 513
      1809 {Text:Probability laws related to the Jacobi $\theta $ and Riemann $\zeta $ functions, and Brownian excursions}
      1810 {Pop 5: document_v p_v argument_h biblio_t citation_t hbox_b}
      1811 +stack: level - 3 for brace
      1812 {end-group character }}
      1813 +stack: restoring integer value 0 for \@nomathml
      1814 +stack: restoring integer value 3 for \catcode36
      1815 +stack: level - 2 for brace
      1816 {\@addnl}
      1817 Character sequence:  .
      1818 [24] \cititem{bjournal}{Bull. Amer. Math. Soc.}
      1819 {\cititem}
      1820 {Push bjournal 5}
      1821 Character sequence: Bull. Amer. Math. Soc..
      1822 {Text:Bull. Amer. Math. Soc.}
      1823 {Pop 5: document_v p_v argument_h biblio_t citation_t bjournal_t}
      1824 Character sequence:  .
      1825 [25] \cititem{bvolume}{38}
      1826 {\cititem}
      1827 {Push bvolume 5}
      1828 Character sequence: 38.
      1829 {Text:38}
      1830 {Pop 5: document_v p_v argument_h biblio_t citation_t bvolume_t}
      1831 Character sequence:  .
      1832 [26] \cititem{byear}{2001}
      1833 {\cititem}
      1834 {Push byear 5}
      1835 Character sequence: 2001.
      1836 {Text:2001}
      1837 {Pop 5: document_v p_v argument_h biblio_t citation_t byear_t}
      1838 Character sequence:  .
      1839 [27] \cititem{bpages}{435--465}
      1840 {\cititem}
      1841 {Push bpages 5}
      1842 Character sequence: 435-465.
      1843 {Text:435&ndash;465}
      1844 {Pop 5: document_v p_v argument_h biblio_t citation_t bpages_t}
      1845 Character sequence:  .
      1846 [28] \endcitation
      1847 {\endcitation}
      1848 {Pop 4: document_v p_v argument_h biblio_t citation_t}
      1849 ++ Input stack empty at end of file
      1850 {Pop 3: document_v p_v argument_h biblio_t}
      1851 {Pop 2: document_v p_v argument_h}
      1852 Used 1866 commands
      1853 Math stats: formulas 16, non trivial kernels 10,
      1854    special 0, trivial 0, \mbox 0, large 0, small 8.
      1855 Buffer realloc 8, string 1881, size 25731, merge 2
      1856 Macros created 114, deleted 10.
      1857 Save stack +154 -154.
      1858 Attribute list search 1847(1549) found 1010 in 1620 elements (1450 after boot).
      1859 Number of ref 0, of used labels 0, of defined labels 0, of ext. ref. 0.
      1860 Modules with 0, without 0, sections with 0, without 0
      1861 There was no image.
      1862 Output written on bo.xml (5209 bytes).
      1863 No error found.
      1864 (For more information, see transcript file bo.log)
