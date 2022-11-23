#include "stdafx.h"

namespace local {
 Physical* __gpPhysical = nullptr;

 Physical::Physical() {
  Init();
 }
 Physical::~Physical() {
  UnInit();
 }
 void Physical::Init() {

 }
 void Physical::UnInit() {
  for (auto it = m_CoreQ.begin(); it != m_CoreQ.end();) {
   it->second->Release();
   it = m_CoreQ.erase(it);
  }
 }
 ICore* Physical::CreateCore() {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  auto create = new Core();
  return dynamic_cast<ICore*>(\
   m_CoreQ.emplace(create->Identify(), create).first->second);
 }


}///namespace local

