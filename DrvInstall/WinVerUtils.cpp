#include "stdafx.h"
#include "WinVerUtils.h"

using namespace WinVerUtils;

BOOL WinVerUtils::IsWin2k()
{
    OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 0;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_EQUAL );

    // Perform the test.
    return ::VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);
}

BOOL WinVerUtils::IsWin2k3OrLower()
{
    OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 2;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_LESS_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_LESS_EQUAL );

    // Perform the test.
    return ::VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);
}

BOOL WinVerUtils::IsWin2008R2()
{
    OSVERSIONINFOEX osvi;
    DWORDLONG dwlConditionMask = 0;

    // Initialize the OSVERSIONINFOEX structure.

    ::ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    osvi.dwMajorVersion = 5;
    osvi.dwMinorVersion = 2;

    // Initialize the condition mask.
    VER_SET_CONDITION( dwlConditionMask, VER_MAJORVERSION, VER_LESS_EQUAL );
    VER_SET_CONDITION( dwlConditionMask, VER_MINORVERSION, VER_LESS_EQUAL );

    // Perform the test.
    return ::VerifyVersionInfo(
        &osvi,
        VER_MAJORVERSION | VER_MINORVERSION,
        dwlConditionMask);
}
