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

#ifndef STDAFX_H
#define STDAFX_H

#include <windows.h>
#include <windowsx.h>
//#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <psapi.h>

#include "d3d9/d3d9.h"
#include "d3d9/d3dx9.h"
#pragma comment(lib,"d3d9/d3dx9.lib")
#pragma comment(lib,"d3d9/d3d9.lib")

#include "resource.h"

#include "vector.h"
#include "memManager.h"
#include "D3D9Render.h"
#include "settings.h"
#include "entity.h"
#include "hack.h"


//Addresses from GTAV.exe module
#define ADDRESS_WORLD					0x2303788		//42
#define ADDRESS_WAYPOINT				0x1F59BB0		//wtb a (mini)map struct

//offset from WORLD
#define OFFSET_PLAYER					0x08			//playerbase

//entity offsets
#define OFFSET_ENTITY_POSBASE			0x30			//base of a structure that contains entity coords
#define OFFSET_ENTITY_POSBASE_POS		0x50			//vector3
#define OFFSET_ENTITY_POS				0x90			//vector3
#define OFFSET_ENTITY_HEALTH			0x280			//entity health (except for vehicles); float cur, float max
#define OFFSET_ENTITY_HEALTH_MAX		0x2A0			//they moved this away from curHealth in 1.36 :(
#define OFFSET_ENTITY_ATTACKER			0x2A8			//base to a list of the last 3 entities that attacked the current entity
#define OFFSET_ENTITY_INVEHICLE			0x1458

//player (entity) offsets
#define OFFSET_PLAYER_ARMOR				0x14A0			//armour
#define OFFSET_PLAYER_INFO				0x10A8			//playerInfo struct
#define OFFSET_PLAYER_INFO_WANTED		0x798			//wanted level; DWORD
#define OFFSET_PLAYER_VEHICLE			0x14E0			//ptr to last used vehicle
#define OFFSET_PLAYER_NAME				0x7C

//vehicle offsets
#define OFFSET_VEHICLE_HEALTH			0x82C			//vehicle health; 0.f-1000.f

//weapon offsets
#define OFFSET_WEAPON_MANAGER			0x10B8			//from playerbase
#define OFFSET_WEAPON_CURRENT			0x20			//from weapon manager
#define OFFSET_WEAPON_AMMOINFO			0x48			//from weaponbase
#define OFFSET_WEAPON_AMMOINFO_MAX		0x28			//ammoinfo
#define OFFSET_WEAPON_AMMOINFO_CUR_1	0x08			//ptr lvl 1, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CUR_2	0x00			//ptr tr lvl 2, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CURAMMO	0x18			//offset to cur ammo
#define OFFSET_WEAPON_AMMOINFO_TYPE		0x0C			//offset to projectile type?
#define OFFSET_WEAPON_SPREAD			0x5C			//float set to 0
#define OFFSET_WEAPON_BULLET_DMG		0x98			//float times 10 (so when 0, it will stay 0)
#define OFFSET_WEAPON_RELOAD_MULTIPLIER	0x114			//float times 10
#define OFFSET_WEAPON_RECOIL			0x2A4			//float set to 0
#define OFFSET_WEAPON_MODEL_HASH		0x14
#define OFFSET_WEAPON_NAME_HASH			0x10
#define OFFSET_WEAPON_RELOAD_VEHICLE	0x110
#define OFFSET_WEAPON_RANGE				0x25C
#define OFFSET_WEAPON_SPINUP			0x124
#define OFFSET_WEAPON_SPIN				0x128

#define OFFSET_ATTACKER_DISTANCE		0x18			//changed to 0x18, from 0x10

void		killProgram	();
std::string	vkToStr	(int vk);

#endif
