#pragma once

#include "d3d11.h"
#include "dxgi1_2.h"
#include <string>

struct computeReadback
{
	int windowPosX;
	int windowPosY;
	int playButtonFound;
	int playButtonX;
	int playButtonY;
	int hellButtonFound;
	int hellButtonX;
	int hellButtonY;
	int townWaypointFound;
	int townWaypointX;
	int townWaypointY;
	int lostCityButtonFound;
	int lostCityButtonX;
	int lostCityButtonY;
	int tunnelsEntranceFound;
	int tunnelsEntranceX;
	int tunnelsEntranceY;
	int mercFound;
	int mercX;
	int mercY;
	int plagueBearerFound;
	int plagueBearerX;
	int plagueBearerY;
	int embalmedFound;
	int embalmedX;
	int embalmedY;
	int invaderFound;
	int invaderX;
	int invaderY;
	int horrorMageFound;
	int horrorMageX;
	int horrorMageY;
	int itemFound;
	int itemX;
	int itemY;
	int chestFound;
	int chestX;
	int chestY;
	int mercHealth;
	int playerHealth;
	int playerMP;
	int potionSlot1Type;
	int potionSlot2Type;
	int potionSlot3Type;
	int potionSlot4Type;
	int lostCityEdgeLeft;
	int lostCityEdgeRight;
	int lostCityEdgeTop;
	int lostCityEdgeBottom;
	int tunnelsEdgeLeft;
	int tunnelsEdgeRight;
	int tunnelsEdgeTop;
	int tunnelsEdgeBottom;
	int saveAndQuitButtonFound;
	int saveAndQuitButtonX;
	int saveAndQuitButtonY;
};

class screenReader
{
public:

	IDXGIFactory1* factory1 = NULL;
	IDXGIAdapter1* adapter1 = NULL;
	IDXGIOutput* output = NULL;
	DXGI_OUTPUT_DESC outputDesc;
	IDXGIOutput1* output1 = NULL;
	ID3D11Device* device = NULL;
	ID3D11DeviceContext* context = NULL;
	D3D_FEATURE_LEVEL featureLevel;
	IDXGIOutputDuplication* duplication = NULL;
	D3D11_TEXTURE2D_DESC stagingTexDesc;
	ID3D11Texture2D* stagingTex = NULL;
	DXGI_OUTDUPL_DESC duplicationDesc;

	DXGI_OUTDUPL_FRAME_INFO frameInfo;
	IDXGIResource* desktopDXGIResource = NULL;
	ID3D11Resource* desktopD3D11Resource = NULL;
	ID3D11ShaderResourceView* desktopSRV = NULL;
	D3D11_SHADER_RESOURCE_VIEW_DESC desktopSRVDesc;
	ID3D11Texture2D* screenTex = NULL;
	DXGI_MAPPED_RECT mappedRect;

	ID3D11Buffer* pBufferComputeReadbackGPU;
	ID3D11Buffer* pBufferComputeReadbackCPU;
	ID3D11UnorderedAccessView* pComputeReadbackUAV;
	D3D11_MAPPED_SUBRESOURCE pComputeMapped;
	bool bComputeIsMapped = false;

	computeReadback* pReadback = NULL;

	ID3D11ComputeShader* computeShader = nullptr;

	RECT desktopCoordinates;

	void init();
	void terminate();
	void tickCompute();

	void saveScreenToPNG(std::string pngName);

private:
	void initDuplication();
	void initCompute();
};

extern screenReader g_screen;