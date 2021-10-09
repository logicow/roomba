#include "screenReader.h"
#include "d3d11.h"
#include "dxgi1_2.h"
#include <iostream>
#include <d3dcompiler.h>

#pragma comment(lib, "Dxgi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


IDXGIOutputDuplication* pScreenOutput = NULL;

screenReader g_screen;

void screenReader::init()
{
    initDuplication();
    initCompute();
}

void screenReader::initDuplication()
{
    HRESULT hr;

    bool bSuccess = SetProcessDPIAware();

    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&factory1));
    factory1->EnumAdapters1(0, &adapter1);

    adapter1->EnumOutputs(0, &output);
    output->GetDesc(&outputDesc);

    hr = output->QueryInterface(__uuidof(IDXGIOutput1), (void**)&output1);
    if (S_OK != hr) {
        printf("Error: failed to query the IDXGIOutput1 interface.\n");
        exit(EXIT_FAILURE);
    }

    hr = D3D11CreateDevice(adapter1,              /* Adapter: The adapter (video card) we want to use. We may use NULL to pick the default adapter. */
        D3D_DRIVER_TYPE_UNKNOWN,  /* DriverType: We use the GPU as backing device. */
        NULL,                     /* Software: we're using a D3D_DRIVER_TYPE_HARDWARE so it's not applicaple. */
        NULL,                     /* Flags: maybe we need to use D3D11_CREATE_DEVICE_BGRA_SUPPORT because desktop duplication is using this. */
        NULL,                     /* Feature Levels (ptr to array):  what version to use. */
        0,                        /* Number of feature levels. */
        D3D11_SDK_VERSION,        /* The SDK version, use D3D11_SDK_VERSION */
        &device,              /* OUT: the ID3D11Device object. */
        &featureLevel,       /* OUT: the selected feature level. */
        &context);            /* OUT: the ID3D11DeviceContext that represents the above features. */

    // some stuff from
    // https://github.com/diederickh/screen_capture/blob/master/src/test/test_win_api_directx_research.cpp

    hr = output1->DuplicateOutput(device, &duplication);

    if (S_OK != hr) {
        printf("Error: failed DuplicateOutput\n");
        exit(EXIT_FAILURE);
    }

    {
        hr = output->GetDesc(&outputDesc);
        if (S_OK != hr) {
            printf("Error: failed to get the DXGI_OUTPUT_DESC from the output (monitor). We need this to create a staging texture when downloading the pixels from the gpu.\n");
            exit(EXIT_FAILURE);
        }

        printf("The monitor has the following dimensions: left: %d, right: %d, top: %d, bottom: %d.\n"
            , (int)outputDesc.DesktopCoordinates.left
            , (int)outputDesc.DesktopCoordinates.right
            , (int)outputDesc.DesktopCoordinates.top
            , (int)outputDesc.DesktopCoordinates.bottom
        );

        desktopCoordinates = outputDesc.DesktopCoordinates;
    }

    stagingTexDesc.Width = outputDesc.DesktopCoordinates.right;
    stagingTexDesc.Height = outputDesc.DesktopCoordinates.bottom;
    stagingTexDesc.MipLevels = 1;
    stagingTexDesc.ArraySize = 1; /* When using a texture array. */
    stagingTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; /* This is the default data when using desktop duplication, see https://msdn.microsoft.com/en-us/library/windows/desktop/hh404611(v=vs.85).aspx */
    stagingTexDesc.SampleDesc.Count = 1; /* MultiSampling, we can use 1 as we're just downloading an existing one. */
    stagingTexDesc.SampleDesc.Quality = 0; /* "" */
    stagingTexDesc.Usage = D3D11_USAGE_STAGING;
    stagingTexDesc.BindFlags = 0;
    stagingTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingTexDesc.MiscFlags = 0;

    hr = device->CreateTexture2D(&stagingTexDesc, NULL, &stagingTex);
    if (E_INVALIDARG == hr) {
        printf("Error: received E_INVALIDARG when trying to create the texture.\n");
        exit(EXIT_FAILURE);
    }
    else if (S_OK != hr) {
        printf("Error: failed to create the 2D texture, error: %d.\n", hr);
        exit(EXIT_FAILURE);
    }

    desktopSRVDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desktopSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desktopSRVDesc.Texture2D.MipLevels = 1;
}

void screenReader::terminate()
{
    computeShader->Release();

    if (NULL != desktopDXGIResource) {
        desktopDXGIResource->Release();
        desktopDXGIResource = NULL;
    }

    if (NULL != stagingTex) {
        stagingTex->Release();
        stagingTex = NULL;
    }

    if (NULL != device) {
        device->Release();
        device = NULL;
    }

    if (NULL != context) {
        context->Release();
        context = NULL;
    }

    if (NULL != duplication) {
        duplication->Release();
        duplication = NULL;
    }

    if (NULL != screenTex) {
        screenTex->Release();
        screenTex = NULL;
    }

    adapter1->Release();
    adapter1 = NULL;

    output->Release();
    output = NULL;

    if (NULL != output1) {
        output1->Release();
        output1 = NULL;
    }

    if (NULL != factory1) {
        factory1->Release();
        factory1 = NULL;
    }
}

void screenReader::saveScreenToPNG(std::string pngName)
{
    HRESULT hr = duplication->AcquireNextFrame(1000, &frameInfo, &desktopDXGIResource);
    if (DXGI_ERROR_ACCESS_LOST == hr) {
        printf("Received a DXGI_ERROR_ACCESS_LOST.\n");
        return;
    }
    else if (DXGI_ERROR_WAIT_TIMEOUT == hr) {
        printf("Received a DXGI_ERROR_WAIT_TIMEOUT.\n");
        return;
    }
    else if (DXGI_ERROR_INVALID_CALL == hr) {
        printf("Received a DXGI_ERROR_INVALID_CALL.\n");
        return;
    }

    hr = desktopDXGIResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&screenTex);
    if (S_OK != hr) {
        printf("Error: failed to query the ID3D11Texture2D interface on the IDXGIResource we got.\n");
        exit(EXIT_FAILURE);
    }

    context->CopyResource(stagingTex, screenTex);

    D3D11_MAPPED_SUBRESOURCE map;
    hr = context->Map(stagingTex,          /* Resource */
        0,                    /* Subresource */
        D3D11_MAP_READ,       /* Map type. */
        0,                    /* Map flags. */
        &map);

    if (DXGI_ERROR_INVALID_CALL == hr) {
        printf("MapDesktopSurface returned DXGI_ERROR_INVALID_CALL.\n");
        return;
    }
    else if (DXGI_ERROR_ACCESS_LOST == hr) {
        printf("MapDesktopSurface returned DXGI_ERROR_ACCESS_LOST.\n");
        return;
    }
    else if (E_INVALIDARG == hr) {
        printf("MapDesktopSurface returned E_INVALIDARG.\n");
        return;
    }
    else if(hr != S_OK) {
        printf("MapDesktopSurface returned an unknown error.\n");
        return;
    }

    unsigned char* data = (unsigned char*)map.pData;
    //printf("Mapped the staging tex; we can access the data now.\n");
    printf("RowPitch: %u, DepthPitch: %u, %02X, %02X, %02X\n", map.RowPitch, map.DepthPitch, data[0], data[1], data[2]);

    printf("exporting to ");
    printf(pngName.c_str());
    printf("\n");

    /* We have to make the image opaque. */

    for (int k = 0; k < stagingTexDesc.Width; ++k) {
        for (int l = 0; l < stagingTexDesc.Height; ++l) {
            int dx = l * stagingTexDesc.Width * 4 + k * 4;
            data[dx + 3] = 0xFF;
        }
    }

    std::string outPath = pngName;
    stbi_write_png(outPath.c_str(), stagingTexDesc.Width, stagingTexDesc.Height, 4, data, map.RowPitch);
    
    context->Unmap(stagingTex, 0);

    hr = duplication->ReleaseFrame();
    if (S_OK != hr) {
        printf("Failed to release the duplication frame.\n");
    }
}



HRESULT CompileComputeShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint,
    _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob)
{
    if (!srcFile || !entryPoint || !device || !blob)
        return E_INVALIDARG;

    *blob = nullptr;

    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

    // We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
    LPCSTR profile = (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0) ? "cs_5_0" : "cs_4_0";

    const D3D_SHADER_MACRO defines[] =
    {
        "EXAMPLE_DEFINE", "1",
        NULL, NULL
    };

    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint, profile,
        flags, 0, &shaderBlob, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob)
        {
            OutputDebugStringA((char*)errorBlob->GetBufferPointer());
            errorBlob->Release();
        }

        if (shaderBlob)
            shaderBlob->Release();

        return hr;
    }

    *blob = shaderBlob;

    return hr;
}


void screenReader::initCompute()
{
    HRESULT hr;
    D3D11_BUFFER_DESC bufferDescGPU;
    bufferDescGPU.ByteWidth = sizeof(computeReadback);
    bufferDescGPU.ByteWidth = ((bufferDescGPU.ByteWidth + 255) / 256) * 256;    // pad to 256
    bufferDescGPU.Usage = D3D11_USAGE_DEFAULT;
    bufferDescGPU.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
    bufferDescGPU.CPUAccessFlags = 0;
    bufferDescGPU.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
    bufferDescGPU.StructureByteStride = 0;
    hr = device->CreateBuffer(&bufferDescGPU, NULL, &pBufferComputeReadbackGPU);

    D3D11_BUFFER_DESC bufferDescCPU;
    bufferDescCPU.ByteWidth = sizeof(computeReadback);
    bufferDescCPU.ByteWidth = ((bufferDescCPU.ByteWidth + 255) / 256) * 256;    // pad to 256
    bufferDescCPU.Usage = D3D11_USAGE_STAGING;
    bufferDescCPU.BindFlags = 0;
    bufferDescCPU.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    bufferDescCPU.MiscFlags = 0;
    bufferDescCPU.StructureByteStride = 0;
    hr = device->CreateBuffer(&bufferDescCPU, NULL, &pBufferComputeReadbackCPU);

    D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
    uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
    uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
    uavDesc.Buffer.FirstElement = 0;
    uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
    uavDesc.Buffer.NumElements = ((bufferDescGPU.ByteWidth + 255) / 256) * 256 / 4;    // pad to 256
    hr = device->CreateUnorderedAccessView(pBufferComputeReadbackGPU, &uavDesc, &pComputeReadbackUAV);

    // Compile shader
    ID3DBlob* csBlob = nullptr;
    hr = CompileComputeShader(L"computeCycle.hlsl", "main", device, &csBlob);
    if (FAILED(hr))
    {
        device->Release();
        printf("Failed compiling shader %08X\n", hr);
    }

    // Create shader
    hr = device->CreateComputeShader(csBlob->GetBufferPointer(), csBlob->GetBufferSize(), nullptr, &computeShader);

    csBlob->Release();

    if (FAILED(hr))
    {
        device->Release();
    }

    printf("Success\n");
}


void screenReader::tickCompute()
{
    HRESULT hr;

    if (bComputeIsMapped)
    {
        context->Unmap(pBufferComputeReadbackCPU, 0);
        bComputeIsMapped = false;
    }

    hr = duplication->AcquireNextFrame(1000, &frameInfo, &desktopDXGIResource);
    if (DXGI_ERROR_ACCESS_LOST == hr) {
        printf("Received a DXGI_ERROR_ACCESS_LOST.\n");
        return;
    }
    else if (DXGI_ERROR_WAIT_TIMEOUT == hr) {
        printf("Received a DXGI_ERROR_WAIT_TIMEOUT.\n");
        return;
    }
    else if (DXGI_ERROR_INVALID_CALL == hr) {
        printf("Received a DXGI_ERROR_INVALID_CALL.\n");
        return;
    }

    hr = desktopDXGIResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&screenTex);
    if (S_OK != hr) {
        printf("Error: failed to query the ID3D11Texture2D interface on the IDXGIResource we got.\n");
        exit(EXIT_FAILURE);
    }

    hr = desktopDXGIResource->QueryInterface(__uuidof(ID3D11Resource), (void**)&desktopD3D11Resource);
    if (S_OK != hr) {
        printf("Error: failed to query the ID3D11Texture2D interface on the IDXGIResource we got.\n");
        exit(EXIT_FAILURE);
    }

    hr = device->CreateShaderResourceView(desktopD3D11Resource, &desktopSRVDesc, &desktopSRV);

    context->CSSetShader(computeShader, NULL, 0);
    context->CSSetShaderResources(0, 1, &desktopSRV);
    context->CSSetUnorderedAccessViews(0, 1, &pComputeReadbackUAV, NULL);

    context->Dispatch(1, 1, 1);

    desktopSRV->Release();

    context->CopyResource(pBufferComputeReadbackCPU, pBufferComputeReadbackGPU);

    hr = context->Map(pBufferComputeReadbackCPU, 0, D3D11_MAP_READ, 0, &pComputeMapped);
    bComputeIsMapped = true;

    pReadback = (computeReadback*)pComputeMapped.pData;

    hr = duplication->ReleaseFrame();
    if (S_OK != hr) {
        printf("Failed to release the duplication frame.\n");
    }

    if (pReadback->windowPosX > 0)
    {
        int a = 1;
    }

}