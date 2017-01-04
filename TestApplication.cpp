/*---------------------------------------------------------------------
*
* Copyright © 2015  Minsi Chen
* E-mail: m.chen@derby.ac.uk
*
* The source is written for the Graphics I and II modules. You are free
* to use and extend the functionality. The code provided here is functional
* however the author does not guarantee its performance.
---------------------------------------------------------------------*/
#include "TestApplication.h"
#include "AppWindow.h"
#include "Resource.h"
#include <Windowsx.h>

TestApplication* TestApplication::s_oglapp = NULL;

TestApplication::TestApplication()
{
	m_appwnd = NULL;
	m_hInst = 0;
	m_terminate = FALSE;
}

TestApplication::~TestApplication()
{
	if ( m_appwnd )
		delete m_appwnd;
}

BOOL TestApplication::MyRegisterClass(HINSTANCE hinst)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= this->WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinst;
	wcex.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_OGLWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"AppWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	
	if ( !RegisterClassEx ( &wcex ) )
		return FALSE;

	return TRUE;
}

TestApplication* TestApplication::CreateApplication(HINSTANCE hinst)
{
	if ( ! s_oglapp )
	{
		s_oglapp = new TestApplication();

		s_oglapp->m_hInst = hinst;
		s_oglapp->MyRegisterClass(hinst);

		//Now create an OGLWindow for this application
		s_oglapp->CreateApplicationWindow(1280,720);
	}

	return s_oglapp;
}

void TestApplication::DestroyApplication()
{
	if ( s_oglapp )
		delete s_oglapp;
}

TestApplication* TestApplication::GetApplication()
{
	return s_oglapp;
}

void TestApplication::CreateApplicationWindow( int width, int height )
{
	if ( !m_appwnd  )
	{
		m_appwnd = new AppWindow();
		if ( m_appwnd->InitWindow(m_hInst, width, height) )
			m_appwnd->SetVisible(TRUE);
	}
}

int TestApplication::Run()
{
	MSG msg;

	while ( !m_terminate )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//peek for windows message
			if ( msg.message == WM_QUIT )
			{
				Kill();
			}
			else
			{
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
		}
		else
		{
			//get the AppWindow to render stuff;
			m_appwnd->Render();
		}
		
	}

	return (int) msg.wParam;
}

void TestApplication::Kill()
{
	m_terminate = TRUE;
}

LRESULT CALLBACK TestApplication::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch ( msg )
	{
		case WM_SIZE:
			s_oglapp->GetApplicationWindow()->Resize( LOWORD(lparam), HIWORD(lparam) );
			break;

		case WM_CLOSE:
			s_oglapp->GetApplicationWindow()->DestroyOGLWindow();
			break;

		case WM_MOUSEMOVE:
			//inform the cursor position to OGLWindow
			s_oglapp->GetApplicationWindow()->MouseMove( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONUP:
			s_oglapp->GetApplicationWindow()->MouseLBUp( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONDOWN:
			s_oglapp->GetApplicationWindow()->MouseLBDown( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;
		case WM_KEYUP:
			s_oglapp->GetApplicationWindow()->KeyUp(wparam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_PAINT:
			s_oglapp->GetApplicationWindow()->Render();
			break;
		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
	}

	return 0;
}
