#include "pch.h"
#define LTV23_CONFIG
#include "framework.h"
//#include "types/maplink_platforms.h"
//#include "tgmdll.h"
#include <attributesets/pointattributeset.h>
#include "geometry/point.h"
#include "geometry/text.h"
//#include "tgm_typedefs.h"
#include "SIDC Splat.h"
//#include "tslatomic.h"
//#include "tmsdll.h"
#include "drawingsurfaces/drawingsurface.h"
#include "SIDC SplatDoc.h"
#include <helpers/tgmhostspecific.h>
#include "resources/symbols/maplsymbol.h"
#include "resources/symbols/nttrashrastersymbol.h"
#include <ClassLib/LtWrappr.h>
#include <trashdll.h>
#include <trashinterface/trashobject.h>
#include <NT_LeadTools/ntleadtoolstrashobject.h>
#include <utility/hostspecific.h>

#include <militarysymbology/tslsidc2525d.h>

#include <propkey.h>

#include <vector>
#include <filesystem>

TSLMilitarySymbologyOptions options = {
  /*.scale = */0.333333333f,
};

IMPLEMENT_DYNCREATE(CSidcSplatDoc, CDocument)

BEGIN_MESSAGE_MAP(CSidcSplatDoc, CDocument)
END_MESSAGE_MAP()

CSidcSplatDoc::CSidcSplatDoc() noexcept
{
  const std::filesystem::path maplink_home = std::filesystem::path{ __FILE__ }.parent_path().parent_path().parent_path().parent_path().parent_path().parent_path();
  
  if (!symbolGenerator.loadSymbols((maplink_home / "config/symbols/MILSTD_2525D").u8string().c_str()))
	throw std::exception{ "Failed to populate symbology path cache\n" };
}

CSidcSplatDoc::~CSidcSplatDoc() = default;

bool CSidcSplatDoc::addToSurface(TSLDrawingSurface* ds)
{
	if (!ds || !map_layer)
		return false;
	return ds->addDataLayer(map_layer.get(), "map")
		&& ds->addDataLayer(dynamic_layer.get(), "overlay");
}

void CSidcSplatDoc::extendPolyline(TSLCoord p)
{
	coords->add(p);
	if (coords->size() < 2)
		return;

	if (coords->size() == 2)
	{
		polyline = dynamic_layer->entitySet()->createPolyline(0, coords.get());
		TSLStyleID colour = TSLDrawingSurface::getIDOfNearestColour(127, 0, 255);
		polyline->setRendering(TSLRenderingAttributeEdgeStyle, 48);
		polyline->setRendering(TSLRenderingAttributeEdgeColour, colour);
		polyline->setRendering(TSLRenderingAttributeEdgeThickness, 20.0);
		polyline->setRendering(TSLRenderingAttributeEdgeThicknessUnits, TSLDimensionUnitsMapUnits);
	}
	else
		polyline->points(coords.get());

	dynamic_layer->notifyChanged(true);
}

void CSidcSplatDoc::endPolyline()
{
	coords->clear();
	polyline = nullptr;
}

void CSidcSplatDoc::addSymbol(TSLCoord p)
{
  static std::vector<char const*> const sidcStrings = {
	//"10051000001417010000",
	//"10031000001652000000",
	//"10030100001101010000",
	"10031000001211000000",
	"10031500002301000000",
	"10036000001103000000",
	"10061000001211000000",
	"10031000001211010000",
	"10031000001630000000",
	"10030100001101000000",
	"10030100001201000000",
	"10060100001101000000",
	"10060100001201000000",
	"10034000001101070000",
	"10032000001208010000",
	"10131000001211000000",
	"10231000001630000000",
	"10031010001630000000",
	"10031020001630000000",
	"10031030001630000000",
	"10031040001630000000",
	"10031050001630000000",
	"10031007001630000000",
	"10031000001417000000",
	"10031000001417010000",
	"10031000001417020000",
	"10031000001417950000",
	"10031000001417960000",
	"10031000001417970000",
	"10031000001417980000",
	//"10431037171211059797"
	//"10031037141211050000"
  };

  static auto it = sidcStrings.begin();
  TSLSymbol* symbol = symbolGenerator.getSymbol(TSLSidc2525d{ *it }, TSLSidc2525d::SymbolSet::Space, options);
  if (++it == sidcStrings.end())
	it = sidcStrings.begin();
  if (symbol)
    addSymbol(p, symbol);
}


void CSidcSplatDoc::addSymbol(TSLCoord p, TSLSymbol* symbol)
{
  symbol->move(p, {0,0});
  assert(dynamic_layer->entitySet()->insert(symbol));
  dynamic_layer->notifyChanged(true);
}

BOOL CSidcSplatDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return FALSE;
}

BOOL CSidcSplatDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return false;

	map_layer.reset(new TSLMapDataLayer);
	auto path = TSLUTF8Encoder(lpszPathName);
	if (!map_layer->loadData(path))
	{
		TSLSimpleString msg("");
		bool anyErrors = TSLThreadedErrorStack::errorString(msg, "Cannot load map : \n");
		if (anyErrors)
			AfxMessageBox((LPCTSTR)TSLUTF8Decoder(msg), MB_OK);
		return false;
	}

	dynamic_layer.reset(new TSLStandardDataLayer);


	TSLTMC x1, y1, x2, y2;
	map_layer->getTMCExtent(&x1, &y1, &x2, &y2);
	int n_tests = 9;

	TSLTMC dx = (x2 - x1) / (n_tests - 1);

	TSLTMC x = x1;

	{ // test 0 Standard Identity
	  TSLSidc2525d base{ "10031000001211000000" };
	  using SI = TSLSidc2525d::StandardIdentity;
	  auto last = SI::Hostile;
	  TSLTMC dy = (y2 - y1) / 6;
	  TSLTMC y = y1;
	  for (auto si = SI::Pending; si <= SI::Hostile; si = SI{ static_cast<uint8_t>(si) + 1u })
	  {
		base.standardIdentity = si;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 1 Symbol Set
	  TSLSidc2525d base{ "10031000001100000000" };
	  using SS = TSLSidc2525d::SymbolSet;
	  std::vector<SS> sss = {
		//SS::Unknown,
		SS::Air,
		SS::AirMissile, // use air frame
		SS::Space,
		SS::SpaceMissile, // use space frame
		SS::LandUnit,
		SS::LandCivilianUnitOrganization, // use landUnit frame
		SS::LandEquipment, // use landUnit frame
		SS::LandInstallation,
		//SS::ControlMeasure,
		SS::SeaSurface,
		SS::SeaSubsurface,
		SS::MineWarfare,
		SS::Activities,
		//SS::Atmospheric,
		//SS::Oceanographic,
		//SS::MeteorologicalSpace,
		SS::SignalsIntelligenceSpace,
		SS::SignalsIntelligenceAir,
		SS::SignalsIntelligenceLand,
		SS::SignalsIntelligenceSurface,
		SS::SignalsIntelligenceSubsurface,
		SS::Cyberspace,
	  };
	  TSLTMC dy = (y1 - y2) / ((long)(sss.size()) - 1);
	  TSLTMC y = y2;
	  for (auto ss : sss)
	  {
		base.symbolSet = ss;
		auto symbol = symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options);
		if (symbol)
		  addSymbol(TSLCoord{ x, y }, symbol);
		y += dy;
	  }
	}
	x += dx;

	{ // test 2 Status
	  TSLSidc2525d base{ "10031000001211000000" };
	  using S = TSLSidc2525d::Status;
	  TSLTMC dy = (y2 - y1) / 5;
	  TSLTMC y = y1;
	  for (auto s = S::Present; s <= S::PresentFullToCapacity; s = S{ static_cast<uint8_t>(s) + 1u })
	  {
		base.status = s;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 3 HqTfDummy
	  TSLSidc2525d base{ "10031000001211000000" };
	  using HTD = TSLSidc2525d::HqTfDummy;
	  TSLTMC dy = (y2 - y1) / 7;
	  TSLTMC y = y1;
	  for (auto htd = HTD::None; htd <= HTD::HqTaskForceDummy; htd = HTD{ static_cast<uint8_t>(htd) + 1u })
	  {
		base.hqTfDummy = htd;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 4 Context
	  TSLSidc2525d base{ "10031000001211000000" };
	  using C = TSLSidc2525d::Context;
	  TSLTMC dy = (y2 - y1) / 2;
	  TSLTMC y = y1;
	  for (auto c = C::Reality; c <= C::Simulation; c = C{ static_cast<uint8_t>(c) + 1u })
	  {
		base.context = c;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 5 Echelon
	  TSLSidc2525d base{ "10031000001211000000" };
	  using C = TSLSidc2525d::Context;
	  TSLTMC dy = (y2 - y1) / 13;
	  TSLTMC y = y1;
	  for (uint8_t e = 11; e <= 18; ++e)
	  {
		base.amplifier = e;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	  for (uint8_t e = 21; e <= 26; ++e)
	  {
		base.amplifier = e;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 6 Shape
	  using C = TSLSidc2525d::Context;
	  using SI = TSLSidc2525d::StandardIdentity;
	  using S = TSLSidc2525d::Status;
	  using SS = TSLSidc2525d::SymbolSet;
	  using HTD = TSLSidc2525d::HqTfDummy;
	  struct test {
		SS ss;
	  };
	  std::vector<test> tests = {
		//{CS::PlannedAnticipatedSuspect, SS::Air, HTD::Hq},
		{SS::Air},
		{SS::SeaSurface},
		{SS::LandUnit},
		{SS::SeaSurface},
		{SS::SeaSubsurface},
		{SS::Air},
		{SS::SeaSurface},
		{SS::LandUnit},
		{SS::SeaSurface},
		{SS::SeaSubsurface},
		//{S::PresentDestroyed, SS::SeaSubsurface, HTD::TaskForce},
		//{S::PresentFullToCapacity, SS::LandInstallation, HTD::HqTaskForceDummy},
	  };
	  TSLSidc2525d base{ "10031000001100000000" };
	  TSLTMC dy = (y2 - y1) / ((long)(tests.size()) - 1);
	  TSLTMC y = y1;
	  int i = 0;
	  for (auto t : tests)
	  {
		++i;
		base.context = i % 2 ? C::Excercise : C::Simulation;
		base.standardIdentity = i % 2 ? SI::Friend : SI::Hostile;
		base.status = S{ static_cast<uint8_t>(static_cast<uint8_t>(S::PlannedAnticipatedSuspect) + i % 5) };
		base.symbolSet = t.ss;
		base.hqTfDummy = HTD{ static_cast<uint8_t>(static_cast<uint8_t>(HTD::Hq) + i % 7)};
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, options));
		y += dy;
	  }
	}
	x += dx;

	{ // test 8 Scale
	  TSLSidc2525d base{ "10031000001213000000" };
	  using SI = TSLSidc2525d::StandardIdentity;
	  TSLTMC dy = (y2 - y1) / 3;
	  TSLTMC y = y1;
	  for (int i = 1; i <= 4; ++i)
	  {
		auto local_opt = options;
		local_opt.scale = options.scale * i / 3;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, TSLSidc2525d::SymbolSet::Space, local_opt));
		y += dy;
	  }
	}
	x += dx;

	{ // test 9 APP-6D Cyberspace
	  TSLSidcApp6d base{ "10036000001404000000" };
	  using SS = TSLSidcApp6d::SymbolSet;
	  auto sss = {
		SS::CyberspaceSpace,
		SS::CyberspaceAir,
		SS::CyberspaceLand,
		SS::CyberspaceSurface,
		SS::CyberspaceSubsurface,
	  };
	  TSLTMC dy = (y2 - y1) / (sss.size() - 1);
	  TSLTMC y = y1;
	  for (auto ss : sss)
	  {
		base.symbolSet = ss;
		addSymbol(TSLCoord{ x, y }, symbolGenerator.getSymbol(base, options));
		y += dy;
	  }
	}
	x += dx;

	return true;
}
