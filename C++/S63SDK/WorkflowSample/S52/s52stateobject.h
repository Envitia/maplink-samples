#ifndef s52stateobject_h
#define s52stateobject_h 1
//  ********************************************************
//  *  Copyright (c) 1998 to 2017 by Envitia Group PLC
//  ********************************************************

#include "S52/tsls52displaytextenum.h"
#include "S52/tsls52stateobject.h"

class S52StateObject : public TSLS52StateObject
{
  public:

    S52StateObject ();
     
    virtual ~S52StateObject();

    virtual bool CENTRE_SYMBOLS () const;

    virtual bool CONTOUR_LABELS () const;
    
    virtual TSLTimeType CURRENT_DATE () const;

    virtual int CURRENT_SCALE () const;

    virtual double DEEP_CONTOUR () const;
    
    virtual bool DISPLAY_AT_ALL_DATES () const;

    virtual bool DISPLAY_AT_ALL_SCALES () const;

    virtual int DISPLAY_CATEGORY_MASK () const;

    virtual int DISTANCE_TAGS () const;

    virtual bool FULL_SECTORS () const;

    virtual bool LIGHT_DESCRIPTIONS () const;

    virtual bool LOW_ACCURACY_SYMBOLS () const;
    
	virtual TSLCoord OWNSHIP () const;

    virtual bool RADAR_ON () const;

    virtual double SAFETY_CONTOUR () const;

    virtual double SAFETY_DEPTH () const;

    virtual double SHALLOW_CONTOUR () const;

    virtual bool SHALLOW_PATTERN () const;

    virtual bool SHIPS_OUTLINE () const;
    
    virtual bool SHOW_TEXT () const;

    virtual bool SHOW_TEXT_HALO() const;

    virtual bool SHOW_SHALLOW_DANGERS () const;
    
    virtual bool SHOW_SYMBOLS () const;
    
    //	This method determines whether or not paper chart or simple S52 symbols are
    //	displayed.
    //
    //	Returning true will display simple S52 symbols.
    virtual bool SIMPLE_SYMBOLS () const;

    //	This method determines whether or not paper chart or simple S52  area
    //	boundaries are displayed.
    //
    //	Returning true will display simple S52 area boundaries.
    virtual bool SIMPLE_AREAS () const;

    virtual bool SYMBOL_DISPLAY () const;

    virtual double TIME_TAGS () const;

    virtual bool TWO_SHADES () const;

    virtual TSLS52DisplayTextEnum TEXT_DISPLAY () const;

    //	This method should be used to determine whether or not the supplied dataset
    //	name and producing agency can be considered to be valid hydrographic office
    //	data.
    //
    //	Returning true confirms the dataset and agency can be considered to be valid
    //	HO data.
    virtual bool VALIDATE_HO_DATA (const char* datasetName, const char* datasetAgency) const;

    bool VIEWING_GROUP_DISABLED (int viewingGroup) const;

    virtual double ZERO_CONTOUR () const;
    
    void setScale (int scale);
    
    void setDisplayMask(int displayMask);
    void setContourLabels(bool contourLabels);
    void setCentreSymbols(bool centreSymbols);
    void setSimpleAreas(bool simpleAreas);
    void setSimpleSymbols(bool simpleSymbols);
    void setShowText(bool showText);
    void setShowTextHalo(bool showHalo);
    void setShowSymbols(bool showSymbols);

    void setDeepContours(double deepContours);
    void setShallowContours(double shallowContours);
    void setSafetyDepth(double safetyDepth);
    void setSafetyContours(double safetyContours);
    void setOwnShip(TSLCoord *xy);

    void colourPalette(int index)
    {
      m_paletteIndex = index;
    }
    int colourPalette()
    {
      return m_paletteIndex;
    }
    bool displayNoDataLayer() const
    {
      return m_displayNoDataLayer;
    }
    void displayNoDataLayer(bool state)
    {
      m_displayNoDataLayer = state;
    }

    virtual TSLS52StateObject *clone() const
    {
      return new S52StateObject(*this);
    }
   
    bool DISPLAY_OVERSCALE_PATTERN() const
    {
      return false;
    }

    void setAlterSymbols(bool alterSymbol)
    {
      m_alterSymbol = alterSymbol; 
    }


    void setBuoysBeacons(bool displayBuoysBeacons)
    {
      m_displayBuoysBeacons = displayBuoysBeacons;
    }


    // read and write the ini file data
    void writeConfiguration(const char *iniFile);
    void readConfiguration(const char *iniFile);
  private:

    int m_scale;

    TSLTimeType m_date;
    bool m_simpleAreas;
    bool m_simpleSymbols;
    bool m_alterSymbol;
    bool m_contourLabels;
    bool m_centreSymbols;
    bool m_showText;
    bool m_showTextHalo;
    bool m_showSymbol;

    int m_displayMask;

    double m_deepContours;
    double m_shallowContours;
    double m_safetyDepth;
    double m_safetyContours;

    TSLCoord m_ownShip;

    bool m_displayBuoysBeacons;

    int m_paletteIndex;
    bool m_displayNoDataLayer;
};

#endif