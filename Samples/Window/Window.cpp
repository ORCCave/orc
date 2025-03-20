#include "Orc.h"

int main()
{
    Orc::ApplicationContext ctx("OrcWindow", 1200, 600);
    ctx.initApp();
    Orc::Root* root = ctx.getRoot();
    Orc::GraphicsDevice* device = root->createGraphicsDevice(Orc::GraphicsDevice::GDT_D3D12);

    root->startRendering(device);

    root->destroyGraphicsDevice(device);
    ctx.closeApp();
    return 0;
}