---------
Font test
---------

This files contains `here <#source>`__ a small text that shows all
combinations of latex fonts. We show here `here <#xml1>`__ the XML
result. Each font change is a ``<hi>`` element with some attribute. An
alternate version `here <#xml2>`__ uses elements instead. The result can
be seen `here <#out1>`__ and `here <#out2>`__. We also show the content
of a `config file <#config2>`__, that could be used to produce the
second example.

--------------

This is the tex source:

.. container:: ltx-source

   ::

      \def\Test{
      \textrm{Textrm} \textsf{Textsf} \texttt{Texttt}
      \textbf{Testbf} \textit{Textit} \textsc{Textsc} \textsl{Textsl}
      \textnormal{Textnormal} \textmd{Textmd} \emph{Emph} \textup{Textup}
      \par}

      {\rmfamily \mdseries \upshape rmmdup \Test}
      {\sffamily \mdseries \upshape sfmdup \Test}
      {\ttfamily \mdseries \upshape ttmdup \Test}
      {\rmfamily \bfseries \upshape rmbfup \Test}
      {\sffamily \bfseries \upshape sfbfup \Test}
      {\ttfamily \bfseries \upshape ttbfup \Test}

      {\rmfamily \mdseries \itshape rmmdit \Test}
      {\sffamily \mdseries \itshape sfmdit \Test}
      {\ttfamily \mdseries \itshape ttmdit \Test}
      {\rmfamily \bfseries \itshape rmbfit \Test}
      {\sffamily \bfseries \itshape sfbfit \Test}
      {\ttfamily \bfseries \itshape ttbfit \Test}

      {\rmfamily \mdseries \scshape rmmdsc \Test}
      {\sffamily \mdseries \scshape sfmdsc \Test}
      {\ttfamily \mdseries \scshape ttmdsc \Test}
      {\rmfamily \bfseries \scshape rmbfsc \Test}
      {\sffamily \bfseries \scshape sfbfsc \Test}
      {\ttfamily \bfseries \scshape ttbfsc \Test}

      {\rmfamily \mdseries \slshape rmmdsl \Test}
      {\sffamily \mdseries \slshape sfmdsl \Test}
      {\ttfamily \mdseries \slshape ttmdsl \Test}
      {\rmfamily \bfseries \slshape rmbfsl \Test}
      {\sffamily \bfseries \slshape sfbfsl \Test}
      {\ttfamily \bfseries \slshape ttbfsl \Test}

This is the XML result

.. container:: xml_out

   ::

      <p>rmmdup Textrm <hi rend='sansserif'>Textsf</hi> <hi rend='tt'>Texttt</hi>
      <hi rend='bold'>Testbf</hi> <hi rend='it'>Textit</hi> <hi
      rend='sc'>Textsc</hi> <hi rend='slanted'>Textsl</hi> Textnormal Textmd <hi
      rend='it'>Emph</hi> Textup </p>

      <p><hi rend='sansserif'>sfmdup </hi>Textrm<hi
      rend='sansserif'> Textsf </hi><hi rend='tt'>Texttt</hi><hi rend='sansserif'>
      </hi><hi rend='sansserif'><hi rend='bold'>Testbf</hi></hi><hi
      rend='sansserif'> </hi><hi rend='it'><hi rend='sansserif'>Textit</hi></hi><hi
      rend='sansserif'> </hi><hi rend='sc'><hi rend='sansserif'>Textsc</hi></hi><hi
      rend='sansserif'> </hi><hi rend='slanted'><hi
      rend='sansserif'>Textsl</hi></hi><hi rend='sansserif'> </hi>Textnormal<hi
      rend='sansserif'> Textmd </hi><hi rend='it'><hi
      rend='sansserif'>Emph</hi></hi><hi rend='sansserif'> Textup </hi></p> 

      <p><hi
      rend='tt'>ttmdup </hi>Textrm<hi rend='tt'> </hi><hi
      rend='sansserif'>Textsf</hi><hi rend='tt'> Texttt </hi><hi rend='tt'><hi
      rend='bold'>Testbf</hi></hi><hi rend='tt'> </hi><hi rend='it'><hi
      rend='tt'>Textit</hi></hi><hi rend='tt'> </hi><hi rend='sc'><hi
      rend='tt'>Textsc</hi></hi><hi rend='tt'> </hi><hi rend='slanted'><hi
      rend='tt'>Textsl</hi></hi><hi rend='tt'> </hi>Textnormal<hi rend='tt'> Textmd
      </hi><hi rend='it'><hi rend='tt'>Emph</hi></hi><hi rend='tt'> Textup </hi></p>

      <p><hi rend='bold'>rmbfup Textrm </hi><hi rend='sansserif'><hi
      rend='bold'>Textsf</hi></hi><hi rend='bold'> </hi><hi rend='tt'><hi
      rend='bold'>Texttt</hi></hi><hi rend='bold'> Testbf </hi><hi rend='it'><hi
      rend='bold'>Textit</hi></hi><hi rend='bold'> </hi><hi rend='sc'><hi
      rend='bold'>Textsc</hi></hi><hi rend='bold'> </hi><hi rend='slanted'><hi
      rend='bold'>Textsl</hi></hi><hi rend='bold'> </hi>Textnormal<hi rend='bold'>
      </hi>Textmd<hi rend='bold'> </hi><hi rend='it'><hi
      rend='bold'>Emph</hi></hi><hi rend='bold'> Textup </hi></p> 

      <p><hi
      rend='sansserif'><hi rend='bold'>sfbfup </hi></hi><hi
      rend='bold'>Textrm</hi><hi rend='sansserif'><hi rend='bold'> Textsf
      </hi></hi><hi rend='tt'><hi rend='bold'>Texttt</hi></hi><hi
      rend='sansserif'><hi rend='bold'> Testbf </hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'>Textit</hi></hi></hi><hi rend='sansserif'><hi
      rend='bold'> </hi></hi><hi rend='sc'><hi rend='sansserif'><hi
      rend='bold'>Textsc</hi></hi></hi><hi rend='sansserif'><hi rend='bold'>
      </hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'>Textsl</hi></hi></hi><hi rend='sansserif'><hi rend='bold'>
      </hi></hi>Textnormal<hi rend='sansserif'><hi rend='bold'> </hi></hi><hi
      rend='sansserif'>Textmd</hi><hi rend='sansserif'><hi rend='bold'>
      </hi></hi><hi rend='it'><hi rend='sansserif'><hi
      rend='bold'>Emph</hi></hi></hi><hi rend='sansserif'><hi rend='bold'> Textup
      </hi></hi></p> 

      <p><hi rend='tt'><hi rend='bold'>ttbfup </hi></hi><hi
      rend='bold'>Textrm</hi><hi rend='tt'><hi rend='bold'> </hi></hi><hi
      rend='sansserif'><hi rend='bold'>Textsf</hi></hi><hi rend='tt'><hi
      rend='bold'> Texttt Testbf </hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'>Textit</hi></hi></hi><hi rend='tt'><hi rend='bold'> </hi></hi><hi
      rend='sc'><hi rend='tt'><hi rend='bold'>Textsc</hi></hi></hi><hi rend='tt'><hi
      rend='bold'> </hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Textsl</hi></hi></hi><hi rend='tt'><hi rend='bold'>
      </hi></hi>Textnormal<hi rend='tt'><hi rend='bold'> </hi></hi><hi
      rend='tt'>Textmd</hi><hi rend='tt'><hi rend='bold'> </hi></hi><hi
      rend='it'><hi rend='tt'><hi rend='bold'>Emph</hi></hi></hi><hi rend='tt'><hi
      rend='bold'> Textup </hi></hi></p> 

      <p><hi rend='it'>rmmdit Textrm </hi><hi
      rend='it'><hi rend='sansserif'>Textsf</hi></hi><hi rend='it'> </hi><hi
      rend='it'><hi rend='tt'>Texttt</hi></hi><hi rend='it'> </hi><hi rend='it'><hi
      rend='bold'>Testbf</hi></hi><hi rend='it'> Textit </hi><hi
      rend='sc'>Textsc</hi><hi rend='it'> </hi><hi rend='slanted'>Textsl</hi><hi
      rend='it'> </hi>Textnormal<hi rend='it'> Textmd </hi>Emph<hi rend='it'>
      </hi>Textup<hi rend='it'> </hi></p> 

      <p><hi rend='it'><hi
      rend='sansserif'>sfmdit </hi></hi><hi rend='it'>Textrm</hi><hi rend='it'><hi
      rend='sansserif'> Textsf </hi></hi><hi rend='it'><hi
      rend='tt'>Texttt</hi></hi><hi rend='it'><hi rend='sansserif'> </hi></hi><hi
      rend='it'><hi rend='sansserif'><hi rend='bold'>Testbf</hi></hi></hi><hi
      rend='it'><hi rend='sansserif'> Textit </hi></hi><hi rend='sc'><hi
      rend='sansserif'>Textsc</hi></hi><hi rend='it'><hi rend='sansserif'>
      </hi></hi><hi rend='slanted'><hi rend='sansserif'>Textsl</hi></hi><hi
      rend='it'><hi rend='sansserif'> </hi></hi>Textnormal<hi rend='it'><hi
      rend='sansserif'> Textmd </hi></hi><hi rend='sansserif'>Emph</hi><hi
      rend='it'><hi rend='sansserif'> </hi></hi><hi rend='sansserif'>Textup</hi><hi
      rend='it'><hi rend='sansserif'> </hi></hi></p> 

      <p><hi rend='it'><hi
      rend='tt'>ttmdit </hi></hi><hi rend='it'>Textrm</hi><hi rend='it'><hi
      rend='tt'> </hi></hi><hi rend='it'><hi rend='sansserif'>Textsf</hi></hi><hi
      rend='it'><hi rend='tt'> Texttt </hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'>Testbf</hi></hi></hi><hi rend='it'><hi rend='tt'> Textit
      </hi></hi><hi rend='sc'><hi rend='tt'>Textsc</hi></hi><hi rend='it'><hi
      rend='tt'> </hi></hi><hi rend='slanted'><hi rend='tt'>Textsl</hi></hi><hi
      rend='it'><hi rend='tt'> </hi></hi>Textnormal<hi rend='it'><hi rend='tt'>
      Textmd </hi></hi><hi rend='tt'>Emph</hi><hi rend='it'><hi rend='tt'>
      </hi></hi><hi rend='tt'>Textup</hi><hi rend='it'><hi rend='tt'> </hi></hi></p>

      <p><hi rend='it'><hi rend='bold'>rmbfit Textrm </hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'>Textsf</hi></hi></hi><hi rend='it'><hi
      rend='bold'> </hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'>Texttt</hi></hi></hi><hi rend='it'><hi rend='bold'> Testbf Textit
      </hi></hi><hi rend='sc'><hi rend='bold'>Textsc</hi></hi><hi rend='it'><hi
      rend='bold'> </hi></hi><hi rend='slanted'><hi rend='bold'>Textsl</hi></hi><hi
      rend='it'><hi rend='bold'> </hi></hi>Textnormal<hi rend='it'><hi rend='bold'>
      </hi></hi><hi rend='it'>Textmd</hi><hi rend='it'><hi rend='bold'>
      </hi></hi><hi rend='bold'>Emph</hi><hi rend='it'><hi rend='bold'>
      </hi></hi><hi rend='bold'>Textup</hi><hi rend='it'><hi rend='bold'>
      </hi></hi></p> 

      <p><hi rend='it'><hi rend='sansserif'><hi rend='bold'>sfbfit
      </hi></hi></hi><hi rend='it'><hi rend='bold'>Textrm</hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'> Textsf </hi></hi></hi><hi rend='it'><hi
      rend='tt'><hi rend='bold'>Texttt</hi></hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'> Testbf Textit </hi></hi></hi><hi
      rend='sc'><hi rend='sansserif'><hi rend='bold'>Textsc</hi></hi></hi><hi
      rend='it'><hi rend='sansserif'><hi rend='bold'> </hi></hi></hi><hi
      rend='slanted'><hi rend='sansserif'><hi rend='bold'>Textsl</hi></hi></hi><hi
      rend='it'><hi rend='sansserif'><hi rend='bold'> </hi></hi></hi>Textnormal<hi
      rend='it'><hi rend='sansserif'><hi rend='bold'> </hi></hi></hi><hi
      rend='it'><hi rend='sansserif'>Textmd</hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'> </hi></hi></hi><hi rend='sansserif'><hi
      rend='bold'>Emph</hi></hi><hi rend='it'><hi rend='sansserif'><hi rend='bold'>
      </hi></hi></hi><hi rend='sansserif'><hi rend='bold'>Textup</hi></hi><hi
      rend='it'><hi rend='sansserif'><hi rend='bold'> </hi></hi></hi></p> 

      <p><hi
      rend='it'><hi rend='tt'><hi rend='bold'>ttbfit </hi></hi></hi><hi
      rend='it'><hi rend='bold'>Textrm</hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'> </hi></hi></hi><hi rend='it'><hi rend='sansserif'><hi
      rend='bold'>Textsf</hi></hi></hi><hi rend='it'><hi rend='tt'><hi rend='bold'>
      Texttt Testbf Textit </hi></hi></hi><hi rend='sc'><hi rend='tt'><hi
      rend='bold'>Textsc</hi></hi></hi><hi rend='it'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Textsl</hi></hi></hi><hi rend='it'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi>Textnormal<hi rend='it'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='it'><hi rend='tt'>Textmd</hi></hi><hi rend='it'><hi
      rend='tt'><hi rend='bold'> </hi></hi></hi><hi rend='tt'><hi
      rend='bold'>Emph</hi></hi><hi rend='it'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='tt'><hi rend='bold'>Textup</hi></hi><hi rend='it'><hi
      rend='tt'><hi rend='bold'> </hi></hi></hi></p> 

      <p><hi rend='sc'>rmmdsc Textrm
      </hi><hi rend='sc'><hi rend='sansserif'>Textsf</hi></hi><hi rend='sc'>
      </hi><hi rend='sc'><hi rend='tt'>Texttt</hi></hi><hi rend='sc'> </hi><hi
      rend='sc'><hi rend='bold'>Testbf</hi></hi><hi rend='sc'> </hi><hi
      rend='it'>Textit</hi><hi rend='sc'> Textsc </hi><hi
      rend='slanted'>Textsl</hi><hi rend='sc'> </hi>Textnormal<hi rend='sc'> Textmd
      </hi>Emph<hi rend='sc'> </hi>Textup<hi rend='sc'> </hi></p>

       <p><hi
      rend='sc'><hi rend='sansserif'>sfmdsc </hi></hi><hi rend='sc'>Textrm</hi><hi
      rend='sc'><hi rend='sansserif'> Textsf </hi></hi><hi rend='sc'><hi
      rend='tt'>Texttt</hi></hi><hi rend='sc'><hi rend='sansserif'> </hi></hi><hi
      rend='sc'><hi rend='sansserif'><hi rend='bold'>Testbf</hi></hi></hi><hi
      rend='sc'><hi rend='sansserif'> </hi></hi><hi rend='it'><hi
      rend='sansserif'>Textit</hi></hi><hi rend='sc'><hi rend='sansserif'> Textsc
      </hi></hi><hi rend='slanted'><hi rend='sansserif'>Textsl</hi></hi><hi
      rend='sc'><hi rend='sansserif'> </hi></hi>Textnormal<hi rend='sc'><hi
      rend='sansserif'> Textmd </hi></hi><hi rend='sansserif'>Emph</hi><hi
      rend='sc'><hi rend='sansserif'> </hi></hi><hi rend='sansserif'>Textup</hi><hi
      rend='sc'><hi rend='sansserif'> </hi></hi></p> 

      <p><hi rend='sc'><hi
      rend='tt'>ttmdsc </hi></hi><hi rend='sc'>Textrm</hi><hi rend='sc'><hi
      rend='tt'> </hi></hi><hi rend='sc'><hi rend='sansserif'>Textsf</hi></hi><hi
      rend='sc'><hi rend='tt'> Texttt </hi></hi><hi rend='sc'><hi rend='tt'><hi
      rend='bold'>Testbf</hi></hi></hi><hi rend='sc'><hi rend='tt'> </hi></hi><hi
      rend='it'><hi rend='tt'>Textit</hi></hi><hi rend='sc'><hi rend='tt'> Textsc
      </hi></hi><hi rend='slanted'><hi rend='tt'>Textsl</hi></hi><hi rend='sc'><hi
      rend='tt'> </hi></hi>Textnormal<hi rend='sc'><hi rend='tt'> Textmd
      </hi></hi><hi rend='tt'>Emph</hi><hi rend='sc'><hi rend='tt'> </hi></hi><hi
      rend='tt'>Textup</hi><hi rend='sc'><hi rend='tt'> </hi></hi></p> 

      <p><hi
      rend='sc'><hi rend='bold'>rmbfsc Textrm </hi></hi><hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'>Textsf</hi></hi></hi><hi rend='sc'><hi
      rend='bold'> </hi></hi><hi rend='sc'><hi rend='tt'><hi
      rend='bold'>Texttt</hi></hi></hi><hi rend='sc'><hi rend='bold'> Testbf
      </hi></hi><hi rend='it'><hi rend='bold'>Textit</hi></hi><hi rend='sc'><hi
      rend='bold'> Textsc </hi></hi><hi rend='slanted'><hi
      rend='bold'>Textsl</hi></hi><hi rend='sc'><hi rend='bold'>
      </hi></hi>Textnormal<hi rend='sc'><hi rend='bold'> </hi></hi><hi
      rend='sc'>Textmd</hi><hi rend='sc'><hi rend='bold'> </hi></hi><hi
      rend='bold'>Emph</hi><hi rend='sc'><hi rend='bold'> </hi></hi><hi
      rend='bold'>Textup</hi><hi rend='sc'><hi rend='bold'> </hi></hi></p> 

      <p><hi
      rend='sc'><hi rend='sansserif'><hi rend='bold'>sfbfsc </hi></hi></hi><hi
      rend='sc'><hi rend='bold'>Textrm</hi></hi><hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'> Textsf </hi></hi></hi><hi rend='sc'><hi
      rend='tt'><hi rend='bold'>Texttt</hi></hi></hi><hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'> Testbf </hi></hi></hi><hi rend='it'><hi
      rend='sansserif'><hi rend='bold'>Textit</hi></hi></hi><hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'> Textsc </hi></hi></hi><hi rend='slanted'><hi
      rend='sansserif'><hi rend='bold'>Textsl</hi></hi></hi><hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'> </hi></hi></hi>Textnormal<hi rend='sc'><hi
      rend='sansserif'><hi rend='bold'> </hi></hi></hi><hi rend='sc'><hi
      rend='sansserif'>Textmd</hi></hi><hi rend='sc'><hi rend='sansserif'><hi
      rend='bold'> </hi></hi></hi><hi rend='sansserif'><hi
      rend='bold'>Emph</hi></hi><hi rend='sc'><hi rend='sansserif'><hi rend='bold'>
      </hi></hi></hi><hi rend='sansserif'><hi rend='bold'>Textup</hi></hi><hi
      rend='sc'><hi rend='sansserif'><hi rend='bold'> </hi></hi></hi></p> 

      <p><hi
      rend='sc'><hi rend='tt'><hi rend='bold'>ttbfsc </hi></hi></hi><hi
      rend='sc'><hi rend='bold'>Textrm</hi></hi><hi rend='sc'><hi rend='tt'><hi
      rend='bold'> </hi></hi></hi><hi rend='sc'><hi rend='sansserif'><hi
      rend='bold'>Textsf</hi></hi></hi><hi rend='sc'><hi rend='tt'><hi rend='bold'>
      Texttt Testbf </hi></hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'>Textit</hi></hi></hi><hi rend='sc'><hi rend='tt'><hi rend='bold'>
      Textsc </hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Textsl</hi></hi></hi><hi rend='sc'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi>Textnormal<hi rend='sc'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='sc'><hi rend='tt'>Textmd</hi></hi><hi rend='sc'><hi
      rend='tt'><hi rend='bold'> </hi></hi></hi><hi rend='tt'><hi
      rend='bold'>Emph</hi></hi><hi rend='sc'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='tt'><hi rend='bold'>Textup</hi></hi><hi rend='sc'><hi
      rend='tt'><hi rend='bold'> </hi></hi></hi></p> 

      <p><hi rend='slanted'>rmmdsl
      Textrm </hi><hi rend='slanted'><hi rend='sansserif'>Textsf</hi></hi><hi
      rend='slanted'> </hi><hi rend='slanted'><hi rend='tt'>Texttt</hi></hi><hi
      rend='slanted'> </hi><hi rend='slanted'><hi rend='bold'>Testbf</hi></hi><hi
      rend='slanted'> </hi><hi rend='it'>Textit</hi><hi rend='slanted'> </hi><hi
      rend='sc'>Textsc</hi><hi rend='slanted'> Textsl </hi>Textnormal<hi
      rend='slanted'> Textmd </hi>Emph<hi rend='slanted'> </hi>Textup<hi
      rend='slanted'> </hi></p> 

      <p><hi rend='slanted'><hi rend='sansserif'>sfmdsl
      </hi></hi><hi rend='slanted'>Textrm</hi><hi rend='slanted'><hi
      rend='sansserif'> Textsf </hi></hi><hi rend='slanted'><hi
      rend='tt'>Texttt</hi></hi><hi rend='slanted'><hi rend='sansserif'>
      </hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'>Testbf</hi></hi></hi><hi rend='slanted'><hi rend='sansserif'>
      </hi></hi><hi rend='it'><hi rend='sansserif'>Textit</hi></hi><hi
      rend='slanted'><hi rend='sansserif'> </hi></hi><hi rend='sc'><hi
      rend='sansserif'>Textsc</hi></hi><hi rend='slanted'><hi rend='sansserif'>
      Textsl </hi></hi>Textnormal<hi rend='slanted'><hi rend='sansserif'> Textmd
      </hi></hi><hi rend='sansserif'>Emph</hi><hi rend='slanted'><hi
      rend='sansserif'> </hi></hi><hi rend='sansserif'>Textup</hi><hi
      rend='slanted'><hi rend='sansserif'> </hi></hi></p> 

      <p><hi rend='slanted'><hi
      rend='tt'>ttmdsl </hi></hi><hi rend='slanted'>Textrm</hi><hi
      rend='slanted'><hi rend='tt'> </hi></hi><hi rend='slanted'><hi
      rend='sansserif'>Textsf</hi></hi><hi rend='slanted'><hi rend='tt'> Texttt
      </hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Testbf</hi></hi></hi><hi rend='slanted'><hi rend='tt'>
      </hi></hi><hi rend='it'><hi rend='tt'>Textit</hi></hi><hi rend='slanted'><hi
      rend='tt'> </hi></hi><hi rend='sc'><hi rend='tt'>Textsc</hi></hi><hi
      rend='slanted'><hi rend='tt'> Textsl </hi></hi>Textnormal<hi
      rend='slanted'><hi rend='tt'> Textmd </hi></hi><hi rend='tt'>Emph</hi><hi
      rend='slanted'><hi rend='tt'> </hi></hi><hi rend='tt'>Textup</hi><hi
      rend='slanted'><hi rend='tt'> </hi></hi></p> 

      <p><hi rend='slanted'><hi
      rend='bold'>rmbfsl Textrm </hi></hi><hi rend='slanted'><hi
      rend='sansserif'><hi rend='bold'>Textsf</hi></hi></hi><hi rend='slanted'><hi
      rend='bold'> </hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Texttt</hi></hi></hi><hi rend='slanted'><hi rend='bold'> Testbf
      </hi></hi><hi rend='it'><hi rend='bold'>Textit</hi></hi><hi rend='slanted'><hi
      rend='bold'> </hi></hi><hi rend='sc'><hi rend='bold'>Textsc</hi></hi><hi
      rend='slanted'><hi rend='bold'> Textsl </hi></hi>Textnormal<hi
      rend='slanted'><hi rend='bold'> </hi></hi><hi rend='slanted'>Textmd</hi><hi
      rend='slanted'><hi rend='bold'> </hi></hi><hi rend='bold'>Emph</hi><hi
      rend='slanted'><hi rend='bold'> </hi></hi><hi rend='bold'>Textup</hi><hi
      rend='slanted'><hi rend='bold'> </hi></hi></p> 

      <p><hi rend='slanted'><hi
      rend='sansserif'><hi rend='bold'>sfbfsl </hi></hi></hi><hi rend='slanted'><hi
      rend='bold'>Textrm</hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> Textsf </hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>Texttt</hi></hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> Testbf </hi></hi></hi><hi rend='it'><hi rend='sansserif'><hi
      rend='bold'>Textit</hi></hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> </hi></hi></hi><hi rend='sc'><hi rend='sansserif'><hi
      rend='bold'>Textsc</hi></hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> Textsl </hi></hi></hi>Textnormal<hi rend='slanted'><hi
      rend='sansserif'><hi rend='bold'> </hi></hi></hi><hi rend='slanted'><hi
      rend='sansserif'>Textmd</hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> </hi></hi></hi><hi rend='sansserif'><hi
      rend='bold'>Emph</hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> </hi></hi></hi><hi rend='sansserif'><hi
      rend='bold'>Textup</hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'> </hi></hi></hi></p> 

      <p><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'>ttbfsl </hi></hi></hi><hi rend='slanted'><hi
      rend='bold'>Textrm</hi></hi><hi rend='slanted'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='slanted'><hi rend='sansserif'><hi
      rend='bold'>Textsf</hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'> Texttt Testbf </hi></hi></hi><hi rend='it'><hi rend='tt'><hi
      rend='bold'>Textit</hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'> </hi></hi></hi><hi rend='sc'><hi rend='tt'><hi
      rend='bold'>Textsc</hi></hi></hi><hi rend='slanted'><hi rend='tt'><hi
      rend='bold'> Textsl </hi></hi></hi>Textnormal<hi rend='slanted'><hi
      rend='tt'><hi rend='bold'> </hi></hi></hi><hi rend='slanted'><hi
      rend='tt'>Textmd</hi></hi><hi rend='slanted'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi><hi rend='tt'><hi rend='bold'>Emph</hi></hi><hi
      rend='slanted'><hi rend='tt'><hi rend='bold'> </hi></hi></hi><hi rend='tt'><hi
      rend='bold'>Textup</hi></hi><hi rend='slanted'><hi rend='tt'><hi rend='bold'>
      </hi></hi></hi></p>

Version in which the font is not an attribute.

.. container:: xml_out

   ::

      <p>rmmdup 
      Textrm <sansserif>Textsf</sansserif> <tt>Texttt</tt>
      <bold>Testbf</bold> <it>Textit</it> <sc>Textsc</sc> <slanted>Textsl</slanted>
      Textnormal Textmd <it>Emph</it> Textup</p>
      <p noindent='true'>
      <sansserif>sfmdup 
      </sansserif>Textrm<sansserif> Textsf </sansserif><tt>Texttt</tt><sansserif>
      </sansserif><sansserif><bold>Testbf</bold></sansserif><sansserif> </sansserif>
      <it><sansserif>Textit</sansserif></it><sansserif> </sansserif><sc>
      <sansserif>Textsc</sansserif></sc><sansserif> </sansserif><slanted>
      <sansserif>Textsl</sansserif></slanted><sansserif>
      </sansserif>Textnormal<sansserif> Textmd </sansserif><it>
      <sansserif>Emph</sansserif></it><sansserif> Textup</sansserif></p>
      <p noindent='true'><sansserif/>
      <tt>ttmdup 
      </tt>Textrm<tt> </tt><sansserif>Textsf</sansserif><tt> Texttt
      </tt><tt><bold>Testbf</bold></tt><tt> </tt><it><tt>Textit</tt></it><tt> 
      </tt><sc><tt>Textsc</tt></sc><tt> </tt><slanted><tt>Textsl</tt></slanted><tt>
      </tt>Textnormal<tt> Textmd </tt><it><tt>Emph</tt></it><tt> Textup</tt></p>
      <p noindent='true'><tt/>
      <bold>rmbfup 
      Textrm </bold><sansserif><bold>Textsf</bold></sansserif><bold> </bold><tt>
      <bold>Texttt</bold></tt><bold>
      Testbf </bold><it><bold>Textit</bold></it><bold> </bold><sc>
      <bold>Textsc</bold></sc><bold> </bold><slanted><bold>Textsl</bold></slanted>
      <bold>
      </bold>Textnormal<bold> </bold>Textmd<bold> </bold><it><bold>Emph</bold></it>
      <bold> Textup</bold></p>
      <p noindent='true'><bold/>
      <sansserif/><sansserif><bold>sfbfup 
      </bold></sansserif><bold>Textrm</bold><sansserif><bold> Textsf </bold>
      </sansserif><tt><bold>Texttt</bold></tt><sansserif><bold>
      Testbf </bold></sansserif><it><sansserif><bold>Textit</bold></sansserif>
      </it><sansserif><bold> </bold></sansserif><sc><sansserif><bold>Textsc</bold>
      </sansserif></sc><sansserif><bold> </bold></sansserif><slanted><sansserif>
      <bold>Textsl</bold></sansserif></slanted><sansserif><bold>
      </bold></sansserif>Textnormal<sansserif><bold> </bold></sansserif>
      <sansserif>Textmd</sansserif><sansserif><bold> </bold></sansserif><it>
      <sansserif><bold>Emph</bold></sansserif></it><sansserif><bold> Textup</bold>
      </sansserif></p>
      <p noindent='true'><sansserif><bold/></sansserif>
      <tt/><tt><bold>ttbfup 
      </bold></tt><bold>Textrm</bold><tt><bold> </bold></tt><sansserif>
      <bold>Textsf</bold></sansserif><tt><bold> Texttt
      Testbf </bold></tt><it><tt><bold>Textit</bold></tt></it><tt><bold> </bold>
      </tt><sc><tt><bold>Textsc</bold></tt></sc><tt><bold> </bold></tt><slanted><tt>
      <bold>Textsl</bold></tt></slanted><tt><bold>
      </bold></tt>Textnormal<tt><bold> </bold></tt><tt>Textmd</tt><tt><bold> </bold>
      </tt><it><tt><bold>Emph</bold></tt></it><tt><bold> Textup</bold></tt></p>
      <p noindent='true'><tt><bold/></tt>
      <it>rmmdit 
      Textrm </it><it><sansserif>Textsf</sansserif></it><it> </it><it>
      <tt>Texttt</tt></it><it>
      </it><it><bold>Testbf</bold></it><it> Textit </it><sc>Textsc</sc><it> </it>
      <slanted>Textsl</slanted><it>
      </it>Textnormal<it> Textmd </it>Emph<it> </it>Textup<it></it></p>
      <p noindent='true'><it/>
      <sansserif/><it><sansserif>sfmdit 
      </sansserif></it><it>Textrm</it><it><sansserif> Textsf </sansserif></it><it>
      <tt>Texttt</tt></it><it><sansserif>
      </sansserif></it><it><sansserif><bold>Testbf</bold></sansserif></it><it>
      <sansserif> Textit </sansserif></it><sc><sansserif>Textsc</sansserif></sc>
      <it><sansserif> </sansserif></it><slanted><sansserif>Textsl</sansserif>
      </slanted><it><sansserif>
      </sansserif></it>Textnormal<it><sansserif> Textmd </sansserif></it>
      <sansserif>Emph</sansserif><it><sansserif> </sansserif></it>
      <sansserif>Textup</sansserif><it><sansserif></sansserif></it></p>
      <p noindent='true'><it><sansserif/></it>
      <tt/><it><tt>ttmdit 
      </tt></it><it>Textrm</it><it><tt> </tt></it><it><sansserif>Textsf</sansserif>
      </it><it><tt> Texttt
      </tt></it><it><tt><bold>Testbf</bold></tt></it><it><tt> Textit </tt></it><sc>
      <tt>Textsc</tt></sc><it><tt> </tt></it><slanted><tt>Textsl</tt></slanted><it>
      <tt>
      </tt></it>Textnormal<it><tt> Textmd </tt></it><tt>Emph</tt><it><tt> </tt></it>
      <tt>Textup</tt><it><tt></tt></it></p>
      <p noindent='true'><it><tt/></it>
      <bold/><it><bold>rmbfit 
      Textrm </bold></it><it><sansserif><bold>Textsf</bold></sansserif></it><it>
      <bold> </bold></it><it><tt><bold>Texttt</bold></tt></it><it><bold>
      Testbf Textit </bold></it><sc><bold>Textsc</bold></sc><it><bold> </bold></it>
      <slanted><bold>Textsl</bold></slanted><it><bold>
      </bold></it>Textnormal<it><bold> </bold></it><it>Textmd</it><it><bold> </bold>
      </it><bold>Emph</bold><it><bold> </bold></it><bold>Textup</bold><it><bold>
      </bold></it></p>
      <p noindent='true'><it><bold/></it>
      <sansserif/><sansserif><bold/></sansserif><it><sansserif><bold>sfbfit 
      </bold></sansserif></it><it><bold>Textrm</bold></it><it><sansserif>
      <bold> Textsf </bold></sansserif></it><it><tt><bold>Texttt</bold></tt></it>
      <it><sansserif><bold>
      Testbf Textit </bold></sansserif></it><sc><sansserif><bold>Textsc</bold>
      </sansserif></sc><it><sansserif><bold> </bold></sansserif></it><slanted>
      <sansserif><bold>Textsl</bold></sansserif></slanted><it><sansserif><bold>
      </bold></sansserif></it>Textnormal<it><sansserif><bold> </bold></sansserif>
      </it><it><sansserif>Textmd</sansserif></it><it><sansserif><bold> </bold>
      </sansserif></it><sansserif><bold>Emph</bold></sansserif><it><sansserif>
      <bold> </bold></sansserif></it><sansserif><bold>Textup</bold></sansserif>
      <it><sansserif><bold></bold></sansserif></it></p>
      <p noindent='true'><it><sansserif><bold/></sansserif></it>
      <tt/><tt><bold/></tt><it><tt><bold>ttbfit 
      </bold></tt></it><it><bold>Textrm</bold></it><it><tt><bold> </bold></tt></it>
      <it><sansserif><bold>Textsf</bold></sansserif></it><it><tt><bold> Texttt
      Testbf Textit </bold></tt></it><sc><tt><bold>Textsc</bold></tt></sc><it><tt>
      <bold> </bold></tt></it><slanted><tt><bold>Textsl</bold></tt></slanted><it>
      <tt><bold>
      </bold></tt></it>Textnormal<it><tt><bold> </bold></tt></it><it><tt>Textmd</tt>
      </it><it><tt><bold> </bold></tt></it><tt><bold>Emph</bold></tt><it><tt>
      <bold> </bold></tt></it><tt><bold>Textup</bold></tt><it><tt><bold></bold>
      </tt></it></p>
      <p noindent='true'><it><tt><bold/></tt></it>
      <sc>rmmdsc 
      Textrm </sc><sc><sansserif>Textsf</sansserif></sc><sc> </sc><sc>
      <tt>Texttt</tt></sc><sc>
      </sc><sc><bold>Testbf</bold></sc><sc> </sc><it>Textit</it><sc> Textsc </sc>
      <slanted>Textsl</slanted><sc>
      </sc>Textnormal<sc> Textmd </sc>Emph<sc> </sc>Textup<sc></sc></p>
      <p noindent='true'><sc/>
      <sansserif/><sc><sansserif>sfmdsc 
      </sansserif></sc><sc>Textrm</sc><sc><sansserif> Textsf </sansserif></sc><sc>
      <tt>Texttt</tt></sc><sc><sansserif>
      </sansserif></sc><sc><sansserif><bold>Testbf</bold></sansserif></sc><sc>
      <sansserif> </sansserif></sc><it><sansserif>Textit</sansserif></it><sc>
      <sansserif> Textsc </sansserif></sc><slanted><sansserif>Textsl</sansserif>
      </slanted><sc><sansserif>
      </sansserif></sc>Textnormal<sc><sansserif> Textmd </sansserif></sc>
      <sansserif>Emph</sansserif><sc><sansserif> </sansserif></sc>
      <sansserif>Textup</sansserif><sc><sansserif></sansserif></sc></p>
      <p noindent='true'><sc><sansserif/></sc>
      <tt/><sc><tt>ttmdsc 
      </tt></sc><sc>Textrm</sc><sc><tt> </tt></sc><sc><sansserif>Textsf</sansserif>
      </sc><sc><tt> Texttt
      </tt></sc><sc><tt><bold>Testbf</bold></tt></sc><sc><tt> </tt></sc><it>
      <tt>Textit</tt></it><sc><tt> Textsc </tt></sc><slanted><tt>Textsl</tt>
      </slanted><sc><tt>
      </tt></sc>Textnormal<sc><tt> Textmd </tt></sc><tt>Emph</tt><sc><tt> </tt>
      </sc><tt>Textup</tt><sc><tt></tt></sc></p>
      <p noindent='true'><sc><tt/></sc>
      <bold/><sc><bold>rmbfsc 
      Textrm </bold></sc><sc><sansserif><bold>Textsf</bold></sansserif></sc><sc>
      <bold> </bold></sc><sc><tt><bold>Texttt</bold></tt></sc><sc><bold>
      Testbf </bold></sc><it><bold>Textit</bold></it><sc><bold> Textsc </bold></sc>
      <slanted><bold>Textsl</bold></slanted><sc><bold>
      </bold></sc>Textnormal<sc><bold> </bold></sc><sc>Textmd</sc><sc><bold> </bold>
      </sc><bold>Emph</bold><sc><bold> </bold></sc><bold>Textup</bold><sc><bold>
      </bold></sc></p>
      <p noindent='true'><sc><bold/></sc>
      <sansserif/><sansserif><bold/></sansserif><sc><sansserif><bold>sfbfsc 
      </bold></sansserif></sc><sc><bold>Textrm</bold></sc><sc><sansserif>
      <bold> Textsf </bold></sansserif></sc><sc><tt><bold>Texttt</bold></tt></sc>
      <sc><sansserif><bold>
      Testbf </bold></sansserif></sc><it><sansserif><bold>Textit</bold></sansserif>
      </it><sc><sansserif><bold> Textsc </bold></sansserif></sc><slanted><sansserif>
      <bold>Textsl</bold></sansserif></slanted><sc><sansserif><bold>
      </bold></sansserif></sc>Textnormal<sc><sansserif><bold> </bold></sansserif>
      </sc><sc><sansserif>Textmd</sansserif></sc><sc><sansserif><bold> </bold>
      </sansserif></sc><sansserif><bold>Emph</bold></sansserif><sc><sansserif>
      <bold> </bold></sansserif></sc><sansserif><bold>Textup</bold></sansserif><sc>
      <sansserif><bold></bold></sansserif></sc></p>
      <p noindent='true'><sc><sansserif><bold/></sansserif></sc>
      <tt/><tt><bold/></tt><sc><tt><bold>ttbfsc 
      </bold></tt></sc><sc><bold>Textrm</bold></sc><sc><tt><bold> </bold></tt></sc>
      <sc><sansserif><bold>Textsf</bold></sansserif></sc><sc><tt><bold> Texttt
      Testbf </bold></tt></sc><it><tt><bold>Textit</bold></tt></it><sc><tt>
      <bold> Textsc </bold></tt></sc><slanted><tt><bold>Textsl</bold></tt>
      </slanted><sc><tt><bold>
      </bold></tt></sc>Textnormal<sc><tt><bold> </bold></tt></sc><sc><tt>Textmd</tt>
      </sc><sc><tt><bold> </bold></tt></sc><tt><bold>Emph</bold></tt><sc><tt>
      <bold> </bold></tt></sc><tt><bold>Textup</bold></tt><sc><tt><bold></bold>
      </tt></sc></p>
      <p noindent='true'><sc><tt><bold/></tt></sc>
      <slanted>rmmdsl 
      Textrm </slanted><slanted><sansserif>Textsf</sansserif></slanted>
      <slanted> </slanted><slanted><tt>Texttt</tt></slanted><slanted>
      </slanted><slanted><bold>Testbf</bold></slanted><slanted> </slanted>
      <it>Textit</it><slanted> </slanted><sc>Textsc</sc><slanted> Textsl
      </slanted>Textnormal<slanted> Textmd </slanted>Emph<slanted> </slanted>Textup<slanted></slanted></p>
      <p noindent='true'><slanted/>
      <sansserif/><slanted><sansserif>sfmdsl 
      </sansserif></slanted><slanted>Textrm</slanted><slanted>
      <sansserif> Textsf </sansserif></slanted><slanted><tt>Texttt</tt></slanted>
      <slanted><sansserif>
      </sansserif></slanted><slanted><sansserif><bold>Testbf</bold></sansserif>
      </slanted><slanted><sansserif> </sansserif></slanted><it>
      <sansserif>Textit</sansserif></it><slanted><sansserif> </sansserif>
      </slanted><sc><sansserif>Textsc</sansserif></sc><slanted><sansserif> Textsl
      </sansserif></slanted>Textnormal<slanted><sansserif> Textmd </sansserif>
      </slanted><sansserif>Emph</sansserif><slanted><sansserif> </sansserif>
      </slanted><sansserif>Textup</sansserif><slanted><sansserif></sansserif>
      </slanted></p>
      <p noindent='true'><slanted><sansserif/></slanted>
      <tt/><slanted><tt>ttmdsl 
      </tt></slanted><slanted>Textrm</slanted><slanted><tt> </tt></slanted>
      <slanted><sansserif>Textsf</sansserif></slanted><slanted><tt> Texttt
      </tt></slanted><slanted><tt><bold>Testbf</bold></tt></slanted><slanted>
      <tt> </tt></slanted><it><tt>Textit</tt></it><slanted><tt> </tt></slanted><sc>
      <tt>Textsc</tt></sc><slanted><tt> Textsl
      </tt></slanted>Textnormal<slanted><tt> Textmd </tt></slanted><tt>Emph</tt>
      <slanted><tt> </tt></slanted><tt>Textup</tt><slanted><tt></tt></slanted></p>
      <p noindent='true'><slanted><tt/></slanted>
      <bold/><slanted><bold>rmbfsl 
      Textrm </bold></slanted><slanted><sansserif><bold>Textsf</bold></sansserif>
      </slanted><slanted><bold> </bold></slanted><slanted><tt><bold>Texttt</bold>
      </tt></slanted><slanted><bold>
      Testbf </bold></slanted><it><bold>Textit</bold></it><slanted><bold> </bold>
      </slanted><sc><bold>Textsc</bold></sc><slanted><bold> Textsl
      </bold></slanted>Textnormal<slanted><bold> </bold></slanted>
      <slanted>Textmd</slanted><slanted><bold> </bold></slanted>
      <bold>Emph</bold><slanted><bold> </bold></slanted><bold>Textup</bold>
      <slanted><bold></bold></slanted></p>
      <p noindent='true'><slanted><bold/></slanted>
      <sansserif/><sansserif><bold/></sansserif><slanted><sansserif><bold>sfbfsl 
      </bold></sansserif></slanted><slanted><bold>Textrm</bold></slanted><slanted>
      <sansserif><bold> Textsf </bold></sansserif></slanted><slanted><tt>
      <bold>Texttt</bold></tt></slanted><slanted><sansserif><bold>
      Testbf </bold></sansserif></slanted><it><sansserif><bold>Textit</bold>
      </sansserif></it><slanted><sansserif><bold> </bold></sansserif></slanted><sc>
      <sansserif><bold>Textsc</bold></sansserif></sc><slanted><sansserif>
      <bold> Textsl
      </bold></sansserif></slanted>Textnormal<slanted><sansserif><bold> </bold>
      </sansserif></slanted><slanted><sansserif>Textmd</sansserif></slanted>
      <slanted><sansserif><bold> </bold></sansserif></slanted><sansserif>
      <bold>Emph</bold></sansserif><slanted><sansserif><bold> </bold></sansserif>
      </slanted><sansserif><bold>Textup</bold></sansserif><slanted><sansserif>
      <bold></bold></sansserif></slanted></p>
      <p noindent='true'><slanted><sansserif><bold/></sansserif></slanted>
      <tt/><tt><bold/></tt><slanted><tt><bold>ttbfsl 
      </bold></tt></slanted><slanted><bold>Textrm</bold></slanted><slanted><tt>
      <bold> </bold></tt></slanted><slanted><sansserif><bold>Textsf</bold>
      </sansserif></slanted><slanted><tt><bold> Texttt
      Testbf </bold></tt></slanted><it><tt><bold>Textit</bold></tt></it><slanted>
      <tt><bold> </bold></tt></slanted><sc><tt><bold>Textsc</bold></tt></sc>
      <slanted><tt><bold> Textsl
      </bold></tt></slanted>Textnormal<slanted><tt><bold> </bold></tt></slanted>
      <slanted><tt>Textmd</tt></slanted><slanted><tt><bold> </bold></tt></slanted>
      <tt><bold>Emph</bold></tt><slanted><tt><bold> </bold></tt></slanted><tt><
      bold>Textup</bold></tt><slanted><tt><bold></bold></tt></slanted></p>
      <p noindent='true'><slanted><tt><bold/></tt></slanted></p>

Preview |all font variants|

New version. Preview |all font variants 2|

Configuration source file. This file gives longert names than the
example above; but our XSLT processor handles the long names and the
short names identically.

.. container:: ltx-source

   ::

        use_font_elt = "true"
        xml_font_small = "font-small"
        xml_font_small1 = "font-small1"
        xml_font_small2 = "font-small2"
        xml_font_small3 = "font-small3"
        xml_font_small4 = "font-small4"
        xml_font_large = "font-large"
        xml_font_large1 = "font-large1"
        xml_font_large2 = "font-large2"
        xml_font_large3 = "font-large3"
        xml_font_large4 = "font-large4"
        xml_font_large5 = "font-large5"
        xml_font_normalsize = "font-normalsize"
        xml_font_upright = "font-upright-shape"
        xml_font_medium = "font-medium-series"
        xml_font_roman = "font-roman-family"
        xml_font_it = "font-italic-shape"
        xml_font_slanted = "font-slanted-shape"
        xml_font_sc = "font-small-caps-shape"
        xml_font_tt = "font-typewriter-family"
        xml_font_sansserif = "font-sansserif-family"
        xml_font_bold = "font-bold-series"

.. |all font variants| image:: /img/img_40.png
.. |all font variants 2| image:: /img/img_122.png
