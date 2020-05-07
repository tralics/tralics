All commands, alphabetic order; letter J
========================================

This page contains the description of `\\j \\J <#cmd-j>`__,
`\\jmath <#cmd-jmath>`__, `\\jobname <#cmd-jobname>`__,
`\\Join <#cmd-Join>`__, `\\jot <#cmd-jot>`__.

--------------

.. _cmd-j:

\\j, \\J (constant)
-------------------

The ``\j`` command should produce a j without dot. Currently it produces
a normal j. See the ```\AA`` <doc-a.html#cmd-Caa>`__ command. The ``\J``
command expands to ``J``, but is converted to ``\j`` by
``\MakeLowercase``.

.. _cmd-jmath:

\\jmath (constant)
------------------

The ``\jmath`` command is valid only in math mode. It generates a
miscellaneous symbol. ``<mo>&jmath;</mo>`` (Unicode U+6A, j) See
description of the ```\ldots`` <doc-l.html#cmd-ldots>`__ command.

.. _cmd-jobname:

\\jobname
---------

The expansion of ``\jobname`` is the name of the file that *Tralics* is
assumed to convert (without extension). In some cases, no file is
needed, and ``\jobname`` expands to ``texput``.

.. _cmd-Join:

\\Join (constant)
-----------------

The ``\Join`` command is valid only in math mode. It generates a
relation symbol: ``<mo>&bowtie;</mo>`` (Unicode U+22C8, ⋈). See
description of the ```\le`` <doc-l.html#cmd-le>`__ command.

.. _cmd-jot:

\\jot (rigid length)
--------------------

This is 3pt in LaTeX, a little bit over one millimeter
