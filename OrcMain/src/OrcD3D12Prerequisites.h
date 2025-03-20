#pragma once

#include "OrcPlatform.h"

#if ORC_PLATFORM == ORC_PLATFORM_WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <wrl/wrappers/corewrappers.h>

#include <combaseapi.h>
#include <libloaderapi.h>

#endif