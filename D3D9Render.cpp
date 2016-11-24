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

D3D9Render::D3D9Render() : 
	m_nFont(0)
{
	
}
D3D9Render::~D3D9Render()
{
	this->releaseFont();
	m_pVertexBuffer->Release();	//close and release vertex buffer
	m_pD3dDev->Release();    // close and release the 3D device
    m_pD3d->Release();		// close and release Direct3D
}

bool	D3D9Render::init(HWND hWnd)
{
	m_pD3d	= Direct3DCreate9(D3D_SDK_VERSION);		//create d3d9 interface

	ZeroMemory(&m_d3dParam, sizeof(m_d3dParam));		//clear the struct

	m_d3dParam.Windowed				= true;
	m_d3dParam.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	m_d3dParam.hDeviceWindow		= hWnd;
	m_d3dParam.BackBufferHeight		= m_screen.h;
	m_d3dParam.BackBufferWidth		= m_screen.w;
	m_d3dParam.MultiSampleQuality	= D3DMULTISAMPLE_NONE;
	m_d3dParam.BackBufferFormat		= D3DFMT_A8R8G8B8;

	m_pD3d->CreateDevice(	D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL,
							hWnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING,
							&m_d3dParam,
							&m_pD3dDev);

	//create vertex buffer
	m_pD3dDev->CreateVertexBuffer(	4 * sizeof(Vertex),
									0,
									VERTEX_FORMAT,
									D3DPOOL_DEFAULT, //managed = video ram; D3DPOOL_DEFAULT = auto
									&m_pVertexBuffer,		//vertex buffer interface
									NULL);

	this->createFont("Verdana", 14, true, false);
	this->createFont("Verdana", 14, false, false);
	this->createFont("Verdana", 18, true, false);

	m_pD3dDev->SetFVF(VERTEX_FORMAT);// select which vertex format to use

	return true;
}

bool	D3D9Render::render()
{
	m_pD3dDev->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0,0,0,0), 1.f, 0);
	m_pD3dDev->BeginScene();

	if(g_pSettings->isMenuActive() && g_pMemMan->getWindow() == GetForegroundWindow())
	{
		//Draw header
		this->drawBoxBorder(0, 0, LAYOUT_ELEMENT_WIDTH, LAYOUT_ELEMENT_HEIGHT, LAYOUT_BORDER_SIZE, LAYOUT_COLOR_BACKGROUND, LAYOUT_COLOR_BORDER);
		this->drawText("subVersion menu [unknowncheats]", 5.f, 3.f, 2, LAYOUT_COLOR_TEXT);

		//prevent race conditions
		while(!g_pSettings->lockFeatureCur())
			Sleep(1);

		//draw tabs
		for(int i = 0; i < g_pSettings->getFeatureCategoryCount(); i++)
		{
			featCat*	tab	= g_pSettings->getFeatureCategory(i);
			float		x	= (float) ((LAYOUT_ELEMENT_WIDTH / g_pSettings->getFeatureCategoryCount()) * i),
						y	= (float) LAYOUT_ELEMENT_HEIGHT * 1,
						w	= (float) (LAYOUT_ELEMENT_WIDTH / g_pSettings->getFeatureCategoryCount()),
						h	= (float) LAYOUT_ELEMENT_HEIGHT;
			this->drawBoxBorder(x, y, w, h, LAYOUT_BORDER_SIZE, (i == g_pSettings->getActiveCat()) ? LAYOUT_COLOR_ACTIVE_BG : LAYOUT_COLOR_BACKGROUND, (i == g_pSettings->getActiveCat()) ? LAYOUT_COLOR_ACTIVE_BORDER : LAYOUT_COLOR_BORDER);
			this->drawText(tab->name, x + 5, y + 5, 0, LAYOUT_COLOR_TEXT);
		}
		//draw features
		int n	= g_pSettings->getFeatureCurCount();
		this->drawBoxBorder(0.f, (float) LAYOUT_ELEMENT_HEIGHT * 2, (float) LAYOUT_ELEMENT_WIDTH, (float) LAYOUT_ELEMENT_HEIGHT * n, (float) LAYOUT_BORDER_SIZE, LAYOUT_COLOR_BACKGROUND, LAYOUT_COLOR_BORDER);
		for(int i = 0; i < n; i++)			//this loops through only the features in the current category
		{
			feat*		feature	= g_pSettings->getFeatureCur(i);
			float		x	= 5.f,
						y	= 5.f + (LAYOUT_ELEMENT_HEIGHT * (i + 2));

			//selected box
			if(i == g_pSettings->getActiveFeature())
				this->drawBoxBorder(x-3, y-3, LAYOUT_ELEMENT_WIDTH - (LAYOUT_BORDER_SIZE * 2), LAYOUT_ELEMENT_HEIGHT - (LAYOUT_BORDER_SIZE * 2), LAYOUT_BORDER_SIZE,LAYOUT_COLOR_ACTIVE_BG, LAYOUT_COLOR_SELECTED);
			//checkbox
			if(feature->m_type == feat_toggle || feature->m_type == feat_slider)
				this->drawBoxBorder(x, y, LAYOUT_ELEMENT_HEIGHT - (LAYOUT_BORDER_SIZE * 5), LAYOUT_ELEMENT_HEIGHT - (LAYOUT_BORDER_SIZE * 5), LAYOUT_BORDER_SIZE, (feature->m_bOn == true) ? LAYOUT_COLOR_SELECTED : LAYOUT_COLOR_BACKGROUND, (feature->m_bOn == true) ? LAYOUT_COLOR_ACTIVE_BORDER : LAYOUT_COLOR_BORDER);
			
			this->drawText(feature->m_szName, x + (LAYOUT_ELEMENT_HEIGHT - (LAYOUT_BORDER_SIZE * 2)), y, 1, LAYOUT_COLOR_TEXT);
			
			if(feature->m_type == feat_slider)
			{
				featSlider* slider = dynamic_cast<featSlider*>(feature);
				float	x	= LAYOUT_ELEMENT_WIDTH * .5f,
						y	= (LAYOUT_ELEMENT_HEIGHT * (i + 2)) + 5.f,
						w	= (LAYOUT_ELEMENT_WIDTH * .5f) - (LAYOUT_BORDER_SIZE * 2),
						h	= LAYOUT_ELEMENT_HEIGHT - (LAYOUT_BORDER_SIZE * 5);
				float	mod	= (slider->m_fValue - slider->m_fMin)/(slider->m_fMax - slider->m_fMin);
				this->drawBoxBorder(x, y, w, h, LAYOUT_BORDER_SIZE, LAYOUT_COLOR_SLIDER_BG, LAYOUT_COLOR_BORDER);
				this->drawBoxBorder(x + (mod * (w - h)), y, h, h, LAYOUT_BORDER_SIZE, LAYOUT_COLOR_SLIDER_BTN, LAYOUT_COLOR_BORDER);
			}
		}

		g_pSettings->unlockFeatureCur();
	}

	m_pD3dDev->EndScene();
	m_pD3dDev->Present(nullptr, nullptr, nullptr, nullptr);
	return true;
}

bool	D3D9Render::createFont(char *name, int size, bool bold, bool italic)
{
	if(m_nFont >= FONT_BUFFER_SIZE - 1)
		return false;
	D3DXCreateFont(m_pD3dDev, size, 0, (bold) ? FW_BOLD : FW_NORMAL, 0, (italic) ? 1 : 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, name, &m_pFont[m_nFont]);
	m_nFont++;
	return true;
}

void	D3D9Render::releaseFont()
{
	for(int i = 0; i < m_nFont; i++)
		if(m_pFont[i] != nullptr)
			m_pFont[i]->Release();
	m_nFont = 0;
	return;
}

bool	D3D9Render::getViewport()
{
	RECT rectWnd;
	GetWindowRect(g_pMemMan->getWindow(), &rectWnd);
	m_screen.w = LAYOUT_ELEMENT_WIDTH;//rectWnd.right - rectWnd.left;
	m_screen.h = 300;//rectWnd.bottom - rectWnd.top;
	m_screen.x = rectWnd.left	+ LAYOUT_PADDING_LEFT;
	m_screen.y = rectWnd.top	+ LAYOUT_PADDING_TOP;
	return 1;
}

void	D3D9Render::drawBox(float x, float y, float w, float h, D3DCOLOR color)
{
	Vertex vertex[] =
	{
		{x,		y + h,	1.0f, 1.0f, color},
		{x,		y,		1.0f, 1.0f, color},
		{x + w,	y + h,	1.0f, 1.0f, color},
		{x + w,	y,		1.0f, 1.0f, color}
	};
	void*	pvBuffer;		//pointer to the buffer
	m_pVertexBuffer->Lock(0, 0, (void**) &pvBuffer, 0);
	memcpy(pvBuffer, vertex, sizeof(vertex));
	m_pVertexBuffer->Unlock();

	// select the vertex buffer to display
	m_pD3dDev->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(Vertex));

	// copy the vertex buffer to the back buffer
	m_pD3dDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	return;
}

void	D3D9Render::drawBoxInline(float x, float y, float w, float h, float size, D3DCOLOR color)
{
	this->drawBox(x,			y,				w,		size,	color);	//top
	this->drawBox(x + w - size,	y,				size,	h,		color);	//right
	this->drawBox(x,			y + h - size,	w,		size,	color);	//bottom
	this->drawBox(x,			y,				size,	h,		color);	//left
	return;
}

void	D3D9Render::drawBoxBorder(float x, float y, float w, float h, float borderSize, D3DCOLOR color, D3DCOLOR borderColor)
{
	this->drawBox(x, y, w, h, borderColor);
	this->drawBox(x + borderSize, y + borderSize, w - (borderSize * 2), h - (borderSize * 2), color);
}

void	D3D9Render::drawText(std::string str, float x, float y, int font, D3DCOLOR color)
{
	LPCSTR	pszStr	= str.c_str();
	RECT	pos;
	pos.left	= (int) x;
	pos.top		= (int) y;
	m_pFont[font]->DrawTextA(nullptr, pszStr, (int) strlen(pszStr), &pos, DT_NOCLIP, color);
}