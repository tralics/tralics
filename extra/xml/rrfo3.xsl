<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format RR XML documents 

$Id: rrfo3.xsl,v 2.2 2004/09/15 08:25:02 grimm Exp $
-->


<!-- Copyright Inria 2003-2004 Jose Grimm. This file is an adaptation of
file from the TEI distribution. See original Copyright notice below.
On 2004/09/14, rrfo.xsl copied to rrfo3.xsl
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

<xsl:include href="raweb3-param.xsl"/>
<xsl:include href="rrrafo3.xsl"/>

<xsl:strip-space elements="cell"/>
<xsl:output indent="no"/>


<xsl:variable name="top" select="/"/>
<xsl:variable name="tableSpecs">
  <xsl:choose>
  <xsl:when test="$readColSpecFile">
  <xsl:copy-of
     select="document($readColSpecFile,$top)/Info"/>
 </xsl:when>
 <xsl:otherwise> <Info></Info></xsl:otherwise>
</xsl:choose>
</xsl:variable>


<!-- enable, for new elements to get commented as such-->
<xsl:template match="*">
 <xsl:comment><xsl:text>PASS THROUGH </xsl:text>
   <xsl:value-of select="name()"/>
 </xsl:comment>
 <xsl:apply-templates/>
</xsl:template>


<!-- le rapport de recherche -->
<xsl:template match="rr">
 <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
   <xsl:call-template name="setupPagemasters"/>
   <xsl:call-template name="myheaders"/>
   <xsl:apply-templates select="RRstart"/> 
   <xsl:call-template name="myTOC"/>
   <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="french"
        initial-page-number="auto-odd"
        master-reference="twoside1"
        master-name="twoside1nofirst"
      >
      <fo:flow  
         flow-name="xsl-region-body"
             font-family="{$bodyFont}"  
         font-size="{$bodySize}">
        <xsl:call-template name="rr.body"/>
      </fo:flow>
   </fo:page-sequence>
  <xsl:call-template name="pagetitre4"/>
 </fo:root>
</xsl:template>

<xsl:template name="rr.body">
  <xsl:for-each select = "div0">
    <xsl:apply-templates select="."/>
  </xsl:for-each>
  <xsl:apply-templates select='biblio'/>
</xsl:template>

<xsl:variable name="mainlanguage">
  <xsl:choose>
    <xsl:when test ="/rr/@language='french'">FR</xsl:when>
    <xsl:otherwise>EN</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="rrno">
  <xsl:value-of select="/rr/RRstart/RRnumber"/>
</xsl:variable>

<xsl:variable name="rrtype">
  <xsl:choose>
    <xsl:when test ="/rr/@type='RR'">RR</xsl:when>
    <xsl:otherwise>RT</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:template name="main.title">
  <xsl:choose>
    <xsl:when test ="$mainlanguage='FR'">
       <xsl:apply-templates select="/rr/RRstart/title"/>
    </xsl:when>
    <xsl:otherwise>
       <xsl:apply-templates select="/rr/RRstart/etitle"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="other.title">
  <xsl:choose>
    <xsl:when test ="$mainlanguage='FR'">
       <xsl:apply-templates select="/rr/RRstart/etitle"/>
    </xsl:when>
    <xsl:otherwise>
       <xsl:apply-templates select="/rr/RRstart/title"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="get.abstract">
  <xsl:param name="what"/>
  <xsl:choose>
    <xsl:when test="($what='2' and $mainlanguage='FR') 
	      or ($what='3' and $mainlanguage='EN')">
       <xsl:apply-templates select="/rr/RRstart/resume"/>
    </xsl:when>
    <xsl:otherwise>
       <xsl:apply-templates select="/rr/RRstart/abstract"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="get.keywords">
  <xsl:param name="what"/>
  <xsl:choose>
    <xsl:when test="($what='2' and $mainlanguage='FR') 
	      or ($what='3' and $mainlanguage='EN')">
       <xsl:apply-templates select="/rr/RRstart/motcle"/>
    </xsl:when>
    <xsl:otherwise>
       <xsl:apply-templates select="/rr/RRstart/keyword"/>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="UR.tel">
  <xsl:param name="UR_name"/>
  <xsl:choose>
     <xsl:when test="$UR_name='Lorraine'">3 83 59 30 00</xsl:when>
     <xsl:when test="$UR_name='Rennes'">2 99 84 71 00</xsl:when>
     <xsl:when test="$UR_name='RhoneAlpes'">4 76 61 52 00</xsl:when>
     <xsl:when test="$UR_name='Rocquencourt'">1 39 63 55 11</xsl:when>
     <xsl:when test="$UR_name='Sophia'">4 92 38 77 77</xsl:when>
     <xsl:when test="$UR_name='Futurs'">1 39 63 55 11</xsl:when>
     <xsl:otherwise>1 23 45 67 89</xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="UR.fax">
  <xsl:param name="UR_name"/>
  <xsl:choose>
     <xsl:when test="$UR_name='Lorraine'">3 83 27 83 19</xsl:when>
     <xsl:when test="$UR_name='Rennes'">2 99 84 71 71</xsl:when>
     <xsl:when test="$UR_name='RhoneAlpes'">4 76 61 52 52</xsl:when>
     <xsl:when test="$UR_name='Rocquencourt'">1 39 63 53 30</xsl:when>
     <xsl:when test="$UR_name='Sophia'">4 92 38 77 65</xsl:when>
     <xsl:when test="$UR_name='Futurs'">1 39 63 53 30</xsl:when>
     <xsl:otherwise>1 23 45 67 89</xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="UR.telephone">
  <xsl:param name="UR_name"/>
  <fo:block font-size='8pt'>
     <xsl:text>Téléphone : + 33 </xsl:text>
     <xsl:call-template name="UR.tel">
       <xsl:with-param name="UR_name" select="$UR_name"/>
     </xsl:call-template>
     <xsl:text>---Télécopie : + 33 </xsl:text>
     <xsl:call-template name="UR.fax">
       <xsl:with-param name="UR_name" select="$UR_name"/>
     </xsl:call-template>
  </fo:block>
</xsl:template>

<xsl:template name="which.UR">
  <xsl:choose>
     <xsl:when test="/rr/RRstart/UR/URLorraine">Lorraine</xsl:when>
     <xsl:when test="/rr/RRstart/UR/URRennes">Rennes</xsl:when>
     <xsl:when test="/rr/RRstart/UR/URRhoneAlpes">RhoneAlpes</xsl:when>
     <xsl:when test="/rr/RRstart/UR/URRocquencourt">Rocquencourt</xsl:when>
     <xsl:when test="/rr/RRstart/UR/URSophia">Sophia</xsl:when>
     <xsl:when test="/rr/RRstart/UR/URFuturs">Futurs</xsl:when>
     <xsl:otherwise>Inconnu</xsl:otherwise>
  </xsl:choose>
</xsl:template>


<xsl:template name="UR.real_name">
  <xsl:param name="UR_name"/>
  <xsl:text>Unité de recherche INRIA </xsl:text>
  <xsl:choose>
     <xsl:when test="$UR_name='RhoneAlpes'">Rhône-Alpes</xsl:when>
     <xsl:when test="$UR_name='Sophia'">Sophia Antipolis</xsl:when>
     <xsl:otherwise><xsl:value-of select='$UR_name'/></xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="output.UR.titre2">
  <xsl:variable name="cur_UR">
       <xsl:call-template name="which.UR"/>
  </xsl:variable>
  <xsl:call-template name="output.some.UR">
    <xsl:with-param name="UR_name" select="$cur_UR"/> 
    <xsl:with-param name="mode" select="'normal'"/>
   </xsl:call-template> 
</xsl:template>

<xsl:template name="output.UR">
  <xsl:param name="this_UR"/>
  <xsl:param name="condition"/>
  <xsl:param name="cur_UR"/>
  <xsl:variable name="same_UR">
    <xsl:choose>
      <xsl:when test="$cur_UR=$this_UR">true</xsl:when>
      <xsl:otherwise>false</xsl:otherwise>
    </xsl:choose>
  </xsl:variable>  
  <xsl:if test="($condition='if_different' and $same_UR='false') or
       ($condition != 'if_different' and $same_UR='true')">
    <xsl:call-template name="output.some.UR">
       <xsl:with-param name="UR_name" select="$this_UR"/>
       <xsl:with-param name="mode" select="$condition"/>
    </xsl:call-template>
  </xsl:if>
</xsl:template>

<xsl:template name="output.some.UR">
  <xsl:param name="UR_name"/>
  <xsl:param name="mode"/>
  <xsl:choose>
    <xsl:when test="$mode != 'if_different'">
      <fo:block text-align='center' font-family="Times" font-size= "10pt"
	    font-style="normal">
         <xsl:call-template name="UR.adress">
           <xsl:with-param name="UR_name" select="$UR_name"/>
           <xsl:with-param name="mode" select="$mode"/>
         </xsl:call-template>
         <xsl:if test="$mode='normal'">
            <xsl:call-template name="UR.telephone">
              <xsl:with-param name="UR_name" select="$UR_name"/>
            </xsl:call-template>
         </xsl:if>
      </fo:block>
    </xsl:when>
    <xsl:otherwise>
         <xsl:call-template name="UR.adress">
           <xsl:with-param name="UR_name" select="$UR_name"/>
           <xsl:with-param name="mode" select="$mode"/>
         </xsl:call-template>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template name="output.all_UR">
  <xsl:param name="condition"/>
  <xsl:param name="cur_UR"/>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'Futurs'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'Lorraine'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'Rennes'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'RhoneAlpes'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'Rocquencourt'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
  <xsl:call-template name="output.UR">
    <xsl:with-param name="this_UR" select="'Sophia'"/>
    <xsl:with-param name="condition" select="$condition"/>
    <xsl:with-param name="cur_UR" select="$cur_UR"/>
  </xsl:call-template>
</xsl:template>

<xsl:template name="UR.adress">
  <xsl:param name="UR_name"/>
  <xsl:param name="mode"/>
  <xsl:variable name="realname">
     <xsl:call-template name="UR.real_name">
       <xsl:with-param name="UR_name" select="$UR_name"/>
     </xsl:call-template>
  </xsl:variable>
  <xsl:choose>
    <xsl:when test="$UR_name='Lorraine'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
           <fo:block>LORIA, Technopôle de Nancy-Brabois, Campus scientifique,
             </fo:block>
           <fo:block>615, rue du Jardin Botanique, BP 101,
                  54602 Villers-Lès-Nancy (France)</fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>
               Technopôle de Nancy-Brabois - Campus scientifique
            </fo:block>
        <fo:block>615, rue du Jardin Botanique - BP 101 -
         54602 Villers-Lès-Nancy (France)</fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$UR_name='RhoneAlpes'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
  <fo:block>655, avenue de l'Europe, 38330 Montbonnot-St-Martin (France)
       </fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>
    655, avenue de l'Europe, 38330 Montbonnot-St-Martin (France)
       </fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$UR_name='Rennes'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
           <fo:block>IRISA, Campus universitaire de Beaulieu,
            35042 Rennes Cedex (France)</fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>
             IRISA, Campus universitaire de Beaulieu - 35042 Rennes Cedex (France)</fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$UR_name='Sophia'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
          <fo:block>2004, route des Lucioles, BP 93,
            06902 Sophia Antipolis Cedex (France)
       </fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>2004, route des Lucioles - BP 93 -
            06902 Sophia Antipolis Cedex (France)
       </fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$UR_name='Rocquencourt'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
 <fo:block>Domaine de Voluceau, Rocquencourt, BP 105,
         78153 Le Chesnay Cedex (France)
       </fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>
    Domaine de Voluceau, Rocquencourt - BP 105 -
         78153 Le Chesnay Cedex (France)
       </fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
    <xsl:when test="$UR_name='Futurs'">
      <xsl:choose>
         <xsl:when test="$mode='normal'">
           <fo:block><xsl:value-of select="$realname"/></fo:block>
      <fo:block>Domaine de Voluceau, Rocquencourt, BP 105,
         78153 Le Chesnay Cedex (France)
       </fo:block>
         </xsl:when>
         <xsl:otherwise>
            <fo:block><xsl:value-of select="$realname"/>
               <xsl:text> : </xsl:text>
 Domaine de Voluceau, Rocquencourt - BP 105 -
         78153 Le Chesnay Cedex (France)
       </fo:block>
         </xsl:otherwise>
      </xsl:choose>
    </xsl:when>
  </xsl:choose>
</xsl:template>

<xsl:template match="RRstart">
 <fo:page-sequence
       margin-left="5mm"
        format="1" 
        text-align="center"
        hyphenate="true"
        language="FR"
        initial-page-number="1"
        master-reference="twoside1"
        master-name="twoside1"
      >
   <fo:flow font-style="italic" font-family="Times Roman"> 
    <fo:RRINRIA type="{$rrtype}"/>
    <fo:block font-size= "20pt"> 
            <xsl:call-template name="main.title"/>
    </fo:block>
    <fo:block font-style="normal" font-size= "12pt" space-before="1cm">
            <xsl:value-of select="author"/>
    </fo:block>
    <fo:block font-size= "15pt" font-weight="bold" space-before="1cm">
           Nº <xsl:value-of select="$rrno"/>
    </fo:block>
    <fo:block font-size= "12pt" font-weight="normal" space-before="1cm">
           <xsl:value-of select="date"/>
    </fo:block>
    <fo:block font-size= "10pt" font-style="normal" font-family="Helvetica"  space-before="1cm">
           <fo:RRTHEME><xsl:value-of select="theme"/></fo:RRTHEME>
    </fo:block>
   </fo:flow>
  <fo:flow flow-name="xsl-region-body"> </fo:flow>
  <xsl:call-template name="pagetitre2"/>
  <xsl:call-template name="pagetitre3"/>
  </fo:page-sequence>
</xsl:template>

<xsl:template name="pagetitre2">
 <fo:flow flow-name="xsl-region-body">
   <fo:block text-align = "justify">
      <fo:block font-weight='bold' font-size="14pt" space-before="5pt" 
             keep-with-next='always' text-align="center">
         <xsl:call-template name="main.title"/>
      </fo:block>
      <fo:block font-size="12pt" space-before="20pt" text-align="center">
          <xsl:apply-templates select = "author"/>
      </fo:block>
      <xsl:apply-templates select = "theme" mode = "titre2"/>

      <fo:block space-before="10pt" text-align="center">
         <xsl:choose>
          <xsl:when test="$rrtype='RR'">Rapport de recherche</xsl:when>
          <xsl:otherwise>Rapport technique</xsl:otherwise>
         </xsl:choose>
         nº <xsl:value-of select="$rrno"/> --- <xsl:value-of select="date"/> --- 
            <fo:page-number-citation ref-id="RRlastpageofreport"/> pages
      </fo:block>
    <xsl:call-template name="get.abstract">
      <xsl:with-param name="what" select='2'/>
    </xsl:call-template>
    <xsl:call-template name="get.keywords">
      <xsl:with-param name="what" select='2'/>
    </xsl:call-template>
   <vfill/>
   <xsl:call-template name="output.UR.titre2">
   </xsl:call-template>
   </fo:block>
 </fo:flow>
</xsl:template>

<xsl:template name="pagetitre3">
 <fo:flow  flow-name="xsl-region-body" >
  <fo:block text-align="justify"> 
    <fo:block font-weight='bold' font-size="14pt" space-before="5pt" 
       keep-with-next='always' text-align="center">
         <xsl:call-template name="other.title"/>
    </fo:block>  
    <xsl:call-template name="get.abstract">
      <xsl:with-param name="what" select='3'/>
    </xsl:call-template>
    <xsl:call-template name="get.keywords">
      <xsl:with-param name="what" select='3'/>
    </xsl:call-template>
  </fo:block>
 </fo:flow>
</xsl:template>

<xsl:template name="pagetitre4">
  <xsl:variable name="cur_UR">
    <xsl:call-template name="which.UR"/>
  </xsl:variable>
 <fo:page-sequence
        format="1" 
        text-align="center"
        hyphenate="true"
        language="FR"
        initial-page-number="auto"
        master-reference="twoside1"
        master-name="twoside1nofirst"
      >
   <fo:flow flow-name="xsl-region-body" >
  <fo:static-content flow-name="xsl-region-after-right"><fo:block/></fo:static-content>
  <fo:static-content flow-name="xsl-region-after-left"><fo:block/></fo:static-content>
  <fo:static-content flow-name="xsl-region-before-right"><fo:block/></fo:static-content>
  <fo:static-content flow-name="xsl-region-before-left"><fo:block/></fo:static-content>

   <fo:block>
     <RRlastpage/>
     <fo:inline id="RRlastpageofreport">&#x000A0;
     </fo:inline>
     <vfill/> 
     <fo:external-graphic content-height="1.4cm" src="Logo-INRIA-picto"/>
   </fo:block>
   <fo:block space-before="1.5cm">&#x000A0;
   </fo:block>
   <hrule/>
   <fo:block space-before="1.5cm">&#x000A0;
   </fo:block>
   <xsl:call-template name="output.all_UR">
     <xsl:with-param name="cur_UR" select="$cur_UR"/>
     <xsl:with-param name="condition" select="'if_same'"/>
   </xsl:call-template> 
   <fo:block text-align="center" font-family='Times' font-size="8pt"
     font-style='normal'>
     <xsl:call-template name="output.all_UR">
       <xsl:with-param name="cur_UR" select="$cur_UR"/>
       <xsl:with-param name="condition" select="'if_different'"/>
     </xsl:call-template> 
   </fo:block>
   <fo:block space-before="1.5cm">
     <hrule/>
   </fo:block>
   <fo:block  space-before="0.7cm" text-align="center" font-size="8pt">
       Éditeur</fo:block>
   <fo:block text-align="center" font-size="8pt">
     INRIA - Domaine de Voluceau - Rocquencourt, BP 105 -
     78153 Le Chesnay Cedex (France)
   </fo:block>
     <fo:block text-align="center" font-size="10pt" space-before="5mm">
      http://www.inria.fr
     </fo:block>
     <fo:block text-align="center" font-size="10pt" space-before="5mm">
      <xsl:choose>
        <xsl:when test="/rr/@type='RR'">ISSN 0249-6399</xsl:when>
        <xsl:otherwise>ISSN 0249-0803</xsl:otherwise>
       </xsl:choose>
     </fo:block>
  </fo:flow>
 </fo:page-sequence>
</xsl:template>



<!-- Biblio.  -->
<xsl:template match="biblio">
  <fo:flow>
   <fo:block font-weight='bold' font-size="14pt" space-before="5pt" 
       keep-with-next='always'>
    Bibliographie
  </fo:block>
    <xsl:for-each select="citation">
       <xsl:apply-templates select="(.)"/>
    </xsl:for-each>
  </fo:flow>
</xsl:template>


<!-- Les divisions -->
<xsl:template match="div1|div2|div3|div4">
  <xsl:call-template name="NumberedHeading">
    <xsl:with-param name="level">
       <xsl:value-of select="substring-after(name(),'div')"/>
    </xsl:with-param>
  </xsl:call-template>
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="div0">
 <fo:flow flow-name="xsl-region-body">
  <xsl:call-template name="NumberedHeading">
    <xsl:with-param name="level">0
    </xsl:with-param>
  </xsl:call-template>
  <xsl:apply-templates/>
 </fo:flow>
</xsl:template>

<!-- En-tête pour une division plus basse que la section -->

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
    <xsl:if test="$pdfBookmarks='true'"> <fotex:toto/>
     <!-- Passive TeX extension, to get PDF bookmarks -->
      <fotex:bookmark  
        xmlns:fotex="http://www.tug.org/fotex" 
        fotex-bookmark-level="{$level}" 
        fotex-bookmark-label="{$divid}">
       <xsl:if test="not($numberHeadings='')">
         <xsl:value-of select="$Number"/>
         <xsl:value-of select="$headingNumberSuffix"/>
       </xsl:if>
       <xsl:value-of select="head"/>
     </fotex:bookmark>
    </xsl:if>
  </fo:block>
</xsl:template>

<!-- Table des matières -->
<xsl:template name="myTOC">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="french"
        initial-page-number="1"
        master-reference="twoside1"
        master-name="twoside1nofirst"
      >
    <fo:block margin-right="-3cm" >
       <fo:block font-size="14pt" text-align="center" font-weight="bold" 
           space-after="20pt"><resetpageno/>
         <xsl:text>Table des matières</xsl:text>
       </fo:block>
       <xsl:for-each select="//div0|//div1|//div2|//div3">
          <xsl:apply-templates mode="xtoc" select="(.)"/>
       </xsl:for-each>
    </fo:block>
   </fo:page-sequence>
</xsl:template>

<xsl:template mode="xtoc" match="div0|div1|div2|div3|div4">
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
    <xsl:otherwise><xsl:value-of select="$div1Tocindent"/></xsl:otherwise>
   </xsl:choose>
  </xsl:variable>
  <fo:block>
      <xsl:attribute name="text-indent">
       <xsl:value-of select="$tocindent"/>
      </xsl:attribute>
  <xsl:variable name="Number">
    <xsl:if test="$numberHeadings and $numberDepth &gt; $level">
     <xsl:call-template name="calculateNumber">
       <xsl:with-param name="numbersuffix" select="$tocNumberSuffix"/>
     </xsl:call-template>
   </xsl:if>
   </xsl:variable>
   <xsl:value-of select="$Number"/>
   <xsl:text>&#x2003;</xsl:text>
   <fo:inline>
      <xsl:apply-templates mode="tocsection" select="head"/>
   </fo:inline>
    <fo:leader rule-thickness="0pt"/>
    <fo:inline color="{$linkColor}">
      <xsl:variable name="pagref">
        <xsl:call-template name="idLabel"/>
      </xsl:variable>
     <fo:basic-link internal-destination="{$pagref}">
      <fo:page-number-citation ref-id="{$pagref}"/>
     </fo:basic-link>
    </fo:inline> 
  </fo:block>
</xsl:template>

<!-- Le id disparait ici -->
<xsl:template match="head" mode="tocsection">
   <xsl:apply-templates mode="section"/>
</xsl:template>

<!-- Le id reste ici -->
<xsl:template match="head" mode="section">
  <fo:inline>    
   <xsl:if test="@id">
      <xsl:attribute name="id">
      <xsl:value-of select="translate(@id,'_','-')"/>
      </xsl:attribute>
   </xsl:if>
   <xsl:apply-templates mode="section"/>
  </fo:inline>
</xsl:template>


<!-- unqualified <head> -->
<xsl:template match="head">
 <xsl:variable name="parent" select="name(..)"/>
 <xsl:if test="not(starts-with($parent,'div'))">
  <xsl:apply-templates/>
 </xsl:if>
</xsl:template>


<xsl:template match="head" priority="10">
 <xsl:variable name="parent" select="name(..)"/>
 <xsl:if test="not(starts-with($parent,'div'))"> 
  <xsl:apply-templates/>
 </xsl:if>
</xsl:template>



<!-- références internes -->

<xsl:template match="ref">
 <fo:basic-link color="{$linkColor}">
  <xsl:attribute name="internal-destination"><xsl:value-of select="translate(@target,'_','-')"/></xsl:attribute>
    <xsl:apply-templates mode="xref" select="id(@target)" />
  <xsl:apply-templates/>
 </fo:basic-link>
</xsl:template>

<!-- headings when cross-referenced -->
<xsl:template mode="xref" match="text">
   <xsl:number/>
</xsl:template>


<xsl:template match="head" mode ="xref">
   <xsl:call-template name="calculateNumber"/>
</xsl:template>

<xsl:template match="anchor" mode ="xref">
    <xsl:call-template name="calculateNumber"/>
</xsl:template>

<xsl:template mode="xref" match="div0|div1|div2|div3|div4">
    <xsl:call-template name="calculateNumber"/>
</xsl:template>

<!-- pas pareil pour les maths, les figures les tables -->

<xsl:template match="formula" mode="xref">
  <xsl:number level = "any" count="formula[@id]"/>
</xsl:template>

<xsl:template match='figure' mode="xref">
   <xsl:call-template name="calculateFigureNumber"/>
</xsl:template>

<xsl:template name="xheading">
   <xsl:param name="level"/>
   <xsl:if test="not($numberHeadings='')">
     <xsl:call-template name="calculateNumber"/>
   </xsl:if>
   <xsl:call-template name="divXRefHeading"/>
</xsl:template>


<xsl:template match="item" mode="xref">
  <xsl:variable name="listdepth" select="count(ancestor::list)"/>
  <xsl:if test="parent::list[@type='bibliography']">
     <xsl:text> [</xsl:text>
   </xsl:if>
  <xsl:variable name="listNFormat">
          <xsl:choose>
            <xsl:when test="$listdepth=1">
              <xsl:text>1</xsl:text>
            </xsl:when>
            <xsl:when test="$listdepth=2">
              <xsl:text>i</xsl:text>
            </xsl:when>
            <xsl:when test="$listdepth=3">
              <xsl:text>a</xsl:text>
            </xsl:when>
            <xsl:when test="$listdepth=4">
              <xsl:text>I</xsl:text>
            </xsl:when>
          </xsl:choose>
</xsl:variable>
<xsl:number format="{$listNFormat}"/>
<xsl:if test="parent::list[@type='bibliography']">
  <xsl:text>]</xsl:text>
</xsl:if>
</xsl:template>



<xsl:template match="processing-instruction()[name()='xmltex']" >
   <xsl:message>xmltex pi <xsl:value-of select="."/></xsl:message>
   <xsl:copy-of select="."/>
</xsl:template>


<xsl:template match="text">
 <xsl:apply-templates/>
</xsl:template>



<xsl:template match="*" mode="heading">
 <xsl:apply-templates/>
</xsl:template>



<xsl:template match="titlePart[@type='running']"/>


<xsl:template name="choosePageMaster">
 <xsl:param name="where"/>
 <xsl:param name="force"/>
<xsl:variable name="mn">
   <xsl:choose>
     <xsl:when test="$forcePageMaster">
           <xsl:value-of select="$forcePageMaster"/>
     </xsl:when>
     <xsl:when test="not($where='')">
     <xsl:choose>
      <xsl:when test="$twoSided">twoside2</xsl:when>
      <xsl:otherwise>oneside2</xsl:otherwise>
     </xsl:choose>
     </xsl:when>
     <xsl:otherwise>
     <xsl:choose>
      <xsl:when test="$twoSided">twoside1</xsl:when>
      <xsl:otherwise>oneside1</xsl:otherwise>
     </xsl:choose>
     </xsl:otherwise>
   </xsl:choose>
</xsl:variable>
 <xsl:attribute name="master-reference">
  <xsl:value-of select="$mn"/>
 </xsl:attribute>
</xsl:template>


<xsl:template name="setupPagemasters">
  <fo:layout-master-set>

<!-- one sided, single column -->
     <fo:simple-page-master
        master-reference="simple1"
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
        master-reference="left1"
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
        master-reference="right1"
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
        master-reference="first1"
        master-name="first1"
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


<!-- for pages in one-side mode, 2 column -->
     <fo:simple-page-master
        master-reference="simple2"
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
        master-reference="left2"
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
        master-reference="right2"
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
        master-reference="first2"
        master-name="first2"
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
     <fo:page-sequence-master master-name="twoside1nofirst" master-reference="twoside1nofirst">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
		master-name="right1"
		master-reference="right1"
		odd-or-even="odd"/>
        <fo:conditional-page-master-reference
		master-name="left1"
		master-reference="left1"
		odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for double-sided, 1 column -->
     <fo:page-sequence-master master-name="twoside1" master-reference="twoside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
		master-reference="first1"
		master-name="first1"
		page-position="first"/>
        <fo:conditional-page-master-reference
		master-reference="right1"
		master-name="right1"
		odd-or-even="odd"/>
        <fo:conditional-page-master-reference
		master-name="left1"
		master-reference="left1"
		odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for single-sided, 1 column -->
     <fo:page-sequence-master  master-name="oneside1" master-reference="oneside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
		master-name="first1"
		master-reference="first1"
		page-position="first"/>
        <fo:conditional-page-master-reference 
		master-reference="simple1"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>


<!-- setup for double-sided, 2 column -->
     <fo:page-sequence-master master-name="twoside2" master-reference="twoside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
		master-name="first2"
		master-reference="first2"
		page-position="first"/>
        <fo:conditional-page-master-reference
		master-name="right2"
		master-reference="right2"
		odd-or-even="odd"/>
        <fo:conditional-page-master-reference
		master-name="left2"
		master-reference="left2"
		odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>

<!-- setup for single-sided, 2 column -->
     <fo:page-sequence-master  master-name="oneside2" master-reference="oneside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
		master-name="first2"
		master-reference="first2"
		page-position="first"/>
        <fo:conditional-page-master-reference 
		master-name="simple2"
		master-reference="simple2"
         />
      </fo:repeatable-page-master-alternatives>
     </fo:page-sequence-master>
    <xsl:call-template name="hookDefinepagemasters"/>

    </fo:layout-master-set>
</xsl:template>


<xsl:template match="p">
  <fo:block font-size="{$bodySize}">
    <xsl:if test="preceding-sibling::p">
       <xsl:if test ="not(@noindent)">
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

<xsl:template match="code">
    <fo:inline font-family="{$typewriterFont}">
        <xsl:apply-templates/>
    </fo:inline>
</xsl:template>

<xsl:template match="hi">
<fo:inline>
 <xsl:call-template name="rend">
   <xsl:with-param name="defaultvalue" select="string('bold')"/>
   <xsl:with-param name="defaultstyle" select="string('font-weight')"/>
 </xsl:call-template>
      <xsl:apply-templates/>
</fo:inline>
</xsl:template>

<xsl:template match="hi" mode ="section">
<fo:inline>
 <xsl:call-template name="rend">
   <xsl:with-param name="defaultvalue" select="string('bold')"/>
   <xsl:with-param name="defaultstyle" select="string('font-weight')"/>
 </xsl:call-template>
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

<xsl:template match="note" mode ="section">
 TOTO
</xsl:template>
<xsl:template match="note" mode ="tocsection">
 Ttiti
</xsl:template>


<xsl:template match="list">
 <xsl:if test="child::head">
  <fo:block font-style="italic" 
	text-align="start"
	space-before.optimum="4pt">
    <xsl:apply-templates select="head"/>
  </fo:block>
 </xsl:if>
 <fo:list-block   margin-right="{$listRightMargin}">
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
         <xsl:value-of select="$listLeftIndent"/></xsl:attribute>
   </xsl:otherwise> 
  </xsl:choose>
  <xsl:apply-templates select="item"/>
  </fo:list-block>
</xsl:template>

<xsl:template match="item">
     <xsl:call-template name="makeItem"/>
</xsl:template>

<xsl:template name="makeItem">
<!-- item behaviour depends on the type attribute of our parent:
simple, bullets, ordered, gloss, unordered
-->
<xsl:variable name="listdepth" select="count(ancestor::list)"/>
<fo:list-item space-before.optimum="{$listItemsep}">
  <fo:list-item-label>
<xsl:if test="@id">
<xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
</xsl:if>
    <fo:block>
      <xsl:attribute name="margin-right">2.5pt</xsl:attribute>
      <xsl:choose>
        <xsl:when test="@n">
          <xsl:attribute name="text-align">end</xsl:attribute>
          <xsl:value-of select="@n"/>
        </xsl:when>
        <xsl:when test="../@type='bibliography'">
          <xsl:attribute name="text-align">end</xsl:attribute>
          <xsl:apply-templates mode="xref" select="."/>
        </xsl:when>
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

<!--
<xsl:template match="item/label">
 <fo:inline font-weight='bold'><xsl:apply-templates/></fo:inline>
</xsl:template>
-->


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

<!-- Cree le external graphics pour une figure -->

<xsl:template name="generate-graphics">
  <xsl:variable name="File">
    <xsl:value-of select="@file"/>
  </xsl:variable>
  <fo:external-graphic src="{$graphicsPrefix}{$File}">
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
      <xsl:when test="$autoScaleFigures">
        <xsl:attribute name="content-width">
           <xsl:value-of select="$autoScaleFigures"/></xsl:attribute>
      </xsl:when>
    </xsl:choose>
  </fo:external-graphic>
</xsl:template>

<!-- figure inline -->

<xsl:template match="figure[@rend='inline']">
  <xsl:call-template name='generate-graphics'/>
      <xsl:if test="head">
        <fo:block text-align="center">
           <xsl:apply-templates select="head"/>
        </fo:block>
      </xsl:if>
</xsl:template>

<!-- figure, avec sous-machin -->

<xsl:template match="figure[@rend='array']" >
  <fo:float>
    <xsl:call-template name="addID"/>
    <fo:block text-align="center">
      <xsl:apply-templates select='p'/>
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

<!-- figure,  no-float array -->
<xsl:template match="figure[@rend='nf-array']" >
    <fo:block text-align="center">
      <xsl:apply-templates select='p'/>
    </fo:block>
    <fo:block>
      <xsl:call-template name="figureCaptionstyle"/>
      <xsl:value-of select="$figureWord"/>
      <xsl:call-template name="calculateFigureNumber"/>
      <xsl:text>. </xsl:text>
      <xsl:apply-templates select="head"/>
    </fo:block>
</xsl:template>

<!-- figure autre -->

<xsl:template match='figure'>
 <fo:float>
   <xsl:call-template name="addID"/>
   <fo:block text-align="center">
      <xsl:call-template name='generate-graphics'/>
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


<xsl:template match="xref">
   <fo:basic-link color="{$linkColor}">
   <xsl:attribute name="external-destination">
     <xsl:value-of select="@url"/>
   </xsl:attribute>
      <xsl:apply-templates/>
   </fo:basic-link>
</xsl:template>


<xsl:template match="m:math">
 <m:math>
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates mode="math"/>
 </m:math>
</xsl:template>

<xsl:template match="m:math" mode="section">
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

<xsl:template match="picture|pic-put|pic-bezier|pic-line|pic-circle|
   pic-multiput| pic-thicklines|pic-thinlines|pic-framebox|pic-vector|
   pic-arc|pic-bigcircle|pic-scaleput|pic-linethickness |pic-closecurve|
   pic-curve|pic-tagcurve|pic-frame|pic-dashbox|pic-oval">
 <xsl:element name="{name()}">
  <xsl:copy-of select="@*"/>
  <xsl:apply-templates/>
 </xsl:element>
</xsl:template>


<xsl:template match="formula[@type='display']">
 <xsl:choose>
    <xsl:when test="@id">
     <fo:inline>
      <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
     <fotex:equation>
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


<xsl:variable name="processor">
   <xsl:value-of select="system-property('xsl:vendor')"/>
</xsl:variable>

<xsl:variable name="PRID">
 <xsl:choose>
  <xsl:when test="rr/@language='french'">
    <xsl:value-of select="rr/RRstart/title"/>
  </xsl:when>
  <xsl:otherwise>
    <xsl:value-of select="rr/RRstart/etitle"/>
  </xsl:otherwise>
 </xsl:choose>
</xsl:variable>


<xsl:template name="myheaders">
  <fo:static-content flow-name="xsl-region-before-right">
    <fo:block  text-align="justify" font-size="{$bodySize}">
        <fo:block>
           <fo:inline>
             <xsl:value-of select="$PRID"/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline>
               <fo:page-number/>
           </fo:inline>
        </fo:block>
    </fo:block>
  </fo:static-content>

  <fo:static-content flow-name="xsl-region-before-left">
     <fo:block text-align="justify" font-size="{$bodySize}">
        <fo:block>
           <fo:inline>
             <fo:page-number/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline>
             <xsl:value-of select = "/rr/RRstart/author"/>
           </fo:inline>
        </fo:block>
     </fo:block>
  </fo:static-content>

 <fo:static-content flow-name="xsl-region-before-first">
     <fo:block>
     </fo:block>
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-right">
   <fo:block text-align="left" font-size="{$bodySize}">
        <fo:block>
           <fo:inline>
             <xsl:value-of select="/rr/@type"/> nº <xsl:value-of select="$rrno"/>
           </fo:inline>
        </fo:block>
     </fo:block>
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-left">
    <fo:block text-align="right" font-size="{$bodySize}">
        <fo:block>
           <fo:inline>
             INRIA
           </fo:inline>
        </fo:block>
     </fo:block>
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-first">
  </fo:static-content>
</xsl:template>

<!--
le filet qui ne marche pas
  <fo:block text-align="justify" space-before="2mm">
     <fo:leader leader-pattern="rule" rule-thickness=".3pt"/></fo:block>
-->



<xsl:template match="resume">
 <fo:block text-align="justify" space-before="5mm">
  <fo:inline font-weight='bold'> Résumé :</fo:inline>
  <xsl:apply-templates/>
 </fo:block>
</xsl:template>

<xsl:template match="abstract">
 <fo:block text-align="justify" space-before="5mm">
  <fo:inline font-weight='bold'>Abstract: </fo:inline>
  <xsl:apply-templates/>
 </fo:block>
</xsl:template>

<xsl:template match="motcle">
 <fo:block space-before="5pt">
  <fo:inline font-weight='bold'> Mots clés : </fo:inline>
  <xsl:apply-templates/>
 </fo:block>
</xsl:template>

<xsl:template match="keyword">
 <fo:block space-before="5pt" >
  <fo:inline font-weight='bold'>Keywords: </fo:inline>
  <xsl:apply-templates/>
 </fo:block>
</xsl:template>




<xsl:template match="moreinfo">
  <fo:block font-style="italic"> <xsl:apply-templates/> </fo:block>
</xsl:template>


<xsl:template match="anchor" mode="section">
</xsl:template>

<xsl:template match="anchor">
  <fo:inline> 
     <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
  </fo:inline>
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


<xsl:template name="separateur.objet.spec">
 <xsl:choose>
  <xsl:when test="position() &lt; last()-1">, </xsl:when>
  <xsl:otherwise>.</xsl:otherwise>
 </xsl:choose>
</xsl:template>

<xsl:template match='btitle'>
  <fo:inline font-style='italic'><xsl:apply-templates/>. </fo:inline> 
</xsl:template>

<xsl:template match ="bauteurs">
  <fo:inline font-variant='small-caps'>
     <xsl:for-each select="bpers">
        <xsl:value-of select="@prenom"/> 
        <xsl:text> </xsl:text>
        <xsl:if test="@part">
           <xsl:text> </xsl:text>
           <xsl:value-of select="@part"/>
        </xsl:if>
        <xsl:value-of select="@nom"/>
        <xsl:call-template name="separateur.objet"/>
     </xsl:for-each>
   </fo:inline> 
</xsl:template>

<xsl:template match='bvolume|bnumber|bpages|bchapter|bseries|bedition'>
  <xsl:value-of select="@bname"/>
  <xsl:text> </xsl:text><xsl:apply-templates/>
  <xsl:call-template name="separateur.objet.spec"/>
</xsl:template>

<xsl:template match='bjournal|bbooktitle'>
  <xsl:text>in «&#x000A0;</xsl:text><xsl:apply-templates/><xsl:text>&#x000A0;»</xsl:text>
  <xsl:call-template name="separateur.objet.spec"/>
</xsl:template>

<xsl:template match='byear|bmonth|btype|bschool|bpublisher'>
  <xsl:apply-templates/>
  <xsl:call-template name="separateur.objet.spec"/>
</xsl:template>

<xsl:template match ="bnote|borganization|
     binstitution|baddress|bhowpublished">
  <xsl:value-of select="@bname"/><xsl:text> </xsl:text>
    <xsl:apply-templates/>
  <xsl:call-template name="separateur.objet.spec"/>
</xsl:template>


<xsl:template match ="bediteur">
  <xsl:text> éditeurs </xsl:text> 
  <fo:inline font-variant='small-caps'>
     <xsl:for-each select="bpers">
        <xsl:value-of select="@prenom"/> 
        <xsl:text> </xsl:text>
        <xsl:if test="@part">
           <xsl:text> </xsl:text>
           <xsl:value-of select="@part"/>
        </xsl:if>
        <xsl:value-of select="@nom"/>
        <xsl:call-template name="separateur.objet"/>
     </xsl:for-each>
   </fo:inline> 
  <xsl:call-template name="separateur.objet.spec"/>
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

<xsl:template match="simplemath">
  <m:math><m:mi><xsl:apply-templates/></m:mi></m:math>
</xsl:template>




<xsl:template name="sec.num">
   <xsl:value-of select = 
         "ancestor-or-self::*[self::composition or self::presentation or self::fondements or 
     self::domaine or self::logiciels or self::resultats or 
self::contrats or self::international or self::diffusion or self::biblio]/@numero"/>
  <xsl:text>.</xsl:text>
</xsl:template>

<xsl:template name="calculateNumber">
   <xsl:param name="numbersuffix"/>
       <xsl:number level="multiple" from="rr"
         count="div0|div1|div2|div3|div4"/>
       <xsl:value-of select="$numbersuffix"/>
</xsl:template>

<xsl:template name="calculateFootnoteNumber">
    <xsl:number  from="text" level="any" count="note[@place='foot']"/>
</xsl:template>

<xsl:template name="calculateFigureNumber">
     <xsl:number count="figure[@rend != 'inline']" level="any"/>
</xsl:template>

<xsl:template name="calculateTableNumber">
    <xsl:number count="table[@rend != 'inline']" level="any"/>
</xsl:template>

<xsl:template match='table' mode="xref">
   <xsl:call-template name="calculateTableNumber"/>
</xsl:template>

<xsl:template name="addID">
      <xsl:attribute name="id">
        <xsl:call-template name="idLabel"/>
      </xsl:attribute>
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

<xsl:template match="table">
  <xsl:choose>
    <xsl:when test="@rend='inline'">
      <xsl:call-template name="blockTable"/>
    </xsl:when>
    <xsl:otherwise>
      <xsl:call-template name="floatTable"/>
    </xsl:otherwise>
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
  <xsl:if test="@width">
   <xsl:attribute name="width">
    <xsl:value-of select="@width"/></xsl:attribute>
  </xsl:if>
   <xsl:call-template name="addID"/>
   <xsl:call-template name="deriveColSpecs"/>
   <fo:table-body text-indent="0pt">
     <xsl:for-each select="row">
       <xsl:text>
</xsl:text>
       <fo:table-row>
  <xsl:if test="@specialrow">   
    <xsl:attribute name="specialrow">true</xsl:attribute>
  </xsl:if>

         <xsl:apply-templates select="cell"/>
       </fo:table-row>
      </xsl:for-each>
   </fo:table-body>
 </fo:table>
</xsl:template>

<xsl:template name="generateTableID">
  <xsl:choose>
    <xsl:when test="@id">
      <xsl:value-of select="@id"/>
    </xsl:when>
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

<xsl:include href="raweb3-table.xsl"/>
<xsl:include href="raweb3-makecolspec.xsl"/>

<xsl:template match="preview">
  <preview>
  <xsl:apply-templates/>
  </preview>
</xsl:template>


<xsl:template match="TeX">
   <TeX/>
</xsl:template>
<xsl:template match="LaTeX">
   <LaTeX/>
</xsl:template>

<xsl:template match="TeX" mode ="section">
   <TeX/>
</xsl:template>
<xsl:template match="LaTeX" mode ="section">
   <LaTeX/>
</xsl:template>


<xsl:template match="theme" mode="titre2">
 <xsl:variable name="th" select="normalize-space(.)"/>
  <fo:block text-align="center" space-before="10pt">
  <xsl:choose>
    <xsl:when test="$th=1"> Thème 1 --- Réseaux et systèmes</xsl:when>
    <xsl:when test="$th=2"><fo:block> Thème 2 --- Génie logiciel
        </fo:block><fo:block>et calcul symbolique</fo:block></xsl:when>
    <xsl:when test="$th=3">
    <fo:block>Thème 3 --- Interaction homme-machine,
      </fo:block><fo:block>images, données, connaissances</fo:block></xsl:when>
    <xsl:when test="$th=4">
      <fo:block>Thème 4 --- Simulation et optimisation</fo:block>
      <fo:block>de systèmes complexes</fo:block></xsl:when>
   <xsl:otherwise>*** Numéro de thème incorrect. ***</xsl:otherwise>
  </xsl:choose>
  </fo:block>
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


</xsl:stylesheet>
