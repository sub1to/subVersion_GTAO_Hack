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

std::string vkToStr(int vk)
{
	switch(vk)
	{
		case 0x30:		return "0";
		case 0x31:		return "1";
		case 0x32:		return "2";
		case 0x33:		return "3";
		case 0x34:		return "4";
		case 0x35:		return "5";
		case 0x36:		return "6";
		case 0x37:		return "7";
		case 0x38:		return "8";
		case 0x39:		return "9";

		case 0x41:		return "a";
		case 0x42:		return "b";
		case 0x43:		return "c";
		case 0x44:		return "d";
		case 0x45:		return "e";
		case 0x46:		return "f";
		case 0x47:		return "g";
		case 0x48:		return "h";
		case 0x49:		return "i";
		case 0x4A:		return "j";
		case 0x4B:		return "k";
		case 0x4C:		return "l";
		case 0x4D:		return "m";
		case 0x4E:		return "n";
		case 0x4F:		return "o";
		case 0x50:		return "p";
		case 0x51:		return "q";
		case 0x52:		return "r";
		case 0x53:		return "s";
		case 0x54:		return "t";
		case 0x55:		return "u";
		case 0x56:		return "v";
		case 0x57:		return "w";
		case 0x58:		return "x";
		case 0x59:		return "y";
		case 0x5A:		return "z";
	}
	return "";
}