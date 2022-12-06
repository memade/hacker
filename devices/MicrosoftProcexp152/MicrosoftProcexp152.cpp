#include "stdafx.h"
#include <res\PROCEXP152.SYS.res.h>

namespace device {

 static const DWORD IOCTL_OPEN_PPL_HANDLE = 0x8335003C;

 MicrosoftProcexp152::MicrosoftProcexp152() : IDevice(EnDeviceType::MicrosoftProcexp152) {
  Init();
 }
 MicrosoftProcexp152::~MicrosoftProcexp152() {
  UnInit();
 }
 void MicrosoftProcexp152::Release() const {
  delete this;
 }
 void MicrosoftProcexp152::Init() {
  m_DeviceDosName = LR"(\\.\PROCEXP152)";
  m_ServiceName = LR"(DeviceServiceProcexp)";
  m_ServiceDisplayName = LR"(DeviceServiceDisplayProcexp)";
  m_FileReleasePathname = shared::Win::GetTempPathW() + m_ServiceName + LR"(.sys)";
  m_FileBuffer = shared::Win::Encryption::WemadeDecode(std::string((char*)&PROCEXP152_SYS_res[0], sizeof(PROCEXP152_SYS_res)));
  do {
   if (shared::Win::AccessW(m_FileReleasePathname)) {
    if (::_wremove(m_FileReleasePathname.c_str()) != 0)
     break;
   }
   void(shared::Win::File::Write(shared::IConv::WStringToMBytes(m_FileReleasePathname), m_FileBuffer));
  } while (0);
 }
 void MicrosoftProcexp152::UnInit() {
  void(::_wremove(m_FileReleasePathname.c_str()));
 }

 bool MicrosoftProcexp152::OpenProcess(const DWORD& ProcessId, HANDLE& outHandle) const {
  outHandle = nullptr;
  bool result = false;
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  do {
   if (!Verify())
    break;
   if (ProcessId <= 4)
    break;
   DWORD bytes = 0;
   ULONGLONG openpid = ProcessId;
   if (FALSE == ::DeviceIoControl(m_hDevice, IOCTL_OPEN_PPL_HANDLE, &openpid, sizeof(openpid),
    &outHandle, sizeof(outHandle), &bytes, nullptr))
    break;
   result = true;
  } while (0);
  return result;
 }

}///namespace device