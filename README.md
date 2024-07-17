# PwnedBoot
This is a proof-of-concept payload that can replace `mcupdate_<platform>.dll`, which will get loaded by the Windows bootloader (`winload.efi`) even when Secure Boot is enabled. Tested on Windows 10 22H2 19045.2965.

[![video](https://img.youtube.com/vi/qZgsRD3_KhY/0.jpg)](https://www.youtube.com/watch?v=qZgsRD3_KhY)

*Click to open YouTube*

## Usage
1. Compile the project using [Visual Studio 2022](https://visualstudio.microsoft.com/) and [WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk).
2. Rename the output file to `mcupdate_AuthenticAMD.dll` or `mcupdate_GenuineIntel.dll`.
3. Find a spare Windows install (after replacing the file, the Windows installation where you replace it won't be able to boot). You can use [Windows To Go](https://learn.microsoft.com/en-us/previous-versions/windows/it-pro/windows-10/deployment/windows-to-go/windows-to-go-overview).
4. Move the file to `C:\Windows\System32\mcupdate_<platform>.dll`.
5. Boot from the disk where that Windows install is located.
6. Go to advanced boot options and select 'Disable Driver Signature Enforcement'.
7. The example should be loaded.

## How this works
The Windows bootloader (`winload.efi`) does not check the code signature or integrity of the `mcupdate_<platform>.dll` file when starting the system if the 'Disable Driver Signature Enforcement' option is selected. The file is loaded very early in the boot stage, and its entry point is executed from within the bootloader before a call to `ExitBootServices()`, which means that you can just restore the context and return back to the firmware. The plot twist is that `mcupdate_<platform>.dll` it not inside a valid memory mapping in the firmware context, so this project just remaps itself over the bootloader.

## Next steps
In its current form, the proof-of-concept can only boot from the selected boot device (without rebooting). You may use it to hook into the boot process or, if you want to get creative, manually map the actual bootloader and boot into a different OS altogether (like Linux).
