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
	//SETTINGS CLASS
*/

settings::settings()
{
	m_iniParser.m_szFile = "settings.ini";
	m_iniParser.read();

	//keys
	m_iKeys[keyExit]			= m_iniParser.getValue<int>("keyExit");
	m_iKeys[keyMenu]			= m_iniParser.getValue<int>("keyMenu");
	m_iKeys[keyMenuUp]			= m_iniParser.getValue<int>("keyMenuUp");
	m_iKeys[keyMenuDown]		= m_iniParser.getValue<int>("keyMenuDown");
	m_iKeys[keyMenuLeft]		= m_iniParser.getValue<int>("keyMenuLeft");
	m_iKeys[keyMenuRight]		= m_iniParser.getValue<int>("keyMenuRight");
	m_iKeys[keyMenuSelect]		= m_iniParser.getValue<int>("keyMenuSelect");
	m_iKeys[keyMenuTabNext]		= m_iniParser.getValue<int>("keyMenuTabNext");
	m_iKeys[keyMenuTabPrev]		= m_iniParser.getValue<int>("keyMenuTabPrev");
	m_iKeys[keyMenuSave]		= m_iniParser.getValue<int>("keyMenuSave");
	m_iKeys[keyHotTeleport]		= m_iniParser.getValue<int>("keyHotTeleport");
	m_iKeys[keyHotWanted]		= m_iniParser.getValue<int>("keyHotWanted");
	m_iKeys[keyHotHealth]		= m_iniParser.getValue<int>("keyHotHealth");
	m_iKeys[keyHotAmmo]			= m_iniParser.getValue<int>("keyHotAmmo");
}
settings::~settings()
{
	for(int i = 0; i < MAX_MENU_FEATURES; i++)
	{
		if(m_feature[i])
			delete m_feature[i];
	}

	for(int i = 0; i < MAX_MENU_TABS; i++)
	{
		if(m_featureParent[i])
			delete m_featureParent[i];
	}
}

void settings::toggleMenu()
{
	m_bMenuActive = !m_bMenuActive;
	return;
}

bool settings::isMenuActive()
{
	return m_bMenuActive;
}

void settings::menuDown()
{
	if(m_iActiveFeature + 1 < m_nFeatureCur)
	{
		m_iActiveFeature++;
		if (m_iActiveFeature == m_iFeatureCurDisplayOffset + MAX_MENU_FEATURES_DISPLAYED - MENU_FEATURE_SCROLL_PADDING)
		{
			int displayOffset	= m_iActiveFeature - (MAX_MENU_FEATURES_DISPLAYED - 1 - MENU_FEATURE_SCROLL_PADDING);
			displayOffset	= (displayOffset > m_nFeatureCur - MAX_MENU_FEATURES_DISPLAYED) ? m_nFeatureCur - MAX_MENU_FEATURES_DISPLAYED : displayOffset;
			displayOffset	= (displayOffset < 0) ? 0 : displayOffset;
			m_iFeatureCurDisplayOffset = displayOffset;
		}
	}
	else
	{
		m_iActiveFeature = 0;
		m_iFeatureCurDisplayOffset = 0;
	}

	return;
}

void settings::menuUp()
{
	if(m_iActiveFeature - 1 >= 0)
	{
		m_iActiveFeature--;
		if (m_iActiveFeature == m_iFeatureCurDisplayOffset + MENU_FEATURE_SCROLL_PADDING - 1)
		{
			int displayOffset	= m_iActiveFeature - MENU_FEATURE_SCROLL_PADDING;
				displayOffset	= (displayOffset > m_nFeatureCur - MAX_MENU_FEATURES_DISPLAYED) ? m_nFeatureCur - MAX_MENU_FEATURES_DISPLAYED : displayOffset;
				displayOffset	= (displayOffset < 0) ? 0 : displayOffset;
			m_iFeatureCurDisplayOffset = displayOffset;
		}
	}
	else
	{
		m_iActiveFeature = m_nFeatureCur - 1;
		int displayOffset	= m_nFeatureCur - MAX_MENU_FEATURES_DISPLAYED;
			displayOffset	= (displayOffset < 0) ? 0 : displayOffset;
		m_iFeatureCurDisplayOffset = displayOffset;
	}

	return;
}

void settings::menuRight()
{
	return this->getFeatureCur(m_iActiveFeature)->inc();
}

void settings::menuLeft()
{
	return this->getFeatureCur(m_iActiveFeature)->dec();
}

void settings::menuTabRight()
{
	m_iFeatureCurDisplayOffset = 0;
	if(m_iActiveCat + 1 < m_nFeatureParent)
		this->setActiveCat(m_iActiveCat + 1);
	else
		this->setActiveCat(0);
	return;
}

void settings::menuTabLeft()
{
	m_iFeatureCurDisplayOffset = 0;
	if(m_iActiveCat - 1 >= 0)
		this->setActiveCat(m_iActiveCat - 1);
	else
		this->setActiveCat(m_nFeatureParent - 1);
	return;
}

void settings::menuSelect()
{
	feat* feat	= this->getFeatureCur(m_iActiveFeature);
	feat->toggle();
	return;
}



int	settings::addFeatureCategory(std::string name)
{
	if(m_nFeatureParent >= MAX_MENU_TABS)		//prevent buffer overflow
		return -1;
	m_featureParent[m_nFeatureParent]		= new featCat;
	m_featureParent[m_nFeatureParent]->id	= m_nFeatureParent;
	strcpy_s(m_featureParent[m_nFeatureParent]->name, NAME_BUFFER_SIZE, name.c_str());
	m_nFeatureParent++;
	return m_nFeatureParent - 1;
}

int		settings::addFeature(int parent, std::string name, featType type)
{
	if(m_nFeature >= MAX_MENU_FEATURES || m_featureParent[parent] == nullptr)		//prevent buffer overflow & invalid parent
		return -1;
	switch(type)
	{
		case feat_toggle:
			m_feature[m_nFeature]			= new feat;
		break;
		case feat_slider:
			m_feature[m_nFeature]			= new featSlider;
		break;
		case feat_teleport:
			m_feature[m_nFeature]			= new featTeleport;
		break;
	}
	m_feature[m_nFeature]->m_iId		= m_nFeature;
	m_feature[m_nFeature]->m_iParent	= parent;
	m_feature[m_nFeature]->m_type		= type;
	m_feature[m_nFeature]->m_szName		= name;
	m_nFeature++;
	return m_nFeature - 1;
}

int		settings::addFeature(int parent, std::string name, featType type, std::string iniKey)
{
	int id = this->addFeature(parent, name, type);
	if(id < 0)
		return id;
	m_feature[id]->m_szIniKey	= iniKey;
	m_feature[id]->m_bOn		= (bool) m_iniParser.getValue<bool>(iniKey + "_on");
	m_feature[id]->m_bRestored	= (m_feature[id]->m_bOn) ? false : true;
	return id;
}

int		settings::addFeature(int parent, std::string name, featType type, std::string iniKey, float min, float max)
{
	int id = this->addFeature(parent, name, type, iniKey);
	if(id < 0)
		return id;
	dynamic_cast<featSlider*>(m_feature[id])->m_fMin		= min;
	dynamic_cast<featSlider*>(m_feature[id])->m_fMax		= max;
	float v	= m_iniParser.getValue<float>(iniKey + "_value");
	if(v <= max && v >= min)
		dynamic_cast<featSlider*>(m_feature[id])->m_fValue	= v;
	else
	{
		dynamic_cast<featSlider*>(m_feature[id])->m_fValue	= min;
		m_iniParser.setValue<float>(iniKey + "_value", min);
	}
	return id;
}

int		settings::addFeature(int parent, std::string name, featType type, std::string iniKey, teleType tpType)
{
	if(tpType == tp_waypoint)
		return -1;
	int id = this->addFeature(parent, name, type, iniKey);
	if(id < 0)
		return id;
	dynamic_cast<featTeleport*>(m_feature[id])->m_tpType		= tpType;
	dynamic_cast<featTeleport*>(m_feature[id])->m_v3Pos.x		= m_iniParser.getValue<float>(iniKey + "_x");
	dynamic_cast<featTeleport*>(m_feature[id])->m_v3Pos.y		= m_iniParser.getValue<float>(iniKey + "_y");
	dynamic_cast<featTeleport*>(m_feature[id])->m_v3Pos.z		= -225.f;
	return id;
}

int		settings::addFeature(int parent, std::string name, featType type, teleType tpType)
{
	if(tpType == tp_saved)
		return -1;
	int id = this->addFeature(parent, name, type);
	if(id < 0)
		return id;
	dynamic_cast<featTeleport*>(m_feature[id])->m_tpType	= tpType;
	return id;
}

int		settings::getFeatureCategoryCount()
{
	return m_nFeatureParent;
}

featCat*	settings::getFeatureCategory(int id)
{
	if(id > m_nFeatureParent)
		return 0;
	return m_featureParent[id];
}

int			settings::getActiveCat()
{
	return m_iActiveCat;
}

int			settings::setActiveCat(int cat)
{
	if(cat > m_nFeatureParent)
		return 0;

	//prevent race conditions
	while(!this->lockFeatureCur())
	{
		Sleep(1);
	}

	m_iActiveCat		= cat;
	m_nFeatureCur		= 0;
	m_iActiveFeature	= 0;
	ZeroMemory(&m_featureCur, sizeof(feat*) * MAX_MENU_FEATURES);		//clear all the pointers

	//create a list of features from current category
	for(int i = 0; i < m_nFeature; i++)
	{
		if(m_feature[i]->m_iParent == m_iActiveCat)
		{
			m_featureCur[m_nFeatureCur]	= m_feature[i];
			m_nFeatureCur++;
		}
	}

	this->unlockFeatureCur();

	return 1;
}

int		settings::getFeatureCurCount()
{
	return m_nFeatureCur;
}

feat*	settings::getFeatureCur(int i)
{
	if(i > m_nFeatureCur)
		return 0;
	return m_featureCur[i];
}

int		settings::getFeatureCount()
{
	return m_nFeature;
}

feat*	settings::getFeature(int id)
{
	if(id > m_nFeature)
		return 0;
	return m_feature[id];
}

int		settings::getActiveFeature()
{
	return m_iActiveFeature;
}

bool	settings::getFeatureCurLock()
{
	return m_bFeatureCurLock;
}

bool	settings::lockFeatureCur()
{
	if(m_bFeatureCurLock)
		return false;
	m_bFeatureCurLock = true;
	return true;
}

bool	settings::unlockFeatureCur()
{
	m_bFeatureCurLock = false;
	return true;
}

int		settings::getDisplayOffset()
{
	return m_iFeatureCurDisplayOffset;
}


/*
	//FEATURE CLASSES
*/

feat::feat() {}

feat::~feat() {}

void	feat::toggle()
{
	m_bOn = !m_bOn;
	if(m_bOn)
		m_bRestored = false;
	if(m_szIniKey != "")
		g_pSettings->m_iniParser.setValue<bool>((std::string) m_szIniKey + "_on", (int) m_bOn);
	return;
}

void	feat::inc() {}
void	feat::dec() {}


featSlider::featSlider() {}
featSlider::~featSlider() {}

void	featSlider::inc()
{
	float v		= (m_fValue + ( (m_fMax - m_fMin) * 0.10f) );
	if(v <= m_fMax)
		m_fValue = v;
	else
		m_fValue = m_fMax;
	g_pSettings->m_iniParser.setValue<float>((std::string) m_szIniKey + "_value", m_fValue);
	return;
}

void	featSlider::dec()
{
	float v		= (m_fValue - ( (m_fMax - m_fMin) * 0.10f) );
	if(v >= m_fMin)
		m_fValue = v;
	else
		m_fValue = m_fMin;
	g_pSettings->m_iniParser.setValue<float>((std::string) m_szIniKey + "_value", m_fValue);
	return;
}


featTeleport::featTeleport() {}
featTeleport::~featTeleport() {}

void	featTeleport::toggle()
{
	switch(m_tpType)
	{
		case tp_waypoint:
			g_pHack->teleportWaypoint();
		break;
		case tp_saved:
			g_pHack->teleport(m_v3Pos);
		break;
	}
}



/*
	//INI PARSER CLASS
*/

iniParser::iniParser(){};

iniParser::iniParser(std::string szFile)	:
m_szFile(szFile)
{};

iniParser::~iniParser(){};

void iniParser::read()
{
	std::ifstream file;
	file.open(m_szFile, std::ios::in);
	if(!file.is_open())
		return;

	std::string szLine;
	int	i = 0;

	while(std::getline(file, szLine))
	{
		if(m_iBuffer >= MAX_PROPERTIES)		//prevent buffer overflow
			return;
		if(szLine[0] == ';' || szLine == "")
			continue;

		std::stringstream	ssBuf(szLine);
		std::string			szToken;

		std::getline(ssBuf, szToken, '=');		//read the key
		m_aszBuffer[i][0] = szToken;

		std::getline(ssBuf, szToken, '=');		//read the value
		m_aszBuffer[i][1] = szToken;

		i++;
		m_iBuffer = i;
	}
	return;
}

void iniParser::write()
{
	std::ofstream file;
	file.open(m_szFile, std::ios::out | std::ios::trunc);
	if(!file.is_open())
		return;
	for(auto &v : m_aszBuffer)
	{
		if(v[0] == "" && v[1] == "")
			continue;
		file << v[0] << "=" << v[1] << "\n";
	}
	return;
}

int iniParser::findKey(std::string szKey)
{
	int iR = 0;
	for(iR = 0; iR < m_iBuffer; iR++)
	{
		if(m_aszBuffer[iR][0] == szKey)
			break;
	}
	if(m_aszBuffer[iR][0] != szKey)
		iR = -1;
	return iR;
}

int iniParser::createKey(std::string szKey)
{
	if(m_iBuffer >= MAX_PROPERTIES)		//prevent buffer overflow
		return -1;
	int iR	= m_iBuffer;
	m_iBuffer++;
	m_aszBuffer[iR][0] = szKey;
	return iR;
}