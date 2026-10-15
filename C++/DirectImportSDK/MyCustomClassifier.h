#pragma once

#include "featureconfiguration\tslclientfeatureclassifiercustom.h"

//! 
//! This is a basic custom classifier that works with OpenStreetMap lines feature
//! by loading a pre-defined styling and the applying the relevant classification
//! logic to select the appropriate styling
//!
class MyCustomClassifier :
  public TSLClientFeatureClassifierCustom
{
public:
  MyCustomClassifier();
  virtual ~MyCustomClassifier();

  // Validity checking
  bool initialised() const
  {
    return (m_layerForRendition != NULL);
  }

  // Ownership and lifetime management
  virtual void attach(TSLFeatureClassifierCustom * classifier) ;
  virtual void release(TSLFeatureClassifierCustom * classifier) ;

  // Preprocess - loading predefined features
  virtual bool getClassifiedFeatures(TSLFeature* rootFeature, unsigned int classificationLevel, bool caseSensitive = false) ;

  // Classify a feature by looking at it's attributs and returning the appropriate rendering attributes and feature name
  virtual bool classifyFeature(const TSLMUGeometry * geometry, TSLRenderingAttributes& renderingAttribs, TSLSimpleString & childFeatureName ) ;

private:
  //! Storage for a standard data layer in which to load the pre-defined rendering
  TSLStandardDataLayer * m_layerForRendition;
};

