<?xml version="1.0" encoding="iso-8859-1"?>

<!--  Style sheet for the thesis (multilingual version)
      to be included in thesehtml.xsl or thesishtml.xsl

  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
 -->
<!-- $Id: thesis.xsl,v 1.5 2007/01/17 18:14:26 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:html="http://www.w3.org/1999/xhtml" 
  xmlns="http://www.w3.org/1999/xhtml" 
  exclude-result-prefixes="m html xlink" >

<xsl:output method='xml' encoding='iso-8859-1' doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
/>

<xsl:variable name="shorttoc">false</xsl:variable>

<!-- ignore spaces in the elements from the list -->
<xsl:strip-space elements="cell bediteur bauteurs citation UR"/>
<!-- Divisions -->

<xsl:template name="calculateNumber">
 <xsl:if test= "ancestor::mainmatter">
   <xsl:number level="multiple" from="/mainmatter" grouping-separator="."
               count="div0|div1|div2|div3|div4|div5"/>
 </xsl:if>
</xsl:template>

<xsl:template name="calculateNumberSpace">
  <xsl:if test= "ancestor::mainmatter">
    <xsl:call-template name="calculateNumber"/>
    <xsl:text>. </xsl:text>
  </xsl:if>
</xsl:template>

<xsl:template match="p">
  <p>  
    <xsl:if test="@noindent = 'true'">
      <xsl:attribute name="class">nofirst noindent</xsl:attribute>
    </xsl:if>
    <xsl:apply-templates/>
  </p>
</xsl:template>

<xsl:template match="metadata">
  <h1><xsl:value-of select="thesetitre"/></h1>
  <blockquote class="thesis-heading">
    <table>
      <xsl:apply-templates  />
    </table>
  </blockquote>
</xsl:template>

<xsl:template match="thesetitre"/>

<xsl:template match="lieuthese">
 <tr> 
   <td><xsl:value-of select="$Lieuname"/></td>
   <td><xsl:apply-templates/></td>
 </tr>
</xsl:template>

<xsl:template match="lieuthesesuite">
 <tr><td></td><td><xsl:apply-templates/></td></tr>
</xsl:template>

<xsl:template match="titrefrancais">
 <tr>
   <td><xsl:value-of select="$Frenchtitlename"/></td>
   <td><xsl:apply-templates/></td>
 </tr>
</xsl:template>

<xsl:template match="typethese">
  <tr>
    <td><xsl:value-of select="$Typethesename"/></td>
    <td><xsl:apply-templates/></td>
  </tr>
</xsl:template>

<xsl:template match="directeur">
  <tr>
    <td><xsl:value-of select="$Directorname"/></td>
    <td><xsl:apply-templates/></td>
  </tr>
</xsl:template>

<xsl:template match="doctorant">
 <tr>
   <td><xsl:value-of select="$Doctorantname"/></td>
   <td><xsl:apply-templates/></td>
 </tr>
</xsl:template>

<xsl:template match="datesoumission">
  <tr>
    <td><xsl:value-of select="$Datesoumissionname"/></td>
    <td><xsl:apply-templates/></td>
  </tr>
</xsl:template>

<xsl:template match="jury">
  <tr>
    <td><xsl:value-of select="$Juryname"/></td>
    <td>
      <table> 
        <xsl:apply-templates/>
      </table>
    </td>
  </tr>
</xsl:template>

<xsl:template match="membre">
  <tr>
    <td><xsl:value-of select="@type"/></td>
    <td><xsl:value-of select="@prenom"/></td>
    <td><xsl:value-of select="@nom"/></td>
    <td><xsl:apply-templates/></td>
  </tr>
</xsl:template>

<xsl:template match="dedicace">
 <blockquote><p><i>   <xsl:apply-templates/></i></p></blockquote>
</xsl:template>

<xsl:template match="img">
 <img>  
 <xsl:copy-of select="@*"/>  
 </img>
</xsl:template>

<xsl:template name="header">
</xsl:template>

<xsl:template name="author">
  <xsl:apply-templates select="/std/metadata/doctorant" mode="text"/>
</xsl:template>
<xsl:template name="title">
  <xsl:apply-templates select="/std//metadata/thesetitre" mode="text"/>
</xsl:template>

</xsl:stylesheet>
<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
