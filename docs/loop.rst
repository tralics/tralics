Example of the \\loop command in Tralics
========================================

In this page we describe a non-trivial example of the
`\\loop <doc-l.html#cmd-loop>`__ command. We also show how to analyse
the transcript file produced by *Tralics*.

The example
-----------

Consider a document that contains the following code:

.. container:: ltx-source

   ::

      % this is file testloop.tex $Revision: 1.4 $
      %\tracingall
      \let\olda\a %% \newcount defines \a globally...
      \countdef\td 4 \td=0%% no need to allocate a counter
      \newif\ifprime \newif\ifunknown
      \newcount\n \newcount\p \newcount\d \newcount\a \newcount\N
      \def\primes#1{2,~3% assume that #1 is at least 3
        \n=#1 \advance\n by-2 % n more to go
        \p=5 % odd primes starting with p
        \loop\ifnum\n>0 \printifprime\advance\p by2 \repeat}
      \def\printp{,% we will invoke \printp if p is prime
        \N=\n \divide\N by 8 \multiply \N by -8 \advance\N by \n
        \ifnum\N=0 {
        }\else{ }\fi 
        \ifnum\n=1 and~\fi % this precedes the last value
        \number\p \advance\n by -1 }
      \def\printifprime{\testprimality \ifprime\printp\fi}
      \def\testprimality{{\d=3 \global\primetrue
        \loop\trialdivision \ifunknown\advance\d by2 \repeat}}
      \def\trialdivision{\a=\p \divide\a by\d
          \global\advance\td by 1 %% count calls to trialdivision
        \ifnum\a>\d \unknowntrue\else\unknownfalse\fi
        \multiply\a by\d
        \ifnum\a=\p \global\primefalse\unknownfalse\fi}

      The first thirty prime numbers are \primes{30}. trial division macro was
      expanded \the\td\ times. %% Knuth says cpu time=4 sec, I see 0.02
      \let\a\olda %% restore

The result
----------

The XML translation by *Tralics* is:

.. container:: xml_out

   ::

       
      <?xml version='1.0' encoding='iso-8859-1'?>
      <!DOCTYPE unknown SYSTEM 'unknown.dtd'>
      <!-- translated from latex by tralics 2.2 (pl3)-->
      <unknown>
      <p>The first thirty prime numbers are 2,&nbsp;3, 5, 7, 11, 13,
      17, 19, 23, 29, 31, 37, 41, 43,
      47, 53, 59, 61, 67, 71, 73, 79,
      83, 89, 97, 101, 103, 107, 109, and&nbsp;113. trial division macro was
      expanded 132 times.</p>
      </unknown>

The example above is in the `TeXbook <index.html#texbook>`__, at the end
of Chapter 20. We just added the code that computes the number of calls
of ``\trialdivision``, and the test of ``\n`` mod 8, that avoids long
lines in the XML output. You may wish to look at the description of:
`\\newcount <doc-n.html#cmd-newcount>`__,
`\\newif <doc-n.html#cmd-newif>`__, `\\ifnum <doc-i.html#cmd-ifnum>`__.

What is in the transcript file?
-------------------------------

We show here the content of the transcript file for version 2.15.4
rather than 2.2 (pl3) or 2.7 (pl5). The XML file is the same, modulo (a)
the encoding, which is now UTF-8, (b) the comment line with the version
also indicates teh compilation time, (c) ``&nbsp;`` has been replaced
aby a character. The transcript file starts like this

.. container:: log_out

   ::

      Transcript file of tralics 2.15.4 for file testloop1.tex
      Copyright INRIA/MIAOU/APICS/MARELLE 2002-2015, Jos\'e Grimm
      Tralics is licensed under the CeCILL Free Software Licensing Agreement
      Start compilation: 2015/12/02 16:44:19
      OS: Apple, machine maclaurin
      Starting translation of file testloop1.tex.
      Output encoding: UTF8  (idem transcript).
      Left quote is ` right quote is '
      \notrivialmath=1
      ++ Input encoding is 1 (iso-8859-1) for the main file
      ++ file .tralics_rc does not exist.
      ++ file ../confdir/.tralics_rc exists.
      Configuration file identification: standard $ Revision: 2.24 $
      Read configuration file ../confdir/.tralics_rc.
      Configuration file has type \documentclass
      No \documentclass in source file
      Using some default type
      dtd is unknown from unknown.dtd (standard mode)
      OK with the configuration file, dealing with the TeX file...
      There are 30 lines

These lines are printed before translation starts. What you can see is
that some configuration file was used, that did not define a document
type for *Tralics* compatible with the current ``\documentclass`` (there
is no document class specified in the source). As a consequence, the
main XML element will be called ``<unknown>``.

Let's remove the comment on the line with ``\tracingall``. This will
have as effect to add much more lines to the transcript file. (We could
also use the --verbose flag, this would enter verbose mode from the
start, and add some additional lines to the transcript file).

As a general rule, all lines of the form [number] text indicate source
lines, read by *Tralics*. Other lines start with a plus sign, an open
brace, or a command. A line of the form {\let} indicates that the main
command interpreter of *Tralics* has executed the command ``\let``. In
some cases, a second line is printed by the C++ code associated to the
command. Here you can see the arguments read by the command, and the
effects: new and old value of ``\a``.

.. container:: log_out

   ::

      [3] \let\olda\a %% \newcount defines \a globally...
      {\let}
      {\let \olda \a}
      {changing \olda=undefined}
      {into \olda=\a}

Here, you can see that ``\newif`` reads a command, and calls twice the
inter code of ``\let``.

.. container:: log_out

   ::

      [5] \newif\ifprime \newif\ifunknown
      {\newif}
      {\newif \ifprime}
      {changing \primetrue=undefined}
      {into \primetrue=macro:->\let \ifprime \iftrue }
      {changing \primefalse=undefined}
      {into \primefalse=macro:->\let \ifprime \iffalse }

This is now the trace for ``\global\primetrue``. The ``\global`` command
is a prefix that applies to the assignment that follows, that can be of
the form ``\def cmd`` or ``\count nbr=...``; the next token is fully
expanded. In this case, ``\primetrue`` is a user defined command, the
transcript file shows the body and the arguments (there are none here).
The third line shows the argument of ``\global`` (the first token in the
assignment sequence). What happens is the following; if the first
non-expandable token is not an assignment operator, you will get an
error of the form *You cannot use the prefix \\global before \\par*.
Otherwise, the assignment will be marked global. You can have more than
one prefix (for instance, before ``\def``); so you can see *You cannot
use the prefix \\global\long before \\count*.

.. container:: log_out

   ::

      {\global}
      \primetrue ->\let \ifprime \iftrue 
      {\global\let}
      {\let \ifprime \iftrue}
      {globally changing \ifprime=\iffalse}
      {into \ifprime=\iftrue}

This behaviour is different from TeX: if you say
``\global \let \ifprime \iftrue``, TeX will print only one line in its
transcript file. And for the code above, you will see:

.. container:: log_out

   ::

      {\global}
      \primetrue ->\let \ifprime \iftrue 

Chapter 24 of the `TeXbook <index.html#texbook>`__ starts with
explanations of what a <number> or a <dimen> is. These rules are
complicated, this is why *Tralics* prints in the transcript file lines
starting with +scanint or +scandimen whenever such a thing is scanned.
In general the result is used by a command (here ``\countdef``) or put
in a register (here register number 4, via its symbolic name ``\td``).
If you ask: what is ``\td``, the answer will be ``\count4`` (previous
versions of *Tralics* printed something like ``\countref1540``, for a
command that accesses to position 1540 in the table of integers). If the
value of a counter is changed, you see the old and new value. If these
two values are equal, you see reassigning instead of changing.

.. container:: log_out

   ::

      [4] \countdef\td 4 \td=0
      {\countdef}
      +scanint for \countdef->4
      {\countdef \td=\count4}
      {\count4}
      +scanint for \td->0
      {reassigning \count4=0}
      ...
      {\divide}
      +scanint for \N->8 %this divides N by 8
      {changing \count69=28 into \count69=3}

In previous versions of *Tralics*, the third line was of the form
``{\countdef \n=1553}``, where 1553 is to be interpreted as above (since
1540 was the index for counter 4, an easy computation shows that 1553 is
the index of counter 17; the current version improves a bit the
interpretation of the transcript file; even in silent mode, the
transcript contains a line for each use of command of the type
``\countdef``.

.. container:: log_out

   ::

      [6] \newcount\n \newcount\p \newcount\d \newcount\a \newcount\N
      {\newcount}
      {\countdef \n=\count65}
      ...
      {\countdef \N=\count69}

You may see lines like these when a group starts. Here a begin-group
character, i.e., a character of ``\catcode`` 1, an open brace, caused a
new group to be pushed on the stack. The current level is increased to 2
(bottom level is number 1). You can also see that the current line is 27
(because all the stack alterations are the result of the ``\primes``
command.)

.. container:: log_out

   ::

      {begin-group character}
      +stack: level + 2 for brace entered on line 27

You may see lines like these when a group ends. Here an end-group
character, i.e., a character of ``\catcode`` 2, a closing brace, caused
the stack to be unwound. The current execution level decreases (from 2
down to 1). The commands ``\ifunknown`` and ``\iterate`` are restored
(we truncated the value). Two integers are restored (``\d`` and ``\a``).

.. container:: log_out

   ::

      {end-group character}
      +stack: restoring \ifunknown=\iffalse.
      +stack: restoring \count68=0.
      +stack: restoring \iterate=macro:->\ifnum ...
      +stack: restoring \count67=0.
      +stack: level - 2 for brace from line 27

If in TeX, you say ``\par The first thirty prime numbers are``, a line
is added to the transcript for each group of letters (a line is also
added for each non-letter, for instance a space). As can been seen, a
paragraph is started, and horizontal mode is entered because of the
first letter.

.. container:: log_out

   ::

      {the letter T}
      {horizontal mode: the letter T}
      {blank space  }
      {the letter f}
      {blank space  }
      {the letter t}
      {blank space  }
      {the letter p}
      {blank space  }
      {the letter n}
      {blank space  }
      {the letter a}
      {blank space  }

This is the *Tralics* transcript file

.. container:: log_out

   ::

      Character sequence: The first thirty prime numbers are .

You can see that there is no difference between spaces and other
characters. The character sequence starts with the character that
follows the space after colon, and ends with the characters that
precedes the period (in the example, the first character is a T, the
last is a space). Currently, mode changes are not logged.

Analysis of the transcript file
-------------------------------

This is what the transcript file contains, starting with the line that
follows the definitions. Lines preceding line 25 contain definitions and
global assignments. Current mode is outer vertical, so that newline
characters between definitions are ignored. Empty lines and ``\par``
commands are ignored as well, but appear in the transcript.

.. container:: log_out

   ::

      [25] 
      {\par}
      [26] 
      {\par}

A new paragraph is started when the letter T is seen. The following
things happen: current mode becomes horizontal; a new XML element is
created; a new frame is pushed on the stack. What you see in the
transcript file is the name of the frame, in some cases, as here, the
frame and the element have the same name. At the end of the run, the XML
tree contains three sons: the ``<p>`` element created here, and two
newline characters, one before and one after. This element contains one
son: a piece of text.

For every character translated, something is printed in the transcript
file. The transcript file is flushed whenever a non-character is seen
(here the ``\primes`` command). The characters are put in another
buffer, flushed when needed.

.. container:: log_out

   ::

      [27] The first thirty prime numbers are \primes{30}. trial division macro was
      {Push p 1}
      Character sequence: The first thirty prime numbers are .

You can see the body and arguments of the ``\primes`` command (the
transcript file contains a single line, contrarily to TeX that inserts
newline characters whenever the line is full). You can also see that the
expansion of ``~`` is ``\nobreakspace``. The lines shown here add some
characters to the XML buffer, namely 2, 3, a comma, and whatever
``\nobreakspace`` translates to.

.. container:: log_out

   ::

      \primes #1->2,~3\n =#1 \advance \n by-2 \p =5 \loop \ifnum \n >0 
          \printifprime \advance \p by2 \repeat 
      #1<-30
      Character sequence: 2,.
      ~ ->\nobreakspace 
      {\nobreakspace}
      Character sequence: 3.

This is the transcript for ``\n =#1 \advance \n by-2 \p =5``.

.. container:: log_out

   ::

      {\count65}
      +scanint for \n->30
      {changing \count65=0 into \count65=30}
      {\advance}
      +scanint for \n->-2
      {changing \count65=30 into \count65=28}
      {\count66}
      +scanint for \p->5
      {changing \count66=0 into \count66=5}

This is the outer loop: we have to compute ``\n`` prime numbers (since
we have already printed 2 and 3, the current value of ``\n`` is 28), and
the candidate ``\p`` is 5. After this is found to be prime or not-prime,
we increment it by two (because even numbers greater than two cannot be
prime) and continue the loop.

.. container:: log_out

   ::

      {\loop}
      {changing \iterate=undefined}
      {into \iterate=macro:->\ifnum \n >0 ... }
      \iterate ->\ifnum \n >0 \printifprime \advance \p by2 \relax
          \expandafter \iterate \fi 

Here you can see the expansion of ``\ifnum \n >0``. Two numbers are
compared, the result is true. *Tralics* indicates the values, it also
indicates for each conditional its serial number.

.. container:: log_out

   ::

      +\ifnum1
      +scanint for \ifnum->28
      +scanint for \ifnum->0
      +iftest1 true

We do not indicate here what is done with the number 5. We skip some
code and go to 13.

.. container:: log_out

   ::

      \printifprime ->\testprimality \ifprime \printp \fi 
      ...

The idea is the following: we open a group, and set ``\d`` to 3. We make
the a priori assumption that our number is prime (the ``\global`` is
needed, because we have to know, outside the group, whether the number
is prime or not). The ``\trialdivision`` macro sets ``\ifunknown`` to
some value: if set to false, the status is known, indicated by
``\ifprime``. The number ``\d`` is incremented by 2. Loop invariant: we
assume that ``\p`` is not a multiple of any prime number less that
``\d``).

.. container:: log_out

   ::

      \printifprime ->\testprimality \ifprime \printp \fi 
      \testprimality ->{\d =3 \global \primetrue \loop \trialdivision 
         \ifunknown \advance \d by2 \repeat }
      {begin-group character}
      +stack: level + 2 for brace entered on line 27
      {\count67}
      +scanint for \d->3
      {changing \count67=0 into \count67=3}
      {\global}
      \primetrue ->\let \ifprime \iftrue 
      {\global\let}
      {\let \ifprime \iftrue}
      {globally changing \ifprime=\iftrue}
      {into \ifprime=\iftrue}

We are now in the inner loop. (note that ``\iterate`` is locally
redefined for each loop)

.. container:: log_out

   ::

      {\loop}
      {changing \iterate=macro:->... }
      {into \iterate=macro:-> ...  }
      \iterate->\trialdivision \ifunknown \advance \d by2 \relax 
        \expandafter \iterate \fi 

We divide 13 by 3, quotient in ``\a``, and increment the number of
divisions. We set ``\ifunknown`` to false, because 3 is less than 4.
Said otherwise our divider ``\d`` is less that the square root of
``\p``, not big enough to conclude that ``\p`` is prime.

.. container:: log_out

   ::

      \trialdivision ->\a =\p \divide \a by\d 
         \global \advance \td by 1 
         \ifnum \a >\d \unknowntrue \else \unknownfalse \fi
         \multiply \a by\d \ifnum \a =\p \global \primefalse \unknownfalse \fi 
      {\count68}
      +scanint for \a->13
      {changing \count68=0 into \count68=13}
      {\divide}
      +scanint for \a->3
      {changing \count68=13 into \count68=4}
      {\global}
      {\global\advance}
      +scanint for \td->1
      {globally changing \count4=4 into \count4=5}
      +\ifnum28
      +scanint for \ifnum->4
      +scanint for \ifnum->3
      +iftest28 true
      \unknowntrue ->\let \ifunknown \iftrue 
      {\let}
      {\let \ifunknown \iftrue}
      {changing \ifunknown=\iffalse}
      {into \ifunknown=\iftrue}
      +\else28
      +\fi28

We multiply 4 by 3, in ``\a``, and compared to ``\p``. Comparison is
false, ``\d`` does not divide ``\p``. We cannot conclude that ``\p`` is
not prime.

.. container:: log_out

   ::

      {\multiply}
      +scanint for \a->3
      {changing \count68=4 into \count68=12}
      +\ifnum29
      +scanint for \ifnum->12
      +scanint for \ifnum->13
      +iftest29 false
      +\fi29

Since we do not know that our number is prime, we must continue
checking. We first increment out divider. We find it convenient to
exchange the order of the ``\iterate`` and ``\fi`` tokens (in this case,
the only effect of the ``\fi`` is to pop the condition stack). Purpose
of the ``\relax``: the scanint procedure reads an integer, and could
evaluate the ``\expandafter`` without this token (said otherwise, the
space after the digit 2 is not necessary).

.. container:: log_out

   ::

      +\ifunknown30
      +iftest30 true
      {\advance}
      +scanint for \d->2
      {changing \count67=3 into \count67=5}
      {\relax}
      {\expandafter \iterate \fi}
      +\fi30

We have to execute the loop a second time.

.. container:: log_out

   ::

      \iterate ->\trialdivision \ifunknown \advance \d by2 \relax 
        \expandafter \iterate \fi 
      \trialdivision ->\a =\p \divide \a by\d \global \advance \td by 1 
        \ifnum \a >\d \unknowntrue \else \unknownfalse \fi 
        \multiply \a by\d \ifnum \a =\p \global \primefalse \unknownfalse \fi 

We see here that 5 does not divide 13. However 13 is less than
``5\times(5+1)``, hence is prime. Said otherwise, conditional number 31
shows that primality of 13 depends only on the result of test 32.

.. container:: log_out

   ::

      {\count68} 
      +scanint for \a->13
      {changing \count68=12 into \count68=13}
      {\divide}
      +scanint for \a->5
      {changing \count68=13 into \count68=2}
      {\global}
      {\global\advance}
      +scanint for \td->1
      {globally changing \count4=5 into \count4=6}
      +\ifnum31
      +scanint for \ifnum->2
      +scanint for \ifnum->5
      +iftest31 false
      +\else31
      \unknownfalse ->\let \ifunknown \iffalse 
      {\let}
      {\let \ifunknown \iffalse}
      {changing \ifunknown=\iftrue}
      {into \ifunknown=\iffalse}
      +\fi31
      {\multiply}
      +scanint for \a->5
      {changing \count68=2 into \count68=10}
      +\ifnum32
      +scanint for \ifnum->10
      +scanint for \ifnum->13
      +iftest32 false
      +\fi32

Since we know whether our number is prime or not, it is time to conclude
the inner loop. The assignment ``\let\iterate\relax`` is not really
useful, as will be seen in a minute. Its purpose is to release memory.

.. container:: log_out

   ::

      +\ifunknown33
      +iftest33 false
      +\fi33
      {\let}
      {\let \iterate \relax}
      {changing \iterate=macro:->... }
      {into \iterate=\relax}

The last action of ``\testprimality`` is to close the current group.
What you can see is that two integers are restored (``\a`` and ``\d``).
The value of ``\iterate`` is restored; this is the code of the outer
loop.

.. container:: log_out

   ::

      {end-group character}
      +stack: restoring \ifunknown=\iffalse.
      +stack: restoring \count68=0.
      +stack: restoring \iterate=macro:->\ifnum \n >0 ... \fi .
      +stack: restoring \count67=0.
      +stack: level - 2 for brace from line 27

Our number is prime! We print it.

.. container:: log_out

   ::

      +\ifprime34
      +iftest34 true
      \printp ->,\N =\n \divide \N by 8 ...
      Character sequence: ,.

We print a space or a newline character, depending on whether ``\n`` is
zero modulo 8. This piece of code is not in the original program. The
test could also have been: ``\ifnum\N=0 \char13 \else \char32 \fi``

.. container:: log_out

   ::

      {\count69}
      +scanint for \N->25
      {changing \count69=2 into \count69=25}
      {\divide}
      +scanint for \N->8
      {changing \count69=25 into \count69=3}
      {\multiply}
      +scanint for \N->-8
      {changing \count69=3 into \count69=-24}
      {\advance}
      +scanint for \N->25
      {changing \count69=-24 into \count69=1}
      +\ifnum35
      +scanint for \ifnum->1
      +scanint for \ifnum->0
      +iftest35 false
      +\else35
      {begin-group character}
      +stack: level + 2 for brace entered on line 27
      Character sequence:  .
      {end-group character}
      +stack: level - 2 for brace from line 27
      +\fi35

It is not the last number, so that only comma-space is printed.

.. container:: log_out

   ::

      +\ifnum36
      +scanint for \ifnum->25
      +scanint for \ifnum->1
      +iftest36 false
      +\fi36

The transcript shows a ``\fi34`` corresponding to the ``\ifprime`` above
and ``\fi27`` that corresponds to nothing. In fact, we skipped a big
part of the trace. Testing whether n is prime for n=5, 7, 9, 11, 13, and
15 uses the conditional 5, 12, 19, 24, 34, and 41: you can see that one
more test is needed for 13 than for other numbers; remember that if a
number is prime, a further test has to be done: is it the last one?

.. container:: log_out

   ::

      +scanint for \number->13
      Character sequence: 13.
      {\advance}
      +scanint for \n->-1
      {changing \count65=25 into \count65=24}
      +\fi34
      {\advance}
      +scanint for \p->2
      {changing \count66=13 into \count66=15}
      {\relax}
      {\expandafter \iterate \fi}
      +\fi27

This considers the number 15.

.. container:: log_out

   ::

      \iterate ->\ifnum \n >0 \printifprime \advance \p by2 \relax ...
      +\ifnum37
      +scanint for \ifnum->24
      +scanint for \ifnum->0
      +iftest37 true 
      \printifprime ->\testprimality \ifprime \printp \fi 
      \testprimality ->{\d =3 \global \primetrue \loop \trialdivision ...
      {begin-group character}
      +stack: level + 2 for brace entered on line 27
      +scanint for \d->3
      {changing \count67=0 into \count67=3}
      {\global}
      \primetrue ->\let \ifprime \iftrue 
      {\global\let}
      {\let \ifprime \iftrue}
      {globally changing \ifprime=\iftrue}
      {into \ifprime=\iftrue}

Dividing 15 by 3 is not enough to prove that 15 is a prime number.

.. container:: log_out

   ::

      {\loop}
      {changing \iterate=macro:->\ifnum ... \fi }
      {into \iterate=macro:->\trialdivision \ifunknown ... \fi }
      \iterate ->\trialdivision \ifunknown \advance \d by2 \relax ...
      \trialdivision ->\a =\p \divide \a by\d \global \advance \td by 1  ...
      {\count68}
      +scanint for \a->15
      {changing \count68=0 into \count68=15}
      {\divide}
      +scanint for \a->3
      {changing \count68=15 into \count68=5}
      {\global}
      {\global\advance}
      +scanint for \td->1
      {globally changing \count4=6 into \count4=7}
      +\ifnum38
      +scanint for \ifnum->5
      +scanint for \ifnum->3
      +iftest38 true
      \unknowntrue ->\let \ifunknown \iftrue 
      {\let}
      {\let \ifunknown \iftrue}
      {changing \ifunknown=\iffalse}
      {into \ifunknown=\iftrue}
      +\else38
      +\fi38

But dividing 15 by 3 *is* enough to prove that 15 is a not prime number.

.. container:: log_out

   ::

      {\multiply}
      +scanint for \a->3
      {changing \count68=5 into \count68=15}
      +\ifnum39
      +scanint for \ifnum->15
      +scanint for \ifnum->15
      +iftest39 true
      {\global}
      \primefalse ->\let \ifprime \iffalse 
      {\global\let}
      {\let \ifprime \iffalse}
      {globally changing \ifprime=\iftrue}
      {into \ifprime=\iffalse}
      \unknownfalse ->\let \ifunknown \iffalse 
      {\let}
      {\let \ifunknown \iffalse}
      {changing \ifunknown=\iftrue}
      {into \ifunknown=\iffalse}
      +\fi39

Skipping 6000 lines.

.. container:: log_out

   ::

      +iftest563 false
      +\fi563
      {\let}
      {\let \iterate \relax}
      {changing \iterate=macro:->\ifnum ... \fi }
      {into \iterate=\relax}
      Character sequence: . trial division macro was .
      [28] expanded \the\td\ times. % Knuth says cpu time=4 sec, I see 0.02
      Character sequence: expanded .
      {\the}
      {\the \td}
      \the->132.
      Character sequence: 132.
      {\ }
      Character sequence: times. .

Line 29 of the input file is a hack: the ``\a`` command is not defined
by plainTeX, so that Knuth can use, but it is defined in LaTeX, and we
have to restore it.

.. container:: log_out

   ::

      [29] \let\a\olda %% restore
      {\let}
      {\let \a \olda}
      {changing \a=\count68}
      {into \a=\a}

The last line of the file is empty, this is equivalent ``\par``, and has
as effect to finish the current element.

.. container:: log_out

   ::

      [30] 
      {\par}
      {Text:The first thirty prime numbers are 2, 3, 5, 7, 11, 13,
      17, 19, 23, 29, 31, 37, 41, 43,
      47, 53, 59, 61, 67, 71, 73, 79,
      83, 89, 97, 101, 103, 107, 109, and 113. trial division macro was
      expanded 132 times. }
      {Pop 1: document_v p_v}
      ++ Input stack empty at end of file

Note the last line of the file, the empty one, is not really needed.
Without it, the last line of the XML result would have been
``expanded 132 times. </p></unknown>``. In fact, when *Tralics* executes
a ``\par`` command, it removes the last space (that corresponds in
general to the end-of-line on the line before the empty line), and adds
a newline character in the XML tree after the ``<p>`` element. In this
example, there is a space before the comment line 28, the EOL characters
on lines 28 and 29 are commented out, and this one is visible if the
paragraph is ended without a ``\par`` command.
