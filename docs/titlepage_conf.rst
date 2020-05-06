===============================
Configuration of the title page
===============================

The file config_tpa
-------------------

This is an example of a configuration file for the titlepage. Some long
lines have been split (they start with three dots instead of a line
number). The use of this files is described `here <titlepage.html>`__.

.. container:: ltx-source

   ::

        1 # This is an example of a configuration file for tralics.
        2 # $ Id: config_tpa,v 2.2 2006/06/16 12:32:51 grimm Exp $
        3 
        4 
        5 % Some comments: comments start with % or #
        6 
        7 % this means: take the documentclass value as type name
        8 Type = \documentclass
        9 
       10 % some useless aliases
       11 BeginAlias
       12   article report 
       13   slides inriaslides foiltex
       14 End
       15 
       16 BeginType default
       17   DocType = document devnul.dtd
       18   DocAttrib = x \foo
       19   DocAttrib = y \specialyear
       20   DocAttrib = z \the\day\space \the\month\space \the\year 
       21 BeginCommands
       22   \def\ABC{\recurse\recurse}
       23   \def\DEF{\recurse\recurse}
       24 End
       25 End
       26 
       27 
       28 %% BeginType RA     % Case RA
       29 %%   DocType = raweb raweb.dtd
       30 %%  DocAttrib = year \specialyear
       31 %% #DocAttrib = test "foo"
       32 %% 
       33 %%   makefo="xsltproc --catalogs -o %A.fo %B %C";
       34 %%   makehtml = "xsltproc --catalogs  %B %C";
       35 %%   call_lint = "xmllint --catalogs --valid  --noout %C"
       36 %%   makepdf = "pdflatex -interaction=nonstopmode %w"
       37 %%   %makedvi = "latex -interaction=nonstopmode %w"
       38 %%   % makedvi et dvips pour marie-pierre
       39 %%   %dvitops = "dvips %w.dvi -o %w.ps"
       40 %%   %makedvi = "latex -interaction=nonstopmode %w"
       41 %%   generatedvi = "latex -interaction=nonstopmode %t"
       42 %%   % old latex: "latex \\nonstopmode\\input{%t}"
       43 %%   generateps = "dvips %t.dvi -o %t.ps"
       44 %% End
       45 
       46 % This is used for testing the titlepage command
       47 
       48 BeginType  tpa
       49   DocType = tpa tpa.dtd
       50 BeginTitlePage
       51   \InsertTitlepage <titlepage att1='foo' att2='foo2'> 
      ...             "a1='b1' a2='b2'" "from-tpa='ok' from_type='OK'"
       52   alias \AInsertTitlepage  "" ""
       53   \title <title-element tea='tea-val'> "No title"
       54   \utitle <utitle-element tea='utea-val'> "No title"
       55   \author + <autL al = 'alval' bl = 'blval'>
      ...            <aut auta='autaval'> "No authors"
       56   \uauthor + <uautL al='alval' bl='blval'>
      ...     <aut auta='autaval'> "No authors"
       57   \Eabstract E<abstract ab='AB1'> "no abstract1"
       58   \eabstract e<abstract ab='AB2'> "no abstract2"
       59   \pabstract p<abstract ab='AB3'> "no abstract3"
       60   \abstract <abstract ab='AB4'> "no abstract4"
       61   alias \abstractalias
       62   \uEabstract E<uabstract ab='AB5' > "no abstract1"
       63   \ueabstract e<uabstract ab='AB6'> "no abstract2"
       64   \upabstract p<uabstract ab='AB7'> "no abstract3"
       65   \uabstract <uabstract ab='AB8'> "no abstract4"
       66   alias \uabstractalias
       67   <UR> -
       68   \URsop ?+ <UR>
       69   \URlor ?+ <UR>
       70   \Paris ?<UR> <Rocq>
       71   <sUR fr='unit� de recherche' en='research unit'> -
       72   \sURsop ?+ <sUR fr='dans le sud'>
       73   \sURlor ?+ <sUR fr=" dans l'est">
       74   \sParis ?<sUR> <Rocq en='nearparis'>
       75   \myself \author "JG"
       76   \himself \author "DEK"  # this is a list def
       77   alias \autk
       78   \address  <Address>  "no adress"
       79   \waddress \address "somewhere" # this is a normal def
       80 
       81   \cmdp <cmdp> +"nodefault" 
       82   \cmda <cmdA> A"\cmdAval" 
       83   \cmdb <cmdB> B"\cmdBval" 
       84   \cmdc <cmdC> C"\cmdCval" 
       85 
       86 End
       87 BeginCommands
       88   \def\recurse{\recurse\recurse}
       89 EndCommands
       90 End
       91 
       92 # This one is used for testing entity renaming
       93 
       94 BeginType unused
       95   xml_labelitem_name = "Xlabel"
       96   xml_gloitem_name = "Xglolabel"
       97   xml_item_name = "Xitem"
       98   xml_div0_name = "Xdiv0"
       99   xml_div1_name = "Xdiv1"
      100   xml_div2_name = "Xdiv2"
      101   xml_div3_name = "Xdiv3"
      102   xml_div4_name = "Xdiv4"
      103   xml_div5_name = "Xdiv5"
      104   xml_div6_name = "Xdiv6"
      105   xml_subfigure_name = "Xsubfigure"
      106   xml_row_name = "Xrow"
      107   xml_cell_name = "Xcell"
      108   xml_keywords_name = "Xkeywords"
      109   xml_term_name = "Xterm"
      110   xml_mbox_name = "Xmbox"
      111   xml_topic_name = "Xtopic"
      112   xml_caption_name = "Xcaption"
      113   xml_footnote_name = "Xnote"
      114   xml_fbox_name = "Xfbox"
      115   xml_box_name = "Xbox"
      116   xml_picture_name = "Xpicture"
      117   xml_graphics_name = "Xgraphics"
      118   xml_figure_name = "Xfigure"
      119   xml_table_name = "Xtable"
      120   xml_Table_name = "XTable"
      121   xml_bezier_name = "Xbezier"
      122   xml_put_name = "Xput"
      123   xml_line_name = "Xline"
      124   xml_vector_name = "Xvector"
      125   xml_oval_name = "Xoval"
      126   xml_dashline_name = "Xdashline"
      127   xml_drawline_name = "Xdrawline"
      128   xml_dottedline_name = "Xdottedline"
      129   xml_circle_name = "Xcircle"
      130 EndType
      131 
      132 # This one was used for the eurotex paper
      133 
      134 BeginType  eurotex     % Case eurotex 2003
      135   DocType = eurotex tugboat.dtd
      136 BeginTitlePage
      137   \maketitle <titlepage> "" ""
      138   \title <ti> "No title"
      139   \netaddress <NetA>  "No address"
      140   \personalURL <U> "No url given"
      141   \resume E<resume> "Pas de r\'esum\'e"
      142   \abstract E<abstract> "no abstract"
      143   \author + <author> <auth> "No authors"
      144   \address p<address>  "no address"
      145 End
      146 BeginCommands
      147   \def\recurse{\recurse\recurse}
      148 EndCommands
      149 End
      150 
      151 
      152 # more aliases
      153 BeginAlias
      154   tpa foo foo2004
      155   TPA foo
      156   unused foo  foo2003
      157 End

.. _cedram:

The file cedram.tcf
-------------------

.. container:: ltx-source

   ::

      201 ## This is an example of a configuration file for tralics
      202 ## Copyright 2006 Inria/apics, Jose' Grimm
      203 ## $Id: cedram.tcf,v 1.1 2006/07/17 09:09:06 grimm Exp $
      204 ## tralics ident rc=cedram.tcf $Revision: 1.1 $
      205   DocType = cedram cedram.dtd #
      206 BeginTitlePage 
      207   \maketitle <article> "" "'only title page' 
      ...      'translate also bibliography'"
      208    action \getproduction
      209   action \getid 
      210   \daterecieved <date_reception> "Pas de date"
      211   \dateaccepted <date_acceptation> "Pas de date"
      212   "<auteur>"
      213 #  <nomcomplet> "NC"
      214    \firstname <prenom> "FN"  
      215    \middlename <middlename> "MN"  
      216    \lastname <nom> "LN"  
      217 
      218   \address q<adresse> "no ad"
      219   \email <mel> "no mail"
      220   "</auteur>"
      221 # <nombre_auteurs>
      222 # <nombre_contributeurs>
      223   \Ctitle <titre xml:lang='fr'> "TF"
      224   \Dtitle <TeXtitre xml:lang='fr'> "TF"
      225   \Atitle <titre xml:lang='en'> "TE"
      226   \Btitle <TeXtitre xml:lang='en'> "TE"
      227   \currentlanguage <langue> "en"
      228   \Aabstract e<resume xml:lang='en' > "no abstract"
      229   \Babstract e<TEXresume xml:lang='en' > "no abstract"
      230   \Cabstract e<resume xml:lang='fr' > "no abstract"
      231   \Dabstract e<TEXresume xml:lang='fr' > "no abstract"
      232   \altkeywords <motcle xml:lang='fr'> "pas de resume"
      233   \keywords <motcle xml:lang='en' > "no abstract"
      234   \subjclass <msc> "MSC"
      235 End
      236 
      237 BeginCommands
      238   \let\author\relax
      239   \def\getproduction{\xbox{production}{
      240    \xbox{fichier\_tex}{\jobname}
      241    \xbox{fichier\_bib}{\jobname}
      242    \xbox{date\_prod}{\the\year-\the\month-\the\day}}}
      243   \makeatletter 
      244   \def\title{\@reevaluate\Atitle\Btitle}
      245   \def\abstract{\@reevaluate*{Aabstract}{Babstract}}
      246   \def\altabstract{\@reevaluate*{Cabstract}{Dabstract}}
      247   \def\Btitle@helper{\@nomathml=-1 }
      248   \def\Dtitle@helper{\@nomathml=-1 }
      249   \def\Babstract@helper{\@nomathml=-1 }
      250   \def\Dabstract@helper{\@nomathml=-1 }
      251   \def\alttitle{\@reevaluate\Ctitle\Dtitle}
      252    % for the biblio
      253    \renewcommand\bpers[5][\undefined]{%
      254    \xbox{nom}{#4}%
      255  \ifx#1\undefined\xbox{prenom}{#2}\else
      ...         \xbox{prenom}{#1}\xbox{initiale}{#2}\fi
      256  \xbox{particule}{#3}%
      257  \def\tmp{#1\undefined}\ifx\tmp\undefined\else
      258  \xbox{junior}{#5}\fi}
      259 
      260   \expandafter\def\csname cititem-btitle\endcsname{%
      261     \bgroup\catcode `\$=12 \@nomathml=1 \citembtitle}
      262      \def\citembtitle#1{\xbox{title}{#1}\egroup\@addnl}
      263   \def\lastname@hook#1{\textit{#1}}
      264 # Following commands used by T.B.
      265   \let\nfrac\dfrac
      266   \let\pfrac\dfrac
      267   \newtheorem{theorem}{Theorem}
      268   \newtheorem{proof}{Proof}
      269   \newtheorem{lemma}{Lemma}
      270   \newtheorem{rema*}{Remark}
      271   \newtheorem{remas*}{Remarks}
      272   \newtheorem{defi}{Definition}
      273   \let \tn\text
      274   \let\appendix\relax
      275   \let\notag\relax
      276   \def\sideset#1#2{}
      277   \makeatother 
      278 EndCommands

.. _tpa:

The file tpa.tcf
----------------

.. container:: ltx-source

   ::

      301 ## This is an example of a configuration file for tralics
      302 ## Copyright 2006 Inria/apics, Jose' Grimm
      303 ## $Id: tpa.tcf,v 1.3 2006/07/21 08:47:03 grimm Exp $
      304 ## tralics ident rc=testa.tcf $Revision: 1.3 $
      305 
      306 
      307 
      308   DocType = tpa tpa.dtd
      309 
      310 
      311 BeginTitlePage
      312   \InsertTitlepage <titlepage att1='foo' att2='foo2'>
      ...     "a1='b1' a2='b2'" "from-tpa='ok' from_type='OK'"
      313   alias \AInsertTitlepage  "" ""
      314   \title <title-element tea='tea-val'> "No title"
      315   \utitle <utitle-element tea='utea-val'> "No title"
      316   \author + <autL al = 'alval' bl = 'blval'> 
      ...     <aut auta='autaval'> "No authors"
      317   \uauthor + <uautL al='alval' bl='blval'> 
      ...     <aut auta='autaval'> "No authors"
      318   \Eabstract E<abstract ab='AB1'> "no abstract1"
      319   \eabstract e<abstract ab='AB2'> "no abstract2"
      320   \pabstract p<abstract ab='AB3'> "no abstract3"
      321   \abstract <abstract ab='AB4'> "no abstract4"
      322   alias \abstractalias
      323   \uEabstract E<uabstract ab='AB5' > "no abstract1"
      324   \ueabstract e<uabstract ab='AB6'> "no abstract2"
      325   \upabstract p<uabstract ab='AB7'> "no abstract3"
      326   \uabstract <uabstract ab='AB8'> "no abstract4"
      327   alias \uabstractalias
      328   <UR> -
      329   \URsop ?+ <UR>
      330   \URlor ?+ <UR>
      331   \Paris ?<UR> <Rocq>
      332   <sUR fr='unit&#xe9; de recherche' en='research unit'> -
      333   \sURsop ?+ <sUR fr='dans le sud'>
      334   \sURlor ?+ <sUR fr=" dans l'est">
      335   \sParis ?<sUR> <Rocq en='nearparis'>
      336   \myself \author "JG"
      337   \himself \author "DEK"  # this is a list def
      338   alias \autk
      339   \address  <address>  "no adress"
      340   \waddress \address "somewhere" # this is a normal def
      341 
      342   \cmdp <cmdp> +"nodefault" 
      343   \cmda <cmdA> A"\cmdAval" 
      344   \cmdb <cmdB> B"\cmdBval" 
      345   \cmdc <cmdC> C"\cmdCval" 
      346 
      347 End
      348 BeginCommands
      349   \def\recurse{\recurse\recurse}
      350 EndCommands

`back to description <titlepage.html>`__
