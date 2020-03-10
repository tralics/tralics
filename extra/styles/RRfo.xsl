<?xml version="1.0" encoding="iso-8859-1"?>

<!--  Style sheet for converting RR to FO
  Full Version. Results looks like a real RR report


  Copyright Inria/Apics (José Grimm) 2006-2007
  Licensed under the CeCILL Free Software Licensing Agreement
  see http://www.cecill.info. 
  $Id: RRfo.xsl,v 1.4 2007/05/02 15:20:07 grimm Exp $
-->


<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
>


<!--Read the style sheet that does everything, then modify what is required-->
<xsl:import href="clsfo.xsl"/>

<xsl:param name="maintextMasterReference">twoside1nofirst</xsl:param>
<xsl:param name="tocMasterReference">twoside1nofirst</xsl:param>

<xsl:param name="maintextForcePagecount">end-on-even</xsl:param>
<xsl:param name="tocForcePagecount">auto</xsl:param>
<xsl:param name="maintextInitialPageNumber">1</xsl:param>
<xsl:param name="tocInitialPageNumber">auto</xsl:param>
<xsl:param name="headingOutdent">0pt</xsl:param>

<xsl:variable name="rrnumber">
  <xsl:choose>
    <xsl:when test="/std/rrnumber"> 
      <xsl:value-of select="/std/rrnumber"/>
    </xsl:when>
    <xsl:otherwise>????</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="isRR">
  <xsl:choose>
    <xsl:when test="/std/@RR-type='RR'">true</xsl:when>
    <xsl:otherwise>false</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="issn.value">
  <xsl:choose>
    <xsl:when test="/std/@RR-type='RR'">ISSN 0249-6399</xsl:when>
    <xsl:otherwise>ISSN 0249-0803</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="isfrench">
  <xsl:choose>
    <xsl:when test="/std/@RR-lang='french'">true</xsl:when>
    <xsl:otherwise>false</xsl:otherwise>
  </xsl:choose>
</xsl:variable>


<!-- std is document element-->
<xsl:variable name="Lefthead">
  <xsl:value-of select="/std/title"/>
</xsl:variable>

<xsl:variable name="Righthead">
  <xsl:value-of select="/std/author"/>
</xsl:variable>


<!-- We put the TOC at the end-->
<xsl:template match="std">
 <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
   <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="setupPagemasters"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="myheaders"/>
  <xsl:text>&#xA;</xsl:text>
   <xsl:call-template name="titlepage"/> 
   <xsl:call-template name="maintext"/>
   <xsl:call-template name="toc"/>
   <xsl:call-template name="lastpage"/> 
 </fo:root>
</xsl:template>


<!-- We put the TOC at the end of the document -->

<xsl:template name="toc">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        master-reference="twoside1nofirst"
      > 
   <fo:flow flow-name="xsl-region-body" font-family="{$bodyFont}" >
    <fo:block>
      <xsl:call-template name="toc.body"/>
    </fo:block>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>


<xsl:template name="odd-footer">
  <fo:block text-align="justify" 
	    font-family="{$bodyFont}"
	    text-indent="0pt" font-size="{$bodySize}">
    <fo:inline>
      <xsl:value-of select="/std/@RR-type"/>
      <xsl:text> n° </xsl:text>
      <xsl:value-of select="$rrnumber"/>
    </fo:inline>
    <fo:leader rule-thickness="0pt"/>
  </fo:block>
</xsl:template>

<xsl:template name="even-footer">
  <fo:block text-align="justify" font-size="{$bodySize}"
	    font-family="{$bodyFont}"  text-indent="0pt" >
    <fo:leader rule-thickness="0pt"/>
    <fo:inline>INRIA</fo:inline>
  </fo:block>
</xsl:template>

<xsl:template name="first-footer">
  <xsl:call-template name="odd-footer"/>
</xsl:template>

<!-- Last page  -->

<xsl:template name="FutursZ">
  <fo:block  font-size="8pt">
    Unité de recherche INRIA Futurs :
    Parc Club Orsay Université - ZAC des Vignes
  </fo:block>
  <fo:block  font-size="8pt">
    4, rue Jacques Monod - 91893 ORSAY Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="FutursO">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Futurs
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    Parc Club Orsay Université - ZAC des Vignes 
  </fo:block>
  <fo:block text-align="center" font-size="10pt" space-after="3pt">
    4, rue Jacques Monod - 91893 ORSAY Cedex (France)
  </fo:block>
</xsl:template>
 
<xsl:template name="LorraineZ">
  <fo:block  font-size="8pt">
    Unité de recherche INRIA Lorraine :
    LORIA, Technopôle de Nancy-Brabois - Campus scientifique
  </fo:block>
  <fo:block  font-size="8pt">
    615, rue du Jardin Botanique - BP 101 - 
   54602 Villers-lès-Nancy Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="LorraineO">
  <fo:block font-size="10pt">
    Unité de recherche INRIA Lorraine 
  </fo:block>
  <fo:block  font-size="10pt">
    LORIA, Technopôle de Nancy-Brabois - Campus scientifique
  </fo:block>
  <fo:block font-size="10pt" space-after="3pt">
   615, rue du Jardin Botanique - BP 101 - 
   54602 Villers-lès-Nancy Cedex (France)
  </fo:block>
</xsl:template>
 
<xsl:template name="RennesZ">
  <fo:block  font-size="8pt">
    Unité de recherche INRIA Rennes :
    IRISA, Campus universitaire de Beaulieu - 35042 Rennes Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="RennesO">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Rennes 
  </fo:block>
  <fo:block font-size="10pt" space-after="3pt">
    IRISA, Campus universitaire de Beaulieu - 35042 Rennes Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="RhoneAlpesZ">
  <fo:block  font-size="8pt">
    Unité de recherche INRIA Rhône-Alpes :
    655, avenue de l'Europe -  38334  Montbonnot Saint-Ismier (France)
  </fo:block>
</xsl:template>

<xsl:template name="RhoneAlpesO">
  <fo:block  font-size="10pt">
    Unité de recherche INRIA Rhône-Alpes 
  </fo:block>
  <fo:block  font-size="10pt" space-after="3pt">
    655, avenue de l'Europe -  38334  Montbonnot Saint-Ismier (France)
  </fo:block>
</xsl:template>
 
<xsl:template name="RocqZ">
  <fo:block font-size="8pt">
    Unité de recherche INRIA Rocquencourt :
    Domaine de Voluceau - Rocquencourt - BP 105 - 
    78153 Le Chesnay Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="RocqO">
  <fo:block font-size="10pt">
    Unité de recherche INRIA Rocquencourt 
  </fo:block>
  <fo:block font-size="10pt" space-after="3pt">
    Domaine de Voluceau - Rocquencourt - BP 105 - 
    78153 Le Chesnay Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="SophiaZ">
  <fo:block font-size="8pt">
    Unité de recherche INRIA Sophia Antipolis :

    2004, route des Lucioles -
    BP 93 - 06902 Sophia Antipolis Cedex (France)
  </fo:block>
</xsl:template>

<xsl:template name="SophiaO">
  <fo:block font-size="10pt">
    Unité de recherche INRIA Sophia Antipolis 
  </fo:block>
  <fo:block font-size="10pt" space-after="3pt">
    2004, route des Lucioles -
    BP 93 - 06902 Sophia Antipolis Cedex (France)
  </fo:block>
</xsl:template>


<xsl:template name="FutursX">
  <xsl:call-template name="FutursO"/>
  <xsl:call-template name="LorraineZ"/>
  <xsl:call-template name="RennesZ"/>
  <xsl:call-template name="RhoneAlpesZ"/>
  <xsl:call-template name="RocqZ"/>
  <xsl:call-template name="SophiaZ"/>
</xsl:template>

<xsl:template name="LorraineX">
  <xsl:call-template name="LorraineO"/>
  <xsl:call-template name="FutursZ"/>
  <xsl:call-template name="RennesZ"/>
  <xsl:call-template name="RhoneAlpesZ"/>
  <xsl:call-template name="RocqZ"/>
  <xsl:call-template name="SophiaZ"/>
</xsl:template>

<xsl:template name="RennesX">
  <xsl:call-template name="RennesO"/>
  <xsl:call-template name="FutursZ"/>
  <xsl:call-template name="LorraineZ"/>
  <xsl:call-template name="RhoneAlpesZ"/>
  <xsl:call-template name="RocqZ"/>
  <xsl:call-template name="SophiaZ"/>
</xsl:template>

<xsl:template name="RhoneAlpesX">
  <xsl:call-template name="RhoneAlpesO"/>
  <xsl:call-template name="FutursZ"/>
  <xsl:call-template name="LorraineZ"/>
  <xsl:call-template name="RennesZ"/>
  <xsl:call-template name="RocqZ"/>
  <xsl:call-template name="SophiaZ"/>
</xsl:template>

<xsl:template name="RocqX">
  <xsl:call-template name="RocqO"/>
  <xsl:call-template name="FutursZ"/>
  <xsl:call-template name="LorraineZ"/>
  <xsl:call-template name="RennesZ"/>
  <xsl:call-template name="RhoneAlpesZ"/>
  <xsl:call-template name="SophiaZ"/>
</xsl:template>

<xsl:template name="SophiaX">
  <xsl:call-template name="SophiaO"/>
  <xsl:call-template name="FutursZ"/>
  <xsl:call-template name="LorraineZ"/>
  <xsl:call-template name="RennesZ"/>
  <xsl:call-template name="RhoneAlpesZ"/>
  <xsl:call-template name="RocqZ"/>
</xsl:template>


<xsl:template name="outURX">
  <xsl:choose>
    <xsl:when test="/std/@UR-val='Lorraine'">
      <xsl:call-template name="LorraineX"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Rennes'">
      <xsl:call-template name="RennesX"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='RhoneAlpes'">
      <xsl:call-template name="RhoneAlpesX"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Rocq'">
      <xsl:call-template name="RocqX"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Sophia'">
      <xsl:call-template name="SophiaX"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="FutursX"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="outURT">
  <xsl:choose>
    <xsl:when test="/std/@UR-val='Lorraine'">
      <xsl:call-template name="LorraineT"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Rennes'">
      <xsl:call-template name="RennesT"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='RhoneAlpes'">
      <xsl:call-template name="RhoneAlpesT"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Rocq'">
      <xsl:call-template name="RocqT"/>
    </xsl:when>
    <xsl:when test="/std/@UR-val='Sophia'">
      <xsl:call-template name="SophiaT"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="FutursT"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="outURK">
 <xsl:variable name="file">
  <xsl:choose>
    <xsl:when test="/std/@UR-val='Lorraine'">Lorraine-couleur</xsl:when>
    <xsl:when test="/std/@UR-val='Rennes'">Rennes-couleur</xsl:when>
    <xsl:when test="/std/@UR-val='RhoneAlpes'">Rhone-Alpes-couleur</xsl:when>
    <xsl:when test="/std/@UR-val='Rocq'">Rocq-couleur</xsl:when>
    <xsl:when test="/std/@UR-val='Sophia'">Sophia-couleur</xsl:when>
    <xsl:otherwise>Logo-INRIA-Futurs-couleur</xsl:otherwise>
  </xsl:choose>
 </xsl:variable>
  <vspace break-before="page" space-before="-24mm" />
  <fo:block space-after='2cm'>
     <fo:external-graphic content-width="5.7cm" src="Logo-INRIA-{$file}"/>
  </fo:block>
</xsl:template>

<xsl:template name="RR.editor">
  <fo:block font-size="8pt" space-before="10pt">
    Éditeur
  </fo:block>
  <fo:block font-size="8pt">
    INRIA - Domaine de Voluceau - Rocquencourt, BP 105 -
     78153 Le Chesnay Cedex (France)
  </fo:block> 
  <fo:block font-family='sansserif' font-size="10pt">
     http://www.inria.fr
  </fo:block>
  <fo:block font-size="10pt">
    <xsl:value-of select="$issn.value" />
  </fo:block>
</xsl:template>


<xsl:template name="lastpage.body">
  <vspace space-before="11cm" />
  <fo:block id='lastpage'> 
     <fo:external-graphic content-height="1.4cm" src="Logo-INRIA-picto"/>
  </fo:block>
  <hrule space-before="1cm" space-after="1cm"/>
  <xsl:call-template name="outURX"/>
  <hrule space-before="5mm" space-after="5mm"/>
  <xsl:call-template name="RR.editor"/>
</xsl:template>


<!--Values for the inner page  -->


<xsl:template name="FuturT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Futurs
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    Parc Club Orsay Université, ZAC des Vignes, 
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    4, rue Jacques Monod,
    91893 ORSAY Cedex (France) 
  </fo:block>
  <fo:block text-align="center" font-size="8pt">
    Téléphone : +33 1 72 92 59 00 &#x2014;
    Télécopie :  +33 1 60 19 66 08
  </fo:block>
</xsl:template>


<xsl:template name="LorraineT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Lorraine
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    LORIA, Technopôle de Nancy-Brabois, Campus scientifique,
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    615, rue du Jardin Botanique, BP 101,
    54602 Villers-Lès-Nancy (France)
  </fo:block>
  <fo:block text-align="center" font-size="8pt">
    Téléphone :  +33 3  83 59 30 00 &#x2014;
    Télécopie : +33 3  83 27 83 19
  </fo:block>
</xsl:template>

<xsl:template name="RennesT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Rennes
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    IRISA, Campus universitaire de Beaulieu, 
    35042 Rennes Cedex (France)
  </fo:block>  
  <fo:block text-align="center" font-size="8pt">
    Téléphone :  +33 2 99 84 71 00 &#x2014;
    Télécopie : +33 2 99 84 71 71
  </fo:block>
</xsl:template>


<xsl:template name="RhoneAlpesT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Rhône-Alpes
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    655, avenue de l'Europe, 38334 Montbonnot Saint Ismier (France)
  </fo:block>
  <fo:block text-align="center" font-size="8pt">
    Téléphone : +33 4 76 61 52 00 &#x2014;
    Télécopie : +33 4 76 61 52 52
  </fo:block>
</xsl:template>


<xsl:template name="RocqT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Rocquencourt
  </fo:block>
  <fo:block text-align="center" font-size="10pt">
    Domaine de Voluceau, Rocquencourt, BP 105, 
    78153 Le Chesnay Cedex (France)
  </fo:block>
  <fo:block text-align="center" font-size="8pt">
    Téléphone : +33 1 39 63 55 11 &#x2014;
    Télécopie : +33 1 39 63 53 30
  </fo:block>
</xsl:template>

<xsl:template name="SophiaT">
  <fo:block text-align="center" font-size="10pt">
    Unité de recherche INRIA Sophia Antipolis
  </fo:block> 
  <fo:block text-align="center" font-size="10pt">
    2004, route des Lucioles, BP 93, 
    06902 Sophia Antipolis Cedex (France)
  </fo:block>
  <fo:block text-align="center" font-size="8pt">
    Téléphone : +33 4 92 38 77 77 &#x2014;
    Télécopie : +33 4 92 38 77 65
  </fo:block>
</xsl:template>


<!-- page sequence for the titlepage, redefined -->
<xsl:template name="titlepage">
 <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="blank1"
        force-page-count="end-on-even" >
     <xsl:text>&#xA;</xsl:text>
     <fo:flow font-family="{$bodyFont}" font-size="10pt" text-align='center'>
      <xsl:call-template name="titlepage.body"/>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>

<xsl:template match="title|ftitle" mode="inner">
  <fo:block font-weight='bold' font-size="18pt">
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="title|ftitle" mode="outer">
  <fo:block font-weight='bold' font-style ="italic"
      font-family="Times Roman" font-size="20pt">
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="author" mode="inner">
 <fo:block
     font-size="14pt" space-before='20pt' space-after='10pt'>
   <xsl:apply-templates/>
 </fo:block>
</xsl:template>

<xsl:template match="author" mode="outer">
 <fo:block
     font-size="15pt" font-family="Times Roman" space-before="1cm" 
     space-after='0pt plus 1fill'>
   <xsl:apply-templates/>
 </fo:block>
</xsl:template>

<xsl:template match="resume" mode="inner">
  <fo:block font-weight='bold'> Résumé </fo:block>
  <fo:block text-align='justify' >
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="abstract" mode="inner">
  <fo:block font-weight='bold'>Abstract</fo:block>
  <fo:block text-align='justify' >
    <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="motcle" mode="inner">
  <fo:block text-align='start' space-before='2mm'>
     <fo:inline font-weight='bold'>Mots clés : </fo:inline>
     <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="keyword" mode="inner">
  <fo:block text-align='start' space-before='2mm'>
     <fo:inline font-weight='bold'>Keywords: </fo:inline>
     <xsl:apply-templates/>
  </fo:block>
</xsl:template>


<xsl:template match="RRnote" mode="inner">
  <fo:block text-align='start'>
     <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="RRemail" mode="inner">
  <fo:block text-align='start'>
     <xsl:text>Email: </xsl:text>
     <fo:inline font-family="monospace"><xsl:apply-templates/></fo:inline>
  </fo:block>
</xsl:template>

<xsl:template match="inria-team" mode="inner">
  <fo:block space-after='10pt'>
     <xsl:text>Projet </xsl:text>
     <xsl:apply-templates/>
  </fo:block>
</xsl:template>

<xsl:template match="theme" mode="inner">
  <fo:block space-before='2mm'>
  <xsl:choose>
    <xsl:when test="/std/@theme-val='THnum'">
      Thème NUM &#x2014; Systèmes numériques
    </xsl:when>
    <xsl:when test="/std/@theme-val='THcom'">
      Thème COM &#x2014; Systèmes communicants
    </xsl:when>
    <xsl:when test="/std/@theme-val='THcog'">
      Thème COG &#x2014; Systèmes cognitifs
    </xsl:when>
    <xsl:when test="/std/@theme-val='THsym'">
      Thème SYM &#x2014; Systèmes symboliques
    </xsl:when>
    <xsl:when test="/std/@theme-val='THbio'">
      Thème BIO &#x2014; Systèmes  biologiques
    </xsl:when>
    <xsl:otherwise>Thème inconnu <xsl:apply-templates/></xsl:otherwise>
  </xsl:choose>
  </fo:block>
</xsl:template>

<xsl:template match="theme" mode="outer">
  <fo:inline font-family="Helvetica" font-size='10pt'>
    <xsl:text>Thème </xsl:text>
    <xsl:choose>
      <xsl:when test="/std/@theme-val='THnum'">NUM</xsl:when>
      <xsl:when test="/std/@theme-val='THcom'">COM</xsl:when>
      <xsl:when test="/std/@theme-val='THcog'">COG</xsl:when>
      <xsl:when test="/std/@theme-val='THsym'">SYM</xsl:when>
      <xsl:when test="/std/@theme-val='THbio'">BIO</xsl:when>
      <xsl:otherwise><xsl:apply-templates/></xsl:otherwise>
    </xsl:choose>
  </fo:inline>
</xsl:template>


<xsl:template match="rrnumber" mode='normal'>
 <xsl:apply-templates/>
</xsl:template>

<xsl:template match="version-number" mode='normal'>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="date" mode='normal'>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="rev-date" mode='normal'>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template name="date.french">
  <xsl:choose>
    <xsl:when test="/std/version-number">
     <xsl:text>version </xsl:text>
     <xsl:apply-templates select="/std/version-number" mode='normal'/>
     <xsl:text> &#x2014; version initiale </xsl:text>
     <xsl:apply-templates select="/std/date" mode='normal'/>
     <xsl:text> &#x2014; version révisée </xsl:text>
     <xsl:apply-templates select="/std/rev-date" mode='normal'/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="/std/date" mode='normal'/>
    </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="date.english">
  <xsl:choose>
    <xsl:when test="/std/version-number">
     <xsl:text>version </xsl:text>
     <xsl:apply-templates select="/std/version-number" mode='normal'/>
     <xsl:text> &#x2014; initial version </xsl:text>
     <xsl:apply-templates select="/std/date" mode='normal'/>
     <xsl:text> &#x2014; revised version </xsl:text>
     <xsl:apply-templates select="/std/rev-date" mode='normal'/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="/std/date" mode='normal'/>
    </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="date.french.outer">
  <xsl:choose>
    <xsl:when test="/std/version-number">
     <xsl:text>version initiale </xsl:text>
     <xsl:apply-templates select="/std/date" mode='normal'/>
     <xsl:text> &#x2014; version révisée </xsl:text>
     <xsl:apply-templates select="/std/rev-date" mode='normal'/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="/std/date" mode='normal'/>
    </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="date.english.outer">
  <xsl:choose>
    <xsl:when test="/std/version-number">
     <xsl:text>initial version </xsl:text>
     <xsl:apply-templates select="/std/date" mode='normal'/>
     <xsl:text> &#x2014; revised version </xsl:text>
     <xsl:apply-templates select="/std/rev-date" mode='normal'/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:apply-templates select="/std/date" mode='normal'/>
    </xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template name="date.more.french">
 <fo:block space-after='3cm'>
  <xsl:choose>
    <xsl:when test="$isRR='true'">Rapport de recherche</xsl:when>
    <xsl:otherwise>Rapport technique</xsl:otherwise>
  </xsl:choose>
  <xsl:text> n° </xsl:text>
  <xsl:value-of select="$rrnumber"/>
  <xsl:text> &#x2014; </xsl:text>
  <xsl:call-template name='date.french'/>
  <xsl:text> &#x2014; </xsl:text>
  <fo:page-number-citation ref-id="lastpage"/>
  <xsl:text> pages</xsl:text>
 </fo:block>
</xsl:template>



<xsl:template name="date.more.english">
 <fo:block space-after='3cm'>
  <xsl:choose>
    <xsl:when test="$isRR='true'">Rapport de recherche</xsl:when>
    <xsl:otherwise>Rapport technique</xsl:otherwise>
  </xsl:choose>
  <xsl:text> n° </xsl:text>
  <xsl:value-of select="$rrnumber"/>
  <xsl:text> &#x2014; </xsl:text>
  <xsl:call-template name='date.english'/>
  <xsl:text> &#x2014; </xsl:text>
  <fo:page-number-citation ref-id="lastpage"/>
  <xsl:text> pages</xsl:text>
 </fo:block>
</xsl:template>



<xsl:template name="date.more.french.outer">
  <fo:block font-size='15pt' font-weight='bold' font-family='Times Roman'>
    <xsl:text>N° </xsl:text>
    <xsl:value-of select="$rrnumber"/>
  </fo:block>
  <fo:block font-size='12pt' font-family='Times Roman' space-before='5mm' space-after='9cm'>
    <xsl:call-template name='date.french.outer'/>
  </fo:block>
</xsl:template>

<xsl:template name="date.more.english.outer">
  <fo:block font-size='15pt' font-weight='bold' font-family='Times Roman'>
    <xsl:text>N° </xsl:text>
    <xsl:value-of select="$rrnumber"/>
  </fo:block>
  <fo:block font-size='12pt' font-family='Times Roman' space-before='5mm' space-after='9cm'>
    <xsl:call-template name='date.english.outer'/>
  </fo:block>
</xsl:template>

<xsl:template name="issn">
 <RRissn>
 <fo:inline font-size='8pt'  font-family='Helvetica'>
  <xsl:value-of select="$issn.value" />
  <xsl:text>&#xa0;&#xa0;ISRN INRIA/</xsl:text>
  <xsl:choose>
    <xsl:when test="$isRR='true'">RR</xsl:when>
    <xsl:otherwise>RT</xsl:otherwise>
  </xsl:choose>
  <xsl:text>--</xsl:text>
  <xsl:value-of select="$rrnumber"/>
  <xsl:text>--FR+ENG</xsl:text>
 </fo:inline>
 </RRissn>
</xsl:template>





<xsl:template name="page2.english">
  <xsl:call-template name="outURK"/>
  <xsl:apply-templates select="/std/title" mode="inner"/>
  <xsl:apply-templates select="/std/author" mode="inner"/>
  <xsl:apply-templates select="/std/theme" mode="inner"/>
  <xsl:apply-templates select="/std/inria-team" mode="inner"/>
  <xsl:call-template name='date.more.english'/>
  <xsl:apply-templates select="/std/abstract" mode="inner"/>
  <xsl:apply-templates select="/std/keyword" mode="inner"/>
  <fo:block space-before="3pt plus 1fill"/>
  <xsl:apply-templates select="/std/RRnote" mode="inner"/>
  <xsl:apply-templates select="/std/RRemail" mode="inner"/>
  <fo:block space-before="20pt"/>
  <xsl:call-template name="outURT"/>
  <fo:block space-before="-18mm"/>
</xsl:template>


<xsl:template name="page2.french">
  <xsl:call-template name="outURK"/>
  <xsl:apply-templates select="/std/ftitle" mode="inner"/>
  <xsl:apply-templates select="/std/author" mode="inner"/>
  <xsl:apply-templates select="/std/theme" mode="inner"/>
  <xsl:apply-templates select="/std/inria-team" mode="inner"/>
  <xsl:call-template name='date.more.french'/>
  <xsl:apply-templates select="/std/resume" mode="inner"/>
  <xsl:apply-templates select="/std/motcle" mode="inner"/>
  <fo:block space-before="3pt plus 1fill"/>
  <xsl:apply-templates select="/std/RRnote" mode="inner"/>
  <xsl:apply-templates select="/std/RRemail" mode="inner"/>
  <fo:block space-before="20pt"/>
  <xsl:call-template name="outURT"/>
  <fo:block space-before="-18mm"/>
</xsl:template>

<xsl:template name="page3.french">
 <fo:block break-before="page"/>
 <xsl:apply-templates select="/std/title" mode="inner"/>
 <fo:block space-before='30pt'/>
 <xsl:apply-templates select="/std/abstract" mode="inner"/>
 <xsl:apply-templates select="/std/keyword" mode="inner"/>
</xsl:template>

<xsl:template name="page3.english">
  <fo:block break-before="page"/>
  <xsl:apply-templates select="/std/ftitle" mode="inner"/>
 <fo:block space-before='30pt'/>
  <xsl:apply-templates select="/std/resume" mode="inner"/>
 <xsl:apply-templates select="/std/motcle" mode="inner"/>
</xsl:template>

<xsl:template name="titlepage.body">
  <fo:block></fo:block>
  <fo:RRINRIA>
    <xsl:attribute name="logo">
      <xsl:choose>
	<xsl:when test="$isRR='true'">rap-rech1</xsl:when>
	<xsl:otherwise>rap-tech1</xsl:otherwise>
      </xsl:choose>
    </xsl:attribute>
    <xsl:apply-templates select="/std/theme" mode="outer"/>
  </fo:RRINRIA>
  <vspace space-before="-19mm"/>
  <fo:block space-after='2cm'> 
     <fo:external-graphic content-width="5.7cm" src="Logo-INRIA-couleur"/>
  </fo:block>
  <fo:block space-after='2cm' font-size="10pt" font-family="Times Roman"> 
    INSTITUT NATIONAL DE RECHERCHE EN INFORMATIQUE ET EN AUTOMATIQUE
  </fo:block>
  <xsl:choose>
    <xsl:when test="$isfrench='true'">
      <xsl:apply-templates select="/std/ftitle" mode="outer"/>
    </xsl:when>
   <xsl:otherwise>   
      <xsl:apply-templates select="/std/title" mode="outer"/>
   </xsl:otherwise>
  </xsl:choose>
  <xsl:apply-templates select="/std/author" mode="outer"/>
  <xsl:choose>
    <xsl:when test="$isfrench='true'">
      <xsl:call-template name ='date.more.french.outer' />
    </xsl:when>
   <xsl:otherwise>   
      <xsl:call-template name ='date.more.english.outer' />
   </xsl:otherwise>
  </xsl:choose>
  <xsl:call-template name="issn" />
  <cleardoublepage/>

  <xsl:choose>
    <xsl:when test="$isfrench='true'">
      <xsl:call-template name="page2.french" />
      <xsl:call-template name="page3.french" />
    </xsl:when>
    <xsl:otherwise>  
      <xsl:call-template name="page2.english" />
      <xsl:call-template name="page3.english" />
    </xsl:otherwise>
  </xsl:choose>  
</xsl:template>

</xsl:stylesheet>




