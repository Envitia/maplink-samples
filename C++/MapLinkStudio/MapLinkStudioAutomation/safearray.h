// SafeArrah.H

#ifndef _SAFEARRAY_H_
#define _SAFEARRAY_H_

#include "variant.h"

// Dim class encapsulates an array dimension
//@B Dim
//##ModelId=3D75D62E0056
class Dim : public SAFEARRAYBOUND
{
public:
  //##ModelId=3D75D62E0058
  Dim(const long iLo, const long iHi)
  { cElements = abs(iHi - iLo) + 1; lLbound = iLo; }
  //##ModelId=3D75D62E0060
  Dim(const long c)
  { cElements = c; lLbound = 0; }
  //##ModelId=3D75D62E0062
  const Dim & operator=(const Dim & dim) 
  { cElements = dim.cElements; lLbound = dim.lLbound; return *this; }
  //##ModelId=3D75D62E0064
  const Dim & operator=(const long c) 
  { cElements = c; lLbound = 0; return *this; }
  //##ModelId=3D75D62E0066
  ~Dim() {}
  //##ModelId=3D75D62E006A
  long Elements() { return cElements; }
  //##ModelId=3D75D62E006B
  long LBound() { return lLbound; }
  //##ModelId=3D75D62E006C
  long UBound() { return lLbound + cElements - 1; }
};
//@E Dim


// forward reference
template<class T, VARTYPE vt> 
class SafeArrayDataAccess;



// SafeArray container class for OLE types

//@B SafeArray1
//##ModelId=3D75D62E00B0
template<class T, VARTYPE vt> 
class SafeArray 
{
public:
  // Constructors
  //##ModelId=3D75D62E00BC
  SafeArray();
  //##ModelId=3D75D62E00BD
  SafeArray(SAFEARRAY * psaSrc);
  //##ModelId=3D75D62E00BF
  SafeArray(Dim & dim);
  // Copy constructor
  //##ModelId=3D75D62E00C4
  SafeArray(const SafeArray & saSrc);

  // Destructor
  //##ModelId=3D75D62E00C6
  ~SafeArray(); 

  // Operator equal
  //##ModelId=3D75D62E00C7
  const SafeArray & operator=(const SafeArray & saSrc);

  // Indexing
  //##ModelId=3D75D62E00CE
  T & Get(long i);
  //##ModelId=3D75D62E00D0
  T & Set(T & t, long i);
  //##ModelId=3D75D62E00D8
  T & operator[](const long i);    // C++ style (0-indexed)
  //##ModelId=3D75D62E00DA
  T & operator()(const long i);    // Basic style (LBound-indexed)
  //@E SafeArray1

  // Type casts
  //##ModelId=3D75D62E00DC
  operator SAFEARRAY(); 
  //##ModelId=3D75D62E00DD
  operator SAFEARRAY() const; 
  //##ModelId=3D75D62E00DF
  operator Variant(); 
  //##ModelId=3D75D62E00E2
  operator Variant() const; 

  // Operations
  //##ModelId=3D75D62E00E4
  bool ReDim(Dim & dim);
  //##ModelId=3D75D62E00E6
  long LBound();
  //##ModelId=3D75D62E00E7
  long UBound();
  //##ModelId=3D75D62E00E8
  long Elements();
  //##ModelId=3D75D62E00EC
  long Dimensions();
  //##ModelId=3D75D62E00ED
  bool IsSizable();
  //##ModelId=3D75D62E00EE
  void Lock();
  //##ModelId=3D75D62E00EF
  void Unlock();

  //@B SafeArray2
private:
  //##ModelId=3D75D62E00F7
  SAFEARRAY * psa;
  //##ModelId=3D75D62E00FB
  bool m_constructed ;
  //##ModelId=3D75D62E0100
  void Destroy();
  //##ModelId=3D75D62E0101
  bool IsConstructed();
  //##ModelId=3D75D62E0102
  void Constructed(bool f);

  friend class SafeArrayDataAccess<T, vt>;
};
//@E SafeArray2


// class for getting at the buffer inside a safearray
//##ModelId=3D75D62E0114
template<class T, VARTYPE vt> 
class SafeArrayDataAccess
{
public:
  //##ModelId=3D75D62E0120
  SafeArrayDataAccess( const SafeArray<T, vt>& sa ); // constructor locks the data
  //##ModelId=3D75D62E0122
  ~SafeArrayDataAccess();                      // destructor unlocks the data

  //##ModelId=3D75D62E0123
  operator T*();        // access the data
  //##ModelId=3D75D62E0128
  operator T*() const;

private:
  //##ModelId=3D75D62E012B
  SAFEARRAY* m_array;
  //##ModelId=3D75D62E012F
  T* m_data;
};



//##ModelId=3D75D62E0120
template<class T, VARTYPE vt> 
inline SafeArrayDataAccess<T,vt>::SafeArrayDataAccess( const SafeArray<T, vt>& sa )
{
  m_array = sa.psa;
  SafeArrayAccessData( m_array, (void**)&m_data );
}

//##ModelId=3D75D62E0122
template<class T, VARTYPE vt> 
inline SafeArrayDataAccess<T,vt>::~SafeArrayDataAccess( )
{
  SafeArrayUnaccessData( m_array );
}

//##ModelId=3D75D62E0123
template<class T, VARTYPE vt> 
inline SafeArrayDataAccess<T,vt>::operator T*()
{
  return m_data;
}

//##ModelId=3D75D62E0128
template<class T, VARTYPE vt> 
inline SafeArrayDataAccess<T,vt>::operator T*() const
{
  return m_data;
}




// Private helpers

//##ModelId=3D75D62E0100
template<class T, VARTYPE vt> 
inline void SafeArray<T,vt>::Destroy()
{
  Constructed(false);
  HRESULT hres = SafeArrayDestroy(psa);
  if (hres) throw hres;
}

//##ModelId=3D75D62E0101
template<class T, VARTYPE vt> 
inline bool SafeArray<T,vt>::IsConstructed()
{
  return m_constructed ;
}

//##ModelId=3D75D62E0102
template<class T, VARTYPE vt> 
inline void SafeArray<T,vt>::Constructed(bool f)
{
  m_constructed = f ;
}

// Constructors
//##ModelId=3D75D62E00BC
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::SafeArray() 
{ 
  Dim dim(0);
  psa = SafeArrayCreate(vt, 1, &dim); 
  Constructed(true);
  if (psa == NULL) throw E_OUTOFMEMORY;
}


//##ModelId=3D75D62E00BD
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::SafeArray(SAFEARRAY * psaSrc) 
{ 
  if (SafeArrayGetDim(psaSrc) != 1) throw E_INVALIDARG;
  Constructed(false);
  psa = psaSrc; 
}

//##ModelId=3D75D62E00C4
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::SafeArray(const SafeArray & saSrc) 
{
  HRESULT hres = SafeArrayCopy(saSrc.psa, &psa);
  Constructed(true);
  if (hres) throw hres;
}


//##ModelId=3D75D62E00BF
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::SafeArray(Dim & dim) 
{
  psa = SafeArrayCreate(vt, 1, &dim); 
  Constructed(true);
  if (psa == NULL) throw E_OUTOFMEMORY;
} 

// Destructor
//##ModelId=3D75D62E00C6
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::~SafeArray()
{
  if (IsConstructed()) {
    Destroy();
  }
} 

// Operator = 
//##ModelId=3D75D62E00C7
template<class T, VARTYPE vt> 
const SafeArray<T,vt> & SafeArray<T,vt>::operator=(const SafeArray & saSrc)
{
  if (psa) {

    SafeArrayDestroy(psa);
  }
  HRESULT hres = SafeArrayCopy(saSrc.psa, &psa);
  Constructed(true);
  if (hres) throw hres;
  return *this;
}

// Type casts
//##ModelId=3D75D62E00DC
template<class T, VARTYPE vt> 
inline SafeArray<T,vt>::operator SAFEARRAY()
{
  return *psa; 
}

//##ModelId=3D75D62E00DD
template<class T, VARTYPE vt> 
SafeArray<T,vt>::operator SAFEARRAY() const
{
  static SAFEARRAY * psaT;
  SafeArrayCopy(psa, &psaT);
  return *psaT;
}

//##ModelId=3D75D62E00DF
template<class T, VARTYPE vt> 
SafeArray<T,vt>::operator Variant() 
{
  return Variant(psa);
}

//##ModelId=3D75D62E00E2
template<class T, VARTYPE vt> 
SafeArray<T,vt>::operator Variant() const
{
  static Variant v(psa);
  return v;
}

// Indexing
//##ModelId=3D75D62E00CE
template<class T, VARTYPE vt> 
T & SafeArray<T,vt>::Get(long i)
{
  static T tRes;
  HRESULT hres = SafeArrayGetElement(psa, &i, &tRes);
  if (hres) throw hres;
  return tRes;
}

//@B Indexing
//##ModelId=3D75D62E00D0
template<class T, VARTYPE vt> 
inline T & SafeArray<T,vt>::Set(T & t, long i)
{
  HRESULT hres = SafeArrayPutElement(psa, &i, (T *)&t);
  if (hres) throw hres;
  return t;
}

//##ModelId=3D75D62E00D8
template<class T, VARTYPE vt> 
inline T & SafeArray<T,vt>::operator[](const long i)
{
  if (i < 0 || i > Elements() - 1) throw DISP_E_BADINDEX;
  return ((T*)psa->pvData)[i];
}

//##ModelId=3D75D62E00DA
template<class T, VARTYPE vt> 
T & SafeArray<T,vt>::operator()(const long i)
{
  if (i < LBound() || i > UBound()) throw DISP_E_BADINDEX;
  return ((T*)psa->pvData)[i - LBound()];
}
//@E Indexing

// Operations
//##ModelId=3D75D62E00E4
template<class T, VARTYPE vt> 
bool SafeArray<T,vt>::ReDim(Dim &dim)
{
  if (!IsSizable()) {
    return false;
  }
  HRESULT hres = SafeArrayRedim(psa, &dim);
  if (hres) throw hres;
  return true;
}

//##ModelId=3D75D62E00E6
template<class T, VARTYPE vt> 
long SafeArray<T,vt>::LBound()
{
  long iRes;
  HRESULT hres = SafeArrayGetLBound(psa, 1, &iRes);
  if (hres) throw hres;
  return iRes;
}

//##ModelId=3D75D62E00E8
template<class T, VARTYPE vt> 
inline long SafeArray<T,vt>::Elements()
{
  return psa->rgsabound[0].cElements;
}

//##ModelId=3D75D62E00E7
template<class T, VARTYPE vt> 
long SafeArray<T,vt>::UBound()
{
  long iRes;
  HRESULT hres = SafeArrayGetUBound(psa, 1, &iRes);
  if (hres) throw hres;
  return iRes;
}

//##ModelId=3D75D62E00EC
template<class T, VARTYPE vt> 
inline long SafeArray<T,vt>::Dimensions()
{
  return 1;
}

//##ModelId=3D75D62E00ED
template<class T, VARTYPE vt> 
inline bool SafeArray<T,vt>::IsSizable()
{
  return (psa->fFeatures & FADF_FIXEDSIZE) ? false : true;
}

//##ModelId=3D75D62E00EE
template<class T, VARTYPE vt>
inline void SafeArray<T,vt>::Lock()
{
  HRESULT hres = SafeArrayLock(psa);
  if (hres) throw hres;
}

//##ModelId=3D75D62E00EF
template<class T, VARTYPE vt>
inline void SafeArray<T,vt>::Unlock()
{
  HRESULT hres = SafeArrayUnlock(psa);
  if (hres) throw hres;
}


#endif // _SAFEARRAY_H_
