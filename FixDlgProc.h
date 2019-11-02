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

#define NO_ERROR_INIT 0

#define NO_ERROR_FIXING 0

#define CANCELING_TEXT "Canceling..."

#define ERROR_FIXING_TEXT "Error fixing in line %lld."

#define LENGTH_ERROR_FIXING_TEXT 150

#define WM_USER_ERROR_FIXING WM_USER + 1

#define SEARCHING_SEPARATOR_STATUS 0

#define REACHED_SEPARATOR_STATUS SEARCHING_SEPARATOR_STATUS + 1

//
// FixCsvData
// 
typedef struct FixCsvData { //name of the structure
	// INTEGER SPLIT
	IntegerSplitPtr integerSplitList;
	// THREAD
	HANDLE fixThreadHandle;
	// SEMAPHORE CANCEL
//	BOOL semaphoreCancel;
	// DIALOG
	HWND hWndDlg;
	HWND progressBarHandle;
	HWND staticActionHandle;
	HWND buttonCancelHandle;
	// SCINTILLA
	HWND currentScintilla;
	intptr_t sciLineCount;
	intptr_t sciLineIndex;
	intptr_t sciPositionIndex;
}FixCsvDataType;// type of data to declare the structure

//
// FixCsvDataPtr
//
typedef FixCsvDataType* FixCsvDataPtr;

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam); 
