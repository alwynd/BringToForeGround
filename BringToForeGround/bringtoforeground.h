#pragma once
/**
Author: Alwyn Dippenaar. ( alwyn.j.dippenaar@gmail.com )
    The header for Bring to Foreground.
**/
#ifndef BRING_TO_FOREGROUND_H
#define BRING_TO_FOREGROUND_H

#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <process.h>

#include <Psapi.h>
#pragma comment(lib, "Kernel32.lib")
#pragma comment(lib, "Psapi.lib")

#define BOOL_TEXT(x) (x ? "true" : "false")
#endif
