All commands, alphabetic order; letter Y
========================================

This page contains the description of `\\year <#cmd-year>`__,
`\\yearcite <#cmd-yearcite>`__, `\\yscale <#cmd-yscale>`__,
`\\yscalex <#cmd-yscalex>`__, `\\y@tag, \\@ytag <#cmd-ytag>`__.

--------------

.. _cmd-year:

\\year (internal integer)
-------------------------

The ``\year`` counter is initially set to the current year and can be
changed by saying ``\year=906``. The built-in command ``\today``
produces a string like ``2008/10/29 18:28:46``, that corresponds to the
start of the compilation, while the LaTeX command uses current values of
``\year``, ``\month`` and ``\day``. (See
`scanint <doc-s.html#fct-scanint>`__ for details of argument scanning).
(See ```\time`` <doc-t.html#cmd-time>`__ for an example.)

.. _cmd-yearcite:

\\yearcite (Tralics command)
----------------------------

This is an extension of the ``\cite`` command, explained in section `2.8
Bibliography <raweb.html#rabib>`__ of the raweb. The commands
``\yearcite{foo}`` and ``\yearcite[bar]{foo}`` are equivalent to
``\cite[year][]{foo}`` and ``\cite[year][bar]{foo}``. They are also
equivalent to ``\cite[][]{foo}`` and ``\cite[][bar]{foo}``.

.. _cmd-yscale:

\\yscale
--------

The ``\yscale`` command is defined by the curves package. The value is
``1.0``. See ```\arc`` <doc-a.html#cmd-arc>`__.

.. _cmd-yscalex:

\\yscalex
---------

The ``\yscalex`` command is defined by the curves package. The value is
``0.0``. See ```\arc`` <doc-a.html#cmd-arc>`__.

.. _cmd-ytag:

\\y@tag, \\@ytag (Tralics commands)
-----------------------------------

Internal commands used for equation numbering, see
```\tag`` <doc-t.html#cmd-tag>`__.
