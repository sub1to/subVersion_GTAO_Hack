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

#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_SAVEDPOS		0x06		//max saved positions (to teleport to)
#define MAX_PROPERTIES		0x60		//max iniparser properties
#define MAX_KEYS			0x10		//max indexed keys (keybinds)
#define MAX_MENU_TABS		0x08		//max amount of overlay menu tabs
#define MAX_MENU_FEATURES	0x20		//max amount of features
#define NAME_BUFFER_SIZE	0x20

//Required keys, loaded in settings::settings
#define keyExit			0x0
#define keyMenu			0x1
#define keyMenuUp		0x2
#define keyMenuDown		0x3
#define keyMenuLeft		0x4
#define keyMenuRight	0x5
#define keyMenuSelect	0x6
#define keyMenuTabNext	0x7
#define keyMenuTabPrev	0x8
#define keyMenuSave		0x9
#define keyHotTeleport	0xA
#define keyHotWanted	0xB
#define keyHotHealth	0xC

class featCat
{
	public:
		int		id;
		char	name[NAME_BUFFER_SIZE];
};

enum featType	{ feat_toggle, feat_slider, feat_teleport };
enum teleType	{ tp_saved, tp_waypoint };

class feat
{
	public:
		int			m_iId,
					m_iParent;
		featType	m_type;
		std::string	m_szName;
		std::string	m_szIniKey;
		bool		m_bOn	= false;

						feat();
		virtual			~feat();
		virtual	void	toggle();
		virtual void	inc();
		virtual void	dec();
};

class featSlider : public feat
{
	public:
		float		m_fValue,
					m_fMin,
					m_fMax;

				featSlider();
				~featSlider();
		void	inc();
		void	dec();
};

class featTeleport : public feat
{
	public:
		teleType	m_tpType;
		v3			m_v3Pos;

				featTeleport();
				~featTeleport();
		void	toggle();
};

class iniParser
{
	public:
		std::string	m_szFile;

					iniParser();
					iniParser(std::string szFile);
					~iniParser();

		void		read();
		void		write();

		int			findKey(std::string szKey);
		int			createKey(std::string szKey);


		template	<typename rT>
		rT			getValue(std::string szKey)
		{
			rT		r{};
			int		i	= this->findKey(szKey);
			if(i == -1)
				return r;
			std::stringstream	ssBuf(m_aszBuffer[i][1]);
			ssBuf	>> r;
			return r;
		}

		template	<typename wT>
		bool		setValue(std::string szKey, wT value)
		{
			int i	= findKey(szKey);
			if(i == -1)
				i	= createKey(szKey);
			if(i == -1)
				return false;
			m_aszBuffer[i][1] = std::to_string(value);
			return true;
		}
	protected:
		std::string	m_aszBuffer[MAX_PROPERTIES][2];
		int			m_iBuffer = 0;
};

class settings
{
	public:
		int			m_iKeys[MAX_KEYS];
		iniParser	m_iniParser;
			
					settings();
					~settings();

		void		toggleMenu();
		bool		isMenuActive();
		void		menuUp();		//prev feature
		void		menuDown();		//next feature
		void		menuLeft();		//float value down
		void		menuRight();	//float value up
		void		menuSelect();	//toggle feature/teleport or w/e
		void		menuTabLeft();	//prev tab
		void		menuTabRight();	//next tab

		int			addFeatureCategory(std::string name);
		int			getFeatureCategoryCount();
		featCat*	getFeatureCategory(int id);
		int			setActiveCat(int);
		int			getActiveCat();

		int			addFeature(int parent, std::string name, featType type);
		int			addFeature(int parent, std::string name, featType type, std::string iniKey);
		int			addFeature(int parent, std::string name, featType type, std::string iniKey, float min, float max);
		int			addFeature(int parent, std::string name, featType type, std::string iniKey, teleType tpType);
		int			addFeature(int parent, std::string name, featType type, teleType tpType);
		int			getFeatureCurCount();
		feat*		getFeatureCur(int i);
		int			getFeatureCount();
		feat*		getFeature(int id);
		int			getActiveFeature();

		bool		getFeatureCurLock();
		bool		lockFeatureCur();
		bool		unlockFeatureCur();

	protected:
		featCat*	m_featureParent[MAX_MENU_TABS];
		int			m_nFeatureParent = 0;
		int			m_iActiveCat = 0;			//index for featureParent [should be the same as id]
		feat*		m_feature[MAX_MENU_FEATURES];
		int			m_nFeature = 0;						//total amount of features
		int			m_iActiveFeature = 0;				//index for featureCur [DOES NOT HOLD ID!]
		feat*		m_featureCur[MAX_MENU_FEATURES];	//list of features from current category
		bool		m_bFeatureCurLock = false;
		int			m_nFeatureCur	= 0;			//amount of features in current tab
		bool		m_bMenuActive = false;
};

extern settings*		g_pSettings;

#endif