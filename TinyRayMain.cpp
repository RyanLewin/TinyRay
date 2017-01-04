/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
// OGLWin32.cpp : Defines the entry point for the application.
//

#include "TinyRayMain.h"
#include <stdio.h>
#include <io.h>
#include <strsafe.h>
#include <fcntl.h>

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "TestApplication.h"

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

void PrintUsage()
{
	printf("Use F1 - F6 keys to switch ray trace complexity\n");
	printf("F1: Ambient only\n");
	printf("F2: Full lighting no shadow, reflection and transmission\n");
	printf("F3: Full lighting with shadow\n");
	printf("F4: Full lighting  reflection\n");
	printf("F5: Full lighting  refraction\n");
	printf("F6: Ray trace everything (default)\n");
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR    lpCmdLine,
                     int       nCmdShow)
{
	int exitcode = 0;
	//Allocate a console window
	//so that messages can be redirected to stdout
	if (!AllocConsole())
	{
		ErrorExit(TEXT("AllocConsole"));
	}

	FILE* pf_out;
	freopen_s(&pf_out, "CONOUT$", "w", stdout);

	PrintUsage();

	//Create the application instance
	TestApplication* myapp = TestApplication::CreateApplication(hInstance);

	exitcode = myapp->Run();

	myapp->DestroyApplication();

	fclose(pf_out);

	//Free the console window
	if (!FreeConsole())
	{
		ErrorExit(TEXT("FreeConsole"));
	}

	return exitcode;
}
