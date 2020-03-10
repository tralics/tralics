<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format raweb version3 XML documents 


$Id: raweb3fo.xsl,v 2.12 2007/08/02 12:28:39 grimm Exp $
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


<!-- ignore spaces in the elements of the list -->
<xsl:strip-space elements="cell bediteur bauteurs citation UR"/>
<xsl:output indent="no"/>

<xsl:variable name="equationnumber">
   <xsl:value-of select="/std/@equation-number"/></xsl:variable>
<xsl:variable name="keywords">BIG</xsl:variable>


<xsl:variable name="top" select="/"/>
<xsl:variable name="tableSpecs">
  <Info></Info> 
</xsl:variable>

<xsl:variable name="LeTypeProjet">
  <xsl:choose>
    <xsl:when test='/raweb/accueil/@isproject="true"'>Project-Team</xsl:when>
    <xsl:otherwise>Team</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="PRID">
 <xsl:value-of select="$LeTypeProjet"/>
 <xsl:text> </xsl:text>
 <xsl:value-of select="/raweb/accueil/projet"/>
</xsl:variable>

<xsl:variable name="year">
 <xsl:choose>
   <xsl:when test="/raweb/@year">
      <xsl:value-of select="/raweb/@year" />
   </xsl:when>
   <xsl:otherwise>2000</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<!-- enable, for new elements to get commented as such-->
<xsl:template match="*">
  <xsl:comment><xsl:text>PASS THROUGH </xsl:text>
    <xsl:value-of select="name()"/>
  </xsl:comment>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template name="setupPagemasters">
  <fo:layout-master-set>

<!-- one sided, single column -->
     <fo:simple-page-master
        master-name="simple1"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
            margin-bottom="{$bodyMarginBottom}"
            margin-top="{$bodyMarginTop}"/>
        <fo:region-before extent="{$regionBeforeExtent}"/>
        <fo:region-after extent="{$regionAfterExtent}"/>
      </fo:simple-page-master>

<!-- for left-hand/even pages in twosided mode, single column -->
     <fo:simple-page-master
        master-name="left1"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before
                region-name="xsl-region-before-left" 
                extent="{$regionBeforeExtent}"/>
        <fo:region-after 
                region-name="xsl-region-after-left" 
                extent="{$regionAfterExtent}"/>
      </fo:simple-page-master>

<!-- for right-hand/odd pages in twosided mode, single column -->
      <fo:simple-page-master
        master-name="right1"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before 
                region-name="xsl-region-before-right" 
                extent="{$regionBeforeExtent}"/>
        <fo:region-after
                region-name="xsl-region-after-right" 
                extent="{$regionAfterExtent}"/>
     </fo:simple-page-master>

<!-- special case of first page in either mode, single column -->
      <fo:simple-page-master
        master-name="first1"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before  
                region-name="xsl-region-before-first"
                extent="{$regionBeforeExtent}"/>
        <fo:region-after 
                region-name="xsl-region-after-first"
                extent="{$regionAfterExtent}"/>
     </fo:simple-page-master>

<!-- blank page in either mode, single column -->
      <fo:simple-page-master
        master-name="blank1"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before  
                region-name="DummyRegion"
                extent="{$regionBeforeExtent}"/>
        <fo:region-after 
                region-name="DummyRegion"
                extent="{$regionAfterExtent}"/>
     </fo:simple-page-master>


<!-- for pages in one-side mode, 2 column -->
     <fo:simple-page-master
        master-name="simple2"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                column-count="{$columnCount}"
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before extent="{$regionBeforeExtent}"/>
        <fo:region-after extent="{$regionAfterExtent}"/>
      </fo:simple-page-master>

<!-- for left-hand/even pages in twosided mode, 2 column -->
     <fo:simple-page-master
        master-name="left2"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                column-count="{$columnCount}"
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before
                region-name="xsl-region-before-left" 
                extent="{$regionBeforeExtent}"/>
        <fo:region-after 
                region-name="xsl-region-after-left" 
                extent="{$regionAfterExtent}"/>
      </fo:simple-page-master>

<!-- for right-hand/odd pages in twosided mode, 2 column -->
      <fo:simple-page-master
        master-name="right2"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                column-count="{$columnCount}"
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before 
                region-name="xsl-region-before-right" 
                extent="{$regionBeforeExtent}"/>
        <fo:region-after
                region-name="xsl-region-after-right" 
                extent="{$regionAfterExtent}"/>
     </fo:simple-page-master>

<!-- special case of first page in either mode -->
      <fo:simple-page-master
        master-name="first2"
        page-width="{$pageWidth}"
        page-height="{$pageHeight}"
        margin-top="{$pageMarginTop}"
        margin-bottom="{$pageMarginBottom}"
        margin-left="{$pageMarginLeft}"
        margin-right="{$pageMarginRight}">
        <fo:region-body 
                column-count="{$columnCount}"
                margin-bottom="{$bodyMarginBottom}"
                margin-top="{$bodyMarginTop}"/>
        <fo:region-before  
                region-name="xsl-region-before-first"
                extent="{$regionBeforeExtent}"/>
        <fo:region-after 
                region-name="xsl-region-after-first"
                extent="{$regionAfterExtent}"/>
     </fo:simple-page-master>

<!-- setup for double-sided, 1 column, no first page -->
     <fo:page-sequence-master master-name="twoside1nofirst">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
                master-reference="right1"
                odd-or-even="odd"/>
        <fo:conditional-page-master-reference
                master-reference="left1"
                odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for double-sided, 1 column -->
     <fo:page-sequence-master master-name="twoside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
                master-reference="first1"
                page-position="first"/>
        <fo:conditional-page-master-reference
                master-reference="right1"
                odd-or-even="odd"/>
        <fo:conditional-page-master-reference
                master-reference="left1"
                odd-or-even="even"/>
        <fo:conditional-page-master-reference
                master-reference="blank1"
                blank-or-not-blank="blank"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for single-sided, 1 column -->
     <fo:page-sequence-master  master-name="oneside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
                master-reference="first1"
                page-position="first"/>
        <fo:conditional-page-master-reference 
                master-reference="simple1"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>


<!-- setup for double-sided, 2 column -->
     <fo:page-sequence-master master-name="twoside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
                master-reference="first2"
                page-position="first"/>
        <fo:conditional-page-master-reference
                master-reference="right2"
                odd-or-even="odd"/>
        <fo:conditional-page-master-reference
                master-reference="left2"
                odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for single-sided, 2 column -->
     <fo:page-sequence-master  master-name="oneside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
                master-reference="first2"
                page-position="first"/>
        <fo:conditional-page-master-reference 
                master-reference="simple2"
         />
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>
    <xsl:call-template name="hookDefinepagemasters"/>
    </fo:layout-master-set>
</xsl:template>


<xsl:template name="myheaders">
  <fo:static-content flow-name="xsl-region-before-right">
    <fo:block  text-align="justify" font-size="{$bodySize}">
       <fo:block border-after-style="solid" text-indent="0pt">
           <fo:inline font-style="italic">
             <xsl:value-of select="$PRID"/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline>
               <fo:page-number/>
           </fo:inline>
        </fo:block>
    </fo:block>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-before-left">
     <fo:block text-align="justify" font-size="{$bodySize}">
        <fo:block border-after-style="solid" text-indent="0pt">
           <fo:inline>
             <fo:page-number/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline font-style="italic">Activity Report INRIA <xsl:value-of select="$year"/>
           </fo:inline>
        </fo:block>
     </fo:block>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-before-first"/>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-right"/>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-left"/>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-first"/>
</xsl:template>

<!-- page sequence for the main text -->
<xsl:template name="maintext">
   <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
      >
      <fo:flow  
         flow-name="xsl-region-body"
         font-family="{$bodyFont}"  
         font-size="{$bodySize}">
        <xsl:call-template name="raweb.body"/>
      </fo:flow>
   </fo:page-sequence>
</xsl:template>


<!-- page sequence for the titlepage -->
<xsl:template match="accueil">
 <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
        force-page-count="end-on-even" >
     <xsl:text>&#xA;</xsl:text>
     <fo:flow font-style="italic" font-family="Times Roman" >
      <xsl:call-template name="accueil.body"/>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>

<!-- page sequence for the TOC --> 

<xsl:template name="myTOC">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
        force-page-count="end-on-even"
      > 
   <fo:flow flow-name="xsl-region-body" font-family="{$bodyFont}" >
    <fo:block>
      <xsl:call-template name="toc.body"/>
    </fo:block>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>


<!-- raweb is document element-->
<xsl:template match="raweb">
 <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
   <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="setupPagemasters"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="myheaders"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:apply-templates select="accueil"/> 
   <xsl:call-template name="myTOC"/>
   <xsl:call-template name="maintext"/>
 </fo:root>
</xsl:template>


<xsl:template name="raweb.body">
  <xsl:apply-templates select="composition"/>
  <xsl:apply-templates select="presentation"/>
  <xsl:apply-templates select="fondements"/>
  <xsl:apply-templates select="domaine"/>
  <xsl:apply-templates select="logiciels"/>
  <xsl:apply-templates select="resultats"/>
  <xsl:apply-templates select="contrats"/>
  <xsl:apply-templates select="international"/>
  <xsl:apply-templates select="diffusion"/>
  <xsl:apply-templates select="biblio"/>
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
    <xsl:apply-templates select='.' mode = 'xref'/>
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
     <xsl:when test="$level=4"><xsl:call-template name="setupDiv4"/></xsl:when>
    </xsl:choose>
    <xsl:call-template name="blockStartHook"/>
    <xsl:variable name="Number">
      <xsl:if test="$numberHeadings and $numberDepth &gt; $level">
        <xsl:call-template name="calculateNumber">
          <xsl:with-param name="numbersuffix" select="$headingNumberSuffix"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:variable>
    <xsl:value-of select="$Number"/>
    <xsl:apply-templates mode="section" select="head"/>
  </fo:block>
</xsl:template>

<!-- Composition. inserts the moreinfo paragraphs -->
<xsl:template match="composition">
   <xsl:call-template name="secNumberedHeading"/>
   <xsl:apply-templates select="/raweb/moreinfo"/>
   <xsl:apply-templates/>
</xsl:template>

<!-- Other section -->
<xsl:template 
    match="presentation|fondements|domaine|logiciels|resultats|contrats
  |international|diffusion">
   <xsl:call-template name="secNumberedHeading"/>
   <xsl:apply-templates/>
</xsl:template>

<!-- Modules -->

<xsl:template match="module">
 <xsl:if test="./head!='(Sans Titre)'">
  <xsl:call-template name="NumberedHeading">
    <xsl:with-param name="level" select="'1'"/>
   </xsl:call-template>
 </xsl:if>
 <xsl:apply-templates/>
</xsl:template>


<!--  Divisions -->
<xsl:template match="div2|div3|div4">
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
  <xsl:for-each select="//composition|//presentation|//fondements|
           //domaine|//logiciels|//resultats|//contrats|//international|
           //diffusion|//biblio|//module|//div2|//div3">
    <xsl:apply-templates mode="xtoc" select="(.)"/>
  </xsl:for-each>
</xsl:template>

<!-- Headings for section -->

<xsl:template mode="xtoc" match="composition|presentation|
    fondements|domaine|logiciels|resultats|contrats|international|
     diffusion|biblio">
  <xsl:variable name="tocindent">
    <xsl:value-of select="$div0Tocindent"/>
  </xsl:variable>
  <fo:block>
      <xsl:attribute name="font-weight">bold</xsl:attribute>
      <xsl:attribute name="text-indent">
       <xsl:value-of select="$tocindent"/>
      </xsl:attribute>
   <xsl:call-template name="sec.num"/>
   <xsl:text>&#x2003;</xsl:text>
   <fo:inline>
     <xsl:value-of select="@titre"/>
   </fo:inline>
    <fo:leader  leader-pattern="dots" />
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

<!-- Headings for modules -->

<xsl:template mode="xtoc" match="module">
 <xsl:if test="head!='(Sans Titre)'">
   <xsl:call-template name="tocheading">
    <xsl:with-param name="level">
        <xsl:value-of select="1"/></xsl:with-param>
   </xsl:call-template>
 </xsl:if>
</xsl:template>

<!-- Headings for divisions -->

<xsl:template mode="xtoc" match="div2|div3|div4">
   <xsl:call-template name="tocheading">
    <xsl:with-param name="level">
        <xsl:value-of select="substring-after(name(),'div')"/></xsl:with-param>
   </xsl:call-template>
</xsl:template>


<xsl:template name="tocheading">
  <xsl:param name="level"/>
  <xsl:variable name="tocindent">
    <xsl:choose>
      <xsl:when test="$level='0'">
        <xsl:value-of select="$div0Tocindent"/></xsl:when>
      <xsl:when test="$level='1'">
        <xsl:value-of select="$div1Tocindent"/></xsl:when>
      <xsl:when test="$level='2'">
        <xsl:value-of select="$div2Tocindent"/></xsl:when>
      <xsl:when test="$level='3'">
        <xsl:value-of select="$div3Tocindent"/></xsl:when>
      <xsl:when test="$level='4'">
        <xsl:value-of select="$div4Tocindent"/></xsl:when>
      <xsl:when test="$level='5'">
        <xsl:value-of select="$div5Tocindent"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="$div6Tocindent"/></xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="tocwidth">
    <xsl:choose>
      <xsl:when test="$level='0'">
        <xsl:value-of select="$div0Tocwidth"/></xsl:when>
      <xsl:when test="$level='1'">
        <xsl:value-of select="$div1Tocwidth"/></xsl:when>
      <xsl:when test="$level='2'">
        <xsl:value-of select="$div2Tocwidth"/></xsl:when>
      <xsl:when test="$level='3'">
        <xsl:value-of select="$div3Tocwidth"/></xsl:when>
      <xsl:when test="$level='4'">
        <xsl:value-of select="$div4Tocwidth"/></xsl:when>
      <xsl:when test="$level='5'">
        <xsl:value-of select="$div5Tocwidth"/></xsl:when>
      <xsl:otherwise><xsl:value-of select="$div6Tocwidth"/></xsl:otherwise>
    </xsl:choose>
  </xsl:variable>
  <xsl:variable name="Number">
    <xsl:call-template name="calculateNumber">
      <xsl:with-param name="numbersuffix" select="$tocNumberSuffix"/>
    </xsl:call-template>
  </xsl:variable>
  <fo:block toc='true' margin-right='15pt' text-indent ='{$tocindent}' margin-left ='{$tocwidth}'>
    <fotex:section-number width ='{$tocwidth}'>
      <fo:inline>
	<xsl:value-of select="$Number"/>
      </fo:inline> 
    </fotex:section-number>
    <xsl:if test="head!='(Sans Titre)'">
      <fo:inline>
	<xsl:apply-templates mode="tocsection" select="head"/>
      </fo:inline>
    </xsl:if>
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

<!-- Biblio. Il y a trois parties, des sous-parties -->
<xsl:template match="biblio">
   <xsl:call-template name="secNumberedHeading"/>
   <xsl:call-template name="biblioA"/>
   <xsl:call-template name='biblioBA'/>
   <xsl:call-template name='biblioBC'/>
   <xsl:call-template name='biblioBD'/>
   <xsl:call-template name='biblioBE'/>
   <xsl:call-template name='biblioBH'/>
   <xsl:call-template name='biblioBJ'/>
   <xsl:call-template name="biblioC"/>
</xsl:template>


<!-- Aux pour la biblio. Imprime le nom de la section -->
<xsl:template name="biblioname">
  <xsl:param name="name">Unknown bibliography section</xsl:param>
  <fo:block font-weight='bold' font-size="14pt" space-before="5pt" 
       keep-with-next='always'>
    <xsl:value-of select="$name"/>
  </fo:block>
</xsl:template>

<xsl:template name="biblioA">
  <xsl:if test="citation[@from ='refer']">
    <xsl:call-template name="biblioname">
      <xsl:with-param name="name">Major publications by the team in recent years</xsl:with-param>
    </xsl:call-template>
    <xsl:for-each select="citation[@from ='refer']">
       <xsl:apply-templates select="(.)"/>
    </xsl:for-each>
  </xsl:if>
</xsl:template>

<!-- Biblio de notes de bas de page -->
<xsl:template name="biblioC">
  <xsl:if test="citation[@from ='foot']">
    <xsl:call-template name="biblioname">
      <xsl:with-param name="name">Bibliography in notes</xsl:with-param>
    </xsl:call-template>
    <xsl:for-each select="citation[@from ='foot']">
      <xsl:apply-templates select="(.)"/>
    </xsl:for-each>
  </xsl:if>
</xsl:template>

<!-- Biblio livres -->
<xsl:template name='biblioBA'>
 <xsl:if test="citation[@from ='year' and 
      (@type='book' or @type='booklet' or @type='proceedings')]">
   <xsl:call-template name="biblioname">
     <xsl:with-param name="name">Books and Monographs</xsl:with-param>
   </xsl:call-template>
   <xsl:for-each select="citation[@from ='year' and 
         (@type='book' or @type='booklet' or @type='proceedings')]">
     <xsl:apply-templates select="(.)"/>
   </xsl:for-each>
 </xsl:if>
</xsl:template>

<xsl:template name='biblioBC'>
 <xsl:if test="citation[@from ='year' and @type='phdthesis']">
   <xsl:call-template name="biblioname">
     <xsl:with-param name="name">Doctoral dissertations and ``Habilitation'' theses</xsl:with-param>
   </xsl:call-template>
   <xsl:for-each select="citation[@from ='year'and  @type='phdthesis']">
    <xsl:apply-templates select="(.)"/>
   </xsl:for-each>
 </xsl:if>
</xsl:template>

<xsl:template name='biblioBD'>
 <xsl:if test="citation[@from ='year' and 
      (@type='article' or @type='inbook' or @type='incollection')]">
   <xsl:call-template name="biblioname">
   <xsl:with-param name="name">Articles in referred journals and book chapters</xsl:with-param>
  </xsl:call-template>
  <xsl:for-each select="citation[@from ='year'
       and (@type='article' or @type='inbook' or @type='incollection')]">
    <xsl:apply-templates select="(.)"/>
  </xsl:for-each>
 </xsl:if>
</xsl:template>

<xsl:template name='biblioBE'>
 <xsl:if test="citation[@from ='year' 
       and (@type='inproceedings' or @type='conference')]">
   <xsl:call-template name="biblioname">
   <xsl:with-param name="name">Publications in Conferences and Workshops</xsl:with-param>
  </xsl:call-template>
  <xsl:for-each select="citation[@from ='year'
       and (@type='inproceedings' or @type='conference')]">
    <xsl:apply-templates select="(.)"/>
  </xsl:for-each>
 </xsl:if>
</xsl:template>

<xsl:template name='biblioBH'>
  <xsl:if test="citation[@from ='year' and  
        (@type='manual' or @type='techreport' or @type='coursenotes')]">
    <xsl:call-template name="biblioname">
      <xsl:with-param name="name">Internal Reports</xsl:with-param>
    </xsl:call-template>
    <xsl:for-each select="citation[@from ='year' and  (@type='manual' or
                @type='techreport' or @type='coursenotes')]">
       <xsl:apply-templates select="(.)"/>
    </xsl:for-each>
  </xsl:if>
</xsl:template>

<xsl:template name='biblioBJ'>
  <xsl:if test="citation[@from ='year' and (@type='unpublished' 
      or @type='misc' or @type='masterthesis' or @type='mastersthesis')]">
    <xsl:call-template name="biblioname">
      <xsl:with-param name="name">Miscellaneous</xsl:with-param>
    </xsl:call-template>
    <xsl:for-each select="citation[@from ='year' and (@type='unpublished' 
       or @type='misc' or @type='masterthesis' or @type='mastersthesis')]">
      <xsl:apply-templates select="(.)"/>
    </xsl:for-each>
  </xsl:if>
</xsl:template>

<xsl:template match="citation">
  <fo:block space-before="15pt" text-indent="-2em"> 
     <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
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


<xsl:template match='bvolume'>
  <xsl:text>volume </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='bnumber'>
  <xsl:text>number </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='bpages'>
  <xsl:text>pages </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='bchapter'>
  <xsl:text>chapter </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='bseries'>
  <xsl:text>series </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match='bedition'>
  <xsl:text>edition </xsl:text><xsl:apply-templates/>
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
  <xsl:text>DOI </xsl:text> 
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


<xsl:template match='composition' mode='xref'>1</xsl:template>
<xsl:template match='presentation' mode='xref'>2</xsl:template>
<xsl:template match='fondements' mode='xref'>3</xsl:template>
<xsl:template match='domaine' mode='xref'>4</xsl:template>
<xsl:template match='logiciels' mode='xref'>5</xsl:template>
<xsl:template match='resultats' mode='xref'>6</xsl:template>
<xsl:template match='contrats' mode='xref'>7</xsl:template>
<xsl:template match='international' mode='xref'>8</xsl:template>
<xsl:template match='diffusion' mode='xref'>9</xsl:template>
<xsl:template match='biblio' mode='xref'>10</xsl:template>


<xsl:template name="sec.num">
   <xsl:apply-templates mode='xref' select = 
         "ancestor-or-self::*[self::composition or self::presentation or self::fondements or 
     self::domaine or self::logiciels or self::resultats or 
self::contrats or self::international or self::diffusion or self::biblio]" />
  <xsl:text>.</xsl:text>
</xsl:template>


<xsl:template name="calculateNumber">
   <xsl:param name="numbersuffix"/>
       <xsl:call-template name="sec.num"/> 
       <xsl:number level="multiple" from="raweb"
         count="module|div2|div3|div4"/>
       <xsl:value-of select="$numbersuffix"/>
</xsl:template>


<xsl:template mode="xref" match="module|div2|div3|div4">
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


<xsl:template match="table">
 <xsl:choose>
  <xsl:when test="@rend='inline'"> 
      <xsl:call-template name="blockTable"/>
  </xsl:when>
  <xsl:otherwise> <xsl:call-template name="floatTable"/> </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="floatTable">
  <fo:table-and-caption>
      <xsl:if test="rend='landscape'">
        <xsl:attribute name="reference-direction">-90</xsl:attribute>
      </xsl:if>
      <xsl:call-template name="addID"/>
      <fo:table-caption>
        <fo:block text-align="{$tableCaptionAlign}"
              space-after="{$spaceBelowCaption}">
           <xsl:value-of select="$tableWord"/>
           <xsl:call-template name="calculateTableNumber"/>
           <xsl:text>. </xsl:text>
           <xsl:apply-templates select="head"/>
        </fo:block>
      </fo:table-caption>
      <xsl:call-template name="blockTable"/>
   </fo:table-and-caption>
</xsl:template>


<xsl:template name="blockTable">
 <fo:table  text-align="{$tableAlign}">
   <xsl:call-template name="addID"/>
   <xsl:call-template name="deriveColSpecs"/>
   <fo:table-body text-indent="0pt">
     <xsl:for-each select="row">
       <xsl:text>&#xA;</xsl:text>
       <fo:table-row>
         <xsl:apply-templates select="cell"/>
       </fo:table-row>
      </xsl:for-each>
   </fo:table-body>
 </fo:table>
</xsl:template>

<xsl:template name="generateTableID">
  <xsl:choose>
    <xsl:when test="@id"> <xsl:value-of select="@id"/> </xsl:when>
    <xsl:otherwise> 
       <xsl:text>Table-</xsl:text><xsl:number level='any'/> 
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="deriveColSpecs" >
  <xsl:variable name="no">
     <xsl:call-template name="generateTableID"/>
  </xsl:variable>
  <xsl:call-template name="calculateTableSpecs"/>
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
     <fo:inline>
      <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
     <fotex:equation useleqno='{$nbleft}'>
    <xsl:for-each select="m:math">
     <xsl:apply-templates mode="math"/>
    </xsl:for-each>
    </fotex:equation></fo:inline>
  </xsl:when>
    <xsl:when test="@tag">
     <fo:inline>
      <xsl:attribute name="tag"><xsl:value-of select="@tag"/></xsl:attribute>
     <fotex:equation useleqno='{$nbleft}'>
    <xsl:for-each select="m:math">
     <xsl:apply-templates mode="math"/>
    </xsl:for-each>
    </fotex:equation></fo:inline>
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


<xsl:variable name="processor">
   <xsl:value-of select="system-property('xsl:vendor')"/>
</xsl:variable>

<xsl:template match = "URRocquencourt" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-rocq.en.html"
    >Rocquencourt</fo:basic-link>
</xsl:template>

<xsl:template match = "URRennes" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-ren.en.html"
    >Rennes</fo:basic-link>
</xsl:template>

<xsl:template match = "URSophia" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-sop.en.html"
    >Sophia Antipolis</fo:basic-link>
</xsl:template>

<xsl:template match = "URLorraine" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-lor.en.html"
    >Lorraine</fo:basic-link>
</xsl:template>

<xsl:template match = "URRhoneAlpes" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-ra.en.html"
    >Rhône-Alpes</fo:basic-link>
</xsl:template>
<xsl:template match = "URFuturs" mode ="intro">
  <fo:basic-link external-destination 
     ="http://www.inria.fr/inria/organigramme/fiche_ur-futurs.en.html"
    >Futurs</fo:basic-link>
</xsl:template>


<xsl:template name="accueil.body">
 <fo:INRIA year="{$year}"/>
 <fo:block font-size= "25pt" text-align="center"> 
    <fo:basic-link external-destination=
              "http://www.inria.fr/recherche/equipes/{@html}.en.html">
      <xsl:value-of select="$LeTypeProjet"/> 
      <xsl:text> </xsl:text>
      <xsl:value-of select="projet"/> 
    </fo:basic-link>
 </fo:block>
 <xsl:text>&#xA;</xsl:text>
 <fo:block font-size= "25pt" text-align="center" space-before="1cm"> 
     <xsl:value-of select="projetdeveloppe"/> 
 </fo:block>
 <xsl:text>&#xA;</xsl:text>
 <fo:block font-size= "17.28pt" text-align="center" space-before="1cm">
    <xsl:for-each select ="UR/*">
       <xsl:apply-templates select="." mode ='intro'/>
       <xsl:if test="position()!=last()"> - </xsl:if>
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

<xsl:include href="raweb3-table.xsl"/>
<xsl:include href="raweb3-makecolspec.xsl"/>

</xsl:stylesheet>

<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
