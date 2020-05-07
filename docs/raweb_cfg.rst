Parts of the raweb-cfg file
===========================

Ampersand hacks
~~~~~~~~~~~~~~~

The intend of this code is for insertion of & in an URL. Only the x260A
line is used.

.. container:: ltx-source

   ::

      \expandafter\def\csname grimmentitiesx26\endcsname{\string&}
      \expandafter\def\csname grimmentities38\endcsname{\string&}
      \expandafter\def\csname grimmentities9738\endcsname{\string&}
      \expandafter\def\csname grimmentitiesx260A\endcsname{\string&}
      \expandafter\def\csname grimmentities amp\endcsname{\string&}

Commands added for efficiency.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\utfeightaz@jg@int{\noexpand\utfeightaz\noexpand}
      \def\utfeightb@jg@int#1#2{\noexpand\utfeightb#1\string#2}
      \def\utfeightc@jg@int#1#2#3{\noexpand\utfeightc#1\string#2\string#3}
      \def\utfeightd@jg@int#1#2#3#4{\noexpand\utfeightd#1\string#2\string#3\string#4}
      \def\utfeightb@jg@#1#2{#1\string#2}
      \def\utfeightc@jg@#1#2#3{#1\string#2\string#3}
      \def\utfeightd@jg@#1#2#3#4{#1\string#2\string#3\string#4}
      \def\utfeightaz@jg@int{\noexpand\utfeightaz\noexpand}
      \def\utfeightb@jg@int#1#2{\noexpand\utfeightb#1\string#2}
      \def\utfeightc@jg@int#1#2#3{\noexpand\utfeightc#1\string#2\string#3}
      \def\utfeightd@jg@int#1#2#3#4{\noexpand\utfeightd#1\string#2\string#3\string#4}

Commands modified for efficiency.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

We use ``\let\foo\bar`` instead of ``\def\foo{...}``, because it is more
efficient.

.. container:: ltx-source

   ::

      \def\utfeight@protect@chars{%
        \let\utfeightax\string
        \let\utfeightay\string
        \let\utfeightaz\string
        \let\utfeightb\utfeightb@jg@
        \let\utfeightc\utfeightc@jg@
        \let\utfeightd\utfeightd@jg@}

We are here in a group where ampersand and less-than are active chars.

.. container:: ltx-source

   ::

      \gdef\utfeight@protect@internal{%
        \let\utfeightax\noexpand
        \let\utfeightay\noexpand
        \let\utfeightaz\utfeightaz@jg@int
        \let<\relax\let&\relax
        \let\utfeightb\utfeightb@jg@int
        \let\utfeightc\utfeightc@jg@int
        \let\utfeightd\utfeightd@jg@int}

String declarations (of the form jg@xxx)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \XMLstring\jg@lt<>&lt;</>
      \XMLstring\jg@gt<>&gt;</>
      \XMLstring\jg@accgrave<>`</>
      \XMLstring\jg@accdddot<>&#x20DB;</>
      \XMLstring\jg@accddddot<>&#x20DC;</>
      \XMLstring\jg@lbra<>&#x2329;</>
      \XMLstring\jg@rbra<>&#x232A;</>
      \XMLstring\jg@verbar<>&#x2016;</>
      \XMLstring\jg@Verbar<>&#8214;</>
      \XMLstring\jg@lfloor<>&#x230A;</>
      \XMLstring\jg@rfloor<>&#x230B;</>
      \XMLstring\jg@lceil<>&#x2308;</>
      \XMLstring\jg@rceil<>&#x2309;</>
      \XMLstring\jg@lmoustache<>&#x23B0;</>
      \XMLstring\jg@rmoustache<>&#x23B1;</>
      \XMLstring\jg@lgroup<>&#x3014;</>
      \XMLstring\jg@rgroup<>&#x3015;</>
      \XMLstring\jg@uparrow<>&#x2191;</>
      \XMLstring\jg@Uparrow<>&#x21D1;</>
      \XMLstring\jg@downarrow<>&#x2193;</>
      \XMLstring\jg@Downarrow<>&#x21D3;</>
      \XMLstring\jg@updownarrow<>&#x2195;</>
      \XMLstring\jg@Updownarrow<>&#x21D5;</>
      \XMLstring\jg@OverBrace<>&#xFE37;</>
      \XMLstring\jg@UnderBrace<>&#xFE38;</>
      \XMLstring\jg@OverBar<>&#xAF;</>
      \XMLstring\jg@UnderBAr<>&#x332;</>

String declarations (of the form att@xxx)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \XMLstring\att@true<>true</>
      \XMLstring\att@false<>false</>
      \XMLstring\att@mtd@left<>left</>
      \XMLstring\att@mtd@right<>right</>
      \XMLstring\att@mtd@center<>center</>
      \XMLstring\att@none<>none</>
      \XMLstring\att@dzero<>0</>
      \XMLstring\att@done<>1</>
      \XMLstring\att@dtwo<>2</>
      \XMLstring\att@l<>l</>
      \XMLstring\att@r<>r</>
      \XMLstring\att@t<>t</>
      \XMLstring\att@b<>b</>
      \XMLstring\att@lt<>lt</>
      \XMLstring\att@lb<>lb</>
      \XMLstring\att@rt<>rt</>
      \XMLstring\att@rb<>rb</>
      \XMLstring\att@tl<>tl</>
      \XMLstring\att@bl<>bl</>
      \XMLstring\att@tr<>tr</>
      \XMLstring\att@br<>br</>
      \XMLstring\att@mathml@bold<>bold</>
      \XMLstring\att@mathml@sansserif<>sans-serif</>
      \XMLstring\att@mathml@tt<>monospace</>
      \XMLstring\att@mathml@rm<>rm</>
      \XMLstring\att@BLOCK<>block</>
      \XMLstring\att@PREFIX<>prefix</>
      \XMLstring\att@EQUATION<>equation</>

Hacking function for accent-over
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\jg@bindings{%
         \def\texttildelow {\global\let\jg@cur@acc\jg@tilde@acc}%
      %   \def\textasciimacron {\global\let\jg@cur@acc\jg@cur@accB}%
         \def\textasciicaron {\global\let\jg@cur@acc\jg@check@acc}%
         \def\u{\global\let\jg@cur@acc\jg@breve@acc}%
         \def\hat##1{\global\let\jg@cur@acc\jg@hat@acc}%
         \def\dot##1{\global\let\jg@cur@acc\jg@dot@acc}%
         \def\mathring##1{\global\let\jg@cur@acc\jg@ring@acc}%
         \def\textasciidieresis{\global\let\jg@cur@acc\jg@ddot@acc}%
         \let\JGG@orig@rarrow\rightarrow
         \let\JGG@orig@larrow\leftarrow
         \def\rightarrow{\global\let\jg@cur@acc\jg@overRarrow@acc}%
         \def\leftarrow{\global\let\jg@cur@acc\jg@overLarrow@acc}%
         \def\textoverbrace{\global\let\jg@cur@acc\overbrace}
         \def\textunderbrace{\global\let\jg@cur@acc\underbrace}
         \def\textasciiacute{\global\let\jg@cur@acc\acute}
         \def\textasciimacron{\global\let\jg@cur@acc\overline}
         \def\ring{\global\let\jg@cur@acc\mathring}
      }

Hacking function for accent-under
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\jg@ubindings{%
      \let\JGG@orig@rarrow\rightarrow
      \let\JGG@orig@larrow\leftarrow
         \def\texttildelow {\global\let\jg@cur@acc\jg@tilde@acc}%
         \def\textasciimacron {\global\let\jg@cur@acc\underline}%
         \def\textasciicaron {\global\let\jg@cur@acc\jg@check@acc}%
         \def\u{\global\let\jg@cur@acc\jg@breve@acc}%
         \def\hat##1{\global\let\jg@cur@acc\jg@hat@acc}%
         \def\dot##1{\global\let\jg@cur@acc\jg@dot@acc}%
         \def\textasciidieresis{\global\let\jg@cur@acc\jg@ddot@acc}%
         \let\JGG@orig@rarrow\rightarrow
         \let\JGG@orig@larrow\leftarrow
         \def\rightarrow{\global\let\jg@cur@acc\jg@underRarrow@acc}%
         \def\leftarrow{\global\let\jg@cur@acc\jg@underLarrow@acc}%
         \def\textoverbrace{\global\let\jg@cur@acc\overbrace}
         \def\textunderbrace{\global\let\jg@cur@acc\underbrace}
         \def\jgunderline{\global\let\jg@cur@acc\underline}
      }

Unused implementatin of frac.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\s@frac#1#2{\frac{\scriptstyle#1}{\scriptstyle#2}}
      \def\ss@frac#1#2{\frac{\scriptscriptstyle#1}{\scriptscriptstyle#2}}

      \def\unusedxml@implement@frac#1#2{%
       \ifx\XML@scriptlevel\att@none 
        \ifx\XML@linethickness\att@true\frac{#1}{#2}%
        \else \genfrac{}{}\XML@linethickness{}{#1}{#2}\fi
       \else
          \ifx\XML@scriptlevel\att@dzero
            \ifx\XML@linethickness\att@true\dfrac{#1}{#2}%
            \else \genfrac{}{}\XML@linethickness{0}{#1}{#2}\fi
          \else
            \ifx\XML@scriptlevel\att@done
               \ifx\XML@linethickness\att@true\tfrac{#1}{#2}%
               \else \genfrac{}{}\XML@linethickness{1}{#1}{#2}\fi
            \else
             \ifx\XML@linethickness\att@true\ss@frac{#1}{#2}%
             \else \genfrac{}{}\XML@linethickness{2}{#1}{#2}\fi
            \fi
          \fi
       \fi}

hack \\item
~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\jg@item[#1]{%
        \if@noparitem
          \@donoparitem
        \else
          \if@inlabel
            \indent \par
          \fi
          \ifhmode
            \unskip\unskip \par
          \fi
          \if@newlist
      % removed this test 2000/11/05. always put in topsep.
      %      \if@nobreak
      %        \@nbitem
      %      \else
              \addpenalty\@beginparpenalty
              \addvspace\@topsep
              \addvspace{-\parskip}%
      %      \fi
          \else
            \addpenalty\@itempenalty
            \addvspace\itemsep
          \fi
          \global\@inlabeltrue
        \fi
        \global\everypar{%
          \@minipagefalse
          \global\@newlistfalse
          \if@inlabel
            \global\@inlabelfalse
            {\setbox\z@\lastbox
             \ifvoid\z@
               \kern-\itemindent
             \fi}%
            \box\@labels
            \penalty\z@
          \fi
          \if@nobreak
            \global\@nobreakfalse
            \clubpenalty \@M
          \else
            \clubpenalty \@clubpenalty
            \global\everypar{}%
          \fi}%
        \if@noitemarg
          \@noitemargfalse
          \if@nmbrlist
            \refstepcounter\@listctr
          \fi
        \fi
       \savebox{\ItemBox}{#1}% Added JG
        \sbox\@tempboxa{\makelabel{#1}}%
        \global\setbox\@labels\hbox{%
          \unhbox\@labels
          \hskip \itemindent
          \hskip -\labelwidth
          \hskip -\labelsep
          \ifdim \wd\@tempboxa >\labelwidth
            \box\@tempboxa
          \else
            \hbox to\labelwidth {\unhbox\@tempboxa}%
          \fi
          \hskip \labelsep}%
        \ignorespaces}

hack \\FONormalBlock
~~~~~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \AtBeginDocument{%
      \def\FONormalBlock{%
        \let\@x\relax
        \ifnum\FOTableNesting>0
         \ifx\FOtextalign\att@centered
             \centering
         \fi
         \let\@x\relax
         \FOSetFont{normalblock}%
         \vrule height \f@baselineskip depth \z@ width \z@ 
        \else
          \ifnum\FOinList>0
            \FOlabel
            \ifFOListInnerPar\par\FOvspacebefore\fi
            \ifx\FOwhitespace\att@pre\obeyspaces\obeylines\fi
            \ifx\FOwhitespacecollapse\att@false\obeyspaces\fi
            \ifx\FOwrapoption\att@nowrap\obeylines\fi
          \else
            \ifx\FObreakbefore\att@page
              \let\tempID\FOid
              \penalty -\@M
              \let\FOid\tempID
            \else
              \ifx\FObreakbefore\att@oddpage
              \let\tempID\FOid
              \penalty -\@M
              \ifodd\c@page\else\BlankPage\newpage\fi
              \let\FOid\tempID
              \fi
            \fi
            \par
             %      \FOlabel (deleted) 
            \Quadding
            \ifFOBlockGrab
              \FOBoxedBlock{\linewidth}%
            \else
              \FOBorderTop
              \ifdim\FOpaddingbefore>\z@
               \vskip\FOpaddingbefore
              \fi
              \FOvspacebefore
              \parindent\FOtextindent
              \advance\leftskip by  \FOpaddingstart
              \advance\leftskip by  \FOmarginleft
              \advance\rightskip by \FOpaddingend
              \advance\rightskip by \FOmarginright
            \fi
      %      \par
            \ifx\FOwhitespace\att@pre\obeyspaces\obeylines\fi
            \ifx\FOwhitespacecollapse\att@false\obeyspaces\fi
            \ifx\FOwrapoption\att@nowrap\obeylines\fi
          \fi
            \FOlabel %moved this [jg] 
          \FOSetFont{normal}%
        \fi
      \@x
      }
      }

hack open fence
~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\jg@hacko{%
        \ifx\XML@fenceopen\jg@lt\let\XML@fenceopen\langle\fi
        \ifx\XML@fenceopen\jg@lbra\let\XML@fenceopen\langle\fi
        \ifx\XML@fenceopen\jg@gt\let\XML@fenceopen\rangle\fi
        \ifx\XML@fenceopen\jg@rbra\let\XML@fenceopen\rangle\fi
        \ifx\XML@fenceopen\jg@verbar\let\XML@fenceopen\|\fi
        \ifx\XML@fenceopen\jg@Verbar\let\XML@fenceopen\|\fi
        \ifx\XML@fenceopen\jg@lfloor\let\XML@fenceopen\lfloor\fi
        \ifx\XML@fenceopen\jg@rfloor\let\XML@fenceopen\rfloor\fi
        \ifx\XML@fenceopen\jg@lceil\let\XML@fenceopen\lceil\fi
        \ifx\XML@fenceopen\jg@rceil\let\XML@fenceopen\rceil\fi
        \ifx\XML@fenceopen\jg@lmoustache\let\XML@fenceopen\lmoustache\fi
        \ifx\XML@fenceopen\jg@rmoustache\let\XML@fenceopen\rmoustache\fi
        \ifx\XML@fenceopen\jg@lgroup\let\XML@fenceopen\lgroup\fi
        \ifx\XML@fenceopen\jg@rgroup\let\XML@fenceopen\rgroup\fi
        \ifx\XML@fenceopen\jg@uparrow\let\XML@fenceopen\uparrow\fi
        \ifx\XML@fenceopen\jg@downarrow\let\XML@fenceopen\downarrow\fi
        \ifx\XML@fenceopen\jg@Uparrow\let\XML@fenceopen\Uparrow\fi
        \ifx\XML@fenceopen\jg@Downarrow\let\XML@fenceopen\Downarrow\fi
        \ifx\XML@fenceopen\jg@updownarrow\let\XML@fenceopen\updownarrow\fi
        \ifx\XML@fenceopen\jg@Updownarrow\let\XML@fenceopen\Updownarrow\fi
      }

hack close fence
~~~~~~~~~~~~~~~~

.. container:: ltx-source

   ::

      \def\jg@hackc{%
        \ifx\XML@fenceclose\jg@gt\let\XML@fenceclose\rangle\fi
        \ifx\XML@fenceclose\jg@rbra\let\XML@fenceclose\rangle\fi
        \ifx\XML@fenceclose\jg@lt\let\XML@fenceclose\langle\fi
        \ifx\XML@fenceclose\jg@lbra\let\XML@fenceclose\langle\fi
        \ifx\XML@fenceclose\jg@verbar\let\XML@fenceclose\|\fi
        \ifx\XML@fenceclose\jg@Verbar\let\XML@fenceclose\|\fi
        \ifx\XML@fenceclose\jg@lfloor\let\XML@fenceclose\lfloor\fi
        \ifx\XML@fenceclose\jg@rfloor\let\XML@fenceclose\rfloor\fi
        \ifx\XML@fenceclose\jg@lceil\let\XML@fenceclose\lceil\fi
        \ifx\XML@fenceclose\jg@rceil\let\XML@fenceclose\rceil\fi
        \ifx\XML@fenceclose\jg@lmoustache\let\XML@fenceclose\lmoustache\fi
        \ifx\XML@fenceclose\jg@rmoustache\let\XML@fenceclose\rmoustache\fi
        \ifx\XML@fenceclose\jg@lgroup\let\XML@fenceclose\lgroup\fi
        \ifx\XML@fenceclose\jg@rgroup\let\XML@fenceclose\rgroup\fi
        \ifx\XML@fenceclose\jg@uparrow\let\XML@fenceclose\uparrow\fi
        \ifx\XML@fenceclose\jg@downarrow\let\XML@fenceclose\downarrow\fi
        \ifx\XML@fenceclose\jg@Uparrow\let\XML@fenceclose\Uparrow\fi
        \ifx\XML@fenceclose\jg@Downarrow\let\XML@fenceclose\Downarrow\fi
        \ifx\XML@fenceclose\jg@updownarrow\let\XML@fenceclose\updownarrow\fi
        \ifx\XML@fenceclose\jg@Updownarrow\let\XML@fenceclose\Updownarrow\fi
      }

All chars
~~~~~~~~~

.. container:: ltx-source

   ::

      \def\pxrfont{\fontencoding{TS1}\fontfamily{pxr}\selectfont}
      \def\cyrfont{\fontencoding{T2A}\fontfamily{cmr}\selectfont}
      \UnicodeCharacter{x1F4}{\ifmmode \acute{G}\else \'{G}\fi}
      \UnicodeCharacter{x1CD}{\ifmmode \check{A}\else \v{A}\fi}
      \UnicodeCharacter{x1CE}{\ifmmode \check{a}\else \v{a}\fi}
      \UnicodeCharacter{x1CF}{\ifmmode \check{I}\else \v{I}\fi}
      \UnicodeCharacter{x1D0}{\ifmmode \check{\imath}\else \v{\i}\fi}
      \UnicodeCharacter{x1D1}{\ifmmode \check{O}\else \v{O}\fi}
      \UnicodeCharacter{x1D2}{\ifmmode \check{o}\else \v{o}\fi}
      \UnicodeCharacter{x1D3}{\ifmmode \check{U}\else \v{U}\fi}
      \UnicodeCharacter{x1D4}{\ifmmode \check{u}\else \v{u}\fi}
      \UnicodeCharacter{x1D5}{\ifmmode \mbox{\={\"{U}}}\else \={\"{U}}\fi}
      \UnicodeCharacter{x1D6}{\ifmmode \mbox{\={\"{u}}}\else \={\"u}\fi}
      \UnicodeCharacter{x1D7}{\ifmmode \mbox{\'{\"{U}}}\else \'{\"{U}}\fi}
      \UnicodeCharacter{x1D8}{\ifmmode \mbox{\'{\"{u}}}\else \'{\"u}\fi}
      \UnicodeCharacter{x1D9}{\ifmmode \mbox{\v{\"{U}}}\else \v{\"{U}}\fi}
      \UnicodeCharacter{x1DA}{\ifmmode \mbox{\v{\"{u}}}\else \v{\"u}\fi}
      \UnicodeCharacter{x1DB}{\ifmmode \mbox{\`{\"{U}}}\else \`{\"{U}}\fi}
      \UnicodeCharacter{x1DC}{\ifmmode \mbox{\`{\"{u}}}\else \`{\"u}\fi}
      \UnicodeCharacter{x1DE}{\ifmmode \mbox{\={\"{A}}}\else \={\"A}\fi}
      \UnicodeCharacter{x1DF}{\ifmmode \mbox{\={\"{a}}}\else \={\"a}\fi}
      \UnicodeCharacter{x1E0}{\ensuremath{\bar{\dot{\mbox{A}}}}}
      \UnicodeCharacter{x1E1}{\ensuremath{\bar{\dot{\mbox{a}}}}}
      \UnicodeCharacter{x1E2}{\ifmmode \mbox{\=\AE}\else \={\AE}\fi}
      \UnicodeCharacter{x1E3}{\ifmmode \mbox{\=\ae}\else \={\ae}\fi}
      \UnicodeCharacter{x1E6}{\ifmmode \check{G}\else \v{G}\fi}
      \UnicodeCharacter{x1E7}{\ifmmode \check{g}\else \v{g}\fi}
      \UnicodeCharacter{x1E8}{\ifmmode \check{K}\else \v{K}\fi}
      \UnicodeCharacter{x1E9}{\ifmmode \check{k}\else \v{k}\fi}
      \UnicodeCharacter{x1EA}{\ifmmode \k{O}\else \k{O}\fi}
      \UnicodeCharacter{x1EB}{\ifmmode \k{o}\else \k{o}\fi}
      \UnicodeCharacter{x1EC}{\ifmmode \bar{\k{O}}\else $\bar{\mbox{\k{O}}}$\fi}
      \UnicodeCharacter{x1ED}{\ifmmode \bar{\k{o}}\else $\bar{\mbox{\k{o}}}$\fi}
      \UnicodeCharacter{x1F0}{\ifmmode \check{j}\else \v{j}\fi}
      \UnicodeCharacter{x1F8}{\`{N}}
      \UnicodeCharacter{x1F9}{\`{n}}
      \UnicodeCharacter{x1FA}{\'{\AA}}
      \UnicodeCharacter{x1FB}{\'{\aa}}
      \UnicodeCharacter{x1FC}{\'{\AE}}
      \UnicodeCharacter{x1FD}{\'{\ae}}
      \UnicodeCharacter{x1FE}{\'{\O}}
      \UnicodeCharacter{x1FF}{\'{\o}}

      \UnicodeCharacter{x200}{{\cyrfont\C A}}
      \UnicodeCharacter{x201}{{\cyrfont\C a}}
      \UnicodeCharacter{x202}{{\cyrfont\f A}}
      \UnicodeCharacter{x203}{{\cyrfont\f a}}
      \UnicodeCharacter{x204}{{\cyrfont\C E}}
      \UnicodeCharacter{x205}{{\cyrfont\C e}}
      \UnicodeCharacter{x206}{{\cyrfont\f E}}
      \UnicodeCharacter{x207}{{\cyrfont\f e}}
      \UnicodeCharacter{x208}{{\cyrfont\C I}}
      \UnicodeCharacter{x209}{{\cyrfont\C \i}}
      \UnicodeCharacter{x20A}{{\cyrfont\f I}}
      \UnicodeCharacter{x20B}{{\cyrfont\f \i}}
      \UnicodeCharacter{x20C}{{\cyrfont\C O}}
      \UnicodeCharacter{x20D}{{\cyrfont\C o}}
      \UnicodeCharacter{x20E}{{\cyrfont\f O}}
      \UnicodeCharacter{x20F}{{\cyrfont\f o}}
      \UnicodeCharacter{x210}{{\cyrfont\C R}}
      \UnicodeCharacter{x211}{{\cyrfont\C r}}
      \UnicodeCharacter{x212}{{\cyrfont\f R}}
      \UnicodeCharacter{x213}{{\cyrfont\f r}}
      \UnicodeCharacter{x214}{{\cyrfont\C U}}
      \UnicodeCharacter{x215}{{\cyrfont\C u}}
      \UnicodeCharacter{x216}{{\cyrfont\f U}}
      \UnicodeCharacter{x217}{{\cyrfont\f u}}
      \UnicodeCharacter{x21E}{\ifmmode \check{K}\else \v{H}\fi}
      \UnicodeCharacter{x21F}{\ifmmode \check{k}\else \v{h}\fi}
      \UnicodeCharacter{x226}{\.A}
      \UnicodeCharacter{x227}{\.a}
      \UnicodeCharacter{x228}{\c E}
      \UnicodeCharacter{x229}{\c e}
      \UnicodeCharacter{x22a}{\={\"O}}
      \UnicodeCharacter{x22b}{\={\"o}}
      \UnicodeCharacter{x22c}{\={\~O}}
      \UnicodeCharacter{x22d}{\={\~o}}
      \UnicodeCharacter{x22e}{\.O}
      \UnicodeCharacter{x22f}{\.o}
      \UnicodeCharacter{x230}{\ensuremath{\bar{\mbox{\.O}}}}
      \UnicodeCharacter{x231}{\ensuremath{\bar{\mbox{\.o}}}}
      \UnicodeCharacter{x232}{\=Y}
      \UnicodeCharacter{x233}{\=y}

      %\UnicodeCharacter{x25A1}{\ensuremath{\Box}}

      \def\CAB#1{\leavevmode\setbox0\hbox{#1}\dimen@\ht0 \advance\dimen@.5ex
      \rlap{\raise\dimen@\hbox{\kern2pt\char11}}#1}
      \def\Ohorn{\mbox{\rlap{\raise1.3ex\hbox{\kern0.55em,}}O}}
      \def\Uhorn{\mbox{\rlap{\raise1.7ex\hbox{\kern0.55em,}}U}}
      \def\ohorn{\mbox{\rlap{\raise1.ex\hbox{\kern0.35em,}}o}}
      \def\uhorn{\mbox{\rlap{\raise1.2ex\hbox{\kern0.33em,}}u}}

      \def\circlebelow#1{\hmode@bgroup \o@lign 
      {\relax #1\crcr \hidewidth \sh@ft {10}\char 6 \hidewidth }\egroup}
      \def\cirbelow#1{\hmode@bgroup \o@lign 
      {\relax #1\crcr \hidewidth \sh@ft {10}\char 2 \hidewidth }\egroup}
      \def\tildebelow#1{\hmode@bgroup \o@lign 
      {\relax #1\crcr \hidewidth \sh@ft {10}\char 3 \hidewidth }\egroup}
      \def\somethingbelow#1#2{\hmode@bgroup \o@lign 
      {\relax #1\crcr \hidewidth \sh@ft {10}\char #2 \hidewidth }\egroup}

      \UnicodeCharacter{x400}{\cyrchar{\`\CYRE}}
      \UnicodeCharacter{x40D}{\cyrchar{\`\CYRI}}
      \UnicodeCharacter{x450}{\cyrchar{\`\cyre}}
      \UnicodeCharacter{x45D}{\cyrchar{\`\cyri}}
      \UnicodeCharacter{x4D0}{\cyrchar{\U\CYRA}}
      \UnicodeCharacter{x4D1}{\cyrchar{\U\cyra}}
      \UnicodeCharacter{x4D2}{\cyrchar{\"\CYRA}}
      \UnicodeCharacter{x4D3}{\cyrchar{\"\cyra}}
      \UnicodeCharacter{x4D6}{\cyrchar{\U\CYRE}}
      \UnicodeCharacter{x4D7}{\cyrchar{\U\cyre}}
      \UnicodeCharacter{x4DA}{\cyrchar{\"\CYRSCHWA}}
      \UnicodeCharacter{x4DB}{\cyrchar{\"\cyrschwa}}
      \UnicodeCharacter{x4DC}{\cyrchar{\"\CYRZH}}
      \UnicodeCharacter{x4DD}{\cyrchar{\"\cyrzh}}
      \UnicodeCharacter{x4DE}{\cyrchar{\"\CYRZ}}
      \UnicodeCharacter{x4DF}{\cyrchar{\"\cyrz}}
      \UnicodeCharacter{x4E2}{\cyrchar{\=\CYRI}}
      \UnicodeCharacter{x4E3}{\cyrchar{\=\cyri}}
      \UnicodeCharacter{x4E4}{\cyrchar{\"\CYRI}}
      \UnicodeCharacter{x4E5}{\cyrchar{\"\cyri}}
      \UnicodeCharacter{x4E6}{\cyrchar{\"\CYRO}}
      \UnicodeCharacter{x4E7}{\cyrchar{\"\cyro}}
      \UnicodeCharacter{x4EA}{\cyrchar{\"\CYROTLD}}
      \UnicodeCharacter{x4EB}{\cyrchar{\"\cyrotld}}
      \UnicodeCharacter{x4EC}{\cyrchar{\"\CYREREV}}
      \UnicodeCharacter{x4ED}{\cyrchar{\"\cyrerev}}
      \UnicodeCharacter{x4EE}{\cyrchar{\=\CYRU}}
      \UnicodeCharacter{x4EF}{\cyrchar{\=\cyru}}
      \UnicodeCharacter{x4F0}{\cyrchar{\"\CYRU}}
      \UnicodeCharacter{x4F1}{\cyrchar{\"\cyru}}
      \UnicodeCharacter{x4F2}{\cyrchar{\H\CYRU}}
      \UnicodeCharacter{x4F3}{\cyrchar{\H\cyru}}
      \UnicodeCharacter{x4F4}{\cyrchar{\"\CYRCH}}
      \UnicodeCharacter{x4F5}{\cyrchar{\"\cyrch}}
      \UnicodeCharacter{x4F8}{\cyrchar{\"\CYRERY}}
      \UnicodeCharacter{x4F9}{\cyrchar{\"\cyrery}}


      \UnicodeCharacter{x1E00}{\circlebelow{A}}
      \UnicodeCharacter{x1E01}{\circlebelow{a}}
      \UnicodeCharacter{x1E02}{\.B}
      \UnicodeCharacter{x1E03}{\.b}
      \UnicodeCharacter{x1E04}{\d B}
      \UnicodeCharacter{x1E05}{\d b}
      \UnicodeCharacter{x1E06}{\b B}
      \UnicodeCharacter{x1E07}{\b b}
      \UnicodeCharacter{x1E08}{\'{\c C}}
      \UnicodeCharacter{x1E09}{\'{\c c}}
      \UnicodeCharacter{x1E0A}{\.D}
      \UnicodeCharacter{x1E0B}{\.d}
      \UnicodeCharacter{x1E0C}{\d D}
      \UnicodeCharacter{x1E0D}{\d d}
      \UnicodeCharacter{x1E0E}{\b D}
      \UnicodeCharacter{x1E0F}{\b d}
      \UnicodeCharacter{x1E10}{\c D}
      \UnicodeCharacter{x1E11}{\c d}
      \UnicodeCharacter{x1E12}{\cirbelow D}
      \UnicodeCharacter{x1E13}{\cirbelow d}
      \UnicodeCharacter{x1E14}{\ensuremath{\grave{\mbox{\=E}}}}
      \UnicodeCharacter{x1E15}{\ensuremath{\grave{\mbox{\=e}}}}
      \UnicodeCharacter{x1E16}{\ensuremath{\acute{\mbox{\=E}}}}
      \UnicodeCharacter{x1E17}{\ensuremath{\acute{\mbox{\=e}}}}
      \UnicodeCharacter{x1E18}{\cirbelow E}
      \UnicodeCharacter{x1E19}{\cirbelow e}
      \UnicodeCharacter{x1E1A}{\tildebelow E}
      \UnicodeCharacter{x1E1B}{\tildebelow e}
      \UnicodeCharacter{x1E1C}{\c{\u E}}
      \UnicodeCharacter{x1E1D}{\c{\u e}}
      \UnicodeCharacter{x1E1E}{\.F}
      \UnicodeCharacter{x1E1F}{\.f}
      \UnicodeCharacter{x1E20}{\=G}
      \UnicodeCharacter{x1E21}{\=g}
      \UnicodeCharacter{x1E22}{\.H}
      \UnicodeCharacter{x1E23}{\.h}
      \UnicodeCharacter{x1E24}{\d H}
      \UnicodeCharacter{x1E25}{\d h}
      \UnicodeCharacter{x1E26}{\" H}
      \UnicodeCharacter{x1E27}{\" h}
      \UnicodeCharacter{x1E28}{\c H}
      \UnicodeCharacter{x1E29}{\c h}
      \UnicodeCharacter{x1E2A}{\somethingbelow H{8}}
      \UnicodeCharacter{x1E2B}{\somethingbelow h{8}}
      \UnicodeCharacter{x1E2C}{\tildebelow I}
      \UnicodeCharacter{x1E2D}{\tildebelow i}
      \UnicodeCharacter{x1E2E}{\'{\"I}}
      \UnicodeCharacter{x1E2F}{\'{\"\i}}
      \UnicodeCharacter{x1E30}{\'K}
      \UnicodeCharacter{x1E31}{\'k}
      \UnicodeCharacter{x1E32}{\d K}
      \UnicodeCharacter{x1E33}{\d k}
      \UnicodeCharacter{x1E34}{\b K}
      \UnicodeCharacter{x1E35}{\b k}
      \UnicodeCharacter{x1E36}{\d L}
      \UnicodeCharacter{x1E37}{\d l}
      \UnicodeCharacter{x1E38}{\d {\=L}}
      \UnicodeCharacter{x1E39}{\d {\=l}}
      \UnicodeCharacter{x1E3A}{\b L}
      \UnicodeCharacter{x1E3B}{\b l}
      \UnicodeCharacter{x1E3C}{\cirbelow L}
      \UnicodeCharacter{x1E3D}{\cirbelow l}
      \UnicodeCharacter{x1E3E}{\'M}
      \UnicodeCharacter{x1E3F}{\'m}
      \UnicodeCharacter{x1E40}{\.M}
      \UnicodeCharacter{x1E41}{\.m}
      \UnicodeCharacter{x1E42}{\d M}
      \UnicodeCharacter{x1E43}{\d m}
      \UnicodeCharacter{x1E44}{\.N}
      \UnicodeCharacter{x1E45}{\.n}
      \UnicodeCharacter{x1E46}{\d N}
      \UnicodeCharacter{x1E47}{\d n}
      \UnicodeCharacter{x1E48}{\b N}
      \UnicodeCharacter{x1E49}{\b n}
      \UnicodeCharacter{x1E4A}{\cirbelow N}
      \UnicodeCharacter{x1E4B}{\cirbelow n}
      \UnicodeCharacter{x1E4C}{\'{\~O}}
      \UnicodeCharacter{x1E4D}{\'{\~o}}
      \UnicodeCharacter{x1E4E}{\"{\~O}}
      \UnicodeCharacter{x1E4F}{\"{\~o}}
      \UnicodeCharacter{x1E50}{\ensuremath{\grave{\mbox{\=O}}}}
      \UnicodeCharacter{x1E51}{\ensuremath{\grave{\mbox{\=o}}}}
      \UnicodeCharacter{x1E52}{\ensuremath{\acute{\mbox{\=O}}}}
      \UnicodeCharacter{x1E53}{\ensuremath{\acute{\mbox{\=o}}}}
      \UnicodeCharacter{x1E54}{\'P}
      \UnicodeCharacter{x1E55}{\'p}
      \UnicodeCharacter{x1E56}{\.P}
      \UnicodeCharacter{x1E57}{\.p}
      \UnicodeCharacter{x1E58}{\.R}
      \UnicodeCharacter{x1E59}{\.r}
      \UnicodeCharacter{x1E5A}{\d R}
      \UnicodeCharacter{x1E5B}{\d r}
      \UnicodeCharacter{x1E5C}{\d{\= R}}
      \UnicodeCharacter{x1E5D}{\d{\= r}}
      \UnicodeCharacter{x1E5E}{\b R}
      \UnicodeCharacter{x1E5F}{\b r}
      \UnicodeCharacter{x1E60}{\.S}
      \UnicodeCharacter{x1E61}{\.s}
      \UnicodeCharacter{x1E62}{\d S}
      \UnicodeCharacter{x1E63}{\d s}
      \UnicodeCharacter{x1E64}{\.{\'S}}
      \UnicodeCharacter{x1E65}{\.{\'s}}
      \UnicodeCharacter{x1E66}{\.{\v S}}
      \UnicodeCharacter{x1E67}{\.{\v s}}
      \UnicodeCharacter{x1E68}{\d{\. S}}
      \UnicodeCharacter{x1E69}{\d{\. s}}
      \UnicodeCharacter{x1E6A}{\.T}
      \UnicodeCharacter{x1E6B}{\.t}
      \UnicodeCharacter{x1E6C}{\d T}
      \UnicodeCharacter{x1E6D}{\d t}
      \UnicodeCharacter{x1E6E}{\b T}
      \UnicodeCharacter{x1E6F}{\b t}
      \UnicodeCharacter{x1E70}{\cirbelow T}
      \UnicodeCharacter{x1E71}{\cirbelow t}
      \UnicodeCharacter{x1E72}{\somethingbelow U{4}}
      \UnicodeCharacter{x1E73}{\somethingbelow u{4}}
      \UnicodeCharacter{x1E74}{\tildebelow U}
      \UnicodeCharacter{x1E75}{\tildebelow u}
      \UnicodeCharacter{x1E76}{\cirbelow U}
      \UnicodeCharacter{x1E77}{\cirbelow u}
      \UnicodeCharacter{x1E78}{\ensuremath{\acute{\mbox{\~U}}}}
      \UnicodeCharacter{x1E79}{\ensuremath{\acute{\mbox{\~u}}}}
      \UnicodeCharacter{x1E7A}{\ensuremath{\ddot{\mbox{\=U}}}}
      \UnicodeCharacter{x1E7B}{\ensuremath{\ddot{\mbox{\=u}}}}
      \UnicodeCharacter{x1E7C}{\~ V}
      \UnicodeCharacter{x1E7D}{\~ v}
      \UnicodeCharacter{x1E7E}{\d V}
      \UnicodeCharacter{x1E7F}{\d v}
      \UnicodeCharacter{x1E80}{\`W}
      \UnicodeCharacter{x1E81}{\`w}
      \UnicodeCharacter{x1E82}{\'W}
      \UnicodeCharacter{x1E83}{\'w}
      \UnicodeCharacter{x1E84}{\"W}
      \UnicodeCharacter{x1E85}{\"w}
      \UnicodeCharacter{x1E86}{\.W}
      \UnicodeCharacter{x1E87}{\.w}
      \UnicodeCharacter{x1E88}{\d W}
      \UnicodeCharacter{x1E89}{\d w}
      \UnicodeCharacter{x1E8A}{\.X}
      \UnicodeCharacter{x1E8B}{\.x}
      \UnicodeCharacter{x1E8C}{\"X}
      \UnicodeCharacter{x1E8D}{\"x}
      \UnicodeCharacter{x1E8E}{\.Y}
      \UnicodeCharacter{x1E8F}{\.y}
      \UnicodeCharacter{x1E90}{\^Z}
      \UnicodeCharacter{x1E91}{\^z}
      \UnicodeCharacter{x1E92}{\d Z}
      \UnicodeCharacter{x1E93}{\d z}
      \UnicodeCharacter{x1E94}{\b Z}
      \UnicodeCharacter{x1E95}{\b z}
      \UnicodeCharacter{x1E96}{\b h}
      \UnicodeCharacter{x1E97}{\" t}
      \UnicodeCharacter{x1E98}{\r w}
      \UnicodeCharacter{x1E99}{\r y}
      \UnicodeCharacter{x1E9B}{\. {\fontencoding{LELA}\selectfont\char'173 }}
      \UnicodeCharacter{x1EA0}{\d A}
      \UnicodeCharacter{x1EA1}{\d a}
      \UnicodeCharacter{x1EA2}{\CAB A}
      \UnicodeCharacter{x1EA3}{\CAB a}
      \UnicodeCharacter{x1EA4}{\'{\^A}}
      \UnicodeCharacter{x1EA5}{\'{\^a}}
      \UnicodeCharacter{x1EA6}{\`{\^A}}
      \UnicodeCharacter{x1EA7}{\`{\^a}}
      \UnicodeCharacter{x1EA8}{\CAB{\^A}}
      \UnicodeCharacter{x1EA9}{\CAB{\^a}}
      \UnicodeCharacter{x1EAA}{\~{\^A}}
      \UnicodeCharacter{x1EAB}{\~{\^a}}
      \UnicodeCharacter{x1EAC}{\d{\^A}}
      \UnicodeCharacter{x1EAD}{\d{\^a}}
      \UnicodeCharacter{x1EAE}{\'{\u A}}
      \UnicodeCharacter{x1EAF}{\'{\u a}}
      \UnicodeCharacter{x1EB0}{\`{\u A}}
      \UnicodeCharacter{x1EB1}{\`{\u a}}
      \UnicodeCharacter{x1EB2}{\CAB{\u A}}
      \UnicodeCharacter{x1EB3}{\CAB{\u a}}
      \UnicodeCharacter{x1EB4}{\~{\u A}}
      \UnicodeCharacter{x1EB5}{\~{\u a}}
      \UnicodeCharacter{x1EB6}{\d{\u A}}
      \UnicodeCharacter{x1EB7}{\d{\u a}}
      \UnicodeCharacter{x1EB8}{\d E}
      \UnicodeCharacter{x1EB9}{\d e}
      \UnicodeCharacter{x1EBA}{\CAB E}
      \UnicodeCharacter{x1EBB}{\CAB e}
      \UnicodeCharacter{x1EBC}{\~E}
      \UnicodeCharacter{x1EBD}{\~e}
      \UnicodeCharacter{x1EBE}{\'{\^E}}
      \UnicodeCharacter{x1EBF}{\'{\^e}}
      \UnicodeCharacter{x1EC0}{\`{\^E}}
      \UnicodeCharacter{x1EC1}{\`{\^e}}
      \UnicodeCharacter{x1EC2}{\CAB{\^E}}
      \UnicodeCharacter{x1EC3}{\CAB{\^e}}
      \UnicodeCharacter{x1EC4}{\~{\^E}}
      \UnicodeCharacter{x1EC5}{\~{\^e}}
      \UnicodeCharacter{x1EC6}{\d {\^E}}
      \UnicodeCharacter{x1EC7}{\d {\^e}}
      \UnicodeCharacter{x1EC8}{\CAB I}
      \UnicodeCharacter{x1EC9}{\CAB \i}
      \UnicodeCharacter{x1ECA}{\d I}
      \UnicodeCharacter{x1ECB}{\d i}
      \UnicodeCharacter{x1ECC}{\d O}
      \UnicodeCharacter{x1ECD}{\d o}
      \UnicodeCharacter{x1ECE}{\CAB O}
      \UnicodeCharacter{x1ECF}{\CAB o}
      \UnicodeCharacter{x1ED0}{\'{\^O}}
      \UnicodeCharacter{x1ED1}{\'{\^o}}
      \UnicodeCharacter{x1ED2}{\`{\^O}}
      \UnicodeCharacter{x1ED3}{\`{\^o}}
      \UnicodeCharacter{x1ED4}{\CAB{\^O}}
      \UnicodeCharacter{x1ED5}{\CAB{\^o}}
      \UnicodeCharacter{x1ED6}{\~{\^O}}
      \UnicodeCharacter{x1ED7}{\~{\^o}}
      \UnicodeCharacter{x1ED8}{\d{\^O}}
      \UnicodeCharacter{x1ED9}{\d{\^o}}
      \UnicodeCharacter{x1EDA}{\ensuremath{\acute{\Ohorn}}}
      \UnicodeCharacter{x1EDB}{\ensuremath{\acute{\ohorn}}}
      \UnicodeCharacter{x1EDC}{\ensuremath{\grave{\Ohorn}}}
      \UnicodeCharacter{x1EDD}{\ensuremath{\grave{\ohorn}}}
      \UnicodeCharacter{x1EDE}{\CAB{\Ohorn}}
      \UnicodeCharacter{x1EDF}{\CAB{\ohorn}}
      \UnicodeCharacter{x1EE0}{\ensuremath{\tilde{\Ohorn}}}
      \UnicodeCharacter{x1EE1}{\ensuremath{\tilde{\ohorn}}}
      \UnicodeCharacter{x1EE2}{\d{\Ohorn}}
      \UnicodeCharacter{x1EE3}{\d{\ohorn}}
      \UnicodeCharacter{x1EE4}{\d U}
      \UnicodeCharacter{x1EE5}{\d u}
      \UnicodeCharacter{x1EE6}{\CAB U}
      \UnicodeCharacter{x1EE7}{\CAB u}
      \UnicodeCharacter{x1EE8}{\ensuremath{\acute{\Uhorn}}}
      \UnicodeCharacter{x1EE9}{\ensuremath{\acute{\uhorn}}}
      \UnicodeCharacter{x1EEA}{\ensuremath{\grave{\Uhorn}}}
      \UnicodeCharacter{x1EEB}{\ensuremath{\grave{\uhorn}}}
      \UnicodeCharacter{x1EEC}{\CAB{\Uhorn}}
      \UnicodeCharacter{x1EED}{\CAB{\uhorn}}
      \UnicodeCharacter{x1EEE}{\ensuremath{\tilde{\Uhorn}}}
      \UnicodeCharacter{x1EEF}{\ensuremath{\tilde{\uhorn}}}
      \UnicodeCharacter{x1EF0}{\d{\Uhorn}}
      \UnicodeCharacter{x1EF1}{\d{\uhorn}}
      \UnicodeCharacter{x1EF2}{\`Y}
      \UnicodeCharacter{x1EF3}{\`y}
      \UnicodeCharacter{x1EF4}{\d Y}
      \UnicodeCharacter{x1EF5}{\d y}
      \UnicodeCharacter{x1EF6}{\CAB Y}
      \UnicodeCharacter{x1EF7}{\CAB y}
      \UnicodeCharacter{x1EF8}{\~ Y}
      \UnicodeCharacter{x1EF9}{\~ y}
      \UnicodeCharacter{x23B0}{\lmoustache}
      \UnicodeCharacter{x23B1}{\rmoustache}
      \UnicodeCharacter{x3014}{\lgroup}
      \UnicodeCharacter{x3015}{\rgroup}
      \UnicodeCharacter{xfe37}{\textoverbrace}
      \UnicodeCharacter{xfe38}{\textunderbrace}

More chars
~~~~~~~~~~

.. container:: ltx-source

   ::


        \UnicodeCharacter{xE3F}{{\pxrfont\textbaht}}
        \UnicodeCharacter{x20A9} {{\pxrfont\textwon}}
        \UnicodeCharacter{x20A6} {{\pxrfont\textnaira}}
        \UnicodeCharacter{x20A4} {{\pxrfont\textlira}}
        \UnicodeCharacter{x20AB} {{\pxrfont\textdong}}
        \UnicodeCharacter{x2036} {\textgravedbl}
        \UnicodeCharacter{x2033} {\textacutedbl}
        \UnicodeCharacter{x2035} {\textasciigrave}
        \UnicodeCharacter{x2032} {\ifmmode '\else \textasciiacute\fi}
        \UnicodeCharacter{x2103} {\textcelsius}
        \UnicodeCharacter{x2020} {\ifmmode \dagger \else \textdagger\fi}
        \UnicodeCharacter{x2021} {\ifmmode \ddagger \else \textdaggerdbl\fi}
        \UnicodeCharacter{x2030} {\textperthousand}
        \UnicodeCharacter{x2031} {{\pxrfont\textpertenthousand}}
        \UnicodeCharacter{x203B} {{\pxrfont\textreferencemark}}
        \UnicodeCharacter{x2045} {{\pxrfont\textlquill}}
        \UnicodeCharacter{x2046} {{\pxrfont\textrquill}}
        \UnicodeCharacter{x211E} {{\pxrfont\textrecipe}}
        \UnicodeCharacter{x2117} {{\pxrfont\textcircledP}}
        \UnicodeCharacter{x2120} {{\pxrfont \textservicemark}}
        \UnicodeCharacter{x212E} {{\pxrfont \textestimated}}
        \UnicodeCharacter{x203D} {{\pxrfont \textinterrobang}}
        \UnicodeCharacter{x2190}{\leftarrow}
        \UnicodeCharacter{x2192}{\ensuremath{\rightarrow}}
        \UnicodeCharacter{x2194}{\ensuremath{\leftrightarrow}}
        \UnicodeCharacter{x21D0}{\Leftarrow}
        \UnicodeCharacter{x21D2}{\Rightarrow}
        \UnicodeCharacter{xF576}{\longleftarrow}
        \UnicodeCharacter{xF577}{\longrightarrow}
        \UnicodeCharacter{xF579}{\Longleftarrow}
        \UnicodeCharacter{xF57A}{\Longrightarrow}

        \UnicodeCharacter{xF57A}{\Longrightarrow}

        \UnicodeCharacter{xF57D}{\longmapsto}
        \UnicodeCharacter{x21A6}{\mapsto}
        \UnicodeCharacter{xF578}{\longleftrightarrow}
        \UnicodeCharacter{xF57B}{\Longleftrightarrow}
      %  \UnicodeCharacter{xE365}{\Longleftrightarrow}
        \UnicodeCharacter{x21D4}{\Leftrightarrow}
        \UnicodeCharacter{x2113}{\ell}
        \UnicodeCharacter{x332}{\jgunderline}
        \UnicodeCharacter{x2663}{\relax\ifmmode\clubsuit\else\ding{168}\fi}
        \UnicodeCharacter{x2661}{\heartsuit}
        \UnicodeCharacter{x2660}{\relax\ifmmode\spadesuit\else\ding{171}\fi}
        \UnicodeCharacter{x2666}{\relax\ifmmode\diamondsuit\else\ding{169}\fi}
        \UnicodeCharacter{x22C1}{\bigvee}
        \UnicodeCharacter{x22C0}{\bigwedge}

        \UnicodeCharacter{x230B}{\rfloor}
        \UnicodeCharacter{x260A}{\&}
        \UnicodeCharacter{x127}{{\fontencoding{LELA}\selectfont\char'070 }}
        \UnicodeCharacter{x14A}{{\fontencoding{LELA}\selectfont\char'055  }}
        \UnicodeCharacter{x14B}{{\fontencoding{LELA}\selectfont\char'075  }}
        \UnicodeCharacter{x135}{{\fontencoding{LELA}\selectfont\^{\char'072 } }}
        \UnicodeCharacter{x17F}{{\fontencoding{LELA}\selectfont\char'173 }}
        \UnicodeCharacter{x260B}{\bindnasrepma}
        \UnicodeCharacter{x5C}{\relax\ifmmode\setminus\else\textbackslash\fi}
