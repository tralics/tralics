<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format standard classes XML documents 


$Id: raweb3fo.xsl,v 2.7 2007/01/09 08:39:59 grimm Exp $
-->

<!-- Copyright Inria 2003-2004 Jose Grimm. This file is an adaptation of
file from the TEI distribution. See original Copyright notice below.
On 2004/09/14, rawebfo.xsl copied to rawebfo3.xsl
-->

<!-- 
 Copyright 1999-2001 Sebastian Rahtz/Oxford University  <sebastian.rahtz@oucs.ox.ac.uk>

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and any associated documentation files (the
 ``Software''), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
-->


<xsl:stylesheet
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML" 
  xmlns:fo="http://www.w3.org/1999/XSL/Format">


<xsl:param name="linkColor">red</xsl:param>
<xsl:param name="pageMarginRight">80pt</xsl:param>
<xsl:param name="pdfBookmarks"></xsl:param>
<xsl:param name="bodyFont">LatinModern</xsl:param>
<xsl:param name="divFont">LatinModern</xsl:param>
<xsl:include href="raweb3-param.xsl"/>
<xsl:include href="rrrafo3.xsl"/>
<xsl:include href="clspages.xsl"/>
<xsl:include href="RRfosimple.xsl"/>
<xsl:include href="clsfotable.xsl"/>

<xsl:variable name="equationnumber">
   <xsl:value-of select="/std/@equation-number"/></xsl:variable>
<xsl:variable name="keywords">BIG</xsl:variable>


<!-- ignore spaces in the elements of the list -->
<xsl:strip-space elements="cell bediteur bauteurs citation UR"/>
<xsl:output indent="no"/>


<xsl:variable name="top" select="/"/>
<xsl:variable name="tableSpecs">
  <Info></Info> 
</xsl:variable>

<xsl:variable name="Lefthead">
  <xsl:value-of select="/std/author"/>
</xsl:variable>

<xsl:variable name="Righthead">
  <xsl:value-of select="/std/title"/>
</xsl:variable>

<xsl:template name="odd-footer"/>
<xsl:template name="even-footer"/>
<xsl:template name="first-footer"/>


<!-- enable, for new elements to get commented as such-->
<xsl:template match="*">
  <xsl:comment><xsl:text>PASS THROUGH </xsl:text>
    <xsl:value-of select="name()"/>
  </xsl:comment>
  <xsl:apply-templates/>
</xsl:template>


<!-- std is document element-->
<xsl:template match="std">
 <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
   <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="setupPagemasters"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="myheaders"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="titlepage"/> 
   <xsl:call-template name="toc"/>
   <xsl:call-template name="maintext"/>
 </fo:root>
</xsl:template>

<xsl:template name="titlepage.body">
  <xsl:call-template name="header"/>
</xsl:template>

<xsl:template name="maintext.body">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template name="biblio.start">
  <fo:block keep-with-next.within-page="always" id = "biblio">
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/>
    </xsl:attribute>
    <xsl:call-template name="setupDiv0"/>
    <xsl:attribute name="text-indent">0pt</xsl:attribute>
    <xsl:text>Bibliography</xsl:text>
  </fo:block>
</xsl:template>



<xsl:template name="secNumberedHeading">
  <fo:block keep-with-next.within-page="always">
    <xsl:variable name="divid">        
       <xsl:call-template name="idLabel"/>
    </xsl:variable>
    <xsl:attribute name="id"> <xsl:value-of select="$divid"/> </xsl:attribute>
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/></xsl:attribute>
    <xsl:call-template name="setupDiv0"/>
    <xsl:call-template name="blockStartHook"/>
    <xsl:value-of select="@numero"/>
    <xsl:text>. </xsl:text>
    <xsl:value-of select="@titre"/>
  </fo:block>
</xsl:template>

<xsl:template name="idLabel">
   <xsl:choose>
       <xsl:when test="@id">
         <xsl:value-of select="translate(@id,'_','-')"/>
       </xsl:when>
       <xsl:otherwise>
          <xsl:value-of select="generate-id()"/>
       </xsl:otherwise>
   </xsl:choose>
</xsl:template>

<xsl:template name="NumberedHeading">
  <xsl:param name="level"/>
  <fo:block keep-with-next.within-page="always">
    <xsl:variable name="divid">        
       <xsl:call-template name="idLabel"/>
    </xsl:variable>
    <xsl:attribute name="id">
      <xsl:value-of select="$divid"/>
    </xsl:attribute>
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/>
    </xsl:attribute>
    <xsl:choose>
     <xsl:when test="$level=0"><xsl:call-template name="setupDiv0"/></xsl:when>
     <xsl:when test="$level=1"><xsl:call-template name="setupDiv1"/></xsl:when>
     <xsl:when test="$level=2"><xsl:call-template name="setupDiv2"/></xsl:when>
     <xsl:when test="$level=3"><xsl:call-template name="setupDiv3"/></xsl:when>
     <xsl:otherwise><xsl:call-template name="setupDiv4"/></xsl:otherwise>
    </xsl:choose>
    <xsl:variable name="Number">
      <xsl:call-template name="calculateNumber"/>
    </xsl:variable>
    <xsl:value-of select="$Number"/>
    <xsl:text>. </xsl:text>
    <xsl:apply-templates mode="section" select="head"/>
  </fo:block>
</xsl:template>


<!--  Divisions -->
<xsl:template match="div0|div1|div2|div3|div4|div5">
  <xsl:call-template name="NumberedHeading">
    <xsl:with-param name="level">
       <xsl:value-of select="substring-after(name(),'div')"/>
    </xsl:with-param>
  </xsl:call-template>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template name="toc.body">
  <fo:block font-size="14pt" text-align="center" font-weight="bold" 
         space-after="20pt">
     <xsl:text>Table of contents</xsl:text>
  </fo:block>
  <xsl:for-each select="//div0|//div1|//div2|//div3">
    <xsl:apply-templates mode="xtoc" select="(.)"/>
  </xsl:for-each>
</xsl:template><!-- Headings for divisions -->

<xsl:template mode="xtoc" match="div1|div2|div3|div4|div5">
  <xsl:call-template name="tocheading">
    <xsl:with-param name="level">
        <xsl:value-of select="substring-after(name(),'div')"/></xsl:with-param>
   </xsl:call-template>
</xsl:template>

<xsl:template name="tocheading">
  <xsl:param name="level"/>
  <xsl:variable name="tocindent">
    <xsl:choose>
      <xsl:when test="$level='0'">0pt</xsl:when>
      <xsl:when test="$level='1'">14pt</xsl:when>
      <xsl:when test="$level='2'">25pt</xsl:when>
      <xsl:when test="$level='3'">40pt</xsl:when>
      <xsl:when test="$level='4'">55pt</xsl:when>
      <xsl:when test="$level='5'">85pt</xsl:when>
      <xsl:otherwise>105pt</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="tocwidth">
    <xsl:choose>
      <xsl:when test="$level='0'">10pt</xsl:when>
      <xsl:when test="$level='1'">24pt</xsl:when>
      <xsl:when test="$level='2'">32pt</xsl:when>
      <xsl:when test="$level='3'">42pt</xsl:when>
      <xsl:when test="$level='4'">53pt</xsl:when>
      <xsl:when test="$level='5'">63pt</xsl:when>
      <xsl:otherwise>73pt</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="Number">
    <xsl:if test="$numberHeadings and $numberDepth &gt; $level">
      <xsl:call-template name="calculateNumber"/>
      <xsl:text>. </xsl:text>
    </xsl:if>
  </xsl:variable>
  <fo:block>
    <xsl:attribute name="toc">true</xsl:attribute>
    <xsl:attribute name="margin-right">15pt</xsl:attribute>
    <xsl:attribute name="margin-left"> 
      <xsl:value-of select="$tocwidth"/>
    </xsl:attribute>
    <xsl:attribute name="text-indent">
      <xsl:value-of select="$tocindent"/>
    </xsl:attribute>
    <fotex:section-number width='{$tocwidth}'>
      <fo:inline space-start="20pt">
	<xsl:value-of select="$Number"/>
      </fo:inline>
    </fotex:section-number>
    <fo:inline space-start="20pt">
      <xsl:apply-templates mode="tocsection" select="head"/>
    </fo:inline>
    <fotex:page-number>
      <fo:inline color="{$linkColor}">
	<xsl:variable name="pagref">
          <xsl:call-template name="idLabel"/>
	</xsl:variable>
	<fo:basic-link internal-destination="{$pagref}">
	  <fo:page-number-citation ref-id="{$pagref}"/>
	</fo:basic-link>
      </fo:inline> 
    </fotex:page-number>
  </fo:block>
  <xsl:text>&#xA;</xsl:text>
</xsl:template>


<xsl:template match ="div0" mode="xtoc">
  <xsl:variable name="Number">
    <xsl:if test="$numberHeadings">
      <xsl:call-template name="calculateNumber"/>
      <xsl:text>. </xsl:text>
    </xsl:if>
  </xsl:variable>
  <fo:block font-weight="bold" >
    <xsl:value-of select="$Number"/>
    <xsl:text>&#x2003;</xsl:text>
    <fo:inline> 
      <xsl:apply-templates mode="tocsection" select="head"/>
    </fo:inline>
    <fo:leader leader-pattern="dots"/>
    <fo:inline color="{$linkColor}">
      <xsl:variable name="pagref">
        <xsl:call-template name="idLabel"/>
      </xsl:variable>
      <fo:basic-link internal-destination="{$pagref}">
	<fo:page-number-citation ref-id="{$pagref}"/>
      </fo:basic-link>
    </fo:inline> 
  </fo:block>
  <xsl:text>&#xA;</xsl:text>
</xsl:template>


<xsl:template match="head" mode="tocsection">
   <xsl:apply-templates mode="section"/>
</xsl:template>

<xsl:template match="m:math" mode="section">
 <m:math>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates mode="math"/>
 </m:math>
</xsl:template>

<xsl:template match="head" mode="section">
 <fo:inline>    
  <xsl:if test=".!='(Sans Titre)'">
   <xsl:if test="@id">
    <xsl:attribute name="id">
     <xsl:value-of select="translate(@id,'_','-')"/>
    </xsl:attribute>
   </xsl:if>
   <xsl:apply-templates mode="section"/>
  </xsl:if>
 </fo:inline>
</xsl:template>


<xsl:template match="biblio">
  <xsl:call-template name="biblio.start"/>
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="citation">
  <fo:block space-before="15pt" text-indent="-1em" id="{@id}"> 
     [<xsl:value-of select="@key"/>]
     <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template name="separateur.objet">
 <xsl:choose>
  <xsl:when test="position()!=last()">, </xsl:when>
  <xsl:when test="position()=last()">.</xsl:when>
 </xsl:choose>
</xsl:template>

<xsl:template match='bvolume|bnumber|bpages|bchapter|bseries|bedition'>
  <xsl:value-of select="@bname"/>
  <xsl:text> </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='btitle'>
  <fo:inline font-style='italic'><xsl:apply-templates/>. </fo:inline> 
</xsl:template>


<xsl:template match='bjournal|bbooktitle'>
  <xsl:text>in &#xAB;&#x000A0;</xsl:text>
  <xsl:apply-templates/>
  <xsl:text>&#x000A0;&#xBB;</xsl:text>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>


<xsl:template match='byear|bmonth|btype|bschool|bpublisher|
   bnote|borganization|binstitution|baddress|bhowpublished'>
  <xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>



<xsl:template match='bdoi'>
  <xsl:value-of select="@bname"/><xsl:text> </xsl:text> 
  <fo:basic-link color="{$linkColor}">  
   <xsl:attribute name="external-destination">http://dx.doi.org/<xsl:value-of select="."/>
   </xsl:attribute>
   <xsl:apply-templates/>
 </fo:basic-link>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match ="bauteurs">
  <fo:inline font-variant='small-caps'>
     <xsl:for-each select="bpers|etal">
        <xsl:value-of select="@prenom"/> 
        <xsl:text> </xsl:text>
        <xsl:if test="@part">
           <xsl:value-of select="@part"/>
           <xsl:text> </xsl:text>
        </xsl:if>
        <xsl:value-of select="@nom"/>
        <xsl:call-template name="separateur.objet"/>
     </xsl:for-each>
   </fo:inline> 
   <xsl:text> </xsl:text> 
</xsl:template>

<xsl:template match ="bediteur">
  <fo:inline font-variant='small-caps'>
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
   </fo:inline> 
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


<xsl:template match="cit">
 <fo:basic-link color="{$linkColor}">
    <xsl:attribute name="internal-destination">
      <xsl:value-of select="ref/@target"/>
    </xsl:attribute>
    <xsl:text>[</xsl:text>
    <xsl:value-of select="id(ref/@target)/@key"/>
    <xsl:text>]</xsl:text>
  </fo:basic-link>
</xsl:template>


<xsl:template match="catperso">
  <fo:block space-before="3pt">
    <fo:block font-weight='bold' text-indent="-15pt">
        <xsl:value-of select="head"/></fo:block>
    <xsl:for-each select="pers">
        <fo:block> <xsl:call-template name="pers"/></fo:block>
    </xsl:for-each>
  </fo:block>
</xsl:template>



<xsl:template match="participants|participant|participante|participantes">
 <fo:block space-after.optimum="4pt">
   <fo:inline>
     <xsl:attribute name="font-weight">bold</xsl:attribute>
       <xsl:value-of select="@titre"/> 
   </fo:inline>
   <fo:inline>
    <xsl:for-each select="pers">
       <xsl:call-template name="pers"/>
       <xsl:call-template name="separateur.objet"/>
    </xsl:for-each>
   </fo:inline>
 </fo:block>
</xsl:template>


<xsl:template name="pers">
    <xsl:value-of select="./@prenom"/> 
    <xsl:text> </xsl:text>
    <xsl:value-of select="./@nom"/>
    <xsl:if test="not(normalize-space(string(.)) ='')">
       <xsl:text> [</xsl:text>
        <xsl:apply-templates/> 
       <xsl:text>]</xsl:text>
    </xsl:if> 
</xsl:template>


<xsl:template match="xref">
   <fo:basic-link color="{$linkColor}">
   <xsl:attribute name="external-destination">
     <xsl:value-of select="@url"/>
   </xsl:attribute>
      <xsl:apply-templates/>
   </fo:basic-link>
</xsl:template>


<xsl:template match="citation/xref">
   <fo:basic-link color="{$linkColor}">
   <xsl:attribute name="external-destination">
     <xsl:value-of select="@url"/>
   </xsl:attribute>
      <xsl:apply-templates/>
   </fo:basic-link>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>


<xsl:template match="ref">
 <fo:basic-link color="{$linkColor}">
  <xsl:attribute name="internal-destination"><xsl:value-of select="translate(@target,'_','-')"/></xsl:attribute>
    <xsl:apply-templates mode="xref" select="id(@target)" />
  <xsl:apply-templates/>
 </fo:basic-link>
</xsl:template>

<xsl:template 
    match="biblio|presentation|fondements|domaine|logiciels|resultats|contrats|composition
  |international|diffusion" mode = 'xref'>
  <xsl:value-of select = "@numero"/>
</xsl:template>


<xsl:template name="sec.num">
   <xsl:value-of select = 
         "ancestor-or-self::*[self::composition or self::presentation or self::fondements or 
     self::domaine or self::logiciels or self::resultats or 
self::contrats or self::international or self::diffusion or self::biblio]/@numero"/>
  <xsl:text>.</xsl:text>
</xsl:template>


<xsl:template name="calculateNumber">
  <xsl:number level="multiple" from="/"
              count="div0|div1|div2|div3|div4|div5"/>
</xsl:template>


<xsl:template mode="xref" match="div0|div1|div2|div3|div4|div5">
  <xsl:call-template name="calculateNumber"/>
</xsl:template>

<xsl:template match="head" mode ="xref">
   <xsl:call-template name="calculateNumber"/>
</xsl:template>



<xsl:template match="formula" mode="xref">
  <xsl:number level = "any" count="formula[@id]"/>
</xsl:template>

<xsl:template name="calculateFigureNumber">
     <xsl:number count="figure[@rend != 'inline']" level="any"/>
</xsl:template>

<xsl:template match='figure' mode="xref">
   <xsl:call-template name="calculateFigureNumber"/>
</xsl:template>

<xsl:template name="calculateTableNumber">
    <xsl:number count="table[@rend != 'inline']" level="any"/>
</xsl:template>


<xsl:template match='table' mode="xref">
   <xsl:call-template name="calculateTableNumber"/>
</xsl:template>


<xsl:template name="calculateFootnoteNumber">
  <xsl:number level="any" count="note[@place='foot']"/>
</xsl:template>

<xsl:template match="item" mode="xref">
   <xsl:variable name="listdepth" select="count(ancestor::list)"/>
   <xsl:variable name="listNFormat">
     <xsl:choose>
        <xsl:when test="$listdepth=1"><xsl:text>1</xsl:text></xsl:when>
        <xsl:when test="$listdepth=2"><xsl:text>i</xsl:text></xsl:when>
        <xsl:when test="$listdepth=3"><xsl:text>a</xsl:text></xsl:when>
        <xsl:when test="$listdepth=4"><xsl:text>I</xsl:text></xsl:when>
     </xsl:choose>
   </xsl:variable>
   <xsl:number format="{$listNFormat}"/>
</xsl:template>


<xsl:template match="p">
  <fo:block font-size="{$bodySize}">
    <xsl:if test="preceding-sibling::p">
       <xsl:if test ="not(@noindent='true')">
           <xsl:attribute name="text-indent">
              <xsl:value-of select="$parIndent"/>
           </xsl:attribute>
       </xsl:if>
       <xsl:choose>
         <xsl:when test="@spacebefore">
           <xsl:attribute name="space-before.optimum">
               <xsl:value-of select="@spacebefore"/>
           </xsl:attribute>
         </xsl:when>  
         <xsl:when test="ancestor::pseudocode">
           <xsl:attribute name="space-before.optimum">0pt</xsl:attribute>
         </xsl:when>
         <xsl:otherwise>
           <xsl:attribute name="space-before.optimum">
              <xsl:value-of select="$parSkip"/>
           </xsl:attribute>
         </xsl:otherwise>
       </xsl:choose>
       <xsl:attribute name="space-before.maximum">
         <xsl:value-of select="$parSkipmax"/>
       </xsl:attribute>
    </xsl:if> 
    <xsl:if test="not(preceding-sibling::p)">
       <xsl:if test="@spacebefore">
          <xsl:attribute name="space-before.optimum">
             <xsl:value-of select="@spacebefore"/>
          </xsl:attribute>
       </xsl:if>
    </xsl:if>
    <xsl:choose>
      <xsl:when test="@rend ='centered'">
       <xsl:attribute name="text-align">center</xsl:attribute>
      </xsl:when>
      <xsl:when test="@rend ='center'">
       <xsl:attribute name="text-align">center</xsl:attribute>
      </xsl:when>
      <xsl:when test="@rend ='flushed-left'">
       <xsl:attribute name="text-align">left</xsl:attribute>
      </xsl:when>
      <xsl:when test="@rend ='flushed-right'">
       <xsl:attribute name="text-align">right</xsl:attribute>
      </xsl:when>
      <xsl:when test="@rend ='quoted'">
       <xsl:attribute name="text-align">justify</xsl:attribute>
       <xsl:attribute name="margin-left">1cm</xsl:attribute>
       <xsl:attribute name="margin-right">1cm</xsl:attribute>
      </xsl:when>
      <xsl:when test="@rend ='justify'">
       <xsl:attribute name="text-align">justify</xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="hi">
  <fo:inline>
    <xsl:call-template name="rend" />
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


<!--  paragraph in verbatim mode -->
<xsl:template match="pre/p">
  <fo:block font-size="{$bodySize}">
    <xsl:if test="not(preceding-sibling::p)">
      <xsl:attribute name="space-before">
        <xsl:value-of select="$parSkip"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="not(following-sibling::p)">
      <xsl:attribute name="space-after">
        <xsl:value-of select="$parSkip"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="parent::pre/@class='latex-code'">
      <xsl:attribute name="color">maroon</xsl:attribute>
    </xsl:if>
    <xsl:if test="parent::pre/@class='xml-code'">
      <xsl:attribute name="color">#4422FF</xsl:attribute>
    </xsl:if>
    <xsl:if test="vbnumber">
      <xsl:attribute name="text-indent">-2em</xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>


<xsl:template match="vbnumber">
  <fo:inline font-size="9pt">
    <fotex:line-number><xsl:apply-templates/></fotex:line-number>
  </fo:inline>
</xsl:template>

<xsl:template match="pre">
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="latexcode">
  <fo:inline font-family="{$typewriterFont}" color="maroon">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match="xmlcode">
  <fo:inline font-family="{$typewriterFont}" color="4422FF">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


<xsl:template match="code">
  <fo:inline font-family="{$typewriterFont}">
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match="ident">
    <fo:inline color="{$identColor}" font-family="{$sansFont}">
        <xsl:apply-templates/>
    </fo:inline>
</xsl:template>

<xsl:template match="term">
    <fo:inline font-style="italic">
      <xsl:apply-templates/>
    </fo:inline>
</xsl:template>


<xsl:template match="list">
  <xsl:if test="child::head">
    <fo:block font-style="italic" 
        text-align="start"
        space-before.optimum="4pt">
      <xsl:apply-templates select="head"/>
    </fo:block>
  </xsl:if>
  <fo:list-block margin-right="{$listRightMargin}">
    <xsl:call-template name="setListIndents"/>
    <xsl:choose>
      <xsl:when test="@type='gloss'">
        <xsl:attribute name="margin-left">
          <xsl:choose>
            <xsl:when test="ancestor::list">
              <xsl:value-of select="$listLeftGlossInnerIndent"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$listLeftGlossIndent"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:attribute>
      </xsl:when>
      <xsl:otherwise>
        <xsl:attribute name="margin-left">
          <xsl:value-of select="$listLeftIndent"/>
        </xsl:attribute>
      </xsl:otherwise> 
    </xsl:choose>
    <xsl:apply-templates select="item"/>
  </fo:list-block>
</xsl:template>

<xsl:template name="setListIndents">
 <xsl:variable name="listdepth" select="count(ancestor::list)"/>
   <xsl:choose>
     <xsl:when test="$listdepth=0">
       <xsl:attribute name="space-before">
         <xsl:value-of select="$listAbove-1"/>
       </xsl:attribute>
       <xsl:attribute name="space-after">
         <xsl:value-of select="$listBelow-1"/>
       </xsl:attribute>
     </xsl:when>
     <xsl:when test="$listdepth=1">
       <xsl:attribute name="space-before">
          <xsl:value-of select="$listAbove-2"/>
       </xsl:attribute>
       <xsl:attribute name="space-after">
         <xsl:value-of select="$listBelow-2"/>
       </xsl:attribute>
     </xsl:when>
     <xsl:when test="$listdepth=2">
       <xsl:attribute name="space-before">
         <xsl:value-of select="$listAbove-3"/>
       </xsl:attribute>
       <xsl:attribute name="space-after">
         <xsl:value-of select="$listBelow-3"/>
       </xsl:attribute>
     </xsl:when>
     <xsl:when test="$listdepth=3">
       <xsl:attribute name="space-before">
         <xsl:value-of select="$listAbove-4"/>
       </xsl:attribute>
       <xsl:attribute name="space-after">
         <xsl:value-of select="$listBelow-4"/>
       </xsl:attribute>
    </xsl:when>
  </xsl:choose>
</xsl:template>


<xsl:template match="item">
  <xsl:call-template name="makeItem"/>
</xsl:template>


<xsl:template name="makeItem">
  <xsl:variable name="listdepth" select="count(ancestor::list)"/>
  <fo:list-item space-before.optimum="{$listItemsep}">
    <fo:list-item-label>
    <xsl:if test="@id">
      <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
    </xsl:if>
    <fo:block>
      <xsl:attribute name="margin-right">2.5pt</xsl:attribute>
      <xsl:choose>
        <xsl:when test="../@type='ordered'">
          <xsl:attribute name="text-align">end</xsl:attribute>
          <xsl:apply-templates mode="xref" select="."/>
          <xsl:text>.</xsl:text>
        </xsl:when>
        <xsl:when test="../@type='gloss'">
          <xsl:attribute name="text-align">start</xsl:attribute>
          <xsl:attribute name="font-weight">bold</xsl:attribute>
         <xsl:choose>
         <xsl:when test="label">
          <xsl:apply-templates mode="print" select="label"/>
         </xsl:when>
         <xsl:otherwise>
          <xsl:apply-templates mode="print" select="preceding-sibling::*[1]"/>
         </xsl:otherwise>
         </xsl:choose>
        </xsl:when>
         <xsl:when test="name(preceding-sibling::*[1])='label'">
          <xsl:apply-templates mode="print" select="preceding-sibling::*[1]"/>
         </xsl:when>

        <xsl:otherwise>
           <xsl:attribute name="text-align">center</xsl:attribute>
          <xsl:choose>
            <xsl:when test="$listdepth=1">
              <xsl:value-of select="$bulletOne"/>
            </xsl:when>
            <xsl:when test="$listdepth=2">
              <xsl:value-of select="$bulletTwo"/>
            </xsl:when>
            <xsl:when test="$listdepth=3">
              <xsl:value-of select="$bulletThree"/>
            </xsl:when>
            <xsl:when test="$listdepth=4">
              <xsl:value-of select="$bulletFour"/>
            </xsl:when>
          </xsl:choose>
        </xsl:otherwise>
     </xsl:choose>
   </fo:block>
 </fo:list-item-label>
 <fo:list-item-body>
   <xsl:choose>
   <xsl:when test="p">
     <xsl:apply-templates/>
   </xsl:when>
   <xsl:otherwise>
    <fo:block font-weight="normal"><xsl:apply-templates/></fo:block>
   </xsl:otherwise>
   </xsl:choose>
 </fo:list-item-body>
</fo:list-item>
</xsl:template>

<xsl:template mode="print" match="label">
      <xsl:apply-templates/>
</xsl:template>

<xsl:template match="label"/>


<xsl:template name="generate-graphics">
  <fo:external-graphic src="{@file}">
    <xsl:choose>
      <xsl:when test="@scale">
         <xsl:attribute name="content-width">
           <xsl:value-of select="@scale * 100"/><xsl:text>%</xsl:text>
         </xsl:attribute>
      </xsl:when>
      <xsl:when test="@width">
        <xsl:attribute name="content-width">
          <xsl:value-of select="@width"/>
        </xsl:attribute>
        <xsl:if test="@height">
          <xsl:attribute name="content-height">
            <xsl:value-of select="@height"/>
          </xsl:attribute>
        </xsl:if>
        <xsl:if test="@angle">
           <xsl:attribute name="angle">
               <xsl:value-of select="@angle"/>
           </xsl:attribute>
        </xsl:if>
      </xsl:when>
      <xsl:when test="@height">
         <xsl:attribute name="content-height">
           <xsl:value-of select="@height"/>
         </xsl:attribute>
         <xsl:if test="@angle">
           <xsl:attribute name="angle">
             <xsl:value-of select="@angle"/>
           </xsl:attribute>
         </xsl:if>
      </xsl:when>
    </xsl:choose>
  </fo:external-graphic>
</xsl:template>

<xsl:template match="figure[@rend='inline']">
  <xsl:call-template name='generate-graphics'/>
      <xsl:if test="head">
        <fo:block text-align="center">
           <xsl:apply-templates select="head"/>
        </fo:block>
      </xsl:if>
</xsl:template>

<xsl:template match='figure'>
 <fo:float>
   <xsl:call-template name="addID"/>
   <fo:block text-align="center">
    <xsl:choose>
      <xsl:when test="@file">
       <xsl:call-template name='generate-graphics'/>
      </xsl:when>
      <xsl:otherwise><xsl:apply-templates select='p'/></xsl:otherwise>
    </xsl:choose>
   </fo:block>
   <fo:block>
     <xsl:call-template name="figureCaptionstyle"/>
     <xsl:value-of select="$figureWord"/>
     <xsl:call-template name="calculateFigureNumber"/>
     <xsl:text>. </xsl:text>
     <xsl:apply-templates select="head"/>
   </fo:block>
 </fo:float>
</xsl:template>



<!-- math -->
<xsl:template match="m:math">
 <m:math>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates mode="math"/>
 </m:math>
</xsl:template>

<xsl:template match="m:*|@*|comment()|processing-instruction()|text()" mode="math">
 <xsl:copy>
   <xsl:apply-templates mode="math" select="*|@*|processing-instruction()|text()"/>
 </xsl:copy>
</xsl:template>

<xsl:template match="formula">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="clearpage">
  <fo:block break-after="page"/>
</xsl:template>

<xsl:template match="zws">
  <xsl:text>&#x200B;</xsl:text>
</xsl:template>



<xsl:template match="formula[@type='display']">
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
    <xsl:when test="@id">
      <fo:inline id="{@id}">
        <fotex:equation useleqno='{$nbleft}'>
          <xsl:for-each select="m:math">
            <xsl:apply-templates mode="math"/>
          </xsl:for-each>
        </fotex:equation>
      </fo:inline>
    </xsl:when>
    <xsl:when test="@tag">
      <fo:inline>
        <fotex:equation tag ='{@tag}' useleqno='{$nbleft}'>
          <xsl:for-each select="m:math">
            <xsl:apply-templates mode="math"/>
          </xsl:for-each>
        </fotex:equation> 
      </fo:inline>
    </xsl:when>

  <xsl:otherwise>   
   <fotex:displaymath>
    <xsl:for-each select="m:math">
     <xsl:apply-templates mode="math"/>
    </xsl:for-each>
   </fotex:displaymath>
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template match="simplemath">
  <m:math><m:mi><xsl:apply-templates/></m:mi></m:math>
</xsl:template>

<!-- 7.2.12 Other elements-->

<xsl:template match="head" priority="10">
 <xsl:variable name="parent" select="name(..)"/>
 <xsl:if test="not(starts-with($parent,'div')) and not($parent = 'module')"> 
  <xsl:apply-templates/>
 </xsl:if>
</xsl:template>


<xsl:template match="processing-instruction()[name()='xmltex']" >
   <xsl:message>xmltex pi <xsl:value-of select="."/></xsl:message>
   <xsl:copy-of select="."/>
</xsl:template>


<xsl:template name="accueil.body">
 <fo:INRIA year="2007"/>

 <xsl:text>&#xA;</xsl:text>
 <fo:block font-size= "25pt" text-align="center" space-before="1cm"> 
     <xsl:value-of select="projetdeveloppe"/> 
 </fo:block>
 <xsl:text>&#xA;</xsl:text>
 <fo:block font-size= "17.28pt" text-align="center" space-before="1cm">
    <xsl:for-each select ="UR/*">
      <fo:basic-link external-destination ="{@url}">
        <xsl:value-of select="@nom"/>
        <xsl:if test="position()!=last()"> - </xsl:if>
      </fo:basic-link>
    </xsl:for-each>
 </fo:block>
 <xsl:text>&#xA;</xsl:text>
 <fo:block font-size= "10pt" font-style="normal" 
       font-family="Helvetica" text-align="center" space-before="1cm">
    <fo:RATHEME><xsl:value-of select="theme"/></fo:RATHEME>
 </fo:block>
</xsl:template>

<xsl:template match="moreinfo">
  <fo:block font-style="italic"> <xsl:apply-templates/> </fo:block>
</xsl:template>

<xsl:template match="keywords">
 <fo:block space-after.optimum="4pt">
  <fo:inline>
    <xsl:attribute name="font-weight">bold</xsl:attribute>
    <xsl:value-of select="./@titre"/>
  </fo:inline>
  <xsl:for-each select="term">
    <xsl:apply-templates select="."/> 
    <xsl:call-template name="separateur.objet"/>
  </xsl:for-each>
 </fo:block>
</xsl:template>

<xsl:template name="addID">
  <xsl:attribute name="id">
     <xsl:call-template name="idLabel"/>
  </xsl:attribute>
</xsl:template>


<xsl:template match="TeX">
   <TeX/>
</xsl:template>

<xsl:template match="LaTeX">
   <LaTeX/>
</xsl:template>

<xsl:template match="tracingall">
   <tracingall/>
</xsl:template>

<xsl:template match="inline">
  <fo:inline>
     <xsl:copy-of select="@*"/>
     <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


</xsl:stylesheet>

<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
