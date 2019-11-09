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
	bool exit = FALSE;
	do { 
		character = SendMessage(fixCsvDataPtr->currentScintilla,
			SCI_GETCHARAT,
			(WPARAM)(*positionCharacterIndex) + sciCharAtIndex,
			NOT_USED_LPARAM);
		/*
		 * Reasons for going out the loop:
		 *    1.- The character read matches the split's separator.
		 *    2.- ((*positionCharacterIndex) + ++sciCharAtIndex - fixCsvDataPtr->sciStartPositionLineIndex) matches with line's length.
		 */
		if (character == integerSplitListIndex->separator) {
			exit = TRUE;
		}
		else {
			++sciCharAtIndex;
			intptr_t splitCurrentLength = (*positionCharacterIndex) + sciCharAtIndex - fixCsvDataPtr->sciStartPositionLineIndex;
			if (splitCurrentLength >= fixCsvDataPtr->sciLineLengthCurrentIndex) {
				exit = TRUE;
			}
		}
	} while (exit == FALSE);
	// We analyze the reason why we leave the while loop.
	if (character == integerSplitListIndex->separator) {
		/*
		 * The following condition of the while loop was not executed, and therefore, sciCharAtIndex was not increased.
		 */
		*positionCharacterIndex += sciCharAtIndex;
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
	else {
		/*
		 * If we have not abandoned the while loop for the first condition, then we have done it for the second one, 
		 *and in this case, I have exceeded the line character limit, without finding the desired separator.
		 */
		*positionCharacterIndex += sciCharAtIndex;
		if (sciCharAtIndex == integerSplitListIndex->integer) {
			status = REACHED_ENDOFLINE_IN_LENGTH_STATUS;
		}
		else {
			if (sciCharAtIndex < integerSplitListIndex->integer) {
				status = REACHED_ENDOFLINE_BEFORE_LENGTH_STATUS;
			}
			else {
				status = REACHED_ENDOFLINE_AFTER_LENGTH_STATUS;
			}
		}
	}
	return status;
}

//
// fixDlgProc_FixLine
//
int fixDlgProc_FixLine(FixCsvDataPtr fixCsvDataPtr) {
	IntegerSplitPtr integerSplitListIndex = fixCsvDataPtr->integerSplitList;
	int status = SEARCHING_SEPARATOR_STATUS;
	intptr_t positionCharacterIndex = 0;
	/*
	 * For this line, the following value never changes, even though we can add or insert more characters.
	 */
	fixCsvDataPtr->sciStartPositionLineIndex = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_POSITIONFROMLINE, (WPARAM)fixCsvDataPtr->sciLineIndex, NOT_USED_LPARAM);
	positionCharacterIndex = fixCsvDataPtr->sciStartPositionLineIndex;
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
			case REACHED_ENDOFLINE_IN_LENGTH_STATUS:
				if (integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
					/*
					 * We have reached the end of the line, but there are still splits to try.
					 */
					status = ERROR_REACHED_ENDOFLINE_STATUS;
				}
				else {
					/*
					 * We have reached the end of the line and splits too.
					 */
				}
				break;
			case REACHED_ENDOFLINE_BEFORE_LENGTH_STATUS:
				if (integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
					/*
					 * We have reached the end of the line, but there are still splits to try.
					 */
					status = ERROR_REACHED_ENDOFLINE_STATUS;
				}
				else {
					/*
					 * We have reached the end of the line and splits too.
					 */
				}
				break;
			case REACHED_ENDOFLINE_AFTER_LENGTH_STATUS:
				if (integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
					/*
					 * We have reached the end of the line, but there are still splits to try.
					 */
					status = ERROR_REACHED_ENDOFLINE_STATUS;
				}
				else {
					/*
					 * We have reached the end of the line and splits too.
					 */
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
	return status;
}

//
// fixDlgProc_FixAllLines
//
DWORD WINAPI fixDlgProc_FixAllLines(LPVOID lpParam) {
	FixCsvDataPtr fixCsvDataPtr;
	int status = SEARCHING_SEPARATOR_STATUS;
	bool cancelFix = FALSE;

	fixCsvDataPtr = (FixCsvDataPtr)lpParam;
	// We begin to fix each line of the document.
	// We check that after fixing a line, they have not pressed the cancel button: button cancel is enabled.
	for (fixCsvDataPtr->sciLineIndex = 0, fixCsvDataPtr->sciStartPositionLineIndex = 0;
		(status < ERROR_REACHED_ENDOFLINE_STATUS) && 
			(cancelFix == FALSE) &&
			(fixCsvDataPtr->sciLineIndex < fixCsvDataPtr->sciLineCount);
		fixCsvDataPtr->sciLineIndex++) {
		// Next progress bar step.
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_STEPIT, NOT_USED_WPARAM, NOT_USED_LPARAM);
		// First we fix.
		status = fixDlgProc_FixLine(fixCsvDataPtr);
		// Then, we verify that we do not have a cancellation.
		cancelFix = !IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle);
	}
	if (status >= ERROR_REACHED_ENDOFLINE_STATUS) {
		// Progress bar in red.
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_ERROR, NOT_USED_LPARAM);
		// Requesting to display an error message in the dialog window with the line as information.
		SendMessage(fixCsvDataPtr->hWndDlg, WM_USER_ERROR_FIXING, NOT_USED_WPARAM, (LPARAM)fixCsvDataPtr);
	}
	else {
		// Did anyone press the cancel button of the fixing operation?
		if (cancelFix == TRUE) {
			SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_PAUSED, NOT_USED_LPARAM);
		}
	}
	// Whatever happens, we give the order to close the dialog window.
	SendMessage(fixCsvDataPtr->hWndDlg, WM_CLOSE, NOT_USED_WPARAM, NOT_USED_LPARAM);
	return cancelFix;
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
