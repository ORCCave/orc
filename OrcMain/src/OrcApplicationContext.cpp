#include "OrcApplicationContext.h"
#include "OrcStdHeaders.h"

#include "SDL3/SDL.h"

namespace Orc
{
    class ApplicationContext::impl
    {
    public:
        impl(void* handle, uint32 width, uint32 height) : mRoot(new Root(handle, width, height)) {}
        std::unique_ptr<Root> mRoot;
    };

    ApplicationContext::ApplicationContext(const std::string& windowTitle, uint32 width, uint32 height)
        : mWindowTitle(windowTitle), mWidth(width), mHeight(height), pimpl(nullptr) {}

    Root* ApplicationContext::getRoot() const
    {
        return pimpl->mRoot.get();
    }

    void ApplicationContext::initApp()
    {
        SDL_Init(SDL_INIT_VIDEO);
        mWindowHandle = SDL_CreateWindow(mWindowTitle.c_str(), mWidth, mHeight, 0);
        pimpl = new ApplicationContext::impl(mWindowHandle, mWidth, mHeight);
    }

    void ApplicationContext::closeApp() 
    {
		delete pimpl;
		pimpl = nullptr;
        SDL_DestroyWindow((SDL_Window*)mWindowHandle);
    }
}