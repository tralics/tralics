<?xml version="1.0" encoding="iso-8859-1"?>
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:html="http://www.w3.org/1999/xhtml" 
  xmlns:fo="http://www.w3.org/1999/XSL/Format" 
  exclude-result-prefixes="m html xlink fo" >


<xsl:strip-space elements="cell row table"/>

<!-- 
$Id: extract.xsl,v 2.4 2007/01/24 18:30:17 grimm Exp $
Style sheet for extraction of TREES 
Copyright INRIA/APICS 2006-2007, Jos\'e Grimm
Licensed under the CeCILL Free Software Licensing Agreement
-->

<xsl:output method='xml' encoding='iso-8859-1'/>

<xsl:template match="std">
  <fo:root>
    <fo:block>
      <xsl:apply-templates select ="//table | //preview "/>
    </fo:block>  
  </fo:root>
</xsl:template>

<xsl:template match="preview">
  <xsl:if test="table/row/cell/node">
    <xsl:text>&#xA;</xsl:text>  
    <tree>
      <xsl:copy-of select="@*"/>  
      <xsl:apply-templates mode="preview"/>
    </tree>
  </xsl:if>
</xsl:template>


<xsl:template match="preview/table"/>

<xsl:template name="table">
  <table>
     <xsl:copy-of select="@*"/>  
     <xsl:apply-templates/>  
  </table>
  <xsl:call-template name="extractconnectors"/>
</xsl:template>

<xsl:template match="table">
  <xsl:if test="row/cell/node">
    <xsl:text>&#xA;</xsl:text>  
    <tree>
      <xsl:call-template name="table" select="."/>
    </tree>
  </xsl:if>
</xsl:template>

<xsl:template match="table" mode="preview">
  <xsl:call-template name="table" select="."/>
</xsl:template>

<xsl:template match="row">
  <row>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates />  
  </row>
</xsl:template>

<xsl:template match="cell">
  <cell>
    <xsl:copy-of select="@*"/>
    <xsl:apply-templates mode="copy"/>  
  </cell>
</xsl:template>


<xsl:template name="extractconnectors">
  <xsl:for-each select="row/cell/nodeconnect|row/cell/anodeconnect|
   row/cell/barnodeconnect|row/cell/abarnodeconnect | row/cell/nodetriangle
			|row/cell/nodecurve|row/cell/anodecurve">
    <xsl:copy-of select="."/>
  </xsl:for-each>
</xsl:template>

<xsl:template mode="copy"
 match="nodeconnect|anodeconnext|barnodeconnect| abarnodeconnect
	|nodecurve|anodecurve|nodetriangle"/>

<xsl:template match="*|@*|text()" mode="copy">
 <xsl:copy>
   <xsl:apply-templates mode="copy" select="*|@*|text()"/>
 </xsl:copy>
</xsl:template>

</xsl:stylesheet>


