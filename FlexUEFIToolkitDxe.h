#ifndef FLEX_UEFI_TOOLKIT_DXE_H_
#define FLEX_UEFI_TOOLKIT_DXE_H_

#define FLEX_UEFI_TOOLKIT_VARIABLE_NAME L"FlexUEFIToolkit"
#define FLEX_UEFI_TOOLKIT_VARIABLE_GUID EFI_GUID(0x514c2d84, 0x9cba, 0x9cba, 0x92, 0x97, 0x04, 0x7c, 0x16, 0xba, 0xfa, 0x45)
#define FLEX_UEFI_TOOLKIT_NETLINK_ID 29

#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
    futNonOp            = 0,
    futToLowerCase      = 1,
    futReadFlash        = 2,
    futChangeBIOS	    = 3,
    futSetFisrtBoot     = 4,
    futFuncNum

} FLEX_UEFI_TOOKLIT_FUNC_TYPE;


#ifdef __cplusplus
}
#endif
#endif