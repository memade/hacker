#if !defined(INC_H___23AB1C24_AB25_4EE3_BFA6_B79191D44144__HEAD__)
#define INC_H___23AB1C24_AB25_4EE3_BFA6_B79191D44144__HEAD__

namespace hacker {
 using TypeIdentify = unsigned long long;
 using tf_api_object_init = void* (__stdcall*)(const void*, unsigned long);
 using tf_api_object_uninit = void(__stdcall*)(void);

 class IHacker {
 public:
  virtual bool Start() = 0;
  virtual void Stop() = 0;
  virtual bool Ready() const = 0;
  virtual bool TerminalProcess(const DWORD&) const = 0;
  virtual bool ProtectionProcess(const DWORD&, const DWORD&) const = 0;
  virtual bool MakeSystemProcess(const DWORD&) const = 0;
 protected:
  void* hModule = nullptr;
  tf_api_object_init api_object_init = nullptr;
  tf_api_object_uninit api_object_uninit = nullptr;
 protected:
  inline IHacker();
  inline ~IHacker();
 public:
  inline static IHacker* CreateInterface(
   const char* pchacher_pe_pathname, const void* route, unsigned long nroute);
  inline static void DestoryInterface(IHacker*& pchacher_obj);
 };
 //////////////////////////////////////////////////////////////////////////////////////////
 inline IHacker::IHacker() {}
 inline IHacker::~IHacker() {}
 inline void IHacker::DestoryInterface(IHacker*& instance) {
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
 inline IHacker* IHacker::CreateInterface(const char* module_pathname, const void* param = nullptr, unsigned long size_param = 0) {
  IHacker* result = nullptr;
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