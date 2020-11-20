\DeclareOption{all}{}
\ProcessOptions\relax

\long\def\xymatrix#1{%
\mathbox{xymatrix}{}}
\def\xyoption#1{}
\def\xy#1\endxy{%
\mathbox{xy}{}}
\def\diagram#1\enddiagram{%
\mathbox{diagram}{}}