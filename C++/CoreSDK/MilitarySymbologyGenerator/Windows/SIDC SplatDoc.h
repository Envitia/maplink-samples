
// SIDC SplatDoc.h : interface of the CSidcSplatDoc class
//

#include <tslmilitarysymbologygenerator.h>
#include <MapLink.h>
#include <memory>

#pragma once

template <typename T>
struct TSL_deleter
{
	void operator() (T* p) { p->destroy(); }
};

class CSidcSplatDoc : public CDocument
{
protected: // create from serialization only
	CSidcSplatDoc() noexcept;
	DECLARE_DYNCREATE(CSidcSplatDoc)

public:
	virtual BOOL OnNewDocument();

public:
	virtual ~CSidcSplatDoc();
	bool addToSurface(TSLDrawingSurface*);
	void extendPolyline(TSLCoord);
	void endPolyline();
	void addSymbol(TSLCoord);
	void addSymbol(TSLCoord, TSLSymbol*);

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

private:
	std::unique_ptr<TSLMapDataLayer, TSL_deleter<TSLMapDataLayer>> map_layer;
	std::unique_ptr<TSLStandardDataLayer, TSL_deleter<TSLStandardDataLayer>> dynamic_layer;
	std::unique_ptr<TSLCoordSet, TSL_deleter<TSLCoordSet>> coords{new TSLCoordSet};
	TSLPolyline* polyline = nullptr;
	TSLMilitarySymbologyGenerator symbolGenerator;
};
