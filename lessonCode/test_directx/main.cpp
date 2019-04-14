#include <d3d9.h>
#include <cstdint>
#include <iostream>

#pragma comment(lib, "d3d9.lib")

using namespace std;


void captureScreen(void *data, int width, int height, int bitsize)
{
    // 1.创建directx3d对象
    static IDirect3D9 *d3d = nullptr;
    if (!d3d)
    {
        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        if (!d3d)
        {
            cout << "Direct3DCreate9 failed" << endl;
            return;
        }
    }

    // 2 创建显卡的设备对象
    static IDirect3DDevice9 *device = nullptr;
    if (!device)
    {
        D3DPRESENT_PARAMETERS pa;
        ZeroMemory(&pa, sizeof(pa));
        pa.Windowed = true;
        pa.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
        pa.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pa.hDeviceWindow = GetDesktopWindow();
        d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 0,
            D3DCREATE_HARDWARE_VERTEXPROCESSING, &pa, &device);
        if (!device)
        {
            cout << "CreateDevice failed" << endl;
            return;
        }
    }

    // 3.创建离屏表面
    static IDirect3DSurface9 *sur = nullptr;
    if (!sur)
    {   
        device->CreateOffscreenPlainSurface(width, height,
            D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &sur, nullptr);
        if (!sur)
        {
            cout << "CreateOffscreenPlainSurface failed" << endl;
            return;
        }
    }

    // 4.抓屏
    device->GetFrontBufferData(0, sur);

    // 5.取出数据
    D3DLOCKED_RECT rect;
    ZeroMemory(&rect, sizeof(rect));
    
    if (sur->LockRect(&rect, nullptr, 0) != S_OK)
    {
        cout << "sur->LockRect failed" << endl;
        return;
    }
    memcpy(data, rect.pBits, width * height * bitsize);
    sur->UnlockRect();

    cout << '.' << endl;
}

int main()
{
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);
    int bitsize = 4;
    int bufsize = width * height * bitsize;
    cout << width << ',' << height << endl;

    uint8_t *buffer = new uint8_t[bufsize];

    FILE *fp = nullptr;
    fopen_s(&fp, "test_out.rgb", "wb");

    for (int i = 0; i < 100; i++)
    {
        captureScreen(buffer, width, height, bitsize);
        fwrite(buffer, 1, bufsize, fp);
        Sleep(100);
    }

    fclose(fp);

    system("pause");
    return 0;
}