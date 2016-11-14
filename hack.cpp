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

/*
	TRAINER
*/

trainer::trainer()
{
	m_keyTmr = clock();
}

trainer::~trainer(){}
void	trainer::checkKeys(){}

bool	trainer::checkKeyState(int key, int mod)
{
	if
	(
		clock() - m_keyTmr > 300	&&
		(
			//(GetAsyncKeyState(key) && mod == 0) ||
			//(GetAsyncKeyState(key) && (GetAsyncKeyState(mod) & 0x8001) == 0x8001)
			((GetAsyncKeyState(key) & 0x8001) == 0x8001 && mod == 0)// ||
			//((GetAsyncKeyState(key) & 0x8001) == 0x8001 && (GetAsyncKeyState(mod) & 0x8001) == 0x8001)
		)
	)
	{
		m_keyTmr = clock();
		return true;
	}
	return false;
}


/*
	HACK
*/


hack::hack(){}

hack::~hack(){}

void hack::checkKeys()
{
	if(checkKeyState(g_pSettings->m_iKeys[keyExit], 0))
	{
		killProgram();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenu], 0))
	{
		g_pSettings->toggleMenu();

		//aero workaround
		if(g_pSettings->isMenuActive())
			SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		else
			SetWindowPos(g_hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW);

		return;
	}

	//hotkeys
	if(checkKeyState(g_pSettings->m_iKeys[keyHotTeleport], 0))
	{
		g_pHack->teleportWaypoint();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyHotWanted], 0))
	{
		g_pHack->notWanted();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyHotHealth], 0))
	{
		g_pHack->restoreHealth();
		return;
	}

	//if menu is not active, no need to check other keys
	if(!g_pSettings->isMenuActive())
		return;

	//menu navigation
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuDown], 0))
	{
		g_pSettings->menuDown();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuUp], 0))
	{
		g_pSettings->menuUp();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuRight], 0))
	{
		g_pSettings->menuRight();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuLeft], 0))
	{
		g_pSettings->menuLeft();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuTabNext], 0))
	{
		g_pSettings->menuTabRight();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuTabPrev], 0))
	{
		g_pSettings->menuTabLeft();
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuSave], 0))
	{
		featTeleport* tp	= dynamic_cast<featTeleport*>(g_pSettings->getFeatureCur(g_pSettings->getActiveFeature()));
		if(tp == nullptr || tp->m_tpType != tp_saved)
			return;
		m_player.getPos();
		tp->m_v3Pos.x = m_player.m_pos.x;
		tp->m_v3Pos.y = m_player.m_pos.y;
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_x", m_player.m_pos.x);
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_y", m_player.m_pos.y);
		return;
	}
	if(checkKeyState(g_pSettings->m_iKeys[keyMenuSelect], 0))
	{
		g_pSettings->menuSelect();
		return;
	}
}

bool	hack::initPointers()
{
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_hModule + ADDRESS_WORLD, &m_dwpWorldBase);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWorldBase + OFFSET_PLAYER, &m_dwpPlayerBase);
	m_player.m_dwpPlayerBase		= m_dwpPlayerBase;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_ENTITY_POSBASE, &m_player.m_dwpPlayerPosBase);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_PLAYER_INFO, &m_player.m_dwpPlayerInfo);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_PLAYER_VEHICLE, &m_dwpVehicleBase);
	m_vehicle.m_dwpVehicleBase		= m_dwpVehicleBase;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpVehicleBase + OFFSET_ENTITY_POSBASE, &m_vehicle.m_dwpVehiclePosBase);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_ENTITY_ATTACKER, &m_dwpAttackerBase);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpPlayerBase + OFFSET_WEAPON_MANAGER, &m_dwpWeaponManager);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWeaponManager + OFFSET_WEAPON_CURRENT, &m_dwpWeaponCur);
	m_weapon.m_weapDataCur.m_dwpWeapon = m_dwpWeaponCur;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpWeaponCur + OFFSET_WEAPON_AMMOINFO, &m_dwpAmmoInfo);
	m_weapon.m_dwpAmmoInfo			= m_dwpAmmoInfo;

	return 1;
}

void	hack::getWaypoint()
{
	g_pMemMan->readMem<v2>((DWORD_PTR) m_hModule + ADDRESS_WAYPOINT, &m_v2Waypoint);
	return;
}

void	hack::teleport(v3 v)
{
	m_player.getInVehicle();
	if(m_player.m_dwInVehicle > 0)
	{
		m_vehicle.setPos(v);
		return;
	}
	m_player.setPos(v);
	return;
}

bool	hack::teleportWaypoint()
{
	getWaypoint();
	if(m_v2Waypoint.x == 64000.f && m_v2Waypoint.y == 64000.f)
		return false;

	v3 dest(m_v2Waypoint.x, m_v2Waypoint.y, -225.f);
	teleport(dest);
	return true;
}

void	hack::restoreHealth()
{
	m_player.getHealth();
	if(m_player.m_flArmor < 50.f || m_player.m_hp.cur < m_player.m_hp.max)
		m_player.setHealth(m_player.m_hp.max, 50.f);
	return;
}

void	hack::restoreVehicleHealth()
{
	if(m_dwpVehicleBase == 0)
		return;
	m_vehicle.getHealth();
	if(m_vehicle.m_hp.cur < m_vehicle.m_hp.max && m_vehicle.m_hp.cur > 0.f)
		m_vehicle.setHealth(m_vehicle.m_hp.max);
	return;
}

void	hack::notWanted()
{
	m_player.getWanted();
	if(m_player.m_dwWanted == 0)
		return;
	m_player.setWanted(0);
	return;
}

void	hack::killNpc()
{
	if(m_dwpAttackerBase == 0)
		return;
	for(int i = 0; i < 3; i++)
	{
		DWORD_PTR	npc;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) m_dwpAttackerBase + (i * OFFSET_ATTACKER_DISTANCE), &npc);
		if(npc == m_dwpPlayerBase || npc == 0)
			continue;
		float		health;
		g_pMemMan->readMem<float>((DWORD_PTR) npc + OFFSET_ENTITY_HEALTH, &health);
		float v	= 0.f;
		if(health > v)
			g_pMemMan->writeMem<float>((DWORD_PTR) npc + OFFSET_ENTITY_HEALTH, &v);
	}
	return;
}

void	hack::fillAmmo()
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0 || m_dwpAmmoInfo == 0 || !m_weapon.findAmmoBase())
		return;
	m_weapon.getCurAmmo();
	m_weapon.getMaxAmmo();
	if(m_weapon.m_dwCurAmmo != m_weapon.m_dwMaxAmmo)
		m_weapon.setCurAmmo(m_weapon.m_dwMaxAmmo);
	return;
}

void	hack::noSpread(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fSpread != m_weapon.m_weapDataRestore.m_fSpread)
			m_weapon.setSpread(m_weapon.m_weapDataRestore.m_fSpread);
		return;
	}
	if(m_weapon.m_weapDataCur.m_fSpread != 0)
		m_weapon.setSpread(0);
	return;
}

void	hack::noRecoil(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fRecoil != m_weapon.m_weapDataRestore.m_fRecoil)
			m_weapon.setRecoil(m_weapon.m_weapDataRestore.m_fRecoil);
		return;
	}
	if(m_weapon.m_weapDataCur.m_fRecoil != 0)
		m_weapon.setRecoil(0);
	return;
}

void	hack::quickReload(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fReload != m_weapon.m_weapDataRestore.m_fReload || m_weapon.m_weapDataCur.m_fReloadVeh != m_weapon.m_weapDataRestore.m_fReloadVeh)
		{
			m_weapon.setReloadSpeed(m_weapon.m_weapDataRestore.m_fReload);
			m_weapon.setReloadVehicle(m_weapon.m_weapDataRestore.m_fReloadVeh);
		}
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fReload * static_cast<featSlider*>(g_pSettings->getFeature(g_iFeature[FEATURE_W_RELOAD]))->m_fValue;
	if(m_weapon.m_weapDataCur.m_fReload != fValue)
		m_weapon.setReloadSpeed(fValue);
	if(m_weapon.m_weapDataCur.m_fReloadVeh != 0)
		m_weapon.setReloadVehicle(0);
	return;
}

void	hack::bulletDamage(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fDamage != m_weapon.m_weapDataRestore.m_fDamage)
			m_weapon.setBulletDamage(m_weapon.m_weapDataRestore.m_fDamage);
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fDamage * static_cast<featSlider*>(g_pSettings->getFeature(g_iFeature[FEATURE_W_DAMAGE]))->m_fValue;
	if(m_weapon.m_weapDataCur.m_fDamage != fValue)
		m_weapon.setBulletDamage(fValue);
	return;
}

void	hack::weaponRange(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fRange != m_weapon.m_weapDataRestore.m_fRange)
			m_weapon.setRange(m_weapon.m_weapDataRestore.m_fRange);
		return;
	}
	float fValue	= m_weapon.m_weapDataRestore.m_fRange * static_cast<featSlider*>(g_pSettings->getFeature(g_iFeature[FEATURE_W_RANGE]))->m_fValue;
	if(m_weapon.m_weapDataCur.m_fRange != fValue)
		m_weapon.setRange(fValue);
	return;
}

void	hack::weaponSpin(bool restore)
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return;
	if(restore)
	{
		if(m_weapon.m_weapDataCur.m_fSpinUp != m_weapon.m_weapDataRestore.m_fSpinUp || m_weapon.m_weapDataCur.m_fSpin != m_weapon.m_weapDataRestore.m_fSpin)
		{
			m_weapon.setSpinUp(m_weapon.m_weapDataRestore.m_fSpinUp);
			m_weapon.setSpin(m_weapon.m_weapDataRestore.m_fSpin);
		}
		return;
	}
	if(m_weapon.m_weapDataCur.m_fSpinUp != 0 || m_weapon.m_weapDataCur.m_fSpin != 0)
	{
		m_weapon.setSpin(0);
		m_weapon.setSpinUp(0);
	}
	return;
}

bool	hack::loadWeapon()
{
	if(m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0)
		return 0;
	m_weapon.getHash();
	m_weapon.getBulletDamage();
	m_weapon.getReloadSpeed();
	m_weapon.getReloadVehicle();
	m_weapon.getRecoil();
	m_weapon.getSpread();
	m_weapon.getRange();
	m_weapon.getSpinUp();
	m_weapon.getSpin();
	if(m_weapon.m_weapDataCur.m_dwHash != m_weapon.m_weapDataRestore.m_dwHash)
	{
		if(m_weapon.m_weapDataCur.m_dwpWeapon != 0)
			m_weapon.restoreWeapon();
		m_weapon.m_weapDataRestore	= m_weapon.m_weapDataCur;
	}
	return 1;
}

void	hack::runSpeed(bool restore)
{
	m_player.getRunSpeed();
	if(restore)
	{
		if(m_player.m_flRunSpd > 1.f)
			m_player.setRunSpeed(1.f);
		return;
	}
	float fValue	= static_cast<featSlider*>(g_pSettings->getFeature(g_iFeature[FEATURE_P_RUNSPD]))->m_fValue;
	if(m_player.m_flRunSpd != fValue)
		m_player.setRunSpeed(fValue);
	return;
}

void	hack::swimSpeed(bool restore)
{
	m_player.getSwimSpeed();
	if(restore)
	{
		if(m_player.m_flSwimSpd > 1.f)
			m_player.setSwimSpeed(1.f);
		return;
	}
	float fValue	= static_cast<featSlider*>(g_pSettings->getFeature(g_iFeature[FEATURE_P_SWIMSPD]))->m_fValue;
	if(m_player.m_flSwimSpd != fValue)
		m_player.setSwimSpeed(fValue);
	return;
}