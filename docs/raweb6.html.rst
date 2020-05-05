This page describes the style file used to convert the XML activity
report of Into to Pdf format. It is Chapter 6 of `Tralics and the
Raweb <raweb.html>`__

.. _racfg:

6. The raweb-cfg.sty file
=========================

We explain here the content of the file raweb-cfg.sty. It starts like
this.

.. container:: ltx-source

   ::

      \makeatletter
      \immediate\write20{Loading mathml support and raweb extensions}
      % patch of xmltex.tex

There are some lines that try to solve the problem: how to put an &
character in an URL, i.e. in an attribute. See
`here <raweb3.html#ampersand>`__.

The command ``\utfeight@protect@internal`` redefines, among other, the
command ``\utfeightaz`` as
``\def\utfeightaz {\noexpand \utfeightaz \noexpand}``. A more efficient
code would be: ``\let \utfeightaz \utfeightaz@jg@int``. See
`here <raweb3.html#added-cmds>`__ the definition of
``\utfeightaz@jg@int`` and the like.

We redefine ``\utfeight@protect@internal`` and
``\utfeight@protect@chars``. Code is given
`here <raweb3.html#modified-cmds>`__.

.. container:: ltx-source

   ::

      \def\utfeight@protect@chars{ source code shown here }
      \begingroup
      \catcode`<\active
      \catcode`&\active
      \gdef\utfeight@protect@internal{ source code shown here}

6.2 Ampersands in URL
---------------------

The problem is that the fotex.fmt file has a declaration like this
``\XMLnamespaceattributeX {fo} {external-destination} {\FOexternaldestination} {}``.
As a result, the property list of the \`fo' namespace will contain
(among others) the following tokens
``\XML@attrib  0:external-destination\relax \FOexternaldestination {}\utfeight@chardef  \FOexternaldestination``.
In some places, ``\XMLattributeX`` is called: this defines an attribute
for a single element. In both cases, the X means: call
``\utfeight@chardef``, and these are the only cases where the macro is
used. The X is for \`expand'; the trouble is that entities cannot be
expanded, and we need a way to inhibit this expansion.

We redefine ``\utfeight@chardef`` with a little hack for the &
character, as follows. Normally, ampersand is the same as
``\XML@amp@markup``. We change its value locally. Thus, we change the
behavior only inside some attributes.

.. container:: ltx-source

   ::

      \gdef\utfeight@chardef#1{%
        \begingroup
        \utfeight@protect@chars
        \let&\XML@amp@markup@jg  % line added
        \xdef\x@temp{#1}%
        \endgroup
        \let#1\x@temp}

Original code was complicated. What we do here is just test if the
argument is \`amp;' and replace it with an ampersand. We test the value
of the argument by adding \`amp;' and we look for the string.

.. container:: ltx-source

   ::

      \def\XML@amp@markup@jg#1;{%
        \XML@amp@markup@jgw#1;amp;\@nil}
      \def\XML@amp@markup@jgw#1amp;#2\@nil{%
        \ifx b#2b \XML@amp@markup#1;\else\string&\fi}

6.3 MathML
----------

Here we redefine a lot of MathML functions from mathml2.xmt, and add a
great number of other procedures.

.. container:: ltx-source

   ::

      % Is this needed ? we redefine everything here
      %\inputonce{mathml2.xmt}
      % We do not want mathml to be loaded after this file
      \global\expandafter\let\csname xmt:mathml2.xmt\endcsname\@ne

This declares the three namespaces that will be used later in this file.

.. container:: ltx-source

   ::

      \DeclareNamespace{m}{http://www.w3.org/1998/Math/MathML}
      \DeclareNamespace{fotex}{http://www.tug.org/fotex}
      \DeclareNamespace{fo}{http://www.w3.org/1999/XSL/Format}

We declare `here <raweb3.html#string-decls1>`__ and
`here <raweb3.html#string-decls2>`__ some strings via commands of the
form ``\XMLstring\att@true<>true</>``. After that, we can test that an
attribute is true with a test like ``\ifx\foo\att@true``. Some strings
are prefixed by \`jg' others by \`att'.

.. container:: ltx-source

   ::

      \global\delcode`{"66308
      \global\delcode`}"67309

      % We have to redefine some operators...
      \def\jg@tilde@acc{\mathaccent"707E }
      \def\jg@check@acc{\mathaccent"7014 }
      \def\jg@breve@acc{\mathaccent"7015 }
      \def\jg@hat@acc{\mathaccent"705E }
      \def\jg@dot@acc{\mathaccent"705F }
      \def\jg@ddot@acc{\mathaccent"707F }
      \def\jg@grave@acc{\mathaccent"7012 }
      \def\jg@ring@acc{\protect \mathaccentV {mathring}017 }
      \def\jg@overRarrow@acc{\let\rightarrow\JGG@orig@rarrow\mathpalette{\overarrow@\rightarrowfill@}}
      \def\jg@overLarrow@acc{\let\leftarrow\JGG@orig@larrow\mathpalette{\overarrow@\leftarrowfill@}}
      \def\jg@underRarrow@acc{\let\rightarrow\JGG@orig@rarrow\mathpalette{\underarrow@\rightarrowfill@}}
      \def\jg@underLarrow@acc{\let\leftarrow\JGG@orig@larrow\mathpalette{\underarrow@\leftarrowfill@}}

This makes the underscore character produce an underscore.

.. container:: ltx-source

   ::

      \expandafter\def\csname 8:_\endcsname{%
       \relax\ifmmode\hbox{\textunderscore}\else \textunderscore\fi}

This defines the behavior of the ``<math>`` element. Note. Our style
sheet puts display math elements into ``<fotex:displaymath>`` elements,
whose translation is a ``displaymath`` environment. As a result,
``\GATHER`` could be undefined...

.. container:: ltx-source

   ::

      \XMLelement{m:math}
        {\XMLattribute{display}{\XML@mathmlmode}{foo}}
        {\def\GATHER{1}%
           \ifx\XML@mathmlmode\att@BLOCK\[\else\(\fi
         }
        {
           \ifx\XML@mathmlmode\att@BLOCK\]\else\)\fi
        }

This implements ``<mover>``, ``<munder>``, ``<munderover>`` and
``<mfrac>``. Actual code is elsewhere. *Tralics* does not use
``<munderover>``.

.. container:: ltx-source

   ::

      \XMLelement{m:mover}
        {\XMLattribute{accent}{\XML@overaccent}{none}}
        {\xmlgrab}
        {\xmltextwochildren\xml@implement@over{}#1}

      \XMLelement{m:munder}
        {\XMLattribute{accentunder}{\XML@underaccent}{none}}
        {\xmlgrab}
        {\xmltextwochildren\xml@implement@under{}#1}

      \XMLelement{m:munderover}
        { }
        {\xmlgrab}
        {\xmltexthreechildren\@firstofone\sb\sp#1}

      \XMLelement{m:mfrac}
        {\XMLattribute{linethickness}{\XML@linethickness}{true}%
         \XMLattribute{numalign}{\XML@numalign}{center}%
         \XMLattribute{denomalign}{\XML@denomalign}{center}%
        }
        {\xmlgrab}
        {\xmltextwochildren\xml@implement@frac{}#1}

This implements the ``<mo>`` element. In the special case of
``<mo form="prefix">ln</mo>`` or
``<mo movablelimits="true" form="prefix">lim</mo>`` we use an upright
font, and ``\limits`` or ``\nolimits``. General case is handled below.

.. container:: ltx-source

   ::

      \XMLelement{m:mo}
        {\XMLattribute{form}{\XML@mathmlform}{inline}%
         \XMLattribute{movablelimits}{\XML@movablelimits}{false}}
        {\xmlgrab}
        {\ifx\XML@mathmlform\att@PREFIX
           \ifx\XML@movablelimits\att@true
             \mathop{\operator@font #1}\limits
           \else
             \mathop{\operator@font #1}\nolimits
           \fi
         \else\special@mo{#1}\fi
         }

When you say ``$\bar x$``, the MathML equivalent is
``<mover accent="true"><mi>x</mi>  <mo>&#xAF;</mo></mover>``. When we
see the ``<mo>`` element, we know that it is an accent because
``\notinover`` is no ``\relax``. In this case, we have to do something
special with the content. In some cases, we put the content in
``\cur@mo@content``, in other cases, we set ``\jg@cur@acc``. For some
strange reasons ``&lt;`` translates like ``\lbra``, we change this.

.. container:: ltx-source

   ::

      \let\notinover\relax
      \def\cur@mo@content{TOTO}

      \def\special@mo#1{%
           \def\jg@tck{#1} % save the argument
           \ifx\notinover\over% we cannot typeset here
              \ifx\jg@tck\jg@accgrave % strange 
                \global\let\jg@cur@acc\jg@grave@acc
                \global\let\cur@mo@content\relax
              \else \ifx\jg@tck\jg@accdddot % strange 
                \global\let\jg@cur@acc\dddot
                \global\let\cur@mo@content\relax
              \else \ifx\jg@tck\jg@accddddot % strange 
                \global\let\jg@cur@acc\ddddot
                \global\let\cur@mo@content\relax
              \else\gdef\cur@mo@content{#1}\fi\fi\fi
           \else % typeset the argument, handle < and > in the correct way
           \ifx\jg@tck\jg@gt\string>\else
           \ifx\jg@tck\jg@lt\string<\else
           #1\fi\fi\fi
      }

We can avoid testing against every accent in the following way: If you
want to put a tilde, the XML will contain ``<mo>&#x2DC;</mo>``. This is
defined as ``\texttildelow`` in the file ucharacters.sty. We redefine
this command, so as to define ``\jg@cur@acc``. We use two functions, one
for over accents, another one for underaccents.

.. container:: ltx-source

   ::

      \let\XXacute\acute
      \def\jg@bindings{%
        \def\texttildelow {\global\let\jg@cur@acc\jg@tilde@acc}%
        (complete code here)
        \def\ring{\global\let\jg@cur@acc\mathring}
      }
      \def\jg@ubindings{%
         \let\JGG@orig@rarrow\rightarrow
         \let\JGG@orig@larrow\leftarrow
         \def\texttildelow {\global\let\jg@cur@acc\jg@tilde@acc}%
          (complete code here)
         \def\jgunderline{\global\let\jg@cur@acc\underline}
      }

Translation of ``<mover>xy</mover>`` uses ``\stackrel``, unless it is an
accent. Otherwise, we call ``\jg@bindings``, evaluate the second
argument in a context where ``\notinover`` is ``\over``. We assume that
``\cur@mo@content\jg@cur@acc`` will typeset what is needed.

.. container:: ltx-source

   ::

      \def\xml@implement@over#1#2{%
       \ifx\XML@overaccent\att@true {%
         \jg@bindings
         \let\notinover\over #2\let\notinover\relax \cur@mo@content\jg@cur@acc{#1}%
       }\else\stackrel{#2}{#1}\fi
      }

      \def\xml@implement@under#1#2{%
       \ifx\XML@underaccent\att@true {%
         \jg@ubindings
         \let\notinover\over #2\let\notinover\relax \cur@mo@content\jg@cur@acc{#1}%
       }\else \underset{#2}{#1}\fi%
      }

The code of a fraction is trivial. However, there is an `alternate
version <raweb3.html#unimp-frac>`__ that honors the ``scriptlevel``
attribute (*Tralics* uses a ``<mstyle>`` element for ``\frac`` and
such).

.. container:: ltx-source

   ::

      \def\xml@implement@frac#1#2{%
        \ifx\XML@linethickness\att@true\frac{#1}{#2}%
        \else \genfrac{}{}\XML@linethickness{}{#1}{#2}\fi
      }

The ``\defaultunits`` command calls ``\afterassignment`` in order to
skip all tokens up to ``\@nnil``. As a result, ``\XML@mspacewidth`` can
contain a dimension (for instance 33mm) or a number (like 2.3) case
where a \`pt' unit is inserted.

.. container:: ltx-source

   ::

      \XMLelement{m:mspace}
        {\XMLattribute{width}{\XML@mspacewidth}{0}}
        {}
        {\@defaultunits\dimen@\XML@mspacewidth pt\relax\@nnil
         \ifnum\dimen@=\z@\else\kern\dimen@\fi}

This implements the ``<mstyle>`` element.

.. container:: ltx-source

   ::

      \XMLelement{m:mstyle}
       {\XMLattribute{displaystyle}{\XML@att@displaystyle}{foo} %
         \XMLattribute{scriptlevel}{\XML@scriptlevel}{none}%
       }
       {\xmlgrab}
       {{\ifx\XML@att@displaystyle\att@true\displaystyle
         \else\ifx\XML@att@displaystyle\att@false
          \ifx\XML@scriptlevel\att@dzero\textstyle
           \else\ifx\XML@scriptlevel\att@done\scriptstyle
            \else \scriptscriptstyle\fi\fi
         \fi %do nothing if neither true nor false
         \fi#1}}

We give here the code of some trivial functions.

.. container:: ltx-source

   ::

      \XMLelement{m:mrow}
        {}
        {}
        {}

      \XMLelement{m:msub}
        {}
        {\xmlgrab}
        {\xmltextwochildren\@firstofone\sb#1}

      \XMLelement{m:msup}
        {}
        {\xmlgrab}
        {\xmltextwochildren\@firstofone\sp#1}

      \XMLelement{m:msubsup}
        {}
        {\xmlgrab}
        {\xmltexthreechildren\@firstofone\sb\sp#1}

      \XMLelement{m:mroot}
        {}
        {\xmlgrab}
        {\xmltextwochildren\mathmlroot{}#1}
      \def\mathmlroot#1#2{\root#2\of{#1}}

      \XMLelement{m:msqrt}
        {}
        {\xmlgrab}
        {\sqrt{#1}}

      \XMLelement{m:mtext}
        {}
        {\xmlgrab}
        {\hbox{#1}}

This implements the ``<mn>`` element. Is ``\mathrm`` a good choice? We
implement also the ``<mi>`` element. You can specify a font in the
attribute. There is also the infamous test on the length. This means
that you should not insert spaces. This means also that you cannot use
entities.

.. container:: ltx-source

   ::

      \XMLelement{m:mi}
        {\XMLattribute{mathvariant}{\XML@mathmlvariant}{normal}}
        {\xmlgrab}
        {\mi@test#1\relax}

      \gdef\mi@test#1#2\relax{
       \ifx\XML@mathmlvariant\att@mathml@bold
        \mathbf{#1#2}\else
       \ifx\XML@mathmlvariant\att@mathml@sansserif
        \mathsf{#1#2}\else
       \ifx\XML@mathmlvariant\att@mathml@tt
        \texttt{#1#2}\else
        \ifx\mi@test#2\mi@test
         \expandafter#1
        \else
         \mathrm{#1#2}
        \fi\fi\fi\fi}

      \XMLelement{m:mn}
        {}
        {\xmlgrab}
        {\mathrm{#1}}

This implements the ``<mfence>`` element. The trouble is that the
``\left`` and ``\right`` commands expect (after expansion) a delimiter.
What we have is a character string, that we must convert into a token.
The code is trivial, but a bit longish.

.. container:: ltx-source

   ::

      \XMLelement{m:mfenced}
        { \XMLattribute{open}{\XML@fenceopen}{(}
          \XMLattribute{close}{\XML@fenceclose}{)}  }
        {\jg@hacko\left\XML@fenceopen}
        {\jg@hackc\right\XML@fenceclose}

      \def\jg@hacko{%
        \ifx\XML@fenceopen\jg@lt\let\XML@fenceopen\langle\fi
        (complete code shown here)
      }

      \def\jg@hackc{%
        \ifx\XML@fenceclose\jg@gt\let\XML@fenceclose\rangle\fi}
        (complete code shown here)
      }

6.4 Tables in MathML
--------------------

Original code tested attribute ``class=equation``. Called \`gathered' in
this case. This uses three global variables: ``\GATHER``,
``\StartTable`` and ``\StartRow``

.. container:: ltx-source

   ::

      \XMLelement{m:mtable}
        {}
        {
        \def\StartTable{1}%
        \def\GATHER{0}%
           \begin{array}{*{99}{c}}
        }
        {
          \end{array}
        }

A non trivial question: where the does group start of finish? We
globally define a command ``\temp`` that is executed just after the
current group. This command defines ``\StartRow`` to 1 (true), then
executes the argument. Moreover, if ``\StartTable`` is 0 (first row of
the table) it toggles it to 1; otherwise, it inserts a ``\\``, this
creates a new row.

.. container:: ltx-source

   ::

      \XMLelement{m:mtr}
       {}
       {\xmlgrab}
       {\ifnum\StartTable=1
          \gdef\temp{ \gdef\StartTable{0}\def\StartRow{1}#1}%
        \else
          \gdef\temp{\\\def\StartRow{1}#1}%
        \fi
        \aftergroup\temp}

This piece of code is not complete. It does not handle the multicolumn
case. We construct a ``\temp`` command as above. If ``StartRow`` is one,
we toggle to 0, otherwise insert a ampersand (here ``\tabcellsep``). We
test also alignments (left, right, center). What is the purpose of the
space after ``\hfill``?

.. container:: ltx-source

   ::

      \XMLelement{m:mtd}
       {\XMLattribute{columnalign}{\XML@mtdalign}{center}
        \XMLattribute{columnspan}{\XML@mtdspan}{1}}
       {\xmlgrab}
       {
         %  \ifnum\XML@mtdspan=1 
         \ifnum\StartRow=1
           \ifx\XML@mtdalign\att@mtd@left
              \gdef\temp{\gdef\StartRow{0}#1\hfill{ }}%
              \else \ifx\XML@mtdalign\att@mtd@right
                \gdef\temp{\gdef\StartRow{0}\hfill#1}%
                \else \gdef\temp{\gdef\StartRow{0}#1}\fi\fi%
         \else%
           \ifx\XML@mtdalign\att@mtd@left
              \gdef\temp{\tabcellsep#1\hfill{ }}%
              \else \ifx\XML@mtdalign\att@mtd@right
                \gdef\temp{\tabcellsep\hfill #1}%
                \else \gdef\temp{\tabcellsep#1}\fi\fi%
          \fi
        \aftergroup\temp
       }

6.5 Pictures
------------

We show there the TeX command used to implement the picture environment.

The translation of
``<picture width='A' height='B' xpos='C' ypos='D'> etc </picture>`` is
``\begin{picture}(A,B)(C,D) etc \end{picture}``.

.. container:: ltx-source

   ::

      \XMLelement{picture}
        {\XMLattribute{width}{\XML@width}{1}
         \XMLattribute{height}{\XML@height}{1}
         \XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{ypos}{\XML@ypos}{0}
        }
        {\begin{picture}(\XML@width,\XML@height)(\XML@xpos,\XML@ypos)}
        {\end{picture}}

The translation of ``<pic-put xpos='A' ypos='B'> object </pic-put>`` is
``\put(A,B){object}``.

.. container:: ltx-source

   ::

      \XMLelement{pic-put}
        {\XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{ypos}{\XML@ypos}{0}}
        {\xmlgrab}
        {\put(\XML@xpos,\XML@ypos){#1}}

The translation of ``<pic-arc xpos='A' ypos='B' angle='C'/>`` is
``\arc(A,B){C}``.

.. container:: ltx-source

   ::

      \XMLelement{pic-arc}
        {\XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{angle}{\XML@angle}{0}
         \XMLattribute{ypos}{\XML@ypos}{0}}
        {\xmlgrab}
        {\arc(\XML@xpos,\XML@ypos){\XML@angle}}

The translation of
``<pic-scaleput xscale='A' yscale='B'  xscaley='C' yscalex='D' xpos='E' ypos='F'> argument </pic-scaleput>``
is
``\def\xscale{A} \def\yscale{B} \def\xscaley{C} \def\yscalex{D} \scaleput(E,F){argument}``.
The ``\def``\ s are automatic.

.. container:: ltx-source

   ::

      \XMLelement{pic-scaleput}
        {\XMLattribute{xscale}{\xscale}{1.0}
         \XMLattribute{yscale}{\yscale}{1.0}
         \XMLattribute{xscaley}{\xscaley}{0.0}
         \XMLattribute{yscalex}{\yscalex}{0.0}
         \XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{ypos}{\XML@ypos}{0}}
        {\xmlgrab}
        {\scaleput(\XML@xpos,\XML@ypos){#1}}

The translation of ``<pic-thicklines/>`` is ``\thicklines``; that of
``<pic-thinlines/>`` is ``\thinlines``; and that of
``<pic-linethickness size='A'>`` is ``\linethickness{A}``.

.. container:: ltx-source

   ::

      \XMLelement{pic-thicklines}
        {}{\xmlgrab}{\thicklines}

      \XMLelement{pic-thinlines}
        {}{}{\thinlines}

      \XMLelement{pic-linethickness}
        {\XMLattribute{size}{\XML@size}{1pt}}
        {}
        {\linethickness{\XML@size}}

The previous code works only if the commands modify global variables.
Thus we redefine these.

.. container:: ltx-source

   ::

      \gdef\thinlines{\global\let\@linefnt\tenln \global\let\@circlefnt\tencirc
        \global\@wholewidth\fontdimen8\tenln \global\@halfwidth .5\@wholewidth}

      \gdef\thicklines{\global\let\@linefnt\tenlnw \global\let\@circlefnt\tencircw
        \global\@wholewidth\fontdimen8\tenlnw \global\@halfwidth .5\@wholewidth}

      \gdef\linethickness#1{%
         \global\@wholewidth #1\relax \global\@halfwidth .5\@wholewidth}

The translation of
``<pic-multiput xpos='A' ypos='B' repeat='C' dx='D' dy='E'>  argument </pic-multiput>``
is ``\multiput(A,B)(D,E){C}{argument}``.

.. container:: ltx-source

   ::

      \XMLelement{pic-multiput}
        {\XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{ypos}{\XML@ypos}{0}
         \XMLattribute{repeat}{\XML@repeat}{1}
         \XMLattribute{dx}{\XML@dx}{1}
         \XMLattribute{dy}{\XML@dy}{1}}
        {\xmlgrab}
        {\multiput(\XML@xpos,\XML@ypos)(\XML@dx,\XML@dy){\XML@repeat}{#1}}

The translation of
``<pic-bezier a1='A' a2='B' b1='C' b2='D'  c1='E' d1='F' repeat ='G'/>``
is ``\qbezier[G](A,B)(C,D)(E,F)``. The value of the optional argument
\`repeat' is zero.

.. container:: ltx-source

   ::

      \XMLelement{pic-bezier}
        {\XMLattribute{a1}{\XML@ai}{0}
         \XMLattribute{a2}{\XML@aii}{0}
         \XMLattribute{b1}{\XML@bi}{0}
         \XMLattribute{b2}{\XML@bii}{0}
         \XMLattribute{c1}{\XML@ci}{0}
         \XMLattribute{c2}{\XML@cii}{0}
         \XMLattribute{repeat}{\XML@repeat}{0}}
        {\xmlgrab}
        {\qbezier[\XML@repeat](\XML@ai,\XML@aii)(\XML@bi,\XML@bii)(\XML@ci,\XML@cii)}

Lines and vectors are handled in the same way. The translation of
``<pic-line xdir='A' ydir='B' width='C'/>`` or
``<pic-vector xdir='A' ydir='B' width='C'/>`` is ``\line(A,B){C}`` and
``\vector[G](A,B){C}``.

.. container:: ltx-source

   ::

      \XMLelement{pic-line}
        {\XMLattribute{xdir}{\XML@xdir}{0}
         \XMLattribute{ydir}{\XML@ydir}{0}
         \XMLattribute{width}{\XML@width}{0}}
        {\xmlgrab}
        {\line(\XML@xdir,\XML@ydir){\XML@width}}

      \XMLelement{pic-vector}
        {\XMLattribute{xdir}{\XML@xdir}{0}
         \XMLattribute{ydir}{\XML@ydir}{0}
         \XMLattribute{width}{\XML@width}{0}}
        {\xmlgrab}
        {\vector(\XML@xdir,\XML@ydir){\XML@width}}

Three similar routines. The translation of
``<pic-curve unit-length=unit-length="0.4">300,0, 340,100, 380,0</pic-curve>``
is ``{\setlength\unitlength{0.4pt}\curve(300,0, 340,100, 380,0)}``.
There is a big hack here. Spaces are defined to be space plus
``\ignorespaces`` by xmltex.tex. When the package tries to convert the
340 into a dimension, via ``\mylength=argument\unitlength``, the
``\ignorespaces`` gives a *Missing number, treated as zero* error. For
this reason, we redefine ``\ignorespaces``.

.. container:: ltx-source

   ::

      \XMLelement{pic-curve}
        {\XMLattribute{unit-length}{\XML@ulength}{1}}
        {\xmlgrab}
        {{\def\ignorespaces{}\setlength\unitlength{\XML@ulength pt}\curve(#1)\global\unitlength=1pt }}

      \XMLelement{pic-closecurve}
        {\XMLattribute{unit-length}{\XML@ulength}{1}}
        {\xmlgrab}
        {{\def\ignorespaces{}\setlength\unitlength{\XML@ulength pt}\closecurve(#1)\global\unitlength=1pt}}

      \XMLelement{pic-tagcurve}
        {\XMLattribute{unit-length}{\XML@ulength}{1}}
        {\xmlgrab}
        {{\def\ignorespaces{}\setlength\unitlength{\XML@ulength pt}\tagcurve(#1)\global\unitlength=1pt }}

The translation of ``<pic-frame> argument  </pic-frame>`` is
``\frame{argument}``

.. container:: ltx-source

   ::

      \XMLelement{pic-frame}
       {}
       {\xmlgrab}
       {\frame{#1}}

The translation of ``<pic-circle size='A' full='false'/>`` or
``<pic-circle size='A' full='notfalse'/>`` is ``\circle{A}`` or
``\circle*{A}``. *Tralics* uses nothing or ``true`` as value of the
``full`` attribute.

.. container:: ltx-source

   ::

      \XMLelement{pic-circle}
        {\XMLattribute{size}{\XML@size}{1}
         \XMLattribute{full}{\XML@full}{false}}
        {\xmlgrab}
        {\ifx\XML@full\att@false\circle{\XML@size}\else \circle*{\XML@size}\fi}

The translation of ``<pic-bigcircle size='A' unitlength='B'/>`` should
be ``{\unitlength=Bpt \bigcircle{A}}``. For some reasons, the
``\bigcircle`` command modifies globally ``\unitlength``, so that the
piece of code given above modifies ``\unitlength``. For this reason, we
use another variable, and use a global assignment.

.. container:: ltx-source

   ::

      \newdimen\jgunitlength
      \XMLelement{pic-bigcircle}
        {\XMLattribute{size}{\XML@size}{1}
         \XMLattribute{unit-length}{\XML@ulength}{1}}     
        {\xmlgrab}
        {\jgunitlength=\unitlength\setlength\unitlength{\XML@ulength pt}
          \bigcircle{\XML@size}
          \global\unitlength=\jgunitlength}

This should be changed. This evaluates the command (first argument),
with an argument (second argument), using an optional argument which is
one of L, R, and one of T, B, computed from the global variable
``\XML@pos``.

.. container:: ltx-source

   ::

      \def\@att@to@rtb#1#2{%
        \ifx\XML@pos\att@l \def\foo{#1[l]{#2}}
        \else\ifx\XML@pos\att@r \def\foo{#1[r]{#2}}%
        \else\ifx\XML@pos\att@t \def\foo{#1[t]{#2}}%
        \else\ifx\XML@pos\att@b \def\foo{#1[b]{#2}}%
        \else\ifx\XML@pos\att@lt \def\foo{#1[lt]{#2}}%
        \else\ifx\XML@pos\att@lb \def\foo{#1[lb]{#2}}%
        \else\ifx\XML@pos\att@rt \def\foo{#1[rt]{#2}}%
        \else\ifx\XML@pos\att@rb \def\foo{#1[rb]{#2}}%
        \else\ifx\XML@pos\att@tl \def\foo{#1[lt]{#2}}%
        \else\ifx\XML@pos\att@bl \def\foo{#1[lb]{#2}}%
        \else\ifx\XML@pos\att@tr \def\foo{#1[rt]{#2}}%
        \else\ifx\XML@pos\att@br \def\foo{#1[rb]{#2}}%
        \else \def\foo{#1{#2}}
       \fi\fi\fi\fi\fi\fi\fi\fi\fi\fi\fi\fi
      \foo
      }

The translation of
``<pic-framebox width='A'  height='B' position='xx'> argument </pic-framebox>``
is ``\makebox(A,B)[opt]{argument}``, where the optional argument is
given by the procedure above. In case an non-false ``framed`` attribute
is given, the ``\framebox`` command is used instead of ``\makebox``.

.. container:: ltx-source

   ::

      \XMLelement{pic-framebox}
        {\XMLattribute{width}{\XML@width}{0}
         \XMLattribute{height}{\XML@height}{0}
         \XMLattribute{position}{\XML@pos}{}
         \XMLattribute{framed}{\XML@framed}{false}
        }
       {\xmlgrab}
       {\let\cmd\framebox\ifx\XML@framed\att@false\let\cmd\makebox\fi
        \@att@to@rtb{\cmd(\XML@width,\XML@height)}{#1}}

The translation of
``<pic-dashbox  width='A' height='B' dashdim='C' position='xx'> argument </pic-dashbox>``
is ``\dashbox{C}(A,B)[opt]{argument}``, where the optional argument is
given by the procedure above.

.. container:: ltx-source

   ::

      \XMLelement{pic-dashbox}
        {\XMLattribute{width}{\XML@width}{0}
         \XMLattribute{height}{\XML@height}{0}
         \XMLattribute{position}{\XML@pos}{w}
         \XMLattribute{dashdim}{\XML@dashdim}{1pt}
        }
       {\xmlgrab}
       {\@att@to@rtb{\dashbox{\XML@dashdim}(\XML@width,\XML@height)}{#1}}

The translation of
``<pic-oval  xpos='A' ypos='B' specs='xx'> argument </pic-oval>`` is
``\oval(A,B)[opt]{argument}``, where the optional argument is given by
the procedure above.

.. container:: ltx-source

   ::

      \XMLelement{pic-oval}
        {\XMLattribute{xpos}{\XML@xpos}{0}
         \XMLattribute{specs}{\XML@pos}{}
         \XMLattribute{ypos}{\XML@ypos}{0}}
        {\xmlgrab}
        {\@att@to@rtb{\oval(\XML@xpos,\XML@ypos)}{#1}}

Useful for typesetting TeX and LaTeX logos.

.. container:: ltx-source

   ::

      \XMLelement{TeX}
        {}{\TeX{}}{}
      \XMLelement{LaTeX}
        {}{\LaTeX{}}{}

All images in examples of *Tralics* are obtained by putting the
expression into ``<preview>`` element, whose translation is given here.

.. container:: ltx-source

   ::

      \XMLelement{preview}
        {}
        {\begin{preview}}
        {\end{preview}}

6.6 Tables
----------

In the file fotex.sty, tables are implemented via the \`NoTable'
mechanism. We modify this a bit.

In this piece of code, we do not set ``\TableWidth`` if the width is
declared auto.

.. container:: ltx-source

   ::

      \newif\ifnoheightcheck
      \catcode`\:\active

      \def\myNoTableSetup{%
        \ifx\FOwidth\att@auto\else
         \TableWidth\FOwidth
         \advance\TableWidth by -\tabcolsep
         \advance\TableWidth by -\FOmarginleft
         \advance\TableWidth by -\FOmarginright
       \fi
        \global\advance\AbsoluteTableCount by 1 %
        \DeclareArray{fotable\the\AbsoluteTableCount:}%
        \global\CellCount0 %
      }
      \catcode`\:12

6.7 Getting labels right
------------------------

The fotex.xmt file defines the behaviour of ``<fo:block>``. The action
depends on the context. The action, when entering the block can be
``\FOListBlock`` inside a list, ``\FOOutputBlock`` (inside output) or
``\FOBoxedBlock`` in a table, or ``\FONormalBlock``. We modified the
code of this command `(complete source
here) <raweb3.html#hack-normalblock>`__.

The essential modification is to move the ``\FOlabel`` after
``\FOvspacebefore``. Thus, in case of page break, the label is near the
text.

.. container:: ltx-source

   ::

      \AtBeginDocument{%
      \def\FONormalBlock{%
        \let\@x\relax
        \ifnum\FOTableNesting>0
          ...
        \else
          \ifnum\FOinList>0
            \FOlabel
            ...
          \else
            ...
            \par
             %\FOlabel (deleted here) 
            \Quadding
            \ifFOBlockGrab
              \FOBoxedBlock{\linewidth}%
            \else
              \FOBorderTop
              \ifdim\FOpaddingbefore>\z@
               \vskip\FOpaddingbefore
              \fi
              \FOvspacebefore
              ...
            \fi
            ...
          \fi
          \FOlabel % added here 
          \FOSetFont{normal}%
        \fi
      \@x
      }
      }

The argument of ``\FOgeneratePage`` is the expansion of ``\FOformat``.
If it is, say ``A.``, an A followed by a dot, the result will be
``\@Alph{\c@page}.``, with a dot after the page number.

The ``\thepage`` command is defined as
``\csname @arabic\endcsname \c@page``, and this looks like the
``\yFOlabel`` command.

.. container:: ltx-source

   ::

      %\def\FOgeneratePage#1#2\@null{\csname Format-#1\endcsname{\c@page}#2}
      \def\xFOgeneratePage#1#2\@null{\yFOlabel#1#2}
      \def\yFOlabel#1{\csname Format-#1\endcsname{\c@page}}

The macro ``\xFOlabel`` is the same as the original ``\FOlabel`` except
that ``\yFOlabel`` is not expanded: The purpose of ``\protected@write``
is to inhibit premature expansion of ``\thepage``. Since the code does
not use ``\thepage``, lots of reference in the table of contents are
wrong.

.. container:: ltx-source

   ::

      \def\xFOlabel{%
       \ifx\@empty\FOid\else
        \@bsphack
        \protected@write\@mainaux{\let\yFOlabel\relax}%
          {\string\newlabel{\FOid}{{}{\expandafter\xFOgeneratePage
                  \FOformat\@null}{}{\FOid}{}}}%
        \@esphack
        \hyper@@anchor{\FOid}{\relax}%
        \global\let\FOid\@empty
       \fi
      }

6.8 The title page
------------------

If you say ``\ra@atxy(70mm,174mm){xx}``, then xx is typeset in a box,
shifted by 70mm horizontally and 174mm vertically, and added to the box
``\ra@atxybox``.

.. container:: ltx-source

   ::

      \newbox\ra@atxybox
      \def\ra@atxy(#1,#2)#3{\global\setbox\ra@atxybox=\hbox
       {\unhbox\ra@atxybox
        \vtop to 0pt{\kern #2\hbox to 0pt{\kern #1\relax #3\hss}\vss}}}

The idea is to put the content of the ``\ra@atxybox`` box at the origin.
We have to modify the ``\output`` routine, and the easiest way is to
change ``\@texttop``, which does nothing (it could be used to implement
``\raggedtop``). The expansion is some material that is put at upper
left corner of the page, before headings are added. This means that we
have to shift the material, inserting some kerns. Note that
``\@themargin`` is correct only if the page is odd, or if left and right
margins are the same. The logo appears on only one page, because we kill
``\@texttop``. Note: The raweb.cls file inserts a logo on a given page
by adding ``\setbox255=\vbox{\@usemyatxy \unvbox255}`` in front of the
``\output`` routine. The ``\@usemyatxy`` command is similar to the
command given here.

.. container:: ltx-source

   ::

      \def\ra@useatxy{{%
        \let\@themargin\oddsidemargin
        \vtop to 0pt{\kern-\headsep \kern-\topmargin \kern-\headheight
                     \kern-1in \kern-\voffset
          \hbox to 0pt{\kern-\@themargin \kern-1in \kern-\hoffset
          \unhbox\ra@atxybox \hss}\vss}}%
          \global\let\@texttop\relax}

| We give here two similar functions. The first one is for the Raweb, it
  is a reference to the English page, and the second is for Research
  Reports. In the URL, en is replaced by fr, and THEME is spelled as
  THÈME. The effect of this command is to insert, just above the logo,
  the value of the theme, with a rule on each side. The ``\vfil`` is for
  vertical centering of the title. The 8cm is obtained by trial and
  error. The ``\firstchar`` command is now useless. We show show here
  the two versions of the Logo: French and English. As you can see, they
  are not exactly the same.
| |Logo Ra2003|---|Logo Ra2002|

.. container:: ltx-source

   ::

      \def\foratheme#1{\vskip8cm \vfil
        \ra@atxy(74mm,175mm) {\hbox to 70mm{%
             \hrulefill\hspace{8mm}
               \def\firstchar##1##2\relax{##1##2} % ducon
                 \href{http://www.inria.fr/recherche/equipes/listes/%
            theme_\firstchar#1\relax.en.html}{THEME \uppercase{#1}}%
                   \hspace{8mm}\hrulefill}}}
      \def\forrtheme#1{...}

This puts the logo at the right place in case of a Inria Research or
Technical Report. Digits seem randomly chosen: you would expect both
logos to be of the same size, inserted at the origin, with a width of
210mm.

.. container:: ltx-source

   ::

      \def\forrinria{%
       {\def\tmp{RR}%
         \ifx\tmp\ra@type\ra@atxy(1.05cm,1.625cm){\includegraphics[width=23.1cm]%
              {rap-rech}}%
                    \else\ra@atxy(0.5cm,1.325cm){\includegraphics[width=19.2cm]%
              {rap-tech}\fi}}}

This command is used to put the Raweb logo (depending on the year) and
the Inria logo on the front page. This code is awful: the text INRIA
etc. has a width of 128mm, and is at 39mm from one border and 43 from
the other one. The ``\vfil`` is for vertical alignment of the text that
follows (name of the team etc).

.. container:: ltx-source

   ::

      \def\foinria{%
        \ra@atxy(7.8cm,2.5cm){\includegraphics[width=5.7cm]{Logo-INRIA-couleur}}%
        \ra@atxy(55mm,173mm){\includegraphics{LogoRA\ra@year}}%
       \setbox0\hbox to 14cm{%
           \noindent\hskip3cm\hfill
           {\fontencoding{T1}\fontfamily{ptm}\fontseries{m}%
         \fontshape{n}\fontsize{10pt}{12pt}\selectfont
           \href{http://www.inria.fr}{INSTITUT NATIONAL DE RECHERCHE EN INFORMATIQUE %
              ET EN AUTOMATIQUE}}%
            \hskip-5cm\hfill}%
        \null\vskip0.7cm \leavevmode\hskip-3.5cm\box0\null\vskip2cm\vfil}

We invented ``<cleardoublepage/>``. Its effect is to make the first page
of text appear on a right page (the text starts on a odd page; in fact
it starts on page one, because we use the adequate
``<fo:page-sequence>`` element. Of course, the problem could be solved
by putting the table of contents at the end of the document.

.. container:: ltx-source

   ::

      \XMLelement{cleardoublepage}
       {} {\cleardoublepage} {}

This is another element we have to add, because we do not know how to
express this in xslt-fo: We want a rule just below the page headings.
This produces a box of width zero, with a rule of the width of the page
that will be below the text that follows.

.. container:: ltx-source

   ::

      \XMLelement{pagestylehrule}
       {} {\hbox to0pt{\rule[-1ex]{\textwidth}{.03cm}\hss}} {}

6.9 Math and images
-------------------

There is a PerlScript names extract-math.pl, that tries to convert
MathML into images. The document to be converted is
`here <raweb1.html#extract-math>`__. This converts to 5 images: image
one=\ |mathimage 1|, image two=\ |mathimage 2|, image three=\ |mathimage
3|, image four=\ |mathimage 4|, and image five=\ |mathimage 5|.

The idea is to use some code from latex2html.

.. container:: ltx-source

   ::

      \XMLelement{formula}
       {\XMLattribute{id}{\XML@formid}{none}}
       {\@inlinemathA{\XML@formid}}
       {\@inlinemathZ}

There is no need to redefine brackets, because in the current version,
nothing is in display. The argument of this procedure is the number of
the image (the \`id' attribute of the XML element). We open a box,
containing a rule, and the content of the element.

.. container:: ltx-source

   ::

      \newbox\sizebox
      \def\@inlinemathA#1{%
        \gdef\[{$\displaystyle}\gdef\]{$} % hack...
        \xdef\@mathenv{#1}%
        \adjustnormalsize \newpage\clearpage
        \setbox\sizebox=\hbox\bgroup\kern.05em
        \vrule height1.5ex width0pt }

This is a hack defined by latex2html. Is everything needed?

.. container:: ltx-source

   ::

      \AtBeginDocument{
      \let\realnormalsize=\relax
      \def\adjustnormalsize{%
        \def\normalsize{\mathsurround=0pt \realnormalsize
         \parindent=0pt\abovedisplayskip=0pt\belowdisplayskip=0pt}%
        \def\phantompar{\csname par\endcsname}%
        \normalsize}
      }

Vertical alignment is done as follows: the baseline is either the bottom
or the center. That is, if the depth of the box is not zero, we add a
vrule with the same height and depth.

.. container:: ltx-source

   ::

      \def\@centerinlinemath{%
        \dimen1=\ifdim\ht\sizebox<\dp\sizebox \dp\sizebox\else\ht\sizebox\fi
        \advance\dimen1by.5pt \vrule width0pt height\dimen1 depth\dimen1 
       \dp\sizebox=\dimen1\ht\sizebox=\dimen1\relax}

This is done at the end of the math formula. The log file will contain
lines of the form *l2hSize :1:8.0pt::8.0pt::94.54239pt*. The Perl script
reads these data from the log file, and calls another Perl script:
/usr/bin/perl /usr/local/latex2html/bin/pstoimg -type png -tmp tmpdir
-discard -interlace -antialias -depth 1 -scale 1.4 -geometry 94x16
-margins 62,41 -crop abls -transparent -out math_image_1.png
tmpdir/images001

.. container:: ltx-source

   ::

      \def\@inlinemathZ{%
        \egroup\expandafter\ifdim\dp\sizebox>0pt %
        \expandafter\@centerinlinemath\fi
        \typeout{l2hSize %
         :\@mathenv:\the\ht\sizebox::\the\dp\sizebox::\the\wd\sizebox.}
        \hbox{\vrule width.1em\kern-.05em\vtop{\vbox{%
        \kern.1em\kern0.6 pt\hbox{\hglue.17em\copy\sizebox\hglue0.6 pt}}\kern.3pt%
        \ifdim\dp\sizebox>0pt\kern.1em\fi \kern0.6 pt%
        \ifdim\hsize>\wd\sizebox \hrule depth1pt\fi}}%
        \ifdim\ht\sizebox<\vsize 
            \ifdim\wd\sizebox<\hsize\expandafter\hfill\fi \expandafter\vfill
            \else\expandafter\vss\fi
        \clearpage
      }

Now, sixty lines of hack. Complete code
`here <raweb3.html#hack-item>`__. Translation of ``fo:list-item-label``
calls ``\savebox{\ItemBox}{#1}`` just before ``\item[#1]``. This code is
executed again here. I don't remember why (the ``\ItemBox`` is only used
here).

.. container:: ltx-source

   ::

      \def\jg@item[#1]{%
        \if@noparitem
          \@donoparitem
        \else
          \if@inlabel ...  \fi
          \ifhmode ...     \fi
          \if@newlist ...    \fi
          \global\@inlabeltrue
        \fi
        \global\everypar{ ... }
        \if@noitemarg ... \fi
        \savebox{\ItemBox}{#1}% Added JG
        \sbox\@tempboxa{\makelabel{#1}}%
        \global\setbox\@labels\hbox{ use \@tempboxa here... }
        \ignorespaces}

Last minutes hacks. Some actions must be done after all packages have
been loaded. Some other symbols are defined, or redefined.

.. container:: ltx-source

   ::

      define characters
      \AtBeginDocument{
        \@namedef{Gin@rule@.eps}#1{{eps}{.eps}{#1}}
        \global\let\FOlabel\xFOlabel
        \let\NoTableSetup\myNoTableSetup

        \let\downslopeellipsis\ddots
        \mathchardef\Rightarrow="3229 
         more characters here

        \def\truestate{\models}
        \let\@texttop\ra@useatxy
        \let\XURL\relax
        \errorcontextlines=1000
        \selectlanguage{english}
        \let\@item\jg@item
        \expandafter\let\csname E/:\XMLNS@fo:basic-link\endcsname\patched@basiclink
      }

      \def\L@fr{\selectlanguage{french}}
      \def\L@FR{\selectlanguage{french}}

.. _fotex:

6.10. The fotex.cfg file
========================

The file looks like this. Some lines are commented: they define
quantities unused by the Raweb. Elements hrule and vfill are obsolete?
useless? (I don't remember). Switching to french is valid only for a
French document. The two commands ``\foratheme`` and ``\foinria`` are
defined in the file raweb-cfg.sty.

.. container:: ltx-source

   ::

      \frenchspacing
      \selectlanguage{french}

      \DeclareNamespace{fo}{http://www.w3.org/1999/XSL/Format}

      \XMLelement{fo:RATHEME}
        {}
        {\xmlgrab}
        {\foratheme{#1}}

      % \XMLelement{fo:RRTHEME} ...

      \XMLelement{fo:INRIA}
        {\XMLattribute{year}{\ra@@year}{2001}}
        {}{\xdef\ra@year{\ra@@year}\foinria}

      % \XMLelement{fo:RRINRIA} ...

      \XMLelement{hrule} {} {\par\hrule\par} {}
      \XMLelement{vfill} {} {\vfill} {}

--------------

|Valid XHTML 1.0 Strict| `back to the raweb
documentation <raweb.html#racfg>`__ © INRIA 2004, 2006 Last modified
$Date: 2015/08/31 12:28:12 $

.. |Logo Ra2003| image:: LogoRA2003.png
.. |Logo Ra2002| image:: LogoRA2002.png
.. |mathimage 1| image:: math_image_1.png
   :class: vc
.. |mathimage 2| image:: math_image_2.png
   :class: vc
.. |mathimage 3| image:: math_image_3.png
   :class: vc
.. |mathimage 4| image:: math_image_4.png
   :class: vc
.. |mathimage 5| image:: math_image_5.png
   :class: vc
.. |Valid XHTML 1.0 Strict| image:: http://www.w3.org/Icons/valid-xhtml10
   :width: 88px
   :height: 31px
   :target: http://validator.w3.org/check?uri=referer
