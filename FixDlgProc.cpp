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
// splitIntegerValueMax
//
extern int splitIntegerValueMax;

//
// The data of Notepad++ that you can use in your plugin commands
//
extern NppData nppData;

//
// fixDlgProc_FixLine_reduceAnyCharacter
//
int fixDlgProc_FixLine_reduceLine(int status, FixCsvDataPtr fixCsvDataPtr, intptr_t difference) {
	SendMessage(fixCsvDataPtr->currentScintilla,
		SCI_DELETERANGE,
		(WPARAM)fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex - difference - 1,
		(LPARAM)difference);
	fixCsvDataPtr->canUndo = true;
	fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex -= difference;
	return status;
}

//
// fixDlgProc_FixLine_fillLine
//
int fixDlgProc_FixLine_fillLine(int status, FixCsvDataPtr fixCsvDataPtr, intptr_t difference) {
	fixCsvDataPtr->filler[difference] = 0;
	SendMessage(fixCsvDataPtr->currentScintilla,
		SCI_INSERTTEXT,
		(WPARAM)fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex - 1,
		(LPARAM)fixCsvDataPtr->filler);
	fixCsvDataPtr->canUndo = true;
	fixCsvDataPtr->filler[difference] = ' ';
	fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex += difference;
	return status;
}

//
// fixDlgProc_FixLine_searchSeparator
//
int fixDlgProc_FixLine_searchSeparator(int status, FixCsvDataPtr fixCsvDataPtr) {
	int character = NULL;

	fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex = 0;
	// Just in case.
	if (status != SEARCHING_SEPARATOR_STATUS) {
		return status;
	}
	intptr_t splitCurrentLength = fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex
		- fixCsvDataPtr->sciDocumentCurrentLinePositionStart;
	if (splitCurrentLength < fixCsvDataPtr->sciDocumentCurrentLineLength) {
		// while loop starts.
		bool exit = FALSE;
		do {
			character = SendMessage(fixCsvDataPtr->currentScintilla,
				SCI_GETCHARAT,
				(WPARAM)(fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex + fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex),
				NOT_USED_LPARAM);
			/*
			 * Reasons for going out the loop:
			 *    1.- The character read matches the split's separator.
			 *    2.- (sciDocumentCurrentLineCharacterPositionIndex + (++sciDocumentLineCharacterIndex) - sciDocumentCurrentLinePositionStart) matches with line's length.
			 */
			if (character == fixCsvDataPtr->integerSplitListIndex->separator) {
				exit = TRUE;
			}
			else {
				// Increasing the variable, we obtain the length traveled so far.
				++(fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex);
				// We calculate the total length traveled within the line.
				intptr_t splitCurrentLength = fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex
					+ fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex
					- fixCsvDataPtr->sciDocumentCurrentLinePositionStart;
				if (splitCurrentLength >= fixCsvDataPtr->sciDocumentCurrentLineLength) {
					// We have reached the end of the total length of the line we are traveling.
					exit = TRUE;
				}
			}
		} while (exit == FALSE);
	}
	else {
		// We must avoid entering into "if (character == fixCsvDataPtr->integerSplitListIndex->separator) {"
		character = fixCsvDataPtr->integerSplitListIndex->separator + 1;
	}
	// We analyze the reason why we leave the while loop.
	if (character == fixCsvDataPtr->integerSplitListIndex->separator) {
		/*
		 * The following condition of the while loop was not executed, and therefore, sciCharAtIndex was not increased.
		 */
		fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex += fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex;
		if (fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex == fixCsvDataPtr->integerSplitListIndex->integer) {
			status = REACHED_SEPARATOR_IN_LENGTH_SPLIT_STATUS;
		}
		else {
			if (fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex < fixCsvDataPtr->integerSplitListIndex->integer) {
				status = REACHED_SEPARATOR_BEFORE_LENGTH_SPLIT_STATUS;
			}
			else {
				status = REACHED_SEPARATOR_AFTER_LENGTH_SPLIT_STATUS;
			}
		}
	}
	else {
		/*
		 * If we have not abandoned the while loop for the first condition, then we have done it for the second one,
		 *and in this case, I have exceeded the line character limit, without finding the desired separator.
		 */
		fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex += fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex;
		if (fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex == fixCsvDataPtr->integerSplitListIndex->integer) {
			status = REACHED_ENDOFLINE_IN_LENGTH_SPLIT_STATUS;
		}
		else {
			if (fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex < fixCsvDataPtr->integerSplitListIndex->integer) {
				status = REACHED_ENDOFLINE_BEFORE_LENGTH_SPLIT_STATUS;
			}
			else {
				status = REACHED_ENDOFLINE_AFTER_LENGTH_SPLIT_STATUS;
			}
		}
	}
	return status;
}

//
// fixDlgProc_FixLine
//
int fixDlgProc_FixLine(FixCsvDataPtr fixCsvDataPtr) {
	int fixingStatus = SEARCHING_SEPARATOR_STATUS;

	fixCsvDataPtr->integerSplitListIndex = fixCsvDataPtr->integerSplitList;
	/*
	 * For this line, the following value never changes, even though we can add or insert more characters.
	 */
	fixCsvDataPtr->sciDocumentCurrentLinePositionStart = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_POSITIONFROMLINE, (WPARAM)fixCsvDataPtr->sciDocumentCurrentLineIndex, NOT_USED_LPARAM);
	fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex = fixCsvDataPtr->sciDocumentCurrentLinePositionStart;
	while ((fixingStatus < ERROR_REACHED_ENDOFLINE_STATUS) && (fixCsvDataPtr->integerSplitListIndex != INTEGER_SPLITTER_NULL)) {
		/*
		 * After treating each split, we must recalculate the total length of the line.
		 */
		fixCsvDataPtr->sciDocumentCurrentLineLength = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETLINEENDPOSITION, (WPARAM)fixCsvDataPtr->sciDocumentCurrentLineIndex, NOT_USED_LPARAM)
			- fixCsvDataPtr->sciDocumentCurrentLinePositionStart;
		fixingStatus = fixDlgProc_FixLine_searchSeparator(SEARCHING_SEPARATOR_STATUS, fixCsvDataPtr);
		/*
		 * We analyze the returned status, to decide what action to take.
		 */
		switch (fixingStatus) {
		case REACHED_ENDOFLINE_IN_LENGTH_SPLIT_STATUS:
			if (fixCsvDataPtr->integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
				/*
				 * We have reached the end of the line, but there are still splits to try.
				 */
				fixingStatus = ERROR_REACHED_ENDOFLINE_STATUS;
			}
			else {
				/*
				 * We have reached the end of the line and splits too.
				 */
				 /*
				  * Next line.
				  */
			}
			break;
		case REACHED_ENDOFLINE_BEFORE_LENGTH_SPLIT_STATUS:
			if (fixCsvDataPtr->integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
				/*
				 * We have reached the end of the line, but there are still splits to try.
				 */
				fixingStatus = ERROR_REACHED_ENDOFLINE_STATUS;
			}
			else {
				/*
				 * We have reached the end of the line and splits too.
				 */
			}
			break;
		case REACHED_ENDOFLINE_AFTER_LENGTH_SPLIT_STATUS:
			if (fixCsvDataPtr->integerSplitListIndex->next != INTEGER_SPLITTER_NULL) {
				/*
				 * We have reached the end of the line, but there are still splits to try.
				 */
				fixingStatus = ERROR_REACHED_ENDOFLINE_STATUS;
			}
			else {
				/*
				 * We have reached the end of the line and splits too.
				 */
				fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex++;
				fixingStatus = fixDlgProc_FixLine_reduceLine(REDUCING_LINE_STATUS, fixCsvDataPtr, fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex - fixCsvDataPtr->integerSplitListIndex->integer);
			}
			break;
		case REACHED_SEPARATOR_IN_LENGTH_SPLIT_STATUS:
			fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex++;
			/*
			 * Next split.
			 */
			break;
		case REACHED_SEPARATOR_BEFORE_LENGTH_SPLIT_STATUS:
			fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex++;
			fixingStatus = fixDlgProc_FixLine_fillLine(FILLING_LINE_WITH_SPACE_STATUS, fixCsvDataPtr, fixCsvDataPtr->integerSplitListIndex->integer - fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex);
			break;
		case REACHED_SEPARATOR_AFTER_LENGTH_SPLIT_STATUS:
			fixCsvDataPtr->sciDocumentCurrentLineCharacterPositionIndex++;
			fixingStatus = fixDlgProc_FixLine_reduceLine(REDUCING_LINE_STATUS, fixCsvDataPtr, fixCsvDataPtr->sciDocumentLineCharacterSplitIntegerIndex - fixCsvDataPtr->integerSplitListIndex->integer);
			break;
		}
		fixCsvDataPtr->integerSplitListIndex = fixCsvDataPtr->integerSplitListIndex->next;
	}
	return fixingStatus;
}

//
// fixDlgProc_FixAllLines
//
DWORD WINAPI fixDlgProc_FixAllLines(LPVOID lpParam) {
	FixCsvDataPtr fixCsvDataPtr;
	int fixingStatus = SEARCHING_SEPARATOR_STATUS;
	bool cancelFix = FALSE;
	fixCsvDataPtr = (FixCsvDataPtr)lpParam;
	bool buffereddraw = SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETBUFFEREDDRAW, (WPARAM)TRUE, NOT_USED_LPARAM);
	SendMessage(fixCsvDataPtr->currentScintilla, SCI_SETBUFFEREDDRAW, (WPARAM) TRUE, NOT_USED_LPARAM);
	SendMessage(fixCsvDataPtr->currentScintilla, SCI_BEGINUNDOACTION, NOT_USED_WPARAM, NOT_USED_LPARAM);
	fixCsvDataPtr->canUndo = false;
	// We begin to fix each line of the document.
	// We check that after fixing a line, they have not pressed the cancel button: button cancel is enabled.
	for (fixCsvDataPtr->sciDocumentCurrentLineIndex = 0, fixCsvDataPtr->sciDocumentCurrentLinePositionStart = 0;
		(fixingStatus < ERROR_REACHED_ENDOFLINE_STATUS) &&
		(cancelFix == FALSE) &&
		(fixCsvDataPtr->sciDocumentCurrentLineIndex < fixCsvDataPtr->sciDocumentLinesCount);
		fixCsvDataPtr->sciDocumentCurrentLineIndex++) {
		// Next progress bar step.
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_STEPIT, NOT_USED_WPARAM, NOT_USED_LPARAM);
		// First we fix.
		fixingStatus = fixDlgProc_FixLine(fixCsvDataPtr);
		// Then, we verify that we do not have a cancellation.
		cancelFix = !IsWindowEnabled(fixCsvDataPtr->buttonCancelHandle);
	}
	SendMessage(fixCsvDataPtr->currentScintilla, SCI_ENDUNDOACTION, NOT_USED_WPARAM, NOT_USED_LPARAM);
	SendMessage(fixCsvDataPtr->currentScintilla, SCI_SETBUFFEREDDRAW, (WPARAM)buffereddraw, NOT_USED_LPARAM);
	if (fixingStatus >= ERROR_REACHED_ENDOFLINE_STATUS) {
		if (fixCsvDataPtr->canUndo == true) {
			SendMessage(fixCsvDataPtr->currentScintilla, SCI_UNDO, NOT_USED_WPARAM, NOT_USED_LPARAM);
		}
		// Progress bar in red.
		SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETSTATE, (WPARAM)PBST_ERROR, NOT_USED_LPARAM);
		// Requesting to display an error message in the dialog window with the line as information.
		fixCsvDataPtr->fixingStatus = fixingStatus;
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
	SendMessage(fixCsvDataPtr->progressBarHandle, PBM_SETRANGE32, NOT_USED_WPARAM, (LPARAM)fixCsvDataPtr->sciDocumentLinesCount);
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
	fixCsvDataPtr->sciDocumentLinesCount = ::SendMessage(fixCsvDataPtr->currentScintilla, SCI_GETLINECOUNT, NOT_USED_WPARAM, NOT_USED_LPARAM);
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
		switch (LOWORD(wParam)) {
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
		swprintf_s(buffer, LENGTH_ERROR_FIXING_TEXT, TEXT(ERROR_FIXING_TEXT), fixCsvDataPtr->sciDocumentCurrentLineIndex);
		::MessageBox(hWndDlg, buffer, NPP_PLUGIN_NAME, MB_ICONERROR | MB_OK);
		SendMessage(fixCsvDataPtr->currentScintilla, SCI_GOTOLINE, (WPARAM)fixCsvDataPtr->sciDocumentCurrentLineIndex, NOT_USED_LPARAM);
		return TRUE;
	}
							 // break;
	default:
		break;
	}
	return FALSE;
}
