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
// centerWndDlg
//
void centerWndDlg(const HWND hWndDlg)
{
	HWND hwndOwner;
	RECT rc, rcDlg, rcOwner;

	// Get the owner window and dialog box rectangles. 
	if ((hwndOwner = GetParent(hWndDlg)) == NULL) {
		hwndOwner = GetDesktopWindow();
	}
	GetWindowRect(hwndOwner, &rcOwner);
	GetWindowRect(hWndDlg, &rcDlg);
	CopyRect(&rc, &rcOwner);
	// Offset the owner and dialog box rectangles so that right and bottom 
	// values represent the width and height, and then offset the owner again 
	// to discard space taken up by the dialog box. 
	OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
	OffsetRect(&rc, -rc.left, -rc.top);
	OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);
	// The new position is the sum of half the remaining space and the owner's 
	// original position. 
	SetWindowPos(hWndDlg,
		HWND_TOP,
		rcOwner.left + (rc.right / 2),
		rcOwner.top + (rc.bottom / 2),
		0, 0,          // Ignores size arguments. 
		SWP_NOSIZE);
}

//
// enableMenuItem
//
void enableMenuItem(const HWND hWnd, const int cmdID, const int enable) {
	HMENU hMenu = (HMENU)::SendMessage(hWnd, NPPM_GETMENUHANDLE, NPPPLUGINMENU, 0);
	::EnableMenuItem(hMenu, cmdID,
		MF_BYCOMMAND | (enable ? MF_ENABLED : MF_DISABLED) | (enable ?  MF_DEFAULT: MF_GRAYED));

}
