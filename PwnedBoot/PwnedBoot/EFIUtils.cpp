#include "EFIGlobal.h"

void Utils::StallForever()
{
    while (true)
        gBS->Stall(SECONDS_TO_MICROSECONDS(1));
}

CHAR16* Utils::StrStr(CHAR16* str, CHAR16* subStr)
{
    if (!str || !subStr)
        return nullptr;

    for (; *str != '\0'; str++)
    {
        const CHAR16* strPtr = str;
        const CHAR16* subStrPtr = subStr;

        while (*strPtr == *subStrPtr && *strPtr != '\0')
        {
            strPtr++;
            subStrPtr++;
        }

        if (*subStrPtr == '\0')
            return str;
    }

    return nullptr;
}

CHAR16* Utils::DevicePathToText(const EFI_DEVICE_PATH_PROTOCOL* devicePath, const BOOLEAN displayOnly, const BOOLEAN allowShortcuts)
{
    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL* devicePathToText;
    const EFI_STATUS status = gBS->LocateProtocol(&gEfiDevicePathToTextProtocolGuid, nullptr, reinterpret_cast<void**>(&devicePathToText));
    if (EFI_ERROR(status))
        return nullptr;

    CHAR16* devicePathText = devicePathToText->ConvertDevicePathToText(devicePath, displayOnly, allowShortcuts);
    if (!devicePathText)
        return nullptr;

    return devicePathText;
}

EFI_STATUS Utils::GetUserInput(UINTN* userInput)
{
    EFI_INPUT_KEY key;
    UINTN value = 0;
    BOOLEAN inputValid = false;

    while (!inputValid)
    {
        const EFI_STATUS status = gST->ConIn->ReadKeyStroke(gST->ConIn, &key);
        if (EFI_ERROR(status))
            continue;

        if (key.UnicodeChar >= L'0' && key.UnicodeChar <= L'9')
        {
            value = value * 10 + (key.UnicodeChar - L'0');
            Print(L"%c", key.UnicodeChar);
        }
        else if (key.UnicodeChar == CHAR_CARRIAGE_RETURN)
        {
            inputValid = true;
        }
        else
        {
            Print(L"\nInvalid! Enter index: ");
            value = 0;
        }
    }

    *userInput = value;
    Print(L"\n");
    return EFI_SUCCESS;
}