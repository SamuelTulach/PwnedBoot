#pragma once

namespace Utils
{
#define SECONDS_TO_MICROSECONDS(seconds) ((seconds) * 1000000)

    void StallForever();
    void WaitForKey();
    CHAR16* StrStr(CHAR16* str, CHAR16* subStr);
    CHAR16* DevicePathToText(const EFI_DEVICE_PATH_PROTOCOL* devicePath, BOOLEAN displayOnly, BOOLEAN allowShortcuts);
    EFI_STATUS GetUserInput(UINTN* userInput);
}