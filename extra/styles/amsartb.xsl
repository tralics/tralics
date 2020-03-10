<?xml version="1.0" encoding="iso-8859-1"?>

<!--
  Style file for a paper using amsart class file.
  Copyright Inria/Apics (José Grimm) 2008
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->
<!-- $Id: amsartb.xsl,v 1.5 2008/09/25 15:49:13 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns:html="http://www.w3.org/1999/xhtml" 
  xmlns="http://www.w3.org/1999/xhtml" 
  exclude-result-prefixes="m html" >

<xsl:import href="clsb.xsl"/>
<xsl:import href="amsart.xsl"/>

<xsl:output method='xml' encoding='iso-8859-1' 
  doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
  doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
/>


<xsl:variable name="separate-index">true</xsl:variable>
<xsl:variable name="separate-footnote">true</xsl:variable>
<xsl:variable name="prefix">
 <xsl:value-of select="/std/@htmlprefix"/>
</xsl:variable>

<xsl:variable name="split">false</xsl:variable>
<xsl:variable name="author"><xsl:value-of select="/std/Metadata/author"/></xsl:variable>

<xsl:template name="author">
  <xsl:apply-templates select="//author" mode="text"/>
</xsl:template>
<xsl:template name="title">
  <xsl:apply-templates select="/std/Metadata/title" mode="text"/>
</xsl:template>
<xsl:template name="keywords">
  <xsl:apply-templates select="/std/Metadata/keywords" mode="text"/>
</xsl:template>

<!-- Divisions -->

<xsl:template name="calculateNumber">
  <xsl:number level="multiple" from="/std" grouping-separator="."
              count="div0|div1|div2|div3|div4|div5"/>
</xsl:template>

<xsl:template name="calculateNumberSpace">
  <xsl:choose>
    <xsl:when test="@id-text">
    <xsl:value-of select="@id-text"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="calculateNumber"/> 
    </xsl:otherwise>
  </xsl:choose>
  <xsl:text>. </xsl:text>
</xsl:template>

<xsl:template match="ftitle"/>
<xsl:template match="ftitle" mode='intro'>
  <p>French title: <xsl:apply-templates/></p>
</xsl:template>

<xsl:template match="p">
  <xsl:choose>
    <xsl:when test="parent::pre">
      <xsl:apply-templates/>
    </xsl:when>
    <xsl:when test="parent::theorem and position()=2">
     <p>
      <xsl:apply-templates select=".." mode="first-par"/>
      <xsl:apply-templates/>
     </p>
    </xsl:when> 
     <xsl:when test="parent::theorem and position()=3 and preceding-sibling::alt_head[1]">
     <p>
      <xsl:apply-templates select=".." mode="first-par"/>
      <xsl:apply-templates/>
     </p>
    </xsl:when> 
   <xsl:when test="parent::div1 and position()=2">
     <p class='nofirst'>
      <xsl:apply-templates select=".." mode="first-par"/>
      <xsl:apply-templates/>
     </p>
    </xsl:when>
    <xsl:otherwise>
      <p>  
	<xsl:if test="@noindent = 'true'">
	  <xsl:attribute name="class">nofirst noindent</xsl:attribute>
	</xsl:if>
	<xsl:if test="@rend = 'center'">
	  <xsl:attribute name="style">text-align:center</xsl:attribute>
	</xsl:if>
	<xsl:if test="@class">
	  <xsl:attribute name="class"><xsl:value-of select="@class"/></xsl:attribute>
	</xsl:if>
	<xsl:apply-templates/>
      </p>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="pre">
  <pre>
    <xsl:if test="@class">
      <xsl:attribute name="class">
        <xsl:value-of select="@class"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </pre>
  <xsl:text>&#x0A;</xsl:text>
</xsl:template>


<xsl:template match="xmlelt">
  <tt class='txt'>&lt;<xsl:apply-templates/>&gt;</tt>
</xsl:template>

<xsl:template match="latexcode">
  <samp><xsl:apply-templates/></samp>
</xsl:template>

<xsl:template match="xmlcode">
  <tt class='txt'><xsl:apply-templates/></tt>
</xsl:template>

<xsl:template match="xmlcolor">
  <span class='txt'><xsl:apply-templates/></span>
</xsl:template>


<!-- ignore spaces in the elements from the list -->
<xsl:strip-space elements="cell bediteur bauteurs citation UR theorem div1"/>

</xsl:stylesheet>

