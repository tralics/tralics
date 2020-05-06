====================================
Welcome to the Tralics documentation
====================================

.. toctree::
   :maxdepth: 1
   :caption: Using tralics
   :hidden:

   index.orig
   options
   packages
   array
   chars
   err
   latex3
   loop
   start
   step
   mml_ex
   fp
   laux
   raweb
   raweb_examples
   raweb_cfg
   auxdoc
   cmds/index
   Changelog <changelog>

.. toctree::
   :maxdepth: 1
   :caption: Code documentation
   
.. note::

   This is an important note

.. warning::

   This is an important warning

.. versionadded:: 1.0

   That feature is new

.. deprecated:: 1.0

   That feature is old

.. py:function:: enumerate2(sequence[, start=0])

   Return an iterator that yields tuples of an index and an item of the
   *sequence*. (And so on.) (Alternate version.)

.. seealso::

   Some related info

   Some other related info

.. math::

   (a + b)^2  &=  (a + b)(a + b) \\
              &=  a^2 + 2ab + b^2

Hlists
------

* one
* two
* three

.. hlist::

   * four
   * five
   * six

Code
----

.. code-block:: C++

   size_t length (std::string &s) {
      return s.size();
   }

C++
---

sldkjf and the :cpp:func:`length` function of classes :cpp:struct:`bla` and
:cpp:struct:`bla`.

.. cpp:function:: int length (const std::string &s, bla alt)

   Computes the length of the string.

.. cpp:struct:: bla
