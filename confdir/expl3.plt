% -*- latex -*-
% $Id: expl3.plt,v 1.2 2015/10/23 07:56:56 grimm Exp $

%% Tralics implementation of the latex3 bundle
%%  This is work in progress; works only for versin 2.15.4
%% Assumes that  my-expl3-code.tex is in the current directory


\def\ExplFileName{expl3}
\def\ExplFileDescription{L3 programming layer}
\def\ExplFileDate{2015/08/10}
\def\ExplFileVersion{4814} % must match the number in the main file
\let\ExplLoaderFileVersion\ExplFileVersion
\ProvidesPackage{\ExplFileName}
  [\ExplFileDate\space v\ExplFileVersion\space
    \ExplFileDescription\space (loader) ]

% was \RequirePackage{ifpdf}
\newif\ifpdf \pdffalse

\newcommand\expl@create@bool@option[2]%
  {%
    \DeclareOption{#1}{\chardef #2=1 }%
    \DeclareOption{#1=true}{\chardef #2=1 }%
    \DeclareOption{#1=false}{\chardef #2=0 }%
    \newcommand*#2{}%
    \chardef #2=0 %
  }
\expl@create@bool@option{check-declarations}\l@expl@check@declarations@bool
\expl@create@bool@option{log-functions}\l@expl@log@functions@bool
\let\expl@create@bool@option\@undefined
\newcommand*\l@expl@options@clist{}
\DeclareOption*
  {%
    \ifx\l@expl@options@clist\@empty
      \let\l@expl@options@clist\CurrentOption
    \else
     \expandafter\expandafter\expandafter\def
       \expandafter\expandafter\expandafter\l@expl@options@clist
       \expandafter\expandafter\expandafter
         {\expandafter\l@expl@options@clist\expandafter,\CurrentOption}
    \fi
  }
\ProcessOptions\relax

\protected\def\ProvidesExplPackage#1#2#3#4%
  {%
    \ProvidesPackage{#1}[#2 v#3 #4]%
    \ExplSyntaxOn
  }
\protected\def\ProvidesExplClass#1#2#3#4%
  {%
    \ProvidesClass{#1}[#2 v#3 #4]%
    \ExplSyntaxOn
  }
\protected\def\ProvidesExplFile#1#2#3#4%
  {%
    \ProvidesFile{#1}[#2 v#3 #4]%
    \ExplSyntaxOn
  }

\let\foo\endinput
\input{my-expl3-code.tex}
\foo

% bof
\let\@begindocumenthook\@empty
\let\@filelist\@empty 
\def\@pushfilename{}
\def\@popfilename{}

\protected\def\GetIdInfoLog{}
\AtBeginDocument
  {
    \cs_if_exist:NF \color
      { \DeclareRobustCommand \color [2] [ ] { } }
  }
\__msg_kernel_new:nnnn { expl } { wrong-driver }
  { Driver~request~inconsistent~with~engine:~using~'#2'~driver. }
  {
    You~have~requested~driver~'#1',~but~this~is~not~suitable~for~use~with~the~
    active~engine.~LaTeX3~will~use~the~'#2'~driver~instead.
  }
% simplified code 
\tl_new:N \l__expl_driver_tl
\keys_define:nn { expl }
  {
    driver .choice:,
    driver / auto .code:n =
      { \tl_set:Nn \l__expl_driver_tl { dvips }  },
    driver / dvipdfmx .code:n =
      { \tl_set:Nn \l__expl_driver_tl { dvipdfmx }  },
    driver / dvips .code:n =
      { \tl_set:Nn \l__expl_driver_tl { dvips }  },
    driver / latex2e .code:n =
      { \tl_set:Nn \l__expl_driver_tl { latex2e } },
    driver / pdfmode .code:n =
      {
         \__msg_kernel_error:nnnn { expl } { wrong-driver }  { pdfmode } { dvips }
         \tl_set:Nn \l__expl_driver_tl { dvips }
      },
    driver / xdvipdfmx .code:n =
      {
         \__msg_kernel_error:nnnn { expl } { wrong-driver }  { xdvipdfmx } { dvips }
         \tl_set:Nn \l__expl_driver_tl { dvips }
      },
    driver         .initial:n      = { latex2e } ,
    native-drivers .choice:,
    native-drivers .default:n      = { true },
    native-drivers / false .meta:n = { driver = latex2e },
    native-drivers / true  .meta:n = { driver = auto }
  }
\keys_define:nn { expl }
  {
    check-declarations .bool_set:N = \l@expl@check@declarations@bool,
    log-functions      .bool_set:N = \l@expl@log@functions@bool
  }
\keys_set:nV { expl } \l@expl@options@clist
\str_if_eq:nVTF { latex2e } \l__expl_driver_tl
  {
    \tl_gput_left:Nn \@begindocumenthook { \RequirePackage { graphics } }
    \__msg_kernel_new:nnnn { box } { clipping-not-available }
      { Box~clipping~not~available. }
      {
        The~\box_clip:N~function~is~only~available~when~loading~expl3~
        with~the~"native-drivers"~option.
      }
    \cs_set_protected:Npn \box_clip:N #1
      {
        \hbox_set:Nn #1 { \box_use:N #1 }
        \__msg_kernel_error:nn { box } { clipping-not-available }
      }
    \cs_set_protected:Npn \box_rotate:Nn #1#2
      { \hbox_set:Nn #1 { \rotatebox {#2} { \box_use:N #1 } } }
    \cs_set_protected:Npn \box_resize:Nnn #1#2#3
      {
        \hbox_set:Nn #1
          {
            \resizebox *
              { \__dim_eval:w #2 \__dim_eval_end: }
              { \__dim_eval:w #3 \__dim_eval_end: }
              { \box_use:N #1 }
          }
      }
    \cs_set_protected:Npn \box_resize_to_ht_plus_dp:Nn #1#2
      {
        \hbox_set:Nn #1
          {
            \resizebox * { ! } { \__dim_eval:w #2 \__dim_eval_end: }
              { \box_use:N #1 }
          }
      }
    \cs_set_protected:Npn \box_resize_to_wd:Nn #1#2
      {
        \hbox_set:Nn #1
          {
            \resizebox * { \__dim_eval:w #2 \__dim_eval_end: } { ! }
              { \box_use:N #1 }
          }
      }
    \cs_set_protected:Npn \box_scale:Nnn #1#2#3
      {
        \hbox_set:Nn #1
          {
            \exp_last_unbraced:Nx \scalebox
              { { \fp_eval:n {#2} } [ \fp_eval:n {#3} ] }
              { \box_use:N #1 }
          }
      }
  }
  {
    \cs_set_protected:Npn \ProvidesExplFile #1#2#3#4
      { \ProvidesFile {#1} [ #2~v#3~#4 ] }
    \tl_const:Nn \c__expl_def_ext_tl { def }
    \@onefilewithoptions { l3 \l__expl_driver_tl } [ ] [ ] \c__expl_def_ext_tl
    \cs_set_protected:Npn \ProvidesExplFile #1#2#3#4
      {
        \ProvidesFile {#1} [ #2~v#3~#4 ]
        \ExplSyntaxOn
      }
  }
\tl_put_left:Nn \@pushfilename
  {
    \tl_put_left:Nx \l__expl_status_stack_tl
      {
        \bool_if:NTF \l__kernel_expl_bool
          { 1 }
          { 0 }
      }
    \ExplSyntaxOff
  }
\tl_put_right:Nn \@popfilename
  {
    \tl_if_empty:NTF \l__expl_status_stack_tl
      { \ExplSyntaxOff }
      { \exp_after:wN \__expl_status_pop:w \l__expl_status_stack_tl \q_stop }
  }
\cs_new_protected:Npn \__expl_status_pop:w #1#2 \q_stop
  {
    \tl_set:Nn \l__expl_status_stack_tl {#2}
    \int_if_odd:nTF {#1}
      { \ExplSyntaxOn }
      { \ExplSyntaxOff }
  }
\tl_new:N \l__expl_status_stack_tl
\tl_set:Nn \l__expl_status_stack_tl { 0 }
%% 
%%
%% End of file `expl3.sty'.
