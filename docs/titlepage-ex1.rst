Two examples for the Titlepage mechanism of Tralics
===================================================

We give here two examples of the use of a configuration file:
`config_tpa <titlepage-conf.html>`__. First example: put it in the file
``tpa.tex``, and run it via ``tralics tpa -configfile config_tpa``.
Explanations can be found `here <titlepage.html>`__.

.. container:: ltx-source

   ::

      \def\cmdAval{CMDA}
      \def\cmdBval{\invalid}
      \def\cmdCval{\invalid}
      \documentclass{tpa}
      \def\cmdAval{\invalid}
      \def\Title{Titlepage customisation in Tralics}


      \title{\Title}
      \author{Jos� Grimm}
      \author{Knuth}
      \myself
      \autk

      \Paris \URsop
      \waddress

      \sParis \sURsop \sURlor

      \begin{Eabstract}
      This is an abstract with \par some paragraphs in it\par ok ?
      \end{Eabstract}

      \begin{eabstract}
      This is an abstract without paragraphs in it
      \end{eabstract}
      \pabstract{Another abstract \par with \par in it}
      \abstractalias{Another abstract without par}

      \def\cmdBval{CMDB}
      \begin{document}
      \def\cmdBval{\invalidagain}

      \def\cmdCval{CMDC}
      \InsertTitlepage

      \end{document}

Second example file for the titlepage command.

.. container:: ltx-source

   ::

      \documentclass[AIF]{cedram}

      \begin{document}

      \title{Fourier coefficients for simple $L^\infty$ functions}
      \alttitle{Coefficients Fourier pour fonctions  $L^\infty$ simples}
      \author{\firstname{Donald} \middlename{E.} \lastname{Knuth}} 

      \address{\TeX\ Users Group \\P.O. Box 869\\
      Santa Barbara, CA 93102-0869 USA}

      \email{d.e.knuth@somewhere.on.the.net} 

      \subjclass{11M26,  11M36, 11S40}

      \keywords{simple $L^\infty$ functions, lambda function}

      \altkeywords{fonctions  $L^\infty$ simples, fonction lambda}

      \daterecieved{2004-06-14}%{14 juin 2004}
      \dateaccepted{2004-12-09}%{9 d�cembre 2004}

      \begin{abstract}
        This is an abstract with a beautiful inline formula % Comment!
        $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n + 
         O(\sqrt{n}\log{n})$, where $C_1(\pi)$ is a real-valued constant.
      \end{abstract}


      \begin{altabstract}
        Mon r�sum� avec ma formule
        $\lambda_n(\pi) = \frac{N}{2} n \log n + C_1(\pi) n + 
         O(\sqrt{n}\log{n})$, o� $C_1(\pi)$ est une constante r�elle.
      \end{altabstract}

      \maketitle

      \section{Introduction}

      The content of the document is unimportant.
      We have a simple math formula $\alpha=\beta$
      and two references \cite{Ba03} and \cite{BPY1}


      \bibliography{bo}

      \end{document}

Bibtex file for the second example:

.. container:: ltx-source

   ::




      @Article{Ba03,
        author =   {Barnes, E. W.},
        title =    {On the expression of {E}uler's constant as a
                        definite integral},
        journal =  {Messenger of Math.},
        year =     1903,
        volume =   33,
        pages =    {59--61}
      }


      @Article{BPY01,
        author =   {P. Biane and J. Pitman and M. Yor},
        title =    {Probability laws related to the {J}acobi $\theta$ and
                        {R}iemann $\zeta$ functions, and {B}rownian excursions},
        journal =  {Bull. Amer. Math. Soc.},
        year =     2001,
        volume =   38,
        pages =    {435--465}
      }


      @Article{Bo99,
        author =   {E. Bombieri},
        title =    {Remarks on {W}eil's quadratic functional in the
                        theory of prime numbers~{I}},
        journal =  {Rend. Mat. Acc. Lincei, Ser.~IX},
        year =     2000,
        volume =   11,
        pages =    {183--233}
      }
