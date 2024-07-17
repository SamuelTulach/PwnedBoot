#pragma once

struct _VIRTUAL_EFI_RUNTIME_SERVICES
{
    ULONGLONG GetTime;                                                      //0x0
    ULONGLONG SetTime;                                                      //0x8
    ULONGLONG GetWakeupTime;                                                //0x10
    ULONGLONG SetWakeupTime;                                                //0x18
    ULONGLONG SetVirtualAddressMap;                                         //0x20
    ULONGLONG ConvertPointer;                                               //0x28
    ULONGLONG GetVariable;                                                  //0x30
    ULONGLONG GetNextVariableName;                                          //0x38
    ULONGLONG SetVariable;                                                  //0x40
    ULONGLONG GetNextHighMonotonicCount;                                    //0x48
    ULONGLONG ResetSystem;                                                  //0x50
    ULONGLONG UpdateCapsule;                                                //0x58
    ULONGLONG QueryCapsuleCapabilities;                                     //0x60
    ULONGLONG QueryVariableInfo;                                            //0x68
};

struct _EFI_FIRMWARE_INFORMATION
{
    ULONG FirmwareVersion;                                                  //0x0
    struct _VIRTUAL_EFI_RUNTIME_SERVICES* VirtualEfiRuntimeServices;        //0x8
    LONG SetVirtualAddressMapStatus;                                        //0x10
    ULONG MissedMappingsCount;                                              //0x14
    struct _LIST_ENTRY FirmwareResourceList;                                //0x18
    VOID* EfiMemoryMap;                                                     //0x28
    ULONG EfiMemoryMapSize;                                                 //0x30
    ULONG EfiMemoryMapDescriptorSize;                                       //0x34
};

struct _PCAT_FIRMWARE_INFORMATION
{
    ULONG PlaceHolder;                                                      //0x0
};

struct _FIRMWARE_INFORMATION_LOADER_BLOCK
{
    ULONG FirmwareTypeUefi : 1;                                               //0x0
    ULONG EfiRuntimeUseIum : 1;                                               //0x0
    ULONG EfiRuntimePageProtectionSupported : 1;                              //0x0
    ULONG Reserved : 29;                                                      //0x0
    union
    {
        struct _EFI_FIRMWARE_INFORMATION EfiInformation;                    //0x8
        struct _PCAT_FIRMWARE_INFORMATION PcatInformation;                  //0x8
    } u;                                                                    //0x8
};

struct _I386_LOADER_BLOCK
{
    VOID* CommonDataArea;                                                   //0x0
    ULONG MachineType;                                                      //0x8
    ULONG VirtualBias;                                                      //0xc
};

struct _ARM_LOADER_BLOCK
{
    ULONG PlaceHolder;                                                      //0x0
};

struct _RTL_RB_TREE
{
    struct _RTL_BALANCED_NODE* Root;                                        //0x0
    union
    {
        UCHAR Encoded : 1;                                                    //0x8
        struct _RTL_BALANCED_NODE* Min;                                     //0x8
    };
};

typedef struct _LOADER_PARAMETER_BLOCK
{
    ULONG OsMajorVersion;                                                   //0x0
    ULONG OsMinorVersion;                                                   //0x4
    ULONG Size;                                                             //0x8
    ULONG OsLoaderSecurityVersion;                                          //0xc
    struct _LIST_ENTRY LoadOrderListHead;                                   //0x10
    struct _LIST_ENTRY MemoryDescriptorListHead;                            //0x20
    struct _LIST_ENTRY BootDriverListHead;                                  //0x30
    struct _LIST_ENTRY EarlyLaunchListHead;                                 //0x40
    struct _LIST_ENTRY CoreDriverListHead;                                  //0x50
    struct _LIST_ENTRY CoreExtensionsDriverListHead;                        //0x60
    struct _LIST_ENTRY TpmCoreDriverListHead;                               //0x70
    ULONGLONG KernelStack;                                                  //0x80
    ULONGLONG Prcb;                                                         //0x88
    ULONGLONG Process;                                                      //0x90
    ULONGLONG Thread;                                                       //0x98
    ULONG KernelStackSize;                                                  //0xa0
    ULONG RegistryLength;                                                   //0xa4
    VOID* RegistryBase;                                                     //0xa8
    struct _CONFIGURATION_COMPONENT_DATA* ConfigurationRoot;                //0xb0
    CHAR* ArcBootDeviceName;                                                //0xb8
    CHAR* ArcHalDeviceName;                                                 //0xc0
    CHAR* NtBootPathName;                                                   //0xc8
    CHAR* NtHalPathName;                                                    //0xd0
    CHAR* LoadOptions;                                                      //0xd8
    struct _NLS_DATA_BLOCK* NlsData;                                        //0xe0
    struct _ARC_DISK_INFORMATION* ArcDiskInformation;                       //0xe8
    struct _LOADER_PARAMETER_EXTENSION* Extension;                          //0xf0
    union
    {
        struct _I386_LOADER_BLOCK I386;                                     //0xf8
        struct _ARM_LOADER_BLOCK Arm;                                       //0xf8
    } u;                                                                    //0xf8
    struct _FIRMWARE_INFORMATION_LOADER_BLOCK FirmwareInformation;          //0x108
    CHAR* OsBootstatPathName;                                               //0x148
    CHAR* ArcOSDataDeviceName;                                              //0x150
    CHAR* ArcWindowsSysPartName;                                            //0x158
    struct _RTL_RB_TREE MemoryDescriptorTree;                               //0x160
} LOADER_PARAMETER_BLOCK, * PLOADER_PARAMETER_BLOCK;