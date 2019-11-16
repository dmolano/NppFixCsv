/***************************************************************************/
/*  This program is free software: you can redistribute it and/or modify   */
/*	it under the terms of the GNU General Public License as published by   */
/*	the Free Software Foundation, either version 3 of the License, or      */
/*	(at your option) any later version.                                    */
/*                                                                         */
/*	This program is distributed in the hope that it will be useful,        */
/*	but WITHOUT ANY WARRANTY; without even the implied warranty of         */
/*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          */
/*	GNU General Public License for more details.                           */
/*                                                                         */
/*	You should have received a copy of the GNU General Public License      */
/*	along with this program.  If not, see <https://www.gnu.org/licenses/>. */
/*                                                                         */
/*                                                                         */
/*  Author: Dionisio Molano Robledo                                        */
/*  Date: 2019/10/01                                                       */
/*  Email: dmolano.smriti@gmail.com                                        */
/*                                                                         */
/***************************************************************************/
#pragma once

//
// IniData
// 
typedef struct IniData { //name of the structure
	errno_t errnoInited;
	TCHAR path[MAX_PATH];
	const TCHAR* fileName;
	TCHAR fullPath[MAX_PATH];
}IniDataType;// type of data to declare the structure

//
// IniDataPtr
//
typedef IniDataType* IniDataPtr;

//
// ini_setIniPath
//
errno_t ini_setIniPath(const IniDataPtr iniData, const TCHAR* setPath);