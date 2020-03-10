<?xml version="1.0" encoding="iso-8859-1"?>

<!--  Style sheet for the thesis English version 

  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->

<!-- $Id: thesishtml.xsl,v 1.4 2007/01/17 16:20:02 grimm Exp $ -->

<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML"
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:html="http://www.w3.org/1999/xhtml" 
  xmlns="http://www.w3.org/1999/xhtml" 
  exclude-result-prefixes="m html xlink" >

<xsl:import href="cls.xsl"/>
<xsl:import href="thesis.xsl"/>
<xsl:output method='xml' encoding='iso-8859-1' 
	    doctype-public='-//W3C//DTD XHTML 1.0 Strict//EN'
	    doctype-system='http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd'
/>

<!-- ignore spaces in the elements from the list -->
<xsl:strip-space elements="cell bediteur bauteurs citation UR"/>

<xsl:variable name="prefix">thesis</xsl:variable>
<xsl:variable name="split">true</xsl:variable>


<xsl:variable name="TOCname">Table of Contents</xsl:variable>
<xsl:variable name="Bibliographyname">Bibliography</xsl:variable>
<xsl:variable name="Lieuname">Location</xsl:variable>
<xsl:variable name="Frenchtitlename">French title</xsl:variable>
<xsl:variable name="Typethesename">PhD in</xsl:variable>
<xsl:variable name="Directorname">Supervisor</xsl:variable>
<xsl:variable name="Doctorantname">Doctorant</xsl:variable>
<xsl:variable name="Datesoumissionname">Submitted on  </xsl:variable>
<xsl:variable name="Juryname">Jury</xsl:variable>

<xsl:template name="author">
  <xsl:apply-templates select="/std/metadata/doctorant" mode="text"/>
</xsl:template>
<xsl:template name="title">
  <xsl:apply-templates select="/std//metadata/thesetitre" mode="text"/>
</xsl:template>
<xsl:template name="keywords">
  <xsl:text>verb english history</xsl:text>
</xsl:template>


</xsl:stylesheet>
<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
