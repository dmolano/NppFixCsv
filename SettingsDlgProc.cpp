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
// lengthsUnicodeDataStringSettings
//
extern wchar_t* lengthsUnicodeDataStringSettings;

//
// fixCsvDataPtr
//
FixCsvDataPtr fixCsvDataPtr;

//
// settinsDlgProc_InitEditAndButton
//
void settinsDlgProc_InitEditAndButton(HWND hWndDlg) {
	HWND editTextHwnd;
	editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	SetFocus(editTextHwnd);
	HWND buttonOkHwnd;
	buttonOkHwnd = GetDlgItem(hWndDlg, IDOK);
	if (lengthsUnicodeDataStringSettings != NULL) {
		SendMessage(editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(lengthsUnicodeDataStringSettings));
		SendDlgItemMessage(editTextHwnd, IDC_EDIT_LENGTHS, EM_SETSEL, 0, -1);
		EnableWindow(buttonOkHwnd, TRUE);
	}
	else {
		EnableWindow(buttonOkHwnd, FALSE);
	}
}

//
// settingsDlgProc_IsLengthsOk
//
int settingsDlgProc_IsLengthsOk(HWND hWndDlg, WPARAM wParam, LPARAM lParam) {
	int result = FALSE;
	HWND editTextHwnd;
	editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	LRESULT lengthData = SendMessage(editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
	if (lengthData > 0) {
		wchar_t* lengthsUnicodeDataSettingsTemporal = new wchar_t[lengthData + 1];
		SendMessage(editTextHwnd, WM_GETTEXT, lengthData + 1, LPARAM(lengthsUnicodeDataSettingsTemporal));
		char* lengthsDataSettingsTemporal = wchar_t2char(lengthsUnicodeDataSettingsTemporal);
		IntegerSplitPtr integerSplitListTemporal = integerSplitter_Split(lengthsDataSettingsTemporal, &(fixCsvDataPtr->splitIntegerValueMax));
		delete[] lengthsDataSettingsTemporal;
		if (integerSplitListTemporal != NULL) {
			if (lengthsUnicodeDataStringSettings != NULL) {
				delete[] lengthsUnicodeDataStringSettings;
				lengthsUnicodeDataStringSettings = NULL;
			}
			if (fixCsvDataPtr->integerSplitList != NULL) {
				integerSplitter_Init(&(fixCsvDataPtr->integerSplitList));
			}
			lengthsUnicodeDataStringSettings = lengthsUnicodeDataSettingsTemporal;
			fixCsvDataPtr->integerSplitList = integerSplitListTemporal;
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
	switch (uMsg)
	{
	case WM_INITDIALOG:
		centerWndDlg(hWndDlg);
		settinsDlgProc_InitEditAndButton(hWndDlg);
		if (lParam == NULL) {
			EndDialog(hWndDlg, ERROR_INIT_DIALOG);
			return TRUE;
		}
		fixCsvDataPtr = (FixCsvDataPtr)lParam;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_EDIT_LENGTHS:
			if (HIWORD(wParam) == EN_CHANGE)
			{
				HWND editTextHwnd;
				editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
				LRESULT lengthData = SendMessage(editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
				HWND buttonOkHwnd;
				buttonOkHwnd = GetDlgItem(hWndDlg, IDOK);
				if (lengthData == 0) {
					EnableWindow(buttonOkHwnd, FALSE);
					Beep(1000, 10);
				}
				else {
					EnableWindow(buttonOkHwnd, TRUE);
				}
			}
			break;
		case IDOK:
			if (settingsDlgProc_IsLengthsOk(hWndDlg, wParam, lParam) == TRUE) {
				EndDialog(hWndDlg, 1);
			}
			else {
				::MessageBox(hWndDlg, TEXT("Error. Invalid lengths separator.\r\nRight format: number separator number ...\r\nExamples: 10;20;30\r\n\t10:20:30"), NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
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
	}
	return FALSE;
}