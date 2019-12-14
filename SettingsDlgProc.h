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

// ERROR INIT DIALOG
#define NO_ERROR_INIT_DIALOG 0

#define ERROR_INIT_DIALOG NO_ERROR_INIT_DIALOG + 1

#define NPP_FIX_CSV_SETTINGS_MAJOR_VERSION_NUMBER 1
#define NPP_FIX_CSV_SETTINGS_MINOR_VERSION_NUMBER 0

#define LENGTHS_EDIT_TEXT_BALLOONTIP_TITLE "Lengths"
#define LENGTHS_EDIT_TEXT_BALLOONTIP_TEXT "Enter the length list. e.g. \"5;7;11\" or \"13,17,19\""

#define EDIT_TEXT_HELP_STRING "Enter the length list. e.g. \"5;7;11\" or \"13,17,19\""
#define NOTATION_VALID_LENGTHS_SEPARATOR_STRING "You must enter a list of natural numbers separated by a delimiter. You can use different delimiters appropriate to each need.\r\nFor example: \"1;2-3:5\" or \"7;9;11;13\" ..."
#define INVALID_LENGTHS_SEPARATOR_ERROR_STRING "Error. Invalid lengths separator.\r\nYou must enter a list of natural numbers separated by a delimiter. You can use different delimiters appropriate to each need.\r\nFor example: \"1;2-3:5\" or \"7;9;11;13\" ..."

#define EDIT_TEXT_CLEAR_STRING _T("")

#define COLOR_FOREGROUND_NORMAL RGB(0, 0, 0)
#define COLOR_FOREGROUND_HELP RGB(192, 192, 192)

#define ONE_BALLONTIP_STATUS 0
#define TWO_BALLONTIP_STATUS 1

#define IDT_MOUSETRAP 1
#define ELAPSE_MOUSETRAP 2000

//
// SettingsData
// 
typedef struct SettingsData { //name of the structure
	int nppFixCsvMajorVersionNumber;
	int nppFixCsvMinorVersionNumber;
	int nppFixCsvSettingsMajorVersionNumber;
	int nppFixCsvSettingsMinorVersionNumber;
	HINSTANCE g_DllhInst;
	int lengthsUnicodeDataLength;
	wchar_t* lengthsUnicodeDataString;
	int splitIntegerValueMax;
	IntegerSplitPtr integerSplitList;
	HWND editTextHwnd;
	HWND buttonOkHwnd;
}SettingsDataType;// type of data to declare the structure

//
// SettingsDataPtr
//
typedef SettingsDataType* SettingsDataPtr;


//
// settingsDlgProc_DialogFunc
//
INT_PTR CALLBACK settingsDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);