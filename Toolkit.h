#ifndef FLEX_UEFI_TOOLKIT_TOOLKIT_H_
#define FLEX_UEFI_TOOLKIT_TOOLKIT_H_

#include "FlexUEFIToolkitDxe.h"
#include <linux/efi.h>

static inline int CheckUEFIRuntimeServiceAvailable(void) {
    if (!efi_enabled(EFI_RUNTIME_SERVICES)) {
        pr_info("%s : EFI runtime services are not enabled\n", __func__);
        return 0;
    } else {
        pr_info("%s : EFI runtime services are enabled\n", __func__);
        return 1;
    }
}

static inline efi_status_t UEFIStringLower(const char *src, char *dst) {
    efi_status_t status = efi.flex_uefi_toolkit_func(futToLowerCase, (u8*)src, (u8*)dst);
    return status;
}

static inline efi_status_t UEFIFlexCall(const char *src, char *dst) {
    efi_status_t status = efi.flex_uefi_toolkit_func(((u32*)src)[0], (u8*)src, (u8*)dst);
    return status;
}

static inline efi_status_t GetFlexUEFIToolkitVariable(char *buffer, size_t *n, uint32_t *attr) {
    efi_char16_t name[]= L"FlexUEFIToolkit";
    efi_guid_t guid = FLEX_UEFI_TOOLKIT_VARIABLE_GUID;
    efi_status_t status = efi.get_variable(name, &guid, attr, n, buffer);
    return status;
}


static inline efi_status_t UEFIGetTime(efi_time_t *eft, efi_time_cap_t *cap) {
    efi_status_t status = efi.get_time(eft, cap);
    return status;
}


#endif