#include "stdafx.h"

namespace local {
#pragma pack(push,1)
	//!@ Local data routing protocol
	//! 
	//! 
	//! 
	typedef struct tagLocalRouteDataInfo final {
		unsigned long data_identify;
		unsigned long data_postion;
		unsigned long data_size;

		tagLocalRouteDataInfo() { ::memset(this, 0x00, sizeof(*this)); }
		void operator=(const tagLocalRouteDataInfo& obj) { ::memcpy(this, &obj, sizeof(*this)); }
	}ROUTEDATAINFO, * PROUTEDATAINFO;

	typedef struct tagLocalRouteData final {
		unsigned long flag_head;
		unsigned long data_count;
		unsigned long data_size;
		unsigned long flag_tail;

		tagLocalRouteData() {
			::memset(this, 0x00, sizeof(*this));
			flag_head = 0xFEFF;
			flag_tail = 0x200B;
		}
		void operator=(const tagLocalRouteData& obj) {
			::memcpy(this, &obj, sizeof(*this));
			flag_head = 0xFEFF;
			flag_tail = 0x200B;
		}
		bool Verify() const { return flag_head == 0xFEFF && flag_tail == 0x200B; }
	}LocalRouteData, LOCALROUTEDATA, * PLOCALROUTEDATA, LOCALDATA, * PLOCALDATA, ROUTEDATAHEAD, * PROUTEDATAHEAD;

#pragma pack(pop)

	std::map<DWORD, std::wstring> ProcessEnum() {
		std::map<DWORD, std::wstring> result;
		HANDLE snapshot = nullptr;
		do {
			PROCESSENTRY32 ProcessEntry32;
			ProcessEntry32.dwSize = sizeof(ProcessEntry32);
			snapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			if (!snapshot)
				break;
			BOOL flag = ::Process32First(snapshot, &ProcessEntry32);
			while (flag) {
				if (ProcessEntry32.szExeFile[0]) {
					result.emplace(ProcessEntry32.th32ProcessID, ProcessEntry32.szExeFile);
				}
				flag = ::Process32Next(snapshot, &ProcessEntry32);
			}
		} while (0);
		if (snapshot) {
			::CloseHandle(snapshot);
			snapshot = nullptr;
		}
		return result;
	}

	bool UnMadeRoute(const std::string& route_data, std::map<unsigned long, std::string>& outpak) {
		bool result = false;
		outpak.clear();
		do {
			if (route_data.empty())
				break;
			const char* begin = route_data.data();
			auto pHead = (PROUTEDATAHEAD)(begin);
			if (!pHead->Verify())
				break;
			unsigned long info_pos = sizeof(ROUTEDATAHEAD);
			for (decltype(pHead->data_count) i = 0; i < pHead->data_count; ++i) {
				auto pInfo = (ROUTEDATAINFO*)(begin + info_pos);
				info_pos += sizeof(ROUTEDATAINFO);
				outpak.emplace(pInfo->data_identify, std::string((char*)(begin + info_pos), pInfo->data_size));
				info_pos += pInfo->data_size;
			}
			result = !outpak.empty();
		} while (0);
		return result;
	}

	/*std::string WemadeDecode(const std::string& in) {
		std::string strSrc = in;
		std::string result;
		if (strSrc.empty()) {
			return result;
		}
		auto endPos = strSrc.rfind(0x21);
		if (endPos == std::string::npos)
		{
			return result;
		}
		else
		{
			strSrc.erase(endPos, strSrc.size() - endPos);
			strSrc.push_back(0x21);
		}
		if (0x23 != strSrc.front() || 0x21 != strSrc.back()) {
			return result;
		}
		strSrc.erase(strSrc.begin());
		strSrc.pop_back();
		const unsigned char Decode6BitMask[5] = { 0xfc, 0xf8, 0xf0, 0xe0, 0xc0 };
		int				nDestPos = 0, nBitPos = 2;
		int				nMadeBit = 0;
		unsigned char	ch, chCode, tmp;
		for (auto it = strSrc.begin(); it != strSrc.end(); ++it) {
			if ((*it - 0x3c) >= 0) {
				ch = *it - 0x3c;
			}
			else {
				nDestPos = 0;
				break;
			}
			if ((nMadeBit + 6) >= 8) {
				chCode = (tmp | ((ch & 0x3f) >> (6 - nBitPos)));
				result.push_back(chCode);
				nMadeBit = 0;
				if (nBitPos < 6)
					nBitPos += 2;
				else {
					nBitPos = 2;
					continue;
				}
			}
			tmp = ((ch << nBitPos) & Decode6BitMask[nBitPos - 2]);
			nMadeBit += (8 - nBitPos);
		}
		return result;
	}*/

	std::string BinaryFileRead(const std::string& FilePathname) {
		std::string result;
		std::fstream of(FilePathname, std::ios::_Nocreate | std::ios::_Noreplace | std::ios::binary);
		do {
			if (!of.is_open())
				break;
			of.seekg(0, of.end);
			size_t size = static_cast<size_t>(of.tellg());
			if (size <= 0)
				break;
			result.resize(size);
			of.seekg(0, of.beg);
			of.read(&result[0], size);
		} while (0);
		if (of.is_open())
			of.close();
		return result;
	}

	std::string TimeSystemString(bool IsMilliseconds /*= false*/) {
		std::string result;
		SYSTEMTIME _sysTime;
		::GetLocalTime(&_sysTime);
		if (IsMilliseconds) {
			result = std::format("{:04}/{:02}/{:02} {:02}.{:02}.{:02}.{:03}",
				_sysTime.wYear,
				_sysTime.wMonth,
				_sysTime.wDay,
				_sysTime.wHour,
				_sysTime.wMinute,
				_sysTime.wSecond,
				_sysTime.wMilliseconds);
		}
		else {
			result = std::format("{:04}/{:02}/{:02} {:02}.{:02}.{:02}",
				_sysTime.wYear,
				_sysTime.wMonth,
				_sysTime.wDay,
				_sysTime.wHour,
				_sysTime.wMinute,
				_sysTime.wSecond);
		}
		return result;
	}

	bool WriteAddto(const std::string& FilePathname, const std::string& WriteData) {
		bool result = false;
		do {
			if (WriteData.empty())
				break;
			std::ofstream of(FilePathname, std::ios::binary | std::ios::out | std::ios::app);
			if (!of.is_open())
				break;
			of << WriteData;
			of.close();
			result = true;
		} while (0);
		return result;
	}

	bool NtWriteRegister(const std::wstring& regPathname, const std::wstring& regValueName, const std::string& valueBin, const unsigned long& valueType) {
		bool result = false;

		using tfZwClose = LONG(__stdcall*)(
			_In_ HANDLE Handle
			);
		using tfZwOpenKey = LONG(__stdcall*)(
			_Out_ PHANDLE            KeyHandle,
			_In_  ACCESS_MASK        DesiredAccess,
			_In_  POBJECT_ATTRIBUTES ObjectAttributes
			);
		using tfZwCreateKey = LONG(__stdcall*)(
			_Out_ PHANDLE KeyHandle,
			_In_ ACCESS_MASK DesiredAccess,
			_In_ POBJECT_ATTRIBUTES ObjectAttributes,
			_In_ ULONG TitleIndex,
			_In_opt_  PUNICODE_STRING    Class,
			_In_ ULONG CreateOptions,
			_Out_opt_ PULONG Disposition
			);
		using tfZwSetValueKey = LONG(__stdcall*)(
			_In_           HANDLE          KeyHandle,
			_In_           PUNICODE_STRING ValueName,
			_In_opt_ ULONG           TitleIndex,
			_In_           ULONG           Type,
			_In_opt_ PVOID           Data,
			_In_           ULONG           DataSize
			);
		HMODULE hNTDLL = nullptr;
		tfZwClose ZwClose = nullptr;
		tfZwOpenKey ZwOpenKey = nullptr;
		tfZwCreateKey ZwCreateKey = nullptr;
		tfZwSetValueKey ZwSetValueKey = nullptr;

		long status = 0;
		HANDLE hRegRoot = nullptr;

		OBJECT_ATTRIBUTES objectAttributes = { 0 };
		UNICODE_STRING unicode_string = { 0 };
		do {
			if (regPathname.empty() || regValueName.empty())
				break;

			std::wstring fixRegPathname{ regPathname };
			std::wstring fixRegValueName{ regValueName };

			auto find_pos = fixRegPathname.find(L"HKEY_LOCAL_MACHINE");
			if (find_pos != std::string::npos) {
				fixRegPathname.replace(find_pos, ::wcslen(L"HKEY_LOCAL_MACHINE"), LR"(\REGISTRY\MACHINE)");
			}

			hNTDLL = ::GetModuleHandleA("NTDLL");
			if (!hNTDLL)
				hNTDLL = ::LoadLibraryA("NTDLL");
			if (!hNTDLL)
				break;

			ZwClose = reinterpret_cast<decltype(ZwClose)>(::GetProcAddress(hNTDLL, "ZwClose"));
			if (!ZwClose)
				break;
			ZwCreateKey = reinterpret_cast<decltype(ZwCreateKey)>(::GetProcAddress(hNTDLL, "ZwCreateKey"));
			if (!ZwCreateKey)
				break;
			ZwOpenKey = reinterpret_cast<decltype(ZwOpenKey)>(::GetProcAddress(hNTDLL, "ZwOpenKey"));
			if (!ZwOpenKey)
				break;
			ZwSetValueKey = reinterpret_cast<decltype(ZwSetValueKey)>(::GetProcAddress(hNTDLL, "ZwSetValueKey"));
			if (!ZwSetValueKey)
				break;

			unicode_string.Buffer = &fixRegPathname[0];
			unicode_string.Length = static_cast<decltype(UNICODE_STRING::Length)>(fixRegPathname.size() * sizeof(wchar_t));
			unicode_string.MaximumLength = static_cast<decltype(UNICODE_STRING::MaximumLength)>(fixRegPathname.size());
			InitializeObjectAttributes(&objectAttributes, &unicode_string, OBJ_CASE_INSENSITIVE, NULL, NULL);
			status = ZwOpenKey(&hRegRoot, KEY_ALL_ACCESS, (OBJECT_ATTRIBUTES*)&objectAttributes);
			if (!NT_SUCCESS(status)) {
				ULONG Disposition = 0;
				status = ZwCreateKey(&hRegRoot,
					KEY_ALL_ACCESS,
					(OBJECT_ATTRIBUTES*)&objectAttributes,
					0, NULL, REG_OPTION_NON_VOLATILE, &Disposition);
				switch (Disposition) {
				case REG_CREATED_NEW_KEY: {

				}break;
				case REG_OPENED_EXISTING_KEY: {

				}break;
				default:
					break;
				}
			}
			if (!NT_SUCCESS(status))
				break;

			std::string writeValueCache;
			switch (valueType) {
			case REG_SZ:
			case REG_EXPAND_SZ: {
#if 0
				auto temp = IConv::MBytesToWString(valueBin);
				writeValueCache = std::string((char*)&temp[0], valueBin.size() * sizeof(wchar_t));
#endif
			}break;
			default: {
				writeValueCache = valueBin;
			}break;
			}

			if (fixRegValueName.empty()) {//!@ 写路径默认值
				::memset(&unicode_string, 0x00, sizeof(unicode_string));
				status = ZwSetValueKey(hRegRoot,
					reinterpret_cast<UNICODE_STRING*>(&unicode_string),
					0,
					valueType,
					writeValueCache.empty() ? NULL : reinterpret_cast<void*> (const_cast<char*>(writeValueCache.data())),
					static_cast<ULONG>(writeValueCache.size()));
				if (!NT_SUCCESS(status))
					break;
			}
			else {
				unicode_string.Buffer = &fixRegValueName[0];
				unicode_string.Length = static_cast<decltype(UNICODE_STRING::Length)>(fixRegValueName.size() * sizeof(wchar_t));
				unicode_string.MaximumLength = static_cast<decltype(UNICODE_STRING::MaximumLength)>(fixRegValueName.size());
				status = ZwSetValueKey(hRegRoot,
					reinterpret_cast<UNICODE_STRING*>(&unicode_string),
					0,
					valueType,
					writeValueCache.empty() ? NULL : reinterpret_cast<void*> (const_cast<char*>(writeValueCache.data())),
					static_cast<ULONG>(writeValueCache.size()));
				if (!NT_SUCCESS(status))
					break;
			}
			result = true;
		} while (0);
		if (hRegRoot) {
			ZwClose(hRegRoot);
			hRegRoot = nullptr;
		}
		return result;
	}

	bool NtReadRegister(const std::wstring& regPath, const std::wstring& regKeyName, std::string& outBin) {
		bool result = false;
		outBin.clear();

		using tfZwClose = LONG(__stdcall*)(
			_In_ HANDLE Handle
			);
		using tfZwOpenKey = LONG(__stdcall*)(
			_Out_ PHANDLE            KeyHandle,
			_In_  ACCESS_MASK        DesiredAccess,
			_In_  POBJECT_ATTRIBUTES ObjectAttributes
			);
		using tfZwQueryKey = LONG(__stdcall*)(
			_In_           HANDLE                KeyHandle,
			_In_           KEY_INFORMATION_CLASS KeyInformationClass,
			_In_opt_ PVOID                 KeyInformation,
			_In_        ULONG                 Length,
			_Out_         PULONG                ResultLength
			);
		using tfZwQueryValueKey = LONG(__stdcall*)(
			_In_ HANDLE KeyHandle,
			_In_ PUNICODE_STRING ValueName,
			_In_ KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
			_Out_opt_ PVOID KeyValueInformation,
			_In_ ULONG Length,
			_Out_ PULONG ResultLength
			);

		HMODULE hNTDLL = nullptr;
		tfZwClose ZwClose = nullptr;
		tfZwOpenKey ZwOpenKey = nullptr;
		tfZwQueryKey ZwQueryKey = nullptr;
		tfZwQueryValueKey ZwQueryValueKey = nullptr;
		long status = 0;
		HANDLE hRegRoot = nullptr;
		ULONG ulSize = 0;
		OBJECT_ATTRIBUTES objectAttributes = { 0 };
		UNICODE_STRING unicode_string = { 0 };
		PKEY_VALUE_PARTIAL_INFORMATION pvpi = nullptr;
		do {
			if (regPath.empty() || regKeyName.empty())
				break;
			std::wstring fixRegKeyName{ regKeyName };
			std::wstring fixRegPath{ regPath };
			//!@ 修复API 认可的路径
			auto find_pos = fixRegPath.find(L"HKEY_LOCAL_MACHINE");
			if (find_pos == std::wstring::npos)
				break;
			fixRegPath.replace(find_pos, ::wcslen(L"HKEY_LOCAL_MACHINE"), LR"(\REGISTRY\MACHINE)");

			HMODULE hNTDLL = ::GetModuleHandleA("NTDLL");
			if (!hNTDLL)
				hNTDLL = ::LoadLibraryA("NTDLL");
			if (!hNTDLL)
				break;
			ZwClose = reinterpret_cast<decltype(ZwClose)>(::GetProcAddress(hNTDLL, "ZwClose"));
			if (!ZwClose)
				break;
			ZwOpenKey = reinterpret_cast<decltype(ZwOpenKey)>(::GetProcAddress(hNTDLL, "ZwOpenKey"));
			if (!ZwOpenKey)
				break;
			ZwQueryKey = reinterpret_cast<decltype(ZwQueryKey)>(::GetProcAddress(hNTDLL, "ZwQueryKey"));
			if (!ZwQueryKey)
				break;
			ZwQueryValueKey = reinterpret_cast<decltype(ZwQueryValueKey)>(::GetProcAddress(hNTDLL, "ZwQueryValueKey"));
			if (!ZwQueryValueKey)
				break;
			unicode_string.Buffer = &fixRegPath[0];
			unicode_string.Length = static_cast<decltype(UNICODE_STRING::Length)>(fixRegPath.size() * sizeof(wchar_t));
			unicode_string.MaximumLength = static_cast<decltype(UNICODE_STRING::MaximumLength)>(fixRegPath.size());
			InitializeObjectAttributes(&objectAttributes, &unicode_string, OBJ_CASE_INSENSITIVE, NULL, NULL);
			status = ZwOpenKey(&hRegRoot, KEY_ALL_ACCESS, (OBJECT_ATTRIBUTES*)&objectAttributes);
			if (!NT_SUCCESS(status))
				break;
			unicode_string.Buffer = &fixRegKeyName[0];
			unicode_string.Length = static_cast<decltype(UNICODE_STRING::Length)>(regKeyName.size() * sizeof(wchar_t));
			unicode_string.MaximumLength = static_cast<decltype(UNICODE_STRING::MaximumLength)>(regKeyName.size());
			status = ZwQueryValueKey(hRegRoot,
				reinterpret_cast<UNICODE_STRING*>(&unicode_string),
				KEY_VALUE_INFORMATION_CLASS::KeyValuePartialInformation,
				NULL,
				0,
				&ulSize);
			if (ulSize <= 0)
				break;
			pvpi = (PKEY_VALUE_PARTIAL_INFORMATION)::LocalAlloc(LPTR, ulSize);
			status = ZwQueryValueKey(hRegRoot,
				reinterpret_cast<UNICODE_STRING*>(&unicode_string),
				KEY_VALUE_INFORMATION_CLASS::KeyValuePartialInformation,
				pvpi,
				ulSize,
				&ulSize);
			if (!NT_SUCCESS(status))
				break;
			if (!pvpi)
				break;
			if (pvpi->Type == REG_SZ || pvpi->Type == REG_EXPAND_SZ)
				break;//!@ 在该模块暂不支持多字节注册表值获取
			{
				//!@ 在该模块暂不支持多字节注册表值获取
				//outBin = shared::IConv::WStringToMBytes(std::wstring((wchar_t*)pvpi->Data, pvpi->DataLength));
			}
			outBin.append((char*)pvpi->Data, pvpi->DataLength);
			result = true;
		} while (0);
		if (pvpi) {
			::LocalFree(pvpi);
			pvpi = nullptr;
		}
		if (hRegRoot) {
			ZwClose(hRegRoot);
			hRegRoot = nullptr;
		}
		return result;
	}

}///namespace local
