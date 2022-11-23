#if !defined(INC_H___D5C8E6D8_A211_4E14_93A4_CC0137C17848__HEAD__)
#define INC_H___D5C8E6D8_A211_4E14_93A4_CC0137C17848__HEAD__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void* __stdcall api_object_init(const void*, unsigned long);
__shared_api_ void __stdcall api_object_uninit();

/// /*新生®（上海）**/
/// /*2022_05_06T07:05:14.5964818Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___D5C8E6D8_A211_4E14_93A4_CC0137C17848__HEAD__