<?xml version="1.0" encoding="iso-8859-1"?>

<!--  feuille de style Tralics pour generer du HTML 

  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->
<!-- $Id: cls.xsl,v 1.30 2008/09/16 08:46:47 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns="http://www.w3.org/1999/xhtml" 
  exclude-result-prefixes="m" >


<!-- Should be set by the caller --> 
<xsl:variable name="author" />
<xsl:variable name="title" />
<xsl:variable name="equationnumber">
   <xsl:value-of select="/std/@equation-number"/></xsl:variable>
<xsl:variable name="keywords">BIG</xsl:variable>

<xsl:variable name="TOCname">Table of Contents</xsl:variable>
<xsl:variable name="Bibliographyname">Bibliography</xsl:variable>
<xsl:variable name="shorttoc">maybe</xsl:variable>

<!-- Can be false: no split, true: only in main matter, all: all chapters
     If split is not false, we can generate more than one HTML file, 
     and we must put the file-name in the HREF of an anchor --> 
<xsl:variable name="split">false</xsl:variable>

<!-- Name of the main file and prefix; should be set by the caller --> 
<xsl:variable name="Main">Main</xsl:variable>
<xsl:variable name="index">index</xsl:variable>
<xsl:variable name="glossary">glossary</xsl:variable>
<xsl:variable name="separate-index">false</xsl:variable>
<xsl:variable name="separate-glossary">false</xsl:variable>
<xsl:variable name="separate-biblio">false</xsl:variable>
<xsl:variable name="separate-footnote">false</xsl:variable>
<xsl:variable name="prefix">prefix</xsl:variable>


<!-- The $Main variable is only used here  -->
<xsl:template name="mainfile">
  <xsl:if test="not($split='false') ">
    <xsl:value-of select="concat($Main,'.html')" />
  </xsl:if>
</xsl:template>

<!-- If no separate index, then empty   -->
<xsl:template name="indexfile">
  <xsl:choose>
    <xsl:when test="not ($split='false') and $separate-index='true' ">
      <xsl:value-of select="concat($prefix,'uidI','.html')" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="mainfile" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="glossaryfile">
  <xsl:choose>
    <xsl:when test="not ($split='false') and $separate-glossary='true' ">
      <xsl:value-of select="concat($prefix,'uidG','.html')" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="mainfile" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="footnotefile">
  <xsl:choose>
    <xsl:when test=" not ($split='false') and  $separate-footnote='true' ">
      <xsl:value-of select="concat($prefix,'uidF','.html')" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="mainfile" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="bibliofile">
  <xsl:choose>
    <xsl:when test="not ($split='false') and $separate-biblio='true' ">
      <xsl:value-of select="concat($prefix,'uidH','.html')" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="mainfile" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="first.page" >
  <xsl:call-template name="mainfile" />
  <xsl:choose>
    <xsl:when test="//mainmatter">
      <xsl:text>#mainmatter</xsl:text>
    </xsl:when>
  </xsl:choose>
</xsl:template>

<xsl:template name="last.page" >
  <xsl:call-template name="mainfile" />
  <xsl:choose>
    <xsl:when test="//backmatter">
      <xsl:text>#backmatter</xsl:text>
    </xsl:when>
    <xsl:when test="//biblio and not($separate-biblio='false')">
      <xsl:text>#bibliography</xsl:text>
    </xsl:when>
  </xsl:choose>
</xsl:template>

<!-- Returns the name of the HTML file associated to the chapter
     in case there is a file. 
     The variable $prefix is used only here and in the xsl:document below -->
<xsl:template match="div0" mode="fileprefix">
  <xsl:value-of select="concat($prefix,@id,'.html')" />
</xsl:template>

<!-- Prints back to main  -->
<xsl:template name="back.to.main">
  <xsl:variable name="mainpage">
    <xsl:call-template name ="mainfile" />
  </xsl:variable>
  <div class="float-center"> 
    <a href="{$mainpage}">Back to main page</a>
  </div>
</xsl:template>

<!-- Prints back to main plus standard buttns -->
<xsl:template name="back.to.main.buttons">
  <hr/>
  <xsl:call-template name="std.buttons" />
  <xsl:call-template name="back.to.main" />
</xsl:template>

<xsl:template name="title.buttons">
  <xsl:call-template name="std.buttons" />
  <div class="float-center"><xsl:call-template name="title" /></div>
</xsl:template>

<!-- Translation of div0. In normal mode, translation is in the current
     file, otherwise in the file defined by the $prefix and @id -->
<xsl:template match="div0">
  <xsl:choose>
    <xsl:when test="$split='false'">
      <xsl:apply-templates select='.' mode="normal"/>
    </xsl:when>
    <xsl:when test="$split='all'">
      <xsl:apply-templates select='.' mode="split"/>
    </xsl:when>
    <xsl:when test="ancestor::mainmatter">
      <xsl:apply-templates select='.' mode="split"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select='.' mode="normal"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<!-- Returns the name of the HTML file associated to the target. 
     Must match the algorithm shown above
-->

<xsl:template match="*" mode="targetfile">
  <xsl:choose>
    <xsl:when test="$split='false'" />
    <xsl:when test="not(ancestor-or-self::div0)">
      <xsl:call-template name="mainfile" />
    </xsl:when>
    <xsl:when test="(ancestor::mainmatter) or $split='all'">
      <xsl:apply-templates select="ancestor-or-self::div0" mode="fileprefix" />
    </xsl:when>
   <xsl:otherwise>
      <xsl:call-template name="mainfile" />
    </xsl:otherwise>
  </xsl:choose> 
</xsl:template>


<!-- In the file associated to chapter N, we have a link to the previous 
     and next chapter. In there is no previous chapter, we point to the 
     mainmatter (if any) or the start of the file; 
     if there is no next chapter we point to backmatter (or bibliography)
 -->
<xsl:template match="div0" mode="file-prev-prefix">
  <xsl:choose>
    <xsl:when test="preceding-sibling::div0">
      <xsl:apply-templates select="preceding-sibling::div0[1]" mode="fileprefix"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="first.page" />
    </xsl:otherwise>
  </xsl:choose> 
</xsl:template>



<!-- Returns the name of the HTML file associated to the next chapter-->
<xsl:template match="div0" mode="file-next-prefix">
  <xsl:choose>
    <xsl:when test="following-sibling::div0">
      <xsl:apply-templates select="following-sibling::div0[1]" mode="fileprefix"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="last.page" />
    </xsl:otherwise>
  </xsl:choose> 
</xsl:template>


<!-- This is called when we point to a link-->
<xsl:template name="id-to-href">
  <xsl:attribute name="href">
    <xsl:apply-templates mode="targetfile" select="id(@id)" />
    <xsl:text>#</xsl:text><xsl:value-of select="./@id"/>
  </xsl:attribute>
</xsl:template>
  
<xsl:template name="target-to-href">
  <xsl:attribute name="href">
    <xsl:apply-templates mode="targetfile" select="id(@target)" />
    <xsl:text>#</xsl:text><xsl:value-of select="@target"/>
  </xsl:attribute>
</xsl:template>


<!-- Notes are a bit more complicated. Normal link is from chapter to
     Main, a backlink from Main to chapter.
-->
<xsl:template name="note-to-href">
  <xsl:attribute name="href">
    <xsl:call-template name="footnotefile" />
    <xsl:text>#note</xsl:text>
    <xsl:apply-templates mode="xref" select ="."/>
  </xsl:attribute>
</xsl:template>
  
<xsl:template name="id">
  <xsl:if test="./@id">
    <xsl:attribute name="id">
      <xsl:value-of select="./@id"/>
    </xsl:attribute>
  </xsl:if>
</xsl:template>


<xsl:template name="separateur.objet">
 <xsl:choose>
  <xsl:when test="position()!=last()">, </xsl:when>
  <xsl:when test="position()=last()">.</xsl:when>
 </xsl:choose>
</xsl:template>


<xsl:template name="separateur.objet.spec">
 <xsl:choose>
  <xsl:when test="position()!=last()">, </xsl:when>
 </xsl:choose>
</xsl:template>


<!-- enable, for new elements to get commented as such-->
<xsl:template match="*">
  <xsl:comment><xsl:text>PASS THROUGH </xsl:text>
    <xsl:value-of select="name()"/>
  </xsl:comment>
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="vbnumber">
  <span class='prenumber'>
    <xsl:apply-templates/>
  </span>
</xsl:template>

<!-- everything with an ID in xreftitle mode; title of the link -->

<xsl:template mode="xreftitle" match="div0|div1|div2|div3|div4|div5">
  <xsl:attribute name="title">
     <xsl:apply-templates select="head" mode="caption"/>
  </xsl:attribute>
</xsl:template>

<xsl:template mode="xreftitle" match="citation">
  <xsl:attribute name="title">
    <xsl:apply-templates select="bauteurs"  mode="xref-fullauthor"/>
    <xsl:apply-templates select="byear"  mode="xref-x"/>
  </xsl:attribute>
</xsl:template>

<!-- In the case of a footnote or a bibitem, we put the whole content in
     the title field, truncate to 100 if the text is too long -->
<xsl:template mode="xreftitle" match="note|bibitem">
  <xsl:variable name="text" select="string(normalize-space(.))" />
  <xsl:attribute name="title">
    <xsl:choose>
      <xsl:when test="string-length($text) &gt; 110">
	<xsl:value-of select="substring($text,1,100)"/>
	<xsl:text>...</xsl:text>
      </xsl:when>
      <xsl:otherwise>
	<xsl:value-of select="$text"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:attribute>
</xsl:template>

<!-- Empty production if no title -->

<xsl:template mode="xreftitle" match="p|item|anchor|table|figure|formula|theorem">
</xsl:template>

<xsl:template match="bibitem" mode="xref">
  <xsl:choose>
    <xsl:when test="@bib-key">
      <xsl:value-of select="@bib-key" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template  name="calculatebibitem"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<!--Font changes --> 
<xsl:template match="hi|TeX|LaTeX" mode ="caption">
  <xsl:apply-templates select="."/>
</xsl:template>

<xsl:template match="hi">
  <xsl:choose>
   <xsl:when test="@rend = 'sup'">
     <sup><xsl:apply-templates/></sup>
   </xsl:when>
   <xsl:when test="@rend = 'sub'">
     <sub><xsl:apply-templates/></sub>
   </xsl:when>
   <xsl:when test="@rend = 'bold'">
     <b><xsl:apply-templates/></b>
   </xsl:when>
   <xsl:when test="@rend = 'small'">
     <xsl:choose>
       <xsl:when test="ancestor::pre">
	 <xsl:apply-templates/>
       </xsl:when>
       <xsl:otherwise>
	 <small><xsl:apply-templates/></small> 
       </xsl:otherwise>
     </xsl:choose>
   </xsl:when>
   <xsl:when test="@rend = 'sc'">
      <span class="smallcap"> <xsl:apply-templates/> </span>
   </xsl:when>
   <xsl:when test="@rend = 'sansserif'">
      <span class="sansserif"> <xsl:apply-templates/> </span>
   </xsl:when>
   <xsl:when test="@rend = 'large'">
      <big><xsl:apply-templates/></big>
   </xsl:when>
   <xsl:when test="@rend = 'center'">
      <span class="hc"><xsl:apply-templates/></span>
   </xsl:when>
   <xsl:when test="@rend = 'underline'">
     <span style="text-decoration:underline"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'overline'">
     <span style="text-decoration:overline"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'ul'">
     <span style="text-decoration:underline"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'st'">
     <span style="text-decoration:line-through"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'hl'">
     <span style="background:red"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'so'">
     <span style="letter-spacing:0.1em"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'caps'">
     <span style="letter-spacing:0.1em"><xsl:apply-templates/></span> 
   </xsl:when>
   <xsl:when test="@rend = 'tt'">
     <xsl:choose>
       <xsl:when test="ancestor::pre">
	 <xsl:apply-templates/>
       </xsl:when>
       <xsl:otherwise>
	 <tt><xsl:apply-templates/></tt> 
       </xsl:otherwise>
     </xsl:choose>
   </xsl:when>
   <xsl:otherwise>
     <i><xsl:apply-templates/></i>
   </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="spaces">
  <xsl:text>&#xA0;&#xA0;&#xA0;&#xA0;&#xA0;</xsl:text>
</xsl:template>


<xsl:template mode="xref" match="div0|div1|div2|div3|div4|div5">
  <xsl:call-template name="calculateNumber"/>
</xsl:template>

<xsl:template match="div0" mode="prefix">
</xsl:template>

<xsl:template match="div1" mode="prefix">
  <xsl:call-template name="spaces"/>
</xsl:template>

<xsl:template match="div2" mode="prefix">
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
</xsl:template>


<xsl:template match="div3" mode="prefix">
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
</xsl:template>


<xsl:template match="div4" mode="prefix">
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
</xsl:template>

<xsl:template match="div5" mode="prefix">
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
  <xsl:call-template name="spaces"/>
</xsl:template>


<xsl:template match="div0" mode="xtoc">
  <xsl:text>&#x0A;</xsl:text>
  <br/>
  <b>
    <xsl:call-template name="calculateNumberSpace"/>
    <a>
      <xsl:call-template name="id-to-href"/>
      <xsl:apply-templates select="head" mode="caption"/>
    </a>
  </b>
</xsl:template>

<xsl:template match="div1|div2|div3|div4|div5" mode="xtoc">
  <xsl:text>&#x0A;</xsl:text>
  <br/>
  <xsl:apply-templates mode="prefix" select="."/>
  <xsl:call-template name="calculateNumberSpace"/>
  <a>
    <xsl:call-template name="id-to-href"/>
    <xsl:apply-templates select="head" mode="caption"/>
  </a>
</xsl:template>

<!-- Returns the name of the HTML file associated to the previous chapter
with an anchor in the case it is the current file-->
<xsl:template match="div0" mode="button-prev-prefix">
  <a accesskey='P' title="previous page">
    <xsl:attribute name='href'>
      <xsl:apply-templates select="." mode ="file-prev-prefix" />
    </xsl:attribute>
    <img  style="border:0"  alt="previous" src="previous_motif.gif" />
  </a>
</xsl:template>

<xsl:template match="div0" mode="button-next-prefix">
  <a accesskey='N'  title="next page">
    <xsl:attribute name='href'>
      <xsl:apply-templates select="." mode ="file-next-prefix" />
    </xsl:attribute>
    <img  style="border:0" alt="next" src="next_motif.gif" />
  </a>
</xsl:template>

<xsl:template name="std.buttons">
  <div class="float-left">
    <a accesskey='P' title="previous page">
      <xsl:attribute name='href'>
	<xsl:call-template name="first.page" />
      </xsl:attribute>
      <img  style="border:0"  alt="previous" src="previous_motif.gif" />
    </a>
  </div> 
  <div class="float-left">
    <a accesskey='T' title="table of contents" >
      <xsl:attribute name='href'>
	<xsl:call-template name ="mainfile" />
	<xsl:text>#tableofcontents</xsl:text>
      </xsl:attribute>
      <img style="border:0"  alt="TOC" src="contents_motif.gif" />
    </a>
  </div> 
  <div class="float-right">
    <a accesskey='N'  title="next page">
      <xsl:attribute name='href'>
	<xsl:call-template name="last.page" />
      </xsl:attribute>
      <img  style="border:0" alt="next" src="next_motif.gif" />
    </a>
  </div> 
</xsl:template>

<xsl:template match="div0" mode="button">
  <div class="float-left">
    <xsl:apply-templates select="." mode ="button-prev-prefix" />
  </div> 
  <div class="float-left">
    <a accesskey='T' title="table of contents" >
      <xsl:attribute name='href'>
	<xsl:call-template name ="mainfile" />
	<xsl:text>#tableofcontents</xsl:text>
      </xsl:attribute>
      <img style="border:0"  alt="TOC" src="contents_motif.gif" />
    </a>
  </div> 
  <div class="float-right">
    <xsl:apply-templates select="." mode ="button-next-prefix" />
  </div>
</xsl:template>

<!-- A chapter -->
<xsl:template match="div0" mode="normal">
  <h1>
     <xsl:call-template name="id"/>
     <xsl:call-template name="calculateNumberSpace"/>
     <xsl:apply-templates select="head" mode="full"/> 
  </h1>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div0" mode ="split">
  <h1>
     <xsl:call-template name="id"/>
     <xsl:call-template name="calculateNumberSpace"/>
     <a> 
       <xsl:attribute name="href">
	 <xsl:apply-templates select="." mode="fileprefix" />
       </xsl:attribute>
       <xsl:apply-templates select="head" mode="caption"/> 
    </a>
  </h1>
  <xsl:apply-templates select="." mode="xsplit"/> 
</xsl:template>


<xsl:template match="div0" mode ="xsplit">
  <xsl:variable name="filename">
    <xsl:apply-templates select="." mode="fileprefix" />
  </xsl:variable>
  <xsl:document href="{$filename}" 
		encoding='iso-8859-1' doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
		doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
		method="xml" >
    <html>
      <xsl:call-template name="html-meta" />
      <body>
	<xsl:apply-templates select="." mode="button" />
	<div class="float-center"><xsl:call-template name="title" /></div>
	<h1>
	  <xsl:call-template name="id"/>
	  <xsl:call-template name="calculateNumberSpace"/>
	  <xsl:apply-templates select="head" mode="full"/> 
	</h1>
	<xsl:apply-templates/>
	<xsl:apply-templates select="." mode="button" />
	<xsl:call-template name="back.to.main" />
      </body>
    </html>
  </xsl:document>
</xsl:template>

<xsl:template name="separate.page">
  <xsl:param name="filename" />
  <xsl:document href="{$filename}" encoding='iso-8859-1' 
		doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
		doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
		method="xml" >
    <html>
      <xsl:call-template name="html-meta" />
      <body>
	<xsl:call-template name="title.buttons" />
	<xsl:apply-templates select="." mode="ysplit" />
	<xsl:call-template name="back.to.main.buttons" />
      </body>
    </html>
  </xsl:document>
</xsl:template>

<xsl:template match="theindex" mode ="xsplit">
  <xsl:variable name="filename">
    <xsl:call-template name="indexfile" />
  </xsl:variable>
  <xsl:call-template name="separate.page">
    <xsl:with-param select="$filename" name="filename"/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="theglossary" mode ="xsplit">
  <xsl:variable name="filename">
    <xsl:call-template name="glossaryfile" />
  </xsl:variable>
  <xsl:call-template name="separate.page">
    <xsl:with-param select="$filename" name="filename"/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="biblio" mode="xsplit">
  <xsl:variable name="filename">
    <xsl:call-template name="bibliofile" />
  </xsl:variable>
  <xsl:call-template name="separate.page">
    <xsl:with-param select="$filename" name="filename"/>
  </xsl:call-template>
</xsl:template>

<xsl:template match="frontmatter">
  <hr/>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="mainmatter">
  <hr id="mainmatter" />
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="backmatter">
  <hr id="backmatter"/>
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="div1">
  <h2> 
    <xsl:call-template name="id"/>
    <xsl:call-template name="calculateNumberSpace"/>
    <xsl:apply-templates select="head" mode="full"/> 
  </h2>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div2">
  <h3> 
    <xsl:call-template name="id"/>
    <xsl:call-template name="calculateNumberSpace"/>
    <xsl:apply-templates select="head" mode="full"/> 
  </h3>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div3">
  <h4>
    <xsl:call-template name="id"/>
    <xsl:call-template name="calculateNumberSpace"/>
    <xsl:apply-templates select="head" mode="full"/> 
  </h4>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div4"> 
  <h5>
    <xsl:call-template name="id"/>
    <xsl:call-template name="calculateNumberSpace"/>
    <xsl:apply-templates select="head" mode="full"/>
  </h5>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div5"> 
  <h6>
    <xsl:call-template name="id"/>
    <xsl:call-template name="calculateNumberSpace"/>
    <xsl:apply-templates select="head" mode="full"/>
  </h6>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="head" mode="caption">
  <xsl:apply-templates mode="caption"/>
</xsl:template>

<xsl:template match="head" mode="full">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="item" mode="xref">
  <xsl:choose>
    <xsl:when test="@label">
      <xsl:value-of select="@label" />
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="calculateItemNumber"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="head"/>

<xsl:template match="anchor">
  <a> 
    <xsl:call-template name="id"/>
  </a>
</xsl:template>

<xsl:template match="anchor" mode="caption" />

<xsl:template match="anchor" mode ='xref'>
  <xsl:text>&#x273B;</xsl:text>
</xsl:template>

<xsl:template match="xref">
  <a> 
    <xsl:attribute name="href">
      <xsl:value-of select="@url"/>
    </xsl:attribute>
    <xsl:apply-templates/>
  </a>
</xsl:template>

<xsl:template match="unused-theindex">
  <h1 id="index">Index</h1>
  <p class='noindent nofirst'>
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="theglossary">
 <xsl:choose>
   <xsl:when test="not($split='false') and $separate-glossary='true' ">
     <xsl:apply-templates select="." mode="xsplit" />
   </xsl:when>
   <xsl:otherwise>
     <xsl:apply-templates select="." mode="ysplit" />
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>


<xsl:template match="theindex">
 <xsl:choose>
   <xsl:when test="not($split='false') and $separate-index='true' ">
     <xsl:apply-templates select="." mode="xsplit" />
   </xsl:when>
   <xsl:otherwise>
     <xsl:apply-templates select="." mode="ysplit" />
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="index.name">
  <xsl:choose>
    <xsl:when test="@title=''">Index</xsl:when>
    <xsl:otherwise><xsl:value-of select="@title"/></xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="glossary.name">
  <xsl:choose>
    <xsl:when test="@title=''">Glossary</xsl:when>
    <xsl:otherwise><xsl:value-of select="@title"/></xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="theindex" mode="ysplit">
  <h1>
    <xsl:attribute name="id">
      <xsl:text>index</xsl:text> 
      <xsl:call-template name="calculateindex"/>
    </xsl:attribute>
    <xsl:call-template name="index.name"/>
   </h1>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="theglossary" mode="ysplit">
  <h1 id="glossary"><xsl:call-template name="glossary.name"/></h1>
  <xsl:apply-templates/>
</xsl:template>

<!-- used only in the TOC; We may have more than one index -->
<xsl:template match="theindex" mode="xtoc">
  <xsl:variable name="link">
    <xsl:call-template name="indexfile" />
    <xsl:text>#index</xsl:text>
    <xsl:call-template name="calculateindex"/>
  </xsl:variable>
  <xsl:text>&#x0A;</xsl:text> <br/>
  <a href="{$link}"> <b> <xsl:call-template name="index.name"/></b></a>
</xsl:template>

<xsl:template match="theglossary" mode="xtoc">
  <xsl:variable name="link">
    <xsl:call-template name="glossaryfile" />
    <xsl:text>#glossary</xsl:text>
  </xsl:variable>
  <xsl:text>&#x0A;</xsl:text> <br/>
  <a href="{$link}"> <b><xsl:call-template name="index.name"/></b></a>
</xsl:template>

<xsl:template match="index">
  <xsl:text>&#x0A;</xsl:text>
<!--  <xsl:if test="position()!=1"> <br/> </xsl:if> -->
  <div class="idx{@level}"> 
   <xsl:apply-templates/>
   <xsl:for-each select="id(@target)">
     <xsl:if test="position()=1">: </xsl:if>
     <a>
       <xsl:call-template name="id-to-href"/>
       <xsl:apply-templates mode="xref" select="."/>
     </a>
     <xsl:call-template name="separateur.objet"/>
   </xsl:for-each>
  </div>
</xsl:template>

<!-- bibliography ============================================= -->


<xsl:template match="xbibitem" mode="xref">
  <xsl:call-template  name="calculatebibitem"/>
</xsl:template>


<xsl:template name="calculatebibitem">
  <xsl:number level="any" count="bibitem"/>
</xsl:template>

<xsl:template name="calculateindex">
  <xsl:number from = "/" level="any" count="theindex"/>
</xsl:template>




<xsl:template match="biblio">
 <xsl:choose>
   <xsl:when test="not($split='false') and $separate-biblio='true' ">
     <xsl:apply-templates select="." mode="xsplit" />
   </xsl:when>
   <xsl:otherwise>
     <xsl:apply-templates select="." mode="ysplit" />
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="footnotes">
 <xsl:choose>
   <xsl:when test="not($split='false') and $separate-footnote='true' ">
     <xsl:call-template name="footnotes-xsplit" />
   </xsl:when>
   <xsl:otherwise>
     <xsl:call-template name="footnotes-ysplit" />
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="footnotes-xsplit">
  <xsl:variable name="filename">
    <xsl:call-template name="footnotefile" />
  </xsl:variable>
  <xsl:document href="{$filename}" encoding='iso-8859-1' 
		doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
		doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
		method="xml" >
    <html>
      <xsl:call-template name="html-meta" />
      <body>
	<xsl:call-template name="title.buttons" />
	<xsl:call-template name="footnotes-ysplit" />
	<hr />
	<xsl:call-template name="std.buttons" />
	<xsl:call-template name="back.to.main.buttons" />
      </body>
    </html>
  </xsl:document>
</xsl:template>

<xsl:template name="footnotes-ysplit">
  <h1>Notes</h1> 
  <xsl:for-each select="//note">
    <xsl:apply-templates select="." mode="footnote"/>
  </xsl:for-each>
</xsl:template>

<xsl:template match="biblio" mode = "ysplit">
  <h1 id="bibliography"><xsl:value-of select="$Bibliographyname"/></h1>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="biblio" mode="xtoc">
  <xsl:variable name="link">
    <xsl:call-template name="bibliofile" />
    <xsl:text>#bibliography</xsl:text>
  </xsl:variable>
  <xsl:text>&#x0A;</xsl:text> <br/>
  <a href="{$link}">
    <b><xsl:value-of select="$Bibliographyname"/></b>
  </a>
</xsl:template>

<!-- Translate <cit atts> <ref/> </cit>
   We have a single <ref>, with an @id.
 -->
<xsl:template match="cit"> 
  <a>
    <xsl:apply-templates mode="cit" select="ref" />
    <xsl:choose>
      <xsl:when test="@type ='fullauthor'">
	<xsl:apply-templates mode="xref-fullauthor" />
      </xsl:when>
      <xsl:when test="@type ='year'">
	<xsl:apply-templates mode="xref-year" />
      </xsl:when>
      <xsl:otherwise>
	<xsl:text>[</xsl:text>    
	<xsl:apply-templates mode="xref" />
	<xsl:apply-templates mode="citetext" />
	<xsl:text>]</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
  </a>
</xsl:template>

<xsl:template match="Cit"> 
  <xsl:apply-templates />
</xsl:template>

<xsl:template match="ref" mode="cit"> 
  <xsl:call-template name="target-to-href"/> 
  <xsl:apply-templates mode="xreftitle" select="id(@target)" />
</xsl:template>

<xsl:template match="bibitem" mode="cit"> 
  <xsl:if test="@bib-key">
    <xsl:text>[</xsl:text>
    <xsl:value-of select="@bib-key" />
    <xsl:text>] okok</xsl:text>
  </xsl:if>
</xsl:template>


<xsl:template match="ref" mode="xref">
  <xsl:apply-templates mode="xref" select="id(@target)" />
</xsl:template>

<!-- ref is a cit in text, output a comma if there is some text -->
<xsl:template match="ref" mode="citetext">
  <xsl:if test="string(normalize-space(.))">
    <xsl:text>, </xsl:text>
    <xsl:apply-templates/>
  </xsl:if>
</xsl:template>

<!-- In the case of <cit><ref id='x'>FOO</ref/></cit>, the translation of the
<ref/> is just FOO. Everything else done by the caller  -->
<xsl:template match="cit/ref">
  <xsl:apply-templates />
</xsl:template>

<!-- In the case of <Cit><ref id='x'>FOO</ref/></Cit>, the translation of the
<ref/> is just FOO. Everything else done by the caller -->

<xsl:template match="Cit/ref">
  <a>
  <xsl:call-template name="target-to-href"/> 
  <xsl:apply-templates mode="xref" select="id(@target)" />
    <xsl:apply-templates />
  </a>
</xsl:template>

<xsl:template match="ref" mode="xref-fullauthor">
  <xsl:apply-templates mode="xref-fullauthor" select="id(@target)" />
</xsl:template>

<xsl:template match="ref" mode="xref-year">
  <xsl:apply-templates mode="xref-year" select="id(@target)" />
</xsl:template>

<xsl:template match="ref">
  <a>
    <xsl:call-template name="target-to-href"/> 
    <xsl:apply-templates mode="xreftitle" select="id(@target)" />
    <xsl:apply-templates mode="xref" select="id(@target)" />
  </a>
</xsl:template>

<xsl:template match="citation" mode="xref">
  <xsl:number from ="/" level="any" count="citation" />
</xsl:template>

<xsl:template match="citation" mode="xref-year">
  <xsl:apply-templates select="byear" mode="xref-x"/>
</xsl:template>

<xsl:template match="citation" mode="xref-fullauthor">
  <xsl:apply-templates select="bauteurs"  mode="xref-fullauthor"/>
</xsl:template>


<xsl:template match="citation">
  <p class='noindent nofirst'>
    <xsl:call-template name="id"/>
    <xsl:text>[</xsl:text> 
    <xsl:apply-templates mode="xref" select="." />
    <xsl:text>] </xsl:text>
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="bibitem">
  <span class='noindent nofirst'>
    <xsl:call-template name="id"/>
    <xsl:text>[</xsl:text> 
    <xsl:apply-templates mode="xref" select="." />
    <xsl:text>] </xsl:text>
    <xsl:apply-templates/>
  </span>
</xsl:template>


<xsl:template match='bvolume|bnumber|bpages|bchapter|bseries|bedition'>
  <xsl:value-of select="@bname"/>
  <xsl:text> </xsl:text>
  <xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='btitle'>
  <i>
    <xsl:apply-templates/>
    <xsl:text>. </xsl:text>
  </i> 
</xsl:template>


<xsl:template match='bjournal|bbooktitle'>
  <xsl:text>in &#xAB;&#xA0;</xsl:text>
  <xsl:apply-templates/>
  <xsl:text>&#xA0;&#xBB;</xsl:text>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>


<xsl:template match='byear|bmonth|btype|bschool|bpublisher|
		     bnote|borganization|binstitution|baddress|bhowpublished'>
  <xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='byear|bmonth|btype|bschool|bpublisher|
		     bnote|borganization|binstitution|baddress|bhowpublished'
	      mode="xref-x">
  <xsl:apply-templates/>
</xsl:template>



<xsl:template match ="bauteurs">
  <span class="smallcap">
     <xsl:for-each select="bpers|etal">
        <xsl:value-of select="@prenomcomplet"/> 
        <xsl:text> </xsl:text>
        <xsl:if test="@part">
           <xsl:value-of select="@part"/>
           <xsl:text> </xsl:text>
        </xsl:if>
        <xsl:value-of select="@nom"/>
        <xsl:call-template name="separateur.objet"/>
     </xsl:for-each>
   </span> 
   <xsl:text> </xsl:text> 
</xsl:template>


<xsl:template match ="bauteurs"  mode="xref-fullauthor">
  <xsl:for-each select="bpers">
    <xsl:value-of select="@nom"/>
    <xsl:call-template name="separateur.objet.spec"/>
  </xsl:for-each>
</xsl:template>

<xsl:template match ="bediteur">
  <span class="smallcap">
     <xsl:for-each select="bpers|etal">
        <xsl:value-of select="@prenom"/> 
        <xsl:text> </xsl:text>
        <xsl:if test="@part">
           <xsl:text> </xsl:text>
           <xsl:value-of select="@part"/>
        </xsl:if>
        <xsl:value-of select="@nom"/>
        <xsl:text>, </xsl:text>
     </xsl:for-each>
   </span> 
   <xsl:choose>
    <xsl:when test="count(bpers|etal) != 1">
       <xsl:text>editors</xsl:text> 
    </xsl:when>
    <xsl:otherwise>
      <xsl:text>editor</xsl:text> 
    </xsl:otherwise>
   </xsl:choose>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<!-- Tables -->

<!-- TABLE -->
<xsl:template match='table' mode="xref">
  <xsl:call-template name="calculateTableNumber"/>
</xsl:template>


<xsl:template name="calculateTableNumber">
  <xsl:number count="table[ @rend != 'inline']" level="any"/>
</xsl:template>


<xsl:template match="table[@rend='inline']">
 <xsl:call-template name='generate-table'/>
</xsl:template>


<xsl:template match="table[@rend='array']">
 <div class='hc'>  
    <xsl:call-template name="id"/>
   <p> <strong>
        <xsl:text>Table </xsl:text>
        <xsl:call-template name="calculateTableNumber" />
        <xsl:text>. </xsl:text>
      </strong>
      <xsl:apply-templates select='head' mode='caption'/>
   </p>
    <xsl:apply-templates select="p/table"/>

  </div>

</xsl:template>


<xsl:template match="table">
  <div class="hc" >
    <xsl:call-template name='generate-table'/>
  </div>
</xsl:template>

<xsl:template match="head" mode="intable">
    <caption>
      <strong>
        <xsl:text>Table </xsl:text>
        <xsl:call-template name="calculateTableNumber" />
        <xsl:text>. </xsl:text>
      </strong>
      <xsl:apply-templates select='.' mode='caption'/>
    </caption>
</xsl:template>

<xsl:template name="generate-table">
  <table align="center" style="caption-side:bottom; border-collapse: collapse; border-width:1px;">
    <xsl:call-template name="id"/>
    <xsl:apply-templates select="head" mode="intable" />
    <xsl:apply-templates select="row" />
  </table>
</xsl:template>

<xsl:template match="row">
   <tr>
      <xsl:apply-templates/>
   </tr>
</xsl:template>


<xsl:template match="cell">
   <xsl:element name="td">
     <xsl:call-template name="cell-atts"/>
      <xsl:if test="./@cols>1">
       <xsl:attribute name="colspan"><xsl:value-of select="./@cols" /></xsl:attribute>
     </xsl:if>
     <xsl:if test="./@rows>1">
       <xsl:attribute name="rowspan"><xsl:value-of select="./@rows" /></xsl:attribute>
     </xsl:if>
     <xsl:apply-templates/>
   </xsl:element>
</xsl:template>

<xsl:template match="@halign">
   <xsl:attribute name="halign"><xsl:value-of select="." /></xsl:attribute>
</xsl:template>

<xsl:template name="cell-atts">
  <xsl:if test="(@halign !='') or ( @bottom-border='true') or
		(@top-border='true') or ( @left-border='true') or 
                (parent::row/@bottom-border='true') or
                (parent::row/@top-border='true') or
                (parent::row/@left-border='true') or
                (parent::row/@right-border='true') or
	        (@right-border='true')">
  <xsl:attribute name="WW">
<xsl:value-of select="parent::row/@bottom-border"/>;
  </xsl:attribute>
    <xsl:attribute name="style">
      <xsl:if test="@halign">text-align:<xsl:value-of select="@halign"/>;</xsl:if> 
      <xsl:if test="@right-border='true'">border-right:solid 1px;</xsl:if>
      <xsl:if test="@top-border='true'">border-top:solid 1px;</xsl:if> 
      <xsl:if test="@left-border='true'">border-left:solid 1px;</xsl:if>
      <xsl:if test="@bottom-border='true'">border-bottom:solid 1px;</xsl:if> 
      <xsl:if test="parent::row/@bottom-border='true'">border-bottom:solid 1px;</xsl:if> 
      <xsl:if test="parent::row/@top-border='true'">border-top:solid 1px;</xsl:if> 
      <xsl:if test="parent::row/@right-border='true'">border-right:solid 1px;</xsl:if> 
      <xsl:if test="parent::row/@left-border='true'">border-left:solid 1px;</xsl:if> 
    </xsl:attribute >
  </xsl:if> 
</xsl:template>
<!-- lists -->

<xsl:template match="list">
  <xsl:choose>
    <xsl:when test="@type='description'">
      <dl>
	<xsl:call-template name="id"/>
	<xsl:apply-templates/>
      </dl>
    </xsl:when>
    <xsl:when test="@type='ordered'">
      <ol>
	<xsl:call-template name="id"/>
	<xsl:apply-templates/>
      </ol>
    </xsl:when>
    <xsl:otherwise>
      <ul>
	<xsl:call-template name="id"/>
	<xsl:apply-templates/>
      </ul>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="item">
  <xsl:choose>
    <xsl:when test="parent::list/@type='description'">
      <dt>
	<xsl:call-template name="id"/>
	<xsl:text>(</xsl:text>
	<xsl:value-of select="@label"/>	
	<xsl:text>)</xsl:text>
      </dt>
      <dd><xsl:apply-templates/> </dd>
    </xsl:when>
    <xsl:otherwise>
      <li>
	<xsl:call-template name="id"/>
	<xsl:apply-templates/>
      </li>
    </xsl:otherwise>
</xsl:choose></xsl:template>


<!-- maths-->


<xsl:template match="m:math">
 <math  xmlns='http://www.w3.org/1998/Math/MathML'>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates mode="math"/>
 </math>
</xsl:template>

<xsl:template match="m:*|@*|text()" mode="math">
 <xsl:copy>
   <xsl:apply-templates mode="math" select="*|@*|text()"/>
 </xsl:copy>
</xsl:template>

<xsl:template match="formula" mode="xref">
  <xsl:number level="any" count="formula[@id]" />
</xsl:template>

<xsl:template match="formula">
  <xsl:variable name="eqclass">
    <xsl:choose>
      <xsl:when test="@eqleft='true'">leftequation</xsl:when>
      <xsl:otherwise></xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
 <xsl:variable name="nbleft">
   <xsl:choose>
     <xsl:when test="@eqnpos='left'">true</xsl:when>
     <xsl:when test="@eqnpos='right'">false</xsl:when>
     <xsl:when test="$equationnumber='left'">true</xsl:when>
     <xsl:otherwise>false</xsl:otherwise>
   </xsl:choose>
 </xsl:variable>
  <xsl:choose>
    <!-- Implicit equation number -->
    <xsl:when test="@type = 'display' and @id">
      <xsl:call-template name="tag-equation">
	<xsl:with-param select="$nbleft" name="leq"/>
      </xsl:call-template>
    </xsl:when>
    <!-- Explicit equation number, left aligned -->
    <xsl:when test="@type = 'display' and @tag and @eqleft='true'">
      <div>
        <table id="{@id}" width='100%'>
          <tr valign='middle'>
            <td class="leqno"/>
            <td  class='leftequation' align='left'>
              <xsl:apply-templates/>
            </td>
            <td class="eqno"><xsl:value-of select="@tag" /></td>
          </tr>
        </table>
      </div>
    </xsl:when>
    <!-- Explicit equation number -->
    <xsl:when test="@type = 'display' and @tag">
      <xsl:call-template name="tag-equation">
	<xsl:with-param select="$nbleft" name="leq"/>
      </xsl:call-template>
    </xsl:when>
    <!-- Unnumbered display; flush left -->
    <xsl:when test="@eqleft='true' and @type = 'display' " >
      <div class='mathdisplay'>
        <table with="100%">
          <tr valign='middle'>
            <td class="leqno"/>
            <td style='text-align:left'> <xsl:apply-templates/> </td>
            <td class="eqno"/>
          </tr>
        </table>
      </div>
    </xsl:when>
    <!-- Unnumbered display, center -->
    <xsl:when test="@type = 'display'">
      <div class="mathdisplay">
        <xsl:apply-templates/>
      </div>
    </xsl:when>
    <!-- Inline math -->
    <xsl:otherwise>
      <span class="math">
        <xsl:apply-templates />
      </span>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="tag-equation">
  <xsl:param name="leq" />
  <div class="mathdisplay" >
    <table width='100%' >
      <xsl:call-template name="id"/>
      <tr valign='middle'>
        <td class="leqno"> 
	  <xsl:if test="$leq='true'">
	    <xsl:call-template name="lequation"/>
          </xsl:if>
	</td>
        <td> <xsl:apply-templates/> </td>
        <td class="eqno">
	  <xsl:if test="$leq !='true'">
	    <xsl:call-template name="requation"/> 
          </xsl:if>
	</td>
      </tr>
    </table>
  </div>
</xsl:template>
  
<xsl:template name="requation">
  <xsl:if test="@tag-up">
    <xsl:attribute name="style">
      <xsl:value-of select="concat('padding-bottom:',@tag-up)"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@tag-down">
    <xsl:attribute name="style">
      <xsl:value-of select="concat('padding-top:',@tag-down)"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="@id">
      <xsl:text>(</xsl:text>
      <xsl:number level="any" count="formula[@id]" />
      <xsl:text>)</xsl:text>
    </xsl:when>
    <xsl:otherwise>
     <xsl:value-of select="@tag" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!--  Meaning of up and down exchanged -->
<xsl:template name="lequation">
  <xsl:if test="@tag-down">
    <xsl:attribute name="style">
      <xsl:value-of select="concat('padding-bottom:',@tag-up)"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:if test="@tag-up">
    <xsl:attribute name="style">
      <xsl:value-of select="concat('padding-top:',@tag-down)"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="@id">
      <xsl:text>(</xsl:text>
      <xsl:number level="any" count="formula[@id]" />
      <xsl:text>)</xsl:text>
    </xsl:when>
    <xsl:otherwise>
      <xsl:value-of select="@tag" />
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>




<xsl:template match="simplemath">
  <i><xsl:apply-templates/></i>
</xsl:template>


<!-- Foot notes, separate page -->

<!-- remove footnotes in bad cases  -->
<xsl:template match='note' mode="text" />

<!-- This is a possible translation of notes  -->
<xsl:template match='unused-note'>
 <small><xsl:text>(note: </xsl:text><xsl:apply-templates /><xsl:text>)</xsl:text></small>
</xsl:template>

<xsl:template match='note'>
  <a id="{@id}">
    <xsl:call-template name="note-to-href"/>
    <xsl:apply-templates mode="xreftitle" select="."/>
    <small>(note: </small>
    <xsl:text>&#x27B3;</xsl:text>
    <small>)</small>
  </a>
</xsl:template>


<xsl:template match='note' mode="xref">
   <xsl:call-template name="calculateFootnoteNumber"/>
</xsl:template>

<xsl:template match='note' mode="footnote">
    <hr/>
    <p class='nofirst noindent'>
      <xsl:attribute name="id">
	<xsl:text>note</xsl:text>
	<xsl:call-template name="calculateFootnoteNumber"/>
      </xsl:attribute>
      <a title="back to text">
	<xsl:call-template name="id-to-href"/>
	<xsl:text>Note </xsl:text>
	<xsl:call-template name="calculateFootnoteNumber"/>
	<xsl:text>. </xsl:text>
      </a>
      <xsl:if test="not(p)">
        <xsl:apply-templates />
      </xsl:if>
    </p>
      <xsl:if test="p">
        <xsl:apply-templates />
      </xsl:if>
</xsl:template>



<xsl:template name="calculateFootnoteNumber">
  <xsl:number level="any" count="note[@place='foot']"/>
</xsl:template>


<!-- FIGURE -->
<xsl:template match='figure' mode="xref">
   <xsl:call-template name="calculateFigureNumber"/>
</xsl:template>

<xsl:template match="figure">
  <div class='hc'>
      <xsl:apply-templates/>
  </div>
</xsl:template>


<xsl:template match="figure[@rend='inline']">
 <xsl:call-template name='generate-graphics'/>
</xsl:template>

<xsl:template match="figure[@file and not(@rend='inline')]">
    <table style='caption-side:bottom' width="80%" id="{./@id}">
      <caption>
	<strong>
	  <xsl:text>Figure </xsl:text>
	  <xsl:call-template name="calculateFigureNumber" />
	  <xsl:text>. </xsl:text>
	</strong>
	<xsl:apply-templates select='head' mode='caption'/>
      </caption>
    <tbody align="center">
      <tr><td><xsl:call-template name='generate-graphics'/></td></tr>
    </tbody></table>
</xsl:template>

<xsl:template name="generate-graphics">
  <img>
    <xsl:attribute name="src">
      <xsl:apply-templates  select='@file'/>
      <xsl:text>.png</xsl:text>
    </xsl:attribute>
    <xsl:attribute name="alt">
      <xsl:apply-templates  select='@file'/>
    </xsl:attribute>
  </img>
</xsl:template>

<xsl:template name="calculateItemNumber">
  <xsl:number count="item" level="multiple"/>
</xsl:template>

<xsl:template name="calculateFigureNumber">
  <xsl:number count="figure[@rend != 'inline']" level="any"/>
</xsl:template>

<xsl:template match="rawimage">
  <img>
    <xsl:attribute name="src">
      <xsl:apply-templates/>
    </xsl:attribute>
    <xsl:attribute name="alt">
      <xsl:apply-templates/>
    </xsl:attribute>
  </img>
</xsl:template>

<!-- TOC -->
<!-- This is called frm the main file -->
<xsl:template match="tableofcontents" mode='xtoc'>
  <xsl:text>&#x0A;</xsl:text> <br/>
  <a href="#tableofcontents">
    <b><xsl:value-of select="$TOCname"/></b>
  </a>
</xsl:template>

<xsl:template match="tableofcontents">
  <h1 id='tableofcontents'> <xsl:value-of select="$TOCname"/></h1>
  <p>
    <xsl:for-each select="//div0|//div1|//div2|//div3 |//div4 | //div5|
			  //biblio| //theindex | //theglossary">
      <xsl:apply-templates mode="xtoc" select="(.)"/>
    </xsl:for-each>
    <xsl:text>&#x0A;</xsl:text>
   </p>
</xsl:template>



<xsl:template name="shorttoc">
  <h1>   <xsl:text>Short Table of Contents</xsl:text></h1>
  <p>
    <xsl:for-each select="//div0 |  //biblio| //theindex | //theglossary| //tableofcontents">
      <xsl:apply-templates mode="xtoc" select="(.)"/>
    </xsl:for-each>
  </p>
</xsl:template>


<!-- Other -->

<xsl:template match="LaTeX">LaTeX</xsl:template>
<xsl:template match="TeX">TeX</xsl:template>

<xsl:template match="hss"/>
<xsl:template match="hfil"/>
<xsl:template match="hfill"/>

<xsl:template match="node|nodeconnect|nodetriangle|anodecurve">
      <xsl:apply-templates/>
</xsl:template>


<xsl:template name="html-meta">
  <head>
    <xsl:text>&#x0A;</xsl:text>
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
    <xsl:text>&#x0A;</xsl:text>
    <meta name="author">
      <xsl:attribute name="content">
	<xsl:call-template name="author" />
      </xsl:attribute>
    </meta>
    <xsl:text>&#x0A;</xsl:text>
    <title> <xsl:call-template name="title" /></title>
    <xsl:text>&#x0A;</xsl:text>
    <link rel="stylesheet" href="tralics.css" />
    <xsl:text>&#x0A;</xsl:text>
    <meta name="keywords">
      <xsl:attribute name="content">
	<xsl:call-template name="keywords" />
      </xsl:attribute>
    </meta>
    <xsl:text>&#x0A;</xsl:text>
  </head>
</xsl:template>

<xsl:template match="std">
  <html>
    <xsl:call-template name="html-meta" />
    <body>
      <xsl:call-template name="header"/>  
      <xsl:if test="$shorttoc='true' or 
		    ($shorttoc='maybe' and $split='false')">
	<xsl:call-template name="shorttoc"/>
      </xsl:if>
      <xsl:apply-templates/>
      <xsl:call-template name="footnotes" />
 
    </body>
  </html>
</xsl:template>


<!-- Theorems -->
<xsl:template match="theorem/head"/>
<xsl:template match="theorem/alt_head"/>

<xsl:template match="theorem/alt_head" mode="text">
  <xsl:text> (</xsl:text><xsl:apply-templates/><xsl:text>)</xsl:text>
</xsl:template>

<xsl:template name="calculateTheoremNumber">
  <xsl:variable name="thistype">
    <xsl:value-of select="@type"/>
  </xsl:variable>
  <xsl:number count="theorem[@type = $thistype]" level="any"/>
</xsl:template>

<xsl:template match='theorem' mode="xref">
   <xsl:call-template name="calculateTheoremNumber"/>
</xsl:template>

<xsl:template match="theorem">
  <div class='theorem-{@type}'>
    <p><a><b>
	  <xsl:call-template name="id"/>
	  <xsl:value-of select="head"/>
	  <xsl:text> </xsl:text>
	  <xsl:call-template name="calculateTheoremNumber"/>
	  <xsl:apply-templates select="alt_head" mode="text"/>
    </b></a></p>
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="proof">
  <div class='proof'>
    <p><b>Proof</b></p>
    <xsl:apply-templates/>
  </div>
</xsl:template>


<xsl:template match="maplein">
  <div class='maple-in'>
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="inline">
  <span>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates/>
  </span>
</xsl:template>


<xsl:template match="mapleout">
  <div class='maple-out hc mathdisplay '>
    <xsl:apply-templates/>
  </div>
</xsl:template>

<xsl:template match="pseudocode">
  <p class='nofirst noindent'>
  <xsl:if test="@leftskip">
    <xsl:attribute name="style">
       <xsl:text>margin-left:</xsl:text>
       <xsl:value-of select="@leftskip"/>
    </xsl:attribute>
  </xsl:if> 
  <xsl:for-each select="p">
   <xsl:apply-templates/>
   <xsl:text>&#x0A;</xsl:text><br/>
  </xsl:for-each>
  </p>
</xsl:template>

<xsl:template match="clearpage"/>
<xsl:template match="zws"/>

</xsl:stylesheet>

<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
