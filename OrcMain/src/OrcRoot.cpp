#include "OrcRoot.h"
#include "OrcException.h"
#include "OrcPlatform.h"
#if ORC_PLATFORM == ORC_PLATFORM_WIN32
#include "OrcD3D12GraphicsDevice.h"
#endif

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include "SDL3/SDL_properties.h"

namespace Orc
{
    GraphicsDevice* Root::createGraphicsDevice(GraphicsDevice::GraphicsDeviceTypes type)
    {
        GraphicsDevice* device;
        switch (type)
        {
#if ORC_PLATFORM == ORC_PLATFORM_WIN32
        case GraphicsDevice::GDT_D3D12:
        {
            auto props = SDL_GetWindowProperties((SDL_Window*)mWindowHandle);
            auto hwnd = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
            device = createD3D12GraphicsDevice(hwnd, mWidthForSwapChain, mHeightForSwapChain);
            break;
        }
#endif
        default:
            throw OrcException("Invalid GraphicsDeviceType");
        }
        return device;
    }

    void Root::destroyGraphicsDevice(GraphicsDevice* device)
    {
        delete device;
    }

    void Root::startRendering(GraphicsDevice* device)
    {
        SDL_Event event;
        bool running = true;

        while (running)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    running = false;
                    break;
                }
            }

            if (!running)
                break;

            // todo

            device->present();
        }
    }
}