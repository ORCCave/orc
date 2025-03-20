#pragma once

#include "OrcDefines.h"

namespace Orc
{
    class GraphicsDevice
    {
    public:
        enum GraphicsDeviceTypes
        {
            GDT_D3D12,
        };

        GraphicsDevice() = default;
        virtual ~GraphicsDevice() = default;
        virtual void* getRawGraphicsDevice() const = 0;
        virtual void present() = 0;

        ORC_DISABLE_COPY_AND_MOVE(GraphicsDevice)

    };
}