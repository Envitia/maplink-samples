using Envitia.MapLink;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace CSharpDirectImportSample
{
  class CustomClassifier : TSLNClientFeatureClassifierCustom
  {
    private TSLNStandardDataLayer m_layerForRendition = null;

    public CustomClassifier()
    {
      string path = TSLNUtilityFunctions.getMapLinkHome();
      if (path.Length != 0 && path.Substring(path.Length - 1) == "\\")
        path += "config\\directimport\\OpenStreetMapLines.rnd";
      else
        path += "\\config\\directimport\\OpenStreetMapLines.rnd";

      if (!File.Exists(path))
      {
        MessageBox.Show(path + " Does not exist");
        return;
      }
      
      // Load the detailed rendition list
      m_layerForRendition = new TSLNStandardDataLayer();
      if (!m_layerForRendition.loadRendering(path))
      {
        m_layerForRendition = null;
      }
    }


    public bool Initialised
    {
      get { return m_layerForRendition != null; }
    }

    public override void attach(TSLNFeatureClassifierCustom classifier)
    {
      //! Informs handler that a TSLNFeatureClassifierCustom is attached to it.
      //! Could use this for reference counting, but in this example the classifier is owned by the application
    }

    public override void release(TSLNFeatureClassifierCustom classifier)
    {
      //! Informs handler that a TSLNFeatureClassifierCustom has been detached
      //! Could use this for reference counting, but in this example the classifier is owned by the application
    }

    //! Process the classification definition and add child features to the hierarchy.
    //!
    //! @param rootFeature The top-level feature (TSLNFeature) in the hierarchy the sub-classing should be applied to.
    //! @param classificationLevel The level of this classification, starting at 0.
    //! @param caseSensitive Whether the identification of existing classifications should be case sensitive or not.
    //!
    //! @return true if any new features were created, false otherwise.
    public override bool getClassifiedFeatures(TSLNFeature rootFeature, uint classificationLevel, bool caseSensitive = false)
    {
      if (m_layerForRendition == null)
        return false;

      TSLNFeatureClassList fcl = m_layerForRendition.featureList;
      if (fcl == null || fcl.size == 0)
        return false;

      TSLNRenderingAttributes attributes = new TSLNRenderingAttributes();

      for (int i = 0; i < fcl.size; ++i)
      {
        int id;
        string name = fcl.getDetails(i, out id);
        if( string.IsNullOrEmpty(name) )
        {
          continue;
        }
        m_layerForRendition.getFeatureRendering(name, id, ref attributes);
        rootFeature.addChild( new TSLNFeature(name, attributes) );
      }
      return true;
    }

    //! Query the feature name for a given geometry
    //!
    //! @param geometry The geometry to classify
    //! @param renderingAttribs The given TSLNRenderingAttributes object will be set to the rendering attributes (if any) of the feature. 
    //!                         Not all classifier definitions can specify rendering attributes.
    //!
    //! @return The child feature name for the geometry, or null if none could be determined.
    public override bool classifyFeature(TSLNMUGeometry geometry, TSLNRenderingAttributes renderingAttribs, ref string childFeatureName)
    {
      if (m_layerForRendition == null)
        return false;
      
      // Example classification of "lines" from open street map
      // In this sample the classifier is only used for "lines"
      // so there's no need to check geometry->featureName();
      string highway = geometry.getAttribute("highway");
      string waterway = geometry.getAttribute("waterway");
      string aerialway = geometry.getAttribute("aerialway");
      string barrier = geometry.getAttribute("barrier");
      string railway = geometry.getAttribute("railway");

      string extendedValue = string.Empty;
      string baseFeature = string.Empty;

      if (!string.IsNullOrEmpty(highway))
      {
        baseFeature = "highway";
        extendedValue = highway;
      }
      else if (!string.IsNullOrEmpty(waterway))
      {
        baseFeature = "waterway";
        extendedValue = waterway;
      }
      else if (!string.IsNullOrEmpty(aerialway))
      {
        baseFeature = "aerialway";
        extendedValue = aerialway;
      }
      else if (!string.IsNullOrEmpty(barrier))
      {
        baseFeature = "barrier";
        extendedValue = barrier;
      }
      else if (!string.IsNullOrEmpty(railway))
      {
        baseFeature = "railway";
        extendedValue = railway;
      }

      // If it's not something we recognise  
      if (baseFeature.Length == 0)
        return false;

      // Now look up to see if we have any detailed rendering for the full class
      string extendedName = baseFeature + "." + extendedValue;
      
      int id = m_layerForRendition.featureList.getID(extendedName);
      if (m_layerForRendition.getFeatureRendering(extendedName, id, ref renderingAttribs))
      {
        childFeatureName = extendedName;
        return true;
      }

      // Nothing for the detailed, see if we have anything for the base class
      id = m_layerForRendition.featureList.getID(baseFeature);
      childFeatureName = baseFeature;
      return m_layerForRendition.getFeatureRendering(baseFeature, id, ref renderingAttribs);
    }
  }
}
