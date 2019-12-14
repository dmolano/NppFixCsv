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
// balloonTipStatus
// 
int balloonTipStatus;

//
// editTextRect
//
RECT editTextRect;

//
// currentPoint
//
POINT currentPoint;

//
// lastPoint
//
POINT lastPoint;

//
// settinsDlgProc_ClearEditLengthsHelpText
//
void settinsDlgProc_ClearEditLengthsHelpText(HWND hWndDlg) {
	SendMessage(settingsDataPtr->editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, (LPARAM)EDIT_TEXT_CLEAR_STRING);
}

//
// settinsDlgProc_SetEditLengthsHelpText
//
void settinsDlgProc_SetEditLengthsHelpText(HWND hWndDlg) {
	SendMessage(settingsDataPtr->editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, (LPARAM)_T(EDIT_TEXT_HELP_STRING));
}

//
// settinsDlgProc_EditLengthsCommandEnChange
//
void settinsDlgProc_EditLengthsCommandEnChange(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (editLengthsFocus == TRUE) {
		lengthData = SendMessage(settingsDataPtr->editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
		if (lengthData == 0) {
			EnableWindow(settingsDataPtr->buttonOkHwnd, FALSE);
			Beep(1000, 10);
		}
		else {
			EnableWindow(settingsDataPtr->buttonOkHwnd, TRUE);
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
	settingsDataPtr->buttonOkHwnd = GetDlgItem(hWndDlg, IDOK);
	settingsDataPtr->editTextHwnd = GetDlgItem(hWndDlg, IDC_EDIT_LENGTHS);
	if (settingsDataPtr->lengthsUnicodeDataString != NULL) {
		SendMessage(settingsDataPtr->editTextHwnd, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(settingsDataPtr->lengthsUnicodeDataString));
		lengthData = SendMessage(settingsDataPtr->editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
		SendDlgItemMessage(settingsDataPtr->editTextHwnd, IDC_EDIT_LENGTHS, EM_SETSEL, 0, -1);
		EnableWindow(settingsDataPtr->buttonOkHwnd, TRUE);
	}
	else {
		EnableWindow(settingsDataPtr->buttonOkHwnd, FALSE);
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
	lengthData = SendMessage(settingsDataPtr->editTextHwnd, WM_GETTEXTLENGTH, NOT_USED_WPARAM, NOT_USED_LPARAM);
	if (lengthData > 0) {
		settingsDataPtr->lengthsUnicodeDataLength = lengthData;
		wchar_t* lengthsUnicodeDataSettingsTemporal = new wchar_t[lengthData + 1];
		SendMessage(settingsDataPtr->editTextHwnd, WM_GETTEXT, lengthData + 1, LPARAM(lengthsUnicodeDataSettingsTemporal));
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
//	wchar_t buffer[200];
	switch (uMsg) {
	case WM_INITDIALOG:
		if (lParam == NULL) {
			EndDialog(hWndDlg, ERROR_INIT_DIALOG);
			return TRUE;
		}
		settingsDataPtr = (SettingsDataPtr)lParam;
		centerWndDlg(hWndDlg);
		settinsDlgProc_InitEditAndButton(hWndDlg, settingsDataPtr);
		balloonTipStatus = ONE_BALLONTIP_STATUS;
		GetCursorPos(&lastPoint);
		SetTimer(hWndDlg,             // handle to main window
			IDT_MOUSETRAP,            // timer identifier
			ELAPSE_MOUSETRAP,         // 2-second interval
			(TIMERPROC)NULL);         // no timer callback
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_EDIT_LENGTHS:
			settinsDlgProc_EditLengthsCommand(hWndDlg, uMsg, wParam, lParam);
			break;
		case IDOK:
			if (settingsDlgProc_IsLengthsOk(hWndDlg, wParam, lParam) == TRUE) {
				EndDialog(hWndDlg, 1);
			}
			else {
				::MessageBox(hWndDlg, _T(INVALID_LENGTHS_SEPARATOR_ERROR_STRING), NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
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
	case WM_DESTROY:
		KillTimer(hWndDlg, IDT_MOUSETRAP);
		break;
	case WM_TIMER:
		GetWindowRect(settingsDataPtr->editTextHwnd, &editTextRect);
		GetCursorPos(&currentPoint);
//		swprintf_s(buffer, 200, TEXT("[%d,%d] - [%d,%d,%d,%d] %d - WM_TIMER"),
//			currentPoint.x, currentPoint.y, editTextRect.left, editTextRect.top, editTextRect.right, editTextRect.bottom,
//			PtInRect(&editTextRect, currentPoint));
//		SendMessage(GetDlgItem(hWndDlg, IDC_STATIC_INFO), WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
		switch (balloonTipStatus) {
		case ONE_BALLONTIP_STATUS:
			if (PtInRect(&editTextRect, currentPoint) == TRUE) {
				if ((lastPoint.x == currentPoint.x) && (lastPoint.y == currentPoint.y)) {
					EDITBALLOONTIP editBalloonTip;
					editBalloonTip.cbStruct = sizeof(editBalloonTip);
					editBalloonTip.pszTitle = _T(LENGTHS_EDIT_TEXT_BALLOONTIP_TITLE);
					editBalloonTip.pszText = _T(LENGTHS_EDIT_TEXT_BALLOONTIP_TEXT);
					editBalloonTip.ttiIcon = TTI_INFO;
					balloonTipStatus = TWO_BALLONTIP_STATUS;
					lastPoint.x = 0;
					lastPoint.y = 0;
					Edit_ShowBalloonTip(settingsDataPtr->editTextHwnd, &editBalloonTip);
				}
				else {
					lastPoint = currentPoint;
				}
			}
			else {
				lastPoint = currentPoint;
			}
			break;
		case TWO_BALLONTIP_STATUS:
			if (PtInRect(&editTextRect, currentPoint) == FALSE) {
				balloonTipStatus = ONE_BALLONTIP_STATUS;
			}
			break;
		}
		break;
	}
	return FALSE;
}