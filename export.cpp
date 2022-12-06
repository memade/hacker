#include "stdafx.h"

__shared_api_ void* __stdcall api_object_init(const void* route, unsigned long route_size) {
 void* result = nullptr;
 do {
  local::__gpHacker = new local::Hacker();
  result = dynamic_cast<hacker::IHacker*>(local::__gpHacker);
 } while (0);
 return result;
}

__shared_api_ void __stdcall api_object_uninit() {
 SK_DELETE_PTR(local::__gpHacker);
}

