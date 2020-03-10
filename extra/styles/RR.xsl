<?xml version="1.0" encoding="iso-8859-1"?>

<!--  feuille de style raweb pour generer du HTML
   Headet for RR.sty
  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->
<!-- $Id: RR.xsl,v 1.2 2007/05/02 07:15:46 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns="http://www.w3.org/1999/xhtml"> 

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
  <xsl:apply-templates select="RRnote" mode="intro"/>
  <xsl:apply-templates select="RRemail" mode="intro"/>
  <xsl:apply-templates select="abstract" mode="intro"/>
  <xsl:apply-templates select="resume" mode="intro"/>
  <hr/>
</xsl:template>

<!-- Default translation is empty -->

<xsl:template match="title"/>
<xsl:template match="abstract"/>
<xsl:template match="ftitle"/>
<xsl:template match="resume"/>
<xsl:template match="author"/>
<xsl:template match="inria-team"/>
<xsl:template match="rrnumber"/>
<xsl:template match="version-number"/>
<xsl:template match="theme"/>
<xsl:template match="date"/>
<xsl:template match="rev-date"/>
<xsl:template match="location"/>
<xsl:template match="keyword"/>
<xsl:template match="motcle"/>
<xsl:template match="titlehead"/>
<xsl:template match="authorhead"/>
<xsl:template match="RRnote"/>
<xsl:template match="RRemail"/>


<!-- The title of the document can be used elsewhere-->
<xsl:template match="title" mode='text'>
  <xsl:apply-templates/>
</xsl:template>

<!-- Real translation in mode  Intro -->


<xsl:template match="title" mode='intro'>
  <h1><xsl:apply-templates/></h1>
</xsl:template>


<xsl:template match="abstract" mode='intro'>
  <h2>Abstract</h2><xsl:apply-templates/>
</xsl:template>

<xsl:template match="ftitle" mode='intro'>
  <p>French title: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="resume" mode='intro'>
  <h2>French Abstract</h2><xsl:apply-templates/>
</xsl:template>

<xsl:template match="author" mode='intro'>
  <p> Author: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="inria-team" mode='intro'>
  <p> Team: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="rrnumber" mode='intro'>
  <p>Inria Research Report Number: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="version-number" mode='intro'>
  <p>Revision: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="theme" mode='intro'>
  <p>Inria Research Theme: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="date" mode='intro'>
  <p> Date: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="rev-date" mode='intro'>
  <p>Revised Date: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="location" mode='intro'>
  <p> Location: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="keyword" mode='intro'>
  <p>Keywords: <xsl:apply-templates/>.</p>
</xsl:template>

<xsl:template match="motcle" mode='intro'>
  <p>French keywords: <xsl:apply-templates/>.</p>
</xsl:template>

<xsl:template match="RRnote" mode='intro'>
  <p>Note: <xsl:apply-templates/>.</p>
</xsl:template>

<xsl:template match="RRemail" mode='intro'>
  <p>Author's email address: <xsl:apply-templates/>.</p>
</xsl:template>

</xsl:stylesheet>
