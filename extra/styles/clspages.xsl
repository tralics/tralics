<?xml version="1.0" encoding="iso-8859-1" ?>
<!-- 

XSL FO stylesheet to format standard classes XML documents 

$Id: clspages.xsl,v 1.2 2007/04/25 17:51:01 grimm Exp $
-->

<xsl:stylesheet
  xmlns:fotex="http://www.tug.org/fotex"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
  xmlns:m="http://www.w3.org/1998/Math/MathML" 
  xmlns:fo="http://www.w3.org/1999/XSL/Format">



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


<xsl:template name="myheaders">
  <fo:static-content flow-name="xsl-region-before-right">
    <fo:block  text-align="justify" font-size="{$bodySize}"
         font-family="{$bodyFont}">
       <fo:block border-after-style="solid" text-indent="0pt">
           <fo:inline font-style="italic">
             <xsl:value-of select="$Lefthead"/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline>
               <fo:page-number/>
           </fo:inline>
        </fo:block>
    </fo:block>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-before-left">
     <fo:block text-align="justify" font-size="{$bodySize}"
	       font-family="{$bodyFont}">
        <fo:block border-after-style="solid" text-indent="0pt">
           <fo:inline>
             <fo:page-number/>
           </fo:inline>
           <fo:leader rule-thickness="0pt"/>
           <fo:inline font-style="italic">
	     <xsl:value-of select="$Righthead"/>
	   </fo:inline>
        </fo:block>
     </fo:block>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-before-first"/>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-right">
    <xsl:call-template name="odd-footer"/>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-left">
    <xsl:call-template name="even-footer"/>
  </fo:static-content>
  <xsl:text>&#xA;</xsl:text>
  <fo:static-content flow-name="xsl-region-after-first">
    <xsl:call-template name="first-footer"/>
  </fo:static-content>
</xsl:template>




<!-- page sequence for the main text -->
<xsl:template name="maintext">
   <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="{$maintextInitialPageNumber}"
        master-reference="{$maintextMasterReference}"
        force-page-count="{$maintextForcePagecount}"
      >
      <fo:flow  
         flow-name="xsl-region-body"
         font-family="{$bodyFont}"  
         font-size="{$bodySize}">
        <xsl:call-template name="maintext.body"/>
      </fo:flow>
   </fo:page-sequence>
</xsl:template>


<!-- page sequence for the titlepage -->
<xsl:template name="titlepage">
 <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="1"
        master-reference="twoside1"
        force-page-count="end-on-even" >
     <xsl:text>&#xA;</xsl:text>
     <fo:flow font-style="italic" font-family="Times Roman" >
      <xsl:call-template name="titlepage.body"/>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>

<!-- page sequence for the TOC --> 

<xsl:template name="toc">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="{$tocInitialPageNumber}"
        master-reference="{$tocMasterReference}"
        force-page-count="{$tocForcePagecount}"
      > 
   <fo:flow flow-name="xsl-region-body" font-family="{$bodyFont}" >
    <fo:block>
      <xsl:call-template name="toc.body"/>
    </fo:block>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>


<xsl:template name="lastpage">
  <fo:page-sequence
        format="1" 
        text-align="justify"
        hyphenate="true"
        language="english"
        initial-page-number="auto"
        master-reference="blank1"
        force-page-count="auto"
      > 
   <fo:flow flow-name="xsl-region-body" font-family="Times Roman" >
    <fo:block text-align="center">
      <xsl:call-template name="lastpage.body"/>
    </fo:block>
   </fo:flow>
  </fo:page-sequence>
</xsl:template>


</xsl:stylesheet>

