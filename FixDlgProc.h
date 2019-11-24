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

#define NO_FIX_CSV_DATA_ERROR_INIT_DIALOG NO_ERROR_INIT_DIALOG + 1

#define NO_INTEGER_SPLIT_DATA_ERROR_INIT_DIALOG NO_ERROR_INIT_DIALOG + 2

#define NO_CURRENT_SCINTILLA_ERROR_INIT_DIALOG NO_ERROR_INIT_DIALOG + 3

#define NO_FIX_THREAD_ERROR_INIT_DIALOG NO_ERROR_INIT_DIALOG + 4

// ERROR FIXING
//#define NO_ERROR_FIXING 0

// CANCELLING
#define CANCELING_TEXT "Canceling..."

// ERROR
#define ERROR_FIXING_TEXT "Error fixing in line %lld."

#define LENGTH_ERROR_FIXING_TEXT 150

#define WM_USER_ERROR_FIXING WM_USER + 1

// STATUS
#define SEARCHING_SEPARATOR_STATUS 0

#define REACHED_ENDOFLINE_IN_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 1

#define REACHED_ENDOFLINE_BEFORE_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 2

#define REACHED_ENDOFLINE_AFTER_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 3

#define REACHED_SEPARATOR_IN_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 4

#define REACHED_SEPARATOR_BEFORE_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 5

#define REACHED_SEPARATOR_AFTER_LENGTH_SPLIT_STATUS SEARCHING_SEPARATOR_STATUS + 6

#define FILLING_LINE_WITH_SPACE_STATUS SEARCHING_SEPARATOR_STATUS + 7

#define REDUCING_LINE_STATUS SEARCHING_SEPARATOR_STATUS + 8


#define ERROR_REACHED_ENDOFLINE_STATUS SEARCHING_SEPARATOR_STATUS + 100

//
// FixingData
// 
typedef struct FixingData { //name of the structure
	// Fixing
	int fixingStatus;
	// INTEGER SPLIT
	IntegerSplitPtr integerSplitList;
	IntegerSplitPtr integerSplitListIndex;
	// THREAD
	HANDLE fixThreadHandle;
	// DIALOG
	HWND hWndDlg;
	HWND progressBarHandle;
	HWND staticActionHandle;
	HWND buttonCancelHandle;
	// SCINTILLA
	HWND currentScintilla;
	intptr_t sciDocumentLinesCount;
	intptr_t sciDocumentCurrentLineIndex;
	intptr_t sciDocumentCurrentLineLength; // Without the end of the line.
	intptr_t sciDocumentCurrentLinePositionStart;
	intptr_t sciDocumentCurrentLineCharacterPositionIndex;
	intptr_t sciDocumentLineCharacterSplitIntegerIndex;
	char* filler;
	bool canUndo;
}FixingDataType;// type of data to declare the structure

//
// FixingDataPtr
//
typedef FixingDataType* FixingDataPtr;

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); 
