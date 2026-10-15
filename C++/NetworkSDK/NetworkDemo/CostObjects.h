#ifndef costobjects_h_
#define costobjects_h_ 1

//
//  The following classes are the different type of cost objects
//  that can be used by the routing algorithm.
//
//  The objects defined are as follows:
//
//  0.  Shortest route (this is the default cost object supplied by MapLink).
//  1.  Quickest route.
//


// Some default speeds for cars.
// -----------------------------
// The values are converted from km/hour to meters/sec.
static const double METRES_PER_SEC	   =  1000.0 / 3600.0;

//
// a.  Normal speed.
static const double car_NormalSpeed	   =  50.0*METRES_PER_SEC;
// b.  Motorway speed.
static const double car_MotorwaySpeed	   = 125.0*METRES_PER_SEC;
// c.  Major Road 0 speed.
static const double car_MajorRoad0Speed    =  90.0*METRES_PER_SEC;
// d.  Major Road 1 speed.
static const double car_MajorRoad1Speed    =  80.0*METRES_PER_SEC;
// e.  Secondary Road speed.
static const double car_SecondaryRoadSpeed =  65.0*METRES_PER_SEC;
// f.  Local Road 0 speed.
static const double car_LocalRoad0Speed    =  60.0*METRES_PER_SEC;
// g.  Local Road 1 speed.
static const double car_LocalRoad1Speed    =  55.0*METRES_PER_SEC;
// h.  Local Road 2 speed.
static const double car_LocalRoad2Speed    =  40.0*METRES_PER_SEC;


// This contains the MapLink enums and macros for manipulating link feature codes.
#include "TSLNetworkFeatureCodes.h"


// This function returns a car speed based on road type.
static inline double getSpeed(long featureCode)
{
  double speed = car_NormalSpeed;

  switch( TSLFC_FUNCTIONAL_CLASS(featureCode) )
  {
  case TSLFunctionalClass_Motorway:
    speed = car_MotorwaySpeed;
    break;

  case TSLFunctionalClass_ImportantMajorRoad:
    speed = car_MajorRoad0Speed;
    break;

  case TSLFunctionalClass_OtherMajorRoad:
    speed = car_MajorRoad1Speed;
    break;

  case TSLFunctionalClass_SecondaryRoad:
    speed = car_SecondaryRoadSpeed;
    break;

  case TSLFunctionalClass_LocalConnectingRoad:
  case TSLFunctionalClass_ImportantLocalRoad:
    speed = car_LocalRoad0Speed;
    break;

  case TSLFunctionalClass_LocalRoad:
    speed = car_LocalRoad1Speed;
    break;

  case TSLFunctionalClass_MinorLocalRoad:
    speed = car_LocalRoad2Speed;
    break;

  case TSLFunctionalClass_OtherRoad:
    break;
  }

  return speed;
}


//
//  0.  Shortest route cost object.
//  -------------------------------
//
//  TSLPathCost - this is the default supplied by MapLink.

//
//  1.  Quickest route cost object.
//  -------------------------------
//
class TSLQuickest : public TSLPathCost
{
public:
  virtual distance_type operator () (const TSLLink& link, const TSLTurn& turn) const
  {
    return link.length()/getSpeed( link.featureCode() ) + turn;
  }
};

#endif // costobjects_h_