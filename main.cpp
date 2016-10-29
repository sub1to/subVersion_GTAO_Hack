/*
	Copyright 2016 sub1to

	This file is part of subVersion GTA:O SC External Hack.

    subVersion GTA:O SC External Hack is free software: you can redistribute
	it and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    subVersion GTA:O SC External Hack is distributed in the hope that it
	will be useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
	the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with subVersion GTA:O SC External Hack.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stdafx.h"

//global vars
HWND		g_hWnd;
hack*		g_pHack;
settings*	g_pSettings;
memManager*	g_pMemMan;
D3D9Render*	g_pD3D9Render;

bool		g_bKillSwitch	= false;
bool		g_bKillRender	= false;
bool		g_bKillAttach	= false;

//fuction prototypes
LRESULT	CALLBACK	WindowProc(	HWND	hWnd,
								UINT	message,
								WPARAM	wParam,
								LPARAM	lParam);
DWORD __stdcall		threadAttach(LPVOID lpParam);
DWORD __stdcall		threadRender(LPVOID lpParam);
DWORD __stdcall		threadHack	(LPVOID lpParam);

int WINAPI WinMain(	HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPSTR		lpCmdLine,
					int			nCmdShow)
{
	g_pMemMan		= new memManager;
	g_pSettings		= new settings;
	g_pD3D9Render	= new D3D9Render;
	g_pHack			= new hack;

	//LPCSTR	szWindowTitleTarget	= "Untitled - Notepad";
	LPCSTR	szWindowTitleTarget	= "Grand Theft Auto V";
	LPCSTR	szWindowTitle		= "subVersion menu [unknowncheats.me]";
	g_pMemMan->setWindowName(szWindowTitleTarget);

	g_pSettings->addFeatureCategory("Player");		//0
	g_pSettings->addFeatureCategory("Weapon");		//1
	g_pSettings->addFeatureCategory("Teleport");	//2

	g_pSettings->addFeature(0, "Godmode", feat_toggle, "godMode");								//0
	g_pSettings->addFeature(0, "Never Wanted", feat_toggle, "neverWanted");						//1
	g_pSettings->addFeature(0, "Anti NPC", feat_toggle, "antiNpc");								//2
	g_pSettings->addFeature(0, "Vehicle Godmode", feat_toggle, "vehGodMode");					//3
	g_pSettings->addFeature(1, "No Spread", feat_toggle, "noSpread");							//4
	g_pSettings->addFeature(1, "No Recoil", feat_toggle, "noRecoil");							//5
	g_pSettings->addFeature(1, "Quick Reload", feat_slider, "quickReload", 1.f, 10.f);			//6
	g_pSettings->addFeature(1, "Bullet Damage", feat_slider, "bulletDamage", 1.f, 10.f);		//7
	g_pSettings->addFeature(1, "Infinite Ammo", feat_toggle, "infAmmo");						//8
	g_pSettings->addFeature(1, "Range", feat_slider, "weapRange", 1.f, 5.f);					//9
	g_pSettings->addFeature(1, "No Spin-Up", feat_toggle, "weapSpin");							//10
	g_pSettings->addFeature(2, "Waypoint", feat_teleport, tp_waypoint);
	g_pSettings->addFeature(2, "Position 1", feat_teleport, "pos0", tp_saved);
	g_pSettings->addFeature(2, "Position 2", feat_teleport, "pos1", tp_saved);
	g_pSettings->addFeature(2, "Position 3", feat_teleport, "pos2", tp_saved);
	g_pSettings->addFeature(2, "Position 4", feat_teleport, "pos3", tp_saved);
	g_pSettings->addFeature(2, "Position 5", feat_teleport, "pos4", tp_saved);
	g_pSettings->addFeature(2, "Position 6", feat_teleport, "pos5", tp_saved);

	g_pSettings->setActiveCat(0);			//this needs to be called so we can full the current feature buffer

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "sub1toOverlay";

	RegisterClassEx(&wc);
	g_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,
							"sub1toOverlay",    // name of the window class
							szWindowTitle,   // title of the window
							WS_POPUP,    // window style
							0,    // x-position of the window
							0,    // y-position of the window
							800,    // width of the window
							600,    // height of the window
							NULL,    // we have no parent window, NULL
							NULL,    // we aren't using menus, NULL
							NULL,    // application handle
							NULL);    // used with multiple windows, NULL

	SetLayeredWindowAttributes(g_hWnd, 0, 1, LWA_ALPHA);
	SetLayeredWindowAttributes(g_hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);

	ShowWindow(g_hWnd, SW_SHOW);

	CreateThread(	NULL,
					0,
					threadAttach,
					NULL,
					0,
					nullptr);
	Sleep(100);
	CreateThread(	NULL,
					0,
					threadRender,
					NULL,
					0,
					nullptr);
	Sleep(100);
	CreateThread(	NULL,
					0,
					threadHack,
					NULL,
					0,
					nullptr);

	MSG msg;	// this struct holds Windows event messages
	while(true)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Check to see if any messages are waiting in the queue
		{
			TranslateMessage(&msg);		// Translate the message and dispatch it to WindowProc()
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)	// If the message is WM_QUIT, exit the while loop
			break;

		Sleep(100);
	}

	return (int) msg.wParam;
}

//main message handler
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:		// this message is read when the window is closed
			PostQuitMessage(0);
			return 0;
		break;
	}

	return DefWindowProc (hWnd, message, wParam, lParam);	// Handle any messages the switch statement didn't
}

DWORD __stdcall threadAttach(LPVOID lpParam)
{
	while(!g_bKillSwitch)
	{
		if(g_pMemMan->attach() && g_pMemMan->findWindow())
		{
			if(g_pD3D9Render->getViewport())
				MoveWindow(g_hWnd, g_pD3D9Render->m_screen.x, g_pD3D9Render->m_screen.y, g_pD3D9Render->m_screen.w, g_pD3D9Render->m_screen.h, true);

			if(g_hWnd == GetForegroundWindow())
			{
				ShowWindow(g_pMemMan->getWindow(), SW_SHOW);
				SetForegroundWindow(g_pMemMan->getWindow());
			}
		}
		else
			MessageBox(nullptr, "Make sure the game is running!", "subVersion failed to attach", MB_OK | MB_ICONERROR);

		Sleep(0x60);
	}
	g_bKillAttach	= true;
	return 1;
}

DWORD __stdcall threadRender(LPVOID lpParam)
{
	g_pD3D9Render->init(g_hWnd);
	while(!g_bKillSwitch)
	{
		g_pD3D9Render->render();
		Sleep(0x10);
	}
	g_bKillRender	= true;
	return 0;
}

DWORD __stdcall threadHack(LPVOID lpParam)
{
	g_pHack->m_hModule = g_pMemMan->getModuleAddress("GTA5.exe");
	while(!g_bKillSwitch)
	{
		g_pHack->initPointers();
		g_pHack->checkKeys();

		if(g_pSettings->getFeature(0)->m_bOn)
			g_pHack->restoreHealth();
		if(g_pSettings->getFeature(1)->m_bOn)
			g_pHack->notWanted();
		if(g_pSettings->getFeature(2)->m_bOn)
			g_pHack->killNpc();
		if(g_pSettings->getFeature(3)->m_bOn)
			g_pHack->restoreVehicleHealth();

		if(g_pHack->loadWeapon())
		{
			g_pHack->noSpread(!g_pSettings->getFeature(4)->m_bOn);
			g_pHack->noRecoil(!g_pSettings->getFeature(5)->m_bOn);
			g_pHack->quickReload(!g_pSettings->getFeature(6)->m_bOn);
			g_pHack->bulletDamage(!g_pSettings->getFeature(7)->m_bOn);
			g_pHack->weaponRange(!g_pSettings->getFeature(9)->m_bOn);
			g_pHack->weaponSpin(!g_pSettings->getFeature(10)->m_bOn);
		}
		if(g_pSettings->getFeature(8)->m_bOn)
			g_pHack->fillAmmo();

		Sleep(1);
	}
	if(g_pHack->loadWeapon())
		g_pHack->m_weapon.restoreWeapon();
	return 0;
}

//Kill the program; Prototype in stdafx.h
void	killProgram()
{
	g_bKillSwitch = true;				//enable thread killswitch
	g_pSettings->m_iniParser.write();	//save options

	//make sure we shut down all threads before deleting the objects
	while(!g_bKillAttach || !g_bKillRender)
		Sleep(1);

	delete	g_pHack;
	delete	g_pD3D9Render;
	delete	g_pSettings;
	delete	g_pMemMan;

	exit(0);
}