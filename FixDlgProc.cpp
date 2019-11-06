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
int fixDlgProc_FixLine_searchSeparator(int status, FixCsvDataPtr fixCsvDataPtr, IntegerSplitPtr integerSplitListIndex, intptr_t *positionCharacterIndex) {
	intptr_t sciCharAtIndex = 0;
	int character = NULL;
	// Just in case.
	if (status != SEARCHING_SEPARATOR_STATUS) {
		return status;
	}
	// while loop starts.
	do { 
		character = SendMessage(fixCsvDataPtr->currentScintilla,
			SCI_GETCHARAT,
			(WPARAM)(*positionCharacterIndex) + sciCharAtIndex,
			NOT_USED_LPARAM);
		// TODO Remove.
//		wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
		// TODO Remove.
//		swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT("DEBUG-Len(%d)=%d StartPosition=%d Split=(%d,'%c') %d '%c'"),
//			fixCsvDataPtr->sciLineIndex + 1,
//			fixCsvDataPtr->sciLineLengthCurrentIndex,
//			fixCsvDataPtr->sciStartPositionLineIndex,
//			integerSplitListIndex->integer,
//			(integerSplitListIndex->separator==NULL? ' ' : integerSplitListIndex->separator),
//			character, (character==NULL? ' ': character));
		// TODO Remove.
//		SendMessage(fixCsvDataPtr->staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
//		::MessageBox(fixCsvDataPtr->hWndDlg, TEXT("Push"), NPP_PLUGIN_NAME, MB_ICONINFORMATION | MB_OK);
	} while ((character != integerSplitListIndex->separator) &&
		(++sciCharAtIndex < fixCsvDataPtr->sciLineLengthCurrentIndex));
	// TODO Remove.
//	wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
	// TODO Remove.
//	swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT("while %c != %c && %d < %d"),
//		character,
//		integerSplitListIndex->separator,
//		sciCharAtIndex,
//		fixCsvDataPtr->sciLineLengthCurrentIndex);
	// TODO Remove.
//	SendMessage(fixCsvDataPtr->staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
//	::MessageBox(fixCsvDataPtr->hWndDlg, TEXT("Push"), NPP_PLUGIN_NAME, MB_ICONINFORMATION | MB_OK);
	// We analyze the reason why we leave the while loop.
	if (sciCharAtIndex >= fixCsvDataPtr->sciLineLengthCurrentIndex) {
		/*
		 * We have exceeded the total length of the line, and by the first condition 
		 *of the while loop, we know that the last character is not the separator.
		 */
		status = REACHED_ENDOFLINE_STATUS;
	}
	else {
		*positionCharacterIndex += sciCharAtIndex;
		/*
		 * If we have not reached the end of the line, then we have detected the separator character.
		 * We must remember that the separating character is not included in the length of the split.
		 * ++sciCharAtIndex did not run.
		 */
		if (sciCharAtIndex == integerSplitListIndex->integer) {
			status = REACHED_SEPARATOR_IN_LENGTH_STATUS;
		}
		else {
			if (sciCharAtIndex < integerSplitListIndex->integer) {
				status = REACHED_SEPARATOR_BEFORE_LENGTH_STATUS;
			}
			else {
				status = REACHED_SEPARATOR_AFTER_LENGTH_STATUS;
			}
		}
	}
	// TODO Remove.
//	swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT("Status=%d"),status);
	// TODO Remove.
//	SendMessage(fixCsvDataPtr->staticActionHandle, WM_SETTEXT, NOT_USED_WPARAM, LPARAM(&buffer));
//	Sleep(3000);
	return status;
}

//
// fixDlgProc_FixLine
//
int fixDlgProc_FixLine(FixCsvDataPtr fixCsvDataPtr) {
	IntegerSplitPtr integerSplitListIndex = fixCsvDataPtr->integerSplitList;
	int status = SEARCHING_SEPARATOR_STATUS;
	int result = NO_ERROR_FIXING;
	intptr_t positionCharacterIndex = 0;
	/*
	 * For this line, the following value never changes, even though we can add or insert more characters.
	 */
	fixCsvDataPtr->sciStartPositionLineIndex = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_POSITIONFROMLINE, (WPARAM)fixCsvDataPtr->sciLineIndex, NOT_USED_LPARAM);
	while ((status < ERROR_REACHED_ENDOFLINE_STATUS) && (integerSplitListIndex != INTEGER_SPLITTER_NULL)) {
		/*
		 * After treating each split, we must recalculate the total length of the line.
		 */
		fixCsvDataPtr->sciLineLengthCurrentIndex = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETLINEENDPOSITION, (WPARAM)fixCsvDataPtr->sciLineIndex, NOT_USED_LPARAM)
			- fixCsvDataPtr->sciStartPositionLineIndex;
		status = fixDlgProc_FixLine_searchSeparator(SEARCHING_SEPARATOR_STATUS, fixCsvDataPtr, integerSplitListIndex, &positionCharacterIndex);
		/*
		 * We analyze the returned status, to decide what action to take.
		 */
		switch (status) {
			case REACHED_ENDOFLINE_STATUS:
				if (integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
					/*
					 * We have reached the end of the line, but there are still splits to try.
					 */
					status = ERROR_REACHED_ENDOFLINE_STATUS;
				}
				break;
			case REACHED_SEPARATOR_IN_LENGTH_STATUS:
				positionCharacterIndex++;
				break;
			case REACHED_SEPARATOR_BEFORE_LENGTH_STATUS:
				positionCharacterIndex++;
				break;
			case REACHED_SEPARATOR_AFTER_LENGTH_STATUS:
				positionCharacterIndex++;
				break;
		}
		integerSplitListIndex = integerSplitListIndex->next;
	}
	return result;
}

//
// fixDlgProc_FixAllLines
//
DWORD WINAPI fixDlgProc_FixAllLines(LPVOID lpParam) {
	FixCsvDataPtr fixCsvDataPtr;

	fixCsvDataPtr = (FixCsvDataPtr)lpParam;
	// We begin to fix each line of the document.
	for (fixCsvDataPtr->sciLineIndex = 0, fixCsvDataPtr->sciStartPositionLineIndex = 0;
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
			SendMessage(fixCsvDataPtr->hWndDlg, WM_USER_ERROR_FIXING, NOT_USED_WPARAM, (LPARAM)fixCsvDataPtr);
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
		return NO_CURRENT_SCINTILLA_ERROR_INIT_DIALOG;
	}
	fixCsvDataPtr->currentScintilla = (which == 0) ? nppData._scintillaMainHandle : nppData._scintillaSecondHandle;
	fixCsvDataPtr->sciLineCount = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETLINECOUNT, NOT_USED_WPARAM, NOT_USED_LPARAM);
	// Progress Bar
	fixCsvDataPtr->progressBarHandle = fixDlgProc_DialogFunc_InitProgressBar(fixCsvDataPtr);
	// Thread
	fixCsvDataPtr->fixThreadHandle = CreateThread(NULL, 0, fixDlgProc_FixAllLines, fixCsvDataPtr, 0, NULL);
	if (fixCsvDataPtr->fixThreadHandle == NULL) {
		return NO_FIX_THREAD_ERROR_INIT_DIALOG;
	}
	return NO_ERROR_INIT_DIALOG;
}

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_INITDIALOG: {
			FixCsvDataPtr fixCsvDataPtr;
			int errorInitDialog;
			if (lParam == NULL) {
				EndDialog(hWndDlg, NO_FIX_CSV_DATA_ERROR_INIT_DIALOG);
				return TRUE;
			}
			fixCsvDataPtr = (FixCsvDataPtr)lParam;
			if (fixCsvDataPtr->integerSplitList == NULL) {
				EndDialog(hWndDlg, NO_INTEGER_SPLIT_DATA_ERROR_INIT_DIALOG);
				return TRUE;
			}
			// We take note of the dialog window handler.
			fixCsvDataPtr->hWndDlg = hWndDlg;
			errorInitDialog = fixDlgProc_DialogFunc_InitDialog(fixCsvDataPtr);
			if (errorInitDialog != NO_ERROR_INIT_DIALOG) {
				EndDialog(hWndDlg, errorInitDialog);
				return TRUE;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))	{
			case IDCANCEL:
				// Someone pressed the cancel button of the fixing operation.
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
	case WM_USER_ERROR_FIXING: {
			FixCsvDataPtr fixCsvDataPtr;
			if (lParam == NULL) {
				EndDialog(hWndDlg, NO_FIX_CSV_DATA_ERROR_INIT_DIALOG);
				return TRUE;
			}
			fixCsvDataPtr = (FixCsvDataPtr)lParam;
			wchar_t buffer[LENGTH_ERROR_FIXING_TEXT];
			swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT(ERROR_FIXING_TEXT), fixCsvDataPtr->sciLineIndex + 1);
			::MessageBox(hWndDlg, buffer, NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
			return TRUE;
		}
		// break;
	default:
		break;
	}
	return FALSE;
}
