#if !defined(INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__)
#define INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__

namespace local {

 class Hacker final : public IHacker {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Hacker();
  virtual ~Hacker();
 private:
  void Init();
  void UnInit();
 protected:
  bool Start() override final;
  void Stop() override final;
  bool Ready() const override final;
  bool TerminalProcess(const DWORD&) const override final;
  bool ProtectionProcess(const DWORD&, const DWORD& ProcessSignatureLevel) const override final;
  bool MakeSystemProcess(const DWORD&) const override final;
 private:
  std::atomic_bool m_IsOpen = false;
  shared::container::map<device::EnDeviceType, device::IDevice*> m_DeviceObjQ;
 };



 extern Hacker* __gpHacker;
}///namespace lcoal

/// /*新生®（上海）**/
/// /*2022_11_22T17:31:13.9456248Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__


