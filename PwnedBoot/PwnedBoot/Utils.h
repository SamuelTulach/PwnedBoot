#pragma once

namespace Utils
{
    ULONG64 FindPattern(void* baseAddress, ULONG64 size, const char* pattern);
    ULONG64 FindPatternImage(void* base, const char* pattern);
}