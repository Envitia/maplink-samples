// App6aAircraft.h: interface for the App6aAircraft class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_App6aAircraft_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
#define AFX_App6aAircraft_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <vector>

typedef std::vector<TSLDisplayObject*> DisplayMap_;

class App6aAircraft : public TSLDynamicDataObject  
{
public:
  App6aAircraft(unsigned int cls);
  virtual ~App6aAircraft();
  
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

  void destination(long dest_x, long dest_y)
  {
    m_destination.x( dest_x );
    m_destination.y( dest_y );
  }

  void update(double dt, double rate);	// Flies the damn thing!

protected:
private:
  static double	      m_tmcPerMeter;	// TMC units / meter.

  unsigned int	      m_class;		// The class of App6aAircraft.

  double	      m_velocity;
  double	      m_heading;

  TSLCoord	      m_destination;	// Destination in TMC units.
};

#endif // !defined(AFX_App6aAircraft_H__56D381A8_65BC_11D3_8A80_00104BB7492E__INCLUDED_)
