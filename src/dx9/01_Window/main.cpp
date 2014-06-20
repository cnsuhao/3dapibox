#include <DXUT.h>

int main(int argc, char* argv[])
{
	DXUTInit(true, false);
	DXUTCreateWindow(L"01_WindowDx9");
	DXUTCreateDevice(true, 800, 600);

	DXUTMainLoop();

	return DXUTGetExitCode();
}
