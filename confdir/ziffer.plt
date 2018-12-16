% -*- latex -*-
% $Id: ziffer.plt,v 1.1 2015/11/25 07:51:35 grimm Exp $
\ProvidesPackage{ziffer}[2017/08/04 v1.6 ziffer for Tralics]
% Author: M. V\"ath      martin@mvath.de


\def\ZifferAn{\ZifferPunktAn\ZifferStrichAn}
\def\ZifferAus{\ZifferPunktAus\ZifferStrichAus}
\def\ZifferPunktAn{\mathcode`,="8000\mathcode`.="8000 }
\def\ZifferPunktAus{\mathcode`,=0\mathcode`.=0 }
\def\ZifferStrichAn{\mathcode`-="8000 }
\def\ZifferStrichAus{\mathcode`-=0 }

%% translation of --
\def\ZifferStrich{\char`-\kern1pt\char`-}
\def\ziffer@MinusOri{\char`-} % mathchar "2200

% tranalation of dot
\def\ZifferLeer{\phantom{.}}
\def\ziffer@DotOri{\char`.}

%translation of comma
\def\ziffer@CommaMy{\char`,}  % matchar  "613B
\def\ziffer@CommaOri{\char`,} % mathchar "013B


{\ZifferAn
 \catcode`\,=\active\gdef,{\begingroup\obeyspaces\futurelet\n\ziffer@ccheck}
 \catcode`\.=\active\gdef.{\begingroup\obeyspaces\futurelet\n\ziffer@dcheck}
 \catcode`\-=\active\gdef-{\begingroup\obeyspaces\futurelet\n\ziffer@mcheck}}

\def\ziffer@ccheck{\ziffer@check\ziffer@CommaMy\ziffer@CommaOri}
\def\ziffer@dcheck{\ziffer@check\ZifferLeer\ziffer@DotOri}
\def\ziffer@mcheck{\ifx\n-\def\n-{\endgroup\ZifferStrich}%
   \else\def\n{\endgroup\ziffer@MinusOri}\fi\n}
\def\ziffer@check#1#2{%
  \ifx\n1\endgroup#1\else
    \ifx\n2\endgroup#1\else
      \ifx\n3\endgroup#1\else
        \ifx\n4\endgroup#1\else
          \ifx\n5\endgroup#1\else
            \ifx\n6\endgroup#1\else
              \ifx\n7\endgroup#1\else
                \ifx\n8\endgroup#1\else
                  \ifx\n9\endgroup#1\else
                    \ifx\n0\endgroup#1\else
                      \endgroup#2%
                    \fi
                  \fi
                \fi
              \fi
            \fi
          \fi
        \fi
      \fi
    \fi
  \fi}
\ZifferPunktAn

\endinput