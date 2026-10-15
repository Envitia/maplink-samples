#include "StdAfx.h"
#include "MyCustomClassifier.h"

MyCustomClassifier::MyCustomClassifier() : m_layerForRendition( NULL )
{
  // The application can apply any complex rules or styling using this mechanism.
  // As an example, this class loads a pre-defined rendition list for
  // OpenStreetMap and applies some logic to the classification for lines by
  // accessing multiple attributes of the data

  // MapLink Home may or may not have a trailing backslash, since it
  // may be passed on the command line.  Deal with either case
  TSLSimpleString path(TSLUtilityFunctions::getMapLinkHome());
  if ( path.length() != 0 && path.c_str()[ path.length()-1 ] == '\\' )
    path += "config\\directimport\\OpenStreetMapLines.rnd";
  else
    path += "\\config\\directimport\\OpenStreetMapLines.rnd";

  if (!TSLFileHelper::fileExists(path))
    return;

  // Load the detailed rendition list
  m_layerForRendition = new TSLStandardDataLayer();
  if (!m_layerForRendition->loadRendering(path))
  {
    m_layerForRendition->destroy();
    m_layerForRendition = NULL;
  }
}

MyCustomClassifier::~MyCustomClassifier()
{
  if ( m_layerForRendition)
    m_layerForRendition->destroy();
}

//! Informs handler that a TSLFeatureClassifierCustom is attached to it.
//!
void MyCustomClassifier::attach(TSLFeatureClassifierCustom * classifier)
{
  // Could use this for reference counting, but in this example, we are owned by the document
}

//! Informs handler that a TSLFeatureClassifierCustom no longer needs it.
//!
void MyCustomClassifier::release(TSLFeatureClassifierCustom * classifier)
{
  // Could use this for reference counting, but in this example, we are owned by the document
}

//! Process the classification definition and add child features to the hierarchy.
//!
//! @param rootFeature The top-level feature (TSLFeature) in the hierarchy the sub-classing should be applied to.
//! @param classificationLevel The level of this classification, starting at 0.
//! @param caseSensitive Whether the identification of existing classifications should be case sensitive or not.
//!
//! @return `true` if any new features were created, `false` otherwise.
bool MyCustomClassifier::getClassifiedFeatures(TSLFeature* rootFeature, unsigned int classificationLevel, bool caseSensitive)
{
  // Pre-create any features we loaded from the rendition file
  if (!m_layerForRendition)
    return false;

  const TSLFeatureClassList * fcl = m_layerForRendition->featureList();
  if (!fcl || fcl->size() == 0)
    return false;

  TSLRenderingAttributes attributes;

  for (int i = 0; i < fcl->size(); ++i)
  {
    TSLFeatureID id;
    const char * name = fcl->getDetails(i, &id);
    if (name)
    {
      m_layerForRendition->getFeatureRendering(0, id, &attributes);
      rootFeature->addChild( new TSLFeature(name, attributes) );
    }
  }
  return true;
}

//! Query the feature name for a given geometry
//!
//! @param geometry The geometry to classify
//! @param renderingAttribs The given TSLRenderingAttributes object will be set to the rendering attributes (if any) of the feature. 
//!                         Not all classifier definitions can specify rendering attributes.
//!
//! @return The child feature name for the geometry, or null if none could be determined.
bool MyCustomClassifier::classifyFeature(const TSLMUGeometry * geometry, TSLRenderingAttributes& renderingAttribs, TSLSimpleString & childFeatureName)
{
  if (!m_layerForRendition)
    return false;

  const char * highway  = geometry->getAttribute("highway");
  const char * waterway = geometry->getAttribute("waterway");
  const char * aerialway = geometry->getAttribute("aerialway");
  const char * barrier = geometry->getAttribute("barrier");
  const char * railway = geometry->getAttribute("railway");

  TSLSimpleString extendedValue;
  TSLSimpleString baseFeature;

  // Example classification of "lines" from open street map
  // In this sample the classifier is only used for "lines"
  // so there's no need to check geometry->featureName();
  if (highway && *highway )
  {
    baseFeature = "highway";
    extendedValue = highway;
  }
  else if (waterway && *waterway)
  {
    baseFeature = "waterway";
    extendedValue = waterway;
  }
  else if (aerialway && *aerialway)
  {
    baseFeature = "aerialway";
    extendedValue = aerialway;
  }
  else if (barrier && *barrier)
  {
    baseFeature = "barrier";
    extendedValue = barrier;
  }
  else if (railway && *railway)
  {
    baseFeature = "railway";
    extendedValue = railway;
  }

  // If it's not something we recognise  
  if (baseFeature.length() == 0)
    return false;

  // Now look up to see if we have any detailed rendering for the full class
  TSLSimpleString extendedName(baseFeature);
  extendedName += ".";
  extendedName += extendedValue;

  int id = m_layerForRendition->featureList()->getID(extendedName.c_str());
  if (m_layerForRendition->getFeatureRendering(0, id, &renderingAttribs))
  {
    childFeatureName = extendedName;
    return true;
  }

  // Nothing for the detailed, see if we have anything for the base class
  id = m_layerForRendition->featureList()->getID(baseFeature.c_str());
  childFeatureName = baseFeature;
  return m_layerForRendition->getFeatureRendering(0, id, &renderingAttribs);
}