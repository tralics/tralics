% 
% $Id: tralics-iso.plt,v 2.3 2007/11/29 11:47:02 grimm Exp $
\ProvidesPackage{tralics-iso}[2006/09/16: version 1.2. 
  support for Unicode characters in Tralics, J Grimm]

%% Copyright Inria, Jose Grimm, Apics, 2004.
%% This file is distributed with Tralics. See the Copyright notice
%% in the Tralics main directory

%% This software is governed by the CeCILL license under French law and
%% abiding by the rules of distribution of free software.  You can  use, 
%% modify and/ or redistribute the software under the terms of the CeCILL
%% license as circulated by CEA, CNRS and INRIA at the following URL
%% "http://www.cecill.info". 
%% (See the file COPYING in the main directory for details)

% This package defines commands like \mathslarr
% to be equivalent to \mathmo{&#x02190;}
% The name is math, followded by the Unicode name.
% In case of conflict, \math@slarr will be used.

% Includes: \mathord (x02A5D), \mathring (x002DA)
% \mathdagger (x02020) \mathDagger (x02021) \mathit(x02062)

% Copyright (Inria) 2004.
%     Entity names in this file are derived from files carrying the
%     following notice:
%
%     (C) International Organization for Standardization 1991
%     Permission to copy in any form is granted for use with
%     conforming SGML systems and applications as defined in
%     ISO 8879, provided this notice is included in all copies.


\def\makecmd#1{\expandafter\newcommand\csname math#1\endcsname}

\def\makemo#1#2{\makecmd{#2}{\mathmo{\amp\##1;}}}
\def\makemi#1#2{\makecmd{#2}{\mathmi{\amp\##1;}}}
\def\makemn#1#2{\makecmd{#2}{\mathmn{\amp\##1;}}}


%% These are from iso9573-13/isoamsa.ent
\makemo{x02190}{slarr}    %short left arrow 
\makemo{x02192}{srarr}    %short right arrow 
\makemo{x02194}{harr}    %/leftrightarrow A: l&r arrow 
\makemo{x02195}{varr}    %/updownarrow A: up&down arrow 
\makemo{x02196}{nwarr}    %/nwarrow A: NW pointing arrow 
\makemo{x02197}{nearr}    %/nearrow A: NE pointing arrow 
\makemo{x02198}{searr}    %/searrow A: SE pointing arrow 
\makemo{x02199}{swarr}    %/swarrow A: SW pointing arrow 
\makemo{x0219A}{nlarr}    %/nleftarrow A: not left arrow 
\makemo{x0219B}{nrarr}    %/nrightarrow A: not right arrow 
\makemo{x0219D}{rarrw}    %/rightsquigarrow A: rt arrow-wavy 
\makemo{x0219E}{Larr}    %/twoheadleftarrow A: 
\makemo{x0219F}{Uarr}    %up two-headed arrow 
\makemo{x021A0}{Rarr}    %/twoheadrightarrow A: 
\makemo{x021A1}{Darr}    %down two-headed arrow 
\makemo{x021A2}{larrtl}    %/leftarrowtail A: left arrow-tailed 
\makemo{x021A3}{rarrtl}    %/rightarrowtail A: rt arrow-tailed 
\makemo{x021A6}{map}    %/mapsto A: 
\makemo{x021A9}{larrhk}    %/hookleftarrow A: left arrow-hooked 
\makemo{x021AA}{rarrhk}    %/hookrightarrow A: rt arrow-hooked 
\makemo{x021AB}{larrlp}    %/looparrowleft A: left arrow-looped 
\makemo{x021AC}{rarrlp}    %/looparrowright A: rt arrow-looped 
\makemo{x021AD}{harrw}    %/leftrightsquigarrow A: l&r arr-wavy 
\makemo{x021AE}{nharr}    %/nleftrightarrow A: not l&r arrow 
\makemo{x021B0}{lsh}    %/Lsh A: 
\makemo{x021B1}{rsh}    %/Rsh A: 
\makemo{x021B2}{ldsh}    %left down angled arrow 
\makemo{x021B3}{rdsh}    %right down angled arrow 
\makemo{x021B6}{cularr}    %/curvearrowleft A: left curved arrow 
\makemo{x021B7}{curarr}    %/curvearrowright A: rt curved arrow 
\makemo{x021BA}{olarr}    %/circlearrowleft A: l arr in circle 
\makemo{x021BB}{orarr}    %/circlearrowright A: r arr in circle 
\makemo{x021BC}{lharu}    %/leftharpoonup A: left harpoon-up 
\makemo{x021BD}{lhard}    %/leftharpoondown A: l harpoon-down 
\makemo{x021BE}{uharr}    %/upharpoonright /restriction A: up harp-r 
\makemo{x021BF}{uharl}    %/upharpoonleft A: up harpoon-left 
\makemo{x021C0}{rharu}    %/rightharpoonup A: rt harpoon-up 
\makemo{x021C1}{rhard}    %/rightharpoondown A: rt harpoon-down 
\makemo{x021C2}{dharr}    %/downharpoonright A: down harpoon-rt 
\makemo{x021C3}{dharl}    %/downharpoonleft A: dn harpoon-left 
\makemo{x021C4}{rlarr}    %/rightleftarrows A: r arr over l arr 
\makemo{x021C5}{udarr}    %up arrow, down arrow 
\makemo{x021C6}{lrarr}    %/leftrightarrows A: l arr over r arr 
\makemo{x021C7}{llarr}    %/leftleftarrows A: two left arrows 
\makemo{x021C8}{uuarr}    %/upuparrows A: two up arrows 
\makemo{x021C9}{rrarr}    %/rightrightarrows A: two rt arrows 
\makemo{x021CA}{ddarr}    %/downdownarrows A: two down arrows 
\makemo{x021CB}{lrhar}    %/leftrightharpoons A: l harp over r 
\makemo{x021CC}{rlhar}    %/rightleftharpoons A: r harp over l 
\makemo{x021CD}{nlArr}    %/nLeftarrow A: not implied by 
\makemo{x021CE}{nhArr}    %/nLeftrightarrow A: not l&r dbl arr 
\makemo{x021CF}{nrArr}    %/nRightarrow A: not implies 
\makemo{x021D1}{uArr}    %/Uparrow A: up dbl arrow 
\makemo{x021D3}{dArr}    %/Downarrow A: down dbl arrow 
\makemo{x021D4}{hArr}    %/Leftrightarrow A: l&r dbl arrow 
\makemo{x021D5}{vArr}    %/Updownarrow A: up&down dbl arrow 
\makemo{x021D6}{nwArr}    %NW pointing dbl arrow 
\makemo{x021D7}{neArr}    %NE pointing dbl arrow 
\makemo{x021D8}{seArr}    %SE pointing dbl arrow 
\makemo{x021D9}{swArr}    %SW pointing dbl arrow 
\makemo{x021DA}{lAarr}    %/Lleftarrow A: left triple arrow 
\makemo{x021DB}{rAarr}    %/Rrightarrow A: right triple arrow 
\makemo{x021DD}{zigrarr}    %right zig-zag arrow 
\makemo{x021F5}{duarr}    %down arrow, up arrow 
\makemo{x021FD}{loarr}    %left open arrow 
\makemo{x021FE}{roarr}    %right open arrow 
\makemo{x021FF}{hoarr}    %horizontal open arrow 
\makemo{x022B6}{origof}    %original of 
\makemo{x022B7}{imof}    %image of 
\makemo{x022B8}{mumap}    %/multimap A: 
\makemo{x0237C}{angzarr}    %angle with down zig-zag arrow 
\makemo{x027F5}{xlarr}    %/longleftarrow A: long left arrow 
\makemo{x027F6}{xrarr}    %/longrightarrow A: long right arrow 
\makemo{x027F7}{xharr}    %/longleftrightarrow A: long l&r arr 
\makemo{x027F8}{xlArr}    %/Longleftarrow A: long l dbl arrow 
\makemo{x027F9}{xrArr}    %/Longrightarrow A: long rt dbl arr 
\makemo{x027FA}{xhArr}    %/Longleftrightarrow A: long l&r dbl arr 
\makemo{x027FC}{xmap}    %/longmapsto A: 
\makemo{x027FF}{dzigrarr}    %right long zig-zag arrow 
\makemo{x02902}{nvlArr}    %not, vert, left double arrow 
\makemo{x02903}{nvrArr}    %not, vert, right double arrow 
\makemo{x02904}{nvHarr}    %not, vert, left and right double arrow 
\makemo{x02905}{Map}    %twoheaded mapsto 
\makemo{x0290C}{lbarr}    %left broken arrow 
\makemo{x0290D}{rbarr}    %/bkarow A: right broken arrow 
\makemo{x0290E}{lBarr}    %left doubly broken arrow 
\makemo{x0290F}{rBarr}    %/dbkarow A: right doubly broken arrow 
\makemo{x02910}{RBarr}    %/drbkarow A: twoheaded right broken arrow 
\makemo{x02911}{DDotrahd}    %right arrow with dotted stem 
\makemo{x02916}{Rarrtl}    %right two-headed arrow with tail 
\makemo{x02919}{latail}    %left arrow-tail 
\makemo{x0291A}{ratail}    %right arrow-tail 
\makemo{x0291B}{lAtail}    %left double arrow-tail 
\makemo{x0291C}{rAtail}    %right double arrow-tail 
\makemo{x0291D}{larrfs}    %left arrow, filled square 
\makemo{x0291E}{rarrfs}    %right arrow, filled square 
\makemo{x0291F}{larrbfs}    %left arrow-bar, filled square 
\makemo{x02920}{rarrbfs}    %right arrow-bar, filled square 
\makemo{x02923}{nwarhk}    %NW arrow-hooked 
\makemo{x02924}{nearhk}    %NE arrow-hooked 
\makemo{x02925}{searhk}    %/hksearow A: SE arrow-hooken 
\makemo{x02926}{swarhk}    %/hkswarow A: SW arrow-hooked 
\makemo{x02927}{nwnear}    %NW & NE arrows 
\makemo{x02928}{nesear}    %/toea A: NE & SE arrows 
\makemo{x02929}{seswar}    %/tosa A: SE & SW arrows 
\makemo{x0292A}{swnwar}    %SW & NW arrows 
\makemo{x02933}{rarrc}    %right arrow-curved 
\makemo{x02935}{cudarrr}    %right, curved, down arrow 
\makemo{x02936}{ldca}    %left down curved arrow 
\makemo{x02937}{rdca}    %right down curved arrow 
\makemo{x02938}{cudarrl}    %left, curved, down arrow 
\makemo{x02939}{larrpl}    %left arrow, plus 
\makemo{x0293C}{curarrm}    %curved right arrow with minus 
\makemo{x0293D}{cularrp}    %curved left arrow with plus 
\makemo{x02945}{rarrpl}    %right arrow, plus 
\makemo{x02948}{harrcir}    %left and right arrow with a circle 
\makemo{x02949}{Uarrocir}    %up two-headed arrow above circle 
\makemo{x0294A}{lurdshar}    %left-up-right-down harpoon 
\makemo{x0294B}{ldrushar}    %left-down-right-up harpoon 
\makemo{x02962}{lHar}    %left harpoon-up over left harpoon-down 
\makemo{x02963}{uHar}    %up harpoon-left, up harpoon-right 
\makemo{x02964}{rHar}    %right harpoon-up over right harpoon-down 
\makemo{x02965}{dHar}    %down harpoon-left, down harpoon-right 
\makemo{x02966}{luruhar}    %left harpoon-up over right harpoon-up 
\makemo{x02967}{ldrdhar}    %left harpoon-down over right harpoon-down 
\makemo{x02968}{ruluhar}    %right harpoon-up over left harpoon-up 
\makemo{x02969}{rdldhar}    %right harpoon-down over left harpoon-down 
\makemo{x0296A}{lharul}    %left harpoon-up over long dash 
\makemo{x0296B}{llhard}    %left harpoon-down below long dash 
\makemo{x0296C}{rharul}    %right harpoon-up over long dash 
\makemo{x0296D}{lrhard}    %right harpoon-down below long dash 
\makemo{x0296E}{udhar}    %up harp, down harp 
\makemo{x0296F}{duhar}    %down harp, up harp 
\makemo{x02971}{erarr}    %equal, right arrow below 
\makemo{x02972}{simrarr}    %similar, right arrow below 
\makemo{x02973}{larrsim}    %left arrow, similar 
\makemo{x02974}{rarrsim}    %right arrow, similar 
\makemo{x02975}{rarrap}    %approximate, right arrow above 
\makemo{x0297C}{lfisht}    %left fish tail 
\makemo{x0297D}{rfisht}    %right fish tail 
\makemo{x0297E}{ufisht}    %up fish tail 
\makemo{x0297F}{dfisht}    %down fish tail 
\makemo{x02AEF}{cirmid}    %circle, mid below 
\makemo{x02AF0}{midcir}    %mid, circle below 


%% These are from iso9573-13/isoamsb.ent

\makemo{x0002A}{midast}    %/ast B: asterisk 
\makemo{x02020}{dagger}    %/dagger B: dagger relation 
\makemo{x02021}{Dagger}    %/ddagger B: double dagger relation 
\makemo{x0220F}{prod}    %/prod L: product operator 
\makemo{x02210}{coprod}    %/coprod L: coproduct operator 
\makemo{x02211}{sum}    %/sum L: summation operator 
\makemo{x02214}{plusdo}    %/dotplus B: plus sign, dot above 
\makemo{x02216}{setmn}    %/setminus B: reverse solidus 
\makemo{x02216}{ssetmn}    %/smallsetminus B: sm reverse solidus 
\makemo{x02238}{minusd}    %/dotminus B: minus sign, dot above 
\makemo{x0223E}{ac}    %most positive 
\makemo{x02240}{wreath}    %/wr B: wreath product 
\makemo{x0228D}{cupdot}    %union, with dot 
\makemo{x0228E}{uplus}    %/uplus B: plus sign in union 
\makemo{x02293}{sqcap}    %/sqcap B: square intersection 
\makemo{x02294}{sqcup}    %/sqcup B: square union 
\makemo{x02295}{oplus}    %/oplus B: plus sign in circle 
\makemo{x02296}{ominus}    %/ominus B: minus sign in circle 
\makemo{x02297}{otimes}    %/otimes B: multiply sign in circle 
\makemo{x02298}{osol}    %/oslash B: solidus in circle 
\makemo{x02299}{odot}    %/odot B: middle dot in circle 
\makemo{x0229A}{ocir}    %/circledcirc B: small circle in circle 
\makemo{x0229B}{oast}    %/circledast B: asterisk in circle 
\makemo{x0229D}{odash}    %/circleddash B: hyphen in circle 
\makemo{x0229E}{plusb}    %/boxplus B: plus sign in box 
\makemo{x0229F}{minusb}    %/boxminus B: minus sign in box 
\makemo{x022A0}{timesb}    %/boxtimes B: multiply sign in box 
\makemo{x022A1}{sdotb}    %/dotsquare /boxdot B: small dot in box 
\makemo{x022B9}{hercon}    %hermitian conjugate matrix 
\makemo{x022BA}{intcal}    %/intercal B: intercal 
\makemo{x022BB}{veebar}    %/veebar B: logical or, bar below 
\makemo{x022BD}{barvee}    %bar, vee 
\makemo{x022C0}{xwedge}    %/bigwedge L: logical or operator 
\makemo{x022C1}{xvee}    %/bigvee L: logical and operator 
\makemo{x022C2}{xcap}    %/bigcap L: intersection operator 
\makemo{x022C3}{xcup}    %/bigcup L: union operator 
\makemo{x022C4}{diam}    %/diamond B: open diamond 
\makemo{x022C5}{sdot}    %/cdot B: small middle dot 
\makemo{x022C6}{sstarf}    %/star B: small star, filled 
\makemo{x022C7}{divonx}    %/divideontimes B: division on times 
\makemo{x022C9}{ltimes}    %/ltimes B: times sign, left closed 
\makemo{x022CA}{rtimes}    %/rtimes B: times sign, right closed 
\makemo{x022CB}{lthree}    %/leftthreetimes B: 
\makemo{x022CC}{rthree}    %/rightthreetimes B: 
\makemo{x022CE}{cuvee}    %/curlyvee B: curly logical or 
\makemo{x022CF}{cuwed}    %/curlywedge B: curly logical and 
\makemo{x022D2}{Cap}    %/Cap /doublecap B: dbl intersection 
\makemo{x022D3}{Cup}    %/Cup /doublecup B: dbl union 
\makemo{x02305}{barwed}    %/barwedge B: logical and, bar above 
\makemo{x02306}{Barwed}    %/doublebarwedge B: log and, dbl bar above 
\makemo{x0233D}{ovbar}    %circle with vertical bar 
\makemo{x025B3}{xutri}    %/bigtriangleup B: big up tri, open 
\makemo{x025BD}{xdtri}    %/bigtriangledown B: big dn tri, open 
\makemo{x025EC}{tridot}    %dot in triangle 
\makemo{x025EF}{xcirc}    %/bigcirc B: large circle 
\makemo{x029B5}{ohbar}    %circle with horizontal bar 
\makemo{x029B6}{omid}    %vertical bar in circle 
\makemo{x029B7}{opar}    %parallel in circle 
\makemo{x029B9}{operp}    %perpendicular in circle 
\makemo{x029BC}{odsold}    %dot, solidus, dot in circle 
\makemo{x029BE}{olcir}    %large circle in circle 
\makemo{x029BF}{ofcir}    %filled circle in circle 
\makemo{x029C0}{olt}    %less-than in circle 
\makemo{x029C1}{ogt}    %greater-than in circle 
\makemo{x029C4}{solb}    %solidus in square 
\makemo{x029C5}{bsolb}    %reverse solidus in square 
\makemo{x029CD}{trisb}    %triangle, serifs at bottom 
\makemo{x029DA}{race}    %reverse most positive, line below 
\makemo{x02A00}{xodot}    %/bigodot L: circle dot operator 
\makemo{x02A01}{xoplus}    %/bigoplus L: circle plus operator 
\makemo{x02A02}{xotime}    %/bigotimes L: circle times operator 
\makemo{x02A04}{xuplus}    %/biguplus L: 
\makemo{x02A06}{xsqcup}    %/bigsqcup L: square union operator 
\makemo{x02A22}{pluscir}    %plus, small circle above 
\makemo{x02A23}{plusacir}    %plus, circumflex accent above 
\makemo{x02A24}{simplus}    %plus, similar above 
\makemo{x02A25}{plusdu}    %plus sign, dot below 
\makemo{x02A26}{plussim}    %plus, similar below 
\makemo{x02A27}{plustwo}    %plus, two; Nim-addition 
\makemo{x02A2A}{minusdu}    %minus sign, dot below 
\makemo{x02A2D}{loplus}    %plus sign in left half circle 
\makemo{x02A2E}{roplus}    %plus sign in right half circle 
\makemo{x02A30}{timesd}    %times, dot 
\makemo{x02A31}{timesbar}    %multiply sign, bar below 
\makemo{x02A33}{smashp}    %smash product 
\makemo{x02A34}{lotimes}    %multiply sign in left half circle 
\makemo{x02A35}{rotimes}    %multiply sign in right half circle 
\makemo{x02A36}{otimesas}    %multiply sign in circle, circumflex accent 
\makemo{x02A37}{Otimes}    %multiply sign in double circle 
\makemo{x02A38}{odiv}    %divide in circle 
\makemo{x02A39}{triplus}    %plus in triangle 
\makemo{x02A3A}{triminus}    %minus in triangle 
\makemo{x02A3B}{tritime}    %multiply in triangle 
\makemo{x02A3C}{iprod}    %/intprod 
\makemo{x02A3F}{amalg}    %/amalg B: amalgamation or coproduct 
\makemo{x02A40}{capdot}    %intersection, with dot 
\makemo{x02A42}{ncup}    %bar, union 
\makemo{x02A43}{ncap}    %bar, intersection 
\makemo{x02A44}{capand}    %intersection, and 
\makemo{x02A45}{cupor}    %union, or 
\makemo{x02A46}{cupcap}    %union above intersection 
\makemo{x02A47}{capcup}    %intersection above union 
\makemo{x02A48}{cupbrcap}    %union, bar, intersection 
\makemo{x02A49}{capbrcup}    %intersection, bar, union 
\makemo{x02A4A}{cupcup}    %union, union, joined 
\makemo{x02A4B}{capcap}    %intersection, intersection, joined 
\makemo{x02A4C}{ccups}    %closed union, serifs 
\makemo{x02A4D}{ccaps}    %closed intersection, serifs 
\makemo{x02A50}{ccupssm}    %closed union, serifs, smash product 
\makemo{x02A5F}{wedbar}    %wedge, bar below 
\makemo{x02A71}{eplus}    %equal, plus 
\makemo{x02A72}{pluse}    %plus, equals 
\makemo{x02ABD}{subdot}    %subset, with dot 
\makemo{x02ABE}{supdot}    %superset, with dot 

%% These are from iso9573-13/isoamsc.ent
\makemo{x02308}{lceil}    %/lceil O: left ceiling 
\makemo{x02309}{rceil}    %/rceil C: right ceiling 
\makemo{x0230A}{lfloor}    %/lfloor O: left floor 
\makemo{x0230B}{rfloor}    %/rfloor C: right floor 
\makemo{x0231C}{ulcorn}    %/ulcorner O: upper left corner 
\makemo{x0231D}{urcorn}    %/urcorner C: upper right corner 
\makemo{x0231E}{dlcorn}    %/llcorner O: lower left corner 
\makemo{x0231F}{drcorn}    %/lrcorner C: lower right corner 
\makemo{x023B0}{lmoust}    %/lmoustache 
\makemo{x023B1}{rmoust}    %/rmoustache 
\makemo{x0298B}{lbrke}    %left bracket, equal 
\makemo{x0298C}{rbrke}    %right bracket, equal 
\makemo{x0298D}{lbrkslu}    %left bracket, solidus top corner 
\makemo{x0298E}{rbrksld}    %right bracket, solidus bottom corner 
\makemo{x0298F}{lbrksld}    %left bracket, solidus bottom corner 
\makemo{x02990}{rbrkslu}    %right bracket, solidus top corner 
\makemo{x02991}{langd}    %left angle, dot 
\makemo{x02992}{rangd}    %right angle, dot 
\makemo{x02993}{lparlt}    %O: left parenthesis, lt 
\makemo{x02994}{rpargt}    %C: right paren, gt 
\makemo{x02995}{gtlPar}    %dbl left parenthesis, greater 
\makemo{x02996}{ltrPar}    %dbl right parenthesis, less 

%% These are from iso9573-13/isoamsn.ent
\makemo{x02224}{nmid}    %/nmid 
\makemo{x02224}{nsmid}    %/nshortmid 
\makemo{x02226}{npar}    %/nparallel N: not parallel 
\makemo{x02226}{nspar}    %/nshortparallel N: not short par 
\makemo{x02241}{nsim}    %/nsim N: not similar 
\makemo{x02244}{nsime}    %/nsimeq N: not similar, equals 
\makemo{x02246}{simne}    %similar, not equals 
\makemo{x02247}{ncong}    %/ncong N: not congruent with 
\makemo{x02249}{nap}    %/napprox N: not approximate 
\makemo{x02262}{nequiv}    %/nequiv N: not identical with 
\makemo{x02268}{lnE}    %/lneqq N: less, not double equals 
\makemo{x02269}{gnE}    %/gneqq N: greater, not dbl equals 
\makemo{x0226E}{nlt}    %/nless N: not less-than 
\makemo{x0226F}{ngt}    %/ngtr N: not greater-than 
\makemo{x02270}{nle}    %/nleq N: not less-than-or-equal 
\makemo{x02271}{nge}    %/ngeq N: not greater-than-or-equal 
\makemo{x02274}{nlsim}    %not less, similar 
\makemo{x02275}{ngsim}    %not greater, similar 
\makemo{x02278}{ntlg}    %not less, greater 
\makemo{x02279}{ntgl}    %not greater, less 
\makemo{x02280}{npr}    %/nprec N: not precedes 
\makemo{x02281}{nsc}    %/nsucc N: not succeeds 
\makemo{x02284}{nsub}    %not subset 
\makemo{x02285}{nsup}    %not superset 
\makemo{x02288}{nsube}    %/nsubseteq N: not subset, equals 
\makemo{x02289}{nsupe}    %/nsupseteq N: not superset, equals 
\makemo{x0228A}{subne}    %/subsetneq N: subset, not equals 
\makemo{x0228B}{supne}    %/supsetneq N: superset, not equals 
\makemo{x022AC}{nvdash}    %/nvdash N: not vertical, dash 
\makemo{x022AD}{nvDash}    %/nvDash N: not vertical, dbl dash 
\makemo{x022AE}{nVdash}    %/nVdash N: not dbl vertical, dash 
\makemo{x022AF}{nVDash}    %/nVDash N: not dbl vert, dbl dash 
\makemo{x022E0}{nprcue}    %not curly precedes, eq 
\makemo{x022E1}{nsccue}    %not succeeds, curly eq 
\makemo{x022E2}{nsqsube}    %not, square subset, equals 
\makemo{x022E3}{nsqsupe}    %not, square superset, equals 
\makemo{x022E6}{lnsim}    %/lnsim N: less, not similar 
\makemo{x022E7}{gnsim}    %/gnsim N: greater, not similar 
\makemo{x022E8}{prnsim}    %/precnsim N: precedes, not similar 
\makemo{x022E9}{scnsim}    %/succnsim N: succeeds, not similar 
\makemo{x022EA}{nltri}    %/ntriangleleft N: not left triangle 
\makemo{x022EB}{nrtri}    %/ntriangleright N: not rt triangle 
\makemo{x022EC}{nltrie}    %/ntrianglelefteq N: not l tri, eq 
\makemo{x022ED}{nrtrie}    %/ntrianglerighteq N: not r tri, eq 
\makemo{x0233F}{solbar}    %solidus, bar through 
\makemo{x02A87}{lne}    %/lneq N: less, not equals 
\makemo{x02A88}{gne}    %/gneq N: greater, not equals 
\makemo{x02A89}{lnap}    %/lnapprox N: less, not approximate 
\makemo{x02A8A}{gnap}    %/gnapprox N: greater, not approximate 
\makemo{x02AB5}{prnE}    %/precneqq N: precedes, not dbl eq 
\makemo{x02AB6}{scnE}    %/succneqq N: succeeds, not dbl eq 
\makemo{x02AB9}{prnap}    %/precnapprox N: precedes, not approx 
\makemo{x02ABA}{scnap}    %/succnapprox N: succeeds, not approx 
\makemo{x02ACB}{subnE}    %/subsetneqq N: subset, not dbl eq 
\makemo{x02ACC}{supnE}    %/supsetneqq N: superset, not dbl eq 
\makemo{x02AEE}{rnmid}    %reverse /nmid 
\makemo{x02AF3}{parsim}    %parallel, similar 

%% These are from iso9573-13/isoamso.ent
\makemo{x0006A}{jmath}    %/jmath - small j, no dot 
\makemo{x00131}{imath}    %/imath - small i, no dot 
\makemo{x02035}{bprime}    %/backprime - reverse prime 
\makemo{x0204F}{bsemi}    %reverse semi-colon 
\makemo{x0210F}{planck}    %/hbar - Planck's over 2pi 
\makemo{x0210F}{plankv}    %/hslash - variant Planck's over 2pi 
\makemo{x02111}{image}    %/Im - imaginary 
\makemo{x02113}{ell}    %/ell - cursive small l 
\makemo{x02118}{weierp}    %/wp - Weierstrass p 
\makemo{x0211C}{real}    %/Re - real 
\makemo{x02127}{mho}    %/mho - conductance 
\makemo{x02129}{iiota}    %inverted iota 
\makemo{x02136}{beth}    %/beth - beth, Hebrew 
\makemo{x02137}{gimel}    %/gimel - gimel, Hebrew 
\makemo{x02138}{daleth}    %/daleth - daleth, Hebrew 
\makemo{x02201}{comp}    %/complement - complement sign 
\makemo{x02204}{nexist}    %/nexists - negated exists 
\makemo{x02205}{emptyv}    %/varnothing - circle, slash 
\makemo{x02205}{empty}    %/emptyset - zero, slash 
\makemo{x02220}{ang}    %/angle - angle 
\makemo{x02221}{angmsd}    %/measuredangle - angle-measured 
\makemo{x022BE}{angrtvb}    %right angle-measured 
\makemo{x022BF}{lrtri}    %lower right triangle 
\makemo{x023B4}{tbrk}    %top square bracket 
\makemo{x023B5}{bbrk}    %bottom square bracket 
\makemo{x023B6}{bbrktbrk}    %bottom above top square bracket 
\makemo{x024C8}{oS}    %/circledS - capital S in circle 
\makemo{x025F8}{ultri}    %upper left triangle 
\makemo{x025F9}{urtri}    %upper right triangle 
\makemo{x025FA}{lltri}    %lower left triangle 
\makemo{x0299A}{vzigzag}    %vertical zig-zag line 
\makemo{x0299D}{angrtvbd}    %right angle-measured, dot 
\makemo{x029A4}{ange}    %angle, equal 
\makemo{x029A5}{range}    %reverse angle, equal 
\makemo{x029A8}{angmsdaa}    %angle-measured, arrow, up, right 
\makemo{x029A9}{angmsdab}    %angle-measured, arrow, up, left 
\makemo{x029AA}{angmsdac}    %angle-measured, arrow, down, right 
\makemo{x029AB}{angmsdad}    %angle-measured, arrow, down, left 
\makemo{x029AC}{angmsdae}    %angle-measured, arrow, right, up 
\makemo{x029AD}{angmsdaf}    %angle-measured, arrow, left, up 
\makemo{x029AE}{angmsdag}    %angle-measured, arrow, right, down 
\makemo{x029AF}{angmsdah}    %angle-measured, arrow, left, down 
\makemo{x029B0}{bemptyv}    %reversed circle, slash 
\makemo{x029B1}{demptyv}    %circle, slash, bar above 
\makemo{x029B2}{cemptyv}    %circle, slash, small circle above 
\makemo{x029B3}{raemptyv}    %circle, slash, right arrow above 
\makemo{x029B4}{laemptyv}    %circle, slash, left arrow above 
\makemo{x029C2}{cirscir}    %circle, small circle to the right 
\makemo{x029C3}{cirE}    %circle, two horizontal stroked to the right 
\makemo{x029C9}{boxbox}    %two joined squares 
\makemo{x0FFFD}{trpezium}    %trapezium 

%% These are from iso9573-13/isoamsr.ent
\makemo{x003F6}{bepsi}    %/backepsilon R: such that 
\makemo{x0221D}{vprop}    %/varpropto R: proportional, variant 
\makemo{x02223}{mid}    %/mid R: 
\makemo{x02223}{smid}    %/shortmid R: 
\makemo{x02225}{spar}    %/shortparallel R: short parallel 
\makemo{x02236}{ratio}    %/ratio 
\makemo{x02237}{Colon}    %/Colon, two colons 
\makemo{x0223A}{mDDot}    %minus with four dots, geometric properties 
\makemo{x0223B}{homtht}    %homothetic 
\makemo{x0223C}{thksim}    %/thicksim R: thick similar 
\makemo{x0223D}{bsim}    %/backsim R: reverse similar 
\makemo{x0223E}{mstpos}    %most positive 
\makemo{x02242}{esim}    %/esim R: equals, similar 
\makemo{x02248}{asymp}    %/asymp R: asymptotically equal to 
\makemo{x02248}{thkap}    %/thickapprox R: thick approximate 
\makemo{x0224A}{ape}    %/approxeq R: approximate, equals 
\makemo{x0224B}{apid}    %approximately identical to 
\makemo{x0224C}{bcong}    %/backcong R: reverse congruent 
\makemo{x0224E}{bump}    %/Bumpeq R: bumpy equals 
\makemo{x0224F}{bumpe}    %/bumpeq R: bumpy equals, equals 
\makemo{x02250}{esdot}    %/doteq R: equals, single dot above 
\makemo{x02251}{eDot}    %/doteqdot /Doteq R: eq, even dots 
\makemo{x02252}{efDot}    %/fallingdotseq R: eq, falling dots 
\makemo{x02253}{erDot}    %/risingdotseq R: eq, rising dots 
\makemo{x02254}{colone}    %/coloneq R: colon, equals 
\makemo{x02255}{ecolon}    %/eqcolon R: equals, colon 
\makemo{x02256}{ecir}    %/eqcirc R: circle on equals sign 
\makemo{x02257}{cire}    %/circeq R: circle, equals 
\makemo{x0225C}{trie}    %/triangleq R: triangle, equals 
\makemo{x0225F}{equest}    %/questeq R: equal with questionmark 
\makemo{x02266}{lE}    %/leqq R: less, double equals 
\makemo{x02267}{gE}    %/geqq R: greater, double equals 
\makemo{x0226A}{Lt}    %/ll R: double less-than sign 
\makemo{x0226B}{Gt}    %/gg R: dbl greater-than sign 
\makemo{x0226C}{twixt}    %/between R: between 
\makemo{x02272}{lsim}    %/lesssim R: less, similar 
\makemo{x02273}{gsim}    %/gtrsim R: greater, similar 
\makemo{x02276}{lg}    %/lessgtr R: less, greater 
\makemo{x02277}{gl}    %/gtrless R: greater, less 
\makemo{x0227A}{pr}    %/prec R: precedes 
\makemo{x0227B}{sc}    %/succ R: succeeds 
\makemo{x0227C}{prcue}    %/preccurlyeq R: precedes, curly eq 
\makemo{x0227D}{sccue}    %/succcurlyeq R: succeeds, curly eq 
\makemo{x0227E}{prsim}    %/precsim R: precedes, similar 
\makemo{x0227F}{scsim}    %/succsim R: succeeds, similar 
\makemo{x0228F}{sqsub}    %/sqsubset R: square subset 
\makemo{x02290}{sqsup}    %/sqsupset R: square superset 
\makemo{x02291}{sqsube}    %/sqsubseteq R: square subset, equals 
\makemo{x02292}{sqsupe}    %/sqsupseteq R: square superset, eq 
\makemo{x022A2}{vdash}    %/vdash R: vertical, dash 
\makemo{x022A3}{dashv}    %/dashv R: dash, vertical 
\makemo{x022A7}{models}    %/models R: 
\makemo{x022A8}{vDash}    %/vDash R: vertical, dbl dash 
\makemo{x022A9}{Vdash}    %/Vdash R: dbl vertical, dash 
\makemo{x022AA}{Vvdash}    %/Vvdash R: triple vertical, dash 
\makemo{x022AB}{VDash}    %dbl vert, dbl dash 
\makemo{x022B0}{prurel}    %element precedes under relation 
\makemo{x022B2}{vltri}    %/vartriangleleft R: l tri, open, var 
\makemo{x022B3}{vrtri}    %/vartriangleright R: r tri, open, var 
\makemo{x022B4}{ltrie}    %/trianglelefteq R: left triangle, eq 
\makemo{x022B5}{rtrie}    %/trianglerighteq R: right tri, eq 
\makemo{x022C8}{bowtie}    %/bowtie R: 
\makemo{x022CD}{bsime}    %/backsimeq R: reverse similar, eq 
\makemo{x022D0}{Sub}    %/Subset R: double subset 
\makemo{x022D1}{Sup}    %/Supset R: dbl superset 
\makemo{x022D4}{fork}    %/pitchfork R: pitchfork 
\makemo{x022D6}{ltdot}    %/lessdot R: less than, with dot 
\makemo{x022D7}{gtdot}    %/gtrdot R: greater than, with dot 
\makemo{x022D8}{Ll}    %/Ll /lll /llless R: triple less-than 
\makemo{x022D9}{Gg}    %/ggg /Gg /gggtr R: triple gtr-than 
\makemo{x022DA}{leg}    %/lesseqgtr R: less, eq, greater 
\makemo{x022DB}{gel}    %/gtreqless R: greater, equals, less 
\makemo{x022DE}{cuepr}    %/curlyeqprec R: curly eq, precedes 
\makemo{x022DF}{cuesc}    %/curlyeqsucc R: curly eq, succeeds 
\makemo{x02322}{frown}    %/frown R: down curve 
\makemo{x02322}{sfrown}    %/smallfrown R: small down curve 
\makemo{x02323}{smile}    %/smile R: up curve 
\makemo{x02323}{ssmile}    %/smallsmile R: small up curve 
\makemo{x02976}{ltlarr}    %less than, left arrow 
\makemo{x02978}{gtrarr}    %greater than, right arrow 
\makemo{x02979}{subrarr}    %subset, right arrow 
\makemo{x0297B}{suplarr}    %superset, left arrow 
\makemo{x029CE}{rtriltri}    %right triangle above left triangle 
\makemo{x02A29}{mcomma}    %minus, comma above 
\makemo{x02A66}{sdote}    %equal, dot below 
\makemo{x02A6D}{congdot}    %congruent, dot 
\makemo{x02A6E}{easter}    %equal, asterisk above 
\makemo{x02A70}{apE}    %approximately equal or equal to 
\makemo{x02A73}{Esim}    %equal, similar 
\makemo{x02A74}{Colone}    %double colon, equals 
\makemo{x02A77}{eDDot}    %/ddotseq R: equal with four dots 
\makemo{x02A78}{equivDD}    %equivalent, four dots above 
\makemo{x02A79}{ltcir}    %less than, circle inside 
\makemo{x02A7A}{gtcir}    %greater than, circle inside 
\makemo{x02A7B}{ltquest}    %less than, questionmark above 
\makemo{x02A7C}{gtquest}    %greater than, questionmark above 
\makemo{x02A7D}{les}    %/leqslant R: less-than-or-eq, slant 
\makemo{x02A7E}{ges}    %/geqslant R: gt-or-equal, slanted 
\makemo{x02A7F}{lesdot}    %less-than-or-equal, slanted, dot inside 
\makemo{x02A80}{gesdot}    %greater-than-or-equal, slanted, dot inside 
\makemo{x02A81}{lesdoto}    %less-than-or-equal, slanted, dot above 
\makemo{x02A82}{gesdoto}    %greater-than-or-equal, slanted, dot above 
\makemo{x02A83}{lesdotor}    %less-than-or-equal, slanted, dot above right 
\makemo{x02A84}{gesdotol}    %greater-than-or-equal, slanted, dot above left 
\makemo{x02A85}{lap}    %/lessapprox R: less, approximate 
\makemo{x02A86}{gap}    %/gtrapprox R: greater, approximate 
\makemo{x02A8B}{lEg}    %/lesseqqgtr R: less, dbl eq, greater 
\makemo{x02A8C}{gEl}    %/gtreqqless R: gt, dbl equals, less 
\makemo{x02A8D}{lsime}    %less, similar, equal 
\makemo{x02A8E}{gsime}    %greater, similar, equal 
\makemo{x02A8F}{lsimg}    %less, similar, greater 
\makemo{x02A90}{gsiml}    %greater, similar, less 
\makemo{x02A91}{lgE}    %less, greater, equal 
\makemo{x02A92}{glE}    %greater, less, equal 
\makemo{x02A93}{lesges}    %less, equal, slanted, greater, equal, slanted 
\makemo{x02A94}{gesles}    %greater, equal, slanted, less, equal, slanted 
\makemo{x02A95}{els}    %/eqslantless R: eq-or-less, slanted 
\makemo{x02A96}{egs}    %/eqslantgtr R: equal-or-gtr, slanted 
\makemo{x02A97}{elsdot}    %equal-or-less, slanted, dot inside 
\makemo{x02A98}{egsdot}    %equal-or-greater, slanted, dot inside 
\makemo{x02A99}{el}    %equal-or-less 
\makemo{x02A9A}{eg}    %equal-or-greater 
\makemo{x02A9D}{siml}    %similar, less 
\makemo{x02A9E}{simg}    %similar, greater 
\makemo{x02A9F}{simlE}    %similar, less, equal 
\makemo{x02AA0}{simgE}    %similar, greater, equal 
\makemo{x02AA4}{glj}    %greater, less, overlapping 
\makemo{x02AA5}{gla}    %greater, less, apart 
\makemo{x02AA6}{ltcc}    %less than, closed by curve 
\makemo{x02AA7}{gtcc}    %greater than, closed by curve 
\makemo{x02AA8}{lescc}    %less than, closed by curve, equal, slanted 
\makemo{x02AA9}{gescc}    %greater than, closed by curve, equal, slanted 
\makemo{x02AAA}{smt}    %smaller than 
\makemo{x02AAB}{lat}    %larger than 
\makemo{x02AAC}{smte}    %smaller than or equal 
\makemo{x02AAD}{late}    %larger than or equal 
\makemo{x02AAE}{bumpE}    %bump, equals 
\makemo{x02AAF}{pre}    %/preceq R: precedes, equals 
\makemo{x02AB0}{sce}    %/succeq R: succeeds, equals 
\makemo{x02AB3}{prE}    %precedes, dbl equals 
\makemo{x02AB4}{scE}    %succeeds, dbl equals 
\makemo{x02AB7}{prap}    %/precapprox R: precedes, approximate 
\makemo{x02AB8}{scap}    %/succapprox R: succeeds, approximate 
\makemo{x02ABB}{Pr}    %dbl precedes 
\makemo{x02ABC}{Sc}    %dbl succeeds 
\makemo{x02ABF}{subplus}    %subset, plus 
\makemo{x02AC0}{supplus}    %superset, plus 
\makemo{x02AC1}{submult}    %subset, multiply 
\makemo{x02AC2}{supmult}    %superset, multiply 
\makemo{x02AC3}{subedot}    %subset, equals, dot 
\makemo{x02AC4}{supedot}    %superset, equals, dot 
\makemo{x02AC5}{subE}    %/subseteqq R: subset, dbl equals 
\makemo{x02AC6}{supE}    %/supseteqq R: superset, dbl equals 
\makemo{x02AC7}{subsim}    %subset, similar 
\makemo{x02AC8}{supsim}    %superset, similar 
\makemo{x02ACF}{csub}    %subset, closed 
\makemo{x02AD0}{csup}    %superset, closed 
\makemo{x02AD1}{csube}    %subset, closed, equals 
\makemo{x02AD2}{csupe}    %superset, closed, equals 
\makemo{x02AD3}{subsup}    %subset above superset 
\makemo{x02AD4}{supsub}    %superset above subset 
\makemo{x02AD5}{subsub}    %subset above subset 
\makemo{x02AD6}{supsup}    %superset above superset 
\makemo{x02AD7}{suphsub}    %superset, subset 
\makemo{x02AD8}{supdsub}    %superset, subset, dash joining them 
\makemo{x02AD9}{forkv}    %fork, variant 
\makemo{x02ADA}{topfork}    %fork with top 
\makemo{x02ADB}{mlcp}    %/mlcp 
\makemo{x02AE4}{Dashv}    %dbl dash, vertical 
\makemo{x02AE6}{Vdashl}    %vertical, dash (long) 
\makemo{x02AE7}{Barv}    %vert, dbl bar (over) 
\makemo{x02AE8}{vBar}    %vert, dbl bar (under) 
\makemo{x02AE9}{vBarv}    %dbl bar, vert over and under 
\makemo{x02AEB}{Vbar}    %dbl vert, bar (under) 

%% These are from iso9573-13/isogrk3.ent
\makemo{x00393}{Gamma}    %/Gamma capital Gamma, Greek 
\makemo{x00394}{Delta}    %/Delta capital Delta, Greek 
\makemo{x00398}{Theta}    %/Theta capital Theta, Greek 
\makemo{x0039B}{Lambda}    %/Lambda capital Lambda, Greek 
\makemo{x0039E}{Xi}    %/Xi capital Xi, Greek 
\makemo{x003A0}{Pi}    %/Pi capital Pi, Greek 
\makemo{x003A3}{Sigma}    %/Sigma capital Sigma, Greek 
\makemo{x003A6}{Phi}    %/Phi capital Phi, Greek 
\makemo{x003A8}{Psi}    %/Psi capital Psi, Greek 
\makemo{x003A9}{Omega}    %/Omega capital Omega, Greek 
\makemo{x003B1}{alpha}    %/alpha small alpha, Greek 
\makemo{x003B2}{beta}    %/beta small beta, Greek 
\makemo{x003B3}{gamma}    %/gamma small gamma, Greek 
\makemo{x003B4}{delta}    %/delta small delta, Greek 
\makemo{x003B5}{epsiv}    %/varepsilon 
\makemo{x003B6}{zeta}    %/zeta small zeta, Greek 
\makemo{x003B7}{eta}    %/eta small eta, Greek 
\makemo{x003B8}{theta}    %/theta straight theta, small theta, Greek 
\makemo{x003B9}{iota}    %/iota small iota, Greek 
\makemo{x003BA}{kappa}    %/kappa small kappa, Greek 
\makemo{x003BB}{lambda}    %/lambda small lambda, Greek 
\makemo{x003BC}{mu}    %/mu small mu, Greek 
\makemo{x003BD}{nu}    %/nu small nu, Greek 
\makemo{x003BE}{xi}    %/xi small xi, Greek 
\makemo{x003C0}{pi}    %/pi small pi, Greek 
\makemo{x003C1}{rho}    %/rho small rho, Greek 
\makemo{x003C2}{sigmav}    %/varsigma 
\makemo{x003C3}{sigma}    %/sigma small sigma, Greek 
\makemo{x003C4}{tau}    %/tau small tau, Greek 
\makemo{x003C5}{upsi}    %/upsilon small upsilon, Greek 
\makemo{x003C6}{phiv}    %/varphi - curly or open phi 
\makemo{x003C7}{chi}    %/chi small chi, Greek 
\makemo{x003C8}{psi}    %/psi small psi, Greek 
\makemo{x003C9}{omega}    %/omega small omega, Greek 
\makemo{x003D1}{thetav}    %/vartheta - curly or open theta 
\makemo{x003D2}{Upsi}    %/Upsilon capital Upsilon, Greek 
\makemo{x003D5}{phi}    %/straightphi - small phi, Greek 
\makemo{x003D6}{piv}    %/varpi 
\makemo{x003DC}{Gammad}    %capital digamma 
\makemo{x003DD}{gammad}    %/digamma 
\makemo{x003F0}{kappav}    %/varkappa 
\makemo{x003F1}{rhov}    %/varrho 
\makemo{x003F5}{epsi}    %/straightepsilon, small epsilon, Greek 

%% These are from iso9573-13/isomfrk.ent
\makemo{x0210C}{Hfr}    %/frak H, upper case h 
\makemo{x02111}{Ifr}    %/frak I, upper case i 
\makemo{x0211C}{Rfr}    %/frak R, upper case r 
\makemo{x02128}{Zfr}    %/frak Z, upper case z 
\makemo{x0212D}{Cfr}    %/frak C, upper case c 
\makemo{x1D504}{Afr}    %/frak A, upper case a 
\makemo{x1D505}{Bfr}    %/frak B, upper case b 
\makemo{x1D507}{Dfr}    %/frak D, upper case d 
\makemo{x1D508}{Efr}    %/frak E, upper case e 
\makemo{x1D509}{Ffr}    %/frak F, upper case f 
\makemo{x1D50A}{Gfr}    %/frak G, upper case g 
\makemo{x1D50D}{Jfr}    %/frak J, upper case j 
\makemo{x1D50E}{Kfr}    %/frak K, upper case k 
\makemo{x1D50F}{Lfr}    %/frak L, upper case l 
\makemo{x1D510}{Mfr}    %/frak M, upper case m 
\makemo{x1D511}{Nfr}    %/frak N, upper case n 
\makemo{x1D512}{Ofr}    %/frak O, upper case o 
\makemo{x1D513}{Pfr}    %/frak P, upper case p 
\makemo{x1D514}{Qfr}    %/frak Q, upper case q 
\makemo{x1D516}{Sfr}    %/frak S, upper case s 
\makemo{x1D517}{Tfr}    %/frak T, upper case t 
\makemo{x1D518}{Ufr}    %/frak U, upper case u 
\makemo{x1D519}{Vfr}    %/frak V, upper case v 
\makemo{x1D51A}{Wfr}    %/frak W, upper case w 
\makemo{x1D51B}{Xfr}    %/frak X, upper case x 
\makemo{x1D51C}{Yfr}    %/frak Y, upper case y 
\makemo{x1D51E}{afr}    %/frak a, lower case a 
\makemo{x1D51F}{bfr}    %/frak b, lower case b 
\makemo{x1D520}{cfr}    %/frak c, lower case c 
\makemo{x1D521}{dfr}    %/frak d, lower case d 
\makemo{x1D522}{efr}    %/frak e, lower case e 
\makemo{x1D523}{ffr}    %/frak f, lower case f 
\makemo{x1D524}{gfr}    %/frak g, lower case g 
\makemo{x1D525}{hfr}    %/frak h, lower case h 
\makemo{x1D526}{ifr}    %/frak i, lower case i 
\makemo{x1D527}{jfr}    %/frak j, lower case j 
\makemo{x1D528}{kfr}    %/frak k, lower case k 
\makemo{x1D529}{lfr}    %/frak l, lower case l 
\makemo{x1D52A}{mfr}    %/frak m, lower case m 
\makemo{x1D52B}{nfr}    %/frak n, lower case n 
\makemo{x1D52C}{ofr}    %/frak o, lower case o 
\makemo{x1D52D}{pfr}    %/frak p, lower case p 
\makemo{x1D52E}{qfr}    %/frak q, lower case q 
\makemo{x1D52F}{rfr}    %/frak r, lower case r 
\makemo{x1D530}{sfr}    %/frak s, lower case s 
\makemo{x1D531}{tfr}    %/frak t, lower case t 
\makemo{x1D532}{ufr}    %/frak u, lower case u 
\makemo{x1D533}{vfr}    %/frak v, lower case v 
\makemo{x1D534}{wfr}    %/frak w, lower case w 
\makemo{x1D535}{xfr}    %/frak x, lower case x 
\makemo{x1D536}{yfr}    %/frak y, lower case y 
\makemo{x1D537}{zfr}    %/frak z, lower case z 

%% These are from iso9573-13/isomopf.ent
\makemo{x02102}{Copf}    %/Bbb C, open face C 
\makemo{x0210D}{Hopf}    %/Bbb H, open face H 
\makemo{x02115}{Nopf}    %/Bbb N, open face N 
\makemo{x02119}{Popf}    %/Bbb P, open face P 
\makemo{x0211A}{Qopf}    %/Bbb Q, open face Q 
\makemo{x0211D}{Ropf}    %/Bbb R, open face R 
\makemo{x02124}{Zopf}    %/Bbb Z, open face Z 
\makemo{x1D538}{Aopf}    %/Bbb A, open face A 
\makemo{x1D539}{Bopf}    %/Bbb B, open face B 
\makemo{x1D53B}{Dopf}    %/Bbb D, open face D 
\makemo{x1D53C}{Eopf}    %/Bbb E, open face E 
\makemo{x1D53D}{Fopf}    %/Bbb F, open face F 
\makemo{x1D53E}{Gopf}    %/Bbb G, open face G 
\makemo{x1D540}{Iopf}    %/Bbb I, open face I 
\makemo{x1D541}{Jopf}    %/Bbb J, open face J 
\makemo{x1D542}{Kopf}    %/Bbb K, open face K 
\makemo{x1D543}{Lopf}    %/Bbb L, open face L 
\makemo{x1D544}{Mopf}    %/Bbb M, open face M 
\makemo{x1D546}{Oopf}    %/Bbb O, open face O 
\makemo{x1D54A}{Sopf}    %/Bbb S, open face S 
\makemo{x1D54B}{Topf}    %/Bbb T, open face T 
\makemo{x1D54C}{Uopf}    %/Bbb U, open face U 
\makemo{x1D54D}{Vopf}    %/Bbb V, open face V 
\makemo{x1D54E}{Wopf}    %/Bbb W, open face W 
\makemo{x1D54F}{Xopf}    %/Bbb X, open face X 
\makemo{x1D550}{Yopf}    %/Bbb Y, open face Y 

%% These are from iso9573-13/isomscr.ent
\makemo{x0210A}{gscr}    %/scr g, script letter g 
\makemo{x0210B}{Hscr}    %/scr H, script letter H 
\makemo{x02110}{Iscr}    %/scr I, script letter I 
\makemo{x02112}{Lscr}    %/scr L, script letter L 
\makemo{x0211B}{Rscr}    %/scr R, script letter R 
\makemo{x0212C}{Bscr}    %/scr B, script letter B 
\makemo{x0212F}{escr}    %/scr e, script letter e 
\makemo{x02130}{Escr}    %/scr E, script letter E 
\makemo{x02131}{Fscr}    %/scr F, script letter F 
\makemo{x02133}{Mscr}    %/scr M, script letter M 
\makemo{x02134}{oscr}    %/scr o, script letter o 
\makemo{x1D49C}{Ascr}    %/scr A, script letter A 
\makemo{x1D49E}{Cscr}    %/scr C, script letter C 
\makemo{x1D49F}{Dscr}    %/scr D, script letter D 
\makemo{x1D4A2}{Gscr}    %/scr G, script letter G 
\makemo{x1D4A5}{Jscr}    %/scr J, script letter J 
\makemo{x1D4A6}{Kscr}    %/scr K, script letter K 
\makemo{x1D4A9}{Nscr}    %/scr N, script letter N 
\makemo{x1D4AA}{Oscr}    %/scr O, script letter O 
\makemo{x1D4AB}{Pscr}    %/scr P, script letter P 
\makemo{x1D4AC}{Qscr}    %/scr Q, script letter Q 
\makemo{x1D4AE}{Sscr}    %/scr S, script letter S 
\makemo{x1D4AF}{Tscr}    %/scr T, script letter T 
\makemo{x1D4B0}{Uscr}    %/scr U, script letter U 
\makemo{x1D4B1}{Vscr}    %/scr V, script letter V 
\makemo{x1D4B2}{Wscr}    %/scr W, script letter W 
\makemo{x1D4B3}{Xscr}    %/scr X, script letter X 
\makemo{x1D4B4}{Yscr}    %/scr Y, script letter Y 
\makemo{x1D4B5}{Zscr}    %/scr Z, script letter Z 
\makemo{x1D4B6}{ascr}    %/scr a, script letter a 
\makemo{x1D4B7}{bscr}    %/scr b, script letter b 
\makemo{x1D4B8}{cscr}    %/scr c, script letter c 
\makemo{x1D4B9}{dscr}    %/scr d, script letter d 
\makemo{x1D4BB}{fscr}    %/scr f, script letter f 
\makemo{x1D4BD}{hscr}    %/scr h, script letter h 
\makemo{x1D4BE}{iscr}    %/scr i, script letter i 
\makemo{x1D4BF}{jscr}    %/scr j, script letter j 
\makemo{x1D4C0}{kscr}    %/scr k, script letter k 
\makemo{x1D4C1}{lscr}    %/scr l, script letter l 
\makemo{x1D4C2}{mscr}    %/scr m, script letter m 
\makemo{x1D4C3}{nscr}    %/scr n, script letter n 
\makemo{x1D4C5}{pscr}    %/scr p, script letter p 
\makemo{x1D4C6}{qscr}    %/scr q, script letter q 
\makemo{x1D4C7}{rscr}    %/scr r, script letter r 
\makemo{x1D4C8}{sscr}    %/scr s, script letter s 
\makemo{x1D4C9}{tscr}    %/scr t, script letter t 
\makemo{x1D4CA}{uscr}    %/scr u, script letter u 
\makemo{x1D4CB}{vscr}    %/scr v, script letter v 
\makemo{x1D4CC}{wscr}    %/scr w, script letter w 
\makemo{x1D4CD}{xscr}    %/scr x, script letter x 
\makemo{x1D4CE}{yscr}    %/scr y, script letter y 
\makemo{x1D4CF}{zscr}    %/scr z, script letter z 

%% These are from iso9573-13/isotech.ent
\makemo{x000A8}{Dot}    %dieresis or umlaut mark 
\makemo{x000AF}{strns}    %straightness 
\makemo{x00192}{fnof}    %function of (italic small f) 
\makemo{x001B5}{imped}    %impedance 
\makemo{x02016}{Verbar}    %/Vert dbl vertical bar 
\makemo{x02030}{permil}    %per thousand 
\makemo{x02031}{pertenk}    %per 10 thousand 
\makemo{x02032}{prime}    %/prime prime or minute 
\makemo{x02033}{Prime}    %double prime or second 
\makemo{x02034}{tprime}    %triple prime 
\makemo{x02057}{qprime}    %quadruple prime 
\makemo{x020DB}{tdot}    %three dots above 
\makemo{x020DC}{DotDot}    %four dots above 
\makemo{x0210B}{hamilt}    %Hamiltonian (script capital H) 
\makemo{x02112}{lagran}    %Lagrangian (script capital L) 
\makemo{x0212B}{angst}    %Angstrom capital A, ring 
\makemo{x0212C}{bernou}    %Bernoulli function (script capital B) 
\makemo{x02133}{phmmat}    %physics M-matrix (script capital M) 
\makemo{x02134}{order}    %order of (script small o) 
\makemo{x02135}{aleph}    %/aleph aleph, Hebrew 
\makemo{x021D0}{lArr}    %/Leftarrow A: is implied by 
\makemo{x021D2}{rArr}    %/Rightarrow A: implies 
\makemo{x021D4}{iff}    %/iff if and only if 
\makemo{x02200}{forall}    %/forall for all 
\makemo{x02202}{part}    %/partial partial differential 
\makemo{x02203}{exist}    %/exists at least one exists 
\makemo{x02207}{nabla}    %/nabla del, Hamilton operator 
\makemo{x02208}{isinv}    %set membership, variant 
\makemo{x02208}{isin}    %/in R: set membership 
\makemo{x02209}{notinva}    %negated set membership, variant 
\makemo{x02209}{notin}    %/notin N: negated set membership 
\makemo{x0220B}{niv}    %contains, variant 
\makemo{x0220B}{ni}    %/ni /owns R: contains 
\makemo{x0220C}{notniva}    %negated contains, variant 
\makemo{x0220C}{notni}    %negated contains 
\makemo{x02212}{minus}    %B: minus sign 
\makemo{x02213}{mnplus}    %/mp B: minus-or-plus sign 
\makemo{x02217}{lowast}    %low asterisk 
\makemo{x02218}{compfn}    %/circ B: composite function (small circle) 
\makemo{x0221A}{radic}    %/surd radical 
\makemo{x0221D}{prop}    %/propto R: is proportional to 
\makemo{x0221E}{infin}    %/infty infinity 
\makemo{x0221F}{angrt}    %right (90 degree) angle 
\makemo{x02222}{angsph}    %/sphericalangle angle-spherical 
\makemo{x02225}{par}    %/parallel R: parallel 
\makemo{x02227}{and}    %/wedge /land B: logical and 
\makemo{x02228}{or}    %/vee /lor B: logical or 
\makemo{x02229}{cap}    %/cap B: intersection 
\makemo{x0222A}{cup}    %/cup B: union or logical sum 
\makemo{x0222B}{int}    %/int L: integral operator 
\makemo{x0222C}{Int}    %double integral operator 
\makemo{x0222D}{tint}    %/iiint triple integral operator 
\makemo{x0222E}{conint}    %/oint L: contour integral operator 
\makemo{x0222F}{Conint}    %double contour integral operator 
\makemo{x02230}{Cconint}    %triple contour integral operator 
\makemo{x02231}{cwint}    %clockwise integral 
\makemo{x02232}{cwconint}    %contour integral, clockwise 
\makemo{x02233}{awconint}    %contour integral, anti-clockwise 
\makemo{x02234}{there4}    %/therefore R: therefore 
\makemo{x02235}{becaus}    %/because R: because 
\makemo{x0223C}{sim}    %/sim R: similar 
\makemo{x0223F}{acd}    %ac current 
\makemo{x02243}{sime}    %/simeq R: similar, equals 
\makemo{x02245}{cong}    %/cong R: congruent with 
\makemo{x02248}{ap}    %/approx R: approximate 
\makemo{x02259}{wedgeq}    %/wedgeq R: corresponds to (wedge, equals) 
\makemo{x0225A}{veeeq}    %logical or, equals 
\makemo{x02260}{ne}    %/ne /neq R: not equal 
\makemo{x02261}{equiv}    %/equiv R: identical with 
\makemo{x02264}{le}    %/leq /le R: less-than-or-equal 
\makemo{x02265}{ge}    %/geq /ge R: greater-than-or-equal 
\makemo{x02282}{sub}    %/subset R: subset or is implied by 
\makemo{x02283}{sup}    %/supset R: superset or implies 
\makemo{x02286}{sube}    %/subseteq R: subset, equals 
\makemo{x02287}{supe}    %/supseteq R: superset, equals 
\makemo{x022A4}{top}    %/top top 
\makemo{x022A5}{bottom}    %/bot bottom 
\makemo{x022A5}{perp}    %/perp R: perpendicular 
\makemo{x022D5}{epar}    %parallel, equal; equal or parallel 
\makemo{x022EF}{ctdot}    %/cdots, three dots, centered 
\makemo{x022F0}{utdot}    %three dots, ascending 
\makemo{x022F1}{dtdot}    %/ddots, three dots, descending 
\makemo{x022F2}{disin}    %set membership, long horizontal stroke 
\makemo{x022F3}{isinsv}    %large set membership, vertical bar on horizontal stroke 
\makemo{x022F4}{isins}    %set membership, vertical bar on horizontal stroke 
\makemo{x022F5}{isindot}    %set membership, dot above 
\makemo{x022F6}{notinvc}    %negated set membership, variant 
\makemo{x022F7}{notinvb}    %negated set membership, variant 
\makemo{x022F9}{isinE}    %set membership, two horizontal strokes 
\makemo{x022FA}{nisd}    %contains, long horizontal stroke 
\makemo{x022FB}{xnis}    %large contains, vertical bar on horizontal stroke 
\makemo{x022FC}{nis}    %contains, vertical bar on horizontal stroke 
\makemo{x022FD}{notnivc}    %contains, variant 
\makemo{x022FE}{notnivb}    %contains, variant 
\makemo{x02310}{bnot}    %reverse not 
\makemo{x02312}{profline}    %profile of a line 
\makemo{x02313}{profsurf}    %profile of a surface 
\makemo{x02329}{lang}    %/langle O: left angle bracket 
\makemo{x0232A}{rang}    %/rangle C: right angle bracket 
\makemo{x0232D}{cylcty}    %cylindricity 
\makemo{x0232E}{profalar}    %all-around profile 
\makemo{x02336}{topbot}    %top and bottom 
\makemo{x025A1}{square}    %/square, square 
\makemo{x025AA}{squarf}    %/blacksquare, square, filled 
\makemo{x025B1}{fltns}    %flatness 
\makemo{x02985}{lopar}    %left open parenthesis 
\makemo{x02986}{ropar}    %right open parenthesis 
\makemo{x0299C}{vangrt}    %right angle, variant 
\makemo{x029A6}{dwangle}    %large downward pointing angle 
\makemo{x029A7}{uwangle}    %large upward pointing angle 
\makemo{x029BB}{olcross}    %circle, cross 
\makemo{x029DC}{iinfin}    %infinity sign, incomplete 
\makemo{x029DD}{infintie}    %tie, infinity 
\makemo{x029DE}{nvinfin}    %not, vert, infinity 
\makemo{x029E3}{eparsl}    %parallel, slanted, equal; homothetically congruent to 
\makemo{x029E4}{smeparsl}    %similar, parallel, slanted, equal 
\makemo{x029E5}{eqvparsl}    %equivalent, equal; congruent and parallel 
\makemo{x029F6}{dsol}    %solidus, bar above 
\makemo{x02A0C}{qint}    %/iiiint quadruple integral operator 
\makemo{x02A0D}{fpartint}    %finite part integral 
\makemo{x02A10}{cirfnint}    %circulation function 
\makemo{x02A11}{awint}    %anti clock-wise integration 
\makemo{x02A12}{rppolint}    %line integration, rectangular path around pole 
\makemo{x02A13}{scpolint}    %line integration, semi-circular path around pole 
\makemo{x02A14}{npolint}    %line integration, not including the pole 
\makemo{x02A15}{pointint}    %integral around a point operator 
\makemo{x02A16}{quatint}    %quaternion integral operator 
\makemo{x02A17}{intlarhk}    %integral, left arrow with hook 
\makemo{x02A53}{And}    %dbl logical and 
\makemo{x02A54}{Or}    %dbl logical or 
\makemo{x02A55}{andand}    %two logical and 
\makemo{x02A56}{oror}    %two logical or 
\makemo{x02A57}{orslope}    %sloping large or 
\makemo{x02A58}{andslope}    %sloping large and 
\makemo{x02A5A}{andv}    %and with middle stem 
\makemo{x02A5B}{orv}    %or with middle stem 
\makemo{x02A5C}{andd}    %and, horizontal dash 
\makemo{x02A5D}{@ord}    %or, horizontal dash 
\makemo{x02A6A}{simdot}    %similar, dot 
\makemo{x02A6F}{apacir}    %approximate, circumflex accent 
\makemo{x02AEC}{Not}    %not with two horizontal strokes 
\makemo{x02AED}{bNot}    %reverse not with two horizontal strokes 
\makemo{x02AF1}{topcir}    %top, circle below 
\makemo{x02AF2}{nhpar}    %not, horizontal, parallel 
\makemo{x02AFD}{parsl}    %parallel, slanted 
\makemo{x0300A}{Lang}    %left angle bracket, double 
\makemo{x0300B}{Rang}    %right angle bracket, double 
\makemo{x03014}{lbbrk}    %left broken bracket 
\makemo{x03015}{rbbrk}    %right broken bracket 
\makemo{x03018}{loang}    %left open angular bracket 
\makemo{x03019}{roang}    %right open angular bracket 
\makemo{x0301A}{lobrk}    %left open bracket 
\makemo{x0301B}{robrk}    %right open bracket 
\makemo{x0FFFD}{elinters}    %electrical intersection 

%% These are from iso8879/isobox.ent
\makemo{x02500}{boxh}    %horizontal line 
\makemo{x02502}{boxv}    %vertical line 
\makemo{x0250C}{boxdr}    %lower right quadrant 
\makemo{x02510}{boxdl}    %lower left quadrant 
\makemo{x02514}{boxur}    %upper right quadrant 
\makemo{x02518}{boxul}    %upper left quadrant 
\makemo{x0251C}{boxvr}    %upper and lower right quadrants 
\makemo{x02524}{boxvl}    %upper and lower left quadrants 
\makemo{x0252C}{boxhd}    %lower left and right quadrants 
\makemo{x02534}{boxhu}    %upper left and right quadrants 
\makemo{x0253C}{boxvh}    %all four quadrants 
\makemo{x02550}{boxH}    %horizontal line 
\makemo{x02551}{boxV}    %vertical line 
\makemo{x02552}{boxdR}    %lower right quadrant 
\makemo{x02553}{boxDr}    %lower right quadrant 
\makemo{x02554}{boxDR}    %lower right quadrant 
\makemo{x02555}{boxdL}    %lower left quadrant 
\makemo{x02556}{boxDl}    %lower left quadrant 
\makemo{x02557}{boxDL}    %lower left quadrant 
\makemo{x02558}{boxuR}    %upper right quadrant 
\makemo{x02559}{boxUr}    %upper right quadrant 
\makemo{x0255A}{boxUR}    %upper right quadrant 
\makemo{x0255B}{boxuL}    %upper left quadrant 
\makemo{x0255C}{boxUl}    %upper left quadrant 
\makemo{x0255D}{boxUL}    %upper left quadrant 
\makemo{x0255E}{boxvR}    %upper and lower right quadrants 
\makemo{x0255F}{boxVr}    %upper and lower right quadrants 
\makemo{x02560}{boxVR}    %upper and lower right quadrants 
\makemo{x02561}{boxvL}    %upper and lower left quadrants 
\makemo{x02562}{boxVl}    %upper and lower left quadrants 
\makemo{x02563}{boxVL}    %upper and lower left quadrants 
\makemo{x02564}{boxHd}    %lower left and right quadrants 
\makemo{x02565}{boxhD}    %lower left and right quadrants 
\makemo{x02566}{boxHD}    %lower left and right quadrants 
\makemo{x02567}{boxHu}    %upper left and right quadrants 
\makemo{x02568}{boxhU}    %upper left and right quadrants 
\makemo{x02569}{boxHU}    %upper left and right quadrants 
\makemo{x0256A}{boxvH}    %all four quadrants 
\makemo{x0256B}{boxVh}    %all four quadrants 
\makemo{x0256C}{boxVH}    %all four quadrants 

%% These are from iso8879/isocyr1.ent
\makemo{x00401}{IOcy}    %=capital IO, Russian 
\makemo{x00410}{Acy}    %=capital A, Cyrillic 
\makemo{x00411}{Bcy}    %=capital BE, Cyrillic 
\makemo{x00412}{Vcy}    %=capital VE, Cyrillic 
\makemo{x00413}{Gcy}    %=capital GHE, Cyrillic 
\makemo{x00414}{Dcy}    %=capital DE, Cyrillic 
\makemo{x00415}{IEcy}    %=capital IE, Cyrillic 
\makemo{x00416}{ZHcy}    %=capital ZHE, Cyrillic 
\makemo{x00417}{Zcy}    %=capital ZE, Cyrillic 
\makemo{x00418}{Icy}    %=capital I, Cyrillic 
\makemo{x00419}{Jcy}    %=capital short I, Cyrillic 
\makemo{x0041A}{Kcy}    %=capital KA, Cyrillic 
\makemo{x0041B}{Lcy}    %=capital EL, Cyrillic 
\makemo{x0041C}{Mcy}    %=capital EM, Cyrillic 
\makemo{x0041D}{Ncy}    %=capital EN, Cyrillic 
\makemo{x0041E}{Ocy}    %=capital O, Cyrillic 
\makemo{x0041F}{Pcy}    %=capital PE, Cyrillic 
\makemo{x00420}{Rcy}    %=capital ER, Cyrillic 
\makemo{x00421}{Scy}    %=capital ES, Cyrillic 
\makemo{x00422}{Tcy}    %=capital TE, Cyrillic 
\makemo{x00423}{Ucy}    %=capital U, Cyrillic 
\makemo{x00424}{Fcy}    %=capital EF, Cyrillic 
\makemo{x00425}{KHcy}    %=capital HA, Cyrillic 
\makemo{x00426}{TScy}    %=capital TSE, Cyrillic 
\makemo{x00427}{CHcy}    %=capital CHE, Cyrillic 
\makemo{x00428}{SHcy}    %=capital SHA, Cyrillic 
\makemo{x00429}{SHCHcy}    %=capital SHCHA, Cyrillic 
\makemo{x0042A}{HARDcy}    %=capital HARD sign, Cyrillic 
\makemo{x0042B}{Ycy}    %=capital YERU, Cyrillic 
\makemo{x0042C}{SOFTcy}    %=capital SOFT sign, Cyrillic 
\makemo{x0042D}{Ecy}    %=capital E, Cyrillic 
\makemo{x0042E}{YUcy}    %=capital YU, Cyrillic 
\makemo{x0042F}{YAcy}    %=capital YA, Cyrillic 
\makemo{x00430}{acy}    %=small a, Cyrillic 
\makemo{x00431}{bcy}    %=small be, Cyrillic 
\makemo{x00432}{vcy}    %=small ve, Cyrillic 
\makemo{x00433}{gcy}    %=small ghe, Cyrillic 
\makemo{x00434}{dcy}    %=small de, Cyrillic 
\makemo{x00435}{iecy}    %=small ie, Cyrillic 
\makemo{x00436}{zhcy}    %=small zhe, Cyrillic 
\makemo{x00437}{zcy}    %=small ze, Cyrillic 
\makemo{x00438}{icy}    %=small i, Cyrillic 
\makemo{x00439}{jcy}    %=small short i, Cyrillic 
\makemo{x0043A}{kcy}    %=small ka, Cyrillic 
\makemo{x0043B}{lcy}    %=small el, Cyrillic 
\makemo{x0043C}{mcy}    %=small em, Cyrillic 
\makemo{x0043D}{ncy}    %=small en, Cyrillic 
\makemo{x0043E}{ocy}    %=small o, Cyrillic 
\makemo{x0043F}{pcy}    %=small pe, Cyrillic 
\makemo{x00440}{rcy}    %=small er, Cyrillic 
\makemo{x00441}{scy}    %=small es, Cyrillic 
\makemo{x00442}{tcy}    %=small te, Cyrillic 
\makemo{x00443}{ucy}    %=small u, Cyrillic 
\makemo{x00444}{fcy}    %=small ef, Cyrillic 
\makemo{x00445}{khcy}    %=small ha, Cyrillic 
\makemo{x00446}{tscy}    %=small tse, Cyrillic 
\makemo{x00447}{chcy}    %=small che, Cyrillic 
\makemo{x00448}{shcy}    %=small sha, Cyrillic 
\makemo{x00449}{shchcy}    %=small shcha, Cyrillic 
\makemo{x0044A}{hardcy}    %=small hard sign, Cyrillic 
\makemo{x0044B}{ycy}    %=small yeru, Cyrillic 
\makemo{x0044C}{softcy}    %=small soft sign, Cyrillic 
\makemo{x0044D}{ecy}    %=small e, Cyrillic 
\makemo{x0044E}{yucy}    %=small yu, Cyrillic 
\makemo{x0044F}{yacy}    %=small ya, Cyrillic 
\makemo{x00451}{iocy}    %=small io, Russian 
\makemo{x02116}{numero}    %=numero sign 

%% These are from iso8879/isocyr2.ent
\makemo{x00402}{DJcy}    %=capital DJE, Serbian 
\makemo{x00403}{GJcy}    %=capital GJE Macedonian 
\makemo{x00404}{Jukcy}    %=capital JE, Ukrainian 
\makemo{x00405}{DScy}    %=capital DSE, Macedonian 
\makemo{x00406}{Iukcy}    %=capital I, Ukrainian 
\makemo{x00407}{YIcy}    %=capital YI, Ukrainian 
\makemo{x00408}{Jsercy}    %=capital JE, Serbian 
\makemo{x00409}{LJcy}    %=capital LJE, Serbian 
\makemo{x0040A}{NJcy}    %=capital NJE, Serbian 
\makemo{x0040B}{TSHcy}    %=capital TSHE, Serbian 
\makemo{x0040C}{KJcy}    %=capital KJE, Macedonian 
\makemo{x0040E}{Ubrcy}    %=capital U, Byelorussian 
\makemo{x0040F}{DZcy}    %=capital dze, Serbian 
\makemo{x00452}{djcy}    %=small dje, Serbian 
\makemo{x00453}{gjcy}    %=small gje, Macedonian 
\makemo{x00454}{jukcy}    %=small je, Ukrainian 
\makemo{x00455}{dscy}    %=small dse, Macedonian 
\makemo{x00456}{iukcy}    %=small i, Ukrainian 
\makemo{x00457}{yicy}    %=small yi, Ukrainian 
\makemo{x00458}{jsercy}    %=small je, Serbian 
\makemo{x00459}{ljcy}    %=small lje, Serbian 
\makemo{x0045A}{njcy}    %=small nje, Serbian 
\makemo{x0045B}{tshcy}    %=small tshe, Serbian 
\makemo{x0045C}{kjcy}    %=small kje Macedonian 
\makemo{x0045E}{ubrcy}    %=small u, Byelorussian 
\makemo{x0045F}{dzcy}    %=small dze, Serbian 

%% These are from iso8879/isodia.ent
\makemo{x00060}{grave}    %=grave accent 
\makemo{x000A8}{die}    %=dieresis 
\makemo{x000A8}{uml}    %=umlaut mark 
\makemo{x000AF}{macr}    %=macron 
\makemo{x000B4}{acute}    %=acute accent 
\makemo{x000B8}{cedil}    %=cedilla 
\makemo{x002C6}{circ}    %circumflex accent 
\makemo{x002C7}{caron}    %=caron 
\makemo{x002D8}{breve}    %=breve 
\makemo{x002D9}{dot}    %=dot above 
\makemo{x002DA}{@ring}    %=ring 
\makemo{x002DB}{ogon}    %=ogonek 
\makemo{x002DC}{tilde}    %=tilde 
\makemo{x002DD}{dblac}    %=double acute accent 

%% These are from iso8879/isolat1.ent
\makemo{x000C0}{Agrave}    %=capital A, grave accent 
\makemo{x000C1}{Aacute}    %=capital A, acute accent 
\makemo{x000C2}{Acirc}    %=capital A, circumflex accent 
\makemo{x000C3}{Atilde}    %=capital A, tilde 
\makemo{x000C4}{Auml}    %=capital A, dieresis or umlaut mark 
\makemo{x000C5}{Aring}    %=capital A, ring 
\makemo{x000C6}{AElig}    %=capital AE diphthong (ligature) 
\makemo{x000C7}{Ccedil}    %=capital C, cedilla 
\makemo{x000C8}{Egrave}    %=capital E, grave accent 
\makemo{x000C9}{Eacute}    %=capital E, acute accent 
\makemo{x000CA}{Ecirc}    %=capital E, circumflex accent 
\makemo{x000CB}{Euml}    %=capital E, dieresis or umlaut mark 
\makemo{x000CC}{Igrave}    %=capital I, grave accent 
\makemo{x000CD}{Iacute}    %=capital I, acute accent 
\makemo{x000CE}{Icirc}    %=capital I, circumflex accent 
\makemo{x000CF}{Iuml}    %=capital I, dieresis or umlaut mark 
\makemo{x000D0}{ETH}    %=capital Eth, Icelandic 
\makemo{x000D1}{Ntilde}    %=capital N, tilde 
\makemo{x000D2}{Ograve}    %=capital O, grave accent 
\makemo{x000D3}{Oacute}    %=capital O, acute accent 
\makemo{x000D4}{Ocirc}    %=capital O, circumflex accent 
\makemo{x000D5}{Otilde}    %=capital O, tilde 
\makemo{x000D6}{Ouml}    %=capital O, dieresis or umlaut mark 
\makemo{x000D8}{Oslash}    %=capital O, slash 
\makemo{x000D9}{Ugrave}    %=capital U, grave accent 
\makemo{x000DA}{Uacute}    %=capital U, acute accent 
\makemo{x000DB}{Ucirc}    %=capital U, circumflex accent 
\makemo{x000DC}{Uuml}    %=capital U, dieresis or umlaut mark 
\makemo{x000DD}{Yacute}    %=capital Y, acute accent 
\makemo{x000DE}{THORN}    %=capital THORN, Icelandic 
\makemo{x000DF}{szlig}    %=small sharp s, German (sz ligature) 
\makemo{x000E0}{agrave}    %=small a, grave accent 
\makemo{x000E1}{aacute}    %=small a, acute accent 
\makemo{x000E2}{acirc}    %=small a, circumflex accent 
\makemo{x000E3}{atilde}    %=small a, tilde 
\makemo{x000E4}{auml}    %=small a, dieresis or umlaut mark 
\makemo{x000E5}{aring}    %=small a, ring 
\makemo{x000E6}{aelig}    %=small ae diphthong (ligature) 
\makemo{x000E7}{ccedil}    %=small c, cedilla 
\makemo{x000E8}{egrave}    %=small e, grave accent 
\makemo{x000E9}{eacute}    %=small e, acute accent 
\makemo{x000EA}{ecirc}    %=small e, circumflex accent 
\makemo{x000EB}{euml}    %=small e, dieresis or umlaut mark 
\makemo{x000EC}{igrave}    %=small i, grave accent 
\makemo{x000ED}{iacute}    %=small i, acute accent 
\makemo{x000EE}{icirc}    %=small i, circumflex accent 
\makemo{x000EF}{iuml}    %=small i, dieresis or umlaut mark 
\makemo{x000F0}{eth}    %=small eth, Icelandic 
\makemo{x000F1}{ntilde}    %=small n, tilde 
\makemo{x000F2}{ograve}    %=small o, grave accent 
\makemo{x000F3}{oacute}    %=small o, acute accent 
\makemo{x000F4}{ocirc}    %=small o, circumflex accent 
\makemo{x000F5}{otilde}    %=small o, tilde 
\makemo{x000F6}{ouml}    %=small o, dieresis or umlaut mark 
\makemo{x000F8}{oslash}    %latin small letter o with stroke 
\makemo{x000F9}{ugrave}    %=small u, grave accent 
\makemo{x000FA}{uacute}    %=small u, acute accent 
\makemo{x000FB}{ucirc}    %=small u, circumflex accent 
\makemo{x000FC}{uuml}    %=small u, dieresis or umlaut mark 
\makemo{x000FD}{yacute}    %=small y, acute accent 
\makemo{x000FE}{thorn}    %=small thorn, Icelandic 
\makemo{x000FF}{yuml}    %=small y, dieresis or umlaut mark 

%% These are from iso8879/isolat2.ent
\makemo{x00100}{Amacr}    %=capital A, macron 
\makemo{x00101}{amacr}    %=small a, macron 
\makemo{x00102}{Abreve}    %=capital A, breve 
\makemo{x00103}{abreve}    %=small a, breve 
\makemo{x00104}{Aogon}    %=capital A, ogonek 
\makemo{x00105}{aogon}    %=small a, ogonek 
\makemo{x00106}{Cacute}    %=capital C, acute accent 
\makemo{x00107}{cacute}    %=small c, acute accent 
\makemo{x00108}{Ccirc}    %=capital C, circumflex accent 
\makemo{x00109}{ccirc}    %=small c, circumflex accent 
\makemo{x0010A}{Cdot}    %=capital C, dot above 
\makemo{x0010B}{cdot}    %=small c, dot above 
\makemo{x0010C}{Ccaron}    %=capital C, caron 
\makemo{x0010D}{ccaron}    %=small c, caron 
\makemo{x0010E}{Dcaron}    %=capital D, caron 
\makemo{x0010F}{dcaron}    %=small d, caron 
\makemo{x00110}{Dstrok}    %=capital D, stroke 
\makemo{x00111}{dstrok}    %=small d, stroke 
\makemo{x00112}{Emacr}    %=capital E, macron 
\makemo{x00113}{emacr}    %=small e, macron 
\makemo{x00116}{Edot}    %=capital E, dot above 
\makemo{x00117}{edot}    %=small e, dot above 
\makemo{x00118}{Eogon}    %=capital E, ogonek 
\makemo{x00119}{eogon}    %=small e, ogonek 
\makemo{x0011A}{Ecaron}    %=capital E, caron 
\makemo{x0011B}{ecaron}    %=small e, caron 
\makemo{x0011C}{Gcirc}    %=capital G, circumflex accent 
\makemo{x0011D}{gcirc}    %=small g, circumflex accent 
\makemo{x0011E}{Gbreve}    %=capital G, breve 
\makemo{x0011F}{gbreve}    %=small g, breve 
\makemo{x00120}{Gdot}    %=capital G, dot above 
\makemo{x00121}{gdot}    %=small g, dot above 
\makemo{x00122}{Gcedil}    %=capital G, cedilla 
\makemo{x00124}{Hcirc}    %=capital H, circumflex accent 
\makemo{x00125}{hcirc}    %=small h, circumflex accent 
\makemo{x00126}{Hstrok}    %=capital H, stroke 
\makemo{x00127}{hstrok}    %=small h, stroke 
\makemo{x00128}{Itilde}    %=capital I, tilde 
\makemo{x00129}{itilde}    %=small i, tilde 
\makemo{x0012A}{Imacr}    %=capital I, macron 
\makemo{x0012B}{imacr}    %=small i, macron 
\makemo{x0012E}{Iogon}    %=capital I, ogonek 
\makemo{x0012F}{iogon}    %=small i, ogonek 
\makemo{x00130}{Idot}    %=capital I, dot above 
\makemo{x00131}{inodot}    %=small i without dot 
\makemo{x00132}{IJlig}    %=capital IJ ligature 
\makemo{x00133}{ijlig}    %=small ij ligature 
\makemo{x00134}{Jcirc}    %=capital J, circumflex accent 
\makemo{x00135}{jcirc}    %=small j, circumflex accent 
\makemo{x00136}{Kcedil}    %=capital K, cedilla 
\makemo{x00137}{kcedil}    %=small k, cedilla 
\makemo{x00138}{kgreen}    %=small k, Greenlandic 
\makemo{x00139}{Lacute}    %=capital L, acute accent 
\makemo{x0013A}{lacute}    %=small l, acute accent 
\makemo{x0013B}{Lcedil}    %=capital L, cedilla 
\makemo{x0013C}{lcedil}    %=small l, cedilla 
\makemo{x0013D}{Lcaron}    %=capital L, caron 
\makemo{x0013E}{lcaron}    %=small l, caron 
\makemo{x0013F}{Lmidot}    %=capital L, middle dot 
\makemo{x00140}{lmidot}    %=small l, middle dot 
\makemo{x00141}{Lstrok}    %=capital L, stroke 
\makemo{x00142}{lstrok}    %=small l, stroke 
\makemo{x00143}{Nacute}    %=capital N, acute accent 
\makemo{x00144}{nacute}    %=small n, acute accent 
\makemo{x00145}{Ncedil}    %=capital N, cedilla 
\makemo{x00146}{ncedil}    %=small n, cedilla 
\makemo{x00147}{Ncaron}    %=capital N, caron 
\makemo{x00148}{ncaron}    %=small n, caron 
\makemo{x00149}{napos}    %=small n, apostrophe 
\makemo{x0014A}{ENG}    %=capital ENG, Lapp 
\makemo{x0014B}{eng}    %=small eng, Lapp 
\makemo{x0014C}{Omacr}    %=capital O, macron 
\makemo{x0014D}{omacr}    %=small o, macron 
\makemo{x00150}{Odblac}    %=capital O, double acute accent 
\makemo{x00151}{odblac}    %=small o, double acute accent 
\makemo{x00152}{OElig}    %=capital OE ligature 
\makemo{x00153}{oelig}    %=small oe ligature 
\makemo{x00154}{Racute}    %=capital R, acute accent 
\makemo{x00155}{racute}    %=small r, acute accent 
\makemo{x00156}{Rcedil}    %=capital R, cedilla 
\makemo{x00157}{rcedil}    %=small r, cedilla 
\makemo{x00158}{Rcaron}    %=capital R, caron 
\makemo{x00159}{rcaron}    %=small r, caron 
\makemo{x0015A}{Sacute}    %=capital S, acute accent 
\makemo{x0015B}{sacute}    %=small s, acute accent 
\makemo{x0015C}{Scirc}    %=capital S, circumflex accent 
\makemo{x0015D}{scirc}    %=small s, circumflex accent 
\makemo{x0015E}{Scedil}    %=capital S, cedilla 
\makemo{x0015F}{scedil}    %=small s, cedilla 
\makemo{x00160}{Scaron}    %=capital S, caron 
\makemo{x00161}{scaron}    %=small s, caron 
\makemo{x00162}{Tcedil}    %=capital T, cedilla 
\makemo{x00163}{tcedil}    %=small t, cedilla 
\makemo{x00164}{Tcaron}    %=capital T, caron 
\makemo{x00165}{tcaron}    %=small t, caron 
\makemo{x00166}{Tstrok}    %=capital T, stroke 
\makemo{x00167}{tstrok}    %=small t, stroke 
\makemo{x00168}{Utilde}    %=capital U, tilde 
\makemo{x00169}{utilde}    %=small u, tilde 
\makemo{x0016A}{Umacr}    %=capital U, macron 
\makemo{x0016B}{umacr}    %=small u, macron 
\makemo{x0016C}{Ubreve}    %=capital U, breve 
\makemo{x0016D}{ubreve}    %=small u, breve 
\makemo{x0016E}{Uring}    %=capital U, ring 
\makemo{x0016F}{uring}    %=small u, ring 
\makemo{x00170}{Udblac}    %=capital U, double acute accent 
\makemo{x00171}{udblac}    %=small u, double acute accent 
\makemo{x00172}{Uogon}    %=capital U, ogonek 
\makemo{x00173}{uogon}    %=small u, ogonek 
\makemo{x00174}{Wcirc}    %=capital W, circumflex accent 
\makemo{x00175}{wcirc}    %=small w, circumflex accent 
\makemo{x00176}{Ycirc}    %=capital Y, circumflex accent 
\makemo{x00177}{ycirc}    %=small y, circumflex accent 
\makemo{x00178}{Yuml}    %=capital Y, dieresis or umlaut mark 
\makemo{x00179}{Zacute}    %=capital Z, acute accent 
\makemo{x0017A}{zacute}    %=small z, acute accent 
\makemo{x0017B}{Zdot}    %=capital Z, dot above 
\makemo{x0017C}{zdot}    %=small z, dot above 
\makemo{x0017D}{Zcaron}    %=capital Z, caron 
\makemo{x0017E}{zcaron}    %=small z, caron 
\makemo{x001F5}{gacute}    %=small g, acute accent 

%% These are from iso8879/isonum.ent
%\makemo{38;#38}{amp}    %=ampersand 
%\makemo{38;#60}{lt}    %=less-than sign R: 
\makemo{x00021}{excl}    %=exclamation mark 
\makemo{x00022}{quot}    %=quotation mark 
\makemo{x00023}{num}    %=number sign 
% mathdollar already exists
%\makemo{x00024}{dollar}    %=dollar sign 
\makemo{x00025}{percnt}    %=percent sign 
\makemo{x00027}{apos}    %=apostrophe 
\makemo{x00028}{lpar}    %O: =left parenthesis 
\makemo{x00029}{rpar}    %C: =right parenthesis 
\makemo{x0002A}{ast}    %/ast B: =asterisk 
\makemo{x0002B}{plus}    %=plus sign B: 
\makemo{x0002C}{comma}    %P: =comma 
\makemo{x0002E}{period}    %=full stop, period 
\makemo{x0002F}{sol}    %=solidus 
\makemo{x0003A}{colon}    %/colon P: 
\makemo{x0003B}{semi}    %=semicolon P: 
\makemo{x0003D}{equals}    %=equals sign R: 
\makemo{x0003E}{gt}    %=greater-than sign R: 
\makemo{x0003F}{quest}    %=question mark 
\makemo{x00040}{commat}    %=commercial at 
\makemo{x0005B}{lsqb}    %/lbrack O: =left square bracket 
\makemo{x0005C}{bsol}    %/backslash =reverse solidus 
\makemo{x0005D}{rsqb}    %/rbrack C: =right square bracket 
\makemo{x0005F}{lowbar}    %=low line 
\makemo{x0007B}{lcub}    %/lbrace O: =left curly bracket 
\makemo{x0007C}{verbar}    %/vert =vertical bar 
\makemo{x0007D}{rcub}    %/rbrace C: =right curly bracket 
\makemo{x000A0}{nbsp}    %=no break (required) space 
\makemo{x000A1}{iexcl}    %=inverted exclamation mark 
\makemo{x000A2}{cent}    %=cent sign 
\makemo{x000A3}{pound}    %=pound sign 
\makemo{x000A4}{curren}    %=general currency sign 
\makemo{x000A5}{yen}    %/yen =yen sign 
\makemo{x000A6}{brvbar}    %=broken (vertical) bar 
\makemo{x000A7}{sect}    %=section sign 
\makemo{x000A9}{copy}    %=copyright sign 
\makemo{x000AA}{ordf}    %=ordinal indicator, feminine 
\makemo{x000AB}{laquo}    %=angle quotation mark, left 
\makemo{x000AC}{not}    %/neg /lnot =not sign 
\makemo{x000AD}{shy}    %=soft hyphen 
\makemo{x000AE}{reg}    %/circledR =registered sign 
\makemo{x000B0}{deg}    %=degree sign 
\makemo{x000B1}{plusmn}    %/pm B: =plus-or-minus sign 
\makemo{x000B2}{sup2}    %=superscript two 
\makemo{x000B3}{sup3}    %=superscript three 
\makemo{x000B5}{micro}    %=micro sign 
\makemo{x000B6}{para}    %=pilcrow (paragraph sign) 
\makemo{x000B7}{middot}    %/centerdot B: =middle dot 
\makemo{x000B9}{sup1}    %=superscript one 
\makemo{x000BA}{ordm}    %=ordinal indicator, masculine 
\makemo{x000BB}{raquo}    %=angle quotation mark, right 
\makemo{x000BC}{frac14}    %=fraction one-quarter 
\makemo{x000BD}{frac12}    %=fraction one-half 
\makemo{x000BD}{half}    %=fraction one-half 
\makemo{x000BE}{frac34}    %=fraction three-quarters 
\makemo{x000BF}{iquest}    %=inverted question mark 
\makemo{x000D7}{times}    %/times B: =multiply sign 
\makemo{x000F7}{divide}    %/div B: =divide sign 
\makemo{x02010}{hyphen}    %=hyphen 
\makemo{x02015}{horbar}    %=horizontal bar 
\makemo{x02018}{lsquo}    %=single quotation mark, left 
\makemo{x02019}{rsquo}    %=single quotation mark, right 
\makemo{x0201C}{ldquo}    %=double quotation mark, left 
\makemo{x0201D}{rdquo}    %=double quotation mark, right 
\makemo{x02122}{trade}    %=trade mark sign 
\makemo{x02126}{ohm}    %=ohm sign 
\makemo{x0215B}{frac18}    %=fraction one-eighth 
\makemo{x0215C}{frac38}    %=fraction three-eighths 
\makemo{x0215D}{frac58}    %=fraction five-eighths 
\makemo{x0215E}{frac78}    %=fraction seven-eighths 
\makemo{x02190}{larr}    %/leftarrow /gets A: =leftward arrow 
\makemo{x02191}{uarr}    %/uparrow A: =upward arrow 
\makemo{x02192}{rarr}    %/rightarrow /to A: =rightward arrow 
\makemo{x02193}{darr}    %/downarrow A: =downward arrow 
\makemo{x0266A}{sung}    %=music note (sung text sign) 

%% These are from iso8879/isopub.ent
\makemo{x02002}{ensp}    %=en space (1/2-em) 
\makemo{x02003}{emsp}    %=em space 
\makemo{x02004}{emsp13}    %=1/3-em space 
\makemo{x02005}{emsp14}    %=1/4-em space 
\makemo{x02007}{numsp}    %=digit space (width of a number) 
\makemo{x02008}{puncsp}    %=punctuation space (width of comma) 
\makemo{x02009}{thinsp}    %=thin space (1/6-em) 
\makemo{x0200A}{hairsp}    %=hair space 
\makemo{x02010}{dash}    %=hyphen (true graphic) 
\makemo{x02013}{ndash}    %=en dash 
\makemo{x02014}{mdash}    %=em dash 
\makemo{x02019}{rsquor}    %rising single quote, right (high) 
\makemo{x0201A}{lsquor}    %=rising single quote, left (low) 
\makemo{x0201D}{rdquor}    %rising dbl quote, right (high) 
\makemo{x0201E}{ldquor}    %=rising dbl quote, left (low) 
\makemo{x02020}{@dagger}    %/dagger B: =dagger 
\makemo{x02021}{@Dagger}    %/ddagger B: =double dagger 
\makemo{x02022}{bull}    %/bullet B: =round bullet, filled 
\makemo{x02025}{nldr}    %=double baseline dot (en leader) 
\makemo{x02026}{hellip}    %=ellipsis (horizontal) 
\makemo{x02026}{mldr}    %em leader 
\makemo{x02041}{caret}    %=caret (insertion mark) 
\makemo{x02043}{hybull}    %rectangle, filled (hyphen bullet) 
\makemo{x02105}{incare}    %=in-care-of symbol 
\makemo{x02117}{copysr}    %=sound recording copyright sign 
\makemo{x0211E}{rx}    %pharmaceutical prescription (Rx) 
\makemo{x02153}{frac13}    %=fraction one-third 
\makemo{x02154}{frac23}    %=fraction two-thirds 
\makemo{x02155}{frac15}    %=fraction one-fifth 
\makemo{x02156}{frac25}    %=fraction two-fifths 
\makemo{x02157}{frac35}    %=fraction three-fifths 
\makemo{x02158}{frac45}    %=fraction four-fifths 
\makemo{x02159}{frac16}    %=fraction one-sixth 
\makemo{x0215A}{frac56}    %=fraction five-sixths 
\makemo{x022EE}{vellip}    %vertical ellipsis 
\makemo{x0230C}{drcrop}    %downward right crop mark 
\makemo{x0230D}{dlcrop}    %downward left crop mark 
\makemo{x0230E}{urcrop}    %upward right crop mark 
\makemo{x0230F}{ulcrop}    %upward left crop mark 
\makemo{x02315}{telrec}    %=telephone recorder symbol 
\makemo{x02316}{target}    %register mark or target 
\makemo{x02423}{blank}    %=significant blank symbol 
\makemo{x02580}{uhblk}    %=upper half block 
\makemo{x02584}{lhblk}    %=lower half block 
\makemo{x02588}{block}    %=full block 
\makemo{x02591}{blk14}    %=25% shaded block 
\makemo{x02592}{blk12}    %=50% shaded block 
\makemo{x02593}{blk34}    %=75% shaded block 
\makemo{x025A1}{squ}    %=square, open 
\makemo{x025AA}{squf}    %/blacksquare =sq bullet, filled 
\makemo{x025AD}{rect}    %=rectangle, open 
\makemo{x025AE}{marker}    %=histogram marker 
\makemo{x025B4}{utrif}    %/blacktriangle =up tri, filled 
\makemo{x025B5}{utri}    %/triangle =up triangle, open 
\makemo{x025B8}{rtrif}    %/blacktriangleright R: =r tri, filled 
\makemo{x025B9}{rtri}    %/triangleright B: r triangle, open 
\makemo{x025BE}{dtrif}    %/blacktriangledown =dn tri, filled 
\makemo{x025BF}{dtri}    %/triangledown =down triangle, open 
\makemo{x025C2}{ltrif}    %/blacktriangleleft R: =l tri, filled 
\makemo{x025C3}{ltri}    %/triangleleft B: l triangle, open 
\makemo{x025CA}{loz}    %/lozenge - lozenge or total mark 
\makemo{x025CB}{cir}    %/circ B: =circle, open 
\makemo{x02605}{starf}    %/bigstar - star, filled 
\makemo{x02606}{star}    %=star, open 
\makemo{x0260E}{phone}    %=telephone symbol 
\makemo{x02640}{female}    %=female symbol 
\makemo{x02642}{male}    %=male symbol 
\makemo{x02660}{spades}    %/spadesuit =spades suit symbol 
\makemo{x02663}{clubs}    %/clubsuit =club suit symbol 
\makemo{x02665}{hearts}    %/heartsuit =heart suit symbol 
\makemo{x02666}{diams}    %/diamondsuit =diamond suit symbol 
\makemo{x0266D}{flat}    %/flat =musical flat 
\makemo{x0266E}{natur}    %/natural - music natural 
\makemo{x0266F}{sharp}    %/sharp =musical sharp 
\makemo{x02713}{check}    %/checkmark =tick, check mark 
\makemo{x02717}{cross}    %=ballot cross 
\makemo{x02720}{malt}    %/maltese =maltese cross 
\makemo{x02736}{sext}    %sextile (6-pointed star) 
\makemo{x029EB}{lozf}    %/blacklozenge - lozenge, filled 
\makemo{x0FB00}{fflig}    %small ff ligature 
\makemo{x0FB01}{filig}    %small fi ligature 
\makemo{x0FB02}{fllig}    %small fl ligature 
\makemo{x0FB03}{ffilig}    %small ffi ligature 
\makemo{x0FB04}{ffllig}    %small ffl ligature 

%% These are from mathml/mmlextra
\makemo{x00009}{Tab}    %tabulator stop; horizontal tabulation 
\makemo{x0000A}{NewLine}    %force a line break; line feed 
\makemo{x0005E}{Hat}    %circumflex accent 
\makemo{x0007C}{VerticalLine}    %alias ISONUM verbar 
\makemo{x000AF}{OverBar}    %over bar 
\makemo{x00311}{DownBreve}    %breve, inverted (non-spacing) 
\makemo{x00332}{UnderBar}    %combining low line 
\makemo{x003A5}{Upsilon}    %ISOGRK1 Ugr, HTML4 Upsilon 
\makemo{x0200B}{ZeroWidthSpace}    %zero width space 
\makemo{x0205F}{MediumSpace}    %space of width 4/18 em 
\makemo{x02060}{NoBreak}    %never break line here 
\makemo{x02061}{af}    %character showing function application in presentation tagging 
\makemo{x02062}{@it}    %marks multiplication when it is understood without a mark 
\makemo{x02063}{ic}    %short form of &InvisibleComma; 
\makemo{x0210E}{planckh}    %the ring (skew field) of quaternions 
\makemo{x02145}{DD}    %D for use in differentials, e.g., within integrals 
\makemo{x02146}{dd}    %d for use in differentials, e.g., within integrals 
\makemo{x02147}{ee}    %e use for the exponential base of the natural logarithms 
\makemo{x02148}{ii}    %i for use as a square root of -1 
\makemo{x021A4}{mapstoleft}    %leftwards arrow from bar 
\makemo{x021A5}{mapstoup}    %upwards arrow from bar 
\makemo{x021A7}{mapstodown}    %downwards arrow from bar 
\makemo{x021E4}{larrb}    %leftwards arrow to bar 
\makemo{x021E5}{rarrb}    %leftwards arrow to bar 
\makemo{x0220F}{Product}    %alias for &prod; 
\makemo{x0224D}{asympeq}    %Old ISOAMSR asymp (for HTML compatibility) 
\makemo{x0226D}{NotCupCap}    %alias for &nasymp; 
\makemo{x023B4}{OverBracket}    %over bracket 
\makemo{x023B5}{UnderBracket}    %under bracket 
\makemo{x02500}{HorizontalLine}    %short horizontal line 
\makemo{x025AA}{FilledVerySmallSquare}    %filled very small square 
\makemo{x025AB}{EmptyVerySmallSquare}    %empty small square 
\makemo{x025FB}{EmptySmallSquare}    %empty small square 
\makemo{x025FC}{FilledSmallSquare}    %filled small square 
\makemo{x02758}{VerticalSeparator}    %vertical separating operator 
\makemo{x02912}{UpArrowBar}    %up arrow to bar 
\makemo{x02913}{DownArrowBar}    %down arrow to bar 
\makemo{x0294E}{LeftRightVector}    %left-up-right-up harpoon 
\makemo{x0294F}{RightUpDownVector}    %up-right-down-right harpoon 
\makemo{x02950}{DownLeftRightVector}    %left-down-right-down harpoon 
\makemo{x02951}{LeftUpDownVector}    %up-left-down-left harpoon 
\makemo{x02952}{LeftVectorBar}    %left-up harpoon to bar 
\makemo{x02953}{RightVectorBar}    %up-right harpoon to bar 
\makemo{x02954}{RightUpVectorBar}    %up-right harpoon to bar 
\makemo{x02955}{RightDownVectorBar}    %down-right harpoon to bar 
\makemo{x02956}{DownLeftVectorBar}    %left-down harpoon to bar 
\makemo{x02957}{DownRightVectorBar}    %right-down harpoon to bar 
\makemo{x02958}{LeftUpVectorBar}    %up-left harpoon to bar 
\makemo{x02959}{LeftDownVectorBar}    %down-left harpoon to bar 
\makemo{x0295A}{LeftTeeVector}    %left-up harpoon from bar 
\makemo{x0295B}{RightTeeVector}    %right-up harpoon from bar 
\makemo{x0295C}{RightUpTeeVector}    %up-right harpoon from bar 
\makemo{x0295D}{RightDownTeeVector}    %down-right harpoon from bar 
\makemo{x0295E}{DownLeftTeeVector}    %left-down harpoon from bar 
\makemo{x0295F}{DownRightTeeVector}    %right-down harpoon from bar 
\makemo{x02960}{LeftUpTeeVector}    %up-left harpoon from bar 
\makemo{x02961}{LeftDownTeeVector}    %down-left harpoon from bar 
\makemo{x02970}{RoundImplies}    %round implies 
\makemo{x029CF}{LeftTriangleBar}    %left triangle, vertical bar 
\makemo{x029D0}{RightTriangleBar}    %vertical bar, right triangle 
\makemo{x029F4}{RuleDelayed}    %rule-delayed (colon right arrow) 
\makemo{x02A2F}{Cross}    %cross or vector product 
\makemo{x02A75}{Equal}    %two consecutive equal signs 
\makemo{x02AA1}{LessLess}    %alias for Lt 
\makemo{x02AA2}{GreaterGreater}    %alias for GT 
\makemo{x0FE35}{OverParenthesis}    %over parenthesis 
\makemo{x0FE36}{UnderParenthesis}    %under parenthesis 
\makemo{x0FE37}{OverBrace}    %over brace 
\makemo{x0FE38}{UnderBrace}    %under brace 
\makemo{x1D552}{aopf}    % 
\makemo{x1D553}{bopf}    % 
\makemo{x1D554}{copf}    % 
\makemo{x1D555}{dopf}    % 
\makemo{x1D556}{eopf}    % 
\makemo{x1D557}{fopf}    % 
\makemo{x1D558}{gopf}    % 
\makemo{x1D559}{hopf}    % 
\makemo{x1D55A}{iopf}    % 
\makemo{x1D55B}{jopf}    % 
\makemo{x1D55C}{kopf}    % 
\makemo{x1D55D}{lopf}    % 
\makemo{x1D55E}{mopf}    % 
\makemo{x1D55F}{nopf}    % 
\makemo{x1D560}{oopf}    % 
\makemo{x1D561}{popf}    % 
\makemo{x1D562}{qopf}    % 
\makemo{x1D563}{ropf}    % 
\makemo{x1D564}{sopf}    % 
\makemo{x1D565}{topf}    % 
\makemo{x1D566}{uopf}    % 
\makemo{x1D567}{vopf}    % 
\makemo{x1D568}{wopf}    % 
\makemo{x1D569}{xopf}    % 
\makemo{x1D56A}{yopf}    % 
\makemo{x1D56B}{zopf}    % 

%% These are from mathml/mmlalias
\makemo{x0005B}{lbrack}    %alias ISONUM lsqb 
\makemo{x0005D}{rbrack}    %alias ISONUM rsqb 
\makemo{x00060}{DiacriticalGrave}    %alias ISODIA grave 
\makemo{x0007B}{lbrace}    %alias ISONUM lcub 
\makemo{x0007C}{vert}    %alias ISONUM verbar 
\makemo{x0007D}{rbrace}    %alias ISONUM rcub 
\makemo{x000A0}{NonBreakingSpace}    %alias ISONUM nbsp 
\makemo{x000A8}{DoubleDot}    %alias ISODIA die 
\makemo{x000AE}{circledR}    %alias ISONUM reg 
\makemo{x000B1}{PlusMinus}    %alias ISONUM plusmn 
\makemo{x000B1}{pm}    %alias ISONUM plusmn 
\makemo{x000B4}{DiacriticalAcute}    %alias ISODIA acute 
\makemo{x000B7}{CenterDot}    %alias ISONUM middot 
\makemo{x000B7}{centerdot}    %alias ISONUM middot 
\makemo{x000B8}{Cedilla}    %alias ISODIA cedil 
\makemo{x000F7}{div}    %alias ISONUM divide 
\makemo{x002C7}{Hacek}    %alias ISODIA caron 
\makemo{x002D8}{Breve}    %alias ISODIA breve 
\makemo{x002D9}{DiacriticalDot}    %alias ISODIA dot 
\makemo{x002DC}{DiacriticalTilde}    %alias ISODIA tilde 
\makemo{x002DD}{DiacriticalDoubleAcute}    %alias ISODIA dblac 
\makemo{x003B5}{varepsilon}    %alias ISOGRK3 epsiv 
\makemo{x003C2}{varsigma}    %alias ISOGRK3 sigmav 
\makemo{x003C5}{upsilon}    %alias ISOGRK3 upsi 
\makemo{x003C6}{varphi}    %alias ISOGRK3 phiv 
\makemo{x003D1}{vartheta}    %alias ISOGRK3 thetav 
\makemo{x003D5}{straightphi}    %alias ISOGRK3 phi 
\makemo{x003D6}{varpi}    %alias ISOGRK3 piv 
\makemo{x003DD}{digamma}    %alias ISOGRK3 gammad 
\makemo{x003F0}{varkappa}    %alias ISOGRK3 kappav 
\makemo{x003F1}{varrho}    %alias ISOGRK3 rhov 
\makemo{x003F5}{straightepsilon}    %alias ISOGRK3 epsi 
\makemo{x003F6}{backepsilon}    %alias ISOAMSR bepsi 
\makemo{x02009}{ThinSpace}    %space of width 3/18 em alias ISOPUB thinsp 
\makemo{x0200A}{VeryThinSpace}    %space of width 1/18 em alias ISOPUB hairsp 
\makemo{x0200B}{NegativeMediumSpace}    %space of width -4/18 em 
\makemo{x0200B}{NegativeThickSpace}    %space of width -5/18 em 
\makemo{x0200B}{NegativeThinSpace}    %space of width -3/18 em 
\makemo{x0200B}{NegativeVeryThinSpace}    %space of width -1/18 em 
\makemo{x02016}{Vert}    %alias ISOTECH Verbar 
\makemo{x02018}{OpenCurlyQuote}    %alias ISONUM lsquo 
\makemo{x02019}{CloseCurlyQuote}    %alias ISONUM rsquo 
\makemo{x0201C}{OpenCurlyDoubleQuote}    %alias ISONUM ldquo 
\makemo{x0201D}{CloseCurlyDoubleQuote}    %alias ISONUM rdquo 
\makemo{x02021}{ddagger}    %alias ISOPUB Dagger 
\makemo{x02022}{bullet}    %alias ISOPUB bull 
\makemo{x02035}{backprime}    %alias ISOAMSO bprime 
\makemo{x02061}{ApplyFunction}    %character showing function application in presentation tagging 
\makemo{x02062}{InvisibleTimes}    %marks multiplication when it is understood without a mark 
\makemo{x02063}{InvisibleComma}    %used as a separator, e.g., in indices 
\makemo{x020DB}{TripleDot}    %alias ISOTECH tdot 
\makemo{x02102}{complexes}    %the field of complex numbers 
\makemo{x0210B}{HilbertSpace}    %Hilbert space 
\makemo{x0210C}{Poincareplane}    %the Poincare upper half-plane 
\makemo{x0210D}{quaternions}    %the ring (skew field) of quaternions 
\makemo{x0210F}{hbar}    %alias ISOAMSO plank 
\makemo{x0210F}{hslash}    %alias ISOAMSO plankv 
\makemo{x02110}{imagline}    %the geometric imaginary line 
\makemo{x02111}{Im}    %alias ISOAMSO image 
\makemo{x02111}{imagpart}    %alias ISOAMSO image 
\makemo{x02112}{Laplacetrf}    %Laplace transform 
\makemo{x02115}{naturals}    %the semi-ring of natural numbers 
\makemo{x02118}{wp}    %alias ISOAMSO weierp 
\makemo{x02119}{primes}    %the prime natural numbers 
\makemo{x0211A}{rationals}    %the field of rational numbers 
\makemo{x0211B}{realine}    %the geometric real line 
\makemo{x0211C}{Re}    %alias ISOAMSO real 
\makemo{x0211C}{realpart}    %alias ISOAMSO real 
\makemo{x0211D}{reals}    %the field of real numbers 
\makemo{x02124}{integers}    %the ring of integers 
\makemo{x02128}{zeetrf}    %zee transform 
\makemo{x0212C}{Bernoullis}    %alias ISOTECH bernou 
\makemo{x0212D}{Cayleys}    %the non-associative ring of octonions or Cayley numbers 
\makemo{x02130}{expectation}    %expectation (operator) 
\makemo{x02131}{Fouriertrf}    %Fourier transform 
\makemo{x02133}{Mellintrf}    %Mellin transform 
\makemo{x02134}{orderof}    %alias ISOTECH order 
\makemo{x02145}{CapitalDifferentialD}    %D for use in differentials, e.g., within integrals 
\makemo{x02146}{DifferentialD}    %d for use in differentials, e.g., within integrals 
\makemo{x02147}{ExponentialE}    %e use for the exponential base of the natural logarithms 
\makemo{x02147}{exponentiale}    %base of the Napierian logarithms 
\makemo{x02148}{ImaginaryI}    %i for use as a square root of -1 
\makemo{x02190}{LeftArrow}    %alias ISONUM larr 
\makemo{x02190}{ShortLeftArrow}    %alias ISOAMSA slarr 
\makemo{x02190}{leftarrow}    %alias ISONUM larr 
\makemo{x02191}{ShortUpArrow}    %short up arrow 
\makemo{x02191}{UpArrow}    %alias ISONUM uarr 
\makemo{x02191}{uparrow}    %alias ISONUM uarr 
\makemo{x02192}{RightArrow}    %alias ISONUM rarr 
\makemo{x02192}{ShortRightArrow}    %alias ISOAMSA srarr 
\makemo{x02192}{rightarrow}    %alias ISONUM rarr 
\makemo{x02193}{DownArrow}    %alias ISONUM darr 
\makemo{x02193}{ShortDownArrow}    %short down arrow 
\makemo{x02193}{downarrow}    %alias ISONUM darr 
\makemo{x02194}{LeftRightArrow}    %alias ISOAMSA harr 
\makemo{x02194}{leftrightarrow}    %alias ISOAMSA harr 
\makemo{x02195}{UpDownArrow}    %alias ISOAMSA varr 
\makemo{x02195}{updownarrow}    %alias ISOAMSA varr 
\makemo{x02196}{UpperLeftArrow}    %alias ISOAMSA nwarr 
\makemo{x02196}{nwarrow}    %alias ISOAMSA nwarr 
\makemo{x02197}{UpperRightArrow}    %alias ISOAMSA nearr 
\makemo{x02197}{nearrow}    %alias ISOAMSA nearr 
\makemo{x02198}{LowerRightArrow}    %alias ISOAMSA searr 
\makemo{x02198}{searrow}    %alias ISOAMSA searr 
\makemo{x02199}{LowerLeftArrow}    %alias ISOAMSA swarr 
\makemo{x02199}{swarrow}    %alias ISOAMSA swarr 
\makemo{x0219A}{nleftarrow}    %alias ISOAMSA nlarr 
\makemo{x0219B}{nrightarrow}    %alias ISOAMSA nrarr 
\makemo{x0219D}{rightsquigarrow}    %alias ISOAMSA rarrw 
\makemo{x0219E}{twoheadleftarrow}    %alias ISOAMSA Larr 
\makemo{x021A0}{twoheadrightarrow}    %alias ISOAMSA Rarr 
\makemo{x021A2}{leftarrowtail}    %alias ISOAMSA larrtl 
\makemo{x021A3}{rightarrowtail}    %alias ISOAMSA rarrtl 
\makemo{x021A4}{LeftTeeArrow}    %alias for mapstoleft 
\makemo{x021A5}{UpTeeArrow}    %Alias mapstoup 
\makemo{x021A6}{RightTeeArrow}    %alias ISOAMSA map 
\makemo{x021A6}{mapsto}    %alias ISOAMSA map 
\makemo{x021A7}{DownTeeArrow}    %alias for mapstodown 
\makemo{x021A9}{hookleftarrow}    %alias ISOAMSA larrhk 
\makemo{x021AA}{hookrightarrow}    %alias ISOAMSA rarrhk 
\makemo{x021AB}{looparrowleft}    %alias ISOAMSA larrlp 
\makemo{x021AC}{looparrowright}    %alias ISOAMSA rarrlp 
\makemo{x021AD}{leftrightsquigarrow}    %alias ISOAMSA harrw 
\makemo{x021AE}{nleftrightarrow}    %alias ISOAMSA nharr 
\makemo{x021B0}{Lsh}    %alias ISOAMSA lsh 
\makemo{x021B1}{Rsh}    %alias ISOAMSA rsh 
\makemo{x021B6}{curvearrowleft}    %alias ISOAMSA cularr 
\makemo{x021B7}{curvearrowright}    %alias ISOAMSA curarr 
\makemo{x021BA}{circlearrowleft}    %alias ISOAMSA olarr 
\makemo{x021BB}{circlearrowright}    %alias ISOAMSA orarr 
\makemo{x021BC}{LeftVector}    %alias ISOAMSA lharu 
\makemo{x021BC}{leftharpoonup}    %alias ISOAMSA lharu 
\makemo{x021BD}{DownLeftVector}    %alias ISOAMSA lhard 
\makemo{x021BD}{leftharpoondown}    %alias ISOAMSA lhard 
\makemo{x021BE}{RightUpVector}    %alias ISOAMSA uharr 
\makemo{x021BE}{upharpoonright}    %alias ISOAMSA uharr 
\makemo{x021BF}{LeftUpVector}    %alias ISOAMSA uharl 
\makemo{x021BF}{upharpoonleft}    %alias ISOAMSA uharl 
\makemo{x021C0}{RightVector}    %alias ISOAMSA rharu 
\makemo{x021C0}{rightharpoonup}    %alias ISOAMSA rharu 
\makemo{x021C1}{DownRightVector}    %alias ISOAMSA rhard 
\makemo{x021C1}{rightharpoondown}    %alias ISOAMSA rhard 
\makemo{x021C2}{RightDownVector}    %alias ISOAMSA dharr 
\makemo{x021C2}{downharpoonright}    %alias ISOAMSA dharr 
\makemo{x021C3}{LeftDownVector}    %alias ISOAMSA dharl 
\makemo{x021C3}{downharpoonleft}    %alias ISOAMSA dharl 
\makemo{x021C4}{RightArrowLeftArrow}    %alias ISOAMSA rlarr 
\makemo{x021C4}{rightleftarrows}    %alias ISOAMSA rlarr 
\makemo{x021C5}{UpArrowDownArrow}    %alias ISOAMSA udarr 
\makemo{x021C6}{LeftArrowRightArrow}    %alias ISOAMSA lrarr 
\makemo{x021C6}{leftrightarrows}    %alias ISOAMSA lrarr 
\makemo{x021C7}{leftleftarrows}    %alias ISOAMSA llarr 
\makemo{x021C8}{upuparrows}    %alias ISOAMSA uuarr 
\makemo{x021C9}{rightrightarrows}    %alias ISOAMSA rrarr 
\makemo{x021CA}{downdownarrows}    %alias ISOAMSA ddarr 
\makemo{x021CB}{ReverseEquilibrium}    %alias ISOAMSA lrhar 
\makemo{x021CB}{leftrightharpoons}    %alias ISOAMSA lrhar 
\makemo{x021CC}{Equilibrium}    %alias ISOAMSA rlhar 
\makemo{x021CC}{rightleftharpoons}    %alias ISOAMSA rlhar 
\makemo{x021CD}{nLeftarrow}    %alias ISOAMSA nlArr 
\makemo{x021CE}{nLeftrightarrow}    %alias ISOAMSA nhArr 
\makemo{x021CF}{nRightarrow}    %alias ISOAMSA nrArr 
\makemo{x021D0}{DoubleLeftArrow}    %alias ISOTECH lArr 
\makemo{x021D0}{Leftarrow}    %alias ISOTECH lArr 
\makemo{x021D1}{DoubleUpArrow}    %alias ISOAMSA uArr 
\makemo{x021D1}{Uparrow}    %alias ISOAMSA uArr 
\makemo{x021D2}{DoubleRightArrow}    %alias ISOTECH rArr 
\makemo{x021D2}{Implies}    %alias ISOTECH rArr 
\makemo{x021D2}{Rightarrow}    %alias ISOTECH rArr 
\makemo{x021D3}{DoubleDownArrow}    %alias ISOAMSA dArr 
\makemo{x021D3}{Downarrow}    %alias ISOAMSA dArr 
\makemo{x021D4}{DoubleLeftRightArrow}    %alias ISOAMSA hArr 
\makemo{x021D4}{Leftrightarrow}    %alias ISOAMSA hArr 
\makemo{x021D5}{DoubleUpDownArrow}    %alias ISOAMSA vArr 
\makemo{x021D5}{Updownarrow}    %alias ISOAMSA vArr 
\makemo{x021DA}{Lleftarrow}    %alias ISOAMSA lAarr 
\makemo{x021DB}{Rrightarrow}    %alias ISOAMSA rAarr 
\makemo{x021E4}{LeftArrowBar}    %alias for larrb 
\makemo{x021E5}{RightArrowBar}    %alias for rarrb 
\makemo{x021F5}{DownArrowUpArrow}    %alias ISOAMSA duarr 
\makemo{x02200}{ForAll}    %alias ISOTECH forall 
\makemo{x02201}{complement}    %alias ISOAMSO comp 
\makemo{x02202}{PartialD}    %alias ISOTECH part 
\makemo{x02203}{Exists}    %alias ISOTECH exist 
\makemo{x02204}{NotExists}    %alias ISOAMSO nexist 
\makemo{x02204}{nexists}    %alias ISOAMSO nexist 
\makemo{x02205}{emptyset}    %alias ISOAMSO empty 
\makemo{x02205}{varnothing}    %alias ISOAMSO emptyv 
\makemo{x02207}{Del}    %alias ISOTECH nabla 
\makemo{x02208}{Element}    %alias ISOTECH isinv 
\makemo{x02208}{in}    %ISOTECH isin 
\makemo{x02209}{NotElement}    %alias ISOTECH notin 
\makemo{x0220B}{ReverseElement}    %alias ISOTECH niv 
\makemo{x0220B}{SuchThat}    %ISOTECH ni 
\makemo{x0220C}{NotReverseElement}    %alias ISOTECH notniva 
\makemo{x02210}{Coproduct}    %alias ISOAMSB coprod 
\makemo{x02211}{Sum}    %alias ISOAMSB sum 
\makemo{x02213}{MinusPlus}    %alias ISOTECH mnplus 
\makemo{x02213}{mp}    %alias ISOTECH mnplus 
\makemo{x02214}{dotplus}    %alias ISOAMSB plusdo 
\makemo{x02216}{Backslash}    %alias ISOAMSB setmn 
\makemo{x02216}{setminus}    %alias ISOAMSB setmn 
\makemo{x02216}{smallsetminus}    %alias ISOAMSB ssetmn 
\makemo{x02218}{SmallCircle}    %alias ISOTECH compfn 
\makemo{x0221A}{Sqrt}    %alias ISOTECH radic 
\makemo{x0221D}{Proportional}    %alias ISOTECH prop 
\makemo{x0221D}{propto}    %alias ISOTECH prop 
\makemo{x0221D}{varpropto}    %alias ISOAMSR vprop 
\makemo{x02220}{angle}    %alias ISOAMSO ang 
\makemo{x02221}{measuredangle}    %alias ISOAMSO angmsd 
\makemo{x02223}{VerticalBar}    %alias ISOAMSR mid 
\makemo{x02223}{shortmid}    %alias ISOAMSR smid 
\makemo{x02224}{NotVerticalBar}    %alias ISOAMSN nmid 
\makemo{x02224}{nshortmid}    %alias ISOAMSN nsmid 
\makemo{x02225}{DoubleVerticalBar}    %alias ISOTECH par 
\makemo{x02225}{parallel}    %alias ISOTECH par 
\makemo{x02225}{shortparallel}    %alias ISOAMSR spar 
\makemo{x02226}{NotDoubleVerticalBar}    %alias ISOAMSN npar 
\makemo{x02226}{nparallel}    %alias ISOAMSN npar 
\makemo{x02226}{nshortparallel}    %alias ISOAMSN nspar 
\makemo{x02227}{wedge}    %alias ISOTECH and 
\makemo{x02228}{vee}    %alias ISOTECH or 
\makemo{x0222B}{Integral}    %alias ISOTECH int 
\makemo{x0222D}{iiint}    %alias ISOTECH tint 
\makemo{x0222E}{ContourIntegral}    %alias ISOTECH conint 
\makemo{x0222E}{oint}    %alias ISOTECH conint 
\makemo{x0222F}{DoubleContourIntegral}    %alias ISOTECH Conint 
\makemo{x02232}{ClockwiseContourIntegral}    %alias ISOTECH cwconint 
\makemo{x02233}{CounterClockwiseContourIntegral}    %alias ISOTECH awconint 
\makemo{x02234}{Therefore}    %alias ISOTECH there4 
\makemo{x02234}{therefore}    %alias ISOTECH there4 
\makemo{x02235}{Because}    %alias ISOTECH becaus 
\makemo{x02235}{because}    %alias ISOTECH becaus 
\makemo{x02237}{Proportion}    %alias ISOAMSR Colon 
\makemo{x02238}{dotminus}    %alias ISOAMSB minusd 
\makemo{x0223C}{Tilde}    %alias ISOTECH sim 
\makemo{x0223C}{thicksim}    %ISOAMSR thksim 
\makemo{x0223D}{backsim}    %alias ISOAMSR bsim 
\makemo{x02240}{VerticalTilde}    %alias ISOAMSB wreath 
\makemo{x02240}{wr}    %alias ISOAMSB wreath 
\makemo{x02241}{NotTilde}    %alias ISOAMSN nsim 
\makemo{x02242}{EqualTilde}    %alias ISOAMSR esim 
\makemo{x02242}{eqsim}    %alias ISOAMSR esim 
\makemo{x02243}{TildeEqual}    %alias ISOTECH sime 
\makemo{x02243}{simeq}    %alias ISOTECH sime 
\makemo{x02244}{NotTildeEqual}    %alias ISOAMSN nsime 
\makemo{x02244}{nsimeq}    %alias ISOAMSN nsime 
\makemo{x02245}{TildeFullEqual}    %alias ISOTECH cong 
\makemo{x02247}{NotTildeFullEqual}    %alias ISOAMSN ncong 
\makemo{x02248}{TildeTilde}    %alias ISOTECH ap 
\makemo{x02248}{approx}    %alias ISOTECH ap 
\makemo{x02248}{thickapprox}    %ISOAMSR thkap 
\makemo{x02249}{NotTildeTilde}    %alias ISOAMSN nap 
\makemo{x02249}{napprox}    %alias ISOAMSN nap 
\makemo{x0224A}{approxeq}    %alias ISOAMSR ape 
\makemo{x0224C}{backcong}    %alias ISOAMSR bcong 
\makemo{x0224D}{CupCap}    %alias asympeq 
\makemo{x0224E}{Bumpeq}    %alias ISOAMSR bump 
\makemo{x0224E}{HumpDownHump}    %alias ISOAMSR bump 
\makemo{x0224F}{HumpEqual}    %alias ISOAMSR bumpe 
\makemo{x0224F}{bumpeq}    %alias ISOAMSR bumpe 
\makemo{x02250}{DotEqual}    %alias ISOAMSR esdot 
\makemo{x02250}{doteq}    %alias ISOAMSR esdot 
\makemo{x02251}{doteqdot}    %alias ISOAMSR eDot 
\makemo{x02252}{fallingdotseq}    %alias ISOAMSR efDot 
\makemo{x02253}{risingdotseq}    %alias ISOAMSR erDot 
\makemo{x02254}{Assign}    %assignment operator, alias ISOAMSR colone 
\makemo{x02254}{coloneq}    %alias ISOAMSR colone 
\makemo{x02255}{eqcolon}    %alias ISOAMSR ecolon 
\makemo{x02256}{eqcirc}    %alias ISOAMSR ecir 
\makemo{x02257}{circeq}    %alias ISOAMSR cire 
\makemo{x0225C}{triangleq}    %alias ISOAMSR trie 
\makemo{x0225F}{questeq}    %alias ISOAMSR equest 
\makemo{x02260}{NotEqual}    %alias ISOTECH ne 
\makemo{x02261}{Congruent}    %alias ISOTECH equiv 
\makemo{x02262}{NotCongruent}    %alias ISOAMSN nequiv 
\makemo{x02264}{leq}    %alias ISOTECH le 
\makemo{x02265}{GreaterEqual}    %alias ISOTECH ge 
\makemo{x02265}{geq}    %alias ISOTECH ge 
\makemo{x02266}{LessFullEqual}    %alias ISOAMSR lE 
\makemo{x02266}{leqq}    %alias ISOAMSR lE 
\makemo{x02267}{GreaterFullEqual}    %alias ISOAMSR gE 
\makemo{x02267}{geqq}    %alias ISOAMSR gE 
\makemo{x02268}{lneqq}    %alias ISOAMSN lnE 
\makemo{x02269}{gneqq}    %alias ISOAMSN gnE 
\makemo{x0226A}{NestedLessLess}    %alias ISOAMSR Lt 
\makemo{x0226A}{ll}    %alias ISOAMSR Lt 
\makemo{x0226B}{NestedGreaterGreater}    %alias ISOAMSR Gt 
\makemo{x0226B}{gg}    %alias ISOAMSR Gt 
\makemo{x0226C}{@between}    %alias ISOAMSR twixt 
\makemo{x0226E}{NotLess}    %alias ISOAMSN nlt 
\makemo{x0226E}{nless}    %alias ISOAMSN nlt 
\makemo{x0226F}{NotGreater}    %alias ISOAMSN ngt 
\makemo{x0226F}{ngtr}    %alias ISOAMSN ngt 
\makemo{x02270}{NotLessEqual}    %alias ISOAMSN nle 
\makemo{x02270}{nleq}    %alias ISOAMSN nle 
\makemo{x02271}{NotGreaterEqual}    %alias ISOAMSN nge 
\makemo{x02271}{ngeq}    %alias ISOAMSN nge 
\makemo{x02272}{LessTilde}    %alias ISOAMSR lsim 
\makemo{x02272}{lesssim}    %alias ISOAMSR lsim 
\makemo{x02273}{GreaterTilde}    %alias ISOAMSR gsim 
\makemo{x02273}{gtrsim}    %alias ISOAMSR gsim 
\makemo{x02274}{NotLessTilde}    %alias ISOAMSN nlsim 
\makemo{x02275}{NotGreaterTilde}    %alias ISOAMSN ngsim 
\makemo{x02276}{LessGreater}    %alias ISOAMSR lg 
\makemo{x02276}{lessgtr}    %alias ISOAMSR lg 
\makemo{x02277}{GreaterLess}    %alias ISOAMSR gl 
\makemo{x02277}{gtrless}    %alias ISOAMSR gl 
\makemo{x02278}{NotLessGreater}    %alias ISOAMSN ntvlg 
\makemo{x02279}{NotGreaterLess}    %alias ISOAMSN ntvgl 
\makemo{x0227A}{Precedes}    %alias ISOAMSR pr 
\makemo{x0227A}{prec}    %alias ISOAMSR pr 
\makemo{x0227B}{Succeeds}    %alias ISOAMSR sc 
\makemo{x0227B}{succ}    %alias ISOAMSR sc 
\makemo{x0227C}{PrecedesSlantEqual}    %alias ISOAMSR prcue 
\makemo{x0227C}{preccurlyeq}    %alias ISOAMSR prcue 
\makemo{x0227D}{SucceedsSlantEqual}    %alias ISOAMSR sccue 
\makemo{x0227D}{succcurlyeq}    %alias ISOAMSR sccue 
\makemo{x0227E}{PrecedesTilde}    %alias ISOAMSR prsim 
\makemo{x0227E}{precsim}    %alias ISOAMSR prsim 
\makemo{x0227F}{SucceedsTilde}    %alias ISOAMSR scsim 
\makemo{x0227F}{succsim}    %alias ISOAMSR scsim 
\makemo{x02280}{NotPrecedes}    %alias ISOAMSN npr 
\makemo{x02280}{nprec}    %alias ISOAMSN npr 
\makemo{x02281}{NotSucceeds}    %alias ISOAMSN nsc 
\makemo{x02281}{nsucc}    %alias ISOAMSN nsc 
\makemo{x02282}{subset}    %alias ISOTECH sub 
\makemo{x02283}{Superset}    %alias ISOTECH sup 
\makemo{x02283}{supset}    %alias ISOTECH sup 
\makemo{x02286}{SubsetEqual}    %alias ISOTECH sube 
\makemo{x02286}{subseteq}    %alias ISOTECH sube 
\makemo{x02287}{SupersetEqual}    %alias ISOTECH supe 
\makemo{x02287}{supseteq}    %alias ISOTECH supe 
\makemo{x02288}{NotSubsetEqual}    %alias ISOAMSN nsube 
\makemo{x02288}{nsubseteq}    %alias ISOAMSN nsube 
\makemo{x02289}{NotSupersetEqual}    %alias ISOAMSN nsupe 
\makemo{x02289}{nsupseteq}    %alias ISOAMSN nsupe 
\makemo{x0228A}{subsetneq}    %alias ISOAMSN subne 
\makemo{x0228B}{supsetneq}    %alias ISOAMSN supne 
\makemo{x0228E}{UnionPlus}    %alias ISOAMSB uplus 
\makemo{x0228F}{SquareSubset}    %alias ISOAMSR sqsub 
\makemo{x0228F}{sqsubset}    %alias ISOAMSR sqsub 
\makemo{x02290}{SquareSuperset}    %alias ISOAMSR sqsup 
\makemo{x02290}{sqsupset}    %alias ISOAMSR sqsup 
\makemo{x02291}{SquareSubsetEqual}    %alias ISOAMSR sqsube 
\makemo{x02291}{sqsubseteq}    %alias ISOAMSR sqsube 
\makemo{x02292}{SquareSupersetEqual}    %alias ISOAMSR sqsupe 
\makemo{x02292}{sqsupseteq}    %alias ISOAMSR sqsupe 
\makemo{x02293}{SquareIntersection}    %alias ISOAMSB sqcap 
\makemo{x02294}{SquareUnion}    %alias ISOAMSB sqcup 
\makemo{x02295}{CirclePlus}    %alias ISOAMSB oplus 
\makemo{x02296}{CircleMinus}    %alias ISOAMSB ominus 
\makemo{x02297}{CircleTimes}    %alias ISOAMSB otimes 
\makemo{x02299}{CircleDot}    %alias ISOAMSB odot 
\makemo{x0229A}{circledcirc}    %alias ISOAMSB ocir 
\makemo{x0229B}{circledast}    %alias ISOAMSB oast 
\makemo{x0229D}{circleddash}    %alias ISOAMSB odash 
\makemo{x0229E}{boxplus}    %alias ISOAMSB plusb 
\makemo{x0229F}{boxminus}    %alias ISOAMSB minusb 
\makemo{x022A0}{boxtimes}    %alias ISOAMSB timesb 
\makemo{x022A1}{dotsquare}    %alias ISOAMSB sdotb 
\makemo{x022A2}{RightTee}    %alias ISOAMSR vdash 
\makemo{x022A3}{LeftTee}    %alias ISOAMSR dashv 
\makemo{x022A4}{DownTee}    %alias ISOTECH top 
\makemo{x022A5}{UpTee}    %alias ISOTECH perp 
\makemo{x022A5}{bot}    %alias ISOTECH bottom 
\makemo{x022A8}{DoubleRightTee}    %alias ISOAMSR vDash 
\makemo{x022B2}{LeftTriangle}    %alias ISOAMSR vltri 
\makemo{x022B2}{vartriangleleft}    %alias ISOAMSR vltri 
\makemo{x022B3}{RightTriangle}    %alias ISOAMSR vrtri 
\makemo{x022B3}{vartriangleright}    %alias ISOAMSR vrtri 
\makemo{x022B4}{LeftTriangleEqual}    %alias ISOAMSR ltrie 
\makemo{x022B4}{trianglelefteq}    %alias ISOAMSR ltrie 
\makemo{x022B5}{RightTriangleEqual}    %alias ISOAMSR rtrie 
\makemo{x022B5}{trianglerighteq}    %alias ISOAMSR rtrie 
\makemo{x022B8}{multimap}    %alias ISOAMSA mumap 
\makemo{x022BA}{intercal}    %alias ISOAMSB intcal 
\makemo{x022C0}{Wedge}    %alias ISOAMSB xwedge 
\makemo{x022C0}{bigwedge}    %alias ISOAMSB xwedge 
\makemo{x022C1}{Vee}    %alias ISOAMSB xvee 
\makemo{x022C1}{bigvee}    %alias ISOAMSB xvee 
\makemo{x022C2}{Intersection}    %alias ISOAMSB xcap 
\makemo{x022C2}{bigcap}    %alias ISOAMSB xcap 
\makemo{x022C3}{Union}    %alias ISOAMSB xcup 
\makemo{x022C3}{bigcup}    %alias ISOAMSB xcup 
\makemo{x022C4}{Diamond}    %alias ISOAMSB diam 
\makemo{x022C4}{diamond}    %alias ISOAMSB diam 
\makemo{x022C6}{Star}    %alias ISOAMSB sstarf 
\makemo{x022C7}{divideontimes}    %alias ISOAMSB divonx 
\makemo{x022CB}{leftthreetimes}    %alias ISOAMSB lthree 
\makemo{x022CC}{rightthreetimes}    %alias ISOAMSB rthree 
\makemo{x022CD}{backsimeq}    %alias ISOAMSR bsime 
\makemo{x022CE}{curlyvee}    %alias ISOAMSB cuvee 
\makemo{x022CF}{curlywedge}    %alias ISOAMSB cuwed 
\makemo{x022D0}{Subset}    %alias ISOAMSR Sub 
\makemo{x022D1}{Supset}    %alias ISOAMSR Sup 
\makemo{x022D4}{pitchfork}    %alias ISOAMSR fork 
\makemo{x022D6}{lessdot}    %alias ISOAMSR ltdot 
\makemo{x022D7}{gtrdot}    %alias ISOAMSR gtdot 
\makemo{x022D9}{ggg}    %alias ISOAMSR Gg 
\makemo{x022DA}{LessEqualGreater}    %alias ISOAMSR leg 
\makemo{x022DA}{lesseqgtr}    %alias ISOAMSR leg 
\makemo{x022DB}{GreaterEqualLess}    %alias ISOAMSR gel 
\makemo{x022DB}{gtreqless}    %alias ISOAMSR gel 
\makemo{x022DE}{curlyeqprec}    %alias ISOAMSR cuepr 
\makemo{x022DF}{curlyeqsucc}    %alias ISOAMSR cuesc 
\makemo{x022E0}{NotPrecedesSlantEqual}    %alias ISOAMSN nprcue 
\makemo{x022E1}{NotSucceedsSlantEqual}    %alias ISOAMSN nsccue 
\makemo{x022E2}{NotSquareSubsetEqual}    %alias ISOAMSN nsqsube 
\makemo{x022E3}{NotSquareSupersetEqual}    %alias ISOAMSN nsqsupe 
\makemo{x022E8}{precnsim}    %alias ISOAMSN prnsim 
\makemo{x022E9}{succnsim}    %alias ISOAMSN scnsim 
\makemo{x022EA}{NotLeftTriangle}    %alias ISOAMSN nltri 
\makemo{x022EA}{ntriangleleft}    %alias ISOAMSN nltri 
\makemo{x022EB}{NotRightTriangle}    %alias ISOAMSN nrtri 
\makemo{x022EB}{ntriangleright}    %alias ISOAMSN nrtri 
\makemo{x022EC}{NotLeftTriangleEqual}    %alias ISOAMSN nltrie 
\makemo{x022EC}{ntrianglelefteq}    %alias ISOAMSN nltrie 
\makemo{x022ED}{NotRightTriangleEqual}    %alias ISOAMSN nrtrie 
\makemo{x022ED}{ntrianglerighteq}    %alias ISOAMSN nrtrie 
\makemo{x02305}{barwedge}    %alias ISOAMSB barwed 
\makemo{x02306}{doublebarwedge}    %alias ISOAMSB Barwed 
\makemo{x02308}{LeftCeiling}    %alias ISOAMSC lceil 
\makemo{x02309}{RightCeiling}    %alias ISOAMSC rceil 
\makemo{x0230A}{LeftFloor}    %alias ISOAMSC lfloor 
\makemo{x0230B}{RightFloor}    %alias ISOAMSC rfloor 
\makemo{x0231C}{ulcorner}    %alias ISOAMSC ulcorn 
\makemo{x0231D}{urcorner}    %alias ISOAMSC urcorn 
\makemo{x0231E}{llcorner}    %alias ISOAMSC dlcorn 
\makemo{x0231F}{lrcorner}    %alias ISOAMSC drcorn 
\makemo{x02329}{LeftAngleBracket}    %alias ISOTECH lang 
\makemo{x02329}{langle}    %alias ISOTECH lang 
\makemo{x0232A}{RightAngleBracket}    %alias ISOTECH rang 
\makemo{x0232A}{rangle}    %alias ISOTECH rang 
\makemo{x023B0}{lmoustache}    %alias ISOAMSC lmoust 
\makemo{x023B1}{rmoustache}    %alias ISOAMSC rmoust 
\makemo{x024C8}{circledS}    %alias ISOAMSO oS 
\makemo{x025A1}{Square}    %alias for square 
\makemo{x025AA}{blacksquare}    %ISOTECH squarf 
\makemo{x025B3}{bigtriangleup}    %alias ISOAMSB xutri 
\makemo{x025B4}{blacktriangle}    %alias ISOPUB utrif 
\makemo{x025B5}{triangle}    %alias ISOPUB utri 
\makemo{x025B8}{blacktriangleright}    %alias ISOPUB rtrif 
\makemo{x025B9}{triangleright}    %alias ISOPUB rtri 
\makemo{x025BD}{bigtriangledown}    %alias ISOAMSB xdtri 
\makemo{x025BE}{blacktriangledown}    %alias ISOPUB dtrif 
\makemo{x025BF}{triangledown}    %alias ISOPUB dtri 
\makemo{x025C2}{blacktriangleleft}    %alias ISOPUB ltrif 
\makemo{x025C3}{triangleleft}    %alias ISOPUB ltri 
\makemo{x025CA}{lozenge}    %alias ISOPUB loz 
\makemo{x025EF}{bigcirc}    %alias ISOAMSB xcirc 
\makemo{x02605}{bigstar}    %ISOPUB starf 
\makemo{x02660}{spadesuit}    %ISOPUB spades 
\makemo{x02663}{clubsuit}    %ISOPUB clubs 
\makemo{x02665}{heartsuit}    %ISOPUB hearts 
\makemo{x02666}{diamondsuit}    %ISOPUB diams 
\makemo{x0266E}{natural}    %alias ISOPUB natur 
\makemo{x02713}{checkmark}    %alias ISOPUB check 
\makemo{x02720}{maltese}    %alias ISOPUB malt 
\makemo{x027F5}{LongLeftArrow}    %alias ISOAMSA xlarr 
\makemo{x027F5}{longleftarrow}    %alias ISOAMSA xlarr 
\makemo{x027F6}{LongRightArrow}    %alias ISOAMSA xrarr 
\makemo{x027F6}{longrightarrow}    %alias ISOAMSA xrarr 
\makemo{x027F7}{LongLeftRightArrow}    %alias ISOAMSA xharr 
\makemo{x027F7}{longleftrightarrow}    %alias ISOAMSA xharr 
\makemo{x027F8}{DoubleLongLeftArrow}    %alias ISOAMSA xlArr 
\makemo{x027F8}{Longleftarrow}    %alias ISOAMSA xlArr 
\makemo{x027F9}{DoubleLongRightArrow}    %alias ISOAMSA xrArr 
\makemo{x027F9}{Longrightarrow}    %alias ISOAMSA xrArr 
\makemo{x027FA}{DoubleLongLeftRightArrow}    %alias ISOAMSA xhArr 
\makemo{x027FA}{Longleftrightarrow}    %alias ISOAMSA xhArr 
\makemo{x027FC}{longmapsto}    %alias ISOAMSA xmap 
\makemo{x0290D}{bkarow}    %alias ISOAMSA rbarr 
\makemo{x0290F}{dbkarow}    %alias ISOAMSA rBarr 
\makemo{x02910}{drbkarow}    %alias ISOAMSA RBarr 
\makemo{x02925}{hksearow}    %alias ISOAMSA searhk 
\makemo{x02926}{hkswarow}    %alias ISOAMSA swarhk 
\makemo{x02928}{toea}    %alias ISOAMSA nesear 
\makemo{x02929}{tosa}    %alias ISOAMSA seswar 
\makemo{x0296E}{UpEquilibrium}    %alias ISOAMSA udhar 
\makemo{x0296F}{ReverseUpEquilibrium}    %alias ISOAMSA duhar 
\makemo{x029EB}{blacklozenge}    %alias ISOPUB lozf 
\makemo{x02A00}{bigodot}    %alias ISOAMSB xodot 
\makemo{x02A01}{bigoplus}    %alias ISOAMSB xoplus 
\makemo{x02A02}{bigotimes}    %alias ISOAMSB xotime 
\makemo{x02A04}{biguplus}    %alias ISOAMSB xuplus 
\makemo{x02A06}{bigsqcup}    %alias ISOAMSB xsqcup 
\makemo{x02A0C}{iiiint}    %alias ISOTECH qint 
\makemo{x02A3C}{intprod}    %alias ISOAMSB iprod 
\makemo{x02A77}{ddotseq}    %alias ISOAMSR eDDot 
\makemo{x02A7D}{LessSlantEqual}    %alias ISOAMSR les 
\makemo{x02A7D}{leqslant}    %alias ISOAMSR les 
\makemo{x02A7E}{GreaterSlantEqual}    %alias ISOAMSR ges 
\makemo{x02A7E}{geqslant}    %alias ISOAMSR ges 
\makemo{x02A85}{lessapprox}    %alias ISOAMSR lap 
\makemo{x02A86}{gtrapprox}    %alias ISOAMSR gap 
\makemo{x02A87}{lneq}    %alias ISOAMSN lne 
\makemo{x02A88}{gneq}    %alias ISOAMSN gne 
\makemo{x02A89}{lnapprox}    %alias ISOAMSN lnap 
\makemo{x02A8A}{gnapprox}    %alias ISOAMSN gnap 
\makemo{x02A8B}{lesseqqgtr}    %alias ISOAMSR lEg 
\makemo{x02A8C}{gtreqqless}    %alias ISOAMSR gEl 
\makemo{x02A95}{eqslantless}    %alias ISOAMSR els 
\makemo{x02A96}{eqslantgtr}    %alias ISOAMSR egs 
\makemo{x02AAF}{PrecedesEqual}    %alias ISOAMSR pre 
\makemo{x02AAF}{preceq}    %alias ISOAMSR pre 
\makemo{x02AB0}{SucceedsEqual}    %alias ISOAMSR sce 
\makemo{x02AB0}{succeq}    %alias ISOAMSR sce 
\makemo{x02AB5}{precneqq}    %alias ISOAMSN prnE 
\makemo{x02AB6}{succneqq}    %alias ISOAMSN scnE 
\makemo{x02AB7}{precapprox}    %alias ISOAMSR prap 
\makemo{x02AB8}{succapprox}    %alias ISOAMSR scap 
\makemo{x02AB9}{precnapprox}    %alias ISOAMSN prnap 
\makemo{x02ABA}{succnapprox}    %alias ISOAMSN scnap 
\makemo{x02AC5}{subseteqq}    %alias ISOAMSR subE 
\makemo{x02AC6}{supseteqq}    %alias ISOAMSR supE 
\makemo{x02ACB}{subsetneqq}    %alias ISOAMSN subnE 
\makemo{x02ACC}{supsetneqq}    %alias ISOAMSN supnE 
\makemo{x02AE4}{DoubleLeftTee}    %alias for &Dashv; 
\makemo{x0301A}{LeftDoubleBracket}    %left double bracket delimiter 
\makemo{x0301B}{RightDoubleBracket}    %right double bracket delimiter 

%%%%%%%%%%%%
% Composed symbols
% amsa 
\makemo{x0219D;\&\#x00338}{nrarrw}    %not right arrow-wavy 
\makemo{x02933;\&\#x00338}{nrarrc}    %not right arrow-curved 
% amsb
\makemo{x02229;\&\#x0FE00}{caps}    %intersection, serifs 
\makemo{x0222A;\&\#x0FE00}{cups}    %union, serifs 
\makemo{x0223E;\&\#x00333}{acE}    %most positive, two lines below 
\makemo{x02293;\&\#x0FE00}{sqcaps}    %square intersection, serifs 
\makemo{x02294;\&\#x0FE00}{sqcups}    %square union, serifs 
% amsn
\makemo{x0003C;\&\#x020D2}{nvlt}    %not, vert, less-than 
\makemo{x0003E;\&\#x020D2}{nvgt}    %not, vert, greater-than 
\makemo{x0223C;\&\#x020D2}{nvsim}    %not, vert, similar 
\makemo{x0224B;\&\#x00338}{napid}    %not approximately identical to 
\makemo{x0224D;\&\#x020D2}{nvap}    %not, vert, approximate 
\makemo{x02264;\&\#x020D2}{nvle}    %not, vert, less-than-or-equal 
\makemo{x02265;\&\#x020D2}{nvge}    %not, vert, greater-than-or-equal 
\makemo{x02266;\&\#x00338}{nlE}    %/nleqq N: not less, dbl equals 
\makemo{x02267;\&\#x00338}{ngE}    %/ngeqq N: not greater, dbl equals 
\makemo{x02268;\&\#x0FE00}{lvnE}    %/lvertneqq N: less, vert, not dbl eq 
\makemo{x02269;\&\#x0FE00}{gvnE}    %/gvertneqq N: gt, vert, not dbl eq 
\makemo{x0226A;\&\#x00338}{nLtv}    %not much less than, variant 
\makemo{x0226A;\&\#x020D2}{nLt}    %not, vert, much less than 
\makemo{x0226B;\&\#x00338}{nGtv}    %not much greater than, variant 
\makemo{x0226B;\&\#x020D2}{nGt}    %not, vert, much greater than 
\makemo{x02282;\&\#x020D2}{vnsub}    %/nsubset N: not subset, var 
\makemo{x02283;\&\#x020D2}{vnsup}    %/nsupset N: not superset, var 
\makemo{x0228A;\&\#x0FE00}{vsubne}    %/varsubsetneq N: subset, not eq, var 
\makemo{x0228B;\&\#x0FE00}{vsupne}    %/varsupsetneq N: superset, not eq, var 
\makemo{x022B4;\&\#x020D2}{nvltrie}    %not, vert, left triangle, equals 
\makemo{x022B5;\&\#x020D2}{nvrtrie}    %not, vert, right triangle, equals 
\makemo{x022D8;\&\#x00338}{nLl}    %not triple less than 
\makemo{x022D9;\&\#x00338}{nGg}    %not triple greater than 
\makemo{x02A6D;\&\#x00338}{ncongdot}    %not congruent, dot 
\makemo{x02A70;\&\#x00338}{napE}    %not approximately equal or equal to 
\makemo{x02A7D;\&\#x00338}{nles}    %/nleqslant N: not less-or-eq, slant 
\makemo{x02A7E;\&\#x00338}{nges}    %/ngeqslant N: not gt-or-eq, slanted 
\makemo{x02AAF;\&\#x00338}{npre}    %/npreceq N: not precedes, equals 
\makemo{x02AB0;\&\#x00338}{nsce}    %/nsucceq N: not succeeds, equals 
\makemo{x02AC5;\&\#x00338}{nsubE}    %/nsubseteqq N: not subset, dbl eq 
\makemo{x02AC6;\&\#x00338}{nsupE}    %/nsupseteqq N: not superset, dbl eq 
\makemo{x02ACB;\&\#x0FE00}{vsubnE}    %/varsubsetneqq N: subset not dbl eq, var
\makemo{x02ACC;\&\#x0FE00}{vsupnE}    %/varsupsetneqq N: super not dbl eq, var 
% amso
\makemo{x02220;\&\#x020D2}{nang}    %not, vert, angle 
% amsr
\makemo{x0005C;\&\#x02282}{bsolhsub}    %reverse solidus, subset 
\makemo{x02283;\&\#x0002F}{suphsol}    %superset, solidus 
\makemo{x022DA;\&\#x0FE00}{lesg}    %less, equal, slanted, greater 
\makemo{x022DB;\&\#x0FE00}{gesl}    %greater, equal, slanted, less 
\makemo{x02AAC;\&\#x0FE00}{smtes}    %smaller than or equal, slanted 
\makemo{x02AAD;\&\#x0FE00}{lates}    %larger than or equal, slanted 
%  isotech
\makemo{x0003D;\&\#x020E5}{bne}    %reverse not equal 
\makemo{x02202;\&\#x00338}{npart}    %not partial differential 
\makemo{x02250;\&\#x00338}{nedot}    %not equal, dot 
\makemo{x02261;\&\#x020E5}{bnequiv}    %reverse not equivalent 
\makemo{x022F5;\&\#x00338}{notindot}    %negated set membership, dot above 
\makemo{x022F9;\&\#x00338}{notinE}    %negated set membership, two horizontal strokes 
\makemo{x02AFD;\&\#x020E5}{nparsl}    %not parallel, slanted 
% mmlextra
\makemo{x02009;\&\#x0200A;\&\#x0200A}{ThickSpace}    %space of width 5/18 em 
\makemo{x02242;\&\#x00338}{nesim}    %not equal or similar 
\makemo{x0224E;\&\#x00338}{nbump}    %not bumpy equals 
\makemo{x0224F;\&\#x00338}{NotHumpEqual}    %alias for &nbumpe; 
\makemo{x0224F;\&\#x00338}{nbumpe}    %not bumpy single equals 
\makemo{x0227F;\&\#x00338}{NotSucceedsTilde}    %not succeeds or similar 
\makemo{x0228F;\&\#x00338}{NotSquareSubset}    %square not subset 
\makemo{x02290;\&\#x00338}{NotSquareSuperset}    %negated set-like partial order operator 
\makemo{x029CF;\&\#x00338}{NotLeftTriangleBar}    %not left triangle, vertical bar 
\makemo{x029D0;\&\#x00338}{NotRightTriangleBar}    %not vertical bar, right triangle 
\makemo{x02AA1;\&\#x00338}{NotNestedLessLess}    %not double less-than sign 
\makemo{x02AA2;\&\#x00338}{NotNestedGreaterGreater}    %not double greater-than sign 
% mmlalias
\makemo{x02266;\&\#x00338}{NotGreaterFullEqual}    %alias ISOAMSN nlE 
\makemo{x02266;\&\#x00338}{nleqq}    %alias ISOAMSN nlE 
\makemo{x02267;\&\#x00338}{ngeqq}    %alias ISOAMSN ngE 
\makemo{x02268;\&\#x0FE00}{lvertneqq}    %alias ISOAMSN lvnE 
\makemo{x02269;\&\#x0FE00}{gvertneqq}    %alias ISOAMSN gvnE 
\makemo{x0226A;\&\#x00338}{NotLessLess}    %alias ISOAMSN nLtv 
\makemo{x0226B;\&\#x00338}{NotGreaterGreater}    %alias ISOAMSN nGtv 
\makemo{x02282;\&\#x020D2}{NotSubset}    %alias ISOAMSN vnsub 
\makemo{x02282;\&\#x020D2}{nsubset}    %alias ISOAMSN vnsub 
\makemo{x02283;\&\#x020D2}{NotSuperset}    %alias ISOAMSN vnsup 
\makemo{x02283;\&\#x020D2}{nsupset}    %alias ISOAMSN vnsup 
\makemo{x0228A;\&\#x0FE00}{varsubsetneq}    %alias ISOAMSN vsubne 
\makemo{x0228B;\&\#x0FE00}{varsupsetneq}    %alias ISOAMSN vsupne 
\makemo{x02A7D;\&\#x00338}{NotLessSlantEqual}    %alias ISOAMSN nles 
\makemo{x02A7D;\&\#x00338}{nleqslant}    %alias ISOAMSN nles 
\makemo{x02A7E;\&\#x00338}{NotGreaterSlantEqual}    %alias ISOAMSN nges 
\makemo{x02A7E;\&\#x00338}{ngeqslant}    %alias ISOAMSN nges 
\makemo{x02AAF;\&\#x00338}{NotPrecedesEqual}    %alias ISOAMSN npre 
\makemo{x02AAF;\&\#x00338}{npreceq}    %alias ISOAMSN npre 
\makemo{x02AB0;\&\#x00338}{NotSucceedsEqual}    %alias ISOAMSN nsce 
\makemo{x02AB0;\&\#x00338}{nsucceq}    %alias ISOAMSN nsce 
\makemo{x02AC5;\&\#x00338}{nsubseteqq}    %alias ISOAMSN nsubE 
\makemo{x02AC6;\&\#x00338}{nsupseteqq}    %alias ISOAMSN nsupE 
\makemo{x02ACB;\&\#x0FE00}{varsubsetneqq}    %alias ISOAMSN vsubnE 
\makemo{x02ACC;\&\#x0FE00}{varsupsetneqq}    %alias ISOAMSN vsupnE 
\makemo{x02242;\&\#x00338}{NotEqualTilde}    %alias for &nesim; 
\makemo{x0224E;\&\#x00338}{NotHumpDownHump}    %alias for &nbump; 



\endinput
