<?xml version="1.0" encoding="iso-8859-1"?>

<!--  Style sheet for the thesis French version 

  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
-->
<!-- $Id: thesehtml.xsl,v 1.8 2007/01/17 18:14:26 grimm Exp $ -->

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

<xsl:variable name="prefix">these</xsl:variable>
<xsl:variable name="split">true</xsl:variable>

<xsl:variable name="TOCname">Table des matières</xsl:variable>
<xsl:variable name="Bibliographyname">Bibliographie</xsl:variable>
<xsl:variable name="Lieuname">Lieu</xsl:variable>
<xsl:variable name="Frenchtitlename">Titre Français</xsl:variable>
<xsl:variable name="Typethesename">Doctorat en</xsl:variable>
<xsl:variable name="Directorname">Thèse dirigée par</xsl:variable>
<xsl:variable name="Doctorantname">Doctorant</xsl:variable>
<xsl:variable name="Datesoumissionname">Date de soutenance</xsl:variable>
<xsl:variable name="Juryname">Jury</xsl:variable>

<xsl:template name="keywords">
  <xsl:text>verbe anglais histoire</xsl:text>
</xsl:template>

</xsl:stylesheet>
<!-- Keep this comment at the end of the file
Local variables:
mode: xml
End:
-->
