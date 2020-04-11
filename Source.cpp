#include <iostream>
#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")


LPDIRECT3D9 direct = NULL;
PDIRECT3DDEVICE9 direct_device = NULL;
D3DPRESENT_PARAMETERS direct_params = { };
D3DCOLOR clear_color = D3DCOLOR_RGBA(0, 0, 0, 0);


LRESULT CALLBACK WindowProcess(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);
BOOL CALLBACK EnumWindowsProc(HWND handle, LPARAM lparam);
void Draw();


BOOL CALLBACK EnumWindowsProc(HWND handle, LPARAM lparam)
{
	HWND defview_handle = FindWindowEx(handle, NULL, "SHELLDLL_DefView", NULL);
	HWND* return_pointer = (HWND*)lparam;
	if (defview_handle) *return_pointer = FindWindowEx(NULL, handle, "WorkerW", NULL);
	return true;
}

LRESULT CALLBACK WindowProcess(HWND handle, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		direct_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_color, 1.0f, 0);

		if (direct_device->BeginScene() >= 0)
		{
			Draw();
			direct_device->EndScene();
		}

		HRESULT present_result = direct_device->Present(NULL, NULL, NULL, NULL);
		if (present_result == D3DERR_DEVICELOST && direct_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			direct_device->Reset(&direct_params);
		}
		break;
	}

	default:
		return DefWindowProc(handle, message, wparam, lparam);
	}
	return 0;
}

int main()
{
	std::cout << "DirectX Background" << std::endl << std::endl;
	
	std::cout << "[+] Getting Progman handle" << std::endl;
	HWND progman_handle = FindWindowEx(0, 0, "Progman", 0);
	if (progman_handle == NULL)
	{
		std::cout << "[-] Couldn't get Progman handle" << std::endl;
		return 0;
	}

	std::cout << "[+] Sending WM_SPAWN message" << std::endl;
	SendMessageA(progman_handle, 0x052C, 0, 0);

	HWND workerw_handle = 0;
	std::cout << "[+] Getting WorkerW handle" << std::endl;
	EnumWindows(&EnumWindowsProc, reinterpret_cast<LPARAM>(&workerw_handle));
	if (workerw_handle == NULL)
	{
		std::cout << "[-] Couldn't get WorkerW handle" << std::endl;
		return 0;
	}
	
	WNDCLASS window_class = { 0 };
	window_class.hbrBackground = (HBRUSH)CreateSolidBrush(clear_color);
	window_class.hInstance = NULL;
	window_class.lpfnWndProc = WindowProcess;
	window_class.lpszClassName = "DXBG";
	window_class.style = CS_HREDRAW | CS_VREDRAW;

	std::cout << "[+] Registering window class" << std::endl;
	if (!RegisterClass(&window_class))
	{
		std::cout << "[-] Couldn't register window class" << std::endl;
		return 0;
	}
	
	std::cout << "[+] Creating DXBG window" << std::endl;
	HWND window_handle = CreateWindow("DXBG", NULL, WS_POPUP, 0, 0, GetSystemMetrics(SM_CXSCREEN) * 2, GetSystemMetrics(SM_CYSCREEN) * 2, NULL, NULL, NULL, NULL);
	if (window_handle == NULL)
	{
		std::cout << "[-] Couldn't create DXBG window" << std::endl;
		return 0;
	}

	std::cout << "[+] Creating D3D9" << std::endl;
	direct = Direct3DCreate9(D3D_SDK_VERSION);
	if (direct == NULL)
	{
		std::cout << "[-] Couldn't create D3D9" << std::endl;
		return 0;
	}

	ZeroMemory(&direct_params, sizeof(direct_params));
	direct_params.Windowed = TRUE;
	direct_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	direct_params.BackBufferFormat = D3DFMT_UNKNOWN;
	direct_params.EnableAutoDepthStencil = TRUE;
	direct_params.AutoDepthStencilFormat = D3DFMT_D16;
	direct_params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	
	std::cout << "[+] Creating D3D9 device" << std::endl;
	if (direct->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window_handle, D3DCREATE_HARDWARE_VERTEXPROCESSING, &direct_params, &direct_device) < 0)
	{
		std::cout << "[-] Couldn't create D3D9 device" << std::endl;
		return 0;
	}

	std::cout << "[+] Showing window" << std::endl;
	ShowWindow(window_handle, SW_RESTORE);
	SetParent(window_handle, workerw_handle);

	std::cout << "[+] DXBG ready" << std::endl;

	MSG message;
	while (GetMessage(&message, NULL, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	direct_device->Release();
	direct->Release();
	DeleteObject(window_handle);

	return 0;
}

// Sample Drawing

int x = 1;
int y = 1;
int velocity_x = 16;
int velocity_y = 16;

void Draw()
{
	if (x <= 0 || x >= GetSystemMetrics(SM_CXSCREEN) - 100) velocity_x *= -1;
	if (y <= 0 || y >= GetSystemMetrics(SM_CYSCREEN) - 100) velocity_y *= -1;

	D3DRECT test_rect = { x, y, x + 100, y + 100 };
	direct_device->Clear(1, &test_rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 0, 0);

	x += velocity_x;
	y += velocity_y;
}