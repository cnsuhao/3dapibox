#include <DXUT.h>


LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL;

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)


bool CALLBACK IsD3D9DeviceAcceptable(
	D3DCAPS9* pCaps,
	D3DFORMAT AdapterFormat,
	D3DFORMAT BackBufferFormat,
	bool bWindowed,
	void* pUserContext)
{
	// Typically want to skip back buffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if (FAILED(pD3D->CheckDeviceFormat(pCaps->AdapterOrdinal, pCaps->DeviceType,
				AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
				D3DRTYPE_TEXTURE, BackBufferFormat)))
		return false;

	return true;
}

// Before a device is created, modify the device settings as needed
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}

// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice,
																		const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
																		void* pUserContext)
{
	return S_OK;
}

// create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT)
// or that are tied to the back buffer size
HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice,
																	 const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
																	 void* pUserContext)
{
	HRESULT hr;
	CUSTOMVERTEX vertices[] = {
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{  50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	V( pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0,
		D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &g_pVB, NULL) );

	VOID* pVertices;
	V( g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0) );
	memcpy(pVertices, vertices, sizeof(vertices));
	V( g_pVB->Unlock() );

	return S_OK;
}

// Handle updates to the scene. This is called regardless of which D3D API is used
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
}

// Render the scene using the D3D9 device
void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime,
	float fElapsedTime, void* pUserContext)
{
	HRESULT hr;

	// Clear the render target and the zbuffer
	V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0) );

	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		V( pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX)) );
		V( pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX) );
		V( pd3dDevice->DrawPrimitive(D3DPT_POINTLIST, 0, 3) );

		V( pd3dDevice->EndScene() );
	}
}

// Handle messages to the application
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
												 bool* pbNoFurtherProcessing, void* pUserContext)
{
	return 0;
}

// Release D3D9 resources created in the OnD3D9ResetDevice callback
void CALLBACK OnD3D9LostDevice(void* pUserContext)
{
	SAFE_RELEASE(g_pVB);
}

// Release D3D9 resources created in OnD3D9CreateDevice callback
void CALLBACK OnD3D9DestroyDevice(void* pUserContext)
{
}


int main(int argc, char* argv[])
{
	// Set the callback functions
	DXUTSetCallbackD3D9DeviceAcceptable(IsD3D9DeviceAcceptable);
	DXUTSetCallbackD3D9DeviceCreated(OnD3D9CreateDevice);
	DXUTSetCallbackD3D9DeviceReset(OnD3D9ResetDevice);
	DXUTSetCallbackD3D9FrameRender(OnD3D9FrameRender);
	DXUTSetCallbackD3D9DeviceLost(OnD3D9LostDevice);
	DXUTSetCallbackD3D9DeviceDestroyed(OnD3D9DestroyDevice);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackFrameMove(OnFrameMove);

	// Initialize DXUT and create the desired Win32 window and D3D device for the application
	DXUTInit(true, false); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling(true, true, true); // handle the default hotkeys
	DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
	DXUTCreateWindow(L"02_HelloDotDx9");
	DXUTCreateDevice(true, 800, 600);

	DXUTMainLoop();

	return DXUTGetExitCode();
}
