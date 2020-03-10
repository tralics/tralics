<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:exsl="http://exslt.org/common"
  exclude-result-prefixes="saxon exsl" 
  extension-element-prefixes="saxon exsl fotex"
  xmlns:saxon="http://icl.com/saxon"
  xmlns:fo="http://www.w3.org/1999/XSL/Format"
                version="1.0">


<xsl:template name="calculateTableSpecs">
  <xsl:variable name="tds">
     <xsl:for-each select=".//cell">
        <xsl:variable name="stuff">
           <xsl:apply-templates/>
        </xsl:variable>
        <cell>
            <xsl:attribute name="col"><xsl:number/></xsl:attribute>
            <xsl:value-of select="string-length($stuff) + 100"/>
        </cell>
     </xsl:for-each>
  </xsl:variable>
  <xsl:variable name="total">
      <xsl:value-of select="sum(exsl:node-set($tds)/cell)"/>
  </xsl:variable>
  <xsl:for-each select="exsl:node-set($tds)/cell">
     <xsl:sort select="@col" data-type="number"/>
     <xsl:variable name="c" select="@col"/>
     <xsl:if test="not(preceding-sibling::cell[$c=@col])">
       <xsl:variable name="len">
          <xsl:value-of select="sum(following-sibling::cell[$c=@col]) + current()"/>
       </xsl:variable>
       <xsl:text>
       </xsl:text>
       <fo:table-column column-number="{@col}" 
           fotex:column-align="L" column-width="{$len div $total * 100}%" />
     </xsl:if>
  </xsl:for-each>
  <xsl:text>
  </xsl:text>
</xsl:template>


</xsl:stylesheet>
