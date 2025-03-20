#pragma once

#include "OrcDefines.h"
#include "OrcGraphicsDevice.h"
#include "OrcStdHeaders.h"
#include "OrcTypes.h"

namespace Orc
{
    class ApplicationContext;
    class Root
    {
    public:
        ~Root() = default;
        GraphicsDevice* createGraphicsDevice(GraphicsDevice::GraphicsDeviceTypes type);
        void destroyGraphicsDevice(GraphicsDevice* device);
        void startRendering(GraphicsDevice* device);

        ORC_DISABLE_COPY_AND_MOVE(Root)
    private:
        Root(void* handle, uint32 w, uint32 h) : 
            mWindowHandle(handle), mWidthForSwapChain(w), mHeightForSwapChain(h) {}

        void* mWindowHandle;

        uint32 mWidthForSwapChain;
        uint32 mHeightForSwapChain;

        friend class ApplicationContext;
    };
}