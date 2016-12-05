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
int			g_iFeature[MAX_MENU_FEATURES]	= {};

bool		g_bKillSwitch	= false;
bool		g_bKillRender	= false;
bool		g_bKillAttach	= false;

//fuction prototypes
LRESULT	__stdcall	WindowProc(	HWND	hWnd,
								UINT	message,
								WPARAM	wParam,
								LPARAM	lParam);
DWORD __stdcall		threadAttach(LPVOID lpParam);
DWORD __stdcall		threadRender(LPVOID lpParam);
DWORD __stdcall		threadHack	(LPVOID lpParam);

int __stdcall WinMain(	HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPSTR		lpCmdLine,
					int			nCmdShow)
{
	if(FindWindow("sub1toOverlay", nullptr))	//make sure the hack is not already running
		exit(0);

	g_pMemMan		= new memManager;
	g_pSettings		= new settings;
	g_pD3D9Render	= new D3D9Render;
	g_pHack			= new hack;

	//LPCSTR	szWindowTitleTarget	= "Untitled - Notepad";
	LPCSTR	szWindowTitleTarget	= "Grand Theft Auto V";
	LPCSTR	szWindowTitle		= "subVersion [unknowncheats.me]";
	g_pMemMan->setWindowName(szWindowTitleTarget);

	g_pSettings->addFeatureCategory("Player");		//0
	g_pSettings->addFeatureCategory("Weapon");		//1
	g_pSettings->addFeatureCategory("Vehicle");		//2
	g_pSettings->addFeatureCategory("Teleport");	//3
	

	g_iFeature[FEATURE_P_TRUEGOD]			= g_pSettings->addFeature(0, "God", feat_toggle, "trueGodMode");
	g_iFeature[FEATURE_P_GOD]				= g_pSettings->addFeature(0, "Demi-God", feat_toggle, "godMode");
	g_iFeature[FEATURE_P_WANTED]			= g_pSettings->addFeature(0, "Wanted", feat_slider, "wanted", 0.f, 5.f, .2f);
	g_iFeature[FEATURE_P_NEVERWANTED]		= g_pSettings->addFeature(0, "Never Wanted", feat_toggle, "neverWanted");
	g_iFeature[FEATURE_P_ANTINPC]			= g_pSettings->addFeature(0, "Anti NPC", feat_toggle, "antiNpc");
	g_iFeature[FEATURE_P_RUNSPD]			= g_pSettings->addFeature(0, "Run Speed", feat_slider, "runSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SWIMSPD]			= g_pSettings->addFeature(0, "Swim Speed", feat_slider, "swimSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SUPERJUMP]			= g_pSettings->addFeature(0, "Super Jump", feat_toggle, "superJump");
	g_iFeature[FEATURE_P_EXPLOSIVEMELEE]	= g_pSettings->addFeature(0, "Explosive Melee", feat_toggle, "explMelee");
	g_iFeature[FEATURE_P_NORAGDOLL]			= g_pSettings->addFeature(0, "No Ragdoll", feat_toggle, "noRagdoll");
	g_iFeature[FEATURE_W_SPREAD]			= g_pSettings->addFeature(1, "No Spread", feat_toggle, "noSpread");	
	g_iFeature[FEATURE_W_RECOIL]			= g_pSettings->addFeature(1, "No Recoil", feat_toggle, "noRecoil");	
	g_iFeature[FEATURE_W_NORELOAD]			= g_pSettings->addFeature(1, "No Reload", feat_toggle, "noReload");
	g_iFeature[FEATURE_W_RELOAD]			= g_pSettings->addFeature(1, "Quick Reload", feat_slider, "quickReload", 1.f, 10.f);
	g_iFeature[FEATURE_W_DAMAGE]			= g_pSettings->addFeature(1, "Bullet Damage", feat_slider, "bulletDamage", 1.f, 10.f);
	g_iFeature[FEATURE_W_AMMO]				= g_pSettings->addFeature(1, "Infinite Ammo", feat_toggle, "infAmmo");
	g_iFeature[FEATURE_W_RANGE]				= g_pSettings->addFeature(1, "Range", feat_slider, "weapRange", 1.f, 10.f);
	g_iFeature[FEATURE_W_SPINUP]			= g_pSettings->addFeature(1, "No Spin-Up", feat_toggle, "weapSpin");
	g_iFeature[FEATURE_W_EXPLOSIVEAMMO]		= g_pSettings->addFeature(1, "Explosive Ammo", feat_toggle, "explAmmo");
	g_iFeature[FEATURE_W_FIREAMMO]			= g_pSettings->addFeature(1, "Fire Ammo", feat_toggle, "fireAmmo");
	g_iFeature[FEATURE_W_BULLET_BATCH]		= g_pSettings->addFeature(1, "Bullets per Shot", feat_slider, "bulletBatch", 1.f, 10.f, (float) 1.f / 9.f);
	g_iFeature[FEATURE_W_MUZZLE_VELOCITY]	= g_pSettings->addFeature(1, "Muzzle Velocity", feat_slider, "muzzleVelo", 1.f, 10.f);
	g_iFeature[FEATURE_V_TRUEGOD]			= g_pSettings->addFeature(2, "God", feat_toggle, "vehTrueGodMode");
	g_iFeature[FEATURE_V_GOD]				= g_pSettings->addFeature(2, "Demi-God", feat_toggle, "vehGodMode");
	g_iFeature[FEATURE_V_SEATBELT]			= g_pSettings->addFeature(2, "Seatbelt", feat_toggle, "seatbelt");
	g_iFeature[FEATURE_V_DEFORMATION]		= g_pSettings->addFeature(2, "Deformation", feat_slider, "vehDeform", 0.f, 1.f);
	g_iFeature[FEATURE_V_ACCELERATION]		= g_pSettings->addFeature(2, "Acceleration", feat_slider, "vehAccel", 1.f, 10.f);
	g_iFeature[FEATURE_V_BRAKEFORCE]		= g_pSettings->addFeature(2, "Brake force", feat_slider, "vehBrakeForce", 1.f, 10.f);
	g_iFeature[FEATURE_V_TRACTION]			= g_pSettings->addFeature(2, "Traction", feat_slider, "vehTraction", 1.f, 2.f);
	g_iFeature[FEATURE_V_GRAVITY]			= g_pSettings->addFeature(2, "Gravity", feat_slider, "vehGravity", 0.f, 25.f);
	g_iFeature[FEATURE_V_BULLETPROOFTIRES]	= g_pSettings->addFeature(2, "Bulletproof Tires", feat_toggle, "vehBulletproofTires");
	g_pSettings->addFeature(3, "Waypoint", feat_teleport, tp_waypoint);
	g_pSettings->addFeature(3, "Position 1", feat_teleport, "pos0", tp_saved);
	g_pSettings->addFeature(3, "Position 2", feat_teleport, "pos1", tp_saved);
	g_pSettings->addFeature(3, "Position 3", feat_teleport, "pos2", tp_saved);
	g_pSettings->addFeature(3, "Position 4", feat_teleport, "pos3", tp_saved);
	g_pSettings->addFeature(3, "Position 5", feat_teleport, "pos4", tp_saved);
	g_pSettings->addFeature(3, "Position 6", feat_teleport, "pos5", tp_saved);

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
	wc.hbrBackground = dynamic_cast<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0)));//(HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "sub1toOverlay";

	RegisterClassEx(&wc);
	g_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,		//dwExStyle [in]
							"sub1toOverlay",										//lpClassName [in, optional]
							szWindowTitle,											//lpWindowName [in, optional]
							WS_POPUP,												//dwStyle [in]
							0,														//x [in]
							0,														//y [in]
							300,													//nWidth [in]
							300,													//nHeight [in]
							nullptr,												//hWndParent [in, optional]
							nullptr,												//hMenu [in, optional]
							hInstance,												//hInstance [in, optional]		A handle to the instance of the module to be associated with the window.
							nullptr);												//lpParam [in, optional]

	SetLayeredWindowAttributes(g_hWnd, 0, 0, LWA_ALPHA);
	SetLayeredWindowAttributes(g_hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	MARGINS margins {1, 1, 1, 1};
	DwmExtendFrameIntoClientArea(g_hWnd, &margins);			//aero fix

	ShowWindow(g_hWnd, SW_SHOWNORMAL);

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

	MSG msg;
	while(true)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Check to see if any messages are waiting in the queue
		{
			TranslateMessage(&msg);		//Translate the message and dispatch it to WindowProc()
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;

		Sleep(100);
	}

	return (int) msg.wParam;
}

//main message handler
LRESULT __stdcall WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;
	}

	return DefWindowProc (hWnd, message, wParam, lParam); //default behaviour for any unhandled messages
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

		if(g_pSettings->getFeature(g_iFeature[FEATURE_P_GOD])->m_bOn)
			g_pHack->restoreHealth();
		if(g_pSettings->getFeature(g_iFeature[FEATURE_V_GOD])->m_bOn)
			g_pHack->restoreVehicleHealth();
		if(g_pSettings->getFeature(g_iFeature[FEATURE_P_ANTINPC])->m_bOn)
			g_pHack->killNpc();
		if(g_pSettings->getFeature(g_iFeature[FEATURE_P_NEVERWANTED])->m_bOn)
			g_pHack->notWanted();
		g_pHack->neverWanted(g_pSettings->getFeature(g_iFeature[FEATURE_P_NEVERWANTED]));
		g_pHack->wanted(g_pSettings->getFeature(g_iFeature[FEATURE_P_WANTED]));
		g_pHack->runSpeed(g_pSettings->getFeature(g_iFeature[FEATURE_P_RUNSPD]));
		g_pHack->swimSpeed(g_pSettings->getFeature(g_iFeature[FEATURE_P_SWIMSPD]));
		g_pHack->godMode(g_pSettings->getFeature(g_iFeature[FEATURE_P_TRUEGOD]));
		g_pHack->vehicleGod(g_pSettings->getFeature(g_iFeature[FEATURE_V_TRUEGOD]));
		g_pHack->noReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_NORELOAD]));
		g_pHack->noRagdoll(g_pSettings->getFeature(g_iFeature[FEATURE_P_NORAGDOLL]));
		g_pHack->seatbelt(g_pSettings->getFeature(g_iFeature[FEATURE_V_SEATBELT]));
		g_pHack->vehicleGravity(g_pSettings->getFeature(g_iFeature[FEATURE_V_GRAVITY]));
		g_pHack->vehicleBulletproofTires(g_pSettings->getFeature(g_iFeature[FEATURE_V_BULLETPROOFTIRES]));

		g_pHack->frameFlags(	g_pSettings->getFeature(g_iFeature[FEATURE_P_SUPERJUMP]),
								g_pSettings->getFeature(g_iFeature[FEATURE_P_EXPLOSIVEMELEE]),
								g_pSettings->getFeature(g_iFeature[FEATURE_W_FIREAMMO]),
								g_pSettings->getFeature(g_iFeature[FEATURE_W_EXPLOSIVEAMMO]));

		if(g_pHack->m_vehicle.loadHandling())
		{
			g_pHack->vehicleAccel(g_pSettings->getFeature(g_iFeature[FEATURE_V_ACCELERATION]));
			g_pHack->vehicleBrake(g_pSettings->getFeature(g_iFeature[FEATURE_V_BRAKEFORCE]));
			g_pHack->vehicleTraction(g_pSettings->getFeature(g_iFeature[FEATURE_V_TRACTION]));
			g_pHack->vehicleDeformation(g_pSettings->getFeature(g_iFeature[FEATURE_V_DEFORMATION]));
		}

		if(g_pHack->m_weapon.loadWeapon())
		{
			g_pHack->noSpread(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPREAD]));
			g_pHack->noRecoil(g_pSettings->getFeature(g_iFeature[FEATURE_W_RECOIL]));
			g_pHack->quickReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_RELOAD]));
			g_pHack->bulletDamage(g_pSettings->getFeature(g_iFeature[FEATURE_W_DAMAGE]));
			g_pHack->weaponRange(g_pSettings->getFeature(g_iFeature[FEATURE_W_RANGE]));
			g_pHack->weaponSpin(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPINUP]));
			g_pHack->bulletBatch(g_pSettings->getFeature(g_iFeature[FEATURE_W_BULLET_BATCH]));
			g_pHack->muzzleVelocity(g_pSettings->getFeature(g_iFeature[FEATURE_W_MUZZLE_VELOCITY]));

			g_pHack->infAmmo(g_pSettings->getFeature(g_iFeature[FEATURE_W_AMMO]));
		}

		Sleep(1);
	}
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

	//restore patched code
	feat dummyFeat;
	dummyFeat.m_bRestored = false;
	g_pHack->noReload(&dummyFeat);
	dummyFeat.m_bRestored = false;
	g_pHack->infAmmo(&dummyFeat);

	delete	g_pHack;
	delete	g_pD3D9Render;
	delete	g_pSettings;
	delete	g_pMemMan;

	exit(0);
}