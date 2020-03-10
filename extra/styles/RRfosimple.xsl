<?xml version="1.0" encoding="iso-8859-1"?>

<!--  Style sheet for converting RR to FO
  Simplified Version. This translates the <header> element  (front page) 


  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->
<!-- $Id: RRfosimple.xsl,v 1.2 2007/05/02 07:15:46 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
>

<xsl:template name="header">
  <xsl:apply-templates select="title" mode="intro"/>
  <xsl:apply-templates select="ftitle" mode="intro"/>
  <xsl:apply-templates select="author" mode="intro"/>
  <xsl:apply-templates select="location" mode="intro"/>
  <xsl:apply-templates select="theme" mode="intro"/>
  <xsl:apply-templates select="rrnumber" mode="intro"/>
  <xsl:apply-templates select="version-number" mode="intro"/>
  <xsl:apply-templates select="inria-team" mode="intro"/>
  <xsl:apply-templates select="date" mode="intro"/>
  <xsl:apply-templates select="rev-date" mode="intro"/>
  <xsl:apply-templates select="keyword" mode="intro"/>
  <xsl:apply-templates select="motcle" mode="intro"/>
  <xsl:apply-templates select="abstract" mode="intro"/>
  <xsl:apply-templates select="resume" mode="intro"/>
  <xsl:apply-templates select="RRnote" mode="intro"/>
  <xsl:apply-templates select="RRemail" mode="intro"/>
  <hr/>
</xsl:template>



<xsl:template match="title"/>
<xsl:template match="abstract"/>
<xsl:template match="ftitle"/>
<xsl:template match="resume"/>
<xsl:template match="author"/>
<xsl:template match="inria-team"/>
<xsl:template match="rrnumber"/>
<xsl:template match="version-number"/>
<xsl:template match="theme"/>
<xsl:template match="rev-date"/>
<xsl:template match="date"/>
<xsl:template match="location"/>
<xsl:template match="keyword"/>
<xsl:template match="motcle"/>
<xsl:template match="titlehead"/>
<xsl:template match="authorhead"/>
<xsl:template match="RRnote"/>
<xsl:template match="RRemail"/>



<xsl:template match="title" mode='text'>
  <xsl:apply-templates/>
</xsl:template>

<!-- Real translation in mode  Intro -->

<xsl:template match="title" mode='intro'>
  <fo:block font-size="14pt" text-align="center" font-weight="bold" 
	    space-after="20pt">
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="ftitle" mode='intro'>
  <fo:block>French title: <xsl:apply-templates/></fo:block>
</xsl:template>


<xsl:template match="abstract" mode='intro'>
  <fo:block keep-with-next.within-page="always">
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/>
    </xsl:attribute>
    <xsl:call-template name="setupDiv1"/>
    <fo:inline>Abstract</fo:inline>
  </fo:block>
  <fo:block><xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="resume" mode='intro'>
  <fo:block keep-with-next.within-page="always">
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/>
    </xsl:attribute>
    <xsl:call-template name="setupDiv1"/>
    <fo:inline>French Abstract</fo:inline>
  </fo:block>
  <fo:block><xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="author" mode='intro'>
  <fo:block> Author: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="inria-team" mode='intro'>
  <fo:block> Team: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="rrnumber" mode='intro'>
  <fo:block>Inria Research Report Number: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="version-number" mode='intro'>
  <fo:block>Revision: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="theme" mode='intro'>
  <fo:block>Inria Research Theme: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="date" mode='intro'>
  <fo:block> Date: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="rev-date" mode='intro'>
  <fo:block>Revised Date: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="location" mode='intro'>
  <fo:block> Location: <xsl:apply-templates/></fo:block>
</xsl:template>

<xsl:template match="keyword" mode='intro'>
  <fo:block>Keywords: <xsl:apply-templates/>.</fo:block>
</xsl:template>

<xsl:template match="motcle" mode='intro'>
  <fo:block>French keywords: <xsl:apply-templates/>.</fo:block>
</xsl:template>


<xsl:template match="RRemail" mode='intro'>
  <fo:block>Email: <xsl:apply-templates/>.</fo:block>
</xsl:template>

</xsl:stylesheet>
