=====================
Characters in Tralics
=====================

Table of contents
-----------------

| `Introduction <#introduction>`__
| `ASCII characters <#ascii>`__ (between U+0020 and U+007F)
| `Latin one <#latin1>`__ (between U+00A0 and U+00FF)
| `Latin extended-A <#ext-latin>`__ (between U+0100 and U+017F)
| `Latin extended-B <#not-done>`__ (between U+0180 and U+024F) [todo]
| `IPA Extensions <#not-done>`__ (between U+0250 and U+02AF) [todo]
| `Greek, etc <#not-done>`__ (between U+0370 and U+03FF) [todo]
| `Cyrillic, etc <#not-done>`__ (between U+0400 and U+04FF) [todo]
| `Latin Extended Additional <#latin-extended>`__ (between U+1E00 and
  U+1EFF)
| `Arrows <#arrows>`__ (between U+2190 and U+21FF)
| `Mathematical Operators <#mathop>`__ (between U+2200 and U+22FF)
| `Miscellaneous Technical <#misc-tech>`__ (between U+2300 and U+23FF)
  [with holes]
| `Miscellaneous Symbols <#misc-tech>`__ (between U+2600 and U+26FF)
  [with holes]
| `Dingbats <#ding>`__ (between U+2700 and U+27BF)
| `Mathematical Alphanumeric Symbols <#math-char>`__ (between U+1D400
  and U+1D5FF)
| `The T1 encoding <#T1-char>`__

Introduction
============

This file explains how to produce all characters defined by the Unicode
standard, and even more. In earlier versions; *Tralics* used internally
the iso-8859-1 encoding, and this is the default input encoding; you
could tell *Tralics* that the text is encoded in UTF-8 format, this
meant that some characters were converted into a sequence of the form
``\char567{}`` (this is read as a sequence of six tokens). The internal
encoding is now Unicode.

UTF-8 is a variable length format; each character consists in one to
four bytes, A, AB, ABC, or ABCD, depending of the first byte. If the
first bit of A is zero, so that A is an integer between 0 and 127, then
one byte is used, and the value is A. Thus, an ASCII character
represents itself in UTF8. Otherwise, A starts with 2, 3 or 4 bits equal
to 1, and is followed by a zero bit, followed by some useful bits (say
\`a'). The total number of bytes is 2, 3 or 4, respectively, each byte
B, C, D starts with a bit equal to 1 , and a bit equal to 0, and is
followed by 6 useful bits, say \`b', or \`c', or \`d'; the value of the
character is \`ab', \`abc' or \`abcd'. Thus, using 2, 3 or 4 bytes, we
can represent 11, 16, or 21 bits. When a line of text is read from a
file, it is converted to UTF8; at the end of the run, you may see
something like *Input conversion: 14 lines converted.* This means that
almost all lines contained only ASCII characters.

A construction like ``\char1000`` means: insert the character number
1000 in the XML tree. The character must fit on 27 bits. A construction
like ``^^ab`` (two lower case hex chars) can be used to represent a 8bit
character, and a construction like ``^^^^abcd`` (four lower case hex
chars) can be used to represent a 16bit character. You can also use five
hats as in ``^^^^^1d7e0`` (this is Unicode Mathematical Double-struck
Digit Eight). An hex char is a digit or a lower case letter between \`a'
and \`f'. You can say ``^^Z``, where \`Z' can be replaced by any 7bit
character, except a hat or a hex char; if this case *Tralics* adds or
subtracts 64, the result is a 7bit character. For instance ``^^Z`` is
the character with code 26, ``^^@`` is the null character, ``^^?`` the
delete character, ``^^M`` is the end of line character (in the input
stream), ``^^J`` is the end of line character (in the output stream).

An important property of a character is its category code. In plain
*TeX* some characters have category code “ignore” or “invalid”, this
explains why the constructions ``^^?`` and ``^^@`` are so useful. When
*Tralics* parses a control sequence like ``\foo``, then either the
character that follows the backslash has category “letter” and all
letters that follow are scanned and a following space is usually
ignored, or the command name has only one character. In the case of
``\fée``, the result is a 1-character command (it tries to put an accent
on the character that follows). You can say
:literal:`\\catcode`é=11\def\fée{fairy}`, and then use the command
``\fée``. All tables that hold character properties are of size 65536;
this excludes characters that need 4 UTF8 bytes, but includes all
characters that can be constructed via the double-hat or four-hat
construction. In particular :literal:`\\catcode`^^^^^1d7e0=11` is an
error. What the command really gets is the following sequence of tokens:
``\char"1D7E0``, a command followed by 6 character tokens category 12.
If you say ``\def\f^^^^^1d7e0{xx}\def\^^^^^1d7e0{xx}``, the hat
conversion algorithm is applied; in the first definition, you get a
command named ``\f``, that has to be followed by some character (note
that ``\f\char"1D7E0`` is an error, since the category code of the
letteres are 11, but 12 is expected). The second definition confuses the
parser: what follows the backslash has no category code, so the name of
the command is empty, and this empty command has to be followed the
character; so ``\^^^^^1d7e1``, provokes the following error *Use of
\\csname\endcsname doesn't match its definition*.

You can use one of -oe8, -oe1, -oe8a, -oe1a as option to *Tralics* for
controlling the encoding used in the XML file. If the option is -oe8 or
-oe8a, then the declaration is ``encoding='UTF-8'``, otherwise it is
``encoding='iso-8859-1'``. Assume that the input is
``\char16\char65\char233\char378\char1064\char10774\char120587``. The
translation is ``AéźШ⨖𝜋`` (the first character being illegal in HTLK, it
is not shown here). If the option is -oe8 (the default), the first and
last characters are encoded as ``&#x10;`` and ``&#x1D70B;``, the others
are in UTF8; the transcript file will contain the same string. If the
option is -oe8a, all characters but ASCII ones will be output in the
form ``&#xE9;``, the transcript file will contain
``&#x10;A^^e9^^^^017a^^^^0428^^^^2a16&#x1D70B;``. If the option is
-oe1a, the contents will be the same, except for the encoding of the
file. Finally, if the option is -oe1, the output is the same, except for
characters whose code is between 128 and 255, that will be printed on
one byte.

You can use one of -te8, -te1, -te8a, -te1a as option for controlling
the encoding used in the transcript file, without the XML file. This
encoding also used when *Tralics* prints something on the terminal. For
instance, this is the output of ``\show``, with the option te1a:
``\f^^e9e=macro: ->fairy.``

You can use one of -utf8, -latin1 for indicating the default input encoding. In
LaTeX, you can use the inputenc package for specifying alternate encodings, this
is not yet implemented. All files are converted into UTF-8 while they are read;
there is an exception: the main file is converted only after the configuration
file has been read (this means that the configuration file could contain the
definition of the encoding). You can declare the encoding of the current file,
by putting near the head of the file a line that contains ``-*-`` followed by
coding: iso-8859-1 or coding: utf8. This works whatever the type of the file:
configuration file, bibliography data base, or TeX file.

A given character can appear in one of three modes: typesetting text,
typesetting maths, and other cases (in general, when creating the name
of an element, of an attribute, the name of a file to be opened, etc).
In the case of ``\input{xx}``, the characters that are valid are system
dependent, and *Tralics* accepts only latin1 characters, without further
tests. In the same fashion, ``\xbox{á b}{\AddAttToCurrent{x=}{y}z}``
generates an XML element that contains a space in its name, and an
attribute name that contains an equals sign. This is illegal, but not
refused. If you say ``\xbox{x\xmllatex{\&\#x5678;}{}y}{}`` the result is
an empty XML with a funny name: ``<x&#x5678;y/>``.

In math mode, each character is classified as an identifier, a number,
or an operator (such an element can consist of more than one character).
Hence the translation of ``$x=1$`` is:
``<mi>x</mi><mo>=</mo><mn>1</mn>``. There is a built-in table that
explains the type of every ASCII character. Non-ASCII latin1 characters
are all considered as operators if their code is between 128 and 191, as
identifiers otherwise. Thus, the translation of ``$±á$`` is
``<mo>±</mo><mi>á</mi>``. There is a large number of commands whose
translation is a Unicode character. LaTeX makes a distinction between
those that can be used in math, or outside math; this restriction has
sometimes been removed. For instance ``\textohm`` is the same as
``^^^^2126``, and the following expression
``\xbox{\textohm b}{\textohm$\textohm$}`` is valid. The math formula
contains an identifier.

If you say ``\mathchardef\foo1234``, then TeX interprets ``\foo`` as
``\mathchar1234``. See the TeX book for what character this produces.
For simplicity, *Tralics* interprets this as ``\char1234``; this is the
character number 1234 (Unicode U+4D2, some Cyrillic character. Any 27
bit character can be used in this way. A command like ``\Omega`` can be
used only in math mode. The translation is an identifier (but most math
symbols are operators), the value is ``&Omega;``. If the option
``-noentnames`` has been given, then translation is ``&#x3A9;``.
Translation of ``\mathbb A`` is ``&Aopf;`` or ``&#x1D358;``. If you
specify the ``mathvariant`` option, then translation changes to
``<mi mathvariant='double-struck'>A</mi>``.

In text mode, there are different ways to obtain a character, and they
are all valid. For instance, if you want a acute, you can say ``á``
(latin1 or UTF8 input) or :literal:`\\char`\á  \char"E1` (two variants
of the ``\char`` command) or ``\'a  \a'a`` (two variants of the accent
command) or ``\xmllatex{\&\#xE1;}{}  \xmllatex{\&aacute;}{}`` (two
variants of the ``\xmllatex`` command that refers to a Unicode character
via an entity).

ASCII characters
----------------

Printing characters with code between 33 and 126 is not complicated.
Character 32 is space, it is impossible to tell the difference between a
cell that contains only space and an empty space. Character 127 is
delete, it cannot be printed. For some strange reason, LaTeX provides no
way for printing a hat or a tilde. This is why ``\char`` is used in the
table below.

.. container:: ltx-source

   ::

      \begin{tabular}{cccccccccccccccc}
      &!&"&\#&\$&\%&\&&'&(&)&*&+&,&-&.&/\\
      0&1&2&3&4&5&6&7&8&9&:&;&<&=&>&?\\
      @&A&B&C&D&E&F&G&H&I&J&K&L&M&N&O\\
      P&Q&R&S&T&U&V&W&X&Y&Z&[&\textbackslash&]&\char`\^&\_\\
      `&a&b&c&d&e&f&g&h&i&j&k&l&m&n&o\\
      p&q&r&s&t&u&v&w&x&y&z&\{&|&\}&\char`\~&
      \end{tabular}

The translation is trivial. We only show the preview (LaTeX image) and
the HTML characters. |Ascii Table|

== == = = = = = = = = == == == = = ==
\   ! " # $ % & ' ( ) \* +  ,  - . /
0  1  2 3 4 5 6 7 8 9  :  ; <  = >  ?
@  A  B C D E F G H I J  K  L  M N O
P  Q  R S T U V W X Y Z  [  \\ ] ^ \_
\` a  b c d e f g h i j  k  l  m n o
p  q  r s t u v w x y z  {  \| } ~ 
== == = = = = = = = = == == == = = ==

Latin1 characters
-----------------

These are all the characters with code between x80 and xFF. In fact,
characters in the range x80 and x9F are not printable. Character A0 is a
space, character AD is a discretionary hyphen.

.. container:: ltx-source

   ::

      \begin{tabular}{cccccccccccccccc}
      ~&¡&¢&£&¤&¥&¦&§&¨&©&ª&«&¬&&®&¯\\
      °&±&²&³&´&µ&¶&·&¸&¹&º&»&¼&½&¾&¿\\
      À&Á&Â&Ã&Ä&Å&Æ&Ç&È&É&Ê&Ë&Ì&Í&Î&Ï\\
      Ð&Ñ&Ò&Ó&Ô&Õ&Ö&×&Ø&Ù&Ú&Û&Ü&Ý&Þ&ß\\
      à&á&â&ã&ä&å&æ&ç&è&é&ê&ë&ì&í&î&ï\\
      ð&ñ&ò&ó&ô&õ&ö&÷&ø&ù&ú&û&ü&ý&þ&ÿ\\
      \end{tabular}

|latin 1, non ascii|

= = = = = = = = = = = == = = = =
  ¡ ¢ £ ¤ ¥ ¦ § ¨ © ª «  ¬ ­ ® ¯
° ± ² ³ ´ µ ¶ · ¸ ¹ º  » ¼ ½ ¾ ¿
À Á Â Ã Ä Å Æ Ç È É Ê Ë  Ì Í Î Ï
Ð Ñ Ò Ó Ô Õ Ö × Ø Ù Ú Û  Ü Ý Þ ß
à á â ã ä å æ ç è é ê ë  ì í î ï
ð ñ ò ó ô õ ö ÷ ø ù ú û  ü ý þ ÿ
= = = = = = = = = = = == = = = =

Seven bit input for the same characters.

.. container:: ltx-source

   ::

       
      \begin{tabular}{cccccccccccccccc}
      \nobreakspace&\textexclamdown&\textcent&\textsterling&\textcurrency&\textyen&
         \textbrokenbar&\textsection&\textasciidieresis&\textcopyright&
         \textordfeminine&\guillemotleft&\textlnot&\textsofthyphen&
         \textregistered&\textasciimacron\\
      \textdegree&\textpm&\texttwosuperior&\textthreesuperior&\textasciiacute&
        \textmu&\textparagraph&\textperiodcentered&\textasciicedilla&
        \textonesuperior&\textordmasculine&\guillemotright&\textonequarter&
        \textonehalf&\textthreequarters&\textquestiondown\\
      \`A&\'A&\^A&\~A&\quot;A&\AA&\AE&\c C&\`E&\'E&\^E&\"E&\`I&\'I&\^I&\"I\\
      \DH&\~N&\`O&\'O&\^O&\~O&\"O&\texttimes&\O&\`U&\'U&\^U&\"U&\'Y&\TH&\ss\\
      \`a&\'a&\^a&\~a&\"a&\aa&\ae&\c c&\`e&\'e&\^e&\"e&\`i&\'i&\^i&\"i\\
      \dh&\~n&\`o&\'o&\^o&\~o&\"o&\textdiv&\o&\`u&\'u&\^u&\"u&\'y&\th&\"y\\
      \end{tabular}

Notes: ``\textsection=\S``, ``\textparagraph=\P``,
``\textcopyright =\copyright``. Concerning French guillemets, you can
use ``\og``, that produces a guillemet and a space, or
``\guillemotleft`` that produces only the guillemet, or ``«`` or (if the
current language is French) ``<<`` that produce intelligent guillemets:
a space is removed if there is one, then another one is added. Example

.. container:: ltx-source

   ::

      {\language=1 a\fg{}b\og{}c\guillemotleft{}d\guillemotright{}e}
      {\language=0 a\fg{}b\og{}c\guillemotleft{}d\guillemotright{}e}
      {\language=1 a<<b<< c<<~d<<\,e\,>>f~>>g >>h>>i}
      {\language=0 a<<b<< c<<~d<<\,e\,>>f~>>g >>h>>i}
      {\language=1 a«b« c«~d«\,e\,»f~»g »h»i}
      {\language=0 a«b« c«~d«\,e\,»f~»g »h»i}

Translation (we replaced character U+A0 by an entity, in order to
distinguish it from a space).

.. container:: xml_out

   ::

      a&#xA0;»b«&#xA0;c«&#xA0;d&#xA0;»e
      a&#xA0;»b«&#xA0;c«d»e
      a«&#xA0;b«&#xA0;c«&#xA0;&#xA0;d«&#xA0;e&#xA0;&#xA0;»f&#xA0;&#xA0;»g&#xA0;»h&#xA0;»i
      a&lt;&lt;b&lt;&lt; c&lt;&lt;&#xA0;d&lt;&lt;&#xA0;e&#xA0;&gt;&gt;f&#xA0;&gt;&gt;g &gt;&gt;h&gt;&gt;i
      a«&#xA0;b«&#xA0;c«&#xA0;&#xA0;d«&#xA0;e&#xA0;&#xA0;»f&#xA0;&#xA0;»g&#xA0;»h&#xA0;»i
      a«b« c«&#xA0;d«&#xA0;e&#xA0;»f&#xA0;»g »h»i

Latin1 extended characters
--------------------------

These are all the characters with code between U+100 and U+17F.

.. container:: ltx-source

   ::

      \def\nument#1{\xmllatex{\&\#x#1;}{}}
      \begin{tabular}{cccccccccccccccc}
      \=A&\=a& \u A&\u a&\k A&\k a&\'C&\'c&\^C&\^c&\.C&\.c&\v C&\v c&\v D&\v d \\
      \DJ&\dj&\=E&\=e&\u E&\u e&\.E&\.e&\k E&\k e&\v E&\v e&\^G&\^g&\u G&\u g\\
      \.G&\.g&\c G&\c g&\^H&\^h&\=H&\=h&\~I&\~i&\=I&\=i&\u I&\u i&\k I&\k i\\
      \.I&\i&\IJ&\ij&\^J&\^j&\c K&\c k&\nument{138}&\'L&\'l&\c L&\c l&\v L&\v l&\.L\\
      \.l&\L&\l&\'N&\'n&\c N&\c n&\v N&\v n&\nument{149}&\NG&\ng&\=O&\=o&\u O&\u o\\
      \H O&\H o&\OE&\oe&\'R&\'r&\c R&\c r&\v R&\v r&\'S&\'s&\^S&\^s&\c S&\c s&\\
      \v S&\v s&\c T&\c t&\v T&\v t&\=T&\=t&\~U&\~u&\=U&\=u&\u U&\u u&\r U&\r u\\
      \H U&\H u&\k U&\k u&\^W&\^w&\^Y&\^y&\"Y&\'Z&\'z&\.Z&\.z&\v Z&\v z&\nument{17F}\\
      \end{tabular}

Characters U+138, U+149 and U+17F have no name in *Tralics*. The first
one is: \`latin small letter kra', the second is \`latin small letter n
preceded by apostrophe', and the last is \`latin small letter long s'.

This is the XML translation, without the \`tabular'.

.. container:: xml_out

   ::

      &#x100; &#x101; &#x102; &#x103; &#x104; &#x105; &#x106; &#x107;
      &#x108; &#x109; &#x10A; &#x10B; &#x10C; &#x10D; &#x10E; &#x10F; 
      &#x110; &#x111; &#x112; &#x113; &#x114; &#x115; &#x116; &#x117; 
      &#x118; &#x119; &#x11A; &#x11B; &#x11C; &#x11D; &#x11E; &#x11F; 
      &#x120; &#x121; &#x122; &#x123; &#x124; &#x125; &#x126; &#x127;
      &#x128; &#x129; &#x12A; &#x12B; &#x12C; &#x12D; &#x12E; &#x12F;
      &#x130; &#x131; &#x132; &#x133; &#x134; &#x135; &#x136; &#x137;
      &#x138; &#x139; &#x13A; &#x13B; &#x13C; &#x13D; &#x13E; &#x13F;
      &#x140; &#x141; &#x142; &#x143; &#x144; &#x145; &#x146; &#x147; 
      &#x148; &#x149; &#x14A; &#x14B; &#x14C; &#x14D; &#x14E; &#x14F;
      &#x150; &#x151; &#x152; &#x153; &#x154; &#x155; &#x156; &#x157;
      &#x158; &#x159; &#x15A; &#x15B; &#x15C; &#x15D; &#x15E; &#x15F; 
      &#x160; &#x161; &#x162; &#x163; &#x164; &#x165; &#x166; &#x167;
      &#x168; &#x169; &#x16A; &#x16B; &#x16C; &#x16D; &#x16E; &#x16F;
      &#x170; &#x171; &#x172; &#x173; &#x174; &#x175; &#x176; &#x177;
      &#x178; &#x179; &#x17A; &#x17B; &#x17C; &#x17D; &#x17E; &#x17F;

As you can see, translation is trivial. In the initial version of the
software, there was a possibility to obtain entity names.

= = = = = = = = = = = = = = = =
Ā ā Ă ă Ą ą Ć ć Ĉ ĉ Ċ ċ Č č Ď ď 
Đ đ Ē ē Ĕ ĕ Ė ė Ę ę Ě ě Ĝ ĝ Ğ ğ 
Ġ ġ Ģ ģ Ĥ ĥ Ħ ħ Ĩ ĩ Ī ī Ĭ ĭ Į į 
İ ı Ĳ ĳ Ĵ ĵ Ķ ķ ĸ Ĺ ĺ Ļ ļ Ľ ľ Ŀ 
ŀ Ł ł Ń ń Ņ ņ Ň ň ŉ Ŋ ŋ Ō ō Ŏ ŏ 
Ő ő Œ œ Ŕ ŕ Ŗ ŗ Ř ř Ś ś Ŝ ŝ Ş ş 
Š š Ţ ţ Ť ť Ŧ ŧ Ũ ũ Ū ū Ŭ ŭ Ů ů 
Ű ű Ų ų Ŵ ŵ Ŷ ŷ Ÿ Ź ź Ż ż Ž ž ſ 
= = = = = = = = = = = = = = = =

|latin extended A|

Latin extended additional characters
------------------------------------

We explain here how to obtain all characters between U+1E00 and U+1EFF.
Character U+1e9a (latin small letter a with right half ring) is not
implemented. There are two holes in the tables: of 4 and 6 characters
respectively.

.. container:: ltx-source

   ::

      \def\xx#1{\xmllatex{\&\#x#1;}{}}
      \catcode`\&13\def&{ }
      \D A &\D a&\.B&\.b &\d B&\d b&\b B&\b b& \'{\c C} &\'{\c c}&
      \.D &\.d &\d D& \d d &\b D &\b d\\
      \c D &\c d& \V D& \V d &\`{\=E}& \`{\=e} &\'{\=E}&\'{\=e}&
      \V E &\V e &\T E& \T e& \u{\c E}& \u{\c e}& \.F &\.f\\
      \=G &\=g& \.H &\.h& \d H&\d h&\"H &\" h&\c H&\c h&\xx{1e2a}&\xx{1e2b}
      &\T I&\T i&\'{\"I}&\'{\"i}\\
      \'K&\'k&\d K&\d k& \b K&\b k&\d L&\d l&\={\d L}&\={\d l}&\b L&\b l&\V L
      &\V l&\'M&\'m \\
      \.M&\.m&\d M&\d m&\.N&\.n&\d N&\d n&\b N&\b n&\V N&\V n&\~{\'O}&\~{\'o}
      &\~{\"O}&\~{\"o}\\
      \`{\=O}&\`{\=o}&\'{\=O}&\'{\=o}&\'P&\'p&\.P&\.p&\. R&\. r&\d R&\d r&
      \={\d R}&\={\d r}&\b R&\b r\\
      \.S&\.s&\d S&\d s&\.{\'S}&\.{\'s}&\v{\.S}&\v{\.s}&\d{\.S}&\d{\.s}
      &\.T&\. t&\d T&\d t&\b T&\b t\\
      \V T&\V t&\xx{1e72}&\xx{1e73}&\T U&\T u&\V U&\V u&\~{\'U}&\~{\'u}&
      \={\"U}&\={\"u}&\~V&\~v&\d V&\d v\\

Translation

.. container:: xml_out

   ::

      <p>&#7680; &#7681; &#7682; &#7683; &#7684; &#7685; &#7686; &#7687;  
         &#7688; &#7689; &#7690; &#7691; &#7692; &#7693; &#7694; &#7695;</p><p noindent='true'>
         &#7696; &#7697; &#7698; &#7699; &#7700; &#7701; &#7702; &#7703; 
         &#7704; &#7705; &#7706; &#7707; &#7708; &#7709; &#7710; &#7711;</p><p noindent='true'>
         &#7712; &#7713; &#7714; &#7715; &#7716; &#7717; &#7718; &#7719;
         &#7720; &#7721; &#x1e2a;&#x1e2b;&#7724; &#7725; &#7726; &#7727;</p><p noindent='true'>
         &#7728; &#7729; &#7730; &#7731; &#7732; &#7733; &#7734; &#7735;
         &#7736; &#7737; &#7738; &#7739; &#7740; &#7741; &#7742; &#7743;</p><p noindent='true'>
         &#7744; &#7745; &#7746; &#7747; &#7748; &#7749; &#7750; &#7751; 
         &#7752; &#7753; &#7754; &#7755; &#7756; &#7757; &#7758; &#7759;</p><p noindent='true'>
         &#7760; &#7761; &#7762; &#7763; &#7764; &#7765; &#7766; &#7767; 
         &#7768; &#7769; &#7770; &#7771; &#7772; &#7773; &#7774; &#7775;</p><p noindent='true'>
         &#7776; &#7777; &#7778; &#7779; &#7780; &#7781; &#7782; &#7783; 
         &#7784; &#7785; &#7786; &#7787; &#7788; &#7789; &#7790; &#7791;</p><p noindent='true'>
         &#7792; &#7793; &#x1e72;&#x1e73;&#7796; &#7797; &#7798; &#7799; 
         &#7800; &#7801; &#7802; &#7803; &#7804; &#7805; &#7806; &#7807;</p>

|latin extended additional|

= = = = = = = = = = = = = = = =
Ḁ ḁ Ḃ ḃ Ḅ ḅ Ḇ ḇ Ḉ ḉ Ḋ ḋ Ḍ ḍ Ḏ ḏ
Ḑ ḑ Ḓ ḓ Ḕ ḕ Ḗ ḗ Ḙ ḙ Ḛ ḛ Ḝ ḝ Ḟ ḟ
Ḡ ḡ Ḣ ḣ Ḥ ḥ Ḧ ḧ Ḩ ḩ Ḫ ḫ Ḭ ḭ Ḯ ḯ
Ḱ ḱ Ḳ ḳ Ḵ ḵ Ḷ ḷ Ḹ ḹ Ḻ ḻ Ḽ ḽ Ḿ ḿ
Ṁ ṁ Ṃ ṃ Ṅ ṅ Ṇ ṇ Ṉ ṉ Ṋ ṋ Ṍ ṍ Ṏ ṏ
Ṑ ṑ Ṓ ṓ Ṕ ṕ Ṗ ṗ Ṙ ṙ Ṛ ṛ Ṝ ṝ Ṟ ṟ
Ṡ ṡ Ṣ ṣ Ṥ ṥ Ṧ ṧ Ṩ ṩ Ṫ ṫ Ṭ ṭ Ṯ ṯ
Ṱ ṱ Ṳ ṳ Ṵ ṵ Ṷ ṷ Ṹ ṹ Ṻ ṻ Ṽ ṽ Ṿ ṿ
= = = = = = = = = = = = = = = =

.. container:: ltx-source

   ::

      \catcode`\&13\def&{ }
      \`W&\`w&\'W&\'w&\"W&\"w&\.W&\.w&\d W&\d w&\. X&\. x&\"X&\"x&\. Y&\.y\\
      \^Z&\^z&\d Z&\d z&\b Z&\b z&\b h&\" t&\r
      w&\r y&?&\xx{1e9b}\\ 
      \d A&\d a&\h A&\h a&\^{\'A}&\^{\'a}&\^{\`A}&\^{\`a}&{\^{\h A}}&\^{\h a}&
      \~{\^A}&\~{\^a}&\d{\^A}&\d{\^a}&\'{\u A}&\'{\u a}\\
      \`{\u A}&\`{\u a}&\h{\u A}&\h{\u a}&\~{\u A}&\~{\u a}&\d{\u A}&\d{\u a}&\d E&
      \d e&\h E&\h e&\~ E&\~e&\'{\^E}&\'{\^e}\\
      \^{\`E}&\^{\`e}&\^{\h E}&\^{\h e}&\~{\^E}&\~{\^e}&\^{\d E}&\^{\d e}&\h I&
      \h i&\d I&\d i& \d O &\d o&\h O&\h o\\
      \'{\^O}&\'{\^o}&\`{\^O}&\`{\^o}&\^{\h O}&\^{\h o}&\~{\^O}&\~{\^o}&
      \^{\d O}&\^{\d o}&\'{\H O}&\'{\H o}&\`{\H O}&\`{\H o}&\h{\H O}&\h{\H o}\\
      \~{\H O}&\~{\H o}&\d{\H O}&\d{\H o}&\d U&\d u&\h U&\h u&\'{\H U}&\'{\H u}&
      \`{\H U}&\`{\H u}&\h{\H U}&\h{\H u}&\~{\H U}&\~{\H u}\\
      \H{\d U}&\H{\d u}&\` Y&\` y&\d Y&\d y&\h Y&\h y&\~ Y&\~y

.. container:: xml_out

   ::

      <p>
       &#7808; &#7809; &#7810; &#7811; &#7812; &#7813; &#7814; &#7815; 
       &#7816; &#7817; &#7818; &#7819; &#7820; &#7821; &#7822; &#7823;</p><p noindent='true'>
       &#7824; &#7825; &#7826; &#7827; &#7828; &#7829; &#7830; &#7831; 
       &#7832; &#7833; ? &#x1e9b;</p><p noindent='true'>
       &#7840; &#7841; &#7842; &#7843; &#7844; &#7845; &#7846; &#7847; 
       &#7848; &#7849; &#7850; &#7851; &#7852; &#7853; &#7854; &#7855;</p><p noindent='true'>
       &#7856; &#7857; &#7858; &#7859; &#7860; &#7861; &#7862; &#7863; 
       &#7864; &#7865; &#7866; &#7867; &#7868; &#7869; &#7870; &#7871;</p><p noindent='true'>
       &#7872; &#7873; &#7874; &#7875; &#7876; &#7877; &#7878; &#7879; 
       &#7880; &#7881; &#7882; &#7883; &#7884; &#7885; &#7886; &#7887;</p><p noindent='true'>
       &#7888; &#7889; &#7890; &#7891; &#7892; &#7893; &#7894; &#7895; 
       &#7896; &#7897; &#7898; &#7899; &#7900; &#7901; &#7902; &#7903;</p><p noindent='true'>
       &#7904; &#7905; &#7906; &#7907; &#7908; &#7909; &#7910; &#7911; 
       &#7912; &#7913; &#7914; &#7915; &#7916; &#7917; &#7918; &#7919;</p><p noindent='true'>
       &#7920; &#7921; &#7922; &#7923; &#7924; &#7925; &#7926; &#7927; 
       &#7928; &#7929;</p>

|latin extended additional 2|

= = = = = = = = = = == = = = = =
Ẁ ẁ Ẃ ẃ Ẅ ẅ Ẇ ẇ Ẉ ẉ Ẋ  ẋ Ẍ ẍ Ẏ ẏ
Ẑ ẑ Ẓ ẓ Ẕ ẕ ẖ ẗ ẘ ẙ  ? ẛ       
Ạ ạ Ả ả Ấ ấ Ầ ầ Ẩ ẩ Ẫ  ẫ Ậ ậ Ắ ắ
Ằ ằ Ẳ ẳ Ẵ ẵ Ặ ặ Ẹ ẹ Ẻ  ẻ Ẽ ẽ Ế ế
Ề ề Ể ể Ễ ễ Ệ ệ Ỉ ỉ Ị  ị Ọ ọ Ỏ ỏ
Ố ố Ồ ồ Ổ ổ Ỗ ỗ Ộ ộ Ớ  ớ Ờ ờ Ở ở
Ỡ ỡ Ợ ợ Ụ ụ Ủ ủ Ứ ứ Ừ  ừ Ử ử Ữ ữ
Ự ự Ỳ ỳ Ỵ ỵ Ỷ ỷ Ỹ ỹ            
= = = = = = = = = = == = = = = =

Dingbats characters
-------------------

You can use the ``\ding`` command in order to access one character of
the Zapf Dingbats fonts. The command takes one argument, an integer (it
is parsed by the ``scanint`` routine, see
`scanint <doc-s.html#fct-scanint>`__ for details, so that
:literal:`\\ding{`\A}` is as valid as :literal:`\\char`\A`). If the
number is between 33 and 126, or between 161 and 239, or between 241 and
254, then you get one of the characters listed below. Otherwise, the
translation is ``bad!``

Example that shows the complete list.

.. container:: ltx-source

   ::

       
      {\LARGE
      \begin{tabular}{cccccccccccccccc}
      %\catcode`\&13\def&{ }
      &\ding{"21}&\ding{"22}&\ding{"23}&\ding{"24}&\ding{"25}&\ding{"26}&\ding{"27}& 
      \ding{"28}&\ding{"29}&\ding{"2A}&\ding{"2B}&\ding{"2C}&\ding{"2D}&\ding{"2E}&\ding{"2F}\\
      \ding{"30}&\ding{"31}&\ding{"32}&\ding{"33}&\ding{"34}&\ding{"35}&\ding{"36}&\ding{"37}&
      \ding{"38}&\ding{"39}&\ding{"3A}&\ding{"3B}&\ding{"3C}&\ding{"3D}&\ding{"3E}&\ding{"3F}\\
      \ding{"40}&\ding{"41}&\ding{"42}&\ding{"43}&\ding{"44}&\ding{"45}&\ding{"46}&\ding{"47}&
      \ding{"48}&\ding{"49}&\ding{"4A}&\ding{"4B}&\ding{"4C}&\ding{"4D}&\ding{"4E}&\ding{"4F}\\
      \ding{"50}&\ding{"51}&\ding{"52}&\ding{"53}&\ding{"54}&\ding{"55}&\ding{"56}&\ding{"57}&
      \ding{"58}&\ding{"59}&\ding{"5A}&\ding{"5B}&\ding{"5C}&\ding{"5D}&\ding{"5E}&\ding{"5F}\\
      \ding{"60}&\ding{"61}&\ding{"62}&\ding{"63}&\ding{"64}&\ding{"65}&\ding{"66}&\ding{"67}&
      \ding{"68}&\ding{"69}&\ding{"6A}&\ding{"6B}&\ding{"6C}&\ding{"6D}&\ding{"6E}&\ding{"6F}\\
      \ding{"70}&\ding{"71}&\ding{"72}&\ding{"73}&\ding{"74}&\ding{"75}&\ding{"76}&\ding{"77}&
      \ding{"78}&\ding{"79}&\ding{"7A}&\ding{"7B}&\ding{"7C}&\ding{"7D}&\ding{"7E}&\\
      &\ding{"A1}&\ding{"A2}&\ding{"A3}&\ding{"A4}&\ding{"A5}&\ding{"A6}&\ding{"A7}&
      \ding{"A8}&\ding{"A9}&\ding{"AA}&\ding{"AB}&\ding{"AC}&\ding{"AD}&\ding{"AE}&\ding{"AF}\\
      \ding{"B0}&\ding{"B1}&\ding{"B2}&\ding{"B3}&\ding{"B4}&\ding{"B5}&\ding{"B6}&\ding{"B7}&
      \ding{"B8}&\ding{"B9}&\ding{"BA}&\ding{"BB}&\ding{"BC}&\ding{"BD}&\ding{"BE}&\ding{"BF}\\
      \ding{"C0}&\ding{"C1}&\ding{"C2}&\ding{"C3}&\ding{"C4}&\ding{"C5}&\ding{"C6}&\ding{"C7}&
      \ding{"C8}&\ding{"C9}&\ding{"CA}&\ding{"CB}&\ding{"CC}&\ding{"CD}&\ding{"CE}&\ding{"CF}\\
      \ding{"D0}&\ding{"D1}&\ding{"D2}&\ding{"D3}&\ding{"D4}&\ding{"D5}&\ding{"D6}&\ding{"D7}&
      \ding{"D8}&\ding{"D9}&\ding{"DA}&\ding{"DB}&\ding{"DC}&\ding{"DD}&\ding{"DE}&\ding{"DF}\\
      \ding{"E0}&\ding{"E1}&\ding{"E2}&\ding{"E3}&\ding{"E4}&\ding{"E5}&\ding{"E6}&\ding{"E7}&
      \ding{"E8}&\ding{"E9}&\ding{"EA}&\ding{"EB}&\ding{"EC}&\ding{"ED}&\ding{"EE}&\ding{"EF}\\
      &\ding{"F1}&\ding{"F2}&\ding{"F3}&\ding{"F4}&\ding{"F5}&\ding{"F6}&\ding{"F7}&
      \ding{"F8}&\ding{"F9}&\ding{"FA}&\ding{"FB}&\ding{"FC}&\ding{"FD}&\ding{"FE}&\\
      \end{tabular}
      }

Translation, without the table and the font change. As you can see, the
codes are between U+2700 and U+27BF, with some exceptions. For instance
character 169 gives U+2666. This is black diamond suit. But the file
mmlalias.ent defines ``&diamondsuit;`` to be the same character, and
this is used to translate ``$\diamondsuit$``.

.. container:: xml_out

   ::

      <p>&#x2701; &#x2702; &#x2703; &#x2704; &#x260E; &#x2706; &#x2707; 
      &#x2708; &#x2709; &#x261B; &#x261E; &#x270C; &#x270D; &#x270E; &#x270F;</p>
      <p noindent='true'>&#x2710; &#x2711; &#x2712; &#x2713; &#x2714; &#x2715; &#x2716; &#x2717; 
      &#x2718; &#x2719; &#x271A; &#x271B; &#x271C; &#x271D; &#x271E; &#x271F;</p>
      <p noindent='true'>&#x2720; &#x2721; &#x2722; &#x2723; &#x2724; &#x2725; &#x2726; &#x2727; 
      &#x2605; &#x2729; &#x272A; &#x272B; &#x272C; &#x272D; &#x272E; &#x272F;</p>
      <p noindent='true'>&#x2730; &#x2731; &#x2732; &#x2733; &#x2734; &#x2735; &#x2736; &#x2737; 
      &#x2738; &#x2739; &#x273A; &#x273B; &#x273C; &#x273D; &#x273E; &#x273F;</p>
      <p noindent='true'>&#x2740; &#x2741; &#x2742; &#x2743; &#x2744; &#x2745; &#x2746; &#x2747; 
      &#x2748; &#x2749; &#x274A; &#x274B; &#x25CF; &#x274D; &#x25A0; &#x274F;</p>
      <p noindent='true'>&#x2750; &#x2751; &#x2752; &#x25B2; &#x25BC; &#x25C6; &#x2756; &#x25D7; 
      &#x2758; &#x2759; &#x275A; &#x275B; &#x275C; &#x275D; &#x275E; </p>
      <p noindent='true'> &#x2761; &#x2762; &#x2763; &#x2764; &#x2765; &#x2766; &#x2767; 
      &#x2663; &#x2666; &#x2665; &#x2660; &#x2460; &#x2461; &#x2462; &#x2463;</p>
      <p noindent='true'>&#x2464; &#x2465; &#x2466; &#x2467; &#x2468; &#x2469; &#x2776; &#x2777; 
      &#x2778; &#x2779; &#x277A; &#x277B; &#x277C; &#x277D; &#x277E; &#x277F;</p>
      <p noindent='true'>&#x2780; &#x2781; &#x2782; &#x2783; &#x2784; &#x2785; &#x2786; &#x2787; 
      &#x2788; &#x2789; &#x278A; &#x278B; &#x278C; &#x278D; &#x278E; &#x278F;</p>
      <p noindent='true'>&#x2790; &#x2791; &#x2792; &#x2793; &#x2794; &#x2192; &#x2194; &#x2195; 
      &#x2798; &#x2799; &#x279A; &#x279B; &#x279C; &#x279D; &#x279E; &#x279F;</p>
      <p noindent='true'>&#x27A0; &#x27A1; &#x27A2; &#x27A3; &#x27A4; &#x27A5; &#x27A6; &#x27A7; 
      &#x27A8; &#x27A9; &#x27AA; &#x27AB; &#x27AC; &#x27AD; &#x27AE; &#x27AF;</p>
      <p noindent='true'> &#x27B1; &#x27B2; &#x27B3; &#x27B4; &#x27B5; &#x27B6; &#x27B7; 
      &#x27B8; &#x27B9; &#x27BA; &#x27BB; &#x27BC; &#x27BD; &#x27BE; </p>

This is the table

|Zapf Dingbats characters|

== = = = = = = = = = = = = = = =
\  ✁ ✂ ✃ ✄ ☎ ✆ ✇ ✈ ✉ ☛ ☞ ✌ ✍ ✎ ✏
✐  ✑ ✒ ✓ ✔ ✕ ✖ ✗ ✘ ✙ ✚ ✛ ✜ ✝ ✞ ✟
✠  ✡ ✢ ✣ ✤ ✥ ✦ ✧ ★ ✩ ✪ ✫ ✬ ✭ ✮ ✯
✰  ✱ ✲ ✳ ✴ ✵ ✶ ✷ ✸ ✹ ✺ ✻ ✼ ✽ ✾ ✿
❀  ❁ ❂ ❃ ❄ ❅ ❆ ❇ ❈ ❉ ❊ ❋ ● ❍ ■ ❏
❐  ❑ ❒ ▲ ▼ ◆ ❖ ◗ ❘ ❙ ❚ ❛ ❜ ❝ ❞ 
\  ❡ ❢ ❣ ❤ ❥ ❦ ❧ ♣ ♦ ♥ ♠ ① ② ③ ④
⑤  ⑥ ⑦ ⑧ ⑨ ⑩ ❶ ❷ ❸ ❹ ❺ ❻ ❼ ❽ ❾ ❿
➀  ➁ ➂ ➃ ➄ ➅ ➆ ➇ ➈ ➉ ➊ ➋ ➌ ➍ ➎ ➏
➐  ➑ ➒ ➓ ➔ → ↔ ↕ ➘ ➙ ➚ ➛ ➜ ➝ ➞ ➟
➠  ➡ ➢ ➣ ➤ ➥ ➦ ➧ ➨ ➩ ➪ ➫ ➬ ➭ ➮ ➯
\  ➱ ➲ ➳ ➴ ➵ ➶ ➷ ➸ ➹ ➺ ➻ ➼ ➽ ➾ 
== = = = = = = = = = = = = = = =

Other symbols
-------------

Here is a list of commands that are known to *Tralics* but whose
translation does not correspond to a Unicode character. Thus, an error
is signaled.

.. container:: ltx-source

   ::

      %\textascendercompwordmark\textcapitalcompwordmark
      % \textlbrackdbl\textrbrackdbl\texttildelow\textleaf\textdivorced\textborn
      % \textquotestraightbase\textquotestraightdblbase\textmarried\textdied
      % \textinterrobangdown\textcopyleft\textdollaroldstyle\textdollaroldstyle
      % \textblank\textcentoldstyle\textcentoldstyle
      % \textcentoldstyle\textdblhyphen
      % \textfouroldstyle \textfiveoldstyle \texteightoldstyle\textnineoldstyle
      % \textoneoldstyle \textsevenoldstyle\textsixoldstyle
      % \textthreeoldstyle\textzerooldstyle \texttwooldstyle

Here is a list of commands whose name start with \`text'.

.. container:: ltx-source

   ::

      \textacutedbl\ \textasciiacute\ 
      \textasciiacutex\  \textasciibreve\  \textasciicaron\  \textasciicedilla\ 
      \textasciicircum\  \textasciidieresis\  \textasciigrave\  \textasciimacron\ 
      \textasciitilde\  \textasteriskcentered\  \textbackslash\ \textbaht\ \textbar\ 
      \textbardbl\  \textbigcircle\    \textbraceleft\ 
      \textbraceright\  \textbrokenbar\  \textbullet\  
      \textcelsius\  \textcent\   \textcircledP\  \textcolonmonetary\ 
      \textcopyright\  \textcurrency\  \textdagger\ 
      \textdaggerdbl\    \textdegree\   
      \textdiv\  \textdollar\    \textdong\ 
      \textdownarrow\ \textellipsis\  \textemdash\  \textendash\ 
      \textestimated\  \texteuro\  \textexclamdown\  \textflorin\ 
      \textfractionsolidus\  \textfractionsolidus\ 
      \textfrenchfranc\ \textgravedbl\  \textgreater\  
      \textinterrobang\  \textlangle\ 
      \textleftarrow\  \textless\  \textlira\  \textlnot\ 
      \textlquill\  \textmho\  \textminus\  \textmu\ 
      \textmusicalnote\  \textnaira\   \textnormal\  \textnospace\ 
      \textnumero\  \textohm\  \textonehalf\   \textonequarter\ 
      \textonesuperior\  \textopenbullet\  \textordfeminine\  \textordmasculine\ 
      \textparagraph\  \textperiodcentered\  \textpertenthousand\  \textperthousand\ 
      \textpeso\  \textpilcrow\  \textpm\  \textquestiondown\  \textquotedblleft\ 
      \textquotedblright\  \textquoteleft\  \textquoteright\  \textquotesingle\ 
      \textrangle\ \textrecipe\  \textreferencemark\  \textregistered\ 
      \textrightarrow\   \textrquill\   \textsection\ 
      \textservicemark\ \textsofthyphen\  \textsterling 
      \textsurd\   \textthreequarters\  \textthreequartersemdash\ 
      \textthreesuperior\   \texttimes\  \texttrademark\ 
      \texttwelveudash\  \texttwosuperior\  \textunderscore\ 
      \textuparrow\  \textvisiblespace\    \textwon\  \textyen\ 

Translation: ″ ´ ′ ̆ ̌ ¸ ⌃ ¨ ‵ ¯ ~ ⁎ \\ ฿ \| ‖ ◯ { } ¦ • ℃ ¢ ℗ ₡ © ¤ † ‡
° ÷ $ ₫ ↓ … — – ℮ € ¡ ƒ ⁄ ⁄ ₣ ‶ > ‽ 〈 ← < ₤ ¬ ⁅ ℧ — µ ♪ ₦ ​ № Ω ½ ¼ ¹ ◦
ª º ¶ · ‱ ‰ ₱ ¶ ± ¿ “ ” ‘ ’ ' 〉 ℞ ※ ® → ⁆ § ℠ ­ £√ ¾ — ³ × ™ — ² \_ ↑ ␣
₩ ¥

Math symbols
============

| We present here some math characters; for a full and up-to-date list
  of constructions consult `Producing MathML with
  Tralics <quadrat/testmath.xml>`__. All character tables shown here are
  converted from XML to Pdf via xmltex, then to png images. You can say
  ``\mathmo{\&\#x2190;}`` in order to access the Unicode character
  U+2190 as a math operator. On the other hand ``^^^^2190`` produces the
  same character as a math identifier. In what follows, we assume the
  following definition
| ``\def\unimp#1{\mathmo{\&\#x#1;}}``

There is a file named tralics-iso.sty, that defines ``\mathslarr`` to be
equivalent to ``\mathmo{\&\#x2190;}``. The Unicode name of this
character is: \`leftwards arrow', and isoamsa defines the entity
``&slarr;`` as \`short left arrow'.

Arrows
------

Characters with code between U+2190 and U+21FF are shown in the table
below. Some symbols have no glyphs in the standard LaTeX fonts. They are
shown in the table by a dot if there is no entity in the isoamsXXX
files, their Unicode number otherwise.

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{2190}&\mathslarr&\mathuarr& \mathsrarr&\mathdarr&
       \mathharr& \mathvarr&\mathnwarr&\mathnearr&
       \mathsearr& \mathswarr& \mathnlarr&\mathnrarr&
       .&\mathrarrw& \mathLarr&\mathUarr\\
      \text{21A0}&\mathRarr& \mathDarr& \mathlarrtl&\mathrarrtl&
        \mathmapstoleft&\mathmapstoup&\mathmap&\mathmapstodown&
        .&\mathlarrhk&\mathrarrhk&\mathlarrlp&
        \mathrarrlp&\mathharrw&\mathnharr&\unimp{21af}\\
      \text{21B0}&\mathlsh&\mathrsh&\mathldsh&\mathrdsh&
       .&.&\mathcularr&\mathcurarr&
       . & .&\matholarr&\mathorarr&
       \mathlharu&\mathlhard&\mathuharr&\mathuharl\\
      \text{21C0}&\mathrharu&\mathrhard&\mathdharr&\mathdharl&
       \mathrlarr&\mathudarr&\mathlrarr&\mathllarr&
       \mathuuarr&\mathrrarr&\mathddarr&\mathlrhar&
       \mathrlhar&\mathnlArr&\mathnhArr&\mathnrArr\\
      \text{21D0}&\mathlArr&\mathuArr&\mathrArr&\mathdArr&
       \mathhArr&\mathvArr&\mathnwArr&\mathneArr&
       \mathseArr&\mathswArr&\mathlAarr&\mathrAarr&
       \unimp{21dc}&\mathzigrarr&.&.\\
      \text{21E0}&\unimp{21e0}& .& \unimp{21e2}& .&
       \mathlarrb& \mathrarrb& .& .&
       .& .& . & . & . & .& .&  .\\
      \text{21F0}& .& .&. & .& . & \mathduarr& .& .& 
       .& .& .&  .& .& \mathloarr& \mathroarr& \mathhoarr\\
      \end{array}$

Translation is trivial. We omit a big part of the table.

.. container:: xml_out

   ::

      <formula type='inline'>
       <math xmlns='http://www.w3.org/1998/Math/MathML'>
        <mtable>
         <mtr>
          <mtd columnalign='left'><mrow><mtext>2190</mtext></mrow></mtd>
          <mtd><mo>&#x02190;</mo></mtd>
          <mtd><mo>&#x02191;</mo></mtd>
          <mtd><mo>&#x02192;</mo></mtd>
         ...
          <mtd><mo>&#x021FE;</mo></mtd>
          <mtd><mo>&#x021FF;</mo></mtd>
         </mtr>
        </mtable>
       </math>
      </formula>

This is the preview. |Unicode Arrows|

Mathematical operators
----------------------

Characters with code between U+2200 and U+22FF are shown in the table
below. Some symbols have no glyphs in the standard LaTeX fonts. Symbols
between U+22F2 and U+22FF (variants of \`element of') are not shown.
Others are shown by their Unicode number.

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{2200}& \mathforall& \mathcomp&\mathpart&\mathexist& 
       \mathnexist&  \mathempty& \unimp{2206}&\mathnabla&
       \mathisin&\mathnotin&\unimp{220a}&\mathni&
       \mathnotni&\unimp{220d}&\unimp{220e}&\mathprod\\
      \text{2210}&\mathcoprod&\mathsum&\mathminus&\mathmnplus&
        \mathplusdo&\unimp{2215}&\mathsetmn&\mathlowast&
        \mathcompfn&\unimp{2219}&\mathradic&\unimp{221B}&
        \unimp{221c}&\mathprop&\mathinfin&\mathangrt\\
      \text{2220}&\mathang&\mathangmsd&\unimp{2222}&\mathshortmid&
        \mathnmid&\mathspar&\mathnpar&\mathand&
        \mathor&\mathcap&\mathcup&\mathint&
        \mathInt&\mathtint&\mathconint&\mathConint\\
      \text{2230}&\mathCconint&\mathcwint&\mathcwconint&\mathawconint&
       \maththerefore&\mathbecaus&\mathratio&\mathColon&
       \mathminusd&\unimp{2239}&\mathmDDot&\mathhomtht&
       \maththksim&\mathbsim&\mathac&\mathmstpos\\
      \text{2240}&\mathwreath&\mathnsim&\mathesim&\mathsime&
        \mathnsime&\mathcong&\mathsimne&\mathncong&
        \mathasymp&\mathnap&\mathape&\mathapid&  
        \mathbcong&\mathasympeq&\mathbump&\mathbumpe\\
      \text{2250}&\mathesdot&\matheDot&\mathefDot&\matherDot&
        \mathcolone&\mathecolon&\mathecir&\mathcire&
         \unimp{2258}&\unimp{2259}&\unimp{225a}&\unimp{225b}&
         \mathtrie&\unimp{225d}&\unimp{225e}&\mathequest\\
      \text{2260}&\mathne&\mathequiv&\mathnequiv&\unimp{2263}&
        \mathle&\mathge&\mathlE&\mathgE&
        \mathlnE&\mathgnE&\mathLt&\mathGt&
        \mathtwixt&\mathNotCupCap&\mathnlt&\mathngt\\
      \text{2270}&\mathnle&\mathnge&\mathlsim&\mathgsim&
        \mathnlsim&\mathngsim&\mathlg&\mathgl&
        \mathntlg&\mathntgl&\mathpr&\mathsc&
        \mathprcue&\mathsccue&\mathprsim&\mathscsim\\
      \text{2280}&\mathnpr&\mathnsc&\mathsub&\mathsup&
       \mathnsub&\mathnsup&\mathsube&\mathsupe&
        \mathnsube&\mathnsupe&\mathsubne&\mathsupne&
       \unimp{228c}&\mathcupdot&\mathuplus&\mathsqsub\\
      \text{2290}&\mathsqsup&\mathsqsube&\mathsqsupe&\mathsqcap&
        \mathsqcup&\mathoplus&\mathominus&\mathotimes&
        \mathosol&\mathodot&\mathocir&\mathoast&
        \unimp{229c}&\mathodash&\mathplusb&\mathminusb\\
      \text{22a0}&\mathtimesb&\mathsdotb&\mathvdash&\mathdashv&
        \mathtop&\mathbottom&\unimp{22a6}&\mathmodels&
         \mathvDash&\mathVdash&\mathVvdash&\mathVDash&
         \mathnvdash&\mathnvDash&\mathnVdash&\mathnVDash\\
      \text{22b0}&\mathprurel&\unimp{22b1}&\mathvltri&\mathvrtri&
       \mathltrie&\mathrtrie&\mathorigof&\mathimof&
        \mathmumap&\mathhercon&\mathintcal&\mathveebar&
        \unimp{22bc}&\mathbarvee&\mathangrtvb&\mathlrtri\\
      \text{22c0}&\mathxwedge&\mathxvee&\mathxcap&\mathxcup&
       \mathdiam&\mathsdot&\mathsstarf&\mathdivonx&
        \mathbowtie&\mathltimes&\mathrtimes&\mathlthree&
        \mathrthree&\mathbsime&\mathcuvee&\mathcuwed\\
      \text{22d0}&\mathSub&\mathSup&\mathCap&\mathCup&
        \mathfork&\mathepar&\mathlessdot&\mathgtrdot&
        \mathLl&\mathGg&\mathleg&\mathgel&
         \unimp{22dc}&\unimp{22dd}&\mathcuepr&\mathcuesc\\
      \text{22e0}&\mathnprcue&\mathnsccue&\mathnsqsube&\mathnsqsupe&
        \unimp{22e4}&\unimp{22e5}&\mathlnsim&\mathgnsim&
        \mathprecnsim&\mathscnsim&\mathnltri&\mathnrtri&
        \mathnltrie&\mathnrtrie&\mathvellip&\mathctdot\\
      \text{22f0}&\mathutdot&\mathdtdot&\mathdisin&\mathisinsv&
        \mathisins&\mathisindot&\mathnotinvc&\mathnotinvb&
        \unimp{22f8}&\mathisinE&\mathnisd&\mathxnis&
        \mathnis&\mathnotnivc&\mathnotnivb&\unimp{22ff}\\
      \end{array}$

This is the preview. |Unicode Mathematical operators|

Miscellaneous symbols
---------------------

We consider here symbols between U+2300 and U+23FF. About 30 symbols are
implemented. The table shows also some symbols between U+2600 and
U+2607, and others between U+263D and U+2685.

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{2300}& \unimp{2300}&\unimp{2301}&\unimp{2302}&\unimp{2303}&
       \unimp{2304}&\mathbarwedge&\mathBarwed&\unimp{2307}&
       \mathlceil&\mathrceil&\mathlfloor&\mathrfloor&
       \mathdrcrop&\mathdlcrop&\mathurcrop&\mathulcrop\\
      \text{2310}&\mathbnot& \unimp{2311}& \mathprofline&\mathprofsurf&
       \unimp{2314}& \mathtelrec&\mathtarget&\unimp{2317}&
       \unimp{2318}& \unimp{2319}& \unimp{231a}& \unimp{231b}&
        \mathulcorner& \mathurcorner&\mathllcorner&\mathlrcorner\\
      \text{2320}&\unimp{2320}&\unimp{2321}&\mathfrown&\mathsmile&
       \unimp{2324}&\unimp{2325}&\unimp{2326}&\unimp{2327}&
        \unimp{2328}&\mathlangle&\mathrangle&\unimp{232b}&
        \unimp{232c}& \mathcylcty&\mathprofalar&\unimp{232f}\\
      \text{2330}&&&&&&&\mathtopbot&&&\unimp{2339}&&&&
        \mathovbar& &\mathsolbar\\
      \text{2340}&\unimp{2340}&&&&&&&\unimp{2347}&\unimp{2348}\\
      \text{2350}&\unimp{2350}&&&&&&&\unimp{2357}\\
      \text{2600}&&\unimp{2601}&&&&\mathbigstar&\mathstar&&&&&&\unimp{260C}&
        \unimp{260d}\unimp{263d}& \unimp{263e}&\unimp{263f}\\
      \text{2640}&\mathfemale&\unimp{2641}&\mathmale&\unimp{2643}
        &\unimp{2644}&\unimp{2645}&\unimp{2646}&\unimp{2647}&\unimp{2648}&
      \unimp{2649}&\unimp{264A}&\unimp{264b}&\unimp{264c}&
        \unimp{264d}&\unimp{264e} &\unimp{264f}\\
      \text{2650}&\unimp{2650}&\unimp{2651}&\unimp{2652}&\unimp{2653}
        &\unimp{2654}&\unimp{2655}&\unimp{2656}&\unimp{2657}&\unimp{2658}&
      \unimp{2659}&\unimp{265A}&\unimp{265b}&\unimp{265c}&
        \unimp{265d}&\unimp{265e} &\unimp{265f}\\
      \text{2660}&\mathspadesuit&\unimp{2661}&\unimp{2662}&\mathclubsuit
        &\unimp{2664}&\mathheartsuit&\mathdiamondsuit&\unimp{2667}&&
      \unimp{2669}&\mathsung&&&
        \mathflat&\mathnatur &\mathsharp\\
      \text{2680}&\unimp{2680}&\unimp{2681}&\unimp{2682}&\unimp{2683}
        &\unimp{2684}&\unimp{2685}\\
      \end{array}$

This is the preview.

|Unicode Miscellaneous symbols|

Math characters
---------------

Symbols between U+1D400 and U+1D7FF. First part. There are 12 empty
slots in the table symbols. These characters are somewhere else. They
are marked by a plus sign.

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{1d400}&\unimp{1d400}&\unimp{1d401}&\unimp{1d402}&\unimp{1d403}&
       \unimp{1d404}&\unimp{1d405}&\unimp{1d406}&\unimp{1d407}&
       \unimp{1d408}&\unimp{1d409}&\unimp{1d40a}&\unimp{1d40b}&
       \unimp{1d40c}&\unimp{1d40d}&\unimp{1d40e}&\unimp{1d40f}\\
      \text{1d410}&\unimp{1d410}&\unimp{1d411}&\unimp{1d412}&\unimp{1d413}&
       \unimp{1d414}&\unimp{1d415}&\unimp{1d416}&\unimp{1d417}&
       \unimp{1d418}&\unimp{1d419}&\unimp{1d41a}&\unimp{1d41b}&
       \unimp{1d41c}&\unimp{1d41d}&\unimp{1d41e}&\unimp{1d41f}\\
      \text{1d420}&\unimp{1d420}&\unimp{1d421}&\unimp{1d422}&\unimp{1d423}&
       \unimp{1d424}&\unimp{1d425}&\unimp{1d426}&\unimp{1d427}&
       \unimp{1d428}&\unimp{1d429}&\unimp{1d42a}&\unimp{1d42b}&
       \unimp{1d42c}&\unimp{1d42d}&\unimp{1d42e}&\unimp{1d42f}\\
      \text{1d430}&\unimp{1d430}&\unimp{1d431}&\unimp{1d432}&\unimp{1d433}&
       \unimp{1d434}&\unimp{1d435}&\unimp{1d436}&\unimp{1d437}&
       \unimp{1d438}&\unimp{1d439}&\unimp{1d43a}&\unimp{1d43b}&
       \unimp{1d43c}&\unimp{1d43d}&\unimp{1d43e}&\unimp{1d43f}\\
      \text{1d440}&\unimp{1d440}&\unimp{1d441}&\unimp{1d442}&\unimp{1d443}&
       \unimp{1d444}&\unimp{1d445}&\unimp{1d446}&\unimp{1d447}&
       \unimp{1d448}&\unimp{1d449}&\unimp{1d44a}&\unimp{1d44b}&
       \unimp{1d44c}&\unimp{1d44d}&\unimp{1d44e}&\unimp{1d44f}\\
      \text{1d450}&\unimp{1d450}&\unimp{1d451}&\unimp{1d452}&\unimp{1d453}&
       \unimp{1d454}&+\unimp{210e}&\unimp{1d456}&\unimp{1d457}&
       \unimp{1d458}&\unimp{1d459}&\unimp{1d45a}&\unimp{1d45b}&
       \unimp{1d45c}&\unimp{1d45d}&\unimp{1d45e}&\unimp{1d45f}\\
      \text{1d460}&\unimp{1d460}&\unimp{1d461}&\unimp{1d462}&\unimp{1d463}&
       \unimp{1d464}&\unimp{1d465}&\unimp{1d466}&\unimp{1d467}&
       \unimp{1d468}&\unimp{1d469}&\unimp{1d46a}&\unimp{1d46b}&
       \unimp{1d46c}&\unimp{1d46d}&\unimp{1d46e}&\unimp{1d46f}\\
      \text{1d470}&\unimp{1d470}&\unimp{1d471}&\unimp{1d472}&\unimp{1d473}&
       \unimp{1d474}&\unimp{1d475}&\unimp{1d476}&\unimp{1d477}&
       \unimp{1d478}&\unimp{1d479}&\unimp{1d47a}&\unimp{1d47b}&
       \unimp{1d47c}&\unimp{1d47d}&\unimp{1d47e}&\unimp{1d47f}\\
      \text{1d480}&\unimp{1d480}&\unimp{1d481}&\unimp{1d482}&\unimp{1d483}&
       \unimp{1d484}&\unimp{1d485}&\unimp{1d486}&\unimp{1d487}&
       \unimp{1d488}&\unimp{1d489}&\unimp{1d48a}&\unimp{1d48b}&
       \unimp{1d48c}&\unimp{1d48d}&\unimp{1d48e}&\unimp{1d48f}\\
      \text{1d490}&\unimp{1d490}&\unimp{1d491}&\unimp{1d492}&\unimp{1d493}&
       \unimp{1d494}&\unimp{1d495}&\unimp{1d496}&\unimp{1d497}&
       \unimp{1d498}&\unimp{1d499}&\unimp{1d49a}&\unimp{1d49b}&
       \mathAscr&+\mathBscr&\mathCscr&\mathDscr\\
      \text{1d4a0}&+\mathEscr&+\mathFscr&\mathGscr&+\mathHscr&
       +\mathIscr&\mathJscr&\mathKscr&+\mathLscr&
       +\mathMscr&\mathNscr&\mathOscr&\mathPscr&
       \mathQscr&+\mathRscr&\mathSscr&\mathTscr\\
      \text{1d4b0}&\mathUscr&\mathVscr&\mathWscr&\mathXscr&
       \mathYscr&\mathZscr&\mathascr&\mathbscr&
       \mathcscr&\mathdscr&+\mathescr&\mathfscr&
       +\mathgscr&\mathhscr&\mathiscr&\mathjscr\\
      \text{1d4c0}&\mathkscr&\mathlscr&\mathmscr&\mathnscr&
       +\mathoscr&\mathpscr&\mathqscr&\mathrscr&
       \mathsscr&\mathtscr&\mathuscr&\mathvscr&
       \mathwscr&\mathxscr&\mathyscr&\mathzscr\\
      \text{1d4d0}&\unimp{1d4d0}&\unimp{1d4d1}&\unimp{1d4d2}&\unimp{1d4d3}&
       \unimp{1d4d4}&\unimp{1d4d5}&\unimp{1d4d6}&\unimp{1d4d7}&
       \unimp{1d4d8}&\unimp{1d4d9}&\unimp{1d4da}&\unimp{1d4db}&
       \unimp{1d4dc}&\unimp{1d4dd}&\unimp{1d4de}&\unimp{1d4df}\\
      \text{1d4e0}&\unimp{1d4e0}&\unimp{1d4e1}&\unimp{1d4e2}&\unimp{1d4e3}&
       \unimp{1d4e4}&\unimp{1d4e5}&\unimp{1d4e6}&\unimp{1d4e7}&
       \unimp{1d4e8}&\unimp{1d4e9}&\unimp{1d4ea}&\unimp{1d4eb}&
       \unimp{1d4ec}&\unimp{1d4ed}&\unimp{1d4ee}&\unimp{1d4ef}\\
      \text{1d4f0}&\unimp{1d4f0}&\unimp{1d4f1}&\unimp{1d4f2}&\unimp{1d4f3}&
       \unimp{1d4f4}&\unimp{1d4f5}&\unimp{1d4f6}&\unimp{1d4f7}&
       \unimp{1d4f8}&\unimp{1d4f9}&\unimp{1d4fa}&\unimp{1d4fb}&
       \unimp{1d4fc}&\unimp{1d4fd}&\unimp{1d4fe}&\unimp{1d4ff}\\
      \end{array}$

|Unicode Mathematical alphanumeric symbols|

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{1d500}&\unimp{1d500}&\unimp{1d501}&\unimp{1d502}&\unimp{1d503}&
       \mathAfr&\mathBfr&+\mathCfr&\mathDfr&
       \mathEfr&\mathFfr&\mathGfr&+\mathHfr&
       +\mathIfr&\mathJfr&\mathKfr&\mathLfr\\
      \text{1d510}&\mathMfr&\mathNfr&\mathOfr&\mathPfr&
       \mathQfr&+\mathRfr&\mathSfr&\mathTfr&
       \mathUfr&\mathVfr&\mathWfr&\mathXfr&
       \mathYfr&+\mathZfr&\mathafr&\mathbfr\\
      \text{1d520}&\mathcfr&\mathdfr&\mathefr&\mathffr&
       \mathgfr&\mathhfr&\mathifr&\mathjfr&
       \mathkfr&\mathlfr&\mathmfr&\mathnfr&
       \mathofr&\mathpfr&\mathqfr&\mathrfr\\
      \text{1d530}&\mathsfr&\mathtfr&\mathufr&\mathvfr&
       \mathwfr&\mathxfr&\mathyfr&\mathzfr&
       \mathAopf&\mathBopf&+\mathCopf&\mathDopf&
       \mathEopf&\mathFopf&\mathGopf&+\mathHopf\\
      \text{1d540}&\mathIopf&\mathJopf&\mathKopf&\mathLopf&
       \mathMopf&+\mathNopf&\mathOopf&+\mathPopf&
       +\mathQopf&+\mathRopf&\mathSopf&\mathTopf&
       \mathUopf&\mathVopf&\mathWopf&\mathXopf\\
      \text{1d550}&\mathYopf&+\mathZopf&\mathaopf&\mathbopf&
       \mathcopf&\mathdopf&\matheopf&\mathfopf&
       \mathgopf&\mathhopf&\mathiopf&\mathjopf&
       \mathkopf&\mathlopf&\mathmopf&\mathnopf\\
      \text{1d560}&\mathoopf&\mathpopf&\mathqopf&\mathropf&
       \mathsopf&\mathtopf&\mathuopf&\mathvopf&
       \mathwopf&\mathxopf&\mathyopf&\mathzopf&
       \unimp{1d56c}&\unimp{1d56d}&\unimp{1d56e}&\unimp{1d56f}\\
      \text{1d570}&\unimp{1d570}&\unimp{1d571}&\unimp{1d572}&\unimp{1d573}&
       \unimp{1d574}&\unimp{1d575}&\unimp{1d576}&\unimp{1d577}&
       \unimp{1d578}&\unimp{1d579}&\unimp{1d57a}&\unimp{1d57b}&
       \unimp{1d57c}&\unimp{1d57d}&\unimp{1d57e}&\unimp{1d57f}\\
      \text{1d580}&\unimp{1d580}&\unimp{1d581}&\unimp{1d582}&\unimp{1d583}&
       \unimp{1d584}&\unimp{1d585}&\unimp{1d586}&\unimp{1d587}&
       \unimp{1d588}&\unimp{1d589}&\unimp{1d58a}&\unimp{1d58b}&
       \unimp{1d58c}&\unimp{1d58d}&\unimp{1d58e}&\unimp{1d58f}\\
      \text{1d590}&\unimp{1d590}&\unimp{1d591}&\unimp{1d592}&\unimp{1d593}&
       \unimp{1d594}&\unimp{1d595}&\unimp{1d596}&\unimp{1d597}&
       \unimp{1d598}&\unimp{1d599}&\unimp{1d59a}&\unimp{1d59b}&
       \unimp{1d59c}&\unimp{1d59d}&\unimp{1d59e}&\unimp{1d59f}\\
      \text{1d5a0}&\unimp{1d5a0}&\unimp{1d5a1}&\unimp{1d5a2}&\unimp{1d5a3}&
       \unimp{1d5a4}&\unimp{1d5a5}&\unimp{1d5a6}&\unimp{1d5a7}&
       \unimp{1d5a8}&\unimp{1d5a9}&\unimp{1d5aa}&\unimp{1d5ab}&
       \unimp{1d5ac}&\unimp{1d5ad}&\unimp{1d5ae}&\unimp{1d5af}\\
      \text{1d5b0}&\unimp{1d5b0}&\unimp{1d5b1}&\unimp{1d5b2}&\unimp{1d5b3}&
       \unimp{1d5b4}&\unimp{1d5b5}&\unimp{1d5b6}&\unimp{1d5b7}&
       \unimp{1d5b8}&\unimp{1d5b9}&\unimp{1d5ba}&\unimp{1d5bb}&
       \unimp{1d5bc}&\unimp{1d5bd}&\unimp{1d5be}&\unimp{1d5af}\\
      \text{1d5c0}&\unimp{1d5c0}&\unimp{1d5c1}&\unimp{1d5c2}&\unimp{1d5c3}&
       \unimp{1d5c4}&\unimp{1d5c5}&\unimp{1d5c6}&\unimp{1d5c7}&
       \unimp{1d5c8}&\unimp{1d5c9}&\unimp{1d5ca}&\unimp{1d5cb}&
       \unimp{1d5cc}&\unimp{1d5cd}&\unimp{1d5ce}&\unimp{1d5cf}\\
      \text{1d5d0}&\unimp{1d5d0}&\unimp{1d5d1}&\unimp{1d5d2}&\unimp{1d5d3}&
       \unimp{1d5d4}&\unimp{1d5d5}&\unimp{1d5d6}&\unimp{1d5d7}&
       \unimp{1d5d8}&\unimp{1d5d9}&\unimp{1d5da}&\unimp{1d5db}&
       \unimp{1d5dc}&\unimp{1d5dd}&\unimp{1d5de}&\unimp{1d5df}\\
      \text{1d5e0}&\unimp{1d5e0}&\unimp{1d5e1}&\unimp{1d5e2}&\unimp{1d5e3}&
       \unimp{1d5e4}&\unimp{1d5e5}&\unimp{1d5e6}&\unimp{1d5e7}&
       \unimp{1d5e8}&\unimp{1d5e9}&\unimp{1d5ea}&\unimp{1d5eb}&
       \unimp{1d5ec}&\unimp{1d5ed}&\unimp{1d5ee}&\unimp{1d5ef}\\
      \text{1d5f0}&\unimp{1d5f0}&\unimp{1d5f1}&\unimp{1d5f2}&\unimp{1d5f3}&
       \unimp{1d5f4}&\unimp{1d5f5}&\unimp{1d5f6}&\unimp{1d5f7}&
       \unimp{1d5f8}&\unimp{1d5f9}&\unimp{1d5fa}&\unimp{1d5fb}&
       \unimp{1d5fc}&\unimp{1d5fd}&\unimp{1d5fe}&\unimp{1d5ff}\\
      \end{array}$

|Unicode Mathematical alphanumeric symbols 2|

.. container:: ltx-source

   ::

      $\begin{array}{lccccccccccccccccc}
      \text{1d600}&\unimp{1d600}&\unimp{1d601}&\unimp{1d602}&\unimp{1d603}&
       \unimp{1d604}&\unimp{1d605}&\unimp{1d606}&\unimp{1d607}&
       \unimp{1d608}&\unimp{1d609}&\unimp{1d60a}&\unimp{1d60b}&
       \unimp{1d60c}&\unimp{1d60d}&\unimp{1d60e}&\unimp{1d60f}\\
      \text{1d610}&\unimp{1d610}&\unimp{1d611}&\unimp{1d612}&\unimp{1d613}&
       \unimp{1d614}&\unimp{1d615}&\unimp{1d616}&\unimp{1d617}&
       \unimp{1d618}&\unimp{1d619}&\unimp{1d61a}&\unimp{1d61b}&
       \unimp{1d61c}&\unimp{1d61d}&\unimp{1d61e}&\unimp{1d61f}\\
      \text{1d620}&\unimp{1d620}&\unimp{1d621}&\unimp{1d622}&\unimp{1d623}&
       \unimp{1d624}&\unimp{1d625}&\unimp{1d626}&\unimp{1d627}&
       \unimp{1d628}&\unimp{1d629}&\unimp{1d62a}&\unimp{1d62b}&
       \unimp{1d62c}&\unimp{1d62d}&\unimp{1d62e}&\unimp{1d62f}\\
      \text{1d630}&\unimp{1d630}&\unimp{1d631}&\unimp{1d632}&\unimp{1d633}&
       \unimp{1d634}&\unimp{1d635}&\unimp{1d636}&\unimp{1d637}&
       \unimp{1d638}&\unimp{1d639}&\unimp{1d63a}&\unimp{1d63b}&
       \unimp{1d63c}&\unimp{1d63d}&\unimp{1d63e}&\unimp{1d63f}\\
      \text{1d640}&\unimp{1d640}&\unimp{1d641}&\unimp{1d642}&\unimp{1d643}&
       \unimp{1d644}&\unimp{1d645}&\unimp{1d646}&\unimp{1d647}&
       \unimp{1d648}&\unimp{1d649}&\unimp{1d64a}&\unimp{1d64b}&
       \unimp{1d64c}&\unimp{1d64d}&\unimp{1d64e}&\unimp{1d64f}\\
      \text{1d650}&\unimp{1d650}&\unimp{1d651}&\unimp{1d652}&\unimp{1d653}&
       \unimp{1d654}&\unimp{1d655}&\unimp{1d656}&\unimp{1d657}&
       \unimp{1d658}&\unimp{1d659}&\unimp{1d65a}&\unimp{1d65b}&
       \unimp{1d65c}&\unimp{1d65d}&\unimp{1d65e}&\unimp{1d65f}\\
      \text{1d660}&\unimp{1d660}&\unimp{1d661}&\unimp{1d662}&\unimp{1d663}&
       \unimp{1d664}&\unimp{1d665}&\unimp{1d666}&\unimp{1d667}&
       \unimp{1d668}&\unimp{1d669}&\unimp{1d66a}&\unimp{1d66b}&
       \unimp{1d66c}&\unimp{1d66d}&\unimp{1d66e}&\unimp{1d66f}\\
      \text{1d670}&\unimp{1d670}&\unimp{1d671}&\unimp{1d672}&\unimp{1d673}&
       \unimp{1d674}&\unimp{1d675}&\unimp{1d676}&\unimp{1d677}&
       \unimp{1d678}&\unimp{1d679}&\unimp{1d67a}&\unimp{1d67b}&
       \unimp{1d67c}&\unimp{1d67d}&\unimp{1d67e}&\unimp{1d67f}\\
      \text{1d670}&\unimp{1d670}&\unimp{1d671}&\unimp{1d672}&\unimp{1d673}&
       \unimp{1d674}&\unimp{1d675}&\unimp{1d676}&\unimp{1d677}&
       \unimp{1d678}&\unimp{1d679}&\unimp{1d67a}&\unimp{1d67b}&
       \unimp{1d67c}&\unimp{1d67d}&\unimp{1d67e}&\unimp{1d67f}\\
      \text{1d680}&\unimp{1d680}&\unimp{1d681}&\unimp{1d682}&\unimp{1d683}&
       \unimp{1d684}&\unimp{1d685}&\unimp{1d686}&\unimp{1d687}&
       \unimp{1d688}&\unimp{1d689}&\unimp{1d68a}&\unimp{1d68b}&
       \unimp{1d68c}&\unimp{1d68d}&\unimp{1d68e}&\unimp{1d68f}\\
      \text{1d690}&\unimp{1d690}&\unimp{1d691}&\unimp{1d692}&\unimp{1d693}&
       \unimp{1d694}&\unimp{1d695}&\unimp{1d696}&\unimp{1d697}&
       \unimp{1d698}&\unimp{1d699}&\unimp{1d69a}&\unimp{1d69b}&
       \unimp{1d69c}&\unimp{1d69d}&\unimp{1d69e}&\unimp{1d69f}\\
      \text{1d6a0}&\unimp{1d6a0}&\unimp{1d6a1}&\unimp{1d6a2}&\unimp{1d6a3}&
       &&&&
       \unimp{1d6a8}&\unimp{1d6a9}&\unimp{1d6aa}&\unimp{1d6ab}&
       \unimp{1d6ac}&\unimp{1d6ad}&\unimp{1d6ae}&\unimp{1d6af}\\
      \text{1d6b0}&\unimp{1d6b0}&\unimp{1d6b1}&\unimp{1d6b2}&\unimp{1d6b3}&
       \unimp{1d6b4}&\unimp{1d6b5}&\unimp{1d6b6}&\unimp{1d6b7}&
       \unimp{1d6b8}&\unimp{1d6b9}&\unimp{1d6ba}&\unimp{1d6bb}&
       \unimp{1d6bc}&\unimp{1d6bd}&\unimp{1d6be}&\unimp{1d6bf}\\
      \text{1d6c0}&\unimp{1d6c0}&\unimp{1d6c1}&\unimp{1d6c2}&\unimp{1d6c3}&
       \unimp{1d6c4}&\unimp{1d6c5}&\unimp{1d6c6}&\unimp{1d6c7}&
       \unimp{1d6c8}&\unimp{1d6c9}&\unimp{1d6ca}&\unimp{1d6cb}&
       \unimp{1d6cc}&\unimp{1d6cd}&\unimp{1d6ce}&\unimp{1d6cf}\\
      \text{1d6d0}&\unimp{1d6d0}&\unimp{1d6d1}&\unimp{1d6d2}&\unimp{1d6d3}&
       \unimp{1d6d4}&\unimp{1d6d5}&\unimp{1d6d6}&\unimp{1d6d7}&
       \unimp{1d6d8}&\unimp{1d6d9}&\unimp{1d6da}&\unimp{1d6db}&
       \unimp{1d6dc}&\unimp{1d6dd}&\unimp{1d6de}&\unimp{1d6df}\\
      \text{1d6e0}&\unimp{1d6e0}&\unimp{1d6e1}&\unimp{1d6e2}&\unimp{1d6e3}&
       \unimp{1d6e4}&\unimp{1d6e5}&\unimp{1d6e6}&\unimp{1d6e7}&
       \unimp{1d6e8}&\unimp{1d6e9}&\unimp{1d6ea}&\unimp{1d6eb}&
       \unimp{1d6ec}&\unimp{1d6ed}&\unimp{1d6ee}&\unimp{1d6ef}\\
      \text{1d6f0}&\unimp{1d6f0}&\unimp{1d6f1}&\unimp{1d6f2}&\unimp{1d6f3}&
       \unimp{1d6f4}&\unimp{1d6f5}&\unimp{1d6f6}&\unimp{1d6f7}&
       \unimp{1d6f8}&\unimp{1d6f9}&\unimp{1d6fa}&\unimp{1d6fb}&
       \unimp{1d6fc}&\unimp{1d6fd}&\unimp{1d6fe}&\unimp{1d6ff}\\
      \end{array}$

|Unicode Mathematical alphanumeric symbols 3|

.. container:: ltx-source

   ::

      \text{1d700}&\unimp{1d700}&\unimp{1d701}&\unimp{1d702}&\unimp{1d703}&
       \unimp{1d704}&\unimp{1d705}&\unimp{1d706}&\unimp{1d707}&
       \unimp{1d708}&\unimp{1d709}&\unimp{1d70a}&\unimp{1d70b}&
       \unimp{1d70c}&\unimp{1d70d}&\unimp{1d70e}&\unimp{1d70f}\\
      \text{1d710}&\unimp{1d710}&\unimp{1d711}&\unimp{1d712}&\unimp{1d713}&
       \unimp{1d714}&\unimp{1d715}&\unimp{1d716}&\unimp{1d717}&
       \unimp{1d718}&\unimp{1d719}&\unimp{1d71a}&\unimp{1d71b}&
       \unimp{1d71c}&\unimp{1d71d}&\unimp{1d71e}&\unimp{1d71f}\\
      \text{1d720}&\unimp{1d720}&\unimp{1d721}&\unimp{1d722}&\unimp{1d723}&
       \unimp{1d724}&\unimp{1d725}&\unimp{1d726}&\unimp{1d727}&
       \unimp{1d728}&\unimp{1d729}&\unimp{1d72a}&\unimp{1d72b}&
       \unimp{1d72c}&\unimp{1d72d}&\unimp{1d72e}&\unimp{1d72f}\\
      \text{1d730}&\unimp{1d730}&\unimp{1d731}&\unimp{1d732}&\unimp{1d733}&
       \unimp{1d734}&\unimp{1d735}&\unimp{1d736}&\unimp{1d737}&
       \unimp{1d738}&\unimp{1d739}&\unimp{1d73a}&\unimp{1d73b}&
       \unimp{1d73c}&\unimp{1d73d}&\unimp{1d73e}&\unimp{1d73f}\\
      \text{1d740}&\unimp{1d740}&\unimp{1d741}&\unimp{1d742}&\unimp{1d743}&
       \unimp{1d744}&\unimp{1d745}&\unimp{1d746}&\unimp{1d747}&
       \unimp{1d748}&\unimp{1d749}&\unimp{1d74a}&\unimp{1d74b}&
       \unimp{1d74c}&\unimp{1d74d}&\unimp{1d74e}&\unimp{1d74f}\\
      \text{1d750}&\unimp{1d750}&\unimp{1d751}&\unimp{1d752}&\unimp{1d753}&
       \unimp{1d754}&\unimp{1d755}&\unimp{1d756}&\unimp{1d757}&
       \unimp{1d758}&\unimp{1d759}&\unimp{1d75a}&\unimp{1d75b}&
       \unimp{1d75c}&\unimp{1d75d}&\unimp{1d75e}&\unimp{1d75f}\\
      \text{1d760}&\unimp{1d760}&\unimp{1d761}&\unimp{1d762}&\unimp{1d763}&
       \unimp{1d764}&\unimp{1d765}&\unimp{1d766}&\unimp{1d767}&
       \unimp{1d768}&\unimp{1d769}&\unimp{1d76a}&\unimp{1d76b}&
       \unimp{1d76c}&\unimp{1d76d}&\unimp{1d76e}&\unimp{1d76f}\\
      \text{1d7c0}&&&&& &&&&&&&&
       &&\unimp{1d7ce}&\unimp{1d7cf}\\
      \text{1d7d0}&\unimp{1d7d0}&\unimp{1d7d1}&\unimp{1d7d2}&\unimp{1d7d3}&
       \unimp{1d7d4}&\unimp{1d7d5}&\unimp{1d7d6}&\unimp{1d7d7}&
       \unimp{1d7d8}&\unimp{1d7d9}&\unimp{1d7da}&\unimp{1d7db}&
       \unimp{1d7dc}&\unimp{1d7dd}&\unimp{1d7de}&\unimp{1d7df}\\
      \text{1d7e0}&\unimp{1d7e0}&\unimp{1d7e1}&\unimp{1d7e2}&\unimp{1d7e3}&
       \unimp{1d7e4}&\unimp{1d7e5}&\unimp{1d7e6}&\unimp{1d7e7}&
       \unimp{1d7e8}&\unimp{1d7e9}&\unimp{1d7ea}&\unimp{1d7eb}&
       \unimp{1d7ec}&\unimp{1d7ed}&\unimp{1d7ee}&\unimp{1d7ef}\\
      \text{1d7f0}&\unimp{1d7f0}&\unimp{1d7f1}&\unimp{1d7f2}&\unimp{1d7f3}&
       \unimp{1d7f4}&\unimp{1d7f5}&\unimp{1d7f6}&\unimp{1d7f7}&
       \unimp{1d7f8}&\unimp{1d7f9}&\unimp{1d7fa}&\unimp{1d7fb}&
       \unimp{1d7fc}&\unimp{1d7fd}&\unimp{1d7fe}&\unimp{1d7ff}\\
      \end{array}$

Some characters are missing: sans serif bold greek lower case sans serif
bold greek italic |Unicode Mathematical alphanumeric symbols 4|

The T1 encoding
---------------

For some encodings, *Tralics* proposes a conversion to UTF8. The name of
the command has the form ``T1-encoding``, hence cannot be used directly;
see below how to alias it to, say, ``\foo``. The command reads an
argument that must be an integer, for instance ``\foo{65}``,
``\foo{'101}``, ``\foo{"41}``, :literal:`\\foo{`A}`, ``\foo{\count0}``
(the braces are mandatory). The expansion is of the form ``\char128``.
This is a single token, as produced by ``\chardef``. In the T1 encoding,
hex location DF contains "SS", but this is not a Unicode character, thus
provokes an error; character with position less than 32 are not
translated. The argument

.. container:: ltx-source

   ::

      \expandafter\let\expandafter\enc\csname T1-encoding\endcsname
      \enc{"20}\enc{"21}\enc{"22}\enc{"23}\enc{"24}\enc{"25}\enc{"26}\enc{"27}%
      \enc{"28}\enc{"29}\enc{"2A}\enc{"2B}\enc{"2C}\enc{"2D}\enc{"2E}\enc{"2F}
      \enc{"30}\enc{"31}\enc{"32}\enc{"33}\enc{"34}\enc{"35}\enc{"36}\enc{"37}%
      \enc{"38}\enc{"39}\enc{"3A}\enc{"3B}\enc{"3C}\enc{"3D}\enc{"3E}\enc{"3F}
      \enc{"40}\enc{"41}\enc{"42}\enc{"43}\enc{"44}\enc{"45}\enc{"46}\enc{"47}%
      \enc{"48}\enc{"49}\enc{"4A}\enc{"4B}\enc{"4C}\enc{"4D}\enc{"4E}\enc{"4F}
      \enc{"50}\enc{"51}\enc{"52}\enc{"53}\enc{"54}\enc{"55}\enc{"56}\enc{"57}%
      \enc{"58}\enc{"59}\enc{"5A}\enc{"5B}\enc{"5C}\enc{"5D}\enc{"5E}\enc{"5F}
      \enc{"60}\enc{"61}\enc{"62}\enc{"63}\enc{"64}\enc{"65}\enc{"66}\enc{"67}%
      \enc{"68}\enc{"69}\enc{"6A}\enc{"6B}\enc{"6C}\enc{"6D}\enc{"6E}\enc{"6F}
      \enc{"70}\enc{"71}\enc{"72}\enc{"73}\enc{"74}\enc{"75}\enc{"76}\enc{"77}%
      \enc{"78}\enc{"79}\enc{"7A}\enc{"7B}\enc{"7C}\enc{"7D}\enc{"7E}\enc{"7F}
      \enc{"80}\enc{"81}\enc{"82}\enc{"83}\enc{"84}\enc{"85}\enc{"86}\enc{"87}%
      \enc{"88}\enc{"89}\enc{"8A}\enc{"8B}\enc{"8C}\enc{"8D}\enc{"8E}\enc{"8F}
      \enc{"90}\enc{"91}\enc{"92}\enc{"93}\enc{"94}\enc{"95}\enc{"96}\enc{"97}%
      \enc{"98}\enc{"99}\enc{"9A}\enc{"9B}\enc{"9C}\enc{"9D}\enc{"9E}\enc{"9F}
      \enc{"A0}\enc{"A1}\enc{"A2}\enc{"A3}\enc{"A4}\enc{"A5}\enc{"A6}\enc{"A7}%
      \enc{"A8}\enc{"A9}\enc{"AA}\enc{"AB}\enc{"AC}\enc{"AD}\enc{"AE}\enc{"AF}
      \enc{"B0}\enc{"B1}\enc{"B2}\enc{"B3}\enc{"B4}\enc{"B5}\enc{"B6}\enc{"B7}%
      \enc{"B8}\enc{"B9}\enc{"BA}\enc{"BB}\enc{"BC}\enc{"BD}\enc{"BE}\enc{"BF}
      \enc{"C0}\enc{"C1}\enc{"C2}\enc{"C3}\enc{"C4}\enc{"C5}\enc{"C6}\enc{"C7}%
      \enc{"C8}\enc{"C9}\enc{"CA}\enc{"CB}\enc{"CC}\enc{"CD}\enc{"CE}\enc{"CF}
      \enc{"D0}\enc{"D1}\enc{"D2}\enc{"D3}\enc{"D4}\enc{"D5}\enc{"D6}\enc{"D7}%"
      \enc{"D8}\enc{"D9}\enc{"DA}\enc{"DB}\enc{"DC}\enc{"DD}\enc{"DE}
      \enc{"E0}\enc{"E1}\enc{"E2}\enc{"E3}\enc{"E4}\enc{"E5}\enc{"E6}\enc{"E7}%
      \enc{"E8}\enc{"E9}\enc{"EA}\enc{"EB}\enc{"EC}\enc{"ED}\enc{"EE}\enc{"EF}
      \enc{"F0}\enc{"F1}\enc{"F2}\enc{"F3}\enc{"F4}\enc{"F5}\enc{"F6}\enc{"F7}%
      \enc{"F8}\enc{"F9}\enc{"FA}\enc{"FB}\enc{"FC}\enc{"FD}\enc{"FE}\enc{"FF}

Translation

.. container:: xml_out

   ::

      ␣!"#$%&'()*+,-./
      0123456789:;<=>?
      @ABCDEFGHIJKLMNO
      PQRSTUVWXYZ[\]^_
      `abcdefghijklmno
      pqrstuvwxyz{|}~-
      ĂĄĆČĎĚĘĞĹĽŁŇŃŊŐŔ
      ŘŚŠŞŤŢŰŮŸŹŽŻĲİđ§
      ăąćčďěęğĺľłńňŋőŕ
      řśšşťţűůÿźžżĳ¡¿£
      ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ
      ÐÑÒÓÔÕÖŒØÙÚÛÜÝÞ
      àáâãäåæçèéêëìíîï
      ðñòóôõöœøùúûüýþß

The command ``\OT2-encoding`` (and its alias ``\wncyr-encoding``)
describes the OT2 (cyrillic) encoding. This is a 7bit alphabet, and we
show all the 128 characters, in order. Some of these characters are
accents, hence placed on the character that precedes (or in the margin).

.. container:: xml_out

   ::

      ЊЉЏЭІЄЂЋњљџэієђћ
      ЮЖЙЁѴѲЅЯюжйёѵѳѕя
      ̈!”Ѣ̆%́’()*ѣ,-./
      0123456789:;«¹»?
      ̆АБЦДЕФГХИЈКЛМНО
      ПЧРСТУВЩШЫЗ[“]ЬЪ
      ‘абцдефгхијклмно
      пчрстувщшыз-—№ьъ

The command ``\OT1-encoding`` describes the OT1 encoding. This is a 7bit
alphabet, and some slots contain accent and ligatures; they are not
implemented, and marked by a red x in the table that follows.

.. container:: xml_out

   ::

      ΓΔΘΛΞΠΣΥΦΨΩxxxxx
      xxxxxxxxxßæœøÆŒØ
      x!"#$%&'()*+,-./
      0123456789:;¡=¿?
      @ABCDEFGHIJKLMNO
      PQRSTUVWXYZ[x]xx
      `abcdefghijklmno
      pqrstuvwxyzxxxxx

Needs to be done
================

Not all characters are available. In particular cyrillic letters are
missing. Some characters, like ``\'\AE`` are recognized, but not shown
in the tables.

.. |Ascii Table| image:: /img/img_69.png
.. |latin 1, non ascii| image:: /img/img_70.png
.. |latin extended A| image:: /img/img_68.png
.. |latin extended additional| image:: /img/img_92.png
.. |latin extended additional 2| image:: /img/img_93.png
.. |Zapf Dingbats characters| image:: /img/img_71.png
.. |Unicode Arrows| image:: /img/img_126.png
.. |Unicode Mathematical operators| image:: /img/img_127.png
.. |Unicode Miscellaneous symbols| image:: /img/img_128.png
.. |Unicode Mathematical alphanumeric symbols| image:: /img/img_129.png
.. |Unicode Mathematical alphanumeric symbols 2| image:: /img/img_130.png
.. |Unicode Mathematical alphanumeric symbols 3| image:: /img/img_131.png
.. |Unicode Mathematical alphanumeric symbols 4| image:: /img/img_132.png
