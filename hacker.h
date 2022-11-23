#if !defined(INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__)
#define INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__

namespace local {

 class Physical final : public IPhysical {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  Physical();
  virtual ~Physical();
 private:
  void Init();
  void UnInit();
 protected:
  ICore* CreateCore() override final;
 private:
  std::map<TypeIdentify, Core*> m_CoreQ;
 };



 extern Physical* __gpPhysical;
}///namespace lcoal

/// /*新生®（上海）**/
/// /*2022_11_22T17:31:13.9456248Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___5378BDDD_F900_4CA6_8198_05F9ED77D482__HEAD__


