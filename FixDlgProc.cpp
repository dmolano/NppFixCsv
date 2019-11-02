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
// integerSplitList
//
extern IntegerSplitPtr integerSplitList;
//
// The data of Notepad++ that you can use in your plugin commands
//
extern NppData nppData;

//
// fixDlgProc_FixLine_searchSeparator
//
int fixDlgProc_FixLine_searchSeparator(int status, FixCsvDataPtr fixCsvDataPtr, IntegerSplitPtr integerSplitListIndex) {
	// TODO Remove.
	wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
	Sleep(1000);
	return REACHED_SEPARATOR_STATUS;
}
//
// fixDlgProc_FixLine
//
int fixDlgProc_FixLine(FixCsvDataPtr fixCsvDataPtr) {
	IntegerSplitPtr integerSplitListIndex = fixCsvDataPtr->integerSplitList;
	int result = NO_ERROR_FIXING;

	/*This returns the length of the line, including any line end characters.
	If line is negative or beyond the last line in the document, the result is 0.
	If you want the length of the line not including any end of line characters,
	use SCI_GETLINEENDPOSITION(line) - SCI_POSITIONFROMLINE(line).*/
	LRESULT lineLen = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_LINELENGTH, fixCsvDataPtr->sciLineIndex, NOT_USED_LPARAM);
	// TODO Remove.
	//wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
	int status = SEARCHING_SEPARATOR_STATUS;
	while (integerSplitListIndex != INTEGER_SPLITTER_NULL) {
		// TODO Remove.
		//swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT("DEBUG-Len(%d)=%d (%d,%c)"),
		//	lineIndex + 1, lineLen, integerSplitListIndex->integer, integerSplitListIndex->separator);
		// TODO Remove.
		//SendMessage(staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
		status = SEARCHING_SEPARATOR_STATUS;
		status = fixDlgProc_FixLine_searchSeparator(status, fixCsvDataPtr, integerSplitListIndex);
		integerSplitListIndex = integerSplitListIndex->next;
	}
	result = 1;
	return result;
}

//
// fixDlgProc_FixAllLines
//
DWORD WINAPI fixDlgProc_FixAllLines(LPVOID lpParam) {
	FixCsvDataPtr fixCsvDataPtr;

	fixCsvDataPtr = (FixCsvDataPtr)lpParam;
	// We begin to fix each line of the document.
	for (fixCsvDataPtr->sciLineIndex = 0,
		fixCsvDataPtr->sciPositionIndex;
		(IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle) == TRUE) && (fixCsvDataPtr->sciLineIndex < fixCsvDataPtr->sciLineCount);
		fixCsvDataPtr->sciLineIndex++) {
		// Next progress bar step.
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_STEPIT, NOT_USED_WPARAM, NOT_USED_LPARAM);
		// First we fix, and if an error occurs, then we verify that we do not have a cancellation.
		if ((fixDlgProc_FixLine(fixCsvDataPtr) != NO_ERROR_FIXING) &&
			// We check that after fixing a line, they have not pressed the cancel button: button cancel is enabled.
			(IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle) == TRUE)) {
			// Progress bar in red.
			SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_ERROR, NOT_USED_LPARAM);
			// Requesting to display an error message in the dialog window with the line as information.
			SendMessage(fixCsvDataPtr->hWndDlg, WM_USER_ERROR_FIXING, NOT_USED_WPARAM, (LPARAM) fixCsvDataPtr);
			break;
		}
	}
	// Did anyone press the cancel button of the fixing operation?
	if (IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle) == FALSE) {
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_PAUSED, NOT_USED_LPARAM);
	}
	// Whatever happens, we give the order to close the dialog window.
	SendMessage(fixCsvDataPtr->hWndDlg, WM_CLOSE, NOT_USED_WPARAM, NOT_USED_LPARAM);
	return IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle);
}

//
// fixDlgProc_DialogFunc_InitProgressBar
//
HWND fixDlgProc_DialogFunc_InitProgressBar(FixCsvDataPtr fixCsvDataPtr) {
	HWND progressBarHandle = GetDlgItem(fixCsvDataPtr->hWndDlg, IDC_PROGRESSBAR_FIXING);
	SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETRANGE32, NOT_USED_WPARAM, (LPARAM)fixCsvDataPtr->sciLineCount);
	SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_NORMAL, NOT_USED_LPARAM);
	SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTEP, (WPARAM)1, NOT_USED_LPARAM);
	return progressBarHandle;
}
//
// fixDlgProc_DialogFunc_InitDialog
//
int fixDlgProc_DialogFunc_InitDialog(FixCsvDataPtr fixCsvDataPtr) {
	// Dialog
	centerWndDlg(fixCsvDataPtr->hWndDlg);
	fixCsvDataPtr->buttonCancelHandle = GetDlgItem(fixCsvDataPtr->hWndDlg, IDCANCEL);
	fixCsvDataPtr->staticActionHandle = GetDlgItem(fixCsvDataPtr->hWndDlg, IDC_STATIC_ACTION);
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, NOT_USED_WPARAM, (LPARAM)&which);
	if (which == -1) {
		return TRUE;
	}
	fixCsvDataPtr->currentScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
	fixCsvDataPtr->sciLineCount = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETLINECOUNT, NOT_USED_WPARAM, NOT_USED_LPARAM);
	// Progress Bar
	fixCsvDataPtr->progressBarHandle = fixDlgProc_DialogFunc_InitProgressBar(fixCsvDataPtr);
	// Thread
	fixCsvDataPtr->fixThreadHandle = CreateThread(NULL, 0, fixDlgProc_FixAllLines, fixCsvDataPtr, 0, NULL);
	if (fixCsvDataPtr->fixThreadHandle == NULL) {
		return TRUE;
	}
	return NO_ERROR_INIT;
}

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	FixCsvDataPtr fixCsvDataPtr = NULL;
	switch (uMsg) {
		case WM_INITDIALOG:
			if (lParam == NULL) {
				EndDialog(hWndDlg, 0);
				return TRUE;
			}
			fixCsvDataPtr = (FixCsvDataPtr)lParam;
			if (fixCsvDataPtr->integerSplitList == NULL) {
				EndDialog(hWndDlg, 0);
				return TRUE;
			}
			fixCsvDataPtr->hWndDlg = hWndDlg;
			if (fixDlgProc_DialogFunc_InitDialog(fixCsvDataPtr) != NO_ERROR_INIT) {
				EndDialog(hWndDlg, 0);
				return TRUE;
			}
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
			case IDCANCEL:
				EnableWindow(GetDlgItem(hWndDlg, IDCANCEL), FALSE);
				SendMessage(GetDlgItem(hWndDlg, IDC_STATIC_ACTION), WM_SETTEXT, NOT_USED_WPARAM, LPARAM(TEXT(CANCELING_TEXT)));
				return TRUE;
				// break;
			default:
				break;
			}
			break;
		case WM_CLOSE:
			EndDialog(hWndDlg, 0);
			return TRUE;
			// break;
		case WM_USER_ERROR_FIXING:
			if (lParam == NULL) {
				EndDialog(hWndDlg, 0);
				return TRUE;
			}
			fixCsvDataPtr = (FixCsvDataPtr)lParam;
			wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
			swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT(ERROR_FIXING_TEXT), fixCsvDataPtr->sciLineIndex + 1);
			::MessageBox(hWndDlg, buffer, NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
			return TRUE;
			// break;
		default:
			break;
		}
	return FALSE;
}


