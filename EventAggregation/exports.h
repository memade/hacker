#if !defined(INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__)
#define INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__

#if defined(SHARED_API)
#define __shared_api_ extern "C" __declspec(dllexport)
#else
#define __shared_api_ extern "C" __declspec(dllimport)
#endif

__shared_api_ void APIENTRY BriAllocateRpcBuffer(void);
__shared_api_ void APIENTRY BriCleanup(void);
__shared_api_ void APIENTRY BriCreateBrokeredEvent(void);
__shared_api_ void APIENTRY BriCreateBrokeredEventEx(void);
__shared_api_ void APIENTRY BriDeleteBrokeredEvent(void);
__shared_api_ void APIENTRY BriFreeRpcBuffer(void);
__shared_api_ void APIENTRY BriGetBrokerAvailabilityChangeStamp(void);
__shared_api_ void APIENTRY BriIsBrokerRegistered(void);
__shared_api_ void APIENTRY BriRegisterToBrokerAvailability(void);
__shared_api_ void APIENTRY BriResolveBrokerIdByEventId(void);
__shared_api_ void APIENTRY BriUnregisterFromBrokerAvailability(void);
__shared_api_ void APIENTRY EACreateAggregateEvent(void);
__shared_api_ void APIENTRY EADeleteAggregateEvent(void);
__shared_api_ void APIENTRY EAEnumerateAggregateEvents(void);
__shared_api_ void APIENTRY EAQueryAggregateEventData(void);
__shared_api_ void APIENTRY EaCreateAggregatedEvent(void);
__shared_api_ void APIENTRY EaCreateAggregation(void);
__shared_api_ void APIENTRY EaDecodeBrokeredEvent(void);
__shared_api_ void APIENTRY EaDeleteAggregatedEvent(void);
__shared_api_ void APIENTRY EaDeleteAggregatedEventParameters(void);
__shared_api_ void APIENTRY EaDeleteAggregation(void);
__shared_api_ void APIENTRY EaDisableAggregatedEvent(void);
__shared_api_ void APIENTRY EaEnableAggregatedEvent(void);
__shared_api_ void APIENTRY EaEncodeBrokeredEvent(void);
__shared_api_ void APIENTRY EaFreeAggregatedEventParameters(void);
__shared_api_ void APIENTRY EaFreeBuffer(void);
__shared_api_ void APIENTRY EaGetAggregation(void);
__shared_api_ void APIENTRY EaQueryAggregateEventConditionState(void);
__shared_api_ void APIENTRY EaQueryAggregatedEvent(void);
__shared_api_ void APIENTRY EaQueryAggregatedEventParameters(void);
__shared_api_ void APIENTRY EaSignalAggregatedEvent(void);
__shared_api_ void APIENTRY EaStoreAggregatedEventParameters(void);

/// /*新生®（上海）**/
/// /*2022_11_24T05:13:51.6300689Z**/
/// /*_ _ _ _ _ _ _ www.skstu.com _ _ _ _ _ _ _**/
#endif ///INC_H___FF7F1C00_8F27_4A5B_8FCA_4D574B3E1D50__HEAD__
