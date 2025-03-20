#include "OrcPlatform.h"

#if ORC_PLATFORM == ORC_PLATFORM_WIN32

#include "OrcD3D12Prerequisites.h"
#include "OrcException.h"
#include "OrcGraphicsDevice.h"
#include "OrcTypes.h"

namespace Orc
{
    inline constexpr uint32 ORC_SWAPCHAIN_COUNT = 3;
    inline constexpr DXGI_FORMAT ORC_SWAPCHAIN_FORMAT = DXGI_FORMAT_R8G8B8A8_UNORM;

    class D3D12GraphicsDevice : public GraphicsDevice
    {
    public:
        D3D12GraphicsDevice(HWND hwnd, uint32 width, uint32 height)
        {
            ++instanceCount;
            if (instanceCount != 1)
            {
                throw OrcException("There can be only one singleton");
            }

            mHD3D12 = LoadLibraryW(L"d3d12.dll");
            mHDXGI = LoadLibraryW(L"dxgi.dll");
            _createDevice();
            _createQueue();

            _createSwapChain(hwnd, width, height);

            mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
            mDevice->CreateFence(mFenceValue[mFrameIndex]++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
            mEvent.Attach(CreateEventW(nullptr, FALSE, FALSE, nullptr));
            mDevice->CreateFence(mCopyFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mCopyFence));
            mCopyEvent.Attach(CreateEventW(nullptr, FALSE, FALSE, nullptr));
            mDevice->CreateFence(mComputeFenceValue++, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mComputeFence));
            mComputeEvent.Attach(CreateEventW(nullptr, FALSE, FALSE, nullptr));
        }

        ~D3D12GraphicsDevice()
        {
            // Can't call FreeLibrary in Release mode
            //FreeLibrary(mHD3D12);
            //FreeLibrary(mHDXGI);
        }

        void* getRawGraphicsDevice() const
        {
            return mDevice.Get();
        }

        void _createDevice()
        {
            uint32 factoryFlag = 0;
#ifdef _DEBUG
            auto pfnD3D12GetDebugInterface = reinterpret_cast<PFN_D3D12_GET_DEBUG_INTERFACE>(GetProcAddress(mHD3D12, "D3D12GetDebugInterface"));
            if (SUCCEEDED(pfnD3D12GetDebugInterface(IID_PPV_ARGS(&mDebugController))))
            {
                mDebugController->EnableDebugLayer();
            }
            factoryFlag = DXGI_CREATE_FACTORY_DEBUG;
#endif
            auto pfnCreateDXGIFactory2 = reinterpret_cast<decltype(&CreateDXGIFactory2)>(GetProcAddress(mHDXGI, "CreateDXGIFactory2"));
            pfnCreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(&mFactory));
            mFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&mAdapter));

            auto pfnD3D12CreateDevice = reinterpret_cast<PFN_D3D12_CREATE_DEVICE>(GetProcAddress(mHD3D12, "D3D12CreateDevice"));
            pfnD3D12CreateDevice(mAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&mDevice));
        }

        void _createQueue()
        {
            D3D12_COMMAND_QUEUE_DESC queueDesc{};
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mGraphicsQueue));
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
            mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCopyQueue));
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mComputeQueue));
        }

        void _createSwapChain(HWND hwnd, uint32 width, uint32 height)
        {
            DXGI_SWAP_CHAIN_DESC1 scDesc{};
            scDesc.BufferCount = ORC_SWAPCHAIN_COUNT;
            scDesc.Width = width;
            scDesc.Height = height;
            scDesc.Format = ORC_SWAPCHAIN_FORMAT;
            scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
            scDesc.SampleDesc.Count = 1;
            scDesc.SampleDesc.Quality = 0;
            scDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
            scDesc.Scaling = DXGI_SCALING_STRETCH;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc{};
            fsSwapChainDesc.Windowed = TRUE;
            IDXGISwapChain1* swapChain;

            mFactory->CreateSwapChainForHwnd(mGraphicsQueue.Get(), hwnd, &scDesc, &fsSwapChainDesc, nullptr, &swapChain);
            mFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);

            swapChain->QueryInterface(mSwapChain.ReleaseAndGetAddressOf());

            swapChain->Release();
        }

        void present()
        {
            mSwapChain->Present(1, 0);
            moveToNextFrame();
        }

        void moveToNextFrame()
        {
            const uint64 currentFenceValue = mFenceValue[mFrameIndex];
            mGraphicsQueue->Signal(mFence.Get(), currentFenceValue);
            mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
            if (mFence->GetCompletedValue() < mFenceValue[mFrameIndex])
            {
                mFence->SetEventOnCompletion(mFenceValue[mFrameIndex], mEvent.Get());
                WaitForSingleObjectEx(mEvent.Get(), INFINITE, FALSE);
            }
            mFenceValue[mFrameIndex] = currentFenceValue + 1;
        }

        Microsoft::WRL::ComPtr<IDXGIAdapter4> mAdapter;
        Microsoft::WRL::ComPtr<ID3D12Debug> mDebugController;
        Microsoft::WRL::ComPtr<IDXGIFactory7> mFactory;
        Microsoft::WRL::ComPtr<ID3D12Device4> mDevice;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mGraphicsQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCopyQueue;
        Microsoft::WRL::ComPtr<ID3D12CommandQueue> mComputeQueue;

        HMODULE mHD3D12;
        HMODULE mHDXGI;

        Microsoft::WRL::ComPtr<IDXGISwapChain4> mSwapChain;

        uint32 mFrameIndex = 0;

        uint64 mFenceValue[ORC_SWAPCHAIN_COUNT]{};
        Microsoft::WRL::ComPtr<ID3D12Fence1> mFence;
        Microsoft::WRL::Wrappers::Event mEvent;

        uint64 mCopyFenceValue = 0;
        Microsoft::WRL::ComPtr<ID3D12Fence1> mCopyFence;
        Microsoft::WRL::Wrappers::Event mCopyEvent;
        uint64 mComputeFenceValue = 0;
        Microsoft::WRL::ComPtr<ID3D12Fence1> mComputeFence;
        Microsoft::WRL::Wrappers::Event mComputeEvent;

        inline static int32 instanceCount = 0;
    };

    GraphicsDevice* createD3D12GraphicsDevice(void* hwnd, uint32 width, uint32 height)
    {
        return new D3D12GraphicsDevice((HWND)hwnd, width, height);
    }
}

#endif