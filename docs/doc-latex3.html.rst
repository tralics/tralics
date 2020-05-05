Implementation of the LaTeX3 Bundle in Tralics
==============================================

Introduction
------------

This document explains the implementation of the LaTeX3 expl3.sty
package. This consists in an extension of the C++ code, a style file
(using normal latex syntax) and a big tex file (using latex3 syntax)
which is expl3-code.tex minus the C++ code.

A partial implementation is available in Tralics version 2.15.4.

The most evident feature of LaTeX3 is that command names contain
underscore and colon characters (no atsign) and are much more explicit.
For instance ``\@ne`` and ``\thr@@`` are LaTeX constants, with value one
and three; in LaTeX3, they are named ``\c_one`` and ``\c_three``.
instead of ``\@tempboxa`` and ``\@tempcnta`` there will be
``\l_tmpa_box`` and ``\g_tmpa_int``; the first one should be used for
local assignment, the second one for global assignment. A variable of
the form ``\l_module_foo_tl`` is local, defined in the module “module”,
named “foo” and contains a token list, while ``\l__module_foo_tl`` is a
private variable of that module. In the case of the kernel, the module
part is omitted from the name.

The two commands ``\module_foo:nnn`` and ``\__module_foo:nnn`` are macro
names, respectively public and private, defined in the module; they take
three arguments (there are three letters after the colon). The meaning
of these letters is the following

-  None. A command whose name ends with a colon takes no argument.
   Example: ``\prg_do_nothing:`` (corresponds to ``\@empty``, is a
   command whose expansion is void, while ``\scan_stop:`` is the sames
   as ``\relax``.
-  ``n``: Unexpanded token or braced token list. Example ``\use:nn``.
   This is a command that takes two arguments, whose expansion is formed
   of these arguments. For instance ``\use:nn {a b}{{c d}}`` expands to
   ``a b{c d}`` (one pair of braces disappears), while ``\use:nn a b``
   expands to ``ab`` (an argument can be preceded by an optional space).
-  ``N``: Single token (unlike ``n``, the argument must not be
   surrounded by braces). Example ``\cs_if_eq:NNTF`` takes for
   arguments, compares the first two, and expands to the third if the
   test is true, to the fourth otherwise. The test is done by
   ``\if_meaning:w`` (aka ``\ifx``) that compares two tokens. It is very
   complicated to grab a single token in TeX, so, generally, all that
   matters is that the arguments consists in a single token; so
   ``\cs_if_eq:NNTF {a}{a} {ok}{\bad}`` expands to ok. If the first
   argument is ``{ab}``, LaTeX compares a and b, and the test is false;
   if the first argument is empty, you could be in trouble. An error is
   signaled in such cases by *Tralics*. On the other hand
   ``\cs_gset_eq:NN`` behaves like ``\global\let``. So
   ``\cs_set_eq:NN    {=}`` tries to redefine the opening brace, which
   is an error (unless the opening brace is an active character, an
   unusual situation).
-  ``T``, ``F``: These are special cases of ``n`` arguments. Example:
   ``\cs_if_eq:NNTF``, ``\cs_if_eq:NNT``, ``\cs_if_eq:NNF`` and
   ``\cs_if_eq_p:NN``, are four commands that process the first two
   arguments in the same way. The command that takes only two parameters
   expand to true or false (this will be explained later on), the first
   one (with four arguments) is explained aove; the other one take three
   argument; the expansion is this argument (if the truth value matches
   the extension) and is empty otherwise. So both
   ``\cs_if_eq:NNT aa {ok}`` and ``\cs_if_eq:NNF ab {ok}`` expand to
   ``ok``. The letters T and F are only used in this case.
-  ``p``: Primitive TeX parameter specification. This is used in command
   that define other commands whose arguments are not specified by a
   suffix. Example: ``\cs_set_nopar:Npn`` is ``\def`` and
   ``\cs_set:Npn`` behaves like ``\long\def``.
-  ``w``: This means that the argument syntaxis “weird” in that it does
   not follow any standard rule. Example ``\if_meaning:w`` cited above;
   this command reads two argument and compares them. It this executes
   the true or false code and skips the other one; this is the “weird”
   part of the syntax. LaTeX3 never defines commands with such a syntax:
   see T and F above.
-  ``D``: This means **Do not use**. Example ``\tex_ifx:D`` and
   ``\tex_let:D``; these are alternate names for ``\ifx`` and ``\let``;
   the idea is to always use macros with a colon in their name in the
   package files. The first macro is also known as ``\if_meaning:w``,
   and the second is like ``\cs_set_eq:NN`` (the difference is that you
   can say ``\let\foo= 3`` where the equals sign and the space that
   follows it are optional, and ``\cs_set_eq:NN~ 3`` makes the active
   character tilde behave like a space and typesets 3.
-  Variants. For instance ``\cs_set_eq:cN {foo}\bar`` is equivalent to
   ``\exp_after:wN \cs_set_eq:cN \cs:w foo\cs_end:\bar`` or, with TeX
   syntax: ``\expandafter \let \csname   foo\endcsname= \bar``. In
   general a variant calls the base function (here ``\cs_set_eq:NN``,
   unless optimiseed for speed.
-  ``c`` variant. As explained above, this means that the argument is
   processed by ``\csname`` (so is a command name). Example ``\use:c``
   is the same as ``\@nameuse`` it reads a sequence of tokens, fully
   expands them, converts the resulting characters into a control
   sequence name (which is the result of the expansion). If the
   expansion contains ``\par``, an error is signaled; note that
   ``\@nameuse`` signals an error is ``\par`` is part of the argument.
   The command ``\cs_if_exist:cTF`` is like ``\cs_if_exist:NTF`` but
   uses ``\if_cs_exist:w`` rather than ``\if_cs_exist:N`` (i.e.,
   ``\ifcsname`` or ``\ifdefined``); the idea is to avoid filling the
   hash table with an undefined token.
-  ``x`` variant. Fully-expanded token or braced token list. Example
   ``\iow_log:x``, which is the same as ``\wlog``; it prints its
   argument in the transcript file; the expansion is done by TeX. Other
   example ``\cs_set_nopar:Npx`` (aka ``\edef``) is the base function
   for doing full expansion. Now ``\use:x{foo}`` is the same as
   ``\edef\tmp{foo}\tmp``. For this reason, the command is not
   expandable, and x-type arguments cannot be nested. Note that
   ``\str_if_eq_x:nnTF`` is similar to ``\str_if_eq:nnTF``. The idea
   here is that the pdftex function that compares two string fully
   expands them; and you have to use ``\unexpanded`` in order to avoid
   this expansion.
