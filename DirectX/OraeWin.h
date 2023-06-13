#pragma once

#include <sdkddkver.h>

#define WIN32_LEAN_AND_MEAN

// #define NOWINMESSAGES  // - WM_#define, EM_#define, LB_#define, CB_#define
// #define NOWINSTYLES    // - WS_#define, CS_#define, ES_#define, LBS_#define, SBS_#define, CBS_#define
// #define NOSHOWWINDOW   // - SW_#define
// #define NOUSER         // - All USER defines and routines
// #define NOWINOFFSETS	  // - GWL_#define, GCL_#define, associated routines
// #define NOMSG          // - typedef MSG and associated routines
// #define NOMB           // - MB_#define and MessageBox()
// #define NOVIRTUALKEYCODES  // - VK_#define

#define NOGDICAPMASKS    // - CC_#define, LC_#define, PC_#define, CP_#define, TC_#define, RC_
#define NOSYSMETRICS     // - SM_#define
#define NOMENUS          // - MF_#define
#define NOICONS          // - IDI_#define
#define NOKEYSTATES      // - MK_#define
#define NOSYSCOMMANDS    // - SC_#define
#define NORASTEROPS      // - Binary and Tertiary raster ops
#define OEMRESOURCE      // - OEM Resource values
#define NOATOM           // - Atom Manager routines
#define NOCLIPBOARD      // - Clipboard routines
#define NOCOLOR          // - Screen colors
#define NOCTLMGR         // - Control and Dialog routines
#define NODRAWTEXT       // - DrawText() and DT_*
#define NOGDI            // - All GDI defines and routines
#define NOKERNEL         // - All KERNEL defines and routines
#define NONLS            // - All NLS defines and routines
#define NOMEMMGR         // - GMEM_#define, LMEM_#define, GHND, LHND, associated routines
#define NOMETAFILE       // - typedef METAFILEPICT
#define NOMINMAX         // - Macros min(a,b) and max(a,b)
#define NOOPENFILE       // - OpenFile(), OemToAnsi, AnsiToOem, and OF_#define
#define NOSCROLL         // - SB_#define and scrolling routines
#define NOSERVICE        // - All Service Controller routines, SERVICE_ equates, etc.
#define NOSOUND          // - Sound driver routines
#define NOTEXTMETRIC     // - typedef TEXTMETRIC and associated routines
#define NOWH             // - SetWindowsHook and WH_#define
#define NOCOMM           // - COMM driver routines
#define NOKANJI          // - Kanji support stuff.
#define NOHELP           // - Help engine interface.
#define NOPROFILER       // - Profiler interface.
#define NODEFERWINDOWPOS // - DeferWindowPos routines
#define NOMCX            // - Modem Configuration Extensions

#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE

#define STRICT

#include <Windows.h>
