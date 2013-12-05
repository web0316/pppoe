#ifndef   __RASENTRY_H__   
  #define   __RASENTRY_H__   
    
  #include   <ras.h>   
    
  class   CRasEntry   
  {   
  public:   
  BOOL   SetEntryDialParams(CString   strEntryName,   CString   strUsername,   CString   strPassword,   BOOL   bRemovePassword);   
  BOOL   EnumModem(char   *szDeviceType,   CStringArray   &strDevArray);   
  BOOL   CreateRasEntry(CString   strEntryName,   RASENTRY   &RasEntry);   
  DWORD   GetCountryInfo(DWORD   dwCID,   RASCTRYINFO   &RasCTryInfo,   char   *szCountryName);   
  CRasEntry();   
  virtual   ~CRasEntry();   
  };   
    
  #endif   __RASENTRY_H__   


