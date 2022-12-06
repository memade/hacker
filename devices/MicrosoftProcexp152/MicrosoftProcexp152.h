#if !defined(__42901F03_5C7F_4D63_AFAD_95C5F2B15C88__)
#define __42901F03_5C7F_4D63_AFAD_95C5F2B15C88__

namespace device {
 /*https://github.com/BeneficialCode/KPPL
KPPL
Kill Protected Process Light Process (include av)
Some antimalware process has some object callbacks by using ObRegisterCallbacks to avoid extern killing.
Somtimes, we have got the process handle right. We can do other thing. For example, we can dump the lsass.exe.
-------------------------------------------------------------------------------------------------------------------------
0: kd> dt nt!_object_type ffffe78f`93ca8d20
   +0x000 TypeList         : _LIST_ENTRY [ 0xffffe78f`93ca8d20 - 0xffffe78f`93ca8d20 ]
   +0x010 Name             : _UNICODE_STRING "Process"
   +0x020 DefaultObject    : (null)
   +0x028 Index            : 0x7 ''
   +0x02c TotalNumberOfObjects : 0x1f8
   +0x030 TotalNumberOfHandles : 0x728
   +0x034 HighWaterNumberOfObjects : 0x1f8
   +0x038 HighWaterNumberOfHandles : 0x881
   +0x040 TypeInfo         : _OBJECT_TYPE_INITIALIZER
   +0x0b8 TypeLock         : _EX_PUSH_LOCK
   +0x0c0 Key              : 0x636f7250
   +0x0c8 CallbackList     : _LIST_ENTRY [ 0xffffd105`77373e40 - 0xffffd105`77373e40 ]

0: kd> dx -id 0,0,ffffe78f93c5d080 -r1 (*((ntkrnlmp!_OBJECT_TYPE_INITIALIZER *)0xffffe78f93ca8d60))
(*((ntkrnlmp!_OBJECT_TYPE_INITIALIZER *)0xffffe78f93ca8d60))                 [Type: _OBJECT_TYPE_INITIALIZER]
    [+0x000] Length           : 0x78 [Type: unsigned short]
    [+0x002] ObjectTypeFlags  : 0xca [Type: unsigned short]
    [+0x002 ( 0: 0)] CaseInsensitive  : 0x0 [Type: unsigned char]
    [+0x002 ( 1: 1)] UnnamedObjectsOnly : 0x1 [Type: unsigned char]
    [+0x002 ( 2: 2)] UseDefaultObject : 0x0 [Type: unsigned char]
    [+0x002 ( 3: 3)] SecurityRequired : 0x1 [Type: unsigned char]
    [+0x002 ( 4: 4)] MaintainHandleCount : 0x0 [Type: unsigned char]
    [+0x002 ( 5: 5)] MaintainTypeList : 0x0 [Type: unsigned char]
    [+0x002 ( 6: 6)] SupportsObjectCallbacks : 0x1 [Type: unsigned char] \\ This flag effect the callback function
    [+0x002 ( 7: 7)] CacheAligned     : 0x1 [Type: unsigned char]
    [+0x003 ( 0: 0)] UseExtendedParameters : 0x0 [Type: unsigned char]
    [+0x003 ( 7: 1)] Reserved         : 0x0 [Type: unsigned char]
    ......

    -------------------------------------------------------------------------------------------------------------------------
    ObDuplicateObject
    ObpFilterOperation
    ObpCallPreOperationCallbacks
    ObpCallPostOperationCallbacks
    ObpCreateHandle

    return (a1->TypeInfo.ObjectTypeFlags & 0x40) != 0 && a1->CallbackList.Flink != &a1->CallbackList;
 */
 class MicrosoftProcexp152 final : public IDevice {
 public:
  MicrosoftProcexp152();
  virtual ~MicrosoftProcexp152();
 private:
  void Init();
  void UnInit();
 public:
  void Release() const override final;
  bool OpenProcess(const DWORD&, HANDLE&) const override final;
 };


}///namespace device

/// /*_ Memade®（新生™） _**/
/// /*_ Fri, 02 Dec 2022 12:34:50 GMT _**/
/// /*_____ https://www.skstu.com/ _____ **/
#endif///__42901F03_5C7F_4D63_AFAD_95C5F2B15C88__