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
// settingsDataPtr
//
SettingsDataPtr settingsDataPtr;

//
// hbrushEditBox
//
HBRUSH hbrushEditBox = 0;

//
// lengthData
//
LRESULT lengthData = 0;

//
// editLengthsFocus
//
BOOL editLengthsFocus = FALSE;

//
// settinsDlgProc_ClearEditLengthsHelpText
//
void settinsDlgProc_ClearEditLengthsHelpText(HWND hWndDlg) {
	HWND editTextHwnd;

	editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	SendMessage(editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, (LPARAM)EDIT_TEXT_CLEAR_STRING);
}

//
// settinsDlgProc_SetEditLengthsHelpText
//
void settinsDlgProc_SetEditLengthsHelpText(HWND hWndDlg) {
	HWND editTextHwnd;

	editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	SendMessage(editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, (LPARAM)EDIT_TEXT_HELP_STRING);
}

//
// settinsDlgProc_EditLengthsCommandEnChange
//
void settinsDlgProc_EditLengthsCommandEnChange(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (editLengthsFocus == TRUE) {
		HWND editTextHwnd;
		HWND buttonOkHwnd;

		buttonOkHwnd = GetDlgItem(hWndDlg, IDOK);
		editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
		lengthData = SendMessage(editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
		if (lengthData == 0) {
			EnableWindow(buttonOkHwnd, FALSE);
			Beep(1000, 10);
		}
		else {
			EnableWindow(buttonOkHwnd, TRUE);
		}
	}
}

//
// settinsDlgProc_EditLengthsCommand
//
void settinsDlgProc_EditLengthsCommand(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (HIWORD(wParam)) {
	case EN_CHANGE:
		settinsDlgProc_EditLengthsCommandEnChange(hWndDlg, uMsg, wParam, lParam);
		break;
	case EN_SETFOCUS:
		editLengthsFocus = TRUE;
		if (lengthData == 0) {
			settinsDlgProc_ClearEditLengthsHelpText(hWndDlg);
		}
		break;
	case EN_KILLFOCUS:
		editLengthsFocus = FALSE;
		if (lengthData == 0) {
			settinsDlgProc_SetEditLengthsHelpText(hWndDlg);
		}
		break;
	}
}

//
// settinsDlgProc_CtlColorEditLengths
//
LONG settinsDlgProc_CtlColorEditLengths(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdcEdit = (HDC)wParam; //Get handles
	if ((editLengthsFocus == FALSE) && (lengthData == 0)) {
		SetTextColor(hdcEdit, COLOR_FOREGROUND_HELP); // Text color
	}
	else {
		SetTextColor(hdcEdit, COLOR_FOREGROUND_NORMAL); // Text color
	}
	SetBkMode(hdcEdit, TRANSPARENT); // EditBox Backround Mode (note: OPAQUE can be used)
	SetBkColor(hdcEdit, (LONG)hbrushEditBox); // Backround color for EditBox
	return (LONG)hbrushEditBox; // Paint it
}

//
// settinsDlgProc_InitEditAndButton
//
void settinsDlgProc_InitEditAndButton(HWND hWndDlg, SettingsDataPtr settingsDataPtr) {
	HWND buttonOkHwnd;
	buttonOkHwnd = GetDlgItem(hWndDlg, IDOK);
	if (settingsDataPtr->lengthsUnicodeDataString != NULL) {
		HWND editTextHwnd;

		editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
		SendMessage(editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(settingsDataPtr->lengthsUnicodeDataString));
		lengthData = SendMessage(editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
		SendDlgItemMessage(editTextHwnd, IDC_EDIT_LENGTHS, EM_SETSEL, 0, -1);
		EnableWindow(buttonOkHwnd, TRUE);
	}
	else {
		EnableWindow(buttonOkHwnd, FALSE);
	}
	if (lengthData == 0) {
		settinsDlgProc_SetEditLengthsHelpText(hWndDlg);
	}
	hbrushEditBox = CreateSolidBrush(RGB(255, 255, 255));
}

//
// settingsDlgProc_IsLengthsOk
//
int settingsDlgProc_IsLengthsOk(HWND hWndDlg, WPARAM wParam, LPARAM lParam) {
	int result = FALSE;
	HWND editTextHwnd;
	editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	lengthData = SendMessage(editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
	if (lengthData > 0) {
		settingsDataPtr->lengthsUnicodeDataLength = lengthData;
		wchar_t* lengthsUnicodeDataSettingsTemporal = new wchar_t[lengthData + 1];
		SendMessage(editTextHwnd, WM_GETTEXT, lengthData + 1, LPARAM(lengthsUnicodeDataSettingsTemporal));
		char* lengthsDataSettingsTemporal = wchar_t2char(lengthsUnicodeDataSettingsTemporal);
		IntegerSplitPtr integerSplitListTemporal = integerSplitter_Split(lengthsDataSettingsTemporal, &(settingsDataPtr->splitIntegerValueMax));
		delete[] lengthsDataSettingsTemporal;
		if (integerSplitListTemporal != NULL) {
			if (settingsDataPtr->lengthsUnicodeDataString != NULL) {
				delete[] settingsDataPtr->lengthsUnicodeDataString;
				settingsDataPtr->lengthsUnicodeDataString = NULL;
			}
			if (settingsDataPtr->integerSplitList != NULL) {
				integerSplitter_Init(&(settingsDataPtr->integerSplitList));
			}
			settingsDataPtr->lengthsUnicodeDataString = lengthsUnicodeDataSettingsTemporal;
			settingsDataPtr->integerSplitList = integerSplitListTemporal;
			result = TRUE;
		}
		else {
			delete[] lengthsUnicodeDataSettingsTemporal;
		}
	}
	return result;
}

//
// settingsDlgProc_DialogFunc
//
INT_PTR CALLBACK settingsDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_INITDIALOG:
		if (lParam == NULL) {
			EndDialog(hWndDlg, ERROR_INIT_DIALOG);
			return TRUE;
		}
		settingsDataPtr = (SettingsDataPtr)lParam;
		centerWndDlg(hWndDlg);
		settinsDlgProc_InitEditAndButton(hWndDlg, settingsDataPtr);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))	{
		case IDC_EDIT_LENGTHS:
			settinsDlgProc_EditLengthsCommand(hWndDlg, uMsg, wParam, lParam);
			break;
		case IDOK:
			if (settingsDlgProc_IsLengthsOk(hWndDlg, wParam, lParam) == TRUE) {
				EndDialog(hWndDlg, 1);
			}
			else {
				::MessageBox(hWndDlg, INVALID_LENGTHS_SEPARATOR_ERROR_STRING, NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
			}
			break;
		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
		return TRUE;
	case WM_CTLCOLOREDIT:
		if (IDC_EDIT_LENGTHS == GetDlgCtrlID((HWND)lParam)) {
			//only then pass back the background brush. else let default procedure do it. 
			return settinsDlgProc_CtlColorEditLengths(hWndDlg, uMsg, wParam, lParam);
		}
		break;
	}
	return FALSE;
}