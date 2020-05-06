MathML Examples for Tralics
===========================

The *Tralics* documentation sometimes shows the translation of an
expression as an image. These do not always correspond to the latest
version of the software. We give here a variant, using MathML. For
instance, the translation of ``$x$`` is
``<formula type='inline'> <math xmlns='http://www.w3.org/1998/Math/MathML'><mi>x</mi></math></formula>``;
the math element of the formula renders as :math:`x`. In the case of
environment like ``align``, we have used the option -multi-math-label.
This has as effect to add ``id`` and ``id-text`` attributes to parts of
the math formula. Since ``id-text`` is not defined by MathML, we have
removed these attributes. Example: on `row1 <#uid1>`__ and
`row3 <#uid2>`__ for the eqnarray example; on `row1 <#uid3>`__ and
`row2 <#uid3>`__ for the gather example; on `row1 <#uid5>`__ and
`row2 <#uid6>`__ for the split example.

.. _accent1:

Accents
-------

Example of math accents:
``$\hat{a} \acute{a} \bar{a} \dot{a} \breve{a} \check{a} \grave{a} \vec{a} \ddot{a} \tilde{a}$``
translates as
:math:`\hat{a}\acute{a}\bar{a}\overset{˙}{a}\breve{a}\check{a}\grave{a}\overset{\rightarrow}{a}\overset{¨}{a}\widetilde{a}`.
Example of usual accents: :literal:`\\`e \'e \^e \"e \~a \.e \=e`
translates as è é ê ë ã ė ē. Example of other accents:
``\c c, \H o, \C o, \"o, \k a, \b b, \d a, \u a, \f a, \v a, \T e, \r a, \D a, \h a, \V e``
translates as ç, ő, ȍ, ö, ą, ḇ, ạ, ă, ȃ, ǎ, ḛ, å, ḁ, ả, ḙ. Example of
double accents, translation of
:literal:`\\={\.a} \={\"a}, \"{\'u} \"{\`u} \"{\v u} \c{\'c} \c{\u e} \d{\. s} \'{\^e}`
is ǡ ǟ, ǘ ǜ ǚ ḉ ḝ ṩ ế. The accentee is normally a 7bit character; there
are exeptions so that ``\=\ae \'\ae \'\aa \'\o \'\i`` translates as
ǣǽǻǿí.

\\accentset
-----------

Translation of
``$\accentset xy = \accentset{\ast}{X} = \hat{\accentset{\star}{\hat h}}   $``
is.
:math:`\overset{x}{y} = \overset{*}{X} = \hat{\overset{☆}{\hat{h}}}`.

.. _acute:

Math accents
------------

``$\acute{x} \bar{x} \breve{x} \check{x}  \ddddot{x} \dddot{x} \ddot{x} \dot{x}  \grave{x} \hat{x} \mathring{x} \tilde{x} \vec{x} \widehat{xyz} \widetilde{xyz}$``
gives
:math:`\acute{x}\bar{x}\breve{x}\check{x}\ddddot{x}\dddot{x}\overset{¨}{x}\overset{˙}{x}\grave{x}\hat{x}\overset{˚}{x}\widetilde{x}\overset{\rightarrow}{x}\hat{xyz}\widetilde{xyz}`.

``$\widetilde{abc} \widehat{abc} \overleftarrow{abc} \overrightarrow{abc} \overline{abc} \underline{abc} \overbrace{abc} \underbrace{abc} \underleftarrow{abc} \underrightarrow{abc}$``
gives
:math:`\widetilde{abc}\hat{abc}\overset{\leftarrow}{abc}\overset{\rightarrow}{abc}\bar{abc}\underline{abc}\overbrace{abc}\underbrace{abc}\underset{\leftarrow}{abc}\underset{\rightarrow}{abc}`.

``$\stackrel{j}{\longrightarrow} \overset{*}{X} \underset{*}{X} \sqrt{abc} \sqrt[n]{abc} \root n \of{abc} \frac{abc}{xyz} \dfrac{abc}{xyz}$``
gives
:math:`\overset{j}{\longrightarrow}\overset{*}{X}\underset{*}{X}\sqrt{abc}\sqrt[n]{abc}\sqrt[n]{abc}\frac{abc}{xyz}\frac{abc}{xyz}`

.. _align:

Alignments
----------

.. container:: ltx-source

   ::

      \begin{align}
      x^2+y^2&=1\\ x&=\sqrt{1-y^2}
      \end{align}

:math:`\begin{aligned}
{x^{2} + y^{2}} & {= 1} \\
x & {= \sqrt{1 - y^{2}}} \\
\end{aligned}`

.. container:: ltx-source

   ::

      \begin{equation}
      \begin{aligned}[x]
      x^2+y^2&=1& 1&=X^2+Y^2\\
      x&=0.01&0.001=X
      \end{aligned}
      \end{equation}

:math:`\begin{array}{rlrl}
{x^{2} + y^{2}} & {= 1} & 1 & {= X^{2} + Y^{2}} \\
x & {= 0.01} & {0.001 = X} & \\
\end{array}`

.. container:: ltx-source

   ::

      \def\R{\mathbf{R}}
      \begin{eqnarray*}
      \left\{\begin{array}{lcl}
      \dot{x} & = & Ax+g(x,u)\\
       y & = & Cx \\
       \multicolumn{3}{l}{x\in \R^n} 
      \end{array}
          \right.
      \end{eqnarray*}

:math:`\begin{array}{r}
\left\{ \begin{array}{lcl}
\overset{˙}{x} & = & {Ax + g\left( x,u \right)} \\
y & = & {Cx} \\
{x \in \mathbf{R}^{n}} & & \\
\end{array} \right. \\
\end{array}`

.. container:: ltx-source

   ::

      \def\MAT#1{\begin{array}{c#1}1&22\\3&4\end{array}}

      \[\left|
      \begin{array}{lcr}
      AAAAAAA&BBBBBCC&CCCCCCC\\
      A&B&C\\
      \multicolumn{1}{c}{A}&\multicolumn{1}{c}{B}&\multicolumn{1}{c}{C}\\
      \multicolumn{1}{r}{A}&\multicolumn{1}{r}{B}&\multicolumn{r}{c}{C}\\
      \multicolumn{1}{l}{A}&\multicolumn{1}{l}{B}&\multicolumn{l}{c}{C}\\
      \MAT l&\MAT c&\MAT r\\
      \multicolumn{2}{c}{0123456789abcdef}&C\\
      A&\multicolumn{2}{c}{0123456789abcdef}\\
      \multicolumn{2}{r}{0123456789abcdef}&C\\
      A&\multicolumn{2}{l}{0123456789abcdef}\\
      A&B&C\\
      \end{array}
      \right|\]

:math:`\left| \begin{matrix}
{AAAAAAA} & {BBBBBCC} & {CCCCCCC} \\
A & B & C \\
A & B & C \\
A & B & C \\
A & B & C \\
\begin{array}{cl}
1 & 22 \\
3 & 4 \\
\end{array} & \begin{matrix}
1 & 22 \\
3 & 4 \\
\end{matrix} & \begin{array}{cr}
1 & 22 \\
3 & 4 \\
\end{array} \\
{0123456789abcdef} & C & \\
A & {0123456789abcdef} & \\
{0123456789abcdef} & C & \\
A & {0123456789abcdef} & \\
A & B & C \\
\end{matrix} \right|`

.. container:: ltx-source

   ::

      \begin{eqnarray}
      x & = &17y \\
      y & > & a + b + c+d+e+f+g+h+i+j+ \nonumber\\
        &   & k+l+m+n+o+p
      \end{eqnarray}

:math:`\begin{array}{ccl} x & = & {17y} \\ y & > & {a + b + c + d + e + f + g + h + i + j +} \\ & & {k + l + m + n + o + p} \\ \end{array}`

.. container:: ltx-source

   ::

      \begin{eqnarray*}
      x & \ll & y_{1} + \cdots + y_{n} \\
        & \leq &z
      \end{eqnarray*}

:math:`\begin{array}{ccl} x & \ll & {y_{1} + \cdots + y_{n}} \\ & \leq & z \\ \end{array}`

.. container:: ltx-source
   :name: gather

   ::

      \begin{gather}
        (a + b)^2 = a^2 + 2ab + b^2          \\
        (a + b) \cdot (a - b) = a^2 - b^2
      \end{gather}

:math:`\begin{matrix}
{\left( a + b \right)^{2} = a^{2} + 2ab + b^{2}} \\
{\left( a + b \right) \cdot \left( a - b \right) = a^{2} - b^{2}} \\
\end{matrix}`

.. container:: ltx-source

   ::

      \begin{multline}
      \text{First line: left align}\\
      \text{centered}\\
      \text{This is the longest line of the table}\\
      \shoveright{\text{shoved left}}\\
      \shoveleft{\text{shoved right}}\\
      \text{Last right}
      \end{multline}

:math:`\begin{matrix}
{\text{First}\text{line:}\text{left}\text{align}} \\
\text{centered} \\
{\text{This}\text{is}\text{the}\text{longest}\text{line}\text{of}\text{the}\text{table}} \\
{\text{shoved}\text{left}} \\
{\text{shoved}\text{right}} \\
{\text{Last}\text{right}} \\
\end{matrix}`

.. container:: ltx-source
   :name: split

   ::

      \begin{equation}
      \begin{split}
      (a+b)^4 &= (a+b)^ 2 (a+b)^2           \\
              &= (a^2+2ab+b^2)(a^2+2ab+b^2) \\
              &= a^4+4a^3b+6a^2b^2+4ab^3+b^4 \\
      \end{split}
      \end{equation}

:math:`\begin{array}{cl} \left( a + b \right)^{4} & {= \left( a + b \right)^{2}\left( a + b \right)^{2}} \\ & {= \left( a^{2} + 2ab + b^{2} \right)\left( a^{2} + 2ab + b^{2} \right)} \\ & {= a^{4} + 4a^{3}b + 6a^{2}b^{2} + 4ab^{3} + b^{4}} \\ \end{array}`

Counters
--------

.. container:: ltx-source

   ::

      \def\showcounter#1{%
      \arabic{#1} \roman{#1} \Roman{#1} \alph{#1} \Alph{#1} \fnsymbol{#1}  $\fnsymbol{#1}$\\}
      \newcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}
      \stepcounter{ctr}\showcounter{ctr}

One can notice that ``\fnsymbol`` works in math mode or outside it, and
the result is not always the same. The effect of ``\\`` is to start a
no-indented paragraph by adding an attribute to the ``<p>`` element. We
have replaced it by a ``class`` attribute interpreted by the style
sheet.

.. container:: blanc

   1 i I a A \* :math:`*`

   2 ii II b B :math:`\dagger` :math:`\dagger`

   3 iii III c C :math:`\ddagger` :math:`\ddagger`

   4 iv IV d D § :math:`§`

   5 v V e E ¶ :math:`¶`

   6 vi VI f F :math:`\parallel` :math:`\parallel`

   7 vii VII g G \*\* :math:`**`

   8 viii VIII h H :math:`\dagger \dagger` :math:`\dagger \dagger`

   9 ix IX i I :math:`\ddagger \ddagger` :math:`\ddagger \ddagger`

The default translation of a Greek letter like ``\alpha`` is an entity.
There is an option -noentnames that replaces the entity names by
character references ``&alpha;`` will be replaced by ``&#x3B1;``. We
show here the entities.
:math:`\alpha\beta\gamma\delta\epsilon\epsilon\zeta\eta\theta\iota\kappa\lambda\mu\nu\xi\pi\rho\sigma\tau\upsilon\varphi\chi\psi\omega\varpi\varrho\varsigma\phi ϰ\vartheta`
:math:`\Gamma\Delta\Theta\Lambda\Xi\Sigma\mathrm{\Upsilon}\Phi\Pi\Psi\Omega`,
and the text version: α β γ δ ϵ ϵ ζ η θ ι κ λ μ ν ξ π ρ σ τ υ φ χ ψ ω ϖ
ϱ ς ϕ ϰ ϑ Γ Δ Θ Λ Ξ Σ ϒ Φ Π Ψ Ω

Binom
-----

.. container:: ltx-source

   ::

       
      \def\n{\mathchoice{a}{b}{c}{d}}
      $$\binom{\frac{\n+m}{2}} \n \quad
      \tbinom{\frac{\n+m}{2}} \n \quad
      \dbinom{\frac{\n+m}{2}} \n\qquad
      {}^{\binom{\frac{\n+m}{2}} \n \quad
      \tbinom{\frac{\n+m}{2}} \n \quad
      \dbinom{\frac{\n+m}{2}} \n}$$

:math:`\binom{\binom{c + m}{2}}{b}\binom{\binom{d + m}{2}}{c}\binom{\binom{c + m}{2}}{b}{}^{{(\binom{\binom{d + m}{2}}{d})}{(\binom{\binom{d + m}{2}}{c})}{(\binom{\binom{c + m}{2}}{b})}}`

.. container:: ltx-source

   ::

      ${a\over b}\quad {a\above1pt b}\quad {a\atop b}$
      ${a\overwithdelims() b}\quad {a\abovewithdelims()1pt b}%
        \quad {a\atopwithdelims() b}$

Translates as :math:`\frac{a}{b}\frac{a}{b}\binom{a}{b}` and
:math:`\left( \frac{a}{b} \right)\left( \frac{a}{b} \right)\binom{a}{b}`.

Bmod
----

.. container:: ltx-source

   ::

      \begin{align*}
      u& \equiv v+1 \mod{n^2} \\
      u& \equiv v+1 \bmod{n^2} \\
      u&   =    v+1 \pmod{n^2} \\
      u&   =    v+1 \pod{n^2} \\
      \end{align*}

:math:`\begin{aligned}
u & {\equiv v + 1\mspace{600mu}\operatorname{mod}\; n^{2}} \\
u & {\equiv v + 1\;\operatorname{mod}\; n^{2}} \\
u & {= v + 1\mspace{1800mu}\left( \operatorname{mod}\; n^{2} \right)} \\
u & {= v + 1\mspace{1800mu}\left( n^{2} \right)} \\
\end{aligned}`

\\boxed
-------

``$x = \boxed{\frac12}+z$`` translates as :math:`x = \begin{matrix}
\frac{1}{2} \\
\end{matrix} + z`.

Cases
-----

.. container:: ltx-source

   ::

      \[\text{The sign function: \ } 
      \mathcal{S}(x) = \begin{cases}
      -1&x<0 \\
      0&x=0 \\
      1&x>0
      \end{cases}
      \]

:math:`\text{The}\text{sign}\text{function:}\mathcal{S}\left( x \right) = \begin{cases}
{- 1} & {x < 0} \\
0 & {x = 0} \\
1 & {x > 0} \\
\end{cases}`

.. _math-font:

Math fonts
----------

``${\cal ABCDE FGHIJ KLMNO PQRST UVWXYZ}$`` gives
:math:`{\mathcal{A}\mathcal{B}\mathcal{C}\mathcal{D}\mathcal{E}}{\mathcal{F}\mathcal{G}\mathcal{H}\mathcal{I}\mathcal{J}}{\mathcal{K}\mathcal{L}\mathcal{M}\mathcal{N}\mathcal{O}}{\mathcal{P}\mathcal{Q}\mathcal{R}\mathcal{S}\mathcal{T}}{\mathcal{U}\mathcal{V}\mathcal{W}\mathcal{X}\mathcal{Y}\mathcal{Z}}`
when using Unicode characters in the SMP and
:math:`\mathcal{ABCDE}\mathcal{FGHIJ}\mathcal{KLMNO}\mathcal{PQRST}\mathcal{UVWXYZ}`
when using 7-bit characters and ``mathvariant`` attribute. The two
results should be identical.

Style
-----

.. container:: ltx-source

   ::

      ${\frac12}{x\displaystyle\frac 12} 
      {x\textstyle\frac 12}{x\scriptstyle\frac 12}{x\scriptscriptstyle\frac 12} \qquad
      {x\displaystyle a}+{x\textstyle a}+{x\scriptstyle a}+{x\scriptscriptstyle a}\qquad
      \frac{x\displaystyle a}{x\textstyle b}+\frac{x\scriptstyle a}{x\scriptscriptstyle b}$

Translates to

:math:`\frac{1}{2}{x\frac{1}{2}}{x\frac{1}{2}}{x\frac{1}{2}}{x\frac{1}{2}}{xa} + {xa} + {xa} + {xa}\frac{xa}{xb} + \frac{xa}{xb}`

.. _frac:

Fractions
---------

.. container:: ltx-source

   ::

      $$\frac12 \dfrac 12 \tfrac12 \qquad
      {}^{\frac12 \dfrac 12 \tfrac12}$$

:math:`\frac{1}{2}\frac{1}{2}\frac{1}{2}{}^{\frac{1}{2}\frac{1}{2}\frac{1}{2}}`

.. container:: ltx-source

   ::

      \def\N{\mathchoice{a}{b}{c}{d}}
      $$\frac{\N\frac{\N+m}{2}} \N \quad
      \tfrac{\N\frac{\N+m}{2}} \N \quad
      \dfrac{\N\frac{\N+m}{2}} \N
      $$

:math:`\frac{b\frac{c + m}{2}}{b}\frac{c\frac{d + m}{2}}{c}\frac{b\frac{c + m}{2}}{b}`

.. container:: ltx-source

   ::

      $$\genfrac{}{}{}{}12 % this is \frac 
      \genfrac[]{2pt}{2}{aaa}{bbb} % brackets, small, thick 
      \genfrac(){0cm}{0}nm % this is \dbinom 
      $$

:math:`\frac{1}{2}\left\lbrack \frac{aaa}{bbb} \right\rbrack\binom{n}{m}`

.. _math1:

Math tables
-----------

.. container:: ltx-source

   ::

      $\smallint \int \land \wedge \lor \owns \ni \notin
      \neg \lnot \gets \leftarrow \to \rightarrow
      \gtrless \geqslant \leqslant \iff \backslash$

Translation
:math:`\left. \int\int \land \land \vee \ni \ni \notin \neg\neg\leftarrow\leftarrow\rightarrow\rightarrow \gtrless \geqslant \leqslant \Leftrightarrow \smallsetminus \right.`

.. container:: ltx-source

   ::

       
      $ \pm \mp \times \div \ast \star \circ \bullet \cdot 
      \cap \cup \uplus \sqcap \sqcup \vee \wedge \setminus \wr
      \diamond \bigtriangleup \bigtriangledown \triangleleft \triangleright
      \oplus \ominus \otimes \oslash \odot \bigcirc \dagger \ddagger \amalg$

Table 8.4
:math:`\pm \mp \times \div *☆ \circ \bullet \cdot \cap \cup \uplus \sqcap \sqcup \vee \land \smallsetminus \wr \diamond \bigtriangleup \bigtriangledown \triangleleft \triangleright \oplus \ominus \otimes \oslash \odot ◯ \dagger \ddagger \amalg`

.. container:: ltx-source

   ::

      $\le \leq \succ \simeq \parallel \subseteq \sqsubset \doteq
      \ge \geq \sim \mid \subset \subseteq \ne \neq \frown \vdash
      \equiv \perp \ll \supset \cong \smile \in \dashv 
      \models \preceq \gg \approx \Join \sqsubseteq \ni
      \prec \succeq \asymp \bowtie \sqsupseteq \propto$

Table 8.5
:math:`\leq \leq \succ \simeq \parallel \subseteq ⊏ \doteq \geq \geq \sim \mid \subset \subseteq \neq \neq \frown \vdash \equiv \bot \ll \supset \cong \smile \in \dashv \models \preceq \gg \approx \bowtie \sqsubseteq \ni \prec \succeq \asymp \bowtie \sqsupseteq \propto`

.. container:: ltx-source

   ::

      $\leftarrow\longleftarrow  \Leftarrow \Longleftarrow 
      \rightarrow \longrightarrow \Rightarrow \Longrightarrow
      \leftrightarrow \Leftrightarrow \longleftrightarrow\Longleftrightarrow
      \mapsto \longmapsto \hookleftarrow \hookrightarrow
      \leftharpoondown \leftharpoonup \rightharpoondown \rightharpoonup
      \uparrow \Uparrow  \updownarrow \Updownarrow 
      \downarrow \Downarrow \nearrow \searrow \swarrow \nwarrow
      $

Table 8.6
:math:`\left. \leftarrow\longleftarrow\Leftarrow\Longleftarrow\rightarrow\longrightarrow\Rightarrow\Longrightarrow\leftrightarrow\Leftrightarrow\longleftrightarrow\Longleftrightarrow\mapsto\longmapsto\hookleftarrow\hookrightarrow\leftharpoondown\leftharpoonup\rightharpoondown\rightharpoonup\uparrow\Uparrow\updownarrow\Updownarrow\downarrow\Downarrow\nearrow\searrow\swarrow\nwarrow \right.`

.. container:: ltx-source

   ::

      $\ldots \prime \exists \Diamond \top \bot \mho 
      \cdots \forall \nabla \imath \flat \clubsuit \Re
      \vdots \infty \surd \jmath \natural \diamondsuit \Im
      \ddots \hbar \square \ell \sharp \heartsuit \angle
      \aleph \emptyset \triangle \neg \wp \spadesuit \partial$

Table 8.7:
:math:`...'\exists \diamond \top\bot ℧\cdots\forall\nabla\imath\flat\clubsuit\Re \vdots \infty{\sqrt{}}j\natural ♦\Im \ddots \hslash\square\ell\sharp ♥\angle\aleph\varnothing\vartriangle\neg\wp\spadesuit\partial`

.. container:: ltx-source

   ::

      $\coprod \bigvee \bigwedge \biguplus \bigcap \bigcup \int \sum 
       \prod \bigotimes \bigoplus \oint \bigsqcup  \bigodot$

Table 8.8:
:math:`\coprod\bigvee\bigwedge \uplus \bigcap\bigcup\int\sum\prod\bigotimes\bigoplus\oint\bigsqcup\bigodot`

.. container:: ltx-source

   ::

       
      $$\arccos^2 (x),\, \arcsin^2(x),\,  \arctan^2(x),\,  \arg^2(x),\, 
        \cos^2(x),\,  \cosh^2(x)  ,\, \cot^2(x)$$
      $$\coth^2(x),\, \csc^2(x),\,   \deg^2(x),\,   \dim^2(x),\, \exp^2(x),\, 
        \hom^2(x),\,  \ker^2(x) ,\, \lg^2(x)$$
      $$\ln^2(x),\, \log^2(x),\,  \Pr^2(x),\, \sec^2(x),\, \sin^2(x) 
        ,\,\sinh^2(x),\,  \tan^2(x),\, \tanh^2(x) $$
      $$\det_{x=1},\, \gcd_{x=1},\, \inf_{x=1},\, \injlim_{x=1},\, \lim_{x=1},\, 
      \liminf_{x=1},\,\limsup_{x=1},\,\max_{x=1},\, \min_{x=1},\, 
       \projlim_{x=1},\, \sup_{x=1}
      $$

Table 8.9

:math:`\arccos^{2}\left( x \right),\,\arcsin^{2}\left( x \right),\,\arctan^{2}\left( x \right),\,\arg^{2}\left( x \right),\,\cos^{2}\left( x \right),\,\cosh^{2}\left( x \right),\,\cot^{2}\left( x \right)`
:math:`\coth^{2}\left( x \right),\,\csc^{2}\left( x \right),\,\deg^{2}\left( x \right),\,\dim^{2}\left( x \right),\,\exp^{2}\left( x \right),\,\hom^{2}\left( x \right),\,\ker^{2}\left( x \right),\,\lg^{2}\left( x \right)`
:math:`\ln^{2}\left( x \right),\,\log^{2}\left( x \right),\,\Pr^{2}\left( x \right),\,\sec^{2}\left( x \right),\,\sin^{2}\left( x \right),\,\sinh^{2}\left( x \right),\,\tan^{2}\left( x \right),\,\tanh^{2}\left( x \right)`
:math:`\det\limits_{x = 1},\,\gcd\limits_{x = 1},\,\inf\limits_{x = 1},\,\operatorname{inj\ lim}\limits_{x = 1},\,\lim\limits_{x = 1},\,\operatorname{lim\ inf}\limits_{x = 1},\,\operatorname{lim\ sup}\limits_{x = 1},\,\max\limits_{x = 1},\,\min\limits_{x = 1},\,\operatorname{proj\ lim}\limits_{x = 1},\,\sup\limits_{x = 1}`

.. container:: ltx-source

   ::

      \def\kernel{\frac{x^1_2}{y^3_4}}
      \def\test{
      \left\lmoustache\left\uparrow\left\Uparrow\left\updownarrow\left\lgroup
      \kernel
      \right\rgroup\right\Updownarrow\right\Downarrow\right\downarrow\right\rmoustache}
      $\test$
      $\let\left\relax\let\right\relax \test$
      $\def\kernel{\int_0^{\frac{x^1_2}{y^3_4}}} \test$

Translation is
:math:`⎰\left. \uparrow\left. \Uparrow\left. \updownarrow{〔\frac{x_{2}^{1}}{y_{4}^{3}}〕}\Updownarrow \right.\Downarrow \right.\downarrow \right.⎱`
and
:math:`\left. ⎰\uparrow\Uparrow\updownarrow{〔\frac{x_{2}^{1}}{y_{4}^{3}}〕}\Updownarrow\Downarrow\downarrow ⎱ \right.`
and
:math:`⎰\left. \uparrow\left. \Uparrow\left. \updownarrow{〔\int_{0}^{\frac{x_{2}^{1}}{y_{4}^{3}}}〕}\Updownarrow \right.\Downarrow \right.\downarrow \right.⎱`

Delimiters
----------

.. container:: ltx-source

   ::

       
      $
      \def\kernel{\frac{x^1_2}{y^3_4}}
      \left\Vert\left|\left[\left(\left\{ \kernel \right\}\right)\right]\right|\right\Vert
      \left\vert\left|\left<\left\langle \kernel \right\rangle\right>\right|\right\vert
      \left\lbrace\left\lceil\left\lfloor \kernel \right\rfloor\right\rceil\right\rbrace
      \lbrack\rbrack
      $

Translation
:math:`{\parallel \left| \left\lbrack \left( \left\{ \frac{x_{2}^{1}}{y_{4}^{3}} \right\} \right) \right\rbrack \right| \parallel}\left| \left| \left\langle \left\langle \frac{x_{2}^{1}}{y_{4}^{3}} \right\rangle \right\rangle \right| \right|\left\{ \left\lceil \left\lfloor \frac{x_{2}^{1}}{y_{4}^{3}} \right\rfloor \right\rceil \right\}{\lbrack\rbrack}`

.. container:: ltx-source

   ::

       
      \[ \left(\left[\left\lbrack \left\{\left\lbrace\left\lfloor 
      \left\lceil\left\langle\left/ \left|\left\|\left\uparrow 
      \left\downarrow\left\updownarrow\left<\left\lmoustache
      \left\lgroup\frac 12\right\rgroup
      \right\rmoustache\right>\right\Updownarrow\right\downarrow 
      \right\Uparrow\right\Vert\right\vert \right\backslash\right\rangle
      \right\rceil\right\rfloor\right\rbrace\right\} \right\rbrack\right]\right)\]

:math:`\left( \left\lbrack \left\lbrack \left\{ \left\{ \left\lfloor \left\lceil \left\langle {/\left| {\parallel \left. \uparrow\left. \downarrow\left. \updownarrow\left\langle {⎰{〔\frac{1}{2}〕}⎱} \right\rangle\Updownarrow \right.\downarrow \right.\Uparrow \right. \parallel} \right| \smallsetminus} \right\rangle \right\rceil \right\rfloor \right\} \right\} \right\rbrack \right\rbrack \right)`

.. _math2:

Math examples
-------------

.. container:: ltx-source

   ::

      \providecommand\operatorname[1]{%
        \mathmo{#1}%
        \mathattribute{form}{prefix}%
        \mathattribute{movablelimits}{true}%
      }
      \def\Dmin{\operatorname{dmin}}
      $\min _xf(x) >\Dmin _xf(x)$.

Translation
:math:`\min_{x}f\left( x \right) > \operatorname{dmin}_{x}f\left( x \right)`

Limits
------

.. container:: ltx-source

   ::

      \def\A#1{\mathop a #1_b, \mathop a#1^c, \mathop a#1_b^c\qquad}
      $\A{} \A\limits \A\nolimits \A\displaylimits $
      \[\A{} \A\limits \A\nolimits \A\displaylimits \]

Compare the inline math formula
:math:`a_{b},a^{c},a_{b}^{c}\underset{b}{a},\overset{c}{a},\underset{b}{\overset{c}{a}} a_{b},a^{c},a_{b}^{c} a_{b},a^{c},a_{b}^{c}`
and the display math formula

:math:`\underset{b}{a},\overset{c}{a},\underset{b}{\overset{c}{a}}\underset{b}{a},\overset{c}{a},\underset{b}{\overset{c}{a}} a_{b},a^{c},a_{b}^{c}\underset{b}{a},\overset{c}{a},\underset{b}{\overset{c}{a}}`

.. container:: ltx-source

   ::

      \DeclareMathOperator{\Sin}{sin}
      \DeclareMathOperator*{\Limsup}{lim \, sup}
      %% the effect of * is visible only in display style
      \[\Sin^2 = \sin^2 = \operatorname{sin}^2 \quad
      \Limsup_x = \limsup_x = \operatorname*{lim\;sup}_x \]

:math:`\sin^{2} = \sin^{2} = \sin^{2}\operatorname{lim\ sup}\limits_{x} = \operatorname{lim\ sup}\limits_{x} = \operatorname{lim\ sup}\limits_{x}`

Attributes
----------

.. container:: ltx-source

   ::

      \begin{align}
      \formulaattribute{tag}{8-2-3}
      \thismathattribute{background}{yellow}
      \tableattribute{mathcolor}{red}
      \rowattribute{mathvariant}{bold} x^2 + y^2+100 &=  z^2 \\
      \cellattribute{columnalign}{left}\cellattribute{class}{bl}  x^3 + y^3+1 &<  z^3
      \cellattribute{mathbackground}{white}\cellattribute{mathvariant}{bold} 
      \end{align}

This example produces illegal MathML (illegal attribute names) so that
what you see below is a modification of the MathML code produced by
*Tralics*. The attribute ``background`` is illegal, it was replaced by
``mathbackground``. This attribute, as well as ``mathcolor`` and
``mathvariant`` can only be put in token elements. So we inserted two
``<mstyle>`` elements with the attributes, one that contains the table,
one that contains the content of the last cell. It is not possible to
add a ``mathvariant`` attribute to the first row since (a) one cannot
put the row in a ``<mstyle>`` element and (b) the child of the row
cannot be a ``<mstyle>`` element.

:math:`\begin{array}{cl}
{x^{2} + y^{2} + 100} & {= z^{2}} \\
{x^{3} + y^{3} + 1} & \mathbf{< z^{3}} \\
\end{array}`

.. _spacing:

Math spacing
------------

.. container:: ltx-source

   ::

      Math spacing\\
      $xxxxx$\\
      $x\,x\>x\;x\!x$\\
      $a\,\,\,\,\,a\>\>\>\>\>a$\\
      $a\;\;\;a\;\;\;\;a$\\
        

| Math spacing
| :math:`xxxxx`
| :math:`x\, x\ x\; x\! x`
| :math:`a\,\,\,\,\, a\ \ \ \ \ a`
| :math:`a\;\;\; a\;\;\;\; a`

Sideset
-------

.. container:: ltx-source

   ::

      $\sideset{}{'}\sum_i x_i$
      $\sideset{^{a}_b}{_{D}^c}\sum_x^yw$

Translates as ∑' i x i and ∑Dcba x y w; same formula in
``\displaystyle`` added ∑' i x i and ∑Dcba x y w

.. container:: ltx-source

   ::

      \[\xleftarrow{U+u}  \xleftarrow[D+d]{} \xleftarrow[U+u+v]{D+d}
      \xrightarrow{U+u}  \xrightarrow[D+d]{} \xrightarrow[U+u]{P+p+q}\]

:math:`\overset{U + u}{\leftarrow}\underset{D + d}{\leftarrow}\underset{U + u + v}{\overset{D + d}{\leftarrow}}\overset{U + u}{\rightarrow}\underset{D + d}{\rightarrow}\underset{U + u}{\overset{P + p + q}{\rightarrow}}`

Characters
----------

The following tables were generated accorging to the code given in `this
file <doc-chars.html>`__. We have regoupred all characters with Unicode
betwenn U+1D400 and U+1D7FF in a single table. At position 1D551 there
is ``\red\mathZopf`` where the first fommand is defined by
``\def\red#1{#1\mathattribute{mathcolor}{red}}``. This means that the
character shouls be there, but is in fact at position U+2124.

:math:`\begin{array}{lcccccccccccccccc}
\text{1d400} & \mathbf{A} & \mathbf{B} & \mathbf{C} & \mathbf{D} & \mathbf{E} & \mathbf{F} & \mathbf{G} & \mathbf{H} & \mathbf{I} & \mathbf{J} & \mathbf{K} & \mathbf{L} & \mathbf{M} & \mathbf{N} & \mathbf{O} & \mathbf{P} \\
\text{1d410} & \mathbf{Q} & \mathbf{R} & \mathbf{S} & \mathbf{T} & \mathbf{U} & \mathbf{V} & \mathbf{W} & \mathbf{X} & \mathbf{Y} & \mathbf{Z} & \mathbf{a} & \mathbf{b} & \mathbf{c} & \mathbf{d} & \mathbf{e} & \mathbf{f} \\
\text{1d420} & \mathbf{g} & \mathbf{h} & \mathbf{i} & \mathbf{j} & \mathbf{k} & \mathbf{l} & \mathbf{m} & \mathbf{n} & \mathbf{o} & \mathbf{p} & \mathbf{q} & \mathbf{r} & \mathbf{s} & \mathbf{t} & \mathbf{u} & \mathbf{v} \\
\text{1d430} & \mathbf{w} & \mathbf{x} & \mathbf{y} & \mathbf{z} & A & B & C & D & E & F & G & H & I & J & K & L \\
\text{1d440} & M & N & O & P & Q & R & S & T & U & V & W & X & Y & Z & a & b \\
\text{1d450} & c & d & e & f & g & h & i & j & k & l & m & n & o & p & q & r \\
\text{1d460} & s & t & u & v & w & x & y & z & \mathbf{A} & \mathbf{B} & \mathbf{C} & \mathbf{D} & \mathbf{E} & \mathbf{F} & \mathbf{G} & \mathbf{H} \\
\text{1d470} & \mathbf{I} & \mathbf{J} & \mathbf{K} & \mathbf{L} & \mathbf{M} & \mathbf{N} & \mathbf{O} & \mathbf{P} & \mathbf{Q} & \mathbf{R} & \mathbf{S} & \mathbf{T} & \mathbf{U} & \mathbf{V} & \mathbf{W} & \mathbf{X} \\
\text{1d480} & \mathbf{Y} & \mathbf{Z} & \mathbf{a} & \mathbf{b} & \mathbf{c} & \mathbf{d} & \mathbf{e} & \mathbf{f} & \mathbf{g} & \mathbf{h} & \mathbf{i} & \mathbf{j} & \mathbf{k} & \mathbf{l} & \mathbf{m} & \mathbf{n} \\
\text{1d490} & \mathbf{o} & \mathbf{p} & \mathbf{q} & \mathbf{r} & \mathbf{s} & \mathbf{t} & \mathbf{u} & \mathbf{v} & \mathbf{w} & \mathbf{x} & \mathbf{y} & \mathbf{z} & \mathcal{A} & \mathcal{B} & \mathcal{C} & \mathcal{D} \\
\text{1d4a0} & \mathcal{E} & \mathcal{F} & \mathcal{G} & \mathcal{H} & \mathcal{I} & \mathcal{J} & \mathcal{K} & \mathcal{L} & \mathcal{M} & \mathcal{N} & \mathcal{O} & \mathcal{P} & \mathcal{Q} & \mathcal{R} & \mathcal{S} & \mathcal{T} \\
\text{1d4b0} & \mathcal{U} & \mathcal{V} & \mathcal{W} & \mathcal{X} & \mathcal{Y} & \mathcal{Z} & \mathcal{a} & \mathcal{b} & \mathcal{c} & \mathcal{d} & \mathcal{e} & \mathcal{f} & \mathcal{g} & \mathcal{h} & \mathcal{i} & \mathcal{j} \\
\text{1d4c0} & \mathcal{k} & \mathcal{l} & \mathcal{m} & \mathcal{n} & \mathcal{o} & \mathcal{p} & \mathcal{q} & \mathcal{r} & \mathcal{s} & \mathcal{t} & \mathcal{u} & \mathcal{v} & \mathcal{w} & \mathcal{x} & \mathcal{y} & \mathcal{z} \\
\text{1d4d0} & \mathcal{A} & \mathcal{B} & \mathcal{C} & \mathcal{D} & \mathcal{E} & \mathcal{F} & \mathcal{G} & \mathcal{H} & \mathcal{I} & \mathcal{J} & \mathcal{K} & \mathcal{L} & \mathcal{M} & \mathcal{N} & \mathcal{O} & \mathcal{P} \\
\text{1d4e0} & \mathcal{Q} & \mathcal{R} & \mathcal{S} & \mathcal{T} & \mathcal{U} & \mathcal{V} & \mathcal{W} & \mathcal{X} & \mathcal{Y} & \mathcal{Z} & \mathcal{a} & \mathcal{b} & \mathcal{c} & \mathcal{d} & \mathcal{e} & \mathcal{f} \\
\text{1d4f0} & \mathcal{g} & \mathcal{h} & \mathcal{i} & \mathcal{j} & \mathcal{k} & \mathcal{l} & \mathcal{m} & \mathcal{n} & \mathcal{o} & \mathcal{p} & \mathcal{q} & \mathcal{r} & \mathcal{s} & \mathcal{t} & \mathcal{u} & \mathcal{v} \\
\text{1d500} & \mathcal{w} & \mathcal{x} & \mathcal{y} & \mathcal{z} & \mathfrak{A} & \mathfrak{B} & \mathfrak{C} & \mathfrak{D} & \mathfrak{E} & \mathfrak{F} & \mathfrak{G} & \mathfrak{H} & \Im & \mathfrak{J} & \mathfrak{K} & \mathfrak{L} \\
\text{1d510} & \mathfrak{M} & \mathfrak{N} & \mathfrak{O} & \mathfrak{P} & \mathfrak{Q} & \Re & \mathfrak{S} & \mathfrak{T} & \mathfrak{U} & \mathfrak{V} & \mathfrak{W} & \mathfrak{X} & \mathfrak{Y} & \mathfrak{Z} & \mathfrak{a} & \mathfrak{b} \\
\text{1d520} & \mathfrak{c} & \mathfrak{d} & \mathfrak{e} & \mathfrak{f} & \mathfrak{g} & \mathfrak{h} & \mathfrak{i} & \mathfrak{j} & \mathfrak{k} & \mathfrak{l} & \mathfrak{m} & \mathfrak{n} & \mathfrak{o} & \mathfrak{p} & \mathfrak{q} & \mathfrak{r} \\
\text{1d530} & \mathfrak{s} & \mathfrak{t} & \mathfrak{u} & \mathfrak{v} & \mathfrak{w} & \mathfrak{x} & \mathfrak{y} & \mathfrak{z} & \mathbb{A} & \mathbb{B} & \mathbb{C} & \mathbb{D} & \mathbb{E} & \mathbb{F} & \mathbb{G} & \mathbb{H} \\
\text{1d540} & \mathbb{I} & \mathbb{J} & \mathbb{K} & \mathbb{L} & \mathbb{M} & \mathbb{N} & \mathbb{O} & \mathbb{P} & \mathbb{Q} & \mathbb{R} & \mathbb{S} & \mathbb{T} & \mathbb{U} & \mathbb{V} & \mathbb{W} & \mathbb{X} \\
\text{1d550} & \mathbb{Y} & \mathbb{Z} & \mathbb{a} & \mathbb{b} & \mathbb{c} & \mathbb{d} & \mathbb{e} & \mathbb{f} & \mathbb{g} & \mathbb{h} & \mathbb{i} & \mathbb{j} & \Bbbk & \mathbb{l} & \mathbb{m} & \mathbb{n} \\
\text{1d560} & \mathbb{o} & \mathbb{p} & \mathbb{q} & \mathbb{r} & \mathbb{s} & \mathbb{t} & \mathbb{u} & \mathbb{v} & \mathbb{w} & \mathbb{x} & \mathbb{y} & \mathbb{z} & \mathfrak{A} & \mathfrak{B} & \mathfrak{C} & \mathfrak{D} \\
\text{1d570} & \mathfrak{E} & \mathfrak{F} & \mathfrak{G} & \mathfrak{H} & \mathfrak{I} & \mathfrak{J} & \mathfrak{K} & \mathfrak{L} & \mathfrak{M} & \mathfrak{N} & \mathfrak{O} & \mathfrak{P} & \mathfrak{Q} & \mathfrak{R} & \mathfrak{S} & \mathfrak{T} \\
\text{1d580} & \mathfrak{U} & \mathfrak{V} & \mathfrak{W} & \mathfrak{X} & \mathfrak{Y} & \mathfrak{Z} & \mathfrak{a} & \mathfrak{b} & \mathfrak{c} & \mathfrak{d} & \mathfrak{e} & \mathfrak{f} & \mathfrak{g} & \mathfrak{h} & \mathfrak{i} & \mathfrak{j} \\
\text{1d590} & \mathfrak{k} & \mathfrak{l} & \mathfrak{m} & \mathfrak{n} & \mathfrak{o} & \mathfrak{p} & \mathfrak{q} & \mathfrak{r} & \mathfrak{s} & \mathfrak{t} & \mathfrak{u} & \mathfrak{v} & \mathfrak{w} & \mathfrak{x} & \mathfrak{y} & \mathfrak{z} \\
\text{1d5a0} & \mathsf{A} & \mathsf{B} & \mathsf{C} & \mathsf{D} & \mathsf{E} & \mathsf{F} & \mathsf{G} & \mathsf{H} & \mathsf{I} & \mathsf{J} & \mathsf{K} & \mathsf{L} & \mathsf{M} & \mathsf{N} & \mathsf{O} & \mathsf{P} \\
\text{1d5b0} & \mathsf{Q} & \mathsf{R} & \mathsf{S} & \mathsf{T} & \mathsf{U} & \mathsf{V} & \mathsf{W} & \mathsf{X} & \mathsf{Y} & \mathsf{Z} & \mathsf{a} & \mathsf{b} & \mathsf{c} & \mathsf{d} & \mathsf{e} & \mathsf{f} \\
\text{1d5c0} & \mathsf{g} & \mathsf{h} & \mathsf{i} & \mathsf{j} & \mathsf{k} & \mathsf{l} & \mathsf{m} & \mathsf{n} & \mathsf{o} & \mathsf{p} & \mathsf{q} & \mathsf{r} & \mathsf{s} & \mathsf{t} & \mathsf{u} & \mathsf{v} \\
\text{1d5d0} & \mathsf{w} & \mathsf{x} & \mathsf{y} & \mathsf{z} & \mathbf{A} & \mathbf{B} & \mathbf{C} & \mathbf{D} & \mathbf{E} & \mathbf{F} & \mathbf{G} & \mathbf{H} & \mathbf{I} & \mathbf{J} & \mathbf{K} & \mathbf{L} \\
\text{1d5e0} & \mathbf{M} & \mathbf{N} & \mathbf{O} & \mathbf{P} & \mathbf{Q} & \mathbf{R} & \mathbf{S} & \mathbf{T} & \mathbf{U} & \mathbf{V} & \mathbf{W} & \mathbf{X} & \mathbf{Y} & \mathbf{Z} & \mathbf{a} & \mathbf{b} \\
\text{1d5f0} & \mathbf{c} & \mathbf{d} & \mathbf{e} & \mathbf{f} & \mathbf{g} & \mathbf{h} & \mathbf{i} & \mathbf{j} & \mathbf{k} & \mathbf{l} & \mathbf{m} & \mathbf{n} & \mathbf{o} & \mathbf{p} & \mathbf{q} & \mathbf{r} \\
\text{1d600} & \mathbf{s} & \mathbf{t} & \mathbf{u} & \mathbf{v} & \mathbf{w} & \mathbf{x} & \mathbf{y} & \mathbf{z} & \mathsf{A} & \mathsf{B} & \mathsf{C} & \mathsf{D} & \mathsf{E} & \mathsf{F} & \mathsf{G} & \mathsf{H} \\
\text{1d610} & \mathsf{I} & \mathsf{J} & \mathsf{K} & \mathsf{L} & \mathsf{M} & \mathsf{N} & \mathsf{O} & \mathsf{P} & \mathsf{Q} & \mathsf{R} & \mathsf{S} & \mathsf{T} & \mathsf{U} & \mathsf{V} & \mathsf{W} & \mathsf{X} \\
\text{1d620} & \mathsf{Y} & \mathsf{Z} & \mathsf{a} & \mathsf{b} & \mathsf{c} & \mathsf{d} & \mathsf{e} & \mathsf{f} & \mathsf{g} & \mathsf{h} & \mathsf{i} & \mathsf{j} & \mathsf{k} & \mathsf{l} & \mathsf{m} & \mathsf{n} \\
\text{1d630} & \mathsf{o} & \mathsf{p} & \mathsf{q} & \mathsf{r} & \mathsf{s} & \mathsf{t} & \mathsf{u} & \mathsf{v} & \mathsf{w} & \mathsf{x} & \mathsf{y} & \mathsf{z} & \mathbf{A} & \mathbf{B} & \mathbf{C} & \mathbf{D} \\
\text{1d640} & \mathbf{E} & \mathbf{F} & \mathbf{G} & \mathbf{H} & \mathbf{I} & \mathbf{J} & \mathbf{K} & \mathbf{L} & \mathbf{M} & \mathbf{N} & \mathbf{O} & \mathbf{P} & \mathbf{Q} & \mathbf{R} & \mathbf{S} & \mathbf{T} \\
\text{1d650} & \mathbf{U} & \mathbf{V} & \mathbf{W} & \mathbf{X} & \mathbf{Y} & \mathbf{Z} & \mathbf{a} & \mathbf{b} & \mathbf{c} & \mathbf{d} & \mathbf{e} & \mathbf{f} & \mathbf{g} & \mathbf{h} & \mathbf{i} & \mathbf{j} \\
\text{1d660} & \mathbf{k} & \mathbf{l} & \mathbf{m} & \mathbf{n} & \mathbf{o} & \mathbf{p} & \mathbf{q} & \mathbf{r} & \mathbf{s} & \mathbf{t} & \mathbf{u} & \mathbf{v} & \mathbf{w} & \mathbf{x} & \mathbf{y} & \mathbf{z} \\
\text{1d670} & \mathtt{A} & \mathtt{B} & \mathtt{C} & \mathtt{D} & \mathtt{E} & \mathtt{F} & \mathtt{G} & \mathtt{H} & \mathtt{I} & \mathtt{J} & \mathtt{K} & \mathtt{L} & \mathtt{M} & \mathtt{N} & \mathtt{O} & \mathtt{P} \\
\text{1d670} & \mathtt{A} & \mathtt{B} & \mathtt{C} & \mathtt{D} & \mathtt{E} & \mathtt{F} & \mathtt{G} & \mathtt{H} & \mathtt{I} & \mathtt{J} & \mathtt{K} & \mathtt{L} & \mathtt{M} & \mathtt{N} & \mathtt{O} & \mathtt{P} \\
\text{1d680} & \mathtt{Q} & \mathtt{R} & \mathtt{S} & \mathtt{T} & \mathtt{U} & \mathtt{V} & \mathtt{W} & \mathtt{X} & \mathtt{Y} & \mathtt{Z} & \mathtt{a} & \mathtt{b} & \mathtt{c} & \mathtt{d} & \mathtt{e} & \mathtt{f} \\
\text{1d690} & \mathtt{g} & \mathtt{h} & \mathtt{i} & \mathtt{j} & \mathtt{k} & \mathtt{l} & \mathtt{m} & \mathtt{n} & \mathtt{o} & \mathtt{p} & \mathtt{q} & \mathtt{r} & \mathtt{s} & \mathtt{t} & \mathtt{u} & \mathtt{v} \\
\text{1d6a0} & \mathtt{w} & \mathtt{x} & \mathtt{y} & \mathtt{z} & & & & & \mathbf{Α} & \mathbf{Β} & \mathbf{\Gamma} & \mathbf{\Delta} & \mathbf{Ε} & \mathbf{Ζ} & \mathbf{Η} & \mathbf{\Theta} \\
\text{1d6b0} & \mathbf{Ι} & \mathbf{Κ} & \mathbf{\Lambda} & \mathbf{Μ} & \mathbf{Ν} & \mathbf{\Xi} & \mathbf{Ο} & \mathbf{\Pi} & \mathbf{Ρ} & \mathbf{ϴ} & \mathbf{\Sigma} & \mathbf{Τ} & \mathbf{\Upsilon} & \mathbf{\Phi} & \mathbf{Χ} & \mathbf{\Psi} \\
\text{1d6c0} & \mathbf{\Omega} & \mathbf{∇} & \mathbf{α} & \mathbf{β} & \mathbf{γ} & \mathbf{δ} & \mathbf{ε} & \mathbf{ζ} & \mathbf{η} & \mathbf{θ} & \mathbf{ι} & \mathbf{κ} & \mathbf{λ} & \mathbf{μ} & \mathbf{ν} & \mathbf{ξ} \\
\text{1d6d0} & \mathbf{ο} & \mathbf{π} & \mathbf{ρ} & \mathbf{ς} & \mathbf{σ} & \mathbf{τ} & \mathbf{υ} & \mathbf{φ} & \mathbf{χ} & \mathbf{ψ} & \mathbf{ω} & \mathbf{∂} & \mathbf{ϵ} & \mathbf{ϑ} & \mathbf{ϰ} & \mathbf{ϕ} \\
\text{1d6e0} & \mathbf{ϱ} & \mathbf{ϖ} & \mathit{Α} & \mathit{Β} & \mathit{Γ} & \mathit{Δ} & \mathit{Ε} & \mathit{Ζ} & \mathit{Η} & \mathit{Θ} & \mathit{Ι} & \mathit{Κ} & \mathit{Λ} & \mathit{Μ} & \mathit{Ν} & \mathit{Ξ} \\
\text{1d6f0} & \mathit{Ο} & \mathit{Π} & \mathit{Ρ} & \mathit{ϴ} & \mathit{Σ} & \mathit{Τ} & \mathit{Υ} & \mathit{Φ} & \mathit{Χ} & \mathit{Ψ} & \mathit{Ω} & \mathit{∇} & \alpha & \beta & \gamma & \delta \\
\text{1d700} & \varepsilon & \zeta & \eta & \theta & \iota & \kappa & \lambda & \mu & \nu & \xi & \mathit{ο} & \pi & \rho & \varsigma & \sigma & \tau \\
\text{1d710} & \upsilon & \varphi & \chi & \psi & \omega & \partial & \epsilon & \vartheta & \varkappa & \phi & \varrho & \varpi & \mathbf{Α} & \mathbf{Β} & \mathbf{Γ} & \mathbf{Δ} \\
\text{1d720} & \mathbf{Ε} & \mathbf{Ζ} & \mathbf{Η} & \mathbf{Θ} & \mathbf{Ι} & \mathbf{Κ} & \mathbf{Λ} & \mathbf{Μ} & \mathbf{Ν} & \mathbf{Ξ} & \mathbf{Ο} & \mathbf{Π} & \mathbf{Ρ} & \mathbf{ϴ} & \mathbf{Σ} & \mathbf{Τ} \\
\text{1d730} & \mathbf{Υ} & \mathbf{Φ} & \mathbf{Χ} & \mathbf{Ψ} & \mathbf{Ω} & \mathbf{∇} & \mathbf{α} & \mathbf{β} & \mathbf{γ} & \mathbf{δ} & \mathbf{ε} & \mathbf{ζ} & \mathbf{η} & \mathbf{θ} & \mathbf{ι} & \mathbf{κ} \\
\text{1d740} & \mathbf{λ} & \mathbf{μ} & \mathbf{ν} & \mathbf{ξ} & \mathbf{ο} & \mathbf{π} & \mathbf{ρ} & \mathbf{ς} & \mathbf{σ} & \mathbf{τ} & \mathbf{υ} & \mathbf{φ} & \mathbf{χ} & \mathbf{ψ} & \mathbf{ω} & \mathbf{∂} \\
\text{1d750} & \mathbf{ϵ} & \mathbf{ϑ} & \mathbf{ϰ} & \mathbf{ϕ} & \mathbf{ϱ} & \mathbf{ϖ} & \mathbf{Α} & \mathbf{Β} & \mathbf{Γ} & \mathbf{Δ} & \mathbf{Ε} & \mathbf{Ζ} & \mathbf{Η} & \mathbf{Θ} & \mathbf{Ι} & \mathbf{Κ} \\
\text{1d760} & \mathbf{Λ} & \mathbf{Μ} & \mathbf{Ν} & \mathbf{Ξ} & \mathbf{Ο} & \mathbf{Π} & \mathbf{Ρ} & \mathbf{ϴ} & \mathbf{Σ} & \mathbf{Τ} & \mathbf{Υ} & \mathbf{Φ} & \mathbf{Χ} & \mathbf{Ψ} & \mathbf{Ω} & \mathbf{∇} \\
\text{1d7c0} & & & & & & & & & & & & & & & \mathbf{0} & \mathbf{1} \\
\text{1d7d0} & \mathbf{2} & \mathbf{3} & \mathbf{4} & \mathbf{5} & \mathbf{6} & \mathbf{7} & \mathbf{8} & \mathbf{9} & \mathbb{0} & \mathbb{1} & \mathbb{2} & \mathbb{3} & \mathbb{4} & \mathbb{5} & \mathbb{6} & \mathbb{7} \\
\text{1d7e0} & \mathbb{8} & \mathbb{9} & \mathsf{0} & \mathsf{1} & \mathsf{2} & \mathsf{3} & \mathsf{4} & \mathsf{5} & \mathsf{6} & \mathsf{7} & \mathsf{8} & \mathsf{9} & \mathbf{0} & \mathbf{1} & \mathbf{2} & \mathbf{3} \\
\text{1d7f0} & \mathbf{4} & \mathbf{5} & \mathbf{6} & \mathbf{7} & \mathbf{8} & \mathbf{9} & \mathtt{0} & \mathtt{1} & \mathtt{2} & \mathtt{3} & \mathtt{4} & \mathtt{5} & \mathtt{6} & \mathtt{7} & \mathtt{8} & \mathtt{9} \\
\end{array}`

Other symbols

:math:`\begin{array}{lcccccccccccccccc}
\text{2300} & \varnothing & ⌁ & ⌂ & ⌃ & ⌄ & \barwedge & \doublebarwedge & ⌇ & \lceil & \rceil & \lfloor & \rfloor & ⌌ & ⌍ & ⌎ & ⌏ \\
\text{2310} & ⌐ & ⌑ & ⌒ & ⌓ & ⌔ & ⌕ & ⌖ & ⌗ & ⌘ & ⌙ & ⌚ & ⌛ & ⌜ & ⌝ & ⌞ & ⌟ \\
\text{2320} & ⌠ & ⌡ & \frown & \smile & ⌤ & ⌥ & ⌦ & ⌧ & ⌨ & \langle & \rangle & ⌫ & ⌬ & ⌭ & ⌮ & ⌯ \\
\text{2330} & & & & & & & ⌶ & & & ⌹ & & & & ⌽ & & ⌿ \\
\text{2340} & ⍀ & & & & & & & ⍇ & ⍈ & & & & & & & \\
\text{2350} & ⍐ & & & & & & & ⍗ & & & & & & & & \\
\text{2600} & & ☁ & & & & \bigstar & ☆ & & & & & & ☌ & {☍☽} & ☾ & ☿ \\
\text{2640} & ♀ & ♁ & ♂ & ♃ & ♄ & ♅ & ♆ & ♇ & ♈ & ♉ & ♊ & ♋ & ♌ & ♍ & ♎ & ♏ \\
\text{2650} & ♐ & ♑ & ♒ & ♓ & ♔ & ♕ & ♖ & ♗ & ♘ & ♙ & ♚ & ♛ & ♜ & ♝ & ♞ & ♟ \\
\text{2660} & \spadesuit & \heartsuit & \diamondsuit & \clubsuit & ♤ & ♥ & ♦ & ♧ & & ♩ & ♪ & & & \flat & \natural & \sharp \\
\text{2680} & ⚀ & ⚁ & ⚂ & ⚃ & ⚄ & ⚅ & & & & & & & & & & \\
\end{array}`

And more

:math:`\begin{array}{lcccccccccccccccc}
\text{2100} & ℀ & ℁ & {\mathbb{C}} & ℃ & ℄ & ℅ & ℆ & ℇ & ℈ & ℉ & \mathcal{g} & \mathcal{H} & \mathfrak{H} & \mathbb{H} & h & \hslash \\
\text{2110} & \mathcal{I} & \Im & \mathcal{L} & \ell & ℔ & \mathbb{N} & № & ℗ & \wp & \mathbb{P} & \mathbb{Q} & \mathcal{R} & \Re & \mathbb{R} & ℞ & ℟ \\
\text{2120} & ℠ & ℡ & ™ & ℣ & \mathbb{Z} & ℥ & \mathrm{\Omega} & ℧ & \mathfrak{Z} & ℩ & K & \mathring{\mathrm{A}} & \mathcal{B} & \mathfrak{C} & ℮ & \mathcal{e} \\
\text{2130} & \mathcal{E} & \mathcal{F} & \Finv & \mathcal{M} & \mathcal{o} & \aleph & \beth & \gimel & \daleth & ℹ & ℺ & ℻ & & \mathbb{γ} & \mathbb{Γ} & \mathbb{Π} \\
\text{2140} & \mathbb{∑} & \Game & ⅂ & ⅃ & ⅄ & \mathbb{D} & \mathbb{d} & \mathbb{e} & \mathbb{i} & \mathbb{j} & ⅊ & ⅋ & & & & \\
\text{2190} & \leftarrow & \uparrow & \rightarrow & \downarrow & \leftrightarrow & \updownarrow & \nwarrow & \nearrow & \searrow & \swarrow & \nleftarrow & \nrightarrow & . & ↝ & \twoheadleftarrow & ↟ \\
\text{21A0} & \twoheadrightarrow & ↡ & \leftarrowtail & \rightarrowtail & ↤ & ↥ & \mapsto & ↧ & . & \hookleftarrow & \hookrightarrow & \looparrowleft & \looparrowright & \leftrightsquigarrow & \nleftrightarrow & ↯ \\
\text{21B0} & \Lsh & \Rsh & ↲ & ↳ & . & . & \curvearrowleft & \curvearrowright & . & . & \circlearrowleft & \circlearrowright & \leftharpoonup & \leftharpoondown & \upharpoonright & \upharpoonleft \\
\text{21C0} & \rightharpoonup & \rightharpoondown & \downharpoonright & \downharpoonleft & \rightleftarrows & ⇅ & \leftrightarrows & \leftleftarrows & \upuparrows & \rightrightarrows & \downdownarrows & \leftrightharpoons & \rightleftharpoons & \nLeftarrow & \nLeftrightarrow & \nRightarrow \\
\text{21D0} & \Leftarrow & \Uparrow & \Rightarrow & \Downarrow & \Leftrightarrow & \Updownarrow & ⇖ & ⇗ & ⇘ & ⇙ & \Lleftarrow & \Rrightarrow & ⇜ & \rightsquigarrow & . & . \\
\text{21E0} & ⇠ & . & ⇢ & . & ⇤ & ⇥ & . & . & . & . & . & . & . & . & . & . \\
\text{21F0} & . & . & . & . & . & ⇵ & . & . & . & . & . & . & . & ⇽ & ⇾ & ⇿ \\
\text{2200} & \forall & \complement & \partial & \exists & \nexists & \varnothing & \mathrm{\Delta} & \nabla & \in & \notin & \in & \ni & ∌ & \ni & \blacksquare & \prod \\
\text{2210} & \coprod & \sum & - & \mp & \dotplus & \slash & \smallsetminus & \ast & \circ & \bullet & \sqrt{} & \sqrt[3]{} & \sqrt[4]{} & \propto & \infty & ∟ \\
\text{2220} & \angle & \measuredangle & \sphericalangle & \mid & \nmid & \parallel & \nparallel & \land & \vee & \cap & \cup & \int & \iint & \iiint & \oint & ∯ \\
\text{2230} & ∰ & ∱ & ∲ & ∳ & \therefore & \because & : & :: & ∸ & -: & ∺ & ∻ & \sim & \backsim & ∾ & ∾ \\
\text{2240} & \wr & \nsim & \eqsim & \simeq & ≄ & \cong & ≆ & \ncong & \approx & ≉ & \approxeq & ≋ & ≌ & \asymp & \Bumpeq & \bumpeq \\
\text{2250} & \doteq & \Doteq & \fallingdotseq & \risingdotseq & ≔ & ≕ & \eqcirc & \circeq & ≘ & ≙ & ≚ & ≛ & \triangleq & ≝ & ≞ & ≟ \\
\text{2260} & \neq & \equiv & ≢ & ≣ & \leq & \geq & \leqq & \geqq & \lneqq & \gneqq & \ll & \gg & \between & ≭ & \nless & \ngtr \\
\text{2270} & \nleq & \ngeq & \lesssim & \gtrsim & ≴ & ≵ & \lessgtr & \gtrless & ≸ & ≹ & \prec & \succ & \preccurlyeq & \succcurlyeq & \precsim & \succsim \\
\text{2280} & \nprec & \nsucc & \subset & \supset & ⊄ & ⊅ & \subseteq & \supseteq & \nsubseteq & \nsupseteq & \subsetneq & \supsetneq & ⊌ & ⊍ & \uplus & ⊏ \\
\text{2290} & ⊐ & \sqsubseteq & \sqsupseteq & \sqcap & \sqcup & \oplus & \ominus & \otimes & \oslash & \odot & \circledcirc & \circledast & ⊜ & \circleddash & \boxplus & \boxminus \\
\text{22a0} & \boxtimes & \boxdot & \vdash & \dashv & \top & \bot & \vdash & \models & \vDash & \Vdash & \Vvdash & ⊫ & \nvdash & \nvDash & \nVdash & \nVDash \\
\text{22b0} & ⊰ & ⊱ & \vartriangleleft & \vartriangleright & \trianglelefteq & \trianglerighteq & ⊶ & ⊷ & \multimap & ⊹ & \intercal & \veebar & \barwedge & ⊽ & ⊾ & ⊿ \\
\text{22c0} & \bigwedge & \bigvee & \bigcap & \bigcup & \diamond & \cdot & \star & \divideontimes & \bowtie & \ltimes & \rtimes & \leftthreetimes & \rightthreetimes & \backsimeq & \curlyvee & \curlywedge \\
\text{22d0} & \Subset & \Supset & \Cap & \Cup & \pitchfork & ⋕ & \lessdot & \gtrdot & \lll & \ggg & \lesseqgtr & \gtreqless & ⋜ & ⋝ & \curlyeqprec & \curlyeqsucc \\
\text{22e0} & \npreceq & \nsucceq & ⋢ & ⋣ & ⋤ & ⋥ & \lnsim & \gnsim & \precnsim & \succnsim & \ntriangleleft & \ntriangleright & \ntrianglelefteq & \ntrianglerighteq & \vdots & \cdots \\
\text{22f0} & ⋰ & \ddots & ⋲ & ⋳ & ⋴ & ⋵ & ⋶ & ⋷ & ⋸ & ⋹ & ⋺ & ⋻ & ⋼ & ⋽ & ⋾ & \mathsf{E} \\
\text{2300} & \varnothing & ⌁ & ⌂ & ⌃ & ⌄ & \barwedge & \doublebarwedge & ⌇ & \lceil & \rceil & \lfloor & \rfloor & ⌌ & ⌍ & ⌎ & ⌏ \\
\text{2310} & ⌐ & ⌑ & ⌒ & ⌓ & ⌔ & ⌕ & ⌖ & ⌗ & ⌘ & ⌙ & ⌚ & ⌛ & ⌜ & ⌝ & ⌞ & ⌟ \\
\text{2320} & ⌠ & ⌡ & \frown & \smile & ⌤ & ⌥ & ⌦ & ⌧ & ⌨ & \langle & \rangle & ⌫ & ⌬ & ⌭ & ⌮ & ⌯ \\
\text{2330} & & & & & & & ⌶ & & & ⌹ & & & & ⌽ & & ⌿ \\
\text{2340} & ⍀ & & & & & & & ⍇ & ⍈ & & & & & & & \\
\text{2350} & ⍐ & & & & & & & ⍗ & & & & & & & & \\
\text{2600} & & ☁ & & & & \bigstar & ☆ & & & & & & ☌ & {☍☽} & ☾ & ☿ \\
\text{2640} & ♀ & ♁ & ♂ & ♃ & ♄ & ♅ & ♆ & ♇ & ♈ & ♉ & ♊ & ♋ & ♌ & ♍ & ♎ & ♏ \\
\text{2650} & ♐ & ♑ & ♒ & ♓ & ♔ & ♕ & ♖ & ♗ & ♘ & ♙ & ♚ & ♛ & ♜ & ♝ & ♞ & ♟ \\
\text{2660} & \spadesuit & \heartsuit & \diamondsuit & \clubsuit & ♤ & ♥ & ♦ & ♧ & & ♩ & ♪ & & & \flat & \natural & \sharp \\
\text{2680} & ⚀ & ⚁ & ⚂ & ⚃ & ⚄ & ⚅ & & & & & & & & & & \\
\end{array}`
