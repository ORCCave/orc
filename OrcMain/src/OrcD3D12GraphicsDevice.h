#pragma once

#include "OrcGraphicsDevice.h"
#include "OrcTypes.h"

namespace Orc
{
    GraphicsDevice* createD3D12GraphicsDevice(void* hwnd, uint32 width, uint32 height);
}