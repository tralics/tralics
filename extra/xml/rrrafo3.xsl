<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format raweb XML documents 
This is the part that is common with rawebfo3.xsl rrfo3.xsl and simplefo3.xsl
It translates the rend attribute of <hi>, as well as some font commands.

Copyright 2004 José Grimm/ Inria apics
Original name changed from rrrafo.xsl to rrrafo3.xsl

$Id: rrrafo3.xsl,v 2.3 2007/01/09 08:39:59 grimm Exp $
-->

<xsl:stylesheet
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML" 
  xmlns:fo="http://www.w3.org/1999/XSL/Format">


<!-- This interprets the rend attribute accordind to the TEI-->
<!-- copied from the old file -->
<xsl:template name="rend">
  <xsl:choose>
    <xsl:when test="@rend='overline'">
      <xsl:attribute name="text-decoration">overline</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='underline'">
      <xsl:attribute name="text-decoration">underline</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='oldstyle'">
      <xsl:attribute name="font-family">Concrete</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='sub'">
      <xsl:attribute name="vertical-align">sub</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='sup'">
      <xsl:attribute name="vertical-align">super</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='it'">
      <xsl:attribute name="font-style">italic</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='slanted'">
      <xsl:attribute name="font-style">italic</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='sc'">
      <xsl:attribute name="font-variant">small-caps</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='tt'">
      <xsl:attribute name="font-family">Computer-Modern-Typewriter</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='sansserif'">
      <xsl:attribute name="font-family">sansserif</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='bold'">
      <xsl:attribute name="font-weight">bold</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='ul'">
      <xsl:attribute name="text-decoration">ul</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='caps'">
      <xsl:attribute name="text-decoration">caps</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='hl'">
      <xsl:attribute name="text-decoration">hl</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='so'">
      <xsl:attribute name="text-decoration">so</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='st'">
      <xsl:attribute name="text-decoration">st</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='small'">
      <xsl:attribute name="font-size">8pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='small1'">
      <xsl:attribute name="font-size">9pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='small2'">
      <xsl:attribute name="font-size">8pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='small3'">
      <xsl:attribute name="font-size">7pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='small4'">
      <xsl:attribute name="font-size">5pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large'">
      <xsl:attribute name="font-size">12pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large1'">
      <xsl:attribute name="font-size">12pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large2'">
      <xsl:attribute name="font-size">14pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large3'">
      <xsl:attribute name="font-size">17pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large4'">
      <xsl:attribute name="font-size">20pt</xsl:attribute>
    </xsl:when>
    <xsl:when test="@rend='large5'">
      <xsl:attribute name="font-size">25pt</xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:attribute name="font-weight">bold</xsl:attribute>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<!-- Replacement code for the `rend' above
  We start with two functions small and large 
  In all cases, wee provide a long and a short name
-->

<xsl:template match='small|font-small'>
  <fo:inline font-size='8pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large|font-large'>
  <fo:inline font-size='12pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- We continue with the ten font size commands of LaTeX -->

<xsl:template match='small4|font-small4'>
  <fo:inline font-size='5pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='small3|font-small3'>
  <fo:inline font-size='7pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='small2|font_small2'>
  <fo:inline font-size='8pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='small1|font-small1'>
  <fo:inline font-size='9pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


<xsl:template match='normalsize|font-normalsize'>
  <fo:inline font-size='10pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large1|font-large1'>
  <fo:inline font-size='12pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large2|font-large2'>
  <fo:inline font-size='14.4pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large3|font-large3'>
  <fo:inline font-size='17.28pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large4|font-large4'>
  <fo:inline font-size='20.74pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='large5|font-large5'>
  <fo:inline font-size='24.88pt'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- Now the four shapes -->

<xsl:template match='it|font-italic-shape'>
  <fo:inline font-style='italic'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='slanted|font-slanted-shape'>
  <fo:inline font-style='oblique'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='sc|font-small-caps-shape'>
  <fo:inline font-variant='small-caps'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='upright|font-upright-shape'>
  <fo:inline font-variant='normal'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- Now the three families -->
<!-- We provide two different tt families -->
<xsl:template match='roman|font-roman-family'>
  <fo:inline font-variant='normal'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='sansserif|font-sansserif-family'>
  <fo:inline font-family='sansserif'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='computer-modern-tt'>
  <fo:inline font-family='Computer-Modern-Typewriter'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='tt|font-typewriter-family'>
  <fo:inline font-family='monospace'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='font-ul'>
  <fo:inline text-decoration='ul'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>
<xsl:template match='font-caps'>
  <fo:inline text-decoration='caps'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>
<xsl:template match='font-hl'>
  <fo:inline text-decoration='hl'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>
<xsl:template match='font-so'>
  <fo:inline text-decoration='so'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>
<xsl:template match='font-st'>
  <fo:inline text-decoration='st'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- Extensions of the soul package -->

<!-- Now the two series -->

<xsl:template match='medium|font-medium-series'>
  <fo:inline font-weight='medium'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='bold|font-bold-series'>
  <fo:inline font-weight='bold'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- Superscript, subscript in text fonts -->
<xsl:template match='sup|font-super'>
  <fo:inline vertical-align='super'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='sub|font-sub'>
  <fo:inline vertical-align='sub'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<!-- Overline underline in text fonts -->
<xsl:template match='underline|font-underline'>
  <fo:inline text-decoration='underline'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>

<xsl:template match='overline|font-overline'>
  <fo:inline text-decoration='overline'>
    <xsl:apply-templates/>
  </fo:inline>
</xsl:template>


<!-- Only foot notes are supported -->

<xsl:template match="note">
    <!-- no attribute n -->
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

</xsl:stylesheet>
