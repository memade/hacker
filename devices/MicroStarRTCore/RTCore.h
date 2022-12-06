#if !defined(INC_H___D6ABA34A_C00D_47F7_924F_943AC7B50F07__HEAD__)
#define INC_H___D6ABA34A_C00D_47F7_924F_943AC7B50F07__HEAD__

namespace device {
 /*https://github.com/RedCursorSecurityConsulting/PPLKiller
 * PPLKiller
 * Tool to bypass LSA Protection (aka Protected Process Light)
 * I’ve noticed there is a common misconception that LSA Protection prevents attacks that leverage
 * SeDebug or Administrative privileges to extract credential material from memory, like Mimikatz.
 * LSA Protection does NOT protect from these attacks, at best it makes them slightly more difficult as an extra step needs to be performed.
 * This https://github.com/wavestone-cdt/EDRSandblast does the same thing and is probably better.
 * This https://github.com/itm4n/PPLdump does the same thing without using a driver.
 */
 class RTCore final : public IDevice {
 public:
  RTCore();
  virtual ~RTCore();
 private:
  void Init();
  void UnInit();
 protected:
  bool ReadMemory(const ULONG_PTR&, const DWORD&, DWORD&) const override final;
  bool WriteMemory(const ULONG_PTR&, const DWORD&, const DWORD&) const override final;
  bool ReadMemoryDWORD64(const ULONG_PTR& address, DWORD64&) const override final;
  bool WriteMemoryDWORD64(const ULONG_PTR& address, const DWORD64& value) const override final;
  bool ReadMemoryDWORD(const ULONG_PTR& address, DWORD&) const override final;
  bool WriteMemoryDWORD(const ULONG_PTR& address, const DWORD& value) const override final;
  bool WriteMemoryWORD(const ULONG_PTR& address, const WORD& value) const override final;
  bool ReadMemoryWORD(const ULONG_PTR& address, WORD&) const override final;
  bool GetProcessAddress(const DWORD& dwProcessId, ULONG_PTR&) const override final;
  bool GetProcessTokenAddress(const DWORD& dwProcessId, ULONG_PTR&) const override final;
  bool GetTokenValueOfSystemProcess(DWORD64&) const override final;
  bool GetProcessSignatureLevelAddress(const DWORD& dwProcessId,ULONG_PTR&) const override final;
 public:
  void Release() const override final;
  bool MakeSystemProcess(const DWORD& ProcessId) const override final;
  bool ProcessProtectionAction(const DWORD& ProcessId, const bool& enable, const DWORD& ProcessProtectionLevel = 0) override final;
 private:
  //!@ System process original value backup.
  std::map<DWORD, DWORD> m_ProcessProtectionActionCaches;
 };

#if 0
 class RTCore final : IDrive {
 public:
  RTCore();
  virtual ~RTCore();
 private:
  void Init();
  void UnInit();
  bool InstallService();
  bool UninstallService();
 public:
  bool Start() override final;
  void Stop() override final;
  bool PPLProtectedAction(const DWORD& ProcessId, const bool& enable = true) const;
 };
#endif


}///namespace device


/// /*新生®（上海）**/
/// /*2022_11_30T14:35:44.6030588Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___D6ABA34A_C00D_47F7_924F_943AC7B50F07__HEAD__