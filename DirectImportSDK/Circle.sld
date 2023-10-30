<?xml version="1.0" encoding="UTF-8" ?>
<sld:StyledLayerDescriptor xmlns="http://www.opengis.net/sld" xmlns:sld="http://www.opengis.net/sld" xmlns:gml="http://www.opengis.net/gml" xmlns:ogc="http://www.opengis.net/ogc" version="1.0.0">
   <sld:UserLayer>
      <sld:Name>Circle</sld:Name>
      <sld:UserStyle>
         <sld:Name>Default Styler</sld:Name>
         <sld:FeatureTypeStyle>
            <sld:Name>name</sld:Name>
            <sld:Rule>
               <ogc:Filter>
                  <ogc:FeatureId fid="GS_00003_Circle_No_Fill_1_3.1" />
               </ogc:Filter>
               <sld:LineSymbolizer>
                  <sld:Stroke>
                     <sld:CssParameter name="stroke">#FF0000</sld:CssParameter>
                     <sld:CssParameter name="stroke-width">5.0</sld:CssParameter>
                  </sld:Stroke>
               </sld:LineSymbolizer>
            </sld:Rule>
         </sld:FeatureTypeStyle>
      </sld:UserStyle>
   </sld:UserLayer>
</sld:StyledLayerDescriptor>