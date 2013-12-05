  #include   "stdafx.h"   
#include "bohao.h"
  #include   <raserror.h>   
   #include<ras.h>
#pragma comment(lib, "Rasapi32.lib") 

#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include<wininet.h>
#pragma comment(lib,"wininet.lib") 
  CRasEntry::CRasEntry()   
  {   
  }   
    
  CRasEntry::~CRasEntry()   
  {   
  }   
    
  //   Retrieves   country-specific   dialing   information   from   the   Windows   Telephony   list   of   countries.   
  //   pass   the   Country   ID   (e.g.   1   =   USA).     
  //   Returns   country   code   for   the   country   identified   by   the   dwCountryID.   
  DWORD   CRasEntry::GetCountryInfo(DWORD   dwCID,   RASCTRYINFO   &RasCTryInfo,   char   *szCountryName)   
  {   
  LPRASCTRYINFO   lpRasCTryInfo   =   NULL;   
          DWORD   cb   =   sizeof(RASCTRYINFO);   
  DWORD   dwBufferSize   =   0;   
  DWORD   dwRet;   
  DWORD   dwCountryID   =   0;   
    
  lpRasCTryInfo   =   (LPRASCTRYINFO)HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,   cb);   
  if(lpRasCTryInfo   ==   NULL)   
  return   0;   
    
  lpRasCTryInfo->dwSize   =   sizeof(RASCTRYINFO);   
  lpRasCTryInfo->dwCountryID   =   dwCID;   
    
  dwRet   =   RasGetCountryInfo(lpRasCTryInfo,   &dwBufferSize);   
    
  if(dwRet   ==   ERROR_BUFFER_TOO_SMALL)   
  {   
  if(HeapFree(GetProcessHeap(),   0,   (LPVOID)lpRasCTryInfo))   //   Free   initial   buffer   
                  {   
  //   And   reassign   a   new   buffer   with   the   value   returned   in   dwBufferSize   
  lpRasCTryInfo   =   (LPRASCTRYINFO)HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,   dwBufferSize);   
  if   (NULL   ==   lpRasCTryInfo)   
  return   0;   
            
  lpRasCTryInfo->dwSize   =   sizeof(RASCTRYINFO);   
  lpRasCTryInfo->dwCountryID   =   dwCID;   
    
  //   Again   call   RasGetCountryInfo   
  dwRet   =   RasGetCountryInfo(lpRasCTryInfo,   &dwBufferSize);   
  if   (dwRet   ==   ERROR_SUCCESS)   
  {   
  //   Store   the   necessary   info.   
  ::memcpy(&RasCTryInfo,   lpRasCTryInfo,   sizeof(RASCTRYINFO));   
  ::strcpy(szCountryName,   (char   *)lpRasCTryInfo   +   lpRasCTryInfo->dwCountryNameOffset);   
  dwCountryID   =   lpRasCTryInfo->dwCountryID;   
  }   
    
    HeapFree(GetProcessHeap(),   0,   (LPVOID)lpRasCTryInfo);   
  }   
  else   
  return   0;   
  }   
    
  return   dwCountryID;   
  }   
    
  BOOL   CRasEntry::CreateRasEntry(CString   strEntryName,   RASENTRY   &RasEntry)   
  {   
  LPRASENTRY   lpRasEntry   =   NULL;   
          DWORD   cb   =   sizeof(RASENTRY);   
  DWORD   dwRet;   
  DWORD   dwBufferSize   =   0;   
    
//     This   is   important!   Find   the   buffer   size   (different   from   sizeof(RASENTRY)).   
  RasGetEntryProperties(NULL,   "",   NULL,   &dwBufferSize,   NULL,   NULL);   
  if(dwBufferSize   ==   0)   
  return   FALSE;   
    
  lpRasEntry   =   (LPRASENTRY)HeapAlloc(GetProcessHeap(),   HEAP_ZERO_MEMORY,   dwBufferSize);   
  if   (lpRasEntry   ==   NULL)   
  return   FALSE;   
    
  lpRasEntry->dwSize   =   dwBufferSize;   
  lpRasEntry->dwfOptions   =   RasEntry.dwfOptions;   
   lpRasEntry->dwfOptions2   =   RasEntry.dwfOptions2;
  lpRasEntry->dwCountryID   =   RasEntry.dwCountryID;   
  lpRasEntry->dwCountryCode   =   RasEntry.dwCountryCode;   
  ::strcpy(lpRasEntry->szAreaCode,   RasEntry.szAreaCode);   
  ::strcpy(lpRasEntry->szLocalPhoneNumber,   RasEntry.szLocalPhoneNumber);   
  ::memcpy(&lpRasEntry->ipaddr,   &RasEntry.ipaddr,   sizeof(CIPAddressCtrl));   
  ::memcpy(&lpRasEntry->ipaddrDns,   &RasEntry.ipaddrDns,   sizeof(CIPAddressCtrl));   
  ::memcpy(&lpRasEntry->ipaddrDnsAlt,   &RasEntry.ipaddrDnsAlt,   sizeof(CIPAddressCtrl));   
  ::memcpy(&lpRasEntry->ipaddrWins,   &RasEntry.ipaddrWins,   sizeof(CIPAddressCtrl));   
  ::memcpy(&lpRasEntry->ipaddrWinsAlt,   &RasEntry.ipaddrWinsAlt,   sizeof(CIPAddressCtrl));   
  lpRasEntry->dwfNetProtocols   =   RASNP_Ip;   
  lpRasEntry->dwFramingProtocol   =   RASFP_Ras;   
  
  ::strcpy(lpRasEntry->szDeviceType,   RasEntry.szDeviceType);   
  ::strcpy(lpRasEntry->szDeviceName,   RasEntry.szDeviceName);   
    
dwRet   =   RasSetEntryProperties(NULL,strEntryName.GetBuffer(strEntryName.GetLength()),   
lpRasEntry,   dwBufferSize,   NULL,   0);   
    
  HeapFree(GetProcessHeap(),   0,   (LPVOID)lpRasEntry);   
    
  if(dwRet   ==   0)   
  return   TRUE;   
  else   
  return   FALSE;   
  }   
    
  //   Retrieves   all   the   available   RAS-capable   devices.   
  //   Specify   the   device   type   in   szDeviceType   from   one   of   these:   
  //   RASDT_Modem,   rasDT_Isdn,   rasDT_X25,   rasDT_Vpn,   rasDT_Pad,   rasDT_Generic,     
  //   rasDT_Serial,   rasDT_FrameRelay,   rasDT_Atm,   rasDT_Sonet,   rasDT_SW56,     
  //   rasDT_Irda,   rasDT_Parallel,   rasDT_PPPoE   
  //   
  //   This   returns   name   of   all   the   available   devices   in   strDevArray.   
  //   If   there   are   no   devices   available,   it   returns   FALSE.   
  BOOL   CRasEntry::EnumModem(char   *szDeviceType,   CStringArray   &strDevArray)   
  {   
  DWORD   i                   =   0;   
  DWORD   dwRet           =   0;   
  DWORD   dwcb             =   0;   
  DWORD   dwDevices   =   0;   
  LPRASDEVINFO   lpRasDevInfo   =   NULL;   
  BOOL   bRet   =   FALSE;   
    
  //   allocate   buffer   for   one   device   
  dwcb   =   sizeof(RASDEVINFO);   
  lpRasDevInfo   =   (LPRASDEVINFO)   malloc((UINT)dwcb);   
  lpRasDevInfo->dwSize   =   dwcb;   
    
  //   RasEnumDevices   
  dwRet   =   RasEnumDevices(lpRasDevInfo,   &dwcb,   &dwDevices);   
  if(dwRet   ==   ERROR_BUFFER_TOO_SMALL   &&   dwDevices   !=   0)     
  {   
  //   found   more   then   one   device(s)   
  lpRasDevInfo   =   (LPRASDEVINFO)   realloc((VOID*)lpRasDevInfo,   (UINT)dwcb);   
  for   (i   =   0;   i   <   dwDevices;   i++)     
  {   
  lpRasDevInfo[i].dwSize   =   sizeof(RASDEVINFO);   
  }   
  dwRet   =   RasEnumDevices(lpRasDevInfo,   &dwcb,   &dwDevices);   
  }   
    
  if(dwRet   ==   0)     
  {   
  for(i   =   0;   i   <   dwDevices;   i++)     
  {   
  if(_strnicmp(lpRasDevInfo[i].szDeviceType,   szDeviceType,   strlen(lpRasDevInfo[i].szDeviceType))   ==   0)   
  {   
  strDevArray.Add(lpRasDevInfo[i].szDeviceName);   
  bRet   =   TRUE;   
  }   
  }   
  }   
  free   ((VOID*)lpRasDevInfo);   
    
  return   bRet;   
  }   
    
  //   Saves   the   user   info   (username,   password)   
  //   Set   bRemovePassword   to   TRUE   if   you   don't   want   the   password   to   be   saved.   
  BOOL   CRasEntry::SetEntryDialParams(CString   strEntryName,   CString   strUsername,   CString   strPassword,   BOOL   bRemovePassword)   
  {   
  RASDIALPARAMS   rdParams;   
  ZeroMemory(&rdParams,   sizeof(RASDIALPARAMS));   
  rdParams.dwSize   =   sizeof(RASDIALPARAMS);   
  ::strcpy(rdParams.szEntryName,   strEntryName.GetBuffer(strEntryName.GetLength()));   
  ::strcpy(rdParams.szUserName,   strUsername.GetBuffer(strUsername.GetLength()));   
  ::strcpy(rdParams.szPassword,   strPassword.GetBuffer(strPassword.GetLength()));   
  DWORD   dwRet   =   RasSetEntryDialParams(NULL,   &rdParams,   bRemovePassword);   
    
  if(dwRet   ==   0)   
  return   TRUE;   
  else     
  return   FALSE;   
  }   
