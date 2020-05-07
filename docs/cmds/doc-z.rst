All commands, alphabetic order; letter Z
========================================

This page contains the description of `\\z@ <#cmd-zat>`__,
`\\z@skip <#cmd-zskip>`__, `\\zap@space <#cmd-zapspace>`__,
`\\zap@fl@space <#cmd-zapspace>`__, `\\zeta <#cmd-zeta>`__.

--------------

.. _cmd-zat:

\\z@ (rigid length, constant)
-----------------------------

The ``\z@`` command refers to a dimension, which is zero, and should not
be changed. You can say ``\count13=\z@``, because the dimension is
automatically converted to an integer (with value 0). For an example,
see ```\m@ne`` <doc-m.html#cmd-m@ne>`__.

.. _cmd-zskip:

\\z@skip (rubber length, constant)
----------------------------------

The ``\z@skip`` command refers to a length, which is zero, and should
not be changed.

.. _cmd-zapspace:

\\zap@space, \\zap@fl@space (Tralics commands)
----------------------------------------------

The ``\zap@space`` command removes all spaces in its argument. The end
of the argument is marked by a space followed by ``\@empty``. *Tralics*
defines ``\zap@fl@space``, a macro with normal syntax, that removes only
initial and final spaces. In the example that follows, blue lines is
what *Tralics* shows.

.. container:: ltx-source

   ::

      \def\includeonly#1{\edef \@partlist {\zap@space#1  \@empty}}
      \def\remove@fl@space#1#2{%
      \expandafter\def\expandafter #1\expandafter {\zap@fl@space{#2}}}
      \def\foo{foo}
      \includeonly{ bar \foo}
      \remove@fl@space\gee{ \empty\space Hello World }
      \show\@partlist
      % \@partlist=macro: ->barfoo.
      \show\gee
      % \gee=macro: ->\empty \space Hello World.

.. _cmd-zeta:

\\zeta (math symbol)
--------------------

The ``\zeta`` command is valid only in math mode. It generates a Greek
letter: ``<mi>&zeta;</mi>`` (Unicode U+3B6, ζ).
