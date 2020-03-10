<!-- 
RCS: $Date: 2004/09/14 15:50:47 $, $Revision: 2.1 $, $Author: grimm $

XSL FO stylesheet to format TEI XML documents 

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
  xmlns:exsl="http://exslt.org/common"
  exclude-result-prefixes="exsl"
  extension-element-prefixes="exsl"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"  >



<xsl:template match="cell">
  <fo:table-cell>
    <xsl:if test="@cols &gt; 1">
      <xsl:attribute name="number-columns-spanned">
        <xsl:value-of select="@cols"/>
      </xsl:attribute>
    </xsl:if>
    <xsl:if test="@rows &gt; 1">
      <xsl:attribute name="number-rows-spanned">
        <xsl:value-of select="@rows"/>
      </xsl:attribute>
    </xsl:if>
   <xsl:call-template name="cellProperties"/>
   <fo:block>
    <xsl:choose>
      <xsl:when test="@role='label' or parent::row[@role='label']">
        <xsl:attribute name="font-weight">bold</xsl:attribute>
      </xsl:when>
    </xsl:choose>
    <xsl:apply-templates/>
   </fo:block>
  </fo:table-cell>
</xsl:template>

<xsl:template name="cellProperties" >
  <xsl:if test="@role='hi'">
    <xsl:attribute name="background-color">silver</xsl:attribute>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="ancestor::table[1][@rend='frame']">
      <xsl:if test="not(parent::row/preceding-sibling::row)">
        <xsl:attribute name="border-before-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:attribute name="border-after-style">solid</xsl:attribute>
      <xsl:if test="not(following-sibling::cell)">
        <xsl:attribute name="border-end-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:attribute name="border-start-style">solid</xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:if test="@left-border='true'">
        <xsl:attribute name="border-start-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="@right-border='true'">
        <xsl:attribute name="border-end-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="ancestor::row/@top-border='true'">
        <xsl:attribute name="border-before-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="ancestor::row/@bottom-border='true'">
        <xsl:attribute name="border-after-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="ancestor::row/@bottom-border='true'">
        <xsl:attribute name="border-after-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="@bottom-border='true'">
        <xsl:attribute name="border-after-style">solid</xsl:attribute>
      </xsl:if>
      <xsl:if test="@top-border='true'">
        <xsl:attribute name="border-before-style">solid</xsl:attribute>
      </xsl:if>
    </xsl:otherwise>
  </xsl:choose>
  <xsl:if test="not(ancestor::table/@rend='tight')">   
    <xsl:attribute name="padding">
       <xsl:value-of select="$tableCellPadding"/>
    </xsl:attribute>
  </xsl:if>
  <xsl:choose>
    <xsl:when test="@halign">
      <xsl:attribute name="text-align">
         <xsl:value-of select="@halign"/>
      </xsl:attribute>
    </xsl:when>
    <xsl:otherwise>
      <xsl:variable name="thiscol">
        <xsl:value-of select="position()"/>
      </xsl:variable>
      <xsl:variable name="tid"><xsl:value-of select="ancestor::table/@id"/></xsl:variable>
      <xsl:variable name="align">
        <xsl:value-of select="exsl:node-set($tableSpecs)/Info/TableSpec[@id=$tid]/fo:table-column[@column-number=$thiscol]/@fotex:column-align"/>
      </xsl:variable>
      <xsl:choose>
          <xsl:when test="$align='R'">
                <xsl:attribute name="text-align">right</xsl:attribute>
          </xsl:when>
          <xsl:when test="$align='L'">
                <xsl:attribute name="text-align">left</xsl:attribute>
          </xsl:when>
          <xsl:when test="$align='C'">
                <xsl:attribute name="text-align">center</xsl:attribute>
          </xsl:when>
          <xsl:otherwise>
            <xsl:if test="not($align='')">
	       <xsl:attribute name="text-align">
                   <xsl:value-of select="$align"/>
               </xsl:attribute>
	    </xsl:if>
          </xsl:otherwise>
      </xsl:choose>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>


</xsl:stylesheet>
