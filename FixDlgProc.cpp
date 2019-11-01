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
// fixThreadHandle
//
HANDLE fixThreadHandle = NULL;
//
// semaphoreCancel
//
int semaphoreCancel = FALSE;
//
// progressBarHandle
//
HWND progressBarHandle;
//
// buttonCancelHandle
//
HWND staticActionHandle;
//
// buttonCancelHandle
//
HWND buttonCancelHandle;
//
// progressBarRange
//
PBRANGE progressBarRange;
//
// curScintilla
//
HWND curScintilla;
//
// sciLineCount
//
LRESULT sciLineCount;

//
// fixDlgProc_FixLine
//
int fixDlgProc_FixLine(HWND curScintilla, LRESULT lineIndex) {
	IntegerSplitPtr integerSplitListIndex = integerSplitList;
	int result = TRUE;

	/*This returns the length of the line, including any line end characters.
	If line is negative or beyond the last line in the document, the result is 0.
	If you want the length of the line not including any end of line characters,
	use SCI_GETLINEENDPOSITION(line) - SCI_POSITIONFROMLINE(line).*/
	LRESULT lineLen = ::SendMessage(curScintilla, SCI_LINELENGTH, lineIndex, NOT_USED_LPARAM);
	// TODO Remove.
	wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
	while (integerSplitListIndex != INTEGER_SPLITTER_NULL) {
		// TODO Remove.
		swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT("DEBUG-Len(%d)=%d (%d,%c)"),
			lineIndex + 1, lineLen, integerSplitListIndex->integer, integerSplitListIndex->separator);
		// TODO Remove.
		SendMessage(staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
		Sleep(1000);
		integerSplitListIndex = integerSplitListIndex->next;
	}
	return result;
}

//
// fixDlgProc_FixAllLines
//
DWORD WINAPI fixDlgProc_FixAllLines(LPVOID lpParam) {
	LRESULT lineIndex;
	HWND curScintilla;
	HWND hWndDlg;

	hWndDlg = (HWND)lpParam;
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, NOT_USED_WPARAM, (LPARAM)&which);
	if (which == -1) {
		return TRUE;
	}
	curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
	// We begin to fix each line of the document.
	for (lineIndex = 0; (semaphoreCancel == FALSE) && (lineIndex < sciLineCount); lineIndex++) {
		SendMessage(progressBarHandle, PBM_STEPIT, NOT_USED_WPARAM, NOT_USED_LPARAM);
		// First we fix, and if an error occurs, then we verify that we do not have a cancellation.
		if ((fixDlgProc_FixLine(curScintilla, lineIndex) == FALSE) &&
			(semaphoreCancel == FALSE)) {
			// Progress bar in red.
			SendMessage(progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_ERROR, NOT_USED_LPARAM);
			// Requesting to display an error message in the dialog window with the line as information.
			SendMessage(hWndDlg, WM_USER_ERROR_FIXING, NOT_USED_WPARAM, (LPARAM) lineIndex + 1);
			break;
		}
	}
	if (semaphoreCancel == TRUE) {
		SendMessage(progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_PAUSED, NOT_USED_LPARAM);
	}
	SendMessage(hWndDlg, WM_CLOSE, NOT_USED_WPARAM, NOT_USED_LPARAM);
	return semaphoreCancel;
}

//
// fixDlgProc_DialogFunc_InitDialog
//
int fixDlgProc_DialogFunc_InitDialog(HWND hWndDlg) {
	// Dialog
	centerWndDlg(hWndDlg);
	buttonCancelHandle = GetDlgItem(hWndDlg, IDCANCEL);
	staticActionHandle = GetDlgItem(hWndDlg, IDC_STATIC_ACTION);
	// Thread
	semaphoreCancel = FALSE;
	if (fixThreadHandle != NULL) {
		CloseHandle(fixThreadHandle);
		fixThreadHandle = NULL;
	}
	fixThreadHandle = CreateThread(NULL, 0, fixDlgProc_FixAllLines, hWndDlg, 0, NULL);
	if (fixThreadHandle == NULL) {
		return TRUE;
	}
	// Get the current scintilla
	int which = -1;
	::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, NOT_USED_WPARAM, (LPARAM)&which);
	if (which == -1) {
		return TRUE;
	}
	curScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
	sciLineCount = ::SendMessage(curScintilla, SCI_GETLINECOUNT, NOT_USED_WPARAM, NOT_USED_LPARAM);
	// Progress Bar
	progressBarHandle = GetDlgItem(hWndDlg, IDC_PROGRESSBAR_FIXING);
	SendMessage(progressBarHandle, PBM_SETRANGE32, NOT_USED_WPARAM, (LPARAM)sciLineCount);
	SendMessage(progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_NORMAL, NOT_USED_LPARAM);
	SendMessage(progressBarHandle, PBM_SETSTEP, (WPARAM)1, NOT_USED_LPARAM);
	return FALSE;
}

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		if (fixDlgProc_DialogFunc_InitDialog(hWndDlg) == TRUE) {
			EndDialog(hWndDlg, 0);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			semaphoreCancel = TRUE;
			EnableWindow(buttonCancelHandle, FALSE);
			SendMessage(staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(TEXT(CANCELING_TEXT)));
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
		wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
		swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT(ERROR_FIXING_TEXT), lParam);
		::MessageBox(hWndDlg, buffer, NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
		return TRUE;
		// break;
	default:
		break;
	}
	return FALSE;
}


