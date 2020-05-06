==============
RAWeb examples
==============

Example of RAWeb source file
============================

Example of a raweb source file. This file is an extract from the RAWEB
of the miaou project. The syntax for the \`composition' module has
changed in 2006.

.. container:: ltx-source

   ::

      001 \documentclass{ra2003}
      002 \usepackage{amsfonts}
      003 \usepackage{amsmath}
      004 \theme{4a}
      005 \isproject{YES} % \isproject{OUI} works also
      006 \projet{MIAOU}{Miaou}{Math�matiques et 
           Informatique de l'Automatique et de l'Optimisation pour l'Utilisateur}
      007 \UR{\URSophia\URFuturs}

      009 \def\CC{{\mathbb C}}
      010 \newcommand{\etc}{etc}
      011 \def\corresp{manager}

      013 \declaretopic{abc}{Topic abc}
      014 \declaretopic{def}{Topic def}

      016 \begin{document}


      019 \maketitle
      020 \nocite{*}

      022 \begin{module}{composition}{en-tete}{}

      024 \begin{catperso}{Head of project team}
      025 \pers{Laurent}{Baratchart}[DR INRIA]
      026 \end{catperso}

      028 \begin{catperso}{Vice-head of project team}
      029 \pers{Juliette}{Leblond}[CR INRIA]
      030 \end{catperso}

      032 \begin{catperso}{Administrative assistant}
      033 \pers{France}{Limouzis}[AI INRIA, partial time in the project]
      034 \end{catperso}

      036 \begin{catperso}{Staff member}
      037 \pers{Jos�}{Grimm}[CR INRIA]
      038 \end{catperso}

      040 \begin{catperso}{Ph. D. Students}
      041 \pers{David}{Avanessoff}[Fellow, INRIA]
      042 \pers{Alex}{Bombrun}[Since November, 1st]
      043 \pers{Moncef}{Mahjoub}[Co-advised, ENIT Tunis 
            (in France in February, March, October, November)]
      044 \end{catperso}
      045 \end{module}

      047 \begin{module}{presentation}{presentation}{}
      048 \begin{moreinfo}
      049 The project was terminated June the 30th, 2003. 
      050 A proposal for a new project named APICS has been submitted to the steering
      051 comittee of Inria Sophia Antipolis.
      052 \end{moreinfo}

      054 The Team develops effective methods for modelling, identification and control
      055 of dynamical systems.

      057 \subsubsection*{Research Themes}
      058 \begin{itemize}
      059 \item Meromorphic and rational approximation in the complex domain,
      060   application to identification of transfer functions  and matrices as well as
      061   singularity detection for 2-D Laplace operators. Development of 
      062 %the hyperion
      063   software for frequency domain identification and synthesis of transfer
      064   matrices.  
      065 \item Control and structure of non-linear systems: continuous stabilization,
      066   non-linear transformations (linearization, classification). 
      067 \end{itemize}

      069 \subsubsection*{International and industrial partners}

      071 \begin{itemize}
      072 \item Industrial collaborations with Alcatel-Space, \etc
      073 \item Exchanges with CWI CNR (Italy), \etc
      074 \item The project is involved in a NATO Collaborative Linkage Grant \etc
      075 \end{itemize}
      076 \end{module}


      079 \begin{module}[def]{fondements}{identif}{Identification and deconvolution}
      080 Let us first introduce  the subject of Identification in some generality.

      082 Abstracting in the form 
      083 of mathematical equations the behavior of a phenomenon is
      084 a step called \emph{modeling}. It typically serves two purposes: the first
      085 is to describe the phenomenon with minimal complexity for some specific 
      086 purpose,
      087 the second is to \emph{predict} its outcome. \etc


      090 \subsubsection{Analytic approximation of incomplete boundary data}
      091 \label{dida-mero}
      092 \label{filtrescnes}
      093 \label{didactique-approx-rat-mat}
      094 \begin{participants}
      095 \pers{Laurent}{Baratchart},
      096 \pers{Jos�}{Grimm},
      097 \pers{Birgit}{Jacob}[University of Leeds (GB)],
      098 \pers{Juliette}{Leblond},
      099 \pers{Jean-Paul}{Marmorat}[CMA, �cole des Mines],
      100 \pers{Jonathan}{Partington},
      101 \pers{Fabien}{Seyfert}
      102 \end{participants}

      104 \begin{motscle}
      105 meromorphic approximation, frequency-domain identification,
      106 extremal problems
      107 \end{motscle}

      109 \etc, so that a prototypical Problem is:

      111 {\sl ($P$)~~Let $p \geq 1$, $N \geq 0$, $K$ be an arc of the unit circle $T$, 
      112   $f \in L^p(K)$, $\psi \in L^p(T \setminus K)$ and $M>0$;
      113   find a function  $g \in H^p + R_N$ such that 
      114   $\|g - \psi\|_{L^p(T \setminus K)} \leq M$ and such that $g - f$ 
      115   is of minimal norm in  $L^p(K)$ under this constraint.}

      117 Problem ($P$) is an extension to the meromorphic case, and to incomplete data,
      118 of classical analytic extremal problems (obtained
      119 by setting $K=T$ and $N=0$), that generically go under the name
      120 \textit{bounded extremal problems}. 
      121 These have been introduced and intensively studied by the Team, 
      122 \cite{blpprep} and ~\footcite{partII}. 

      124 \subsubsection{Scalar rational approximation}
      125 \label{didactique-approx-rat-scal}
      126 \begin{participants}
      127 \pers{Laurent}{Baratchart},
      128 \pers{Reinhold}{K�stner},
      129 \pers{Juliette}{Leblond},
      130 \pers{Martine}{Olivi},
      131 \pers{Edward}{Saff},
      132 \pers{Herbert}{Stahl},
      133 \pers{Franck}{Wielonsky}
      134 \end{participants}

      136 \begin{motscle}
      137 rational approximation, critical point, orthogonal polynomials
      138 \end{motscle}
      139 \etc.
      140 \begin{equation}
      141 \label{crit}
      142 \left\|f - \frac{p_m}{q_n} \right\|_{L^2(d \mu)} 
      143 \end{equation}
      144 where, by definition, 
      145 \[
      146 \|g\|_{L^2(d \mu)}^2=\frac{1}{2\pi} \int_{-\pi}^{\pi}|g(e^{i\theta})|^2
      147 d\mu(\theta),
      148 \]

      150 \etc

      152 If one introduces now as a new variable the rational matrix $R$ defined by
      153 \[
      154 R=\left(\begin{array}{cc}
      155 L            &  H \\
      156 0            &   I_m
      157 \end{array} \right)^{-1}
      158 \]
      159 and if $T$ stands for the first block-row, 
      160 normalizing the variance of the noise to be identity, the maximum likelihood
      161 estimator  is asymptotically equivalent, when the sample size increases, 
      162 to the minimization of
      163 \begin{equation}
      164 \label{defLL}
      165 \|T\|_{\Lambda}^2={\bf Tr}\left\{\frac{1}{2\pi}
      166 \int_{0}^{2\pi}T(e^{i\theta})\,
      167 d\Lambda(\theta)\,T^*(e^{i\theta})\right\},
      168 \end{equation}
      169 where $\Lambda$  is the spectral measure  of the process $(y~u)^t$ 
      170 (which positive and matrix-valued)
      171 and where  ${\bf Tr}$ indicates the trace. 

      173 \subsubsection{Continuous stabilization}
      174 Stabilization by continuous state feedback \etc

      176 \paragraph{Periodic stabilisation of non-linear systems.}
      177 It is known that \etc

      179 \paragraph{Control Lyapunov functions.}
      180 Lyapunov functions  are \etc
      181 \end{module}

      183 \begin{module}{domaine}{chapeau}{}
      184 The activity of the team focuses on two bottom lines, namely \etc
      185 \end{module}

      187 \begin{module}{}{dom-fissures}{Geometric inverse problems 
      188 for the Laplacian}
      189 \begin{participants}
      190 \pers{Laurent}{Baratchart}
      191 \end{participants}

      193 \begin{motscle}   % non destructive
      194 inverse problem, Laplace equation, non destructive control, tomography
      195 \end{motscle}

      197 Localizing cracks, \etc
      198 \end{module}


      201 \begin{module}{domaine}{resonn}{Identification and design of resonant systems}
      202 \begin{motscle}
      203 telecommunications, multiplexing, filtering device, hyperfrequency, surface waves
      204 \end{motscle}
      205 \end{module}

      207 \begin{module}[abc]{domaine}{spatial}{Spatial mechanics}
      208 \etc
      209 \end{module}


      212 \begin{module}[def]{domaine}{optique}{Non-linear Optics}
      213 \etc
      214 \end{module}


      217 \begin{module}{international}{domaine}{plat}{Transformations and equivalence of non-linear systems}

      219 \begin{participants}
      220 \pers{Laurent}{Baratchart},
      221 \pers{Jean-Baptiste}{Pomet},
      222 \pers{David}{Avanessoff}
      223 \end{participants}

      225 \begin{motscle}
      226 path planning, mobile cybernetics,  identification, {(max,plus) algebra}
      227 \end{motscle}
      228 \etc
      229 \end{module}

      231 \begin{module}{logiciels}{hyperion}{The hyperion software}

      233 \begin{participants}
      234 \pers{Jos�}{Grimm}[\corresp],
      235 \pers{Fabien}{Seyfert},
      236 \pers{Franck}{Wielonsky}
      237 \end{participants}
      238 \etc
      239 \end{module}

      241 \begin{module}{logiciels}{logi-tralics}{The Tralics software}

      243 \begin{participant}
      244 \pers{Jos�}{Grimm}[\corresp]
      245 \end{participant}
      246 \etc. \nocite{tralics-eurotex}
      247 \end{module}

      249 \begin{module}{logiciels}{RARL2}{The RARL2 software}
      250 \begin{participant}
      251 \pers{Jean-Paul}{Marmorat},
      252 \pers{Martine}{Olivi}[\corresp]
      253 \end{participant}

      255 RARL2 (R�alisation interne et Approximation Rationnelle L2) is a software for
      256 rational approximation (see module \ref{didactique-approx-rat-mat}). Its web
      257 page is
      258 \htmladdnormallink{\url{http://www-sop.inria.fr/miaou/RARL2/rarl2.html}}
      259 {http://www-sop.inria.fr/miaou/RARL2/rarl2.html}.
      260 \end{module}

      262 \begin{module}{logiciels}{RGC}{The RGC software}


      265 \begin{participants}
      266 \pers{Fabien}{Seyfert},
      267 \pers{Jean-Paul}{Marmorat}
      268 \end{participants}

      270 The RGC software \etc
      271 \end{module}

      273 \begin{module}{logiciels}{PRESTO-HF}{PRESTO-HF}
      274 \begin{participant}
      275 \pers{Fabien}{Seyfert}
      276 \end{participant}

      278 PRESTO-HF: a toolbox dedicated to lowpass parameter identification for
      279 hyperfrequency filters
      280 \htmladdnormallink{\url
            {http://www-sop.inria.fr/miaou/Fabien.Seyfert/Presto_web_page/presto_pres.html}}
      281 {http://www-sop.inria.fr/miaou/Fabien.Seyfert/Presto_web_page/presto_pres.html}
      282 \etc

      284 \end{module}

      286 \begin{module}{resultats}{tralics}{Tralics: a Latex to XML Translator}

      288 The main philosophy of Tralics is to have the same parser as \TeX, but the
      289 same semantics as \LaTeX. This means that commands like \verb+\chardef+, 
      290 \verb+\catcode+, \verb+\ifx+, \verb+\expandafter+, \verb+\csname+, etc.,
      291 that are not described in the \LaTeX\ book and not implemented in translators
      292 like latex2html, tth, h�v�a, etc., are recognised by Tralics. This year we
      293 added constructions like \verb=\endlinechar=, \verb=\read=,
      294 \verb=\uppercase=, \verb=\endinput=, which are less used, and a bit tricky. 
      295 Note that a construction like \verb=\ifdim\wd0>0pt\fi= is recognised by the
      296 parser, but there is no way to change the size of the box number zero, so that
      297 the test is always false. 

      290 For more information, see the
      300 \htmladdnormallink{Tralics web page}{http://www-sop.inria.fr/miaou/tralics/}.

      302 \begin{figure}
      303 \includegraphics[width=15cm]{xml-route}
      304 \label{xml-route}
      305 \caption{A slide that explains how the raweb operates. Rectangular boxes contain
      306   tools, diamond-shape boxes are style sheets, and ellipses contain language
      307   names; the name XML is in a double ellipse, it is the central object. The
      308   Perl script that handles the math formulas is not shown here;  it uses tools
      309   borrowed from latex2html.}
      310 \end{figure}

      312 \end{module}

      314 \begin{module}{resultats}{Couplages-Algebrique}
          {Parametrizations  of matrix-valued lossless functions}
      315 \etc
      316 \end{module}

      318 \begin{module}{resultats}{martine2}{The mathematics of Surface Acoustic Wave filters}
      319 \etc
      320 \end{module}

      322 \begin{module}[abc]{contrats}{cnes}{Contract ABCD-EFGH-INRIA}

      324 Contract \no 1 03 E 2145% this number is OK

      326 In the framework of a contract that links  ABCD, EFGH and Inria, 
      327 whose objective is \etc the work of Inria has been
      328 \begin{itemize}
      329 \item \etc see module \ref{filtrescnes},
      330 \item \etc  (see module  \moduleref{MIAOU}{resultats}{Couplages-Algebrique}),
      331 \item modeling and \etc, see module \ref{filtrescnes}.
      332 \end{itemize}
      333 In this contract, we promised version 1 of our software to both partners.
      334 This contract has been renewed in 2003.
      335 \end{module}


      338 \begin{module}{contrats}{aspi-c}{Contract Company Somename (Cannes)}

      340 Contract \no 1 01 E 0736.

      342 This contract started in 2001, for three years. 
      343 The objective is \etc
      344 \end{module}

      346 \begin{module}{contrats}{c-marcoussis}{Contract OtherName}

      348 Contract \no 1 02 E 0327.
      349 This was a one year contract, that ended formally in February, 2003.
      350 \begin{description}
      351 \item[Subject.] Objective was \etc
      352 \item[Outcome.] We have contributed to \etc
      353 \end{description}
      354 \end{module}




      359 \begin{module}{resultats}{nat}{Scientific Committees}

      361 L. Baratchart is member of the editorial board of \textit{Computational
      362 Methods in Function Theory}.

      364 \end{module}

      366 \begin{module}{international}{nationale}{National Actions}

      368 Together with project-teams Caiman and Odyss�e
      369 (INRIA-Sophia Antipolis, ENPC), the University of Nice (J.A. Dieudonn� lab.), 
      370 CEA, CNRS-LENA (Paris), and a few French hospitals, we are part of the
      371 national action \textbf{ACI Masse de donn�es �~OBS-CERV~�}, 2003-2006 (inverse
      372 problems, EEG).

      374 The \textbf{region PACA} (Provence Alpes C�te d'Azur) is partially supporting
      375 the post-doctaral stay of Per Enquist until May, 2004. We also obtained a (modest) grant from
      376 the region for exchanges with SISSA Trieste (Italy), 2003-2004.

      378 \end{module}

      380 \begin{module}{international}{cee}{Actions  Funded by the EC}
      381 The Team \etc
      382 The Team is member of the \textbf{TMR network}
      383 \emph{European Research Network on System Identification} (ERNSI), see
      384 \htmladdnormallink{\url{http://www.cwi.nl/~schuppen/ernsi/ernsihp.html}}
            {http://www.cwi.nl/~schuppen/ernsi/ernsihp.html}.
      385 This formally ended in February. A new proposal of a Research Training Network
      386 (RTN) has been submitted to the EC.

      388 The team obtained a \textbf{Marie Curie EIF} (Intra European Fellowship)
      389 FP6-2002-Mobility-5-502062, for 24 months (2003-2005). This finances Mario
      390 Sigalotti's post-doc.

      392 The Team is a member of the  \textbf{Marie Curie multi-partner training site}
      393 \emph{Control Training Site}, number HPMT-CT-2001-00278, 2001-2005. See
      394 \htmladdnormallink{\url{http://www.supelec.fr/lss/CTS/}}{http://www.supelec.fr/lss/CTS/}.

      396 The project is member of Working Group Control and System Theory
      397 of the \textbf{ERCIM} consortium, see
      398 \htmladdnormallink{\url{http://www.ladseb.pd.cnr.it/control/ercim/control.html}}
               {http://www.ladseb.pd.cnr.it/control/ercim/control.html}.
      399 \end{module}

      401 \begin{module}{international}{monde}{Extra-european International Actions}
      402 \textbf{NATO CLG} (Collaborative Linkage Grant), PST.CLG.979703, 
      403 �~Constructive approximation and inverse diffusion problems~�, with
      404 Vanderbilt Univ. (Nashville, USA) et le LAMSIN-ENIT (Tunis, Tu.), 2003-2005.
      405 \end{module}

      407 \begin{module}{international}{accueil}{Exterior research visitors}
      408 \iffalse
      409 Ceci est un test de moduleref:
      410 compo\moduleref{MIAOU}{composition}{}
      411 presen\moduleref{MIAOU}{presentation}{}
      412 fonde\moduleref{MIAOU}{fondements}{}
      413 dom\moduleref{MIAOU}{domaine}{}
      414 logici\moduleref{MIAOU}{logiciels}{}
      415 resu \moduleref{MIAOU}{resultats}{}
      416 resu \moduleref{MIAOU}{contrats}{}
      417 resu \moduleref{MIAOU}{international}{}
      418 resu \moduleref{MIAOU}{diffusion}{}
      419 logi-tra\moduleref{MIAOU}{logiciels}{tralics}
      420 \fi
      421 1=\ref{crit}, 2=\ref{xml-route}, 3=\moduleref{MIAOU}{fondements}{} 
         4=\moduleref{MIAOU}{fondements}{identif} 5=\ref{filtrescnes}

      423 In addition to the ``Scientific advisors'' and to the ``Visiting scientists''
      424 listed in section \moduleref{MIAOU}{composition}{}, 
      425 the following scientists visited us in 2003. 
      426 \begin{itemize}
      427 \item Mohamed Jaoua (Lamsin-ENIT, Tunis).
      428 \item Herbert Stahl (TU Berlin).
      429 \item \etc
      430 \end{itemize}
      431 \end{module}

      433 \begin{module}{diffusion}{dif-ens}{Teaching}

      435 \begin{description}
      436 \item [Courses] \ 
      437   \begin{itemize}
      438   \item D. Avanessoff \etc
      439   \item L. Baratchart, \etc
      440   \item J. Leblond \etc
      441   \end{itemize}
        
      443 \item [Trainees] \ 
      444   \begin{itemize}
      445   \item Antoine Chaillet, \etc
      446   \end{itemize}

      448 \item[Ph.D. Students] \ 
      449   \begin{itemize}
      450   \item David Avanessoff, �~Lin�arisation \etc~� 
      451    (dynamic linearization \etc)
      452   \item Fehmi Ben Hassen, <<~Localisation \etc~>>, 
      453   \item Alex Bombrun, \etc
      454 \end{itemize}
      455 \item[Ph.D. thesis defended] \ 
      456 \begin{itemize}
      457 \item Reinhold K�stner, \etc
      458 \end{itemize}
      459 \end{description}

      461 L. Baratchart was (president|rapporteur|examinateur)\footnote{Rayer les
      462   mentions inutiles}
      463  of the Thesis  of X and Y
      464 and Z\footnote{Remplacer les lettres par des noms}.

      466 \end{module}

      468 \begin{module}{diffusion}{dif-anim}{Community service}

      470 L. Baratchart is a member of the  ``bureau'' of the CP
      471 (Comit� des Projets) of INRIA-Sophia Antipolis.
      472 \end{module}

      474 \begin{module}{diffusion}{dif-conf}{Conferences and workshops}
      475 \begin{glossaire}\glo{A}{B\par C}\glo{A1}{B1\par C1}\end{glossaire}

      477 Talks, courses, sessions, software demonstrations at the
      478 CNRS-INRIA summer school ``Harmonic analysis and rational approximation: their
      479 r\^oles in signals, control and dynamical systems theory'',
      480 Porquerolles, september. 
      481 \htmladdnormallink{\url{http://www-sop.inria.fr/miaou/anap03/index.en.html}}
      482 {http://www-sop.inria.fr/miaou/anap03/index.en.html}

      484 J. Grimm gave a talk about Tralics at Eurotex 2003 (Brest)
      485 \end{module}


      488 \loadbiblio
      489 \end{document}

Example of bibtex file miaou2003.bib
====================================

.. container:: ltx-source

   ::

      @InProceedings{tralics-eurotex,
        author =   {Jos� Grimm},
        title =    {Tralics, a {\LaTeX} to XML Translator},
        booktitle =    {Proceedings of Eurotex},
        year =     2003
      }


      @InProceedings{seyfertIMS2003,
         author={F. Seyfert and J.-P. Marmorat and L. Baratchart 
             and S. Bila and J. Sombrin},
         title={Extraction of Coupling Parameters For Microwave Filters: 
           Determination of a Stable Rational Model from Scattering Data},
         journal={Proceedings of the International Microwave Symposium, 
             Philadelphia},
         year={2003}
      }



      @Misc{brevet,
        author =   {{European patent No. 03292257.7-}},
        note =     {Title: ``wavelength converter''. 
            Applicant/proprietor: Alcatel. Inventors: B. Lavigne, O. Leclerc, 
            J.-P. Moncelet, A. Bombrun, F. Seyfert, J.-B. Pomet},
        month =    sep,
        year =     2003,
        howpublished = {European patent office}
      }

      @PhdThesis{TheseReinhold,
        author =   {K�stner, Reinhold},
        title =    {Asymptotic Zero Distribution of Orthogonal 
            Polynomials with respect to
            Complex Measures having Argument of Bounded Variation},
        school =   {Universit� de Nice},
        year =     2003,
        month =    apr
      }

      @Article{blpprep,
        author =     {L. Baratchart and J. Grimm and J. Leblond and
                        J. R. Partington},
        title =      {Approximation and interpolation in $H^2$: Toeplitz
                        operators, recovery problems and error bounds},
        journal =    {Integral Equations and Operator Theory},
       year =        2003,
        volume =     45,
      pages={269--299}
      }

      @TechReport{Avan-Bar-Pom03rr,
        author =   {Avanessoff, D. and Baratchart, L. and Pomet, J.-B.},
        title =    {Sur l'int�grabilit� (tr�s) formelle d'une partie des
                        �quations de la platitude des syst�mes de contr�le},
        institution =  {INRIA},
        year =     2003,
        number =   5045,
        type =     {Rapport de recherche},
        month =    dec,
        url =      {http://www.inria.fr/rrrt/rr-5045.html}
      }

      @TechReport{Bara-Chy-Pom03,
        author =   {Baratchart, Laurent and Chyba, Monique and Pomet,
                        Jean-Baptiste},
        title =    {On the Grobman-Hartman theorem for control systems},
        institution =  {INRIA},
        year =     2003,
        type =     {Rapport de recherche},
        number =   5040,
        month =    dec,
        note={submitted to J. of differential equations},
        url =      {\rrrt{rr-5040}}
      }


      Bibtex standard entry types
      Tralics complains if the year is not 2003. 
      The note field is always optional
      An URL field is always accepted.

      @Article{std1,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {An article from a journal or magazine},
        journal =      {Journal},
        year =     {2003},
        OPTkey =   {},
        volume =   {opt-volume},
        number =   {opt-number},
        pages =    {opt-pages},
        month =    {opt-month},
        note =     {The note},
      }

      @Book{std2,
        author =   {Mittelbach, Frank and Michel Goossens},
        ALTeditor =    {Give author or editor},
        title =    {A book with an explicit publisher},
        publisher =    {The Publisher},
        year =     {2003},
        volume =   {opt-volume},
        OPTnumber =    {volume or number},
        series =   {opt-series},
        address =      {opt-address},
        edition =      {opt-edition},
        month =    {opt-month},
        note =     {The note},
      }

      @Booklet{std3,
        title =    {A work that is printed and bound, but without a named
        publisher or sponsoring institution},
        author =   {Optional Author},
        howpublished = {opt-howpublished},
        address =      {Address of Publisher},
        month =    {opt-month},
        year =     {2003},
        note =     {The note},
      }

      @InBook{std4,
        ALTauthor =    {Give author or editor},
        editor =   {Mittelbach, Frank and Michel Goossens},
        title =    {A part of a book, e.g. a chapter, section,  or whatever
         and/or a range of pages},
        chapter =      {4321},
        publisher =    {A Publisher},
        year =     {2003},
        volume =   {opt-volume},
        OPTnumber =    {volume or number},
        series =   {opt-series},
        type =     {opt-type},
        address =      {opt-address},
        edition =      {opt-edition},
        month =    {opt-month},
        pages =    {123--125},
        note =     {The note},
      }

      @InCollection{std5,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {A part of a book having its own title},
        booktitle =    {The title of the book},
        pages =    {opt-pages},
        publisher =    {Publisher of the paper},
        year =     {2003},
        editor =   {Optional Editor and His Family},
        volume =   {opt-volume},
        OPTnumber =    {volume or number},
        series =   {opt-series},
        type =     {opt-type},
        chapter =      {opt-chapter},
        address =      {Optional Address},
        edition =      {opt-edition},
        month =    {opt-month},
        note =     {The note},
      }

      @InProceedings{std6,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {An article in a conference proceedings},
        booktitle =    {Title of the proceedings},
        pages =    {opt-pages},
        year =     {2003},
        editor =   {One Editor and His Family},
        volume =   {opt-volume},
        OPTnumber =    {volume or number},
        series =   {opt-series},
        address =      {Optional Address},
        month =    {opt-month},
        organization = {Optional Organization of the Conference},
        publisher =    {Optional Publisher},
        note =     {The note},
      }

      @Manual{std7,
        title =    {Technical documentation},
        author =   {Optional Author},
        organization = {Optional Organization},
        address =      {Optional Address},
        edition =      {opt-edition},
        month =    {opt-month},
        year =     {2003},
        note =     {The Note},
      }


      @MastersThesis{std8,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {A master's thesis},
        school =   {School of the thesis},
        year =     {2003},
        type =     {Optional Type},
        address =      {Optional Address},
        month =    {opt-month},
        note =     {The note},
      }

      @Misc{std9,
        author =   {Optional Author},
        title =    {Use this when nothing else fits.
        {A} warning will be issued if all optional fields are empty
        (i.e. the entire field is empty)},
        howpublished = {opt-howpublished},
        month =    {opt-month},
        year =     {2003},
        note =     {The note},
      }


      @PhdThesis{std10,
        author =   {Mittelbach, Frank},
        title =    {A {Ph.D.} thesis},
        school =   {School},
        year =     {2003},
        type =     {optional type},
        address =      {Optional Address},
        month =    {opt-month},
        note =     {The note},
      }


      @Proceedings{std11,
        title =    {Conference proceedings},
        year =     {2003},
        editor =   {Optional Editor},
        volume =   {opt-volume},
        OPTnumber =    {volume or number},
        series =   {optional series},
        address =      {Optional Address},
        month =    {opt-month},
        organization = {Optional Organization},
        publisher =    {Optional Publisher},
        note =     {The note},
      }

      @TechReport{std12,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {A report published by a school or other institution,
            usually numbered within a series},
        institution =  {The institution},
        year =     {2003},
        type =     {optional-type},
        number =   {optional-number},
        address =      {Optional Address},
        month =    {opt-month},
        note =     {The note},
      }

      @Unpublished{std13,
        author =   {Mittelbach, Frank and Michel Goossens},
        title =    {A document having an author and title, but not formally published},
        note =     {The note},
        month =    {opt-month},
        year =     {2003},
      }

Example of bibtex file miaou_foot2003.bib
=========================================

.. container:: ltx-source

   ::

      @Article{lswprep,
        author =       {J. Leblond and E.B. Saff and F. Wielonsky},
        title =        {Weighted {$H_2$} rational approximation and consistency
                        properties},
        journal =      {Numerische Mathematik},
        volume =   90,
        number =   3,
        pages =    {521-554},
      xxurl= {http://dx.doi.org/10.1007/s002110100281},
      doi={10.1007/s002110100281},
      year =       2002
      }


      @InProceedings{O-CDC,
        author =   {Marmorat, Jean-Paul and Olivi, Martine and Hanzon, B. and Peeters, R},
        title =    {Matrix rational $H^2$-approximation: a state-space approach using Schur
      parameters},
        booktitle =    {41st IEEE Conf. on Decision and Control},
        year =     2002,
        address =  {Las Vegas (USA)},
        month =    dec,
        doi="10.1137/S089547980139371X"
      }

      @InProceedings{PHO-MTNS,
        author =   {Peeters, R and Hanzon, B. and Olivi, Martine},
        title =    {On a recursive state-space method for discrete-time $H^2$-approximation},
        booktitle =    {MTNS 2002},
        year =     2002,
        address =  {Notre-Dame (USA)},
        month =    aug,
        doi = "10.1137/S0895479899359539",
      }


      @article{partII,
        author =   {L. Baratchart and J. Leblond},
        title =    {Hardy approximation to {$L^p$} functions on subsets of the
                        circle with $1 \leq p < \infty$},
        journal =  {Constructive Approximation},
        year =     1998,
        volume =   14,
        pages =    {41-56}
      }

Example of bibtex file miaou_refer2003.bib
==========================================

.. container:: ltx-source

   ::

      @Article{BO1,
        author =   {L. Baratchart and M. Olivi},
        title =    {Critical points and error rank in best $H^2$ matrix rational
                        approximation of fixed McMillan degree},
        journal =  {Constructive Approximation},
        volume =   14,
        year =     1998,
        pages =    {273-300}
      }

      @Article{lo,
        author =   {J. Leblond and M. Olivi},
        title =    {Weighted {$H^2$} approximation of transfer functions},
        journal =  "Math. of Control, Signals \& Systems (MCSS)",
        year =     1998,
        volume =   11,
        pages =    {28-39},
      }

      @article{fo,
        author =   {P. Fulcheri and M. Olivi},
        journal =  "SIAM J. on Control \& Optim.",
        title =    {Matrix rational $H^2$-approximation:~a gradient algorithm
                        based on Schur analysis},
        volume =   36,
        year =     1998,
        pages =    {2103-2127}
      }

      @TechReport{RTCNES,
        author =   {L. Baratchart and J. Grimm and J. Leblond and M. Olivi and
                        F. Seyfert and F. Wielonsky},
        title =    {Identification d'un filtre hyperfr\'equence par
                        approximation dans le domaine complexe},
        institution =  {Inria},
        year =     1998,
        type =         {Rapport technique},
        number =   {RT-219},
      url= {http://www.inria.fr/rrrt/rt-0219.html}
      }
      @article{papiercarthage,
        author =   {L. Baratchart and J. Leblond and F. Mandr�a and E.B. Saff},
        title =    {How can meromorphic approximation help to solve some 2D
                        inverse problems for the Laplacian?},
        journal =  {Inverse Problems},
        year =     1999,
        volume =   15,
        pages =    {79--90}
      }

      @article{bco,
        author =   {L. Baratchart and M. Cardelli and M. Olivi},
        title =    {Identification and rational {$L^2$} approximation: a
                        gradient algorithm},
        journal =  {Automatica},
        publisher =    {Pergamon Press},
        volume =   27,
        year =     1991,
        pages =    {413-418}
      }

      @Article{BW,
        author =   {L. Baratchart and F. Wielonsky},
        title =    {Rational approximation in the real {H}ardy space {$H_{2}$}
                        and {S}tieltjes integrals: a uniqueness theorem},
        journal =  {Constructive Approximation},
        VOLUME =   9,
        YEAR =     1993,
        PAGES =    {1-21}
      }

      @Article{BSW,
        author =   "L. Baratchart and E. B. Saff and F. Wielonsky",
        title =    "A criterion for uniqueness of a critical point in {$H^2$}
                        rational approximation",
        journal =  "Journal d'Analyse",
        year =     1996,
        volume =   70,
        pages =    "225-266"
      }

      @ARTICLE{Pome92scl,
        AUTHOR =   {J.-B. Pomet},
        JOURNAL =  "Syst. \& Control Lett.",
        PAGES =    {147-158},
        TITLE =    {Explicit Design of Time-Varying Stabilizing Control Laws for
                        a Class of Controllable Systems without Drift},
        VOLUME =   {18},
        YEAR =     {1992}
      }

      @ARTICLE{Pome97cocv,
        AUTHOR =   {Jean-Baptiste Pomet},
        JOURNAL =  "Control, Optimization, and the Calculus of Variations
                        (COCV)",
        MONTH =    jun,
        url =      {http://www.edpsciences.com/cocv/},
        PAGES =    {151-230},
        TITLE =    {On Dynamic Feedback Linearization of Four-dimensional Affine
                        Control Systems with Two Inputs},
        VOLUME =   {2},
        YEAR =     {1997}
      }

XML generated by extractmath
============================

.. container:: ltx-source

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE fo:root SYSTEM '/user/grimm/home/cvs/raweb//dtd/raaux.dtd'>
      <fo:root xmlns:fo='http://www.w3.org/1999/XSL/Format'
          xmlns:fotex='http://www.tug.org/fotex'
          xmlns:m='http://www.w3.org/1998/Math/MathML'>
      <fo:block>
      <formula id='1'><math xmlns='http://www.w3.org/1998/Math/MathML'>
       <mrow><mo>&Verbar;</mo><mi>g</mi><mo> -</mo>
       <mi>&psi;</mi><msub><mo>&Verbar;</mo> <mrow>
       <msup><mi>L</mi> <mi>p</mi> </msup><mrow>
       <mo>(</mo><mi>T</mi><mo>&setminus;</mo><mi>K
       </mi><mo>)</mo></mrow></mrow> </msub><mo>&le;</mo>
       <mi>M</mi></mrow></math></formula>

      <formula id='2'><math xmlns='http://www.w3.org/1998/Math/MathML'><msub>
       <mfenced open='&Verbar;' close='&Verbar;'><mi>f</mi><mo>-</mo>
       <mfrac><msub><mi>p</mi> <mi>m</mi> </msub> <msub>
       <mi>q</mi> <mi>n</mi> </msub></mfrac></mfenced> 
       <mrow><msup><mi>L</mi> <mn>2</mn> </msup>
       <mrow><mo>(</mo><mi>d</mi><mi>&mu;</mi><mo>)</mo></mrow></mrow> </msub></math></formula>

      <formula id='3'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <mo>&Verbar;</mo><mi>g</mi><msubsup><mo>&Verbar;</mo>
        <mrow><msup><mi>L</mi> <mn>2</mn> </msup><mrow>
       <mo>(</mo><mi>d</mi><mi>&mu;</mi><mo>)</mo>
       </mrow></mrow> <mn>2</mn> </msubsup><mo>=</mo><mfrac>
       <mn>1</mn> <mrow><mn>2</mn><mi>&pi;</mi></mrow>
       </mfrac><msubsup><mo>&int;</mo> <mrow><mo>-</mo>
       <mi>&pi;</mi></mrow> <mi>&pi;</mi> </msubsup>
       <mrow><mo>|</mo><mi>g</mi><mo>(</mo></mrow>
       <msup><mi>e</mi> <mrow><mi>i</mi><mi>&theta;</mi>
       </mrow> </msup><mrow><mo>)</mo></mrow><msup>
       <mo>|</mo> <mn>2</mn> </msup><mrow><mi>d</mi>
       <mi>&mu;</mi><mo>(</mo><mi>&theta;</mi>
      <mo>)</mo><mo>,</mo></mrow></mrow></math></formula>

      <formula id='4'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <mi>R</mi><mo>=</mo><msup><mfenced open='(' close=')'>
       <mtable><mtr><mtd><mi>L</mi></mtd><mtd><mi>H</mi>
       </mtd></mtr><mtr><mtd><mn>0</mn></mtd><mtd>
       <msub><mi>I</mi> <mi>m</mi> </msub></mtd></mtr>
       </mtable></mfenced> <mrow><mo>-</mo><mn>1</mn></mrow> </msup></mrow></math></formula>

      <formula id='5'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <mo>&Verbar;</mo><mi>T</mi><msubsup><mo>&Verbar;</mo>
        <mi>&Lambda;</mi> <mn>2</mn> </msubsup><mo>=</mo>
       <mrow><mi mathvariant='bold'>T</mi><mi mathvariant='bold'>r</mi>
       </mrow><mfenced open='&lbrace;' close='&rbrace;'><mfrac><mn>1</mn>
        <mrow><mn>2</mn><mi>&pi;</mi></mrow></mfrac><msubsup>
       <mo>&int;</mo> <mn>0</mn> <mrow><mn>2</mn>
       <mi>&pi;</mi></mrow> </msubsup><mrow><mi>T</mi>
       <mo>(</mo></mrow><msup><mi>e</mi> <mrow><mi>i</mi>
       <mi>&theta;</mi></mrow> </msup><mrow><mo>)</mo>
       <mspace width='0.166667em'/><mi>d</mi><mi>&Lambda;</mi>
       <mo>(</mo><mi>&theta;</mi><mo>)</mo><mspace width='0.166667em'/>
       </mrow><msup><mi>T</mi> <mo>*</mo> </msup><mrow>
       <mo>(</mo></mrow><msup><mi>e</mi> <mrow><mi>i</mi>
       <mi>&theta;</mi></mrow> </msup><mrow><mo>)</mo>
       </mrow></mfenced>
       <mo>,</mo></mrow></math></formula>
      </fo:block>
      </fo:root>

Result of conversion of miaou2003.tex by tralics -ps
====================================================

.. container:: ltx-source

   ::

      \documentclass{raweb}
      \let\xloadbiblio\loadbiblio
      \let\loadbiblio\relax\let\maketitle\relax\usepackage{amsfonts}
      \usepackage{amsmath}

      \def\CC{{\mathbb C}}
      \newcommand{\etc}{etc}
      \def\corresp{manager}


      \begin{document}
      \def\RAprojet{MIAOU}
      \def\RAprojetlow{miaou}
      \def\RAprojetlatex{Miaou}
      \def\RAprojetfull{Math�matiques et Informatique de l'Automatique et de l'Optimisation pour l'Utilisateur}
      \RAstartprojet{num}{1}{\RAprojetlatex}{\RAprojetfull}{Sophia Antipolis --- Futurs}
      \mytableofcontents


      \maketitle
      \nocite{*}
      \loadbiblio
      \def\RAmysection{composition}
      \section{\choseRAsection{1}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{0}{MIAOU}{1}{en-tete}{}
      \RAlabel{MIAOU@composition@en-tete}


      \begin{catperso}{Head of project team}
      \pers{Laurent}{Baratchart}[DR INRIA]
      \end{catperso}

      \begin{catperso}{Vice-head of project team}
      \pers{Juliette}{Leblond}[CR INRIA]
      \end{catperso}

      \begin{catperso}{Administrative assistant}
      \pers{France}{Limouzis}[AI INRIA, partial time in the project]
      \end{catperso}

      \begin{catperso}{Staff member}
      \pers{Jos�}{Grimm}[CR INRIA]
      \end{catperso}

      \begin{catperso}{Ph. D. Students}
      \pers{David}{Avanessoff}[Fellow, INRIA]
      \pers{Alex}{Bombrun}[Since November, 1st]
      \pers{Moncef}{Mahjoub}[Co-advised, ENIT Tunis (in France in February, March, October, November)]
      \end{catperso}
      \def\RAmysection{presentation}
      \section{\choseRAsection{2}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{0}{MIAOU}{2}{presentation}{}
      \RAlabel{MIAOU@presentation@presentation}

      \begin{moreinfo}
      The project was terminated June the 30th, 2003. 
      A proposal for a new project named APICS has been submitted to the steering
      comittee of Inria Sophia Antipolis.
      \end{moreinfo}

      The Team develops effective methods for modelling, identification and control
      of dynamical systems.

      \subsubsection*{Research Themes}
      \begin{itemize}
      \item Meromorphic and rational approximation in the complex domain,
        application to identification of transfer functions  and matrices as well as
        singularity detection for 2-D Laplace operators. Development of 
      %the hyperion
        software for frequency domain identification and synthesis of transfer
        matrices.  
      \item Control and structure of non-linear systems: continuous stabilization,
        non-linear transformations (linearization, classification). 
      \end{itemize}

      \subsubsection*{International and industrial partners}

      \begin{itemize}
      \item Industrial collaborations with Alcatel-Space, \etc
      \item Exchanges with CWI CNR (Italy), \etc
      \item The project is involved in a NATO Collaborative Linkage Grant \etc
      \end{itemize}
      \def\RAmysection{fondements}
      \section{\choseRAsection{3}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{2}{MIAOU}{3}{identif}{Identification and deconvolution}
      \RAlabel{MIAOU@fondements@identif}

      Let us first introduce  the subject of Identification in some generality.

      Abstracting in the form 
      of mathematical equations the behavior of a phenomenon is
      a step called \emph{modeling}. It typically serves two purposes: the first
      is to describe the phenomenon with minimal complexity for some specific 
      purpose,
      the second is to \emph{predict} its outcome. \etc


      \subsubsection{Analytic approximation of incomplete boundary data}
      \label{dida-mero}
      \label{filtrescnes}
      \label{didactique-approx-rat-mat}
      \begin{participants}
      \pers{Laurent}{Baratchart},
      \pers{Jos�}{Grimm},
      \pers{Birgit}{Jacob}[University of Leeds (GB)],
      \pers{Juliette}{Leblond},
      \pers{Jean-Paul}{Marmorat}[CMA, �cole des Mines],
      \pers{Jonathan}{Partington},
      \pers{Fabien}{Seyfert}
      \end{participants}

      \begin{motscle}
      meromorphic approximation, frequency-domain identification,
      extremal problems 
      \end{motscle}

      \etc, so that a prototypical Problem is:

      {\sl ($P$)~~Let $p \geq 1$, $N \geq 0$, $K$ be an arc of the unit circle $T$, 
        $f \in L^p(K)$, $\psi \in L^p(T \setminus K)$ and $M>0$;
        find a function  $g \in H^p + R_N$ such that 
        $\|g - \psi\|_{L^p(T \setminus K)} \leq M$ and such that $g - f$ 
        is of minimal norm in  $L^p(K)$ under this constraint.}

      Problem ($P$) is an extension to the meromorphic case, and to incomplete data,
      of classical analytic extremal problems (obtained
      by setting $K=T$ and $N=0$), that generically go under the name
      \textit{bounded extremal problems}. 
      These have been introduced and intensively studied by the Team, 
      \cite{blpprep} and ~\footcite{partII}. 

      \subsubsection{Scalar rational approximation}
      \label{didactique-approx-rat-scal}
      \begin{participants}
      \pers{Laurent}{Baratchart},
      \pers{Reinhold}{K�stner},
      \pers{Juliette}{Leblond},
      \pers{Martine}{Olivi},
      \pers{Edward}{Saff},
      \pers{Herbert}{Stahl},
      \pers{Franck}{Wielonsky}
      \end{participants}

      \begin{motscle}
      rational approximation, critical point, orthogonal polynomials
      \end{motscle}
      \etc.
      \begin{equation}
      \label{crit}
      \left\|f - \frac{p_m}{q_n} \right\|_{L^2(d \mu)} 
      \end{equation}
      where, by definition, 
      \[
      \|g\|_{L^2(d \mu)}^2=\frac{1}{2\pi} \int_{-\pi}^{\pi}|g(e^{i\theta})|^2
      d\mu(\theta),
      \]

      \etc

      If one introduces now as a new variable the rational matrix $R$ defined by
      \[
      R=\left(\begin{array}{cc}
      L            &  H \\
      0            &   I_m
      \end{array} \right)^{-1}
      \]
      and if $T$ stands for the first block-row, 
      normalizing the variance of the noise to be identity, the maximum likelihood
      estimator  is asymptotically equivalent, when the sample size increases, 
      to the minimization of
      \begin{equation}
      \label{defLL}
      \|T\|_{\Lambda}^2={\bf Tr}\left\{\frac{1}{2\pi}
      \int_{0}^{2\pi}T(e^{i\theta})\,
      d\Lambda(\theta)\,T^*(e^{i\theta})\right\},
      \end{equation}
      where $\Lambda$  is the spectral measure  of the process $(y~u)^t$ 
      (which positive and matrix-valued)
      and where  ${\bf Tr}$ indicates the trace. 

      \subsubsection{Continuous stabilization}
      Stabilization by continuous state feedback \etc

      \paragraph{Periodic stabilisation of non-linear systems.}
      It is known that \etc

      \paragraph{Control Lyapunov functions.}
      Lyapunov functions  are \etc
      \def\RAmysection{domaine}
      \section{\choseRAsection{4}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{1}{MIAOU}{4}{chapeau}{}
      \RAlabel{MIAOU@domaine@chapeau}

      The activity of the team focuses on two bottom lines, namely \etc
      \RAstartmodule{1}{MIAOU}{4}{dom-fissures}{Geometric inverse problems  for the Laplacian}
      \RAlabel{MIAOU@domaine@dom-fissures}

      \begin{participants}
      \pers{Laurent}{Baratchart}
      \end{participants}

      \begin{motscle}   % non destructive
      inverse problem, Laplace equation, non destructive control, tomography
      \end{motscle}

      Localizing cracks, \etc
      \RAstartmodule{1}{MIAOU}{4}{resonn}{Identification and design of resonant systems}
      \RAlabel{MIAOU@domaine@resonn}

      \begin{motscle}
      telecommunications, multiplexing, filtering device, hyperfrequency, surface waves
      \end{motscle}
      \RAstartmodule{1}{MIAOU}{4}{spatial}{Spatial mechanics}
      \RAlabel{MIAOU@domaine@spatial}

      \etc
      \RAstartmodule{2}{MIAOU}{4}{optique}{Non-linear Optics}
      \RAlabel{MIAOU@domaine@optique}

      \etc
      \RAstartmodule{1}{MIAOU}{4}{plat}{Transformations and equivalence of non-linear systems}
      \RAlabel{MIAOU@domaine@plat}


      \begin{participants}
      \pers{Laurent}{Baratchart},
      \pers{Jean-Baptiste}{Pomet},
      \pers{David}{Avanessoff}
      \end{participants}

      \begin{motscle}
      path planning, mobile cybernetics,  identification, {(max,plus) algebra}
      \end{motscle}
      \etc
      \def\RAmysection{logiciels}
      \section{\choseRAsection{5}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{1}{MIAOU}{5}{hyperion}{The hyperion software}
      \RAlabel{MIAOU@logiciels@hyperion}


      \begin{participants}
      \pers{Jos�}{Grimm}[\corresp],
      \pers{Fabien}{Seyfert},
      \pers{Franck}{Wielonsky}
      \end{participants}
      \etc
      \RAstartmodule{1}{MIAOU}{5}{logi-tralics}{The Tralics software}
      \RAlabel{MIAOU@logiciels@logi-tralics}


      \begin{participant}
      \pers{Jos�}{Grimm}[\corresp]
      \end{participant}
      \etc. \nocite{tralics-eurotex}
      \RAstartmodule{1}{MIAOU}{5}{RARL2}{The RARL2 software}
      \RAlabel{MIAOU@logiciels@RARL2}

      \begin{participant}
      \pers{Jean-Paul}{Marmorat},
      \pers{Martine}{Olivi}[\corresp]
      \end{participant}

      RARL2 (R�alisation interne et Approximation Rationnelle L2) is a software for
      rational approximation (see module \ref{didactique-approx-rat-mat}). Its web
      page is
      \htmladdnormallink{\url{http://www-sop.inria.fr/miaou/RARL2/rarl2.html}}
      {http://www-sop.inria.fr/miaou/RARL2/rarl2.html}.
      \RAstartmodule{1}{MIAOU}{5}{RGC}{The RGC software}
      \RAlabel{MIAOU@logiciels@RGC}



      \begin{participants}
      \pers{Fabien}{Seyfert},
      \pers{Jean-Paul}{Marmorat}
      \end{participants}

      The RGC software \etc
      \RAstartmodule{1}{MIAOU}{5}{PRESTO-HF}{PRESTO-HF}
      \RAlabel{MIAOU@logiciels@PRESTO-HF}

      \begin{participant}
      \pers{Fabien}{Seyfert}
      \end{participant}

      PRESTO-HF: a toolbox dedicated to lowpass parameter identification for
      hyperfrequency filters
      \htmladdnormallink{\url{http://www-sop.inria.fr/miaou/Fabien.Seyfert/Presto_web_page/presto_pres.html}}
      {http://www-sop.inria.fr/miaou/Fabien.Seyfert/Presto_web_page/presto_pres.html}
      \etc

      \def\RAmysection{resultats}
      \section{\choseRAsection{6}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{1}{MIAOU}{6}{tralics}{Tralics: a Latex to XML Translator}
      \RAlabel{MIAOU@resultats@tralics}


      The main philosophy of Tralics is to have the same parser as \TeX, but the
      same semantics as \LaTeX. This means that commands like \verb+\chardef+, 
      \verb+\catcode+, \verb+\ifx+, \verb+\expandafter+, \verb+\csname+, etc.,
      that are not described in the \LaTeX\ book and not implemented in translators
      like latex2html, tth, h�v�a, etc., are recognised by Tralics. This year we
      added constructions like \verb=\endlinechar=, \verb=\read=,
      \verb=\uppercase=, \verb=\endinput=, which are less used, and a bit tricky. 
      Note that a construction like \verb=\ifdim\wd0>0pt\fi= is recognised by the
      parser, but there is no way to change the size of the box number zero, so that
      the test is always false. 

      For more information, see the
      \htmladdnormallink{Tralics web page}{http://www-sop.inria.fr/miaou/tralics/}.

      \begin{figure}
      \includegraphics[width=15cm]{xml-route}
      \label{xml-route}
      \caption{A slide that explains how the raweb operates. Rectangular boxes contain
        tools, diamond-shape boxes are style sheets, and ellipses contain language
        names; the name XML is in a double ellipse, it is the central object. The
        Perl script that handles the math formulas is not shown here;  it uses tools
        borrowed from latex2html.}
      \end{figure}

      \RAstartmodule{1}{MIAOU}{6}{Couplages-Algebrique}{Parametrizations  of matrix-valued lossless functions}
      \RAlabel{MIAOU@resultats@Couplages-Algebrique}

      \etc
      \RAstartmodule{1}{MIAOU}{6}{martine2}{The mathematics of Surface Acoustic Wave filters}
      \RAlabel{MIAOU@resultats@martine2}

      \etc
      \RAstartmodule{1}{MIAOU}{6}{nat}{Scientific Committees}
      \RAlabel{MIAOU@resultats@nat}


      L. Baratchart is member of the editorial board of \textit{Computational
      Methods in Function Theory}.

      \def\RAmysection{contrats}
      \section{\choseRAsection{7}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{0}{MIAOU}{7}{cnes}{Contract ABCD-EFGH-INRIA}
      \RAlabel{MIAOU@contrats@cnes}


      Contract \no 1 03 E 2145% this number is OK

      In the framework of a contract that links  ABCD, EFGH and Inria, 
      whose objective is \etc the work of Inria has been
      \begin{itemize}
      \item \etc see module \ref{filtrescnes},
      \item \etc  (see module  \moduleref{MIAOU}{resultats}{Couplages-Algebrique}),
      \item modeling and \etc, see module \ref{filtrescnes}.
      \end{itemize}
      In this contract, we promised version 1 of our software to both partners.
      This contract has been renewed in 2003.
      \RAstartmodule{0}{MIAOU}{7}{aspi-c}{Contract Company Somename (Cannes)}
      \RAlabel{MIAOU@contrats@aspi-c}


      Contract \no 1 01 E 0736.

      This contract started in 2001, for three years. 
      The objective is \etc
      \RAstartmodule{0}{MIAOU}{7}{c-marcoussis}{Contract OtherName}
      \RAlabel{MIAOU@contrats@c-marcoussis}


      Contract \no 1 02 E 0327.
      This was a one year contract, that ended formally in February, 2003.
      \begin{description}
      \item[Subject.] Objective was \etc
      \item[Outcome.] We have contributed to \etc
      \end{description}
      \def\RAmysection{international}
      \section{\choseRAsection{8}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{0}{MIAOU}{8}{nationale}{National Actions}
      \RAlabel{MIAOU@international@nationale}


      Together with project-teams Caiman and Odyss�e
      (INRIA-Sophia Antipolis, ENPC), the University of Nice (J.A. Dieudonn� lab.), 
      CEA, CNRS-LENA (Paris), and a few French hospitals, we are part of the
      national action \textbf{ACI Masse de donn�es �~OBS-CERV~�}, 2003-2006 (inverse
      problems, EEG).

      The \textbf{region PACA} (Provence Alpes C�te d'Azur) is partially supporting
      the post-doctaral stay of Per Enquist until May, 2004. We also obtained a (modest) grant from
      the region for exchanges with SISSA Trieste (Italy), 2003-2004.

      \RAstartmodule{0}{MIAOU}{8}{cee}{Actions  Funded by the EC}
      \RAlabel{MIAOU@international@cee}

      The Team \etc
      The Team is member of the \textbf{TMR network}
      \emph{European Research Network on System Identification} (ERNSI), see
      \htmladdnormallink{\url{http://www.cwi.nl/~schuppen/ernsi/ernsihp.html}}{http://www.cwi.nl/~schuppen/ernsi/ernsihp.html}.
      This formally ended in February. A new proposal of a Research Training Network
      (RTN) has been submitted to the EC.

      The team obtained a \textbf{Marie Curie EIF} (Intra European Fellowship)
      FP6-2002-Mobility-5-502062, for 24 months (2003-2005). This finances Mario
      Sigalotti's post-doc.

      The Team is a member of the  \textbf{Marie Curie multi-partner training site}
      \emph{Control Training Site}, number HPMT-CT-2001-00278, 2001-2005. See
      \htmladdnormallink{\url{http://www.supelec.fr/lss/CTS/}}{http://www.supelec.fr/lss/CTS/}.

      The project is member of Working Group Control and System Theory
      of the \textbf{ERCIM} consortium, see
      \htmladdnormallink{\url{http://www.ladseb.pd.cnr.it/control/ercim/control.html}}{http://www.ladseb.pd.cnr.it/control/ercim/control.html}.
      \RAstartmodule{0}{MIAOU}{8}{monde}{Extra-european International Actions}
      \RAlabel{MIAOU@international@monde}

      \textbf{NATO CLG} (Collaborative Linkage Grant), PST.CLG.979703, 
      �~Constructive approximation and inverse diffusion problems~�, with
      Vanderbilt Univ. (Nashville, USA) et le LAMSIN-ENIT (Tunis, Tu.), 2003-2005.
      \RAstartmodule{0}{MIAOU}{8}{accueil}{Exterior research visitors}
      \RAlabel{MIAOU@international@accueil}

      \iffalse
      Ceci est un test de moduleref:
      compo\moduleref{MIAOU}{composition}{}
      presen\moduleref{MIAOU}{presentation}{}
      fonde\moduleref{MIAOU}{fondements}{}
      dom\moduleref{MIAOU}{domaine}{}
      logici\moduleref{MIAOU}{logiciels}{}
      resu \moduleref{MIAOU}{resultats}{}
      resu \moduleref{MIAOU}{contrats}{}
      resu \moduleref{MIAOU}{international}{}
      resu \moduleref{MIAOU}{diffusion}{}
      logi-tra\moduleref{MIAOU}{logiciels}{tralics}
      \fi
      1=\ref{crit}, 2=\ref{xml-route}, 3=\moduleref{MIAOU}{fondements}{} 4=\moduleref{MIAOU}{fondements}{identif} 5=\ref{filtrescnes}

      In addition to the ``Scientific advisors'' and to the ``Visiting scientists''
      listed in section \moduleref{MIAOU}{composition}{}, 
      the following scientists visited us in 2003. 
      \begin{itemize}
      \item Mohamed Jaoua (Lamsin-ENIT, Tunis).
      \item Herbert Stahl (TU Berlin).
      \item \etc
      \end{itemize}
      \def\RAmysection{diffusion}
      \section{\choseRAsection{9}}
      \RAlabel{MIAOU@\RAmysection@}

      \RAstartmodule{0}{MIAOU}{9}{dif-ens}{Teaching}
      \RAlabel{MIAOU@diffusion@dif-ens}


      \begin{description}
      \item [Courses] \ 
        \begin{itemize}
        \item D. Avanessoff \etc
        \item L. Baratchart, \etc
        \item J. Leblond \etc
        \end{itemize}
        
      \item [Trainees] \ 
        \begin{itemize}
        \item Antoine Chaillet, \etc
        \end{itemize}

      \item[Ph.D. Students] \ 
        \begin{itemize}
        \item David Avanessoff, �~Lin�arisation \etc~� 
         (dynamic linearization \etc)
        \item Fehmi Ben Hassen, <<~Localisation \etc~>>, 
        \item Alex Bombrun, \etc
      \end{itemize}
      \item[Ph.D. thesis defended] \ 
      \begin{itemize}
      \item Reinhold K�stner, \etc
      \end{itemize}
      \end{description}

      L. Baratchart was (president|rapporteur|examinateur)\footnote{Rayer les
        mentions inutiles}
       of the Thesis  of X and Y
      and Z\footnote{Remplacer les lettres par des noms}.

      \RAstartmodule{0}{MIAOU}{9}{dif-anim}{Community service}
      \RAlabel{MIAOU@diffusion@dif-anim}


      L. Baratchart is a member of the  ``bureau'' of the CP
      (Comit� des Projets) of INRIA-Sophia Antipolis.
      \RAstartmodule{0}{MIAOU}{9}{dif-conf}{Conferences and workshops}
      \RAlabel{MIAOU@diffusion@dif-conf}

      \begin{glossaire}\glo{A}{B\par C}\glo{A1}{B1\par C1}\end{glossaire}

      Talks, courses, sessions, software demonstrations at the
      CNRS-INRIA summer school ``Harmonic analysis and rational approximation: their
      r\^oles in signals, control and dynamical systems theory'',
      Porquerolles, september. 
      \htmladdnormallink{\url{http://www-sop.inria.fr/miaou/anap03/index.en.html}}
      {http://www-sop.inria.fr/miaou/anap03/index.en.html}

      J. Grimm gave a talk about Tralics at Eurotex 2003 (Brest)
      \section{\choseRAsection{10}}
      \xloadbiblio
      \bibliography{miaou2003}
      \end{document}
