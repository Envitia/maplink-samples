// Variant.H - Variant class

#ifndef VARIANT_H_
#define VARIANT_H_

#define VCONSTRUCTBITS
#ifdef VCONSTRUCTBITS
const DWORD VT_CONSTRUCTED = 0x800;
#endif

// Variant class - wraps VARIANT type

//##ModelId=3D75D62601B7
typedef const VARIANT* LPCVARIANT;

//@B Variant1
//##ModelId=3D75D62602A3
class Variant : public VARIANT
{
public:

  // Constructors
  //##ModelId=3D75D62602A5
  Variant();
  //##ModelId=3D75D62602AD
  Variant(const Variant& v);   
  //##ModelId=3D75D62602AF
  Variant(BYTE nSrc);	// VT_UI1
  //@E Variant1
  //##ModelId=3D75D62602B1
  Variant(short iSrc, VARTYPE vtSrc = VT_I2); 
  //##ModelId=3D75D62602B7
  Variant(long lSrc, VARTYPE vtSrc = VT_I4); 
  //##ModelId=3D75D62602BA
  Variant(float fltSrc); // VT_R4
  //##ModelId=3D75D62602BC
  Variant(double dblSrc, VARTYPE vtSrc = VT_R8); 
  // VT_BOOL (see short)
  // VT_ERROR (see long)
  //##ModelId=3D75D62602BF
  Variant(const CURRENCY& curSrc); // VT_CY
  //##ModelId=3D75D62602C2
  Variant(LONGLONG i64Src); // Alternate VT_CY
  // VT_DATE (see double)
  //##ModelId=3D75D62602C4
  Variant(BSTR bsSrc, VARTYPE vtSrc = VT_BSTR);
  //##ModelId=3D75D62602C7
  Variant(LPCWSTR wszSrc);
  //##ModelId=3D75D62602CB
  Variant(IDispatch * pdispSrc); // VT_DISPATCH
  //##ModelId=3D75D62602CD
  Variant(IUnknown * punkSrc); // VT_UNKNOWN
  //##ModelId=3D75D62602CF
  Variant(const SAFEARRAY * psaSrc, VARTYPE vt = VT_NULL); // VT_ARRAY | VT_TYPE

  // Destructor
  //##ModelId=3D75D62602D2
  ~Variant();

  // Copy constructors
  //##ModelId=3D75D62602D5
  const Variant& operator=(const VARIANT& varSrc);
  //##ModelId=3D75D62602D7
  const Variant& operator=(LPCVARIANT pSrc);
  //##ModelId=3D75D62602D9
  const Variant& operator=(const Variant& vSrc);

  // Assignment
  //##ModelId=3D75D62602DF
  const Variant& operator=(BYTE bSrc);	
  //##ModelId=3D75D62602E1
  const Variant& operator=(short iSrc);
  //##ModelId=3D75D62602E3
  const Variant& operator=(long lSrc);
  //##ModelId=3D75D62602E5
  const Variant& operator=(float flt);
  //##ModelId=3D75D62602EA
  const Variant& operator=(double dbl);
  //##ModelId=3D75D62602EC
  const Variant& SetBoolean(VARIANT_BOOL f);
  //##ModelId=3D75D62602EE
  const Variant& SetError(long err);
  //##ModelId=3D75D62602F4
  const Variant& SetDate(DATE dt);

  // VARIANT_BOOL (see short)
  // Error (see long)
  //##ModelId=3D75D62602F6
  const Variant& operator=(const CURRENCY& curSrc);
  //##ModelId=3D75D62602F8
  const Variant& operator=(LONGLONG  curSrc);
  // Date (see double)
  //##ModelId=3D75D62602FD
  const Variant& operator=(const BSTR bsSrc);
  //##ModelId=3D75D62602FF
  const Variant& operator=(LPCSTR szSrc);
  //##ModelId=3D75D6260301
  const Variant& operator=(LPCWSTR wszSrc);
  //##ModelId=3D75D6260307
  const Variant& operator=(IDispatch * pdispSrc); 
  //##ModelId=3D75D6260309
  const Variant& operator=(IUnknown * punkSrc); 
  //##ModelId=3D75D626030B
  const Variant& operator=(const SAFEARRAY * psaSrc); 

  // Operations
  //##ModelId=3D75D626030D
  void Destroy(){VariantDestroy();}
  //##ModelId=3D75D6260311
  void ChangeType(VARTYPE vartype, Variant * pSrc = NULL);
  //##ModelId=3D75D6260314
  BOOL IsMissing();
  //##ModelId=3D75D6260315
  VARTYPE Type();             // Return type
  //##ModelId=3D75D6260316
  void Type(VARTYPE vartype);      // Change type

  // Make a copy of current with given type
  //##ModelId=3D75D626031C
  Variant CopyAs(VARTYPE vtDst);
  // Make a copy of current with given or existing type, 
  // whichever has greater precision 
  //##ModelId=3D75D626031E
  Variant CopyPromote(VARTYPE vtDst);

  // Type casts
  //operator VARIANT_BOOL();
  //##ModelId=3D75D6260320
  operator BYTE();
  //##ModelId=3D75D6260321
  operator BYTE() const;
  //##ModelId=3D75D6260326
  operator short();
  //##ModelId=3D75D6260327
  operator short() const; 
  //##ModelId=3D75D6260329
  operator long();
  //##ModelId=3D75D626032A
  operator long() const;
  //##ModelId=3D75D626032C
  operator float();
  //##ModelId=3D75D626032F
  operator float() const;
  //##ModelId=3D75D6260331
  operator double();
  //##ModelId=3D75D6260332
  operator double() const;
  //##ModelId=3D75D6260334
  operator LONGLONG();
  //##ModelId=3D75D6260335
  operator LONGLONG() const;
  //##ModelId=3D75D626033A
  operator CURRENCY();
  //##ModelId=3D75D626033B
  operator CURRENCY() const;
  //##ModelId=3D75D626033D
  operator BSTR();
  //##ModelId=3D75D626033E
  operator BSTR() const;

  //##ModelId=3D75D6260340
  operator Variant();
  //##ModelId=3D75D6260343
  operator Variant() const;

  //##ModelId=3D75D626036C
  friend ostream& operator<<(ostream& os, Variant& v);

  // Arithmetic operators
  // +
  //##ModelId=3D75D6260370
  friend Variant operator+(Variant& v1, Variant& v2);    
  //##ModelId=3D75D6260345
  const Variant& operator+=(Variant v);
  //##ModelId=3D75D6260347
  Variant & operator++(); // Prefix
  //##ModelId=3D75D6260348
  Variant operator++(int); // Postfix

  // -
  //##ModelId=3D75D6260374
  friend Variant operator-(Variant& v1, Variant& v2);
  //##ModelId=3D75D626034E
  const Variant& operator-=(Variant v);
  //##ModelId=3D75D6260350
  Variant & operator--(); // Prefix
  //##ModelId=3D75D6260351
  Variant operator--(int); // Postfix

  // *
  //##ModelId=3D75D6260378
  friend Variant operator*(Variant& v1, Variant& v2);
  //##ModelId=3D75D6260353
  const Variant& operator*=(Variant v);

  // /
  //##ModelId=3D75D626037C
  friend Variant operator/(Variant& v1, Variant& v2);
  //##ModelId=3D75D6260358
  const Variant& operator/=(Variant v);

  // %
  //##ModelId=3D75D6260380
  friend Variant operator%(Variant& v1, Variant& v2);
  //##ModelId=3D75D626035A
  const Variant& operator%=(Variant v);

  // Logical operators
  //##ModelId=3D75D6260384
  friend BOOL operator==(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D6260388
  friend BOOL operator<(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D626038C
  friend BOOL operator!=(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D6260390
  friend BOOL operator<=(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D6260394
  friend BOOL operator>=(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D6260398
  friend BOOL operator>(const Variant& v1, const Variant& v2);
  //##ModelId=3D75D626035C
  static BOOL Preciser(VARTYPE vt1, VARTYPE vt2);

  //@B Variant2
private: 
  // Constructor helper
  //##ModelId=3D75D6260361
  void VariantCreate(VARTYPE vtA = VT_EMPTY);

  // Destructor helper
  //##ModelId=3D75D6260363
  void VariantDestroy();
  //##ModelId=3D75D6260364
  VARIANT_BOOL IsConstructed();
  //##ModelId=3D75D6260365
  void Constructed(VARIANT_BOOL f);

  // Look, Ma! No data.
};
//@E Variant2


// Implementation of inline members

//##ModelId=3D75D6260361
inline void Variant::VariantCreate(VARTYPE vtA)
{
  vt = vtA;
  wReserved1 = 0;
  Constructed(TRUE);
}

//##ModelId=3D75D6260363
inline void Variant::VariantDestroy()
{
  Constructed(FALSE);
  HRESULT hres = VariantClear(this);
  if (hres) throw hres;
}

//##ModelId=3D75D6260364
inline VARIANT_BOOL Variant::IsConstructed()
{
#ifdef VCONSTRUCTBITS
  return (wReserved1 & VT_CONSTRUCTED) ? TRUE : FALSE;
#else
  return FALSE;
#endif
}

//##ModelId=3D75D6260365
inline void Variant::Constructed(VARIANT_BOOL f)
{
#ifdef VCONSTRUCTBITS
  if (f) {
    wReserved1 |= VT_CONSTRUCTED;
  } else {
    wReserved1 &= ~VT_CONSTRUCTED;
  }
#endif
}

//@B VariantCon
//##ModelId=3D75D62602A5
inline Variant::Variant()
{
  VariantCreate();
}

//##ModelId=3D75D62602AD
inline Variant::Variant(const Variant& v)
{
  VariantCreate();
  HRESULT hres = VariantCopy(this, (Variant *)&v);
  if (hres) throw hres;
}

//##ModelId=3D75D62602AF
inline Variant::Variant(BYTE nSrc)
{ 
  VariantCreate(VT_UI1); 
  bVal = nSrc; 
}
//@E VariantCon

//##ModelId=3D75D62602BA
inline Variant::Variant(float fltSrc)
{ 
  VariantCreate(VT_R4); 
  fltVal = fltSrc; 
}

//##ModelId=3D75D62602BF
inline Variant::Variant(const CURRENCY& curSrc)
{ 
  VariantCreate(VT_CY); 
  cyVal.int64 = curSrc.int64;
}

//##ModelId=3D75D62602C2
inline Variant::Variant(LONGLONG i64Src) // Alternate VT_CY
{ 
  VariantCreate(VT_CY); 
  cyVal.int64 = i64Src; 
}


//##ModelId=3D75D62602C7
inline Variant::Variant(LPCWSTR wszSrc)
{
  VariantCreate(VT_BSTR); 
  bstrVal = SysAllocString(wszSrc);
}

//##ModelId=3D75D62602CB
inline Variant::Variant(IDispatch * pdispSrc)
{ 
  VariantCreate(VT_DISPATCH);
  pdispVal = pdispSrc;
}

//##ModelId=3D75D62602CD
inline Variant::Variant(IUnknown * punkSrc)
{
  VariantCreate(VT_UNKNOWN);
  punkVal = punkSrc;
}

//@B VariantDes
//##ModelId=3D75D62602D2
inline Variant::~Variant()
{
  if (IsConstructed()) {
    VariantDestroy();
  }
}
//@E VariantDes

// Copy constructors
//##ModelId=3D75D62602D5
inline const Variant& Variant::operator=(const VARIANT& varSrc)
{
  HRESULT hres = VariantCopy(this, (LPVARIANT)&varSrc);
  if (hres) throw hres;
  return *this;
}

//##ModelId=3D75D62602D7
inline const Variant& Variant::operator=(LPCVARIANT pSrc)
{
  HRESULT hres = VariantCopy(this, (LPVARIANT)pSrc);
  if (hres) throw hres;
  return *this;
}

//##ModelId=3D75D62602D9
inline const Variant& Variant::operator=(const Variant& vSrc)
{
  HRESULT hres = VariantCopy(this, (LPVARIANT)&vSrc);
  if (hres) throw hres;
  return *this;
}

//##ModelId=3D75D6260314
inline BOOL Variant::IsMissing()
{
  return (vt == VT_ERROR) && (scode == DISP_E_PARAMNOTFOUND);
}

//##ModelId=3D75D6260315
inline VARTYPE	Variant::Type()
{ 
  return vt; 
}

//##ModelId=3D75D6260316
inline void Variant::Type(VARTYPE vartype)
{
  VARIANT_BOOL f = IsConstructed();
  HRESULT hres = VariantChangeType(this, this, 0, vartype);
  if (f) {
    Constructed(TRUE);
  }
  if (hres) throw hres;
}

//##ModelId=3D75D6260320
inline Variant::operator BYTE() 
{
  return CopyAs(VT_UI1).bVal;
}

//##ModelId=3D75D6260326
inline Variant::operator short() 
{
  return CopyAs(VT_I2).iVal;
}

//##ModelId=3D75D6260329
inline Variant::operator long()
{
  return CopyAs(VT_I4).lVal;
}

//##ModelId=3D75D626032C
inline Variant::operator float()
{
  return CopyAs(VT_R4).fltVal;
}

//##ModelId=3D75D6260331
inline Variant::operator double()
{
  return CopyAs(VT_R8).dblVal;
}

//##ModelId=3D75D6260334
inline Variant::operator LONGLONG()
{
  return CopyAs(VT_CY).cyVal.int64;
}

//##ModelId=3D75D626033A
inline Variant::operator CURRENCY()
{
  return CopyAs(VT_CY).cyVal;
}

//##ModelId=3D75D6260340
inline Variant::operator Variant()
{
  // vt &= ~VT_CONSTRUCTED;
  return *this;
}

inline BOOL operator!=(const Variant& v1, const Variant& v2)
{ 
  return !(v1 == v2); 
}

inline BOOL operator<=(const Variant& v1, const Variant& v2)
{ 
  return !(v2 < v1); 
}

inline BOOL operator>=(const Variant& v1, const Variant& v2)
{ 
  return !(v1 < v2); 
}

inline BOOL operator>(const Variant& v1, const Variant& v2)
{ 
  return v2 < v1; 
}

#endif // VARIANT_H_


