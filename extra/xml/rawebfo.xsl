<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format raweb XML documents 

$Id: rawebfo.xsl 1261 2007-09-10 13:56:01Z durollet $
Includes modifications by JG, december 2006
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

<!-- JG: dec 2006 
     removed headers-footers-twoside  and  headers-footers-oneside 
     because headers are constant
     Removed templates runninghead-title and runninghead-author 
     used only in the previous templates. See myheaders.
     Removed choosePageMaster, its always twoside one
     Removed grouptextStatic
     Removed text in mode="toc", in mode="xref", unqualified
     Removed * in mode="heading". Language is english 
-->


<xsl:stylesheet
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML" 
  xmlns:xlink="http://www.w3.org/1999/xlink"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
  exclude-result-prefixes="m xlink fotex">


<xsl:param name="linkColor">red</xsl:param>
<xsl:param name="pageMarginRight">80pt</xsl:param>
<xsl:param name="pdfBookmarks"></xsl:param>

<xsl:include href="raweb-param.xsl"/>
<xsl:include href="bibliofo.xsl"/>
<xsl:include href="tablefo.xsl"/>
<xsl:include href="listfo.xsl"/>


<!-- ignorer les espaces dans les éléments de la liste -->
<xsl:strip-space elements="UR tr table"/>
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


<!-- enable, for new elements to get commented as such -->
<!-- DEBUG
<xsl:template match="*">
 <xsl:comment> PASS THROUGH <xsl:value-of select="name()"/> </xsl:comment>
    <xsl:apply-templates/> 
</xsl:template>
-->

<!-- le raweb 
Changes JG. cleardoublepage removed -->
<xsl:template match="raweb">
 <fo:root xmlns:fo="http://www.w3.org/1999/XSL/Format">
   <xsl:call-template name="setupPagemasters"/>
   <xsl:call-template name="myheaders"/>
   <xsl:apply-templates select="identification"/> 
   <xsl:call-template name="myTOC"/>
   <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
      >
      <fo:flow  
         flow-name="xsl-region-body"
         font-family="{$bodyFont}"  
         font-size="{$bodySize}">
        <xsl:call-template name="raweb.body"/>
      </fo:flow>
   </fo:page-sequence>
 </fo:root>
</xsl:template>

<xsl:template name="raweb.body">
    <xsl:apply-templates select="identification/team"/>
    <xsl:apply-templates select="presentation"/>
    <xsl:apply-templates select="fondements"/>
    <xsl:apply-templates select="domaine"/>
    <xsl:apply-templates select="logiciels"/>
    <xsl:apply-templates select="resultats"/>
    <xsl:apply-templates select="contrats"/>
    <xsl:apply-templates select="international"/>
    <xsl:apply-templates select="diffusion"/>
    <xsl:apply-templates select="biblio"/>
</xsl:template>


<!-- Composition. ajoute les moreinfo -->
<!-- tahia<xsl:template match="composition">-->
<xsl:template match="team">
 <xsl:call-template name="secNumberedHeading"/>
 <xsl:if test="../moreinfo">
   <xsl:apply-templates select="../moreinfo"/>
 </xsl:if>
 <xsl:apply-templates select="participants"/>
</xsl:template>




<!-- Autres sections -->
<xsl:template match="identification|presentation|fondements|domaine|logiciels|resultats|contrats|international|diffusion">
   <xsl:call-template name="secNumberedHeading"/>
   <xsl:apply-templates select="node()[(local-name() != 'bodyTitle')]"/>
</xsl:template>


<!-- Les sous-sections -->
  <xsl:template match="subsection"> 
    <xsl:if test="bodyTitle!='(Sans Titre)'">
      <xsl:call-template name="NumberedHeading">
	<xsl:with-param name="level">
	  <xsl:value-of select="count(ancestor-or-self::subsection)"/>
	</xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <xsl:choose>
      <xsl:when  test="not(./subsection)"> 
	<xsl:call-template name="keywords-list" />
      </xsl:when>
      <xsl:otherwise>
	<xsl:call-template name="keywords-list2" />
      </xsl:otherwise>    
    </xsl:choose>
    <xsl:apply-templates select="node()[(local-name() != 'bodyTitle')]"/>
  </xsl:template>



<xsl:template name="secNumberedHeading">
  <fo:block keep-with-next.within-page="always">
    <xsl:variable name="divid">        
       <xsl:call-template name="idLabel"/>
    </xsl:variable>
    <xsl:attribute name="id"> <xsl:value-of select="$divid"/> </xsl:attribute>
    <xsl:attribute name="text-align">start</xsl:attribute>
    <xsl:attribute name="font-family">
       <xsl:value-of select="$divFont"/></xsl:attribute>
    <xsl:call-template name="setupDiv0"/>
    <xsl:call-template name="blockStartHook"/>
    <!-- tahia<xsl:value-of select="@numero"/> -->
    <xsl:call-template name="calculateNumberSection"/>
    <xsl:if test="parent::raweb">
     <xsl:text> </xsl:text>
    </xsl:if>
    <xsl:choose>
      <xsl:when test="name()='identification'">Identification
      </xsl:when>
      <xsl:when test="name()='team'"><xsl:text> </xsl:text> Team
      </xsl:when>
      <xsl:when test="name()='biblio'">Bibliography
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="bodyTitle"/>
</xsl:otherwise>
    </xsl:choose>
    <xsl:if test="$pdfBookmarks='true'">
    <bookmark  
         xmlns:fotex="http://www.tug.org/fotex" 
         fotex-bookmark-level="0" 
         fotex-bookmark-label="{$divid}">
        <xsl:if test="not($numberHeadings='')">
          
    <!-- tahia<xsl:value-of select="@numero"/> -->
    <xsl:call-template name="calculateNumberSection"/>
          <xsl:value-of select="$headingNumberSuffix"/>
        </xsl:if>
    <xsl:choose>
      <xsl:when test="name()='identification'">Team
      </xsl:when>
      <xsl:when test="name()='biblio'">Bibliography
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="bodyTitle"/>
      </xsl:otherwise>
    </xsl:choose>
      </bookmark>
    </xsl:if>
  </fo:block>
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
    <xsl:apply-templates mode="section" select="bodyTitle"/>
    <xsl:if test="$pdfBookmarks='true'">
     <!-- Passive TeX extension, to get PDF bookmarks -->
      <bookmark  
        xmlns:fotex="http://www.tug.org/fotex" 
        fotex-bookmark-level="{$level}" 
        fotex-bookmark-label="{$divid}">
       <xsl:if test="not($numberHeadings='')">
         <xsl:value-of select="$Number"/>
         <xsl:value-of select="$headingNumberSuffix"/>
       </xsl:if>
       <xsl:value-of select="bodyTitle"/>
     </bookmark>
    </xsl:if>
  </fo:block>
</xsl:template>

<!-- Table des matières
Changes JG: master-name removed, force-page-count addded
 -->
<xsl:template name="myTOC">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
        force-page-count="end-on-even"
      > 
<fo:flow flow-name="xsl-region-body">
    <fo:block>
       <fo:block font-size="14pt" text-align="center" font-weight="bold" 
           space-after="20pt">
         <xsl:text>Table of contents</xsl:text>
       </fo:block>
       <xsl:for-each select="//team|//presentation|//fondements|
             //domaine|//logiciels|//resultats|//contrats|//international|
              //diffusion|//biblio|//subsection">
          <xsl:apply-templates mode="xtoc" select="(.)"/>
       </xsl:for-each>
    </fo:block>
</fo:flow>
   </fo:page-sequence>
</xsl:template>


<xsl:template mode="xtoc" match="subsection">
 <xsl:if test="bodyTitle!='(Sans Titre)'">
   <xsl:call-template name="tocheading">
    <xsl:with-param name="level">
        <xsl:value-of select="count(ancestor-or-self::subsection)"/>
    </xsl:with-param>
   </xsl:call-template>
 </xsl:if>
</xsl:template>


<xsl:template mode="xtoc" match="team|presentation|fondements|domaine|logiciels|resultats|contrats|international|diffusion|biblio">
  <xsl:variable name="tocindent">
         <xsl:value-of select="$div0Tocindent"/>
  </xsl:variable>
  <fo:block>
      <xsl:attribute name="font-weight">bold</xsl:attribute>
      <xsl:attribute name="text-indent">
       <xsl:value-of select="$tocindent"/>
      </xsl:attribute>
   <xsl:call-template name="calculateNumberSection"/>
   <xsl:text>&#x2003;</xsl:text>
   <fo:inline>
<xsl:choose>
      <xsl:when test="name()='team'">Team
      </xsl:when>
      <xsl:when test="name()='biblio'">Bibliography
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="bodyTitle"/>
      </xsl:otherwise>
    </xsl:choose>
   </fo:inline>
    <fo:leader leader-pattern="dots" />
    <fo:inline color="{$linkColor}">
      <xsl:variable name="pagref">
        <xsl:call-template name="idLabel"/>
      </xsl:variable>
<!--MPP no de page de section-->
     <fo:basic-link internal-destination="{$pagref}">
      <fo:page-number-citation ref-id="{$pagref}"/>
     </fo:basic-link>

    </fo:inline> 
  </fo:block>
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
    <xsl:when test="$level='5'">
         <xsl:value-of select="$div5Tocindent"/></xsl:when>
    <xsl:otherwise><xsl:value-of select="$div6Tocindent"/></xsl:otherwise>
   </xsl:choose>
  </xsl:variable>
 <xsl:variable name="tocwidth">
   <xsl:choose>
    <xsl:when test="$level='0'">
        <xsl:value-of select="$div0Tocwidth"/></xsl:when>
    <xsl:when test="$level='1'">
         <xsl:value-of select="$div1Tocwidth"/></xsl:when>
    <xsl:when test="$level='2'">
         <xsl:value-of select="$div2Tocwidth"/></xsl:when>
    <xsl:when test="$level='3'">
         <xsl:value-of select="$div3Tocwidth"/></xsl:when>
    <xsl:when test="$level='4'">
         <xsl:value-of select="$div4Tocwidth"/></xsl:when>
    <xsl:when test="$level='5'">
         <xsl:value-of select="$div5Tocwidth"/></xsl:when>
    <xsl:otherwise><xsl:value-of select="$div6Tocwidth"/></xsl:otherwise>
   </xsl:choose>
  </xsl:variable>
<!-- On traite ici le dedoblonnement du titre de la section et du titre du module lorsqu'il n'y a qu'un module dans la section-->
      <xsl:choose>
        <xsl:when test="count(../subsection)=1 and bodyTitle=../subsection/bodyTitle">
	</xsl:when>
        <xsl:otherwise>

  <fo:block toc='true' margin-right='15pt' margin-left='{$tocwidth}'>
    <xsl:attribute name="text-indent">
       <xsl:value-of select="$tocindent"/>
    </xsl:attribute>
    <xsl:variable name="Number">
     <xsl:call-template name="calculateNumber">
       <xsl:with-param name="numbersuffix" select="$tocNumberSuffix"/>
     </xsl:call-template>
    </xsl:variable>

   <xsl:element name="fotex:section-number">
      <xsl:attribute name="width">
        <xsl:value-of select="$tocwidth"/>
      </xsl:attribute>
      <fo:inline>
        <xsl:value-of select="$Number"/>
      </fo:inline>
    </xsl:element>
 
<fo:inline>
      <xsl:choose>   
        <xsl:when test="bodyTitle!='(Sans Titre)'">
          <xsl:apply-templates mode="section" select="bodyTitle"/>
        </xsl:when>
        <xsl:otherwise>Introduction</xsl:otherwise>
      </xsl:choose>
    </fo:inline>

    <xsl:element name="fotex:page-number">
    <fo:inline color="{$linkColor}">
      <xsl:variable name="pagref">
        <xsl:call-template name="idLabel"/>
      </xsl:variable>
     <fo:basic-link internal-destination="{$pagref}">
      <fo:page-number-citation ref-id="{$pagref}"/>
     </fo:basic-link>
    </fo:inline> 
    </xsl:element>
  </fo:block>
        </xsl:otherwise>
      </xsl:choose>
</xsl:template>

<!-- Le id disparait ici -->
<xsl:template match="head" mode="tocsection">
   <xsl:apply-templates mode="section"/>
</xsl:template>

<!-- Le id reste ici -->
<xsl:template match="bodyTitle" mode="section">
  <fo:inline>    
 <xsl:if test=".!='(Sans Titre)'">
   <xsl:if test="@id">
      <xsl:attribute name="id">
      <xsl:value-of select="translate(@id,'_','-')"/>
      </xsl:attribute>
   </xsl:if>
   <xsl:apply-templates mode="section"/>
 </xsl:if>
  </fo:inline>
</xsl:template>


<!-- unqualified <head> -->
<xsl:template match="bodyTitle">
 <xsl:variable name="parent" select="name(..)"/>
 <xsl:if test="not(starts-with($parent,'subsection'))">
  <xsl:apply-templates/>
 </xsl:if>
</xsl:template>

<xsl:template match="bodyTitle" priority="10">
 <xsl:variable name="parent" select="name(..)"/>
 <xsl:variable name="grandparent" select="name(../../..)"/>
 <xsl:if test="not($parent = 'subsection')and not($grandparent = 'raweb')"> 
  <xsl:apply-templates/>
 </xsl:if>
</xsl:template>

<xsl:template name="positionInBib">
  <xsl:param name="str"/>
  <xsl:param name="countPrevious"/>
  <xsl:param name="current"/>

  <xsl:for-each select="key('bibliotypes',$str)">
   <xsl:sort select="descendant::author[1]/persName[1]/surname/text()"/>
      <xsl:if test="generate-id(.)=generate-id($current)">
        <xsl:value-of select="$countPrevious+position()"/>
      </xsl:if> 
  </xsl:for-each>
</xsl:template>


  <!-- références internes ou externes-->

  <xsl:template match="ref">
    <xsl:choose>
      <xsl:when test="starts-with(@xlink:href, '#')"> <!-- lien interne sur un Id -->
	<xsl:variable name ="curid" select="substring-after(@xlink:href, '#')"/>
	<xsl:variable name ="curelement" select="/raweb//*[@id=$curid]"/>
	<xsl:variable name="curlabel" select="$curelement/ancestor-or-self::subsection" />
	<xsl:variable name="curlabelbiblio" select="$curelement/ancestor-or-self::biblio"/>
	<xsl:variable name="curlabelidentification" select="$curelement/ancestor-or-self::identification"/>
	<!-- il faut traiter le cas ou childs empty -->
	<xsl:choose>
	  <xsl:when test="$curlabelidentification"> 
	    <xsl:apply-templates select="/raweb/identification/team" mode="pre-ref" />	
	    <fo:basic-link color="{$linkColor}" internal-destination="{$curid}">
	      <xsl:apply-templates select="/raweb/identification/team" mode="xref"/>
	    </fo:basic-link>
	    <xsl:apply-templates select="/raweb/identification/team" mode="post-ref" />
	  </xsl:when>
	  <xsl:when test="@location='biblio'">	
	    <xsl:text>[</xsl:text>
	    <fo:basic-link color="{$linkColor}">
	      <xsl:attribute name="internal-destination">
		<xsl:value-of select="$curid"/>
	      </xsl:attribute>

	      <xsl:choose>

		<xsl:when test="count($curelement|key('bibliotypes', 'd'))=count(key('bibliotypes', 'd'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'d'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="0"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'e'))=count(key('bibliotypes', 'e'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'e'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'f'))=count(key('bibliotypes', 'f'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'f'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'g'))=count(key('bibliotypes', 'g'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'g'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e+$f"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'h'))=count(key('bibliotypes', 'h'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'h'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e+$f+$g"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'i'))=count(key('bibliotypes', 'i'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'i'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e+$f+$g+$h"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'j'))=count(key('bibliotypes', 'j'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'j'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e+$f+$g+$h+$i"/>
		  </xsl:call-template>
		</xsl:when>
		<xsl:when test="count($curelement|key('bibliotypes', 'k'))=count(key('bibliotypes', 'k'))">
		  <xsl:call-template name="positionInBib">
		    <xsl:with-param name="str" select="'k'"/>
		    <xsl:with-param name="current" select="$curelement"/>
		    <xsl:with-param name="countPrevious" select="$d+$e+$f+$g+$h+$i+$j"/>
		  </xsl:call-template>
		</xsl:when>
	      </xsl:choose>
	    </fo:basic-link>	
	    <xsl:text>]</xsl:text>
	  </xsl:when>
	  <!-- Hum, mettre section X, ou table X ou Figure X et pas seulement [X] -->
	  <xsl:when test="@location='intern'">
	    <!-- <xsl:text></xsl:text> -->
	    <xsl:apply-templates select="$curelement" mode="pre-ref" />	
	    <fo:basic-link color="{$linkColor}" internal-destination="{$curid}">
	      <xsl:apply-templates select="$curelement" mode="xref"/>
	    </fo:basic-link>
	    <xsl:apply-templates select="$curelement" mode="post-ref" />
	    <!--  <xsl:text>]</xsl:text> -->
	  </xsl:when>
	</xsl:choose>
      </xsl:when>
      <xsl:otherwise>	
	  <xsl:choose>
	    <xsl:when test="@location='extern'"> 
	      <fo:basic-link color="{$linkColor}">
		<xsl:attribute name="external-destination">
		  <xsl:value-of select="@xlink:href"/>
		</xsl:attribute>
		<xsl:apply-templates/>
	      </fo:basic-link>			
	    </xsl:when>

	    <xsl:otherwise>	
	      	        <fo:basic-link color="{$linkColor}" internal-destination="./{$curlabel[1]/@id}.html{@xlink:href}">
		<xsl:apply-templates/>
	      </fo:basic-link>					
	    </xsl:otherwise>
	  </xsl:choose>
	</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

<xsl:template match="refperson" mode="section">
  <xsl:variable name ="curid" select="@ref"/>
  <xsl:variable name ="curelement" select="/raweb//*[@id=$curid]"/>
  <xsl:if test="$curelement[name()='person']">
     <xsl:apply-templates select="$curelement" mode="section"/>
  </xsl:if>
</xsl:template>



<!-- element appartenant a BIBLIO -->
<xsl:template match="citation/xref">
   <fo:basic-link color="{$linkColor}">
   <xsl:attribute name="external-destination">
     <xsl:value-of select="@url"/>
   </xsl:attribute>
      <xsl:apply-templates/>
   </fo:basic-link>
  <xsl:call-template name="separateur.objet"/>
</xsl:template>


<!-- headings when cross-referenced -->

<xsl:template match="table" mode="pre-ref"></xsl:template>
<xsl:template match="table" mode="post-ref"></xsl:template>

<!-- Normalement c'est le numéro de la section -->
<xsl:template match="subsection" mode="pre-ref" />
<xsl:template match="biblio|presentation|fondements|domaine|logiciels|resultats|contrats|identification|international|diffusion|team" mode = 'pre-ref' />

<xsl:template match="subsection" mode="post-ref"></xsl:template>

<xsl:template match="subsection" mode="xref"><xsl:call-template name="calculateNumber"/></xsl:template>

<xsl:template match="bodyTitle" mode ="xref"><xsl:call-template name="calculateNumber"/></xsl:template>

<xsl:template match="anchor" mode ="xref"><xsl:call-template name="calculateNumber"/></xsl:template>


<xsl:template match="biblio|presentation|fondements|domaine|logiciels|resultats|contrats|identification|international|diffusion|team" mode = 'xref'>
  <!-- <xsl:value-of select = "@numero"/> -->
    <xsl:call-template name="calculateNumberSection"/></xsl:template>


<!-- pas pareil pour les maths, les figures les tables -->

<xsl:template match="formula" mode="xref"><xsl:number level = "any" count="formula[@id]"/></xsl:template>

<xsl:template match="formula" mode="pre-ref"></xsl:template>

<xsl:template match="formula" mode="post-ref"></xsl:template>
<xsl:template match="biblio|presentation|fondements|domaine|logiciels|resultats|contrats|identification|international|diffusion|team" mode = 'post-ref' />


<xsl:template name="xheading">
   <xsl:param name="level"/>
   <xsl:if test="not($numberHeadings='')">
     <xsl:call-template name="calculateNumber"/>
   </xsl:if>
   <xsl:call-template name="divXRefHeading"/>
</xsl:template>

<xsl:template match="processing-instruction()[name()='xmltex']" >
   <xsl:message>xmltex pi <xsl:value-of select="."/></xsl:message>
   <xsl:copy-of select="."/>
</xsl:template>


<!-- Changes JG:  all elements had a master-reference and a master-name 
attribute with the same value; spurious ones removed. 
A xx-page-master element has a master-name attribute
a xx-page-master-reference has a master-reference attribute -->

<xsl:template name="setupPagemasters">
  <fo:layout-master-set>
    
    <!-- one sided, single column -->
    
    <fo:simple-page-master
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
       master-name="first1"
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
	 region-name="xsl-region-before-first"
	 extent="{$regionBeforeExtent}"/>
      <fo:region-after 
	 region-name="xsl-region-after-first"
	 extent="{$regionAfterExtent}"/>
    </fo:simple-page-master>

    <!-- blank page in either mode, single column -->
    <fo:simple-page-master
       master-name="blank1"
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
         region-name="DummyRegion"
         extent="{$regionBeforeExtent}"/>
      <fo:region-after 
         region-name="DummyRegion"
         extent="{$regionAfterExtent}"/>
    </fo:simple-page-master>
    
    <!-- for pages in one-side mode, 2 column -->
    <fo:simple-page-master
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
       master-name="first2"
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
	 region-name="xsl-region-before-first"
	 extent="{$regionBeforeExtent}"/>
      <fo:region-after 
	 region-name="xsl-region-after-first"
	 extent="{$regionAfterExtent}"/>
    </fo:simple-page-master>
    
    <!-- setup for double-sided, 1 column, no first page -->
    <fo:page-sequence-master master-name="twoside1nofirst">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
	   master-reference="right1"
	   odd-or-even="odd"/>
        <fo:conditional-page-master-reference
	   master-reference="left1"
	   odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
    </fo:page-sequence-master>
    
    <!-- setup for double-sided, 1 column -->
    <fo:page-sequence-master master-name="twoside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
	   master-reference="first1"
	   page-position="first"/>
        <fo:conditional-page-master-reference
	   master-reference="right1"
	   odd-or-even="odd"/>
        <fo:conditional-page-master-reference
	   master-reference="left1"
	   odd-or-even="even"/>
	<fo:conditional-page-master-reference
           master-reference="blank1"
           blank-or-not-blank="blank"/>
      </fo:repeatable-page-master-alternatives>
    </fo:page-sequence-master>
    
    <!-- setup for single-sided, 1 column -->
    <fo:page-sequence-master  master-name="oneside1">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
	   master-reference="first1"
	   page-position="first"/>
        <fo:conditional-page-master-reference 
	   master-reference="simple1"/>
      </fo:repeatable-page-master-alternatives>
    </fo:page-sequence-master>
    
    
    <!-- setup for double-sided, 2 column -->
    <fo:page-sequence-master master-name="twoside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
	   master-reference="first2"
	   page-position="first"/>
        <fo:conditional-page-master-reference
	   master-reference="right2"
	   odd-or-even="odd"/>
        <fo:conditional-page-master-reference
	   master-reference="left2"
	   odd-or-even="even"/>
      </fo:repeatable-page-master-alternatives>
    </fo:page-sequence-master>
    
    <!-- setup for single-sided, 2 column -->
    <fo:page-sequence-master  master-name="oneside2">
      <fo:repeatable-page-master-alternatives>
        <fo:conditional-page-master-reference
	   master-reference="first2"
	   page-position="first"/>
        <fo:conditional-page-master-reference 
	   master-reference="simple2"
           />
      </fo:repeatable-page-master-alternatives>
    </fo:page-sequence-master>
    <xsl:call-template name="hookDefinepagemasters"/>
    
  </fo:layout-master-set>
</xsl:template>



<xsl:template match="*" mode="plain">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="text()" mode="plain">
  <xsl:value-of select="normalize-space(.)"/>
</xsl:template>

<xsl:template match="p">
  <fo:block font-size="{$bodySize}">
<!-- Next comment is strange: There is no such bug in Tralics.
     If a paragraph should not be indented then @noindent is set true.
     In 2006, indentation was replaced by some vertical space. This can 
     conflict with attributes space-before.optimum and space-before.maximum
     This should be fixed (JG)
     -->
<!-- Hum on ajoute pour fixer un bug tralics! -->
      <xsl:choose>
         <xsl:when test ="@noindent">
         </xsl:when>
         <xsl:otherwise>
           <!-- by default, indent paragraph -->
<!--	   <xsl:attribute name="text-indent">
              <xsl:value-of select="$parIndent"/>-->
	   <xsl:attribute name="space-before">
	    <xsl:text>4pt</xsl:text>
           </xsl:attribute>
         </xsl:otherwise>
       </xsl:choose>
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
    <xsl:if test="not(preceding-sibling::p)">
       <xsl:if test="@spacebefore">
          <xsl:attribute name="space-before.optimum">
             <xsl:value-of select="@spacebefore"/>
          </xsl:attribute>
       </xsl:if>
    </xsl:if>
 
<!-- ARG, useless with the new DTD but ... bug -->

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

<xsl:template match="em">
<fo:inline>
 <xsl:call-template name="style">
   <xsl:with-param name="defaultvalue" select="string('bold')"/>
   <xsl:with-param name="defaultstyle" select="string('font-weight')"/>
 </xsl:call-template>
      <xsl:apply-templates/>
</fo:inline>
</xsl:template>

<xsl:template name="style">
  <xsl:param name="defaultvalue"/>
  <xsl:param name="defaultstyle"/>
  <xsl:choose>
    <xsl:when test="@style='UNDERLINE'">
      <xsl:attribute name="text-decoration">underline</xsl:attribute>
    </xsl:when>
    <xsl:when test="@style='overline'">
      <xsl:attribute name="text-decoration">overline</xsl:attribute>
    </xsl:when>
    <xsl:when test="@style='sansserif'">
      <xsl:attribute name="font-family">sansserif</xsl:attribute>
    </xsl:when>
   <xsl:when test="@style='slanted'">
      <xsl:attribute name="font-style">oblique</xsl:attribute>
    </xsl:when>
    <xsl:when test="@style='HIGHLIGHT'">
      <xsl:attribute name="font-style">cursive</xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="{$defaultstyle}">
        <xsl:value-of select="$defaultvalue"/>
      </xsl:attribute>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="b">
  <fo:inline font-weight="bold">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="i">
  <fo:inline font-style="italic">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="tt">
  <fo:inline font-family='Computer-Modern-Typewriter'>
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="sup">
  <fo:inline vertical-align="super">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="sub">
  <fo:inline vertical-align="sub">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="small">
  <fo:inline font-size="8pt">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="span">
  <fo:inline font-variant='small-caps'>
    <xsl:apply-templates />
  </fo:inline> 
</xsl:template>

<xsl:template match="big">
  <fo:inline font-size="12pt">
    <xsl:apply-templates />
  </fo:inline>
</xsl:template>

<xsl:template match="ident">
    <fo:inline color="{$identColor}" font-family="{$sansFont}">
        <xsl:apply-templates/>
    </fo:inline>
</xsl:template>


<!-- JG dec2006: Code simplified; value of place attribute ignored -->
<xsl:template match="footnote[not(ancestor::biblStruct)]">
  <xsl:variable name="FootID">
    <xsl:call-template name="calculateFootnoteNumber"/>
  </xsl:variable>
  <fo:footnote>
    <fo:inline font-size="{$footnotenumSize}" vertical-align="super">
      <xsl:value-of select="$FootID"/>
    </fo:inline>
    <fo:footnote-body>
      <fo:block end-indent="0pt"
		start-indent="0pt"
		text-indent="{$parIndent}" font-size="{$footnoteSize}">
        <fo:inline font-size="{$footnotenumSize}"
		   vertical-align="super">
          <xsl:value-of select="$FootID"/>
        </fo:inline>
	<xsl:apply-templates/>
      </fo:block>
    </fo:footnote-body>
  </fo:footnote> 
</xsl:template>



<!-- =======================================OBJECT================== -->



<!-- insertion d'image ou autre element media -->

<xsl:template match="ressource">
<!-- il faut tester si cette ressource est pour le PDF. S'il n'existe
pas d'autres ressources de meme niveau dont l'attribut media est egal
a PRINT alors on applique n'importe quelle ressource -->

<xsl:if test="(@media='PRINT') or not(../ressource[@media='PRINT'])">
  <xsl:call-template name='generate-graphics'/>
      <xsl:if test="caption">
        <fo:block text-align="center">
           <xsl:apply-templates select="caption"/>
        </fo:block>
      </xsl:if>
</xsl:if>
</xsl:template>

<!-- environnement object -->

<xsl:template match="object" >
  <fo:float>
    <xsl:call-template name="addID"/>
    <fo:block>
      <xsl:apply-templates select="table" mode="object"/>
    </fo:block>
    <fo:block>
      <xsl:call-template name="figureCaptionstyle"/>
      <xsl:value-of select="$figureWord"/>
      <xsl:call-template name="calculateObjectNumber"/>
      <xsl:text>. </xsl:text>
      <xsl:apply-templates select="caption"/>
    </fo:block>
  </fo:float>
</xsl:template>



<!-- Cree le external graphics pour une ressource -->

<xsl:template name="generate-graphics">
  <xsl:variable name="File">
    <xsl:value-of select="@xlink:href"/>
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




<xsl:template name="calculateObjectNumber"><xsl:number count="object" level="any"/></xsl:template>

<xsl:template name="calculateRessourceNumber"><xsl:number count="ressource" level="any"/></xsl:template>

<xsl:template match="object" mode="pre-ref"></xsl:template>

<xsl:template match="object" mode="post-ref"></xsl:template>

<xsl:template match='object' mode="xref"><xsl:call-template name="calculateObjectNumber"/></xsl:template>

<xsl:template match="ressource" mode="xref"><xsl:call-template name="calculateRessourceNumber"/></xsl:template>

<xsl:template match="object/caption"><xsl:apply-templates/></xsl:template>

<xsl:template match="ressource/caption"><xsl:apply-templates/></xsl:template>

<!-- ============================FIN OBJECT=========== -->

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


<xsl:template match="formula[@type='display']">
 <xsl:choose>
    <xsl:when test="@id">
      <fo:inline>
       <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
       <xsl:element name="fotex:equation">
         <xsl:for-each select="m:math">
           <xsl:apply-templates mode="math"/>
         </xsl:for-each>
       </xsl:element>
     </fo:inline>
  </xsl:when>
  <xsl:otherwise>   
   <xsl:element name="fotex:displaymath">
    <xsl:for-each select="m:math">
     <xsl:apply-templates mode="math"/>
    </xsl:for-each>
   </xsl:element>
   </xsl:otherwise>
 </xsl:choose>
</xsl:template>


<xsl:variable name="processor">
   <xsl:value-of select="system-property('xsl:vendor')"/>
</xsl:variable>

<xsl:variable name="LeTypeProjet">
  <xsl:choose>
    <xsl:when test='/raweb/identification/@isproject="true"'>Project-Team</xsl:when>
    <xsl:otherwise>Team</xsl:otherwise>
  </xsl:choose>
</xsl:variable>

<xsl:variable name="PRID">
 <xsl:value-of select="$LeTypeProjet"/>
 <xsl:text> </xsl:text>
 <xsl:value-of select="/raweb/identification/shortname"/>
</xsl:variable>


<xsl:variable name="year">
 <xsl:choose>
   <xsl:when test="/raweb/@year">
      <xsl:value-of select="/raweb/@year" />
   </xsl:when>
   <xsl:otherwise>2000</xsl:otherwise>
  </xsl:choose>
</xsl:variable>


<!-- JG used border-after instead of a special element-->
<xsl:template name="myheaders">
  <fo:static-content flow-name="xsl-region-before-right">
    <fo:block  text-align="justify" font-size="{$bodySize}">
        <fo:block  border-after-style="solid" text-indent="0pt">
           <fo:inline font-style="italic">
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
        <fo:block border-after-style="solid" text-indent="0pt">
           <fo:inline>
             <fo:page-number/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline font-style="italic">
              Activity Report INRIA <xsl:value-of select="$year"/>
           </fo:inline>
        </fo:block>
     </fo:block>
  </fo:static-content>



 <fo:static-content flow-name="xsl-region-before-first">
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-right">
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-left">
 </fo:static-content>

 <fo:static-content flow-name="xsl-region-after-first">
 </fo:static-content>
</xsl:template>

<!--
le filet qui ne marche pas
  <fo:block text-align="justify" space-before="2mm">
     <fo:leader leader-pattern="rule" rule-thickness=".3pt"/></fo:block>
-->

<!--
Changes JG: master-name removed, force-page-count addded
-->

<xsl:template match="identification">
 <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
        force-page-count="end-on-even"
      >
     <fo:flow font-style="italic" font-family="{$bodyFont}"> 
       <fo:INRIA year="{$year}"/>
       <fo:block font-size= "25pt" text-align="center"> 
         <fo:basic-link external-destination="http://www.inria.fr/recherche/equipes/{@id}.en.html">
            <xsl:value-of select="$LeTypeProjet"/> <xsl:text> </xsl:text>
            <xsl:value-of select="shortname"/> 
         </fo:basic-link>
       </fo:block>
        <fo:block font-size= "25pt" text-align="center" space-before="1cm"> 
              <xsl:value-of select="projectName"/> 
        </fo:block>
       <fo:block font-size= "17.28pt" text-align="center" space-before="1cm">
         <xsl:for-each select="UR">
         <xsl:choose>
            <xsl:when test="@name='Rocquencourt'">
                <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-rocq.en.html"><xsl:value-of select="@name"/></fo:basic-link>
            </xsl:when>
            <xsl:when test="@name='Rennes'">
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-ren.en.html"><xsl:value-of select="@name"/></fo:basic-link>
            </xsl:when>
            <xsl:when test="@name='Sophia'">
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-sop.en.html">Sophia Antipolis</fo:basic-link>
            </xsl:when>
            <xsl:when test="@name='Lorraine'">
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-lor.en.html"><xsl:value-of select="@name"/></fo:basic-link>
            </xsl:when>
            <xsl:when test="@name='RhoneAlpes'">
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-ra.en.html">Rhône-Alpes</fo:basic-link>
            </xsl:when>
            <xsl:when test="@name='Futurs'">
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme/fiche_ur-futurs.en.html"><xsl:value-of select="@name"/></fo:basic-link>
            </xsl:when>
            <xsl:otherwise>
               <fo:basic-link external-destination ="http://www.inria.fr/inria/organigramme">INRIA</fo:basic-link>
            </xsl:otherwise>
         </xsl:choose>
         <xsl:if test="position()!=last()"> - </xsl:if>
        </xsl:for-each>
       </fo:block>
       <fo:block font-size= "10pt" font-style="normal" font-family="Helvetica" text-align="center" space-before="1cm">
           <fo:RATHEME><xsl:value-of select="theme"/></fo:RATHEME>
        </fo:block>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>




<xsl:template match="moreinfo">
  <fo:block font-style="italic"> <xsl:apply-templates/> </fo:block>
</xsl:template>

<xsl:template match="person/moreinfo">
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="anchor" mode="section">
</xsl:template>

<xsl:template match="anchor">
  <fo:inline> 
     <xsl:attribute name="id"><xsl:value-of select="@id"/></xsl:attribute>
  </fo:inline>
</xsl:template>


<xsl:template name="separateur.objet">
 <xsl:choose>
  <xsl:when test="position() &lt; last()">, </xsl:when>
  <xsl:otherwise>.</xsl:otherwise>
 </xsl:choose>
</xsl:template>

  <xsl:template name="separateurED.objet">
       <xsl:if test="position() &lt; last()">, </xsl:if>
   </xsl:template>

<xsl:template name="separateur.objet.spec">
 <xsl:choose>
  <xsl:when test="position() &lt; last()">, </xsl:when>
  <xsl:otherwise>.</xsl:otherwise>
 </xsl:choose>
</xsl:template>



<!-- =========PERS======= -->

<xsl:template match="team/participants">
  <fo:block space-before="3pt">
    <fo:block font-weight='bold' text-indent="-15pt">
        <xsl:value-of select="translate(@category, '_', ' ')"/>
    </fo:block>
    <xsl:for-each select="person">
        <fo:block> <xsl:apply-templates select="."/></fo:block>
    </xsl:for-each>
  </fo:block>
</xsl:template>

<xsl:template match="person">
  <xsl:value-of select="./firstname"/> 
  <xsl:text> </xsl:text>
  <xsl:value-of select="./lastname"/>
  <xsl:choose>
    <xsl:when test="moreinfo">
      <xsl:if test="not(normalize-space(string(.)) ='')">
        <xsl:text> [&#xa0;</xsl:text>
        <xsl:apply-templates select="./moreinfo/node()" />
        <xsl:apply-templates select="hdr" />
        <xsl:text>&#xa0;] </xsl:text>
      </xsl:if>
    </xsl:when>
    <xsl:otherwise>
      <xsl:if test="./hdr">
        <xsl:text> [&#xa0;HdR&#xa0;] </xsl:text>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
<!--    <xsl:apply-templates select="moreinfo" mode="inpers"/>-->
</xsl:template>

  <xsl:template match="hdr">
       <xsl:text>, </xsl:text>
       <xsl:text>HdR</xsl:text>
  </xsl:template>

<xsl:template match="person" mode="section">
    <xsl:value-of select="./firstname"/> 
    <xsl:text> </xsl:text>
    <xsl:value-of select="./lastname"/>
    <xsl:apply-templates select="moreinfo" mode="inpers"/>
</xsl:template>


<xsl:template match="moreinfo" mode ="inpers">
  <xsl:if test="not(normalize-space(string(.)) ='')"><xsl:text> [</xsl:text><xsl:apply-templates/><xsl:text>]</xsl:text></xsl:if>
</xsl:template>

<xsl:template name="pluriel-p">
   <xsl:param name="liste" />
   <xsl:if test="count($liste)>1">s</xsl:if>
</xsl:template>


<xsl:template match="participants">
<xsl:if test="not(preceding-sibling::participants) and not(parent::team)">
 <fo:block space-after.optimum="4pt">
   <fo:inline>
     <xsl:attribute name="font-weight">bold</xsl:attribute>
<xsl:text>Participant</xsl:text><xsl:call-template name="pluriel-p"><xsl:with-param
    name="liste" select="person" /></xsl:call-template>: 
   </fo:inline>
   <fo:inline>
    <xsl:for-each select="person|following-sibling::participants/person|refperson">
       <xsl:apply-templates select="." mode="section"/>
       <xsl:call-template name="separateur.objet"/>
    </xsl:for-each>
   </fo:inline>
 </fo:block>
</xsl:if>
</xsl:template>

<!-- mots cle -->
<xsl:template name="keywords-list">
   <xsl:if test=".//keyword">
      <fo:block space-after.optimum="4pt" space-before.optimum="4pt">
        <fo:inline>
           <xsl:attribute name="font-weight">bold</xsl:attribute>
          Keywords:
        </fo:inline>
        <xsl:apply-templates select="(.//keyword)" mode="section" >
           <xsl:sort />
        </xsl:apply-templates>
      </fo:block>
   </xsl:if>
</xsl:template>

<xsl:template name="keywords-list2">
   <xsl:if test="./keyword">
      <fo:block space-after.optimum="4pt">
        <fo:inline>
           <xsl:attribute name="font-weight">bold</xsl:attribute>
           Keywords:
        </fo:inline>
        <xsl:apply-templates select="(./keyword)" mode="section" >
           <xsl:sort />
        </xsl:apply-templates>
      </fo:block>
   </xsl:if>
</xsl:template>

<xsl:template match="keyword" mode="section">
    <fo:inline font-style="italic">
      <xsl:apply-templates/>
    </fo:inline>
    <xsl:call-template name="separateur.objet"/>
</xsl:template>

<xsl:template match="keyword"/>



<xsl:template match="simplemath">
  <m:math><m:mi><xsl:apply-templates/></m:mi></m:math>
</xsl:template>


<xsl:template name="calculateNumber">
   <xsl:param name="numbersuffix"/>
       <xsl:call-template name="calculateNumberSection"/>
       <xsl:number level="multiple" from="raweb" count="subsection"/>
       <xsl:value-of select="$numbersuffix"/>
</xsl:template>

<xsl:template name="calculateNumberSection">
      <xsl:choose>
<!--<xsl:when test="ancestor-or-self::*[self::identification]">1</xsl:when>
<xsl:when test="ancestor-or-self::*[self::presentation]">2</xsl:when>
<xsl:when test="ancestor-or-self::*[self::fondements]">3</xsl:when>
<xsl:when test="ancestor-or-self::*[self::domaine]">4</xsl:when>
<xsl:when test="ancestor-or-self::*[self::logiciels]">5</xsl:when>
<xsl:when test="ancestor-or-self::*[self::resultats]">6</xsl:when>
<xsl:when test="ancestor-or-self::*[self::contrats]">7</xsl:when>
<xsl:when test="ancestor-or-self::*[self::international]">8</xsl:when>
<xsl:when test="ancestor-or-self::*[self::diffusion]">9</xsl:when>
<xsl:when test="ancestor-or-self::*[self::biblio]">10</xsl:when>-->

<!-- MPD 27/12/2004-->
<xsl:when test="ancestor-or-self::*[self::identification]">1</xsl:when>
<xsl:when test="ancestor-or-self::*[self::presentation]">
<xsl:value-of select="count(/raweb/identification)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::fondements]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::domaine]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::logiciels]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::resultats]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+count(/raweb/logiciels)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::contrats]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+count(/raweb/logiciels)+count(/raweb/resultats)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::international]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+count(/raweb/logiciels)+count(/raweb/resultats)+count(/raweb/contrats)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::diffusion]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+count(/raweb/logiciels)+count(/raweb/resultats)+count(/raweb/contrats)+count(/raweb/international)+1"/>
</xsl:when>
<xsl:when test="ancestor-or-self::*[self::biblio]">
<xsl:value-of select="count(/raweb/identification)+count(/raweb/presentation)+count(/raweb/fondements)+count(/raweb/domaine)+count(/raweb/logiciels)+count(/raweb/resultats)+count(/raweb/contrats)+count(/raweb/international)+count(/raweb/diffusion)+1"/>
</xsl:when>
<xsl:otherwise>Section</xsl:otherwise>
</xsl:choose>
<xsl:text>.</xsl:text>
</xsl:template>

<xsl:template name="calculateFootnoteNumber">
    <xsl:number  from="raweb" level="any" count="footnote[@place='foot']"/>
</xsl:template>

<xsl:template match='li' mode="xref">
    <xsl:number count="li" level="single" />
</xsl:template>

<xsl:template match='li' mode="pre-ref"></xsl:template>

<xsl:template match='li' mode="post-ref"></xsl:template>

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

<xsl:template match="TeX">
   <fo:inline>TeX</fo:inline>
</xsl:template>
<xsl:template match="LaTeX">
   <fo:inline>LaTeX</fo:inline>
</xsl:template>

<xsl:template match="allowbreak">
   <allowbreak/>
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
