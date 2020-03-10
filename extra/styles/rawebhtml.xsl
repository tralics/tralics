<?xml version="1.0" encoding="iso-8859-1"?>

<!--  feuille de style raweb pour generer du HTML -->
<!-- $Id: rawebhtml.xsl,v 1.1 2006/09/05 17:36:14 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:html="http://www.w3.org/1999/xhtml" 
  exclude-result-prefixes="m html xlink" >



  <xsl:output method='html' encoding='iso-8859-1' doctype-public='-//W3C//DTD HTML 4.0//EN'/>

 <!-- ignorer les espaces dans les éléments de la liste -->
  <xsl:strip-space elements="cell bediteur bauteurs citation UR"/>



<!-- enable, for new elements to get commented as such-->
<xsl:template match="*">
  <xsl:comment><xsl:text>PASS THROUGH </xsl:text>
    <xsl:value-of select="name()"/>
  </xsl:comment>
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="std">
  <html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
  <meta name="Author" content="Jose Grimm" />
  <title>these</title>
  <link rel="stylesheet" href="tralics.css" />
  <meta name="keywords" content="latex xml translator" />
</head>

    <body>
     <xsl:apply-templates/>
</body></html>
</xsl:template>

<xsl:template match="p">
  <p>  <xsl:apply-templates/></p>
</xsl:template>

<!-- maths-->


<xsl:template match="m:math">
 <m:math>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates mode="math"/>
 </m:math>
</xsl:template>

<xsl:template match="m:*|@*|text()" mode="math">
 <xsl:copy>
   <xsl:apply-templates mode="math" select="*|text()"/>
 </xsl:copy>
</xsl:template>


  <xsl:template match="formula">
    <xsl:choose>
      <xsl:when test="@type = 'display' and @id">
        <div align="center" class="mathdisplay">
           <a name="{@id}" />
          <table width='100%'>
            <tr valign='middle'>
              <td align='center'>
                <xsl:apply-templates />
              </td>
              <td class="eqno" width="10" align="right">(<xsl:number level="any" count="formula[@id]" />)</td>
            </tr>
          </table>
        </div>
      </xsl:when>
      <xsl:when test="@type = 'display'">
        <div align="center" class="mathdisplay">
          <xsl:apply-templates />
        </div>
      </xsl:when>
      <xsl:otherwise>
        <span class="math">
          <xsl:apply-templates />
        </span>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

<xsl:template match="simplemath">
  <i><xsl:apply-templates/></i>
</xsl:template>

</xsl:stylesheet>
<!-- Keep this comment at the end of the file
Local variables:
mode: xml
sgml-omittag:nil
sgml-shorttag:nil
sgml-namecase-general:nil
sgml-general-insert-case:lower
sgml-minimize-attributes:nil
sgml-always-quote-attributes:t
sgml-indent-step:2
sgml-indent-data:t
sgml-parent-document:nil
sgml-exposed-tags:nil
sgml-local-catalogs:nil
sgml-local-ecat-files:nil
End:
-->
