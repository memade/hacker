#if !defined(INC_H___3E9C18CF_1A66_49CD_A4A1_4C8E935FAD22__HEAD__)
#define INC_H___3E9C18CF_1A66_49CD_A4A1_4C8E935FAD22__HEAD__

namespace local {

 class Core final : public ICore {
  std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
 public:
  class Config final : public ICoreConfig {
   friend class Core;
   std::shared_ptr<std::mutex> m_Mutex = std::make_shared<std::mutex>();
  public:
   const unsigned long long& UnitOfTime() const override final;
   void UnitOfTime(const unsigned long long&) override final;
   const unsigned long long& UnitOfFrame() const override final;
   void UnitOfFrame(const unsigned long long&) override final;
  private:
   //!@ Physical min unit.[pixel]
   unsigned long long m_UnitOfFrame = 1;
   //!@ Time min unit.[microseconds]
   unsigned long long m_UnitOfTime = 1;
  private:
   Config();
   virtual ~Config();
  };
 public:
  Core();
  virtual ~Core();
 private:
  void Init();
  void UnInit();
 public:
  const TypeIdentify& Identify() const override final;
  ICoreConfig* ConfigGet() const override final;
  void Release() const override final;
 private:
  const TypeIdentify m_Identify;
  Config* m_pConfig = nullptr;
 };

}///namespace lcoal


/// /*新生®（上海）**/
/// /*2022_11_22T16:42:16.1880697Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___3E9C18CF_1A66_49CD_A4A1_4C8E935FAD22__HEAD__
