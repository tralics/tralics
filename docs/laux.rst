================================================
XML source and translation of some math commands
================================================

\\le
----

*Tralics* recognizes the following relations:

.. container:: ltx-source

   ::

      $\le \leq \succ \simeq \parallel \subseteq \sqsubset \doteq
      \ge \geq \sim \mid \subset \subseteq \ne \neq \frown \vdash
      \equiv \perp \ll \supset \cong \smile \in \dashv 
      \models \preceq \gg \approx \Join \sqsubseteq \ni
      \prec \succeq \asymp \bowtie \sqsupseteq \propto$

The XML result is

.. container:: xml_out

   ::

      <formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      <mrow><mo>&le;</mo> <mo>&le;</mo> <mo>&succ;</mo> <mo>&simeq;</mo>
      <mo>&parallel;</mo> <mo>&subseteq;</mo> <mo>&sqsubset;</mo> <mo>&doteq;</mo>
      <mo>&ge;</mo> <mo>&ge;</mo> <mo>&sim;</mo> <mo>&mid;</mo>
      <mo>&subset;</mo> <mo>&subseteq;</mo> <mo>&ne;</mo> <mo>&ne;</mo>
      <mo>&frown;</mo> <mo>&vdash;</mo> <mo>&equiv;</mo> <mo>&perp;</mo>
      <mo>&ll;</mo> <mo>&supset;</mo> <mo>&cong;</mo> <mo>&smile;</mo>
      <mo>&Element;</mo><mo> &dashv;</mo> <mo>&models;</mo> <mo>&preceq;</mo> 
      <mo>&gg;</mo> <mo>&approx;</mo> <mo>&bowtie;</mo> <mo>&sqsubseteq;</mo>
      <mo>&ni;</mo> <mo>&prec;</mo> <mo>&succeq;</mo> <mo>&asymp;</mo>
      <mo>&bowtie;</mo> <mo>&sqsupseteq;</mo> <mo>&propto;</mo></mrow></math></formula>

If you do not like entity names, you can use the ``-noentnames`` switch,
and you get

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
         <mrow><mo>&le;</mo><mo>&le;</mo><mo>&#x0227B;</mo><mo>&#x02243;</mo>
         <mo>&#x02225;</mo><mo>&#x02286;</mo><mo>&#x0228F;</mo><mo>&#x02250;</mo>
         <mo>&ge;</mo><mo>&ge;</mo><mo>&#x0223C;</mo><mo>&#x02223;</mo>
         <mo>&#x02282;</mo><mo>&#x02286;</mo><mo>&#x02260;</mo><mo>&#x02260;</mo>
         <mo>&#x02322;</mo><mo>&#x022A2;</mo><mo>&#x02261;</mo><mo>&#x022A5;</mo>
         <mo>&#x0226A;</mo><mo>&#x02283;</mo><mo>&#x02245;</mo><mo>&#x02323;</mo>
         <mo>&#x02208;</mo><mo>&#x022A3;</mo><mo>&#x022A7;</mo><mo>&#x02AAF;</mo>
         <mo>&#x0226B;</mo><mo>&#x02248;</mo><mo>&#x022C8;</mo><mo>&#x02291;</mo>
         <mo>&#x0220B;</mo><mo>&#x0227A;</mo><mo>&#x02AB0;</mo><mo>&#x02248;</mo>
         <mo>&#x022C8;</mo><mo>&#x02292;</mo><mo>&#x0221D;</mo></mrow>
        </math>
      </formula>

All these operators are from Table 8.5 of the `Latex
Companion <index.html#companion>`__. |relations operators from table
8.5|

\\leftarrow
-----------

Arrows:

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

The XML result is

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mrow>
         <mo>&leftarrow;</mo><mo>&longleftarrow;</mo><mo>&Leftarrow;</mo><mo>&Longleftarrow;</mo>
         <mo>&rightarrow;</mo><mo>&longrightarrow;</mo><mo>&Rightarrow;</mo><mo>&Longrightarrow;</mo>
         <mo>&leftrightarrow;</mo><mo>&Leftrightarrow;</mo><mo>&longleftrightarrow;</mo><mo>&Longleftrightarrow;</mo>
         <mo>&mapsto;</mo><mo>&longmapsto;</mo>
         <mo>&hookleftarrow;</mo><mo>&hookrightarrow;</mo>
         <mo>&leftharpoondown;</mo><mo>&leftharpoonup;</mo><mo>&rightharpoondown;</mo><mo>&rightharpoonup;</mo>
         <mo>&uparrow;</mo><mo>&Uparrow;</mo><mo>&updownarrow;</mo><mo>&Updownarrow;</mo><mo>&downarrow;</mo><mo>&Downarrow;</mo>
         <mo>&nearrow;</mo><mo>&searrow;</mo><mo>&swarrow;</mo><mo>&nwarrow;</mo>
        </mrow>
       </math>
      </formula>

If you do not like entity names, you can use the ``-noentnames`` switch,
and you get

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mrow><mo>&#x02190;</mo><mo>&#x027F5;</mo><mo>&#x021D0;</mo><mo>&#x027F8;</mo>
        <mo>&#x02192;</mo><mo>&#x027F6;</mo><mo>&#x021D2;</mo><mo>&#x027F9;</mo>
        <mo>&#x02194;</mo><mo>&#x021D4;</mo><mo>&#x027F7;</mo><mo>&#x027FA;</mo>
        <mo>&#x021A6;</mo><mo>&#x027FC;</mo><mo>&#x021A9;</mo><mo>&#x021AA;</mo>
        <mo>&#x021BD;</mo><mo>&#x021BC;</mo><mo>&#x021C1;</mo><mo>&#x021C0;</mo>
        <mo>&#x02191;</mo><mo>&#x021D1;</mo><mo>&#x02195;</mo><mo>&#x021D5;</mo>
        <mo>&#x02193;</mo><mo>&#x021D3;</mo><mo>&#x02197;</mo><mo>&#x02198;</mo>
        <mo>&#x02199;</mo><mo>&#x02196;</mo></mrow>
       </math>
      </formula>

All these operators are from Table 8.6 of the `Latex
Companion <index.html#companion>`__. |arrows from table 8.6|

\\log
-----

The ``\log`` command is valid only in math mode. Its translation is a
math operator of the same name ``<mo form='prefix'>log</mo>``. Here is
the list of all log-like operators:

.. container:: ltx-source

   ::

      $$\arccos^2 (x),\, \arcsin^2(x),\,  \arctan^2(x),\,  \arg^2(x),\, \cos^2(x),\,  \cosh^2(x)  ,\, \cot^2(x)$$
      $$\coth^2(x),\, \csc^2(x),\,   \deg^2(x),\,   \dim^2(x),\, \exp^2(x),\, \hom^2(x),\,  \ker^2(x) ,\, \lg^2(x)$$
      $$\ln^2(x),\, \log^2(x),\,  \Pr^2(x),\, \sec^2(x),\, \sin^2(x) ,\,\sinh^2(x),\,  \tan^2(x),\, \tanh^2(x) $$
      $$ %These are declared m
      \det_{x=1},\, \gcd_{x=1},\, \inf_{x=1},\, \injlim_{x=1},\, \lim_{x=1},\, 
      \liminf_{x=1},\,\limsup_{x=1},\,\max_{x=1},\, \min_{x=1},\, \projlim_{x=1},\,
      \sup_{x=1}
      $$

The XML translation is the following. Note that some operators have the
attribute ``movablelimits='true'``. These are declared \`m' in the AMS
tables, and subscripts/superscripts should use ``<munderover>`` instead
of ``<msubsup>``. Our renderer understands this syntax, which is not
really MathML conformant, in the same way as L\ :sup:`A`\ T\ :sub:`E`\ X
(except for the space in an operator like ``\liminf``).

.. container:: xml_out

   ::

      <formula type='display'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <msup><mo form='prefix'>arccos</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>arcsin</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>arctan</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>arg</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>cos</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>cosh</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
         <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>cot</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo></mrow>
         </mrow></math></formula>

      <formula type='display'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <msup><mo form='prefix'>coth</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>csc</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>deg</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>dim</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>exp</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>hom</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>ker</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>lg</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo></mrow>
        </mrow></math></formula>

      <formula type='display'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <msup><mo form='prefix'>ln</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow> 
       <msup><mo form='prefix'>log</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>Pr</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>sec</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>sin</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>sinh</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>tan</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo>
          <mo>,</mo><mspace width='0.166667em'/></mrow>
       <msup><mo form='prefix'>tanh</mo> <mn>2</mn> </msup><mrow><mo>(</mo><mi>x</mi><mo>)</mo></mrow>
        </mrow></math></formula>

      <formula type='display'><math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
       <msub><mo movablelimits='true' form='prefix'>det</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>gcd</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>inf</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>inj lim</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>lim</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>lim inf</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>lim sup</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>max</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>min</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>proj lim</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub>
         <mo>,</mo><mspace width='0.166667em'/>
       <msub><mo movablelimits='true' form='prefix'>sup</mo> <mrow><mi>x</mi><mo>=</mo><mn>1</mn></mrow> </msub></mrow>
      </math></formula>

| All commands are listed in Table 8.9 of the `Latex
  Companion <index.html#companion>`__.
| |log-like operators| |log-like operators2| |log-like operators3|
  |log-like operators4|

\\ldots
-------

*Tralics* recognizes the following miscellaneous symbols.

.. container:: ltx-source

   ::

      $\ldots \prime \exists \Diamond \top \bot \mho 
      \cdots \forall \nabla \imath \flat \clubsuit \Re
      \vdots \infty \surd \jmath \natural \diamondsuit \Im
      \ddots \hbar \square \ell \sharp \heartsuit \angle
      \aleph \emptyset \triangle \neg \wp \spadesuit \partial$

The XML result is

.. container:: xml_out

   ::

      <formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      <mrow><mo>...</mo> <mo>&prime;</mo> <mo>&exist;</mo> <mo>&diamond;</mo> <mi>&top;</mi> 
      <mi>&bottom;</mi> <mo>&mho;</mo> <mo>&ctdot;</mo> <mo>&forall;</mo> 
      <mi>&nabla;</mi> <mo>&imath;</mo> <mo>&flat;</mo> <mo>&clubsuit;</mo>
      <mi>&Re;</mi> <mo>&vellip;</mo> <mi>&infin;</mi> <mi>&radic;</mi> 
      <mo>&jmath;</mo> <mo>&natur;</mo> <mo>&diamondsuit;</mo> <mi>&Im;</mi> 
      <mo>&dtdot;</mo> <mi>&hbar;</mi> <mo>&square;</mo> <mi>&ell;</mi> 
      <mo>&sharp;</mo> <mo>&heartsuit;</mo> <mi>&ang;</mi> <mo>&aleph;</mo> 
      <mi>&emptyset;</mi> <mi>&triangle;</mi> <mo>&not;</mo> <mi>&wp;</mi> 
      <mo>&spadesuit;</mo> <mi>&part;</mi></mrow></math></formula>

If you do not like entity names, you can use the ``-noentnames`` switch,
and you get

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mrow><mo>...</mo><mo>&#x02032;</mo><mo>&#x02203;</mo><mo>&#x022C4;</mo>
        <mi>&#x022A4;</mi><mi>&#x022A5;</mi> <mo>&#x02127;</mo><mo>&#x022EF;</mo>
        <mo>&#x02200;</mo><mi>&#x02207;</mi><mo>&#x00131;</mo>  <mo>&#x0266D;</mo>
        <mo>&#x02663;</mo><mi>&#x0211C;</mi><mo>&#x022EE;</mo><mi>&#x0221E;</mi>
        <mi>&#x0221A;</mi><mo>&#x0006A;</mo>  <mo>&#x0266E;</mo><mo>&#x02666;</mo>
        <mi>&#x02111;</mi><mo>&#x022F1;</mo><mi>&#x0210F;</mi><mo>&#x025A1;</mo>
        <mi>&#x02113;</mi><mo>&#x0266F;</mo><mo>&#x02665;</mo><mi>&#x02220;</mi>
        <mo>&#x02135;</mo><mi>&#x02205;</mi><mi>&#x025B5;</mi><mo>&#xAC;</mo>
        <mi>&#x02118;</mi><mo>&#x02660;</mo><mi>&#x02202;</mi></mrow>
       </math>
      </formula>

All these operators are from Table 8.7 of the `Latex
Companion <index.html#companion>`__. The ``\Box`` command is not yet
implemented (but ``\square`` produces the same effect. |operators of
table 8.7|

Math fonts
----------

Example:

.. container:: ltx-source

   ::

      \def\test#1#2{\mathtt{#1#2}\mathcal{#1}\mathbf{#1#2}\mathrm{#1#2}
      \mathit{#1#2}\mathbb{#1}\mathsf{#1#2}\mathnormal{#1#2}}
      \begin{equation}\begin{split}
      \test Aa &\test Bb\\
      \test Cc &\test Dd\\
      \test Ee &\test Ff\\
      \test Gg &\test Hh\\
      \test Ii &\test Jj\\
      \test Kk &\test Ll\\
      \test Mm &\test Nn\\
      \test Oo &\test Pp\\
      \test Qq &\test Rr\\
      \test Ss &\test Tt\\
      \test Uu &\test Vv\\
      \test Ww &\test Xx\\
      \test Yy &\test Zz\\
      \end{split}
      \end{equation}

Translation (a bit longish)

.. container:: xml_out

   ::

      <formula type='display'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mtable>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>A</mi>
            <mi mathvariant='monospace'>a</mi>
            <mi>&Ascr;</mi>
            <mi mathvariant='bold'>A</mi>
            <mi mathvariant='bold'>a</mi>
            <mi> A </mi>
            <mi> a </mi>
            <mi>A</mi>
            <mi>a</mi>
            <mi>&Aopf;</mi>
            <mi mathvariant='sans-serif'>A</mi>
            <mi mathvariant='sans-serif'>a</mi>
            <mi>A</mi>
            <mi>a</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>B</mi>
            <mi mathvariant='monospace'>b</mi>
            <mi>&Bscr;</mi>
            <mi mathvariant='bold'>B</mi>
            <mi mathvariant='bold'>b</mi>
            <mi> B </mi>
            <mi> b </mi>
            <mi>B</mi>
            <mi>b</mi>
            <mi>&Bopf;</mi>
            <mi mathvariant='sans-serif'>B</mi>
            <mi mathvariant='sans-serif'>b</mi>
            <mi>B</mi>
            <mi>b</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>C</mi>
            <mi mathvariant='monospace'>c</mi>
            <mi>&Cscr;</mi>
            <mi mathvariant='bold'>C</mi>
            <mi mathvariant='bold'>c</mi>
            <mi> C </mi>
            <mi> c </mi>
            <mi>C</mi>
            <mi>c</mi>
            <mi>&Copf;</mi>
            <mi mathvariant='sans-serif'>C</mi>
            <mi mathvariant='sans-serif'>c</mi>
            <mi>C</mi>
            <mi>c</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>D</mi>
            <mi mathvariant='monospace'>d</mi>
            <mi>&Dscr;</mi>
            <mi mathvariant='bold'>D</mi>
            <mi mathvariant='bold'>d</mi>
            <mi> D </mi>
            <mi> d </mi>
            <mi>D</mi>
            <mi>d</mi>
            <mi>&Dopf;</mi>
            <mi mathvariant='sans-serif'>D</mi>
            <mi mathvariant='sans-serif'>d</mi>
            <mi>D</mi>
            <mi>d</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>E</mi>
            <mi mathvariant='monospace'>e</mi>
            <mi>&Escr;</mi>
            <mi mathvariant='bold'>E</mi>
            <mi mathvariant='bold'>e</mi>
            <mi> E </mi>
            <mi> e </mi>
            <mi>E</mi>
            <mi>e</mi>
            <mi>&Eopf;</mi>
            <mi mathvariant='sans-serif'>E</mi>
            <mi mathvariant='sans-serif'>e</mi>
            <mi>E</mi>
            <mi>e</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>F</mi>
            <mi mathvariant='monospace'>f</mi>
            <mi>&Fscr;</mi>
            <mi mathvariant='bold'>F</mi>
            <mi mathvariant='bold'>f</mi>
            <mi> F </mi>
            <mi> f </mi>
            <mi>F</mi>
            <mi>f</mi>
            <mi>&Fopf;</mi>
            <mi mathvariant='sans-serif'>F</mi>
            <mi mathvariant='sans-serif'>f</mi>
            <mi>F</mi>
            <mi>f</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>G</mi>
            <mi mathvariant='monospace'>g</mi>
            <mi>&Gscr;</mi>
            <mi mathvariant='bold'>G</mi>
            <mi mathvariant='bold'>g</mi>
            <mi> G </mi>
            <mi> g </mi>
            <mi>G</mi>
            <mi>g</mi>
            <mi>&Gopf;</mi>
            <mi mathvariant='sans-serif'>G</mi>
            <mi mathvariant='sans-serif'>g</mi>
            <mi>G</mi>
            <mi>g</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>H</mi>
            <mi mathvariant='monospace'>h</mi>
            <mi>&Hscr;</mi>
            <mi mathvariant='bold'>H</mi>
            <mi mathvariant='bold'>h</mi>
            <mi> H </mi>
            <mi> h </mi>
            <mi>H</mi>
            <mi>h</mi>
            <mi>&Hopf;</mi>
            <mi mathvariant='sans-serif'>H</mi>
            <mi mathvariant='sans-serif'>h</mi>
            <mi>H</mi>
            <mi>h</mi> 
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>I</mi>
            <mi mathvariant='monospace'>i</mi>
            <mi>&Iscr;</mi>
            <mi mathvariant='bold'>I</mi>
            <mi mathvariant='bold'>i</mi>
            <mi> I </mi>
            <mi> i </mi>
            <mi>I</mi>
            <mi>i</mi>
            <mi>&Iopf;</mi>
            <mi mathvariant='sans-serif'>I</mi>
            <mi mathvariant='sans-serif'>i</mi>
            <mi>I</mi>
            <mi>i</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>J</mi>
            <mi mathvariant='monospace'>j</mi>
            <mi>&Jscr;</mi>
            <mi mathvariant='bold'>J</mi>
            <mi mathvariant='bold'>j</mi>
            <mi> J </mi>
            <mi> j </mi>
            <mi>J</mi>
            <mi>j</mi>
            <mi>&Jopf;</mi>
            <mi mathvariant='sans-serif'>J</mi>
            <mi mathvariant='sans-serif'>j</mi>
            <mi>J</mi>
            <mi>j</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>K</mi>
            <mi mathvariant='monospace'>k</mi>
            <mi>&Kscr;</mi>
            <mi mathvariant='bold'>K</mi>
            <mi mathvariant='bold'>k</mi>
            <mi> K </mi>
            <mi> k </mi>
            <mi>K</mi>
            <mi>k</mi>
            <mi>&Kopf;</mi>
            <mi mathvariant='sans-serif'>K</mi>
            <mi mathvariant='sans-serif'>k</mi>
            <mi>K</mi>
            <mi>k</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>L</mi>
            <mi mathvariant='monospace'>l</mi>
            <mi>&Lscr;</mi>
            <mi mathvariant='bold'>L</mi>
            <mi mathvariant='bold'>l</mi>
            <mi> L </mi>
            <mi> l </mi>
            <mi>L</mi>
            <mi>l</mi>
            <mi>&Lopf;</mi>
            <mi mathvariant='sans-serif'>L</mi>
            <mi mathvariant='sans-serif'>l</mi>
            <mi>L</mi>
            <mi>l</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>M</mi>
            <mi mathvariant='monospace'>m</mi>
            <mi>&Mscr;</mi>
            <mi mathvariant='bold'>M</mi>
            <mi mathvariant='bold'>m</mi>
            <mi> M </mi>
            <mi> m </mi>
            <mi>M</mi>
            <mi>m</mi>
            <mi>&Mopf;</mi>
            <mi mathvariant='sans-serif'>M</mi>
            <mi mathvariant='sans-serif'>m</mi>
            <mi>M</mi>
            <mi>m</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>N</mi>
            <mi mathvariant='monospace'>n</mi>
            <mi>&Nscr;</mi>
            <mi mathvariant='bold'>N</mi>
            <mi mathvariant='bold'>n</mi>
            <mi> N </mi>
            <mi> n </mi>
            <mi>N</mi>
            <mi>n</mi>
            <mi>&Nopf;</mi>
            <mi mathvariant='sans-serif'>N</mi>
            <mi mathvariant='sans-serif'>n</mi>
            <mi>N</mi>
            <mi>n</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>O</mi>
            <mi mathvariant='monospace'>o</mi>
            <mi>&Oscr;</mi>
            <mi mathvariant='bold'>O</mi>
            <mi mathvariant='bold'>o</mi>
            <mi> O </mi>
            <mi> o </mi>
            <mi>O</mi>
            <mi>o</mi>
            <mi>&Oopf;</mi>
            <mi mathvariant='sans-serif'>O</mi>
            <mi mathvariant='sans-serif'>o</mi>
            <mi>O</mi>
            <mi>o</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>P</mi>
            <mi mathvariant='monospace'>p</mi>
            <mi>&Pscr;</mi>
            <mi mathvariant='bold'>P</mi>
            <mi mathvariant='bold'>p</mi>
            <mi> P </mi>
            <mi> p </mi>
            <mi>P</mi>
            <mi>p</mi>
            <mi>&Popf;</mi>
            <mi mathvariant='sans-serif'>P</mi>
            <mi mathvariant='sans-serif'>p</mi>
            <mi>P</mi>
            <mi>p</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>Q</mi>
            <mi mathvariant='monospace'>q</mi>
            <mi>&Qscr;</mi>
            <mi mathvariant='bold'>Q</mi>
            <mi mathvariant='bold'>q</mi>
            <mi> Q </mi>
            <mi> q </mi>
            <mi>Q</mi>
            <mi>q</mi>
            <mi>&Qopf;</mi>
            <mi mathvariant='sans-serif'>Q</mi>
            <mi mathvariant='sans-serif'>q</mi>
            <mi>Q</mi>
            <mi>q</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>R</mi>
            <mi mathvariant='monospace'>r</mi>
            <mi>&Rscr;</mi>
            <mi mathvariant='bold'>R</mi>
            <mi mathvariant='bold'>r</mi>
            <mi> R </mi>
            <mi> r </mi>
            <mi>R</mi>
            <mi>r</mi>
            <mi>&Ropf;</mi>
            <mi mathvariant='sans-serif'>R</mi>
            <mi mathvariant='sans-serif'>r</mi>
            <mi>R</mi>
            <mi>r</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>S</mi>
            <mi mathvariant='monospace'>s</mi>
            <mi>&Sscr;</mi>
            <mi mathvariant='bold'>S</mi>
            <mi mathvariant='bold'>s</mi>
            <mi> S </mi>
            <mi> s </mi>
            <mi>S</mi>
            <mi>s</mi>
            <mi>&Sopf;</mi>
            <mi mathvariant='sans-serif'>S</mi>
            <mi mathvariant='sans-serif'>s</mi>
            <mi>S</mi>
            <mi>s</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>T</mi>
            <mi mathvariant='monospace'>t</mi>
            <mi>&Tscr;</mi>
            <mi mathvariant='bold'>T</mi>
            <mi mathvariant='bold'>t</mi>
            <mi> T </mi>
            <mi> t </mi>
            <mi>T</mi>
            <mi>t</mi>
            <mi>&Topf;</mi>
            <mi mathvariant='sans-serif'>T</mi>
            <mi mathvariant='sans-serif'>t</mi>
            <mi>T</mi>
            <mi>t</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>U</mi>
            <mi mathvariant='monospace'>u</mi>
            <mi>&Uscr;</mi>
            <mi mathvariant='bold'>U</mi>
            <mi mathvariant='bold'>u</mi>
            <mi> U </mi>
            <mi> u </mi>
            <mi>U</mi>
            <mi>u</mi>
            <mi>&Uopf;</mi>
            <mi mathvariant='sans-serif'>U</mi>
            <mi mathvariant='sans-serif'>u</mi>
            <mi>U</mi>
            <mi>u</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>V</mi>
            <mi mathvariant='monospace'>v</mi>
            <mi>&Vscr;</mi>
            <mi mathvariant='bold'>V</mi>
            <mi mathvariant='bold'>v</mi>
            <mi> V </mi>
            <mi> v </mi>
            <mi>V</mi>
            <mi>v</mi>
            <mi>&Vopf;</mi>
            <mi mathvariant='sans-serif'>V</mi>
            <mi mathvariant='sans-serif'>v</mi>
            <mi>V</mi>
            <mi>v</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>W</mi>
            <mi mathvariant='monospace'>w</mi>
            <mi>&Wscr;</mi>
            <mi mathvariant='bold'>W</mi>
            <mi mathvariant='bold'>w</mi>
            <mi> W </mi>
            <mi> w </mi>
            <mi>W</mi>
            <mi>w</mi>
            <mi>&Wopf;</mi>
            <mi mathvariant='sans-serif'>W</mi>
            <mi mathvariant='sans-serif'>w</mi>
            <mi>W</mi>
            <mi>w</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>X</mi>
            <mi mathvariant='monospace'>x</mi>
            <mi>&Xscr;</mi>
            <mi mathvariant='bold'>X</mi>
            <mi mathvariant='bold'>x</mi>
            <mi> X </mi>
            <mi> x </mi>
            <mi>X</mi>
            <mi>x</mi>
            <mi>&Xopf;</mi>
            <mi mathvariant='sans-serif'>X</mi>
            <mi mathvariant='sans-serif'>x</mi>
            <mi>X</mi>
            <mi>x</mi>
           </mrow>
          </mtd>
         </mtr>
         <mtr>
          <mtd columnalign='right'>
           <mrow>
            <mi mathvariant='monospace'>Y</mi>
            <mi mathvariant='monospace'>y</mi>
            <mi>&Yscr;</mi>
            <mi mathvariant='bold'>Y</mi>
            <mi mathvariant='bold'>y</mi>
            <mi> Y </mi>
            <mi> y </mi>
            <mi>Y</mi>
            <mi>y</mi>
            <mi>&Yopf;</mi>
            <mi mathvariant='sans-serif'>Y</mi>
            <mi mathvariant='sans-serif'>y</mi>
            <mi>Y</mi>
            <mi>y</mi>
           </mrow>
          </mtd>
          <mtd columnalign='left'>
           <mrow>
            <mi mathvariant='monospace'>Z</mi>
            <mi mathvariant='monospace'>z</mi>
            <mi>&Zscr;</mi>
            <mi mathvariant='bold'>Z</mi>
            <mi mathvariant='bold'>z</mi>
            <mi> Z </mi>
            <mi> z </mi>
            <mi>Z</mi>
            <mi>z</mi>
            <mi>&Zopf;</mi>
            <mi mathvariant='sans-serif'>Z</mi>
            <mi mathvariant='sans-serif'>z</mi>
            <mi>Z</mi>
            <mi>z</mi>
           </mrow>
          </mtd>
         </mtr>
        </mtable>
       </math>
      </formula>

Preview |math fonts|

CENTER
------

.. container:: ltx-source

   ::

      {\centering a \indent a1 \noindent a2 \\ a3\par a4} \par
      {\raggedleft b \indent b1 \noindent b2 \\ b3\par b4} \par
      {\raggedright c \indent c1 \noindent c2 \\ c3\par c4} \par
      {\centering\raggedright d \indent d1 \noindent d2 \\ d3\par d4} \par
      {f\centering\raggedright e \indent e1 \noindent e2 \\ e3\par e4} \par
      \begin{center}A \indent A1 \noindent A2 \\ A3 \par A4\end{center}
      \begin{flushright}B \indent B1 \noindent B2 \\ B3 \par B4\end{flushright}
      \begin{flushleft}C \indent C1 \noindent C2 \\ C3\par C4\end{flushleft}
      \begin{center}\begin{flushleft}D \indent D1 \noindent D2 \\ D3\par D4\end{flushleft}\end{center}
      F\begin{center}\begin{flushleft} E \indent E1 \noindent E2 \\ E3 \par E4\end{flushleft}\end{center}
      \begin{quote}I \indent I1 \noindent I2 \\ I3 \par I4\end{quote}
      \begin{quotation}J \indent J1 \noindent J2 \\ J3 \par J4\end{quotation}

The translation is the following. There are two lines with
``\centering\raggedright``. In the first case, the command is seen in
vertical mode (outside paragraph), so that the letter d will be in a new
paragraph, a flushed-left one. In the second case, the command appears
in vertical mode, so that ``f\centering`` will center the f (and the e
that follows). The e1 is flushed-left since ``\\`` start a new
paragraph. On the other hand, D and E are flushed left, because the
environement insert an implicit ``\par`` (both letters are seen in
vertical mode).

.. container:: xml_out

   ::

      <p rend='center'>a</p>
      <p rend='center'>a1</p>
      <p rend='center'>a2</p>
      <p rend='center'>a3</p>
      <p rend='center'>a4</p>
      <p rend='flushed-right'>b</p>
      <p noindent='false' rend='flushed-right'>b1</p>
      <p noindent='true' rend='flushed-right'>b2</p>
      <p noindent='true' rend='flushed-right'>b3</p>
      <p rend='flushed-right'>b4</p>
      <p rend='flushed-left'>c</p>
      <p noindent='false' rend='flushed-left'>c1</p>
      <p noindent='true' rend='flushed-left'>c2</p>
      <p noindent='true' rend='flushed-left'>c3</p>
      <p rend='flushed-left'>c4</p>
      <p rend='flushed-left'>d</p>
      <p noindent='false' rend='flushed-left'>d1</p>
      <p noindent='true' rend='flushed-left'>d2</p>
      <p noindent='true' rend='flushed-left'>d3</p>
      <p rend='flushed-left'>d4</p>
      <p rend='center'>fe</p>
      <p noindent='false' rend='flushed-left'>e1</p>
      <p noindent='true' rend='flushed-left'>e2</p>
      <p noindent='true' rend='flushed-left'>e3</p>
      <p rend='flushed-left'>e4</p>
      <p rend='center'>A</p>
      <p rend='center'>A1</p>
      <p rend='center'>A2</p>
      <p rend='center'>A3</p>
      <p rend='center'>A4</p>
      <p rend='flushed-right'>B</p>
      <p noindent='false' rend='flushed-right'>B1</p>
      <p noindent='true' rend='flushed-right'>B2</p>
      <p noindent='true' rend='flushed-right'>B3</p>
      <p rend='flushed-right'>B4</p>
      <p rend='flushed-left'>C</p>
      <p noindent='false' rend='flushed-left'>C1</p>
      <p noindent='true' rend='flushed-left'>C2</p>
      <p noindent='true' rend='flushed-left'>C3</p>
      <p rend='flushed-left'>C4</p>
      <p rend='flushed-left'>D</p>
      <p noindent='false' rend='flushed-left'>D1</p>
      <p noindent='true' rend='flushed-left'>D2</p>
      <p noindent='true' rend='flushed-left'>D3</p>
      <p rend='flushed-left'>D4</p>
      <p>F</p>
      <p rend='flushed-left'>E</p>
      <p noindent='false' rend='flushed-left'>E1</p>
      <p noindent='true' rend='flushed-left'>E2</p>
      <p noindent='true' rend='flushed-left'>E3</p>
      <p rend='flushed-left'>E4</p>
      <p rend='quoted'>I</p>
      <p noindent='false' rend='quoted'>I1</p>
      <p noindent='true' rend='quoted'>I2</p>
      <p noindent='true' rend='quoted'>I3</p>
      <p rend='quoted'>I4</p>
      <p rend='quoted'>J</p>
      <p noindent='false' rend='quoted'>J1</p>
      <p noindent='true' rend='quoted'>J2</p>
      <p noindent='true' rend='quoted'>J3</p>
      <p rend='quoted'>J4</p>

The names in the example above are not hard coded. If the configuration
file contains the following lines,

.. container:: ltx-source

   ::

        att_rend = "Rend"
        att_centering = "Center"
        att_flush_left = "FlushLeft"
        att_flush_right = "FlushRight"
        att_quote = "Quote"
        att_quotation = "Quotation"
        att_noindent = "Noindent"

then the translation will be

.. container:: xml_out

   ::

      <p Rend='Center'>a</p>
      <p Rend='Center'>a1</p>
      <p Rend='Center'>a2</p>
      <p Rend='Center'>a3</p>
      <p Rend='Center'>a4</p>
      <p Rend='FlushRight'>b</p>
      <p Noindent='false' Rend='FlushRight'>b1</p>
      <p Noindent='true' Rend='FlushRight'>b2</p>
      <p Noindent='true' Rend='FlushRight'>b3</p>
      <p Rend='FlushRight'>b4</p>
      <p Rend='FlushLeft'>c</p>
      <p Noindent='false' Rend='FlushLeft'>c1</p>
      <p Noindent='true' Rend='FlushLeft'>c2</p>
      <p Noindent='true' Rend='FlushLeft'>c3</p>
      <p Rend='FlushLeft'>c4</p>
      <p Rend='FlushLeft'>d</p>
      <p Noindent='false' Rend='FlushLeft'>d1</p>
      <p Noindent='true' Rend='FlushLeft'>d2</p>
      <p Noindent='true' Rend='FlushLeft'>d3</p>
      <p Rend='FlushLeft'>d4</p>
      <p Rend='Center'>fe</p>
      <p Noindent='false' Rend='FlushLeft'>e1</p>
      <p Noindent='true' Rend='FlushLeft'>e2</p>
      <p Noindent='true' Rend='FlushLeft'>e3</p>
      <p Rend='FlushLeft'>e4</p>
      <p Rend='Center'>A</p>
      <p Rend='Center'>A1</p>
      <p Rend='Center'>A2</p>
      <p Rend='Center'>A3</p>
      <p Rend='Center'>A4</p>
      <p Rend='FlushRight'>B</p>
      <p Noindent='false' Rend='FlushRight'>B1</p>
      <p Noindent='true' Rend='FlushRight'>B2</p>
      <p Noindent='true' Rend='FlushRight'>B3</p>
      <p Rend='FlushRight'>B4</p>
      <p Rend='FlushLeft'>C</p>
      <p Noindent='false' Rend='FlushLeft'>C1</p>
      <p Noindent='true' Rend='FlushLeft'>C2</p>
      <p Noindent='true' Rend='FlushLeft'>C3</p>
      <p Rend='FlushLeft'>C4</p>
      <p Rend='FlushLeft'>D</p>
      <p Noindent='false' Rend='FlushLeft'>D1</p>
      <p Noindent='true' Rend='FlushLeft'>D2</p>
      <p Noindent='true' Rend='FlushLeft'>D3</p>
      <p Rend='FlushLeft'>D4</p>
      <p>F</p>
      <p Rend='FlushLeft'>E</p>
      <p Noindent='false' Rend='FlushLeft'>E1</p>
      <p Noindent='true' Rend='FlushLeft'>E2</p>
      <p Noindent='true' Rend='FlushLeft'>E3</p>
      <p Rend='FlushLeft'>E4</p>
      <p Rend='Quote'>I</p>
      <p Noindent='false' Rend='Quote'>I1</p>
      <p Noindent='true' Rend='Quote'>I2</p>
      <p Noindent='true' Rend='Quote'>I3</p>
      <p Rend='Quote'>I4</p>
      <p Rend='Quotation'>J</p>
      <p Noindent='false' Rend='Quotation'>J1</p>
      <p Noindent='true' Rend='Quotation'>J2</p>
      <p Noindent='true' Rend='Quotation'>J3</p>
      <p Rend='Quotation'>J4</p>

.. |relations operators from table 8.5| image:: /img/img_8.png
.. |arrows from table 8.6| image:: /img/img_9.png
.. |log-like operators| image:: /img/img_10.png
.. |log-like operators2| image:: /img/img_11.png
.. |log-like operators3| image:: /img/img_56.png
.. |log-like operators4| image:: /img/img_12.png
.. |operators of table 8.7| image:: /img/img_7.png
.. |math fonts| image:: /img/img_111.png
