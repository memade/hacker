#include "stdafx.h"

namespace local {

 Core::Core()
  : m_Identify(shared::Win::Time::TimeStamp<std::chrono::microseconds>()) {
  Init();
 }

 Core::~Core() {
  UnInit();
 }
 void Core::Release() const {
  delete this;
 }
 void Core::Init() {
  m_pConfig = new Config();
 }
 void Core::UnInit() {
  SK_DELETE_PTR(m_pConfig);
 }
 ICoreConfig* Core::ConfigGet() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_pConfig;
 }
 const TypeIdentify& Core::Identify() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_Identify;
 }

 //////////////////////////////////////////////////////////////////////////////////////////////
 Core::Config::Config() {

 }
 Core::Config::~Config() {

 }
 const unsigned long long& Core::Config::UnitOfTime() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_UnitOfTime;
 }
 void Core::Config::UnitOfTime(const unsigned long long& time) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_UnitOfTime = time;
 }
 const unsigned long long& Core::Config::UnitOfFrame() const {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  return m_UnitOfFrame;
 }
 void Core::Config::UnitOfFrame(const unsigned long long& frame) {
  std::lock_guard<std::mutex> lock{ *m_Mutex };
  m_UnitOfFrame = frame;
 }
}///namespace local

