<?xml version="1.0" encoding="iso-8859-1" ?>

<!--
$Id: preview.xsl,v 2.2 2004/10/05 17:05:17 grimm Exp $
  Copyright Inria 2003/ José Grimm
  This file is distributed with Tralics. See the Copyright notice
  in the Tralics main directory

SGML_CATALOG_FILES=./catalog
xsltproc -catalogs -o to.xml preview.xsl torture.xml
xsltproc -catalogs -o torture.fo  rrfo.xsl  to.xml 

 -->

<xsl:stylesheet
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML" 
  xmlns:fo="http://www.w3.org/1999/XSL/Format">

<xsl:template match="ramain">
 <rr>
  <RRstart type='RR'>
    <title>non</title>
    <etitle>non</etitle>
    <resume>non</resume>
    <abstract>non</abstract>
    <motcle>non</motcle>
    <keyword>non</keyword>
    <theme>2</theme>
    <RRnumber>123</RRnumber>
    <UR><URSophia/></UR>
  </RRstart>
   <div0><head>bidon</head><p>
  <xsl:for-each select = "//preview">
    <preview>
      <xsl:apply-templates mode="sel"/>
    </preview>
  </xsl:for-each>
   </p></div0>
 </rr>
</xsl:template>

<xsl:template match="*|@*|comment()|processing-instruction()|text()" mode="sel">
 <xsl:copy>
   <xsl:apply-templates mode="sel" select="*|@*|processing-instruction()|text()"/>
 </xsl:copy>
</xsl:template>

</xsl:stylesheet>
