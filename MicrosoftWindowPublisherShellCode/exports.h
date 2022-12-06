#if !defined(INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__)
#define INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void __stdcall api_object_init(const void*, unsigned long);
__shared_api_ void __stdcall api_object_uninit();

/// /*新生®（上海）**/
/// /*2022_11_24T05:13:51.6300689Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__
