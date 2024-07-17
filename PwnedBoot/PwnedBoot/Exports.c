#include <ntifs.h>

__declspec(dllexport) ULONG64 McImageInfo = 0x3800000001;

__declspec(dllexport) NTSTATUS McMicrocodeOperation(void)
{
    return STATUS_SUCCESS;
}