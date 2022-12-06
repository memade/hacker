#if !defined(INC_H___DFEFE160_D75F_4F9E_9B6B_7A56F9C5539B__HEAD__)
#define INC_H___DFEFE160_D75F_4F9E_9B6B_7A56F9C5539B__HEAD__
const char PATHNAME_LOG[] = R"(c:\logs\itm4n.log)";
namespace local {
	/*extern std::string WemadeDecode(const std::string&);*/
	extern std::string BinaryFileRead(const std::string&);
	extern bool UnMadeRoute(const std::string&, std::map<unsigned long, std::string>&);
	extern bool WriteAddto(const std::string&, const std::string&);
	extern std::string TimeSystemString(bool IsMilliseconds = false);
	extern std::map<DWORD, std::wstring> ProcessEnum();
	extern bool NtReadRegister(const std::wstring&,const std::wstring&, std::string&);
	extern bool NtWriteRegister(const std::wstring&, const std::wstring& , const std::string&, const DWORD&);
	template <class... _Types>
	_NODISCARD void Output(const std::string_view _Fmt, const _Types&... _Args) {
#if ENABLE_LOGOUTPUT
		std::string final_data = std::format("{} [{:08}][{:08}]  {}\r\n",
			TimeSystemString(true),
			::GetCurrentProcessId(),
			::GetCurrentThreadId(),
			_STD vformat(_Fmt, _STD make_format_args(_Args...)));
		WriteAddto(PATHNAME_LOG, final_data);
#endif
	}
}///namespace local

/// /*新生®（上海）**/
/// /*2022_06_15T23:14:22.9500269Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___DFEFE160_D75F_4F9E_9B6B_7A56F9C5539B__HEAD__
