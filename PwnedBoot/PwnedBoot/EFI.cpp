#include "EFIGlobal.h"

extern "C" CHAR8 __ImageBase;

static EFI_SYSTEM_TABLE* g_SystemTable = nullptr;
static EFI_HANDLE g_ImageHandle = nullptr;

EFI_STATUS EFI::FakeUnload(EFI_HANDLE imageHandle)
{
    return EFI_SUCCESS;
}

void EFI::Stage0(void* imageHandle, void* systemTable)
{
    g_ImageHandle = imageHandle;
    g_SystemTable = static_cast<EFI_SYSTEM_TABLE*>(systemTable);
}

void EFI::Stage1()
{
    gST = g_SystemTable;
    gBS = g_SystemTable->BootServices;
    gRT = g_SystemTable->RuntimeServices;
}

void EFI::ChangeResolution()
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL* GOP;
    EFI_STATUS status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, nullptr, reinterpret_cast<void**>(&GOP));
    if (EFI_ERROR(status))
    {
        Print(L"LocateProtocol() failed: %r\n", status);
        return;
    }

    UINT32 maxResolutionMode = 0;
    UINT32 maxResolutionVertical = 0;
    UINT32 maxResolutionHorizontal = 0;

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* graphicsInfo;
    UINTN graphicsInfoSize;

    for (UINT32 i = 0; i < GOP->Mode->MaxMode; ++i)
    {
        GOP->QueryMode(GOP, i, &graphicsInfoSize, &graphicsInfo);

        if (maxResolutionVertical * maxResolutionHorizontal < graphicsInfo->VerticalResolution * graphicsInfo->HorizontalResolution)
        {
            maxResolutionVertical = graphicsInfo->VerticalResolution;
            maxResolutionHorizontal = graphicsInfo->HorizontalResolution;

            maxResolutionMode = i;
        }
    }

    status = GOP->SetMode(GOP, maxResolutionMode);
    if (EFI_ERROR(status))
    {
        Print(L"SetMode() failed: %r\n", status);
        return;
    }
}

void EFI::SplashScreen()
{
    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    gST->ConOut->ClearScreen(gST->ConOut);
    gST->ConOut->EnableCursor(ST->ConOut, true);

    gST->ConOut->SetAttribute(gST->ConOut, EFI_WHITE | EFI_BACKGROUND_BLACK);
    Print(L"Pwned");

    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTRED | EFI_BACKGROUND_BLACK);
    Print(L"Boot\n");

    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
    Print(L"by Samuel Tulach (tulach.cc)\n\n");

    gST->ConOut->SetAttribute(gST->ConOut, EFI_LIGHTGRAY | EFI_BACKGROUND_BLACK);
}

EFI_STATUS DiskPicker(EFI_HANDLE* chosenDiskHandle)
{
    EFI_HANDLE* handleBuffer;
    UINTN handleCount;
    EFI_BLOCK_IO_PROTOCOL* blockIo;
    EFI_DEVICE_PATH_PROTOCOL* devicePath;

    EFI_STATUS status = gBS->LocateHandleBuffer(ByProtocol, &gEfiBlockIoProtocolGuid, NULL, &handleCount, &handleBuffer);
    if (EFI_ERROR(status))
    {
        Print(L"LocateHandleBuffer() failed with status: %r\n", status);
        return status;
    }

    for (UINTN index = 0; index < handleCount; index++)
    {
        status = gBS->HandleProtocol(handleBuffer[index], &gEfiBlockIoProtocolGuid, reinterpret_cast<void**>(&blockIo));
        if (EFI_ERROR(status))
        {
            Print(L"HandleProtocol() failed for handle %u with status: %r\n", index, status);
            continue;
        }

        status = gBS->HandleProtocol(handleBuffer[index], &gEfiDevicePathProtocolGuid, reinterpret_cast<void**>(&devicePath));
        if (EFI_ERROR(status))
        {
            Print(L"HandleProtocol() failed for handle %u with status: %r\n", index, status);
            continue;
        }

        CHAR16* devicePathStr = Utils::DevicePathToText(devicePath, true, false);
        if (!devicePathStr)
        {
            Print(L"DevicePathToText() failed for index %u\n", index);
            continue;
        }

        Print(L"[%u]: %s\n", index, devicePathStr);
        FreePool(devicePathStr);
    }

    Print(L"Enter index: ");
    UINTN chosenIndex;
    status = Utils::GetUserInput(&chosenIndex);
    if (EFI_ERROR(status) || chosenIndex >= handleCount)
    {
        Print(L"Invalid selection\n");
        FreePool(handleBuffer);
        return EFI_INVALID_PARAMETER;
    }

    *chosenDiskHandle = handleBuffer[chosenIndex];

    FreePool(handleBuffer);
    return EFI_SUCCESS;
}

void EFI::BootFromDisk()
{
    EFI_HANDLE targetDisk = nullptr;
    EFI_STATUS status = DiskPicker(&targetDisk);
    if (EFI_ERROR(status))
    {
        Utils::StallForever();
        return;
    }

    Print(L"\n");
    Print(L"Picked disk with handle 0x%X\n", targetDisk);

    Print(L"Loading \\EFI\\BOOT\\bootx64.efi...\n", targetDisk);
    CHAR16* bootloaderPath = const_cast<CHAR16*>(L"\\EFI\\BOOT\\bootx64.efi");
    EFI_DEVICE_PATH_PROTOCOL* bootloaderDevicePath = FileDevicePath(targetDisk, bootloaderPath);
    if (!bootloaderDevicePath)
    {
        Print(L"FileDevicePath() returned null\n");
        Utils::StallForever();
        return;
    }

    EFI_HANDLE bootloaderHandle;
    status = gBS->LoadImage(true, g_ImageHandle, bootloaderDevicePath, nullptr, 0, &bootloaderHandle);
    if (EFI_ERROR(status))
    {
        Print(L"LoadImage() failed: %r\n", status);
        Utils::StallForever();
        return;
    }

    Print(L"Loaded with handle 0x%X\n", bootloaderHandle);

    Print(L"Staring \\EFI\\BOOT\\bootx64.efi...\n");
    //gBS->Stall(SECONDS_TO_MICROSECONDS(1));
    status = gBS->StartImage(bootloaderHandle, nullptr, nullptr);
    if (EFI_ERROR(status))
    {
        Print(L"StartImage() failed: %r\n", status);
        Utils::StallForever();
        return;
    }

    Print(L"Should not get here...\n");
    Utils::StallForever();
}

void EFI::Exec()
{
    Print(L"g_ImageHandle: 0x%X\n", g_ImageHandle);
    Print(L"g_SystemTable: 0x%X\n\n", g_SystemTable);

    Print(L"[0]: Boot from disk\n");
    Print(L"Enter index: ");
    UINTN chosenIndex;
    EFI_STATUS status = Utils::GetUserInput(&chosenIndex);
    if (EFI_ERROR(status) || chosenIndex >= 1)
    {
        Print(L"Invalid selection\n");
        Utils::StallForever();
        return;
    }

    Print(L"\n");

    /*
     * Add options to manual map an application, driver, patch the bootloader...
     */

    BootFromDisk();
}