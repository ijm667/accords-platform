<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet 
    version="1.0"
    xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform"     
    xmlns:exsl="http://exslt.org/common" 
    xmlns:occi="http://schemas.ogf.org/occi" 
    extension-element-prefixes="exsl" >

  <xsl:output method="xml" 
	      encoding="utf-8" 
	      indent="yes" />

  <xsl:template match="@scheme|@term|@name|@version|@rel" >
    <xsl:attribute name="{name()}" ><xsl:value-of select="." /></xsl:attribute>
  </xsl:template>

  <xsl:template match="@location|@structname" >
    <occi:attribute>
      <xsl:attribute name="name" ><xsl:value-of select="name()" /></xsl:attribute>
      <xsl:attribute name="value" ><xsl:value-of select="." /></xsl:attribute>
    </occi:attribute>
  </xsl:template>

  <xsl:template match="@type" >
    <xsl:attribute name="type" >
      <xsl:choose>
	<xsl:when test=". = 'string'" >xsd:string</xsl:when>
	<xsl:when test=". = 'int'" >xsd:string</xsl:when>
      </xsl:choose>
    </xsl:attribute>
  </xsl:template>

  <xsl:template match="@required" >
    <xsl:if test=". = 'true'" >
      <xsl:attribute name="use" >required</xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template name="relAttr" >
    <xsl:param name="relations" />
    <xsl:attribute name="rel" ><xsl:value-of select="$relations" /></xsl:attribute>
  </xsl:template>

  <xsl:template name="relElem" >
    <xsl:param name="relations" />
    <occi:relations>
      <xsl:for-each select="$relations" >
	<occi:relation>
	  <xsl:attribute name="id" ><xsl:value-of select="." /></xsl:attribute>
	</occi:relation>
      </xsl:for-each>
    </occi:relations>
  </xsl:template>

  <xsl:template match="attribute" >
    <occi:attribute>
      <xsl:attribute name="name" ><xsl:value-of select="@name" /></xsl:attribute>
      <xsl:attribute name="immutable" ><xsl:value-of select="@immutable" /></xsl:attribute>
      <xsl:apply-templates select="@required" />
      <xsl:apply-templates select="@type" />
    </occi:attribute>
  </xsl:template>
 
  <xsl:template match="action" >
    <occi:action>
      <xsl:attribute name="id" ><xsl:value-of select="@id" /></xsl:attribute>
    </occi:action>
  </xsl:template>

  <xsl:template match="category" >
    <occi:category>
      <xsl:attribute name="class" ><xsl:value-of select="./@class" /></xsl:attribute>

      <xsl:apply-templates select="@term" />
      <xsl:apply-templates select="@scheme" />
      
      <xsl:choose>
	<xsl:when test="./@class = 'kind'" >
	  <xsl:call-template name="relAttr"><xsl:with-param name="relations" select="@rel" /></xsl:call-template>
	</xsl:when>
	<xsl:when test="./@class = 'mixin'" >
	  <xsl:call-template name="relElem" ><xsl:with-param name="relations" select="@rel" /></xsl:call-template>
	</xsl:when>
      </xsl:choose>

      <xsl:if test="@location or @structname" >
	<occi:annotation>
	  <xsl:apply-templates select="@location|@structname" />
	</occi:annotation>
      </xsl:if>

      <xsl:if test="./attributes" >
	<occi:attributes>
	  <xsl:apply-templates select="./attributes/attribute" />
	</occi:attributes>
      </xsl:if>

      <xsl:if test="./actions" >
	<occi:actions>
	  <xsl:apply-templates select="./actions/action" />
	</occi:actions>
      </xsl:if>
    </occi:category>
  </xsl:template>

  <xsl:template match="model" >
    <occi:extension>
      <xsl:apply-templates select="@name" />
      <xsl:apply-templates select="@scheme" />
      <xsl:apply-templates select="@version" />
      <xsl:if test="./category[@class='kind']" >
	<xsl:apply-templates select="./category[@class='kind']" />
      </xsl:if>
      <xsl:if test="./category[@class='mixin']" >
	<xsl:apply-templates select="./category[@class='mixin']" />
      </xsl:if>
   </occi:extension>
  </xsl:template>

  <xsl:template match="schema" >
    <occi:schema xmlns:occi="http://schemas.ogf.org/occi" 
		 xmlns:xsd="http://www.w3.org/2001/XMLSchema" 
		 version="0.2" >
      <xsl:apply-templates select="./model" />
    </occi:schema>
  </xsl:template>

  <xsl:template match="/" >
    <xsl:apply-templates select="//schema" />
  </xsl:template>
</xsl:stylesheet>
