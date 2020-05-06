Two examples for the Titlepage mechanism of Tralics
===================================================

First example. The source file is described `here <titlepage.html>`__.
This file was produced in Jan. 2004 Some line breaks were inserted, they
are marked by red underscores.

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE tpa SYSTEM 'tpa.dtd'>
      <!-- translated from latex by tralics 1.9t-->
      <tpa from_type='OK' from-tpa='ok' language='english'>
      <titlepage a2='b2' a1='b1' att2='foo2' att1='foo'>
      __<title-element tea='tea-val'>Titlepage customisation 
      __in Tralics</title-element>
      <utitle-element tea='utea-val'>No title</utitle-element>
      <autL bl='blval' al='alval'><aut auta='autaval'>Jos� Grimm</aut>
      <aut auta='autaval'>Knuth</aut>
      <aut auta='autaval'>JG</aut>
      <aut auta='autaval'>DEK</aut>
      </autL>
      <uautL bl='blval' al='alval'><aut auta='autaval'>No authors</aut></uautL>
      <abstract ab='AB1'><p>
      This is an abstract with </p>
      <p>some paragraphs in it</p>
      <p>ok ?
      </p></abstract>
      <abstract ab='AB2'>
      This is an abstract without paragraphs in it
      </abstract>
      <abstract ab='AB3'><p>Another abstract </p>
      <p>with </p>
      <p>in it</p></abstract>
      <abstract ab='AB4'>Another abstract without par</abstract>
      <uabstract ab='AB5'>no abstract1</uabstract>
      <uabstract ab='AB6'>no abstract2</uabstract>
      <uabstract ab='AB7'>no abstract3</uabstract>
      <uabstract ab='AB8'>no abstract4</uabstract>
      <UR><Rocq></Rocq>
      <URsop></URsop>
      </UR>
      <sUR en='research unit' fr='unit� de recherche'><Rocq en='nearparis'></Rocq>
      <sURsop fr='dans le sud'></sURsop>
      <sURlor fr=' dans l&apos;est'></sURlor>
      </sUR>
      <Address>somewhere</Address>
      <cmdp>nodefault</cmdp>
      <cmdA>CMDA</cmdA>
      <cmdB>CMDB</cmdB>
      <cmdC>CMDC</cmdC>
      </titlepage>
      <p></p></tpa>

First example, new syntax

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE tpa SYSTEM 'tpa.dtd'>
      <!-- translated from latex by tralics 2.8-->
      <tpa from_type='OK' from-tpa='ok'>
      <titlepage a2='b2' a1='b1' att2='foo2' att1='foo'><title-element tea='tea-val'>Titlepage customisation in Tralics</title-element>
      <utitle-element tea='utea-val'>No title</utitle-element>
      <autL bl='blval' al='alval'><aut auta='autaval'>Jos� Grimm</aut>
      <aut auta='autaval'>Knuth</aut>
      <aut auta='autaval'>JG</aut>
      <aut auta='autaval'>DEK</aut>
      </autL>
      <uautL bl='blval' al='alval'><aut auta='autaval'>No authors</aut></uautL>
      <abstract ab='AB1'><p>
      This is an abstract with</p>
      <p>some paragraphs in it</p>
      <p>ok ?
      </p></abstract>
      <abstract ab='AB2'>
      This is an abstract without paragraphs in it
      </abstract>
      <abstract ab='AB3'><p>Another abstract</p>
      <p>with</p>
      <p>in it</p></abstract>
      <abstract ab='AB4'>Another abstract without par</abstract>
      <uabstract ab='AB5'>no abstract1</uabstract>
      <uabstract ab='AB6'>no abstract2</uabstract>
      <uabstract ab='AB7'>no abstract3</uabstract>
      <uabstract ab='AB8'>no abstract4</uabstract>
      <UR>
      <Rocq/>
      <URsop/>
      </UR>
      <sUR en='research unit' fr='unit� de recherche'>
      <Rocq en='nearparis'/>
      <sURsop fr='dans le sud'/>
      <sURlor fr=' dans l&apos;est'/>
      </sUR>
      <Address>somewhere</Address>
      <cmdp>nodefault</cmdp>
      <cmdA>CMDA</cmdA>
      <cmdB>CMDB</cmdB>
      <cmdC>CMDC</cmdC>
      </titlepage>
      <p/>
      </tpa>

.. _ex2:

Cedram test 1
-------------

Second example file for the titlepage command. Some line breaks were
inserted, they are marked by red underscores.

.. container:: xml_out

   ::

      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE cedram SYSTEM 'cedram.dtd'>
      <!-- translated from latex by tralics 2.7c-->
      <cedram>
      <article><production>
      <fichier_tex>bo</fichier_tex>
      <fichier_bib>bo</fichier_bib>
      <date_prod>2006-6-16</date_prod></production>

      <date_reception>2004-06-14</date_reception>
      <date_acceptation>2004-12-09</date_acceptation>
      <auteur>
      <prenom>Donald</prenom>
      <middlename>E.</middlename>
      <nom><hi rend='it'>Knuth</hi></nom>
      <adresse><TeX/> Users Group  P.O. Box 869 Santa Barbara,
      __ CA 93102-0869 USA</adresse>
      <mel>d.e.knuth@somewhere.on.the.net</mel>
      </auteur>
      <titre xml:lang='fr'>Coefficients Fourier pour fonctions
      __ <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<msup><mi>L</mi> <mi>&infin;</mi> </msup></math></formula> simples
      __</titre>
      <TeXtitre xml:lang='fr'>Coefficients Fourier pour fonctions 
      __$L^\infty $ simple</TeXtitre>
      <titre xml:lang='en'>Fourier coefficients for simple
      __ <formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<msup><mi>L</mi> <mi>&infin;</mi> </msup>
      __</math></formula> functions</titre>
      <TeXtitre xml:lang='en'>Fourier coefficients for 
      __simple $L^\infty $ functions</TeXtitre>
      <langue>en</langue>
      <resume xml:lang='en'>This is an abstract with a beautiful inline 
      __formule <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><mrow>
      __<msub><mi>&lambda;</mi> <mi>n</mi> </msub>
      __<mrow><mo>(</mo><mi>&pi;</mi><mo>)</mo></mrow>
      __<mo>=</mo><mfrac><mi>N</mi> <mn>2</mn></mfrac><mi>n</mi>
      __<mo form='prefix'>log</mo><mi>n</mi><mo>+</mo><msub><mi>C</mi> 
      __<mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi>
      __<mo>)</mo><mi>n</mi><mo>+</mo><mi>O</mi>
      __<mo>(</mo></mrow><msqrt><mi>n</mi></msqrt><mrow>
      __<mo form='prefix'>log</mo><mi>n</mi><mo>)</mo></mrow>
      __</mrow></math></formula>, where <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<mrow><msub><mi>C</mi> 
      __<mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi>
      __<mo>)</mo></mrow></mrow></math></formula> 
      __is a real-valued constant.</resume>
      <TEXresume xml:lang='en'>This is an abstract with a beautiful inline 
      __formule $\lambda _n(\pi ) = \frac N2 n \log n + C_1(\pi ) n + 
      __O(\sqrt n\log n)$, where $C_1(\pi )$ 
      __is a real-valued constant.</TEXresume>
      <resume xml:lang='fr'>Mon r�sum� avec ma formule <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<mrow><msub><mi>&lambda;</mi> <mi>n</mi> </msub>
      __<mrow><mo>(</mo><mi>&pi;</mi><mo>)</mo></mrow>
      __<mo>=</mo><mfrac><mi>N</mi> <mn>2</mn></mfrac>
      __<mi>n</mi><mo form='prefix'>log</mo><mi>n</mi><mo>+</mo>
      __<msub><mi>C</mi> <mn>1</mn> </msub><mrow><mo>(</mo>
      __<mi>&pi;</mi><mo>)</mo><mi>n</mi><mo>+</mo><mi>O</mi>
      __<mo>(</mo></mrow><msqrt><mi>n</mi></msqrt><mrow>
      __<mo form='prefix'>log</mo><mi>n</mi><mo>)</mo></mrow></mrow>
      __</math></formula>, o� <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<mrow><msub><mi>C</mi> <mn>1</mn>
      __ </msub><mrow><mo>(</mo><mi>&pi;</mi><mo>)</mo>
      __</mrow></mrow></math></formula> est une constante r�elle.</resume>
      <TEXresume xml:lang='fr'>Mon r�sum� avec ma formule $\lambda _n(\pi ) 
      __= \frac N2 n \log n + C_1(\pi ) n + O(\sqrt n\log n)$, o� $C_1(\pi )$ 
      __est une constante r�elle.</TEXresume>
      <motcle xml:lang='fr'>fonctions <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><msup><mi>L</mi>
      __ <mi>&infin;</mi>
      </msup></math></formula>
      __ simples, fonction lambda</motcle>
      <motcle xml:lang='en'>simple <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><msup>
      __<mi>L</mi> <mi>&infin;</mi> </msup></math></formula> functions, lambda
      __ function</motcle>
      <msc>11M26, 11M36, 11S40</msc>
      </article>
      <p/>
      __<biblio>
      <citation from='year' key='Bar03' id='bid0' 
      __userid='cite:Ba03' type='article'>
      <bauteurs><nom>Barnes</nom><prenom>E. W.</prenom><initiale>E. W.</initiale>
      __<particule/><junior/></bauteurs>
      <title>On the expression of Euler's constant as a definite integral</title>
      <bjournal>Messenger of Math.</bjournal>
      <bvolume>33</bvolume>
      <byear>1903</byear>
      <bpages>59&ndash;61</bpages>
      </citation>
      __
      <citation from='year' key='Bom00' id='bid2' 
      __userid='cite:Bo99' type='article'>
      <bauteurs><nom>Bombieri</nom><prenom>E.</prenom>
      __<initiale>E.</initiale><particule/><junior/></bauteurs>
      <title>Remarks on Weil's quadratic functional in the theory
      __ of prime numbers&nbsp;I</title>
      <bjournal>Rend. Mat. Acc. Lincei, Ser.&nbsp;IX</bjournal>
      <bvolume>11</bvolume>
      <byear>2000</byear>
      <bpages>183&ndash;233</bpages>
      </citation>
      __
      <citation from='year' key='BPY01' id='bid1' 
      __userid='cite:BPY01' type='article'>
      <bauteurs><nom>Biane</nom><prenom>P.</prenom><initiale>P.</initiale>
      __<particule/><junior/><nom>Pitman</nom><prenom>J.</prenom>
      __<initiale>J.</initiale><particule/><junior/><nom>Yor</nom>
      __<prenom>M.</prenom><initiale>M.</initiale><particule/><junior/>
      __</bauteurs>
      <title>Probability laws related to the Jacobi $\theta $ and Riemann
      __ $\zeta $ functions, and Brownian excursions</title>
      <bjournal>Bull. Amer. Math. Soc.</bjournal>
      <bvolume>38</bvolume>
      <byear>2001</byear>
      <bpages>435&ndash;465</bpages>
      </citation></biblio></cedram>

.. _ex2new:

Cedram test 2
-------------

.. container:: xml_out

   ::

        <?xml version='1.0' encoding='iso-8859-1'?>
        <!DOCTYPE cedram SYSTEM 'cedram.dtd'>
        <!-- translated from latex by tralics 2.8-->
        <cedram>
        <article><production>
        <fichier_tex>bo</fichier_tex>
        <fichier_bib>bo</fichier_bib>
        <date_prod>2006-7-18</date_prod></production>
        
        <date_reception>2004-06-14</date_reception>
        <date_acceptation>2004-12-09</date_acceptation>
        <auteur>
        <prenom>Donald</prenom>
        <middlename>E.</middlename>
        <nom><hi rend='it'>Knuth</hi></nom>
        <adresse><TeX/> Users Group  P.O. Box 869 Santa Barbara, 
      __CA 93102-0869 USA</adresse>
        <mel>d.e.knuth@somewhere.on.the.net</mel>
        </auteur>
        <titre xml:lang='fr'>Coefficients Fourier pour fonctions 
      __<formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<msup><mi>L</mi> <mi>&infin;</mi> </msup></math></formula> simples</titre>
        <TeXtitre xml:lang='fr'>Coefficients Fourier pour fonctions 
      __<texmath type='inline'>L^\infty </texmath> simples</TeXtitre>
        <titre xml:lang='en'>Fourier coefficients for simple 
      __<formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<msup><mi>L</mi> <mi>&infin;</mi> </msup></math></formula> functions</titre>
        <TeXtitre xml:lang='en'>Fourier coefficients for simple 
      __<texmath type='inline'>L^\infty </texmath> functions</TeXtitre>
        <langue>en</langue>
        <resume xml:lang='en'>This is an abstract with a beautiful inline formula 
      __<formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<mrow><msub><mi>&lambda;</mi> <mi>n</mi> </msub><mrow><mo>(</mo>
      __<mi>&pi;</mi><mo>)</mo></mrow><mo>=</mo><mfrac><mi>N</mi> <mn>2</mn>
      __</mfrac><mi>n</mi><mo form='prefix'>log</mo><mi>n</mi><mo>+</mo><msub>
      __<mi>C</mi> <mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi><mo>)
      __</mo><mi>n</mi><mo>+</mo><mi>O</mi><mo>(</mo></mrow><msqrt><mi>n</mi>
      __</msqrt><mrow><mo form='prefix'>log</mo><mi>n</mi><mo>)</mo></mrow>
      __</mrow></math></formula>, where <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><mrow><msub><mi>C</mi>
      __ <mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi><mo>)</mo></mrow></mrow>
      __</math></formula> is a real-valued constant.
        </resume>
        <TEXresume xml:lang='en'>This is an abstract with a beautiful 
      __inline formula <texmath type='inline'>\lambda _n(\pi ) = \frac{N}{2} 
      __n \log n + C_1(\pi ) n +
      __  O(\sqrt{n}\log {n})</texmath>, where <texmath type='inline'>C_1(\pi )
      __</texmath> is a real-valued constant.
        </TEXresume>
        <resume xml:lang='fr'>Mon r�sum� avec ma formule
        <formula type='inline'><math xmlns='http://www.w3.org/1998/Math/MathML'>
      __<mrow><msub><mi>&lambda;</mi> <mi>n</mi> </msub><mrow><mo>(</mo>
      __<mi>&pi;</mi><mo>)</mo></mrow><mo>=</mo><mfrac><mi>N</mi> <mn>2</mn>
      __</mfrac><mi>n</mi><mo form='prefix'>log</mo><mi>n</mi><mo>+</mo>
      __<msub><mi>C</mi> <mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi><mo>)
      __</mo><mi>n</mi><mo>+</mo><mi>O</mi><mo>(</mo></mrow><msqrt><mi>n</mi>
      __</msqrt><mrow><mo form='prefix'>log</mo><mi>n</mi><mo>)</mo></mrow>
      __</mrow></math></formula>, o� <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><mrow><msub>
      __<mi>C</mi> <mn>1</mn> </msub><mrow><mo>(</mo><mi>&pi;</mi><mo>)</mo>
      __</mrow></mrow></math></formula> est une constante r�elle.
        </resume>
        <TEXresume xml:lang='fr'>Mon r�sum� avec ma formule
        <texmath type='inline'>\lambda _n(\pi ) = \frac{N}{2} n \log n 
      __+ C_1(\pi ) n +
        O(\sqrt{n}\log {n})</texmath>, o� 
      __<texmath type='inline'>C_1(\pi )</texmath> est une constante r�elle.
        </TEXresume>
        <motcle xml:lang='fr'>fonctions <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><msup><mi>L</mi> 
      __<mi>&infin;</mi> </msup></math></formula> simples, fonction lambda</motcle>
      __  <motcle xml:lang='en'>simple <formula type='inline'>
      __<math xmlns='http://www.w3.org/1998/Math/MathML'><msup>
      __<mi>L</mi> <mi>&infin;</mi> </msup></math></formula> functions, 
      __lambda function</motcle>
        <msc>11M26, 11M36, 11S40</msc>
        </article>
        <p/><biblio>
        <citation from='year' key='Bar03' id='bid0' userid='cite:Ba03' 
      __type='article'>
        <bauteurs><nom>Barnes</nom><prenom>E. W.</prenom>
      __<initiale>E. W.</initiale><particule/><junior/></bauteurs>
        <title>On the expression of Euler's constant as a definite integral</title>
        <bjournal>Messenger of Math.</bjournal>
        <bvolume>33</bvolume>
        <byear>1903</byear>
        <bpages>59&ndash;61</bpages>
        </citation>
        <citation from='year' key='Bom00' id='bid2' userid='cite:Bo99' 
      __type='article'>
        <bauteurs><nom>Bombieri</nom><prenom>E.</prenom>
      __<initiale>E.</initiale><particule/><junior/></bauteurs>
        <title>Remarks on Weil's quadratic functional in the theory of 
      __prime numbers&nbsp;I</title>
        <bjournal>Rend. Mat. Acc. Lincei, Ser.&nbsp;IX</bjournal>
        <bvolume>11</bvolume>
        <byear>2000</byear>
        <bpages>183&ndash;233</bpages>
        </citation>
        <citation from='year' key='BPY01' id='bid1' userid='cite:BPY01' 
      __type='article'>
        <bauteurs><nom>Biane</nom><prenom>P.</prenom><initiale>P.</initiale>
      __<particule/><junior/><nom>Pitman</nom><prenom>J.</prenom>
      __<initiale>J.</initiale><particule/><junior/><nom>Yor</nom>
      __<prenom>M.</prenom><initiale>M.</initiale><particule/><junior/></bauteurs>
        <title>Probability laws related to the Jacobi $\theta $ and 
      __Riemann $\zeta $ functions, and Brownian excursions</title>
        <bjournal>Bull. Amer. Math. Soc.</bjournal>
        <bvolume>38</bvolume>
        <byear>2001</byear>
        <bpages>435&ndash;465</bpages>
        </citation></biblio></cedram>
