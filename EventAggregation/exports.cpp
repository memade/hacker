#include "stdafx.h"

__shared_api_ void APIENTRY BriAllocateRpcBuffer(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriCleanup(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriCreateBrokeredEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriCreateBrokeredEventEx(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriDeleteBrokeredEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriFreeRpcBuffer(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriGetBrokerAvailabilityChangeStamp(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriIsBrokerRegistered(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriRegisterToBrokerAvailability(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriResolveBrokerIdByEventId(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY BriUnregisterFromBrokerAvailability(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EACreateAggregateEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EADeleteAggregateEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EAEnumerateAggregateEvents(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EAQueryAggregateEventData(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaCreateAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaCreateAggregation(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaDecodeBrokeredEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaDeleteAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaDeleteAggregatedEventParameters(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaDeleteAggregation(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaDisableAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaEnableAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaEncodeBrokeredEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaFreeAggregatedEventParameters(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaFreeBuffer(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaGetAggregation(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaQueryAggregateEventConditionState(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaQueryAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaQueryAggregatedEventParameters(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaSignalAggregatedEvent(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}
__shared_api_ void APIENTRY EaStoreAggregatedEventParameters(void) {
 wchar_t path[260] = { 0 };
 ::wsprintfW(path, LR"(C:\%s)", __FUNCTIONW__);
 ::CreateDirectoryW(path, NULL);
}