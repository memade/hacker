#if !defined(__192A5938_5662_4708_A98C_CAD06DC9E14A__)
#define __192A5938_5662_4708_A98C_CAD06DC9E14A__

namespace device {

 class IDevice {
 protected:
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  IDevice(const EnDeviceType&);
  ~IDevice();
 public:
  virtual bool Start();
  virtual void Stop();
  virtual bool Ready() const;
  virtual void Release() const = 0;
  virtual const EnDeviceType& Type() const;
  virtual const std::string& FileBuffer() const;
  virtual const std::wstring& FileReleasePathname() const;
  virtual const std::wstring& ServiceName() const;
  virtual const std::wstring& ServiceDisplayName() const;
  virtual bool OpenProcess(const DWORD&, HANDLE&) const { return false; }
  virtual bool MakeSystemProcess(const DWORD& ProcessId) const { return false; }
  virtual bool ProcessProtectionAction(const DWORD& ProcessId, const bool& enable, const DWORD& ProcessProtectionLevel = 0) { return false; }
 protected:
  virtual bool Verify() const;
  virtual bool DeviceLoad();
  virtual void DeviceUnLoad();
  virtual bool ServiceStart();
  virtual void ServiceStop() const;
  virtual ULONG_PTR GetSymbolOffset(const std::string&) const;
  virtual bool GetProcessAddress(const DWORD& dwProcessId, ULONG_PTR&) const { return false; }
  virtual bool GetProcessTokenAddress(const DWORD& dwProcessId, ULONG_PTR&) const { return false; }
  virtual bool GetTokenValueOfSystemProcess(DWORD64&) const { return false; }
  virtual bool GetProcessSignatureLevelAddress(const DWORD& dwProcessId, ULONG_PTR&) const { return false; }
  virtual bool kull_m_service_addWorldToSD(const SC_HANDLE&) const;
  virtual bool kull_m_service_genericControl(const DWORD&, const DWORD&, SERVICE_STATUS&) const;
  virtual bool ReadMemory(const ULONG_PTR&, const DWORD&,DWORD&) const { return false; }
  virtual bool WriteMemory(const ULONG_PTR&, const DWORD&, const DWORD&) const { return false; }
  virtual bool ReadMemoryDWORD64(const ULONG_PTR& address, DWORD64&) const { return false; }
  virtual bool WriteMemoryDWORD64(const ULONG_PTR& address, const DWORD64& value) const { return false; }
  virtual bool ReadMemoryDWORD(const ULONG_PTR& address, DWORD&) const { return false; }
  virtual bool WriteMemoryDWORD(const ULONG_PTR& address, const DWORD& value) const { return false; }
  virtual bool WriteMemoryWORD(const ULONG_PTR& address, const WORD& value) const { return false; }
  virtual bool ReadMemoryWORD(const ULONG_PTR& address,WORD&) const { return false; }
 protected:
  std::atomic_bool m_DeviceNonLocalLoading = false;
  std::atomic_bool m_IsOpen = false;
  std::string m_FileBuffer;
  std::wstring m_FileReleasePathname;
  std::wstring m_ServiceName;
  std::wstring m_ServiceDisplayName;
  std::wstring m_DeviceDosName;
  HANDLE m_hDevice = nullptr;
  HMODULE m_hNtoskrnl = nullptr;
  ULONG_PTR m_KernelBaseAddress = 0;
  std::atomic_uint64_t m_ServiceStopWaittimeMS = 3000;
  const EnDeviceType m_Type;
 };

 //!@ As usual and supported process protection levels.
 extern const std::map<DWORD, DWORD> map_process_protection_levels__signature_level_value;
}///namespace device

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 02 Dec 2022 14:11:21 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__192A5938_5662_4708_A98C_CAD06DC9E14A__
