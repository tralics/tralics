% -*- latex -*-
%% par defaut : op
\providecommand\PrintTeXMML[2]{%
%\ifdefined#1\message{Redéfinition #1 :-> #2^^J}\fi
\def#1{%
\nmORmm0Rnm{\string#1}%
{\mathmo{#2}}%
{\string#1}}}
%
\providecommand\PrintTeXMMLi[2]{%
%\ifdefined#1\message{Redéfinition #1 :-> #2^^J}\fi
\def#1{%
\nmORmm0Rnm{\string#1}%
{\mathmi{#2}}%
{\string#1}}}
%
\providecommand\PrintTeXMMLa[2]{%
%\ifdefined#1\message{Redéfinition #1 :-> #2^^J}\fi
\def#1{%
\nmORmm0Rnm{\string#1}%
{#2}%
{\string#1}}}
%% Redef. commandes connues de tralics 
\PrintTeXMMLi\emptyset                    {\amp empty;}%P
\PrintTeXMMLi\hbar                     {\amp hslash;}%A
\PrintTeXMMLi\prime                     {\amp prime;}%A
%%%
%\PrintTeXMMLa\ddddot                   {\amp DotDot;}%A
%\PrintTeXMMLa\dddot                    {\amp tdot;}%A
%% ??
\PrintTeXMML\ddotseq                  {\amp eDDot;}%A
%%
%% commandes plain inconnues de tralics
% \PrintTeXMML\emdash                   {\amp mdash;}%A
% \PrintTeXMML\endash                   {\amp ndash;}%A
\PrintTeXMML\cdotp                    {\amp middot;}%P
%%295 ajout grimm
%\PrintTeXMML\colon                    {\amp ratio;}%P
%\PrintTeXMML\intercal                 {\amp intcal;}%P
% \PrintTeXMML\Lsh                      {\amp lsh;}%P
% \PrintTeXMML\Rsh                      {\amp rsh;}%P
\PrintTeXMML\thickspace               {\amp emsp14;}%P
%% commandes amslatex inconnues de tralics
\PrintTeXMML\acwcirclearrow           {\amp olarr;}%A
%\PrintTeXMML\approxeq                 {\amp ape;}%A
\PrintTeXMML\backcong                 {\amp bcong;}%A
% \PrintTeXMML\backsim                  {\amp bsim;}%A
% \PrintTeXMML\backsimeq                {\amp bsime;}%A
% \PrintTeXMML\barwedge                 {\amp barwed;}%A
% \PrintTeXMML\because                  {\amp becaus;}%A
% \PrintTeXMML\beth                     {\amp beth;}%A
% \PrintTeXMML\between                  {\amp twixt;}%A
\PrintTeXMML\bigcupdot                {\amp cupdot;}%A
% \PrintTeXMML\bigsqcap                 {\amp sqcap;}%A
\PrintTeXMML\bigstar                  {\amp starf;}%A
\PrintTeXMML\bigtimes                 {\amp times;}%A
%\PrintTeXMML\blacklozenge             {\amp lozf;}%A
\PrintTeXMML\blacksquare              {\amp squarf;}%A
% \PrintTeXMML\blacktriangledown        {\amp dtrif;}%A
% \PrintTeXMML\blacktriangleleft        {\amp ltrif;}%A
% \PrintTeXMML\blacktriangleright       {\amp rtrif;}%A
% \PrintTeXMML\blacktriangle            {\amp utrif;}%A
\PrintTeXMML\boxdiag                  {\amp solb;}%A
% \PrintTeXMML\boxdot                   {\amp sdotb;}%A
% \PrintTeXMML\boxminus                 {\amp minusb;}%A
% \PrintTeXMML\boxplus                  {\amp plusb;}%A
% \PrintTeXMML\boxtimes                 {\amp timesb;}%A
% \PrintTeXMML\Bumpeq                   {\amp bump;}%A
% \PrintTeXMML\bumpeq                   {\amp bumpe;}%A
%\PrintTeXMML\Cap                      {\amp Cap;}%A
\PrintTeXMML\checkmark                {\amp check;}%A
% \PrintTeXMML\circeq                   {\amp cire;}%A
% \PrintTeXMML\circledast               {\amp oast;}%A
% \PrintTeXMML\circledcirc              {\amp ocir;}%A
% \PrintTeXMML\circleddash              {\amp odash;}%A
% \PrintTeXMML\circledS                 {\amp oS;}%A
\PrintTeXMML\Colon                    {\amp Colon;}%A
\PrintTeXMML\coloneq                  {\amp colone;}%A
%\PrintTeXMML\complement               {\amp comp;}%A
%\PrintTeXMML\Cup                      {\amp Cup;}%A
\PrintTeXMML\cupdot                   {\amp cupdot;}%A
% \PrintTeXMML\curlyeqprec              {\amp cuepr;}%A
% \PrintTeXMML\curlyeqsucc              {\amp cuesc;}%A
% \PrintTeXMML\curlyvee                 {\amp cuvee;}%A
% \PrintTeXMML\curlywedge               {\amp cuwed;}%A
\PrintTeXMML\curvearrowleft           {\amp cularr;}%A
\PrintTeXMML\curvearrowright          {\amp curarr;}%A
\PrintTeXMML\cwcirclearrow            {\amp orarr;}%A
%\PrintTeXMML\daleth                   {\amp daleth;}%A
\PrintTeXMML\Dashv                    {\amp Dashv;}%A
%%% Pb d'affichage cf. \dasharrow
\PrintTeXMML\dbkarow                  {\amp rBarr;}%A
% \PrintTeXMML\divideontimes            {\amp divonx;}%A
% \PrintTeXMML\Doteq                    {\amp eDot;}%A
\PrintTeXMML\dotminus                 {\amp minusd;}%A
%\PrintTeXMML\dotplus                  {\amp plusdo;}%A
\PrintTeXMML\doublebarwedge           {\amp Barwed;}%A
% \PrintTeXMML\downdownarrows           {\amp ddarr;}%A
% \PrintTeXMML\downharpoonleft          {\amp dharl;}%A
% \PrintTeXMML\downharpoonright         {\amp dharr;}%A
\PrintTeXMML\drbkarow                 {\amp RBarr;}%A
\PrintTeXMML\drbkarrow                {\amp RBarr;}%A
% \PrintTeXMML\eqcirc                   {\amp ecir;}%A
\PrintTeXMML\eqcolon                  {\amp ecolon;}%A
% \PrintTeXMML\eqsim                    {\amp esim;}%A
% \PrintTeXMML\eqslantgtr               {\amp egs;}%A
% \PrintTeXMML\eqslantless              {\amp els;}%A
% \PrintTeXMML\fallingdotseq            {\amp efDot;}%A
% \PrintTeXMML\geqq                     {\amp gE;}%A
% \PrintTeXMML\ggg                      {\amp Gg;}%A
% \PrintTeXMML\gimel                    {\amp gimel;}%A
% \PrintTeXMML\gnapprox                 {\amp gnap;}%A
% \PrintTeXMML\gneq                     {\amp gne;}%A
% \PrintTeXMML\gneqq                    {\amp gnE;}%A
% \PrintTeXMML\gnsim                    {\amp gnsim;}%A
\PrintTeXMML\greater                  {\amp gt;}%A
% \PrintTeXMML\gtrapprox                {\amp gap;}%A
% \PrintTeXMML\gtrdot                   {\amp gtdot;}%A
% \PrintTeXMML\gtreqless                {\amp gel;}%A
% \PrintTeXMML\gtreqqless               {\amp gEl;}%A
% \PrintTeXMML\gtrsim                   {\amp gsim;}%A
\PrintTeXMML\gvertneqq                {\amp gvnE;}%A
\PrintTeXMML\hksearow                 {\amp searhk;}%A
\PrintTeXMML\hkswarow                 {\amp swarhk;}%A
\PrintTeXMML\hslash                   {\amp plankv;}%A
%\PrintTeXMML\iiiint                   {\amp qint;}%A
%\PrintTeXMML\iiint                    {\amp tint;}%A
%\PrintTeXMML\iint                     {\amp Int;}%A
\PrintTeXMML\intBar                   {\amp fpartint;}%A*
\PrintTeXMML\intbar                   {\amp fpartint;}%A
\PrintTeXMML\intprod                  {\amp iprod;}%A
\PrintTeXMML\kernelcontraction        {\amp homtht;}%A
% \PrintTeXMML\leftarrowtail            {\amp larrtl;}%A
% \PrintTeXMML\leftleftarrows           {\amp llarr;}%A
% \PrintTeXMML\leftrightarrows          {\amp lrarr;}%A
% \PrintTeXMML\leftrightharpoons        {\amp lrhar;}%A
% \PrintTeXMML\leftrightsquigarrow      {\amp harrw;}%A
% \PrintTeXMML\leftthreetimes           {\amp lthree;}%A
% \PrintTeXMML\leqq                     {\amp lE;}%A
% \PrintTeXMML\lessapprox               {\amp lap;}%A
% \PrintTeXMML\lessdot                  {\amp ltdot;}%A
% \PrintTeXMML\lesseqgtr                {\amp leg;}%A
% \PrintTeXMML\lesseqqgtr               {\amp lEg;}%A
% \PrintTeXMML\lessgtr                  {\amp lg;}%A
\PrintTeXMML\less                     {\amp lt;}%A
%\PrintTeXMML\lesssim                  {\amp lsim;}%A
\PrintTeXMML\llcorner                 {\amp dlcorn;}%A
% \PrintTeXMML\Lleftarrow               {\amp lAarr;}%A
% \PrintTeXMML\lll                      {\amp Ll;}%A
% \PrintTeXMML\lnapprox                 {\amp lnap;}%A
% \PrintTeXMML\lneq                     {\amp lne;}%A
% \PrintTeXMML\lneqq                    {\amp lnE;}%A
% \PrintTeXMML\lnsim                    {\amp lnsim;}%A
% \PrintTeXMML\looparrowleft            {\amp larrlp;}%A
% \PrintTeXMML\looparrowright           {\amp rarrlp;}%A
\PrintTeXMML\lozenge                  {\amp loz;}%A
\PrintTeXMML\lrcorner                 {\amp drcorn;}%A
\PrintTeXMML\ltimes                   {\amp ltimes;}%A
\PrintTeXMML\lvertneqq                {\amp lvnE;}%A
\PrintTeXMML\maltese                  {\amp malt;}%A
% \PrintTeXMML\measuredangle            {\amp angmsd;}%A
%\PrintTeXMML\mho                      {\amp mho;}%A
\PrintTeXMML\minusdot                 {\amp minusdu;}%A
\PrintTeXMML\minus                    {\amp minus;}%A
\PrintTeXMML\mlcp                     {\amp mlcp;}%A
\PrintTeXMML\napprox                  {\amp nap;}%A
%\PrintTeXMML\ncong                    {\amp ncong;}%A
\PrintTeXMML\nequiv                   {\amp nequiv;}%A
%\PrintTeXMML\nexists                  {\amp nexist;}%A
%\PrintTeXMML\ngeq                     {\amp nge;}%A
\PrintTeXMML\ngeqq                    {\amp ngE;}%A
\PrintTeXMML\ngeqslant                {\amp nges;}%A
%\PrintTeXMML\ngtr                     {\amp ngt;}%A
%\PrintTeXMML\nleftarrow               {\amp nlarr;}%A
%\PrintTeXMML\nLeftarrow               {\amp nlArr;}%A
% \PrintTeXMML\nleftrightarrow          {\amp nharr;}%A
% \PrintTeXMML\nLeftrightarrow          {\amp nhArr;}%A
% \PrintTeXMML\nleq                     {\amp nle;}%A
\PrintTeXMML\nleqq                    {\amp nlE;}%A
\PrintTeXMML\nleqslant                {\amp nles;}%A
% \PrintTeXMML\nless                    {\amp nlt;}%A
% \PrintTeXMML\nmid                     {\amp nmid;}%A
\PrintTeXMML\nparallel                {\amp npar;}%A
\PrintTeXMML\npreceq                  {\amp npre;}%A
%\PrintTeXMML\nprec                    {\amp npr;}%A
% \PrintTeXMML\nrightarrow              {\amp nrarr;}%A
% \PrintTeXMML\nRightarrow              {\amp nrArr;}%A
\PrintTeXMML\nshortmid                {\amp nsmid;}%A
\PrintTeXMML\nshortparallel           {\amp nspar;}%A
\PrintTeXMML\nsime                    {\amp nsime;}%A
% \PrintTeXMML\nsim                     {\amp nsim;}%A
% \PrintTeXMML\nsubseteq                {\amp nsube;}%A
\PrintTeXMML\nsubseteqq               {\amp nsubE;}%A
\PrintTeXMML\nsubset                  {\amp nsub;}%A
%\PrintTeXMML\nsucceq                  {\amp nsce;}%A
% \PrintTeXMML\nsucc                    {\amp nsc;}%A
% \PrintTeXMML\nsupseteq                {\amp nsupe;}%A
\PrintTeXMML\nsupseteqq               {\amp nsupE;}%A
\PrintTeXMML\nsupset                  {\amp nsup;}%A
% \PrintTeXMML\ntrianglelefteq          {\amp nltrie;}%A
% \PrintTeXMML\ntriangleleft            {\amp nltri;}%A
% \PrintTeXMML\ntrianglerighteq         {\amp nrtrie;}%A
% \PrintTeXMML\ntriangleright           {\amp nrtri;}%A
% \PrintTeXMML\nvdash                   {\amp nvdash;}%A
% \PrintTeXMML\nvDash                   {\amp nvDash;}%A
% \PrintTeXMML\nVdash                   {\amp nVdash;}%A
% \PrintTeXMML\nVDash                   {\amp nVDash;}%A
\PrintTeXMML\ocirc                    {\amp ring;}%A
% \PrintTeXMML\pitchfork                {\amp fork;}%A
\PrintTeXMML\plusdot                  {\amp plusdu;}%A
% \PrintTeXMML\precapprox               {\amp prap;}%A
% \PrintTeXMML\preccurlyeq              {\amp prcue;}%A
% \PrintTeXMML\precnapprox              {\amp prnap;}%A
% \PrintTeXMML\precneqq                 {\amp prnE;}%A
% \PrintTeXMML\precnsim                 {\amp prnsim;}%A
% \PrintTeXMML\precsim                  {\amp prsim;}%A
\PrintTeXMML\questeq                  {\amp equest;}%A
% \PrintTeXMML\rightarrowtail           {\amp rarrtl;}%A
% \PrintTeXMML\rightleftarrows          {\amp rlarr;}%A
% \PrintTeXMML\rightleftharpoons        {\amp rlhar;}%A
% \PrintTeXMML\rightrightarrows         {\amp rrarr;}%A
% \PrintTeXMML\rightsquigarrow          {\amp rarrw;}%A
% \PrintTeXMML\rightthreetimes          {\amp rthree;}%A
% \PrintTeXMML\risingdotseq             {\amp erDot;}%A
% \PrintTeXMML\Rrightarrow              {\amp rAarr;}%A
% \PrintTeXMML\rtimes                   {\amp rtimes;}%A
\PrintTeXMML\shortmid                 {\amp smid;}%A
\PrintTeXMML\shortparallel            {\amp spar;}%A
\PrintTeXMML\smallfrown               {\amp frown;}%A
%\PrintTeXMML\smallsetminus            {\amp setminus;}%A*
\PrintTeXMML\smallsmile               {\amp ssmile;}%A
%\PrintTeXMML\sphericalangle           {\amp angsph;}%A
\PrintTeXMML\sterling                 {\amp pound;}%A
% \PrintTeXMML\subseteqq                {\amp subE;}%A
% \PrintTeXMML\subsetneqq               {\amp subnE;}%A
% \PrintTeXMML\subsetneq                {\amp subne;}%A
% \PrintTeXMML\Subset                   {\amp Sub;}%A
% \PrintTeXMML\succapprox               {\amp scap;}%A
% \PrintTeXMML\succcurlyeq              {\amp sccue;}%A
% \PrintTeXMML\succnapprox              {\amp scnap;}%A
% \PrintTeXMML\succneqq                 {\amp scnE;}%A
% \PrintTeXMML\succnsim                 {\amp scnsim;}%A
% \PrintTeXMML\succsim                  {\amp scsim;}%A
% \PrintTeXMML\supseteqq                {\amp supE;}%A
% \PrintTeXMML\supsetneqq               {\amp supnE;}%A
% \PrintTeXMML\supsetneq                {\amp supne;}%A
% \PrintTeXMML\Supset                   {\amp Sup;}%A
% \PrintTeXMML\therefore                {\amp there4;}%A
% \PrintTeXMML\thickapprox              {\amp approx;}%A
% \PrintTeXMML\thicksim                 {\amp sim;}%A
\PrintTeXMML\toea                     {\amp nesear;}%A
\PrintTeXMML\tona                     {\amp nwnear;}%A
\PrintTeXMML\tosa                     {\amp seswar;}%A
\PrintTeXMML\towa                     {\amp swnwar;}%A
% \PrintTeXMML\triangledown             {\amp dtri;}%A
% \PrintTeXMML\trianglelefteq           {\amp ltrie;}%A
% \PrintTeXMML\triangleq                {\amp trie;}%A
% \PrintTeXMML\trianglerighteq          {\amp rtrie;}%A
% \PrintTeXMML\twoheadleftarrow         {\amp Larr;}%A
% \PrintTeXMML\twoheadrightarrow        {\amp Rarr;}%A
\PrintTeXMML\ulcorner                 {\amp ulcorn;}%A
% \PrintTeXMML\upharpoonleft            {\amp uharl;}%A
% \PrintTeXMML\upharpoonright           {\amp uharr;}%A
% \PrintTeXMML\upuparrows               {\amp uuarr;}%A
\PrintTeXMML\urcorner                 {\amp urcorn;}%A
% \PrintTeXMML\varnothing               {\amp emptyv;}%A
\PrintTeXMML\varpropto                {\amp vprop;}%A
\PrintTeXMML\varsubsetneqq            {\amp vsubnE;}%A
\PrintTeXMML\varsubsetneq             {\amp vsubne;}%A
\PrintTeXMML\varsupsetneqq            {\amp vsupnE;}%A
\PrintTeXMML\varsupsetneq             {\amp vsupne;}%A
% \PrintTeXMML\vartriangleleft          {\amp vltri;}%A
% \PrintTeXMML\vartriangleright         {\amp vrtri;}%A
\PrintTeXMML\vartriangle              {\amp utri;}%A
% \PrintTeXMML\vDash                    {\amp vDash;}%A
% \PrintTeXMML\Vvdash                   {\amp Vvdash;}%A
\PrintTeXMML\wedgeq                   {\amp wedgeq;}%A
\PrintTeXMML\yen                      {\amp yen;}%A
%%% var Greek (amsmath)
\PrintTeXMML\varGamma{\amp \#x1D6E4;}
\PrintTeXMML\varDelta{\amp \#x1D6E5;}
\PrintTeXMML\varTheta{\amp \#x1D6E9;}
\PrintTeXMML\varLambda{\amp \#x1D6EC;}
\PrintTeXMML\varXi{\amp \#x1D6EF;}
\PrintTeXMML\varPi{\amp \#x1D6F1;}
\PrintTeXMML\varSigma{\amp \#x1D6F4;}
\PrintTeXMML\varUpsilon{\amp \#x1D6F6;}
\PrintTeXMML\varPhi{\amp \#x1D6F7;}
\PrintTeXMML\varPsi{\amp \#x1D6F9;}
\PrintTeXMML\varOmega{\amp \#x1D6FA;}
\PrintTeXMMLi\Sha{\amp\#x428;}%