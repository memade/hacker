#include "stdafx.h"

namespace device {
 IDevice::IDevice(const EnDeviceType& type) : m_Type(type) {
  void** pDrivers = nullptr;
  do {
   DWORD size = 0;
   if (FALSE == ::EnumDeviceDrivers(nullptr, 0, &size))
    break;
   pDrivers = reinterpret_cast<void**>(::malloc(size));
   if (!pDrivers)
    break;
   if (FALSE == ::EnumDeviceDrivers(pDrivers, size, &size))
    break;
   m_KernelBaseAddress = reinterpret_cast<ULONG_PTR>(pDrivers[0]);
  } while (0);
  SK_DELETE_PTR_C(pDrivers);
  m_hNtoskrnl = ::LoadLibraryW(LR"(ntoskrnl.exe)");
 }
 IDevice::~IDevice() {
  SK_FREE_LIBRARY(m_hNtoskrnl);
 }
 const EnDeviceType& IDevice::Type() const {
  return m_Type;
 }
 bool IDevice::Start() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!ServiceStart())
    break;
   if (!Verify()) {
    if (!DeviceLoad())
     break;
   }
   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void IDevice::Stop() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   ServiceStop();
   if (m_DeviceNonLocalLoading.load())
    break;
   DeviceUnLoad();
  } while (0);
  m_IsOpen.store(false);
 }
 void IDevice::ServiceStop() const {
  if (shared::Win::Service::StopW(m_ServiceName, m_ServiceStopWaittimeMS.load()))
   shared::Win::Service::DeleteW(m_ServiceName);
 }
 bool IDevice::ServiceStart() {
  bool result = false;
  SC_HANDLE hSCManager = nullptr;
  SC_HANDLE hService = nullptr;
  do {
   if (m_IsOpen.load())
    break;
   if (DeviceLoad()) {
    m_DeviceNonLocalLoading.store(true);
    result = true;
    break;
   }
   if (m_DeviceDosName.empty())
    break;
   if (m_ServiceName.empty())
    break;
   if (m_ServiceDisplayName.empty())
    break;
   if (!shared::Win::AccessW(m_FileReleasePathname))
    break;
   hSCManager = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
   if (!hSCManager)
    break;
   if (SERVICE_RUNNING == shared::Win::Service::StatusW(m_ServiceName)) {
    m_IsOpen.store(true);
    break;
   }
   ServiceStop();

   result = true;

   for (int i = 0; i < 2; ++i) {
    bool __break = true;
    hService = ::OpenServiceW(hSCManager, m_ServiceName.c_str(), SERVICE_ALL_ACCESS);
    if (ERROR_SERVICE_DOES_NOT_EXIST == ::GetLastError() || !hService) {
     hService = ::CreateServiceW(hSCManager,
      m_ServiceName.c_str(),
      m_ServiceDisplayName.c_str(),
      SERVICE_ALL_ACCESS,
      SERVICE_KERNEL_DRIVER,
      SERVICE_DEMAND_START,
      SERVICE_ERROR_NORMAL,
      m_FileReleasePathname.c_str(), NULL, NULL, NULL, NULL, NULL);
     if (!hService)
      break;
     if (!kull_m_service_addWorldToSD(hService))
      break;
     if (!shared::Win::Service::StartW(m_ServiceName, INFINITE))
      break;
    }
    else {
     if (!shared::Win::Service::StartW(m_ServiceName, INFINITE)) {
      DWORD StartResult = ::GetLastError();
      if (StartResult == ERROR_SERVICE_DISABLED) {
       if (shared::Win::Service::StopW(m_ServiceName, 30000))
        shared::Win::Service::DeleteW(m_ServiceName);
       __break = false;
      }
      else if (StartResult == ERROR_SERVICE_ALREADY_RUNNING) {
       break;//!@ Start success.
      }
      else {
       //!@ Start failed.
       result = false;
      }
     }
    }
    if (__break)
     break;
    std::this_thread::sleep_for(std::chrono::seconds(1));
   }
  } while (0);
  SK_CLOSE_SERVICE_HANDLE(hService);
  SK_CLOSE_SERVICE_HANDLE(hSCManager);
  return result;
 }
 bool IDevice::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IsOpen.load() && Verify();
 }
 bool IDevice::Verify() const {
  return m_hDevice != nullptr && m_hDevice != INVALID_HANDLE_VALUE;
 }
 const std::string& IDevice::FileBuffer() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_FileBuffer;
 }
 const std::wstring& IDevice::FileReleasePathname() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_FileReleasePathname;
 }
 const std::wstring& IDevice::ServiceName() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_ServiceName;
 }
 const std::wstring& IDevice::ServiceDisplayName() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_ServiceDisplayName;
 }
 ULONG_PTR IDevice::GetSymbolOffset(const std::string& name) const {
  ULONG_PTR result = 0;
  do {
   if (name.empty())
    break;
   result = reinterpret_cast<ULONG_PTR>(\
    ::GetProcAddress(m_hNtoskrnl, name.c_str())) - reinterpret_cast<ULONG_PTR>(m_hNtoskrnl);
  } while (0);
  return result;
 }
 bool IDevice::kull_m_service_genericControl(const DWORD& dwDesiredAccess, const DWORD& dwControl, SERVICE_STATUS& serviceStatus) const {
  bool result = false;
  SC_HANDLE hSCManager = nullptr;
  SC_HANDLE hService = nullptr;
  do {
   hSCManager = ::OpenSCManagerW(NULL, SERVICES_ACTIVE_DATABASE, SC_MANAGER_CONNECT);
   if (!hSCManager)
    break;
   hService = ::OpenServiceW(hSCManager, m_ServiceName.c_str(), dwDesiredAccess);
   if (!hService)
    break;
   if (FALSE == ::ControlService(hService, dwControl, &serviceStatus) \
    && ::GetLastError() != ERROR_SERVICE_NOT_ACTIVE)
    break;
   result = true;
  } while (0);
  SK_CLOSE_SERVICE_HANDLE(hService);
  SK_CLOSE_SERVICE_HANDLE(hSCManager);
  return result;
 }
 bool IDevice::kull_m_service_addWorldToSD(const SC_HANDLE& hService) const {
  bool result = false;
  DWORD dwSizeNeeded = 0;
  PSECURITY_DESCRIPTOR oldSd{ nullptr }, newSd{ nullptr };
  SECURITY_DESCRIPTOR dummySdForXP = { 0 };
  SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
  EXPLICIT_ACCESS ForEveryOne =
  {
   SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG | SERVICE_INTERROGATE | \
   SERVICE_ENUMERATE_DEPENDENTS | SERVICE_PAUSE_CONTINUE | SERVICE_START | \
   SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL | READ_CONTROL,
   SET_ACCESS,
   NO_INHERITANCE,
   {NULL, NO_MULTIPLE_TRUSTEE, TRUSTEE_IS_SID, TRUSTEE_IS_WELL_KNOWN_GROUP, NULL}
  };
  do {
   void(::QueryServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, &dummySdForXP, 0, &dwSizeNeeded));
   if (ERROR_INSUFFICIENT_BUFFER == ::GetLastError()) {
    oldSd = (PSECURITY_DESCRIPTOR)::LocalAlloc(LPTR, dwSizeNeeded);
    if (FALSE == ::QueryServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, oldSd, dwSizeNeeded, &dwSizeNeeded))
     break;
   }
   if (FALSE == ::AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, (PSID*)&ForEveryOne.Trustee.ptstrName))
    break;
   if (ERROR_SUCCESS != ::BuildSecurityDescriptorW(NULL, NULL, 1, &ForEveryOne, 0, NULL, oldSd, &dwSizeNeeded, &newSd))
    break;
   if (FALSE == ::SetServiceObjectSecurity(hService, DACL_SECURITY_INFORMATION, newSd))
    break;
   result = true;
  } while (0);

  if (ForEveryOne.Trustee.ptstrName) {
   ::FreeSid(ForEveryOne.Trustee.ptstrName);
  }
  if (oldSd) {
   ::LocalFree(oldSd);
   oldSd = nullptr;
  }
  if (newSd) {
   ::LocalFree(newSd);
   newSd = nullptr;
  }
  return result;
 }
 bool IDevice::DeviceLoad() {
  m_hDevice = ::CreateFileW(m_DeviceDosName.c_str(),
   GENERIC_WRITE | GENERIC_READ,
   FILE_SHARE_READ | FILE_SHARE_WRITE,
   NULL,
   OPEN_EXISTING, 0, NULL);
  return Verify();
 }
 void IDevice::DeviceUnLoad() {
  SK_CLOSE_HANDLE(m_hDevice);
 }

#if 0
 //
// Supported process protection levels.
//

#define PROTECTION_LEVEL_WINTCB_LIGHT       0x00000000
#define PROTECTION_LEVEL_WINDOWS            0x00000001
#define PROTECTION_LEVEL_WINDOWS_LIGHT      0x00000002
#define PROTECTION_LEVEL_ANTIMALWARE_LIGHT  0x00000003
#define PROTECTION_LEVEL_LSA_LIGHT          0x00000004

//
// The following protection levels are supplied for testing only (no win32
// callers need these).
//

#define PROTECTION_LEVEL_WINTCB             0x00000005
#define PROTECTION_LEVEL_CODEGEN_LIGHT      0x00000006
#define PROTECTION_LEVEL_AUTHENTICODE       0x00000007
#define PROTECTION_LEVEL_PPL_APP            0x00000008

#define PROTECTION_LEVEL_SAME               0xFFFFFFFF

//
// The following is only used as a value for ProtectionLevel
// when querying ProcessProtectionLevelInfo in GetProcessInformation.
//
#define PROTECTION_LEVEL_NONE               0xFFFFFFFE
#endif

 const std::map<DWORD, DWORD> map_process_protection_levels__signature_level_value = {
  {PROTECTION_LEVEL_WINTCB_LIGHT,0x00610C3E},
  //{PROTECTION_LEVEL_WINDOWS,0x00000000},
  {PROTECTION_LEVEL_WINDOWS_LIGHT,0x00510C3C},
  {PROTECTION_LEVEL_ANTIMALWARE_LIGHT,0x00310737},
  {PROTECTION_LEVEL_LSA_LIGHT,0x0041083C},
  {PROTECTION_LEVEL_WINTCB,0x00720000},
  //{PROTECTION_LEVEL_CODEGEN_LIGHT,0x00000000},
  ////{PROTECTION_LEVEL_AUTHENTICODE,0x00000000},
  //{PROTECTION_LEVEL_PPL_APP,0x00000000},
  //{PROTECTION_LEVEL_SAME,0x00000000},
  //{PROTECTION_LEVEL_NONE,0x00000000},
 };
}///namespace device