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
// fixThreadHandle
//
HANDLE fixThreadHandle;

//
// fixDlgProc_Fix
//
DWORD WINAPI fixDlgProc_Fix(LPVOID lpParam) {
	HWND hWndDlg;
	hWndDlg = (HWND)lpParam;
	Sleep(3000);
	SendMessage(hWndDlg, WM_CLOSE, 0, 0);
	return 0;
}

//
// fixDlgProc_DialogFunc
//
INT_PTR CALLBACK fixDlgProc_DialogFunc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		centerWndDlg(hWndDlg);
		fixThreadHandle = CreateThread(NULL, 0, fixDlgProc_Fix, hWndDlg, 0, NULL);
		if (fixThreadHandle == NULL) {
			EndDialog(hWndDlg, 0);
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
			if (fixThreadHandle != NULL) {
//				CloseHandle(fixThreadHandle);
//				fixThreadHandle = NULL;
			}
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
		if (fixThreadHandle != NULL) {
			CloseHandle(fixThreadHandle);
			fixThreadHandle = NULL;
		}
		return TRUE;
	}
	return FALSE;
}


