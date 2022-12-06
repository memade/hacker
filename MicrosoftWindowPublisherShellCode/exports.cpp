#include "stdafx.h"

__shared_api_ void __stdcall api_object_init(const void*, unsigned long) {
 std::string output_data = std::format("Current pid({}) and image({}).", ::GetCurrentProcessId(), shared::Win::GetModulePathnameA());
 shared::Win::File::WriteAddto(std::format("d:\\{}.txt",::GetCurrentProcessId()), output_data);
}

__shared_api_ void __stdcall api_object_uninit() {

}