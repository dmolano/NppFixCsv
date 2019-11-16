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
#include "pch.h"

//
// ini_setIniPath
//
errno_t ini_SetIniPath(const IniDataPtr iniData, const TCHAR* setPath) {
	if (iniData->errnoInited == NOERROR) {
		return iniData->errnoInited;
	}
	errno_t flag;
	// If there is no executable file
	if (setPath == NULL) {
		TCHAR* ptsLastDirectory;
		GetModuleFileName(NULL, iniData->path, _MAX_PATH);
		ptsLastDirectory = _tcsrchr(iniData->path, _T('\\'));
		*ptsLastDirectory = _T('\0');
	}
	else {
		flag = _tcscpy_s(iniData->path, MAX_PATH, setPath);
		if (flag != NOERROR) {
			iniData->errnoInited = flag;
			return flag;
		}
	}
	// Create full path of ini file
	flag = _tcscpy_s(iniData->fullPath, MAX_PATH, iniData->path);
	if (flag != NOERROR) {
		iniData->errnoInited = flag;
		return flag;
	}
	flag = _tcscat_s(iniData->fullPath, MAX_PATH, _T("\\"));
	if (flag != NOERROR) {
		iniData->errnoInited = flag;
		return flag;
	}
	flag = _tcscat_s(iniData->fullPath, MAX_PATH, iniData->fileName);
	if (flag != NOERROR) {
		iniData->errnoInited = flag;
		return flag; 
	}
	iniData->errnoInited = NOERROR;
	return iniData->errnoInited;
}

//
// ini_WriteDate
// The return value of write type is normal except 0, 0 is error
//NUMDIGIT = 64digit
//
int ini_WriteDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, float data) {
	TCHAR buf[INI_NUMDIGIT];
	_stprintf_s(buf, INI_NUMDIGIT, _T("%f"), data);
	return (int)WritePrivateProfileString(section, key, buf, iniData->fullPath);
}

//
// ini_WriteDate
//
int ini_WriteDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, int data) {
	TCHAR buf[INI_NUMDIGIT];
	_itot_s(data, buf, INI_NUMDIGIT, 10);
	return (int)WritePrivateProfileString(section, key, buf, iniData->fullPath);
}

//
// ini_WriteDate
//
int ini_WriteDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, const TCHAR* data) {
	return (int)WritePrivateProfileString(section, key, data, iniData->fullPath);
}

//
// ini_ReadDate
// Return value is the number of characters read http://msdn.microsoft.com/en-us/library/cc429779.aspx
//
int ini_ReadDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, TCHAR* data, int dataSize) {
	return (int)GetPrivateProfileString(section, key, _T(""), data, (DWORD)dataSize, iniData->fullPath);
}

//
// ini_ReadDate
//
int ini_ReadDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, int* data) {
	TCHAR buf[INI_NUMDIGIT];
	int readSize;
	readSize = (int)GetPrivateProfileString(section, key, _T(""), buf, (DWORD)INI_NUMDIGIT, iniData->fullPath);
	if ((readSize == (INI_NUMDIGIT - 1)) || (readSize == (INI_NUMDIGIT - 2))) {
		*data = 0;
		return 0;
	}
	*data = _ttoi(buf);

	return readSize;
}

//
// ini_ReadDate
//
int ini_ReadDate(const IniDataPtr iniData, const TCHAR* section, const TCHAR* key, float* data) {
	TCHAR buf[INI_NUMDIGIT];
	int readSize;
	readSize = (int)GetPrivateProfileString(section, key, _T(""), buf, (DWORD)INI_NUMDIGIT, iniData->fullPath);
	if ((readSize == (INI_NUMDIGIT - 1)) || (readSize == (INI_NUMDIGIT - 2))) {
		*data = 0;
		return 0;
	}
	*data = (float)_tstof(buf);

	return readSize;
}