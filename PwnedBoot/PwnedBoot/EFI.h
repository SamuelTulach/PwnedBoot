#pragma once

namespace EFI
{
    unsigned long long FakeUnload(void* imageHandle);
    void Stage0(void* imageHandle, void* systemTable);
    void Stage1();
    void ChangeResolution();
    void SplashScreen();

    void BootFromDisk();
    void Exec();
}