#if !defined(INC_H___23AB1C24_AB25_4EE3_BFA6_B79191D44144__HEAD__)
#define INC_H___23AB1C24_AB25_4EE3_BFA6_B79191D44144__HEAD__

namespace physical {
 using TypeIdentify = unsigned long long;
 using tf_api_object_init = void* (__stdcall*)(const void*, unsigned long);
 using tf_api_object_uninit = void(__stdcall*)(void);

 class ICoreConfig {
 public:
  virtual const unsigned long long& UnitOfTime() const = 0;
  virtual void UnitOfTime(const unsigned long long&) = 0;
  virtual const unsigned long long& UnitOfFrame() const = 0;
  virtual void UnitOfFrame(const unsigned long long&) = 0;
 };
 class ICore {
 public:
  virtual const TypeIdentify& Identify() const = 0;
  virtual ICoreConfig* ConfigGet() const = 0;
  virtual void Release() const = 0;
 };

 class IPhysical {
 public:
  virtual ICore* CreateCore() = 0;





 protected:
  void* hModule = nullptr;
  tf_api_object_init api_object_init = nullptr;
  tf_api_object_uninit api_object_uninit = nullptr;
 protected:
  inline IPhysical();
  inline ~IPhysical();
 public:
  inline static IPhysical* CreateInterface(
   const char* pchacher_pe_pathname, const void* route, unsigned long nroute);
  inline static void DestoryInterface(IPhysical*& pchacher_obj);
 };
 //////////////////////////////////////////////////////////////////////////////////////////
 inline IPhysical::IPhysical() {}
 inline IPhysical::~IPhysical() {}
 inline void IPhysical::DestoryInterface(IPhysical*& instance) {
  do {
   if (!instance)
    break;
   if (!instance->hModule || !instance->api_object_uninit)
    break;
   HMODULE freeMod = reinterpret_cast<HMODULE>(instance->hModule);
   instance->api_object_uninit();
   instance = nullptr;
   ::FreeLibrary(freeMod);
   freeMod = nullptr;
  } while (0);
 }
 inline IPhysical* IPhysical::CreateInterface(const char* module_pathname, const void* param = nullptr, unsigned long size_param = 0) {
  IPhysical* result = nullptr;
  HMODULE hModule = nullptr;
  do {
   if (!module_pathname)
    break;
   hModule = ::LoadLibraryA(module_pathname);
   if (!hModule)
    break;
   auto api_object_init = reinterpret_cast<tf_api_object_init>(::GetProcAddress(hModule, "api_object_init"));
   auto api_object_uninit = reinterpret_cast<tf_api_object_uninit>(::GetProcAddress(hModule, "api_object_uninit"));
   if (!api_object_init || !api_object_uninit)
    break;
   result = reinterpret_cast<decltype(result)>(api_object_init(param, size_param));
   if (!result)
    break;
   result->hModule = hModule;
   result->api_object_init = api_object_init;
   result->api_object_uninit = api_object_uninit;
  } while (0);
  if (nullptr == result && hModule != nullptr) {
   ::FreeLibrary(hModule);
   hModule = nullptr;
  }
  return result;
 }
}///namespace physical


/// /*新生®（上海）**/
/// /*2022_11_22T16:38:16.0805856Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___23AB1C24_AB25_4EE3_BFA6_B79191D44144__HEAD__