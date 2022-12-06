// *********************************************************************************
// * Copyright (c) 1998 to 2021 by Envitia Group PLC.
// *********************************************************************************
#ifndef _MAPLINK3DIMODE_H_
#define _MAPLINK3DIMODE_H_ 1

//# include "MapLinkIModeDLL.h"

#include "MapLink.h"

// Include the MapLink Earth SDK
// Note that the Earth SDK makes use of namespaces within the API
// in this case envitia::maplink::earth
#include <MapLinkEarth.h>
using namespace envitia::maplink;

//#include "tslmaplinkimodeerrors.h"

#include "Interactionmoderequest.h"
#include "interaction.h"

#include "createpolygoninteraction.h"
#include "createpolylineinteraction.h"
#include "createsymbolinteraction.h"
#include "createtextinteraction.h"
#include "deletegeometryinteraction.h"
#include "selectinteraction.h"
#include "trackballviewinteraction.h"

#include "interactionmodemanager.h"

#endif
