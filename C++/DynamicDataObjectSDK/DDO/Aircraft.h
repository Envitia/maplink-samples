// Aircraft.h: interface for the Aircraft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AIRCRAFT_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_AIRCRAFT_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


typedef std::vector<TSLDisplayObject*> DisplayMap_;

class Aircraft : public TSLDynamicDataObject  
{
public:
  Aircraft(unsigned int cls, double departureLatitude, double departureLongitude, const TSLCoordinateSystem& coordinateSystem);
  virtual ~Aircraft();
  
  virtual TSLDisplayObject* instantiateDO(TSLDisplayType key, int dsID=0) const;

  static double tmcPerMeter() { return m_tmcPerMeter; }
  static void tmcPerMeter( double d ) { m_tmcPerMeter = d; }

  static int random( int min, int max )
  {
    double rval = (double) rand() / (double) RAND_MAX;
    return (int)( rval * ((max - min) + 0.99999) + min );
  }
  
  double velocity() const;
  void velocity(double vel);		// vel is in km/h.

  double heading() const;
  void heading(double hdng);		// hdng is in degrees.

  const std::pair<double, double> & departure() const;
  void destination(double latitude, double longitude);
  const std::pair<double, double>& destination() const;

  const std::pair<double, double>& currentLocation() const;

  
  void update(const TSLCoordinateSystem & coordinateSystem, double rate);	// Flies the damn thing!

protected:
private:
  static double	      m_tmcPerMeter;	// TMC units / meter.

  unsigned int	      m_class;		// The class of aircraft.

  double	      m_velocity;
  double	      m_heading;

  std::pair<double, double> m_departureLocation;
  std::pair<double, double> m_destinationLocation;
  std::pair<double, double> m_currentLocation;
  TSLCoord m_currentLocationTmc;

  const TSLCoordinateSystem* m_coordinateSystem;
};

#endif // !defined(AFX_AIRCRAFT_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
