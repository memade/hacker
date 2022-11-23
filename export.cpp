#include "stdafx.h"

__shared_api_ void* __stdcall api_object_init(const void* route, unsigned long route_size) {
 void* result = nullptr;
 do {
  local::__gpPhysical = new local::Physical();
  result = dynamic_cast<physical::IPhysical*>(local::__gpPhysical);
 } while (0);
 return result;
}

__shared_api_ void __stdcall api_object_uninit() {
 SK_DELETE_PTR(local::__gpPhysical);
}

