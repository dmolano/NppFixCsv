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
// fixCsvData
//
FixCsvData fixCsvData;

//
// nppFixCsv_ReadSettingsIni
//
DWORD nppFixCsv_ReadSettingsIni() {
	DWORD result = NOERROR;
	//result = ini_ReadStringData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("LENGTHS_UNICODE_DATA_STRING"), &fixCsvData.settingsData.lengthsUnicodeDataString);
	return result;
}

//
// nppFixCsv_RefreshSettingsIni
//
DWORD nppFixCsv_WriteSettingsIni() {
	DWORD result = NOERROR;

	if (((result = ini_WriteIntData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("MAJOR_VERSION_NUMBER"), fixCsvData.settingsData.nppFixCsvMajorVersionNumber)) == NOERROR) &&
		((result = ini_WriteIntData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("MINOR_VERSION_NUMBER"), fixCsvData.settingsData.nppFixCsvMinorVersionNumber)) == NOERROR) &&
		((result = ini_WriteIntData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("SETTINGS_MAJOR_VERSION_NUMBER"), fixCsvData.settingsData.nppFixCsvSettingsMajorVersionNumber)) == NOERROR) &&
		((result = ini_WriteIntData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("SETTINGS_MINOR_VERSION_NUMBER"), fixCsvData.settingsData.nppFixCsvSettingsMinorVersionNumber)) == NOERROR) &&
		((result = ini_WriteIntData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("LENGTHS_UNICODE_DATA_LENGTH"), fixCsvData.settingsData.lengthsUnicodeDataLength)) == NOERROR) &&
		((result = ini_WriteStringData(&(fixCsvData.iniData), TEXT("SETTINGS"), TEXT("LENGTHS_UNICODE_DATA_STRING"), fixCsvData.settingsData.lengthsUnicodeDataString)) == NOERROR)) {
		// TODO OK
	}
	else {
		// TODO Error
	}
	return result;
}

//----------------------------------------------//
//-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
//----------------------------------------------//
//
// nppFixCsv_FunctionFix
//
void nppFixCsv_FunctionFix()
{
	if (fixCsvData.fixingData.fixThreadHandle != NULL) {
		CloseHandle(fixCsvData.fixingData.fixThreadHandle);
		fixCsvData.fixingData.fixThreadHandle = NULL;
	}
	if (fixCsvData.fixingData.filler != NULL) {
		delete[] fixCsvData.fixingData.filler;
		fixCsvData.fixingData.filler = NULL;
	}
	fixCsvData.fixingData.filler = new char[fixCsvData.settingsData.splitIntegerValueMax + 1];
	if (fixCsvData.fixingData.filler == NULL) {
		Beep(1000, 10);
		enableMenuItem(fixCsvData.nppData._nppHandle, fixCsvData.functionItems[NPP_PLUGIN_FIX_MENUITEM_INDEX]._cmdID, FALSE);
	}
	memset(fixCsvData.fixingData.filler, ' ', fixCsvData.settingsData.splitIntegerValueMax);
	fixCsvData.fixingData.filler[fixCsvData.settingsData.splitIntegerValueMax] = 0;
	if (DialogBoxParam(fixCsvData.g_DllhInst, MAKEINTRESOURCE(IDD_DIALOG_FIX), fixCsvData.nppData._nppHandle, fixDlgProc_DialogFunc, (LPARAM)&fixCsvData) == 1) {
	}
}

//
// nppFixCsv_FunctionSettingsDlg
//
void nppFixCsv_FunctionSettingsDlg()
{
	if (DialogBoxParam(fixCsvData.g_DllhInst, MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), fixCsvData.nppData._nppHandle, settingsDlgProc_DialogFunc, (LPARAM) &(fixCsvData.settingsData)) == 1) {
		fixCsvData.settingsData.nppFixCsvMajorVersionNumber = NPP_FIX_CSV_MAJOR_VERSION_NUMBER;
		fixCsvData.settingsData.nppFixCsvMinorVersionNumber = NPP_FIX_CSV_MINOR_VERSION_NUMBER;
		fixCsvData.settingsData.nppFixCsvSettingsMajorVersionNumber = NPP_FIX_CSV_SETTINGS_MAJOR_VERSION_NUMBER;
		fixCsvData.settingsData.nppFixCsvSettingsMinorVersionNumber = NPP_FIX_CSV_SETTINGS_MINOR_VERSION_NUMBER;
		fixCsvData.fixingData.integerSplitList = fixCsvData.settingsData.integerSplitList;
	}
	enableMenuItem(fixCsvData.nppData._nppHandle, fixCsvData.functionItems[NPP_PLUGIN_FIX_MENUITEM_INDEX]._cmdID, (fixCsvData.fixingData.integerSplitList != NULL));
	if (fixCsvData.fixingData.integerSplitList != NULL) {
		nppFixCsv_WriteSettingsIni();
	}
}

//
// nppFixCsv_FunctionAboutDlg
//
void nppFixCsv_FunctionAboutDlg()
{
	if (DialogBox(fixCsvData.g_DllhInst, MAKEINTRESOURCE(IDD_DIALOG_ABOUT), fixCsvData.nppData._nppHandle, aboutDlgProc_DialogFunc) == 1) {
	}
}

//
// This function help you to initialize your plugin commands
//
bool nppFixCsv_SetPluginCommand(size_t index, const TCHAR* cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey* sk, bool check0nInit)
{
	if (index >= NPP_PLUGIN_FUNCTIONS) {
		return false;
	}
	if (!pFunc) {
		return false;
	}
	lstrcpy(fixCsvData.functionItems[index]._itemName, cmdName);
	fixCsvData.functionItems[index]._pFunc = pFunc;
	fixCsvData.functionItems[index]._init2Check = check0nInit;
	fixCsvData.functionItems[index]._pShKey = sk;
	return true;
}

//
// Here you can do the clean up (especially for the shortcut)
//
void nppFixCsv_CommandMenuCleanUp()
{
	// Don't forget to deallocate your shortcut here
}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void nppFixCsv_CommandMenuInit()
{
	//--------------------------------------------//
	//-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
	//--------------------------------------------//
	// with function :
	// setCommand(int index,                      // zero based number to indicate the order of command
	//            TCHAR *commandName,             // the command name that you want to see in plugin menu
	//            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
	//            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
	//            bool check0nInit                // optional. Make this menu item be checked visually
	//            );
	nppFixCsv_SetPluginCommand(NPP_PLUGIN_FIX_MENUITEM_INDEX, NPP_PLUGIN_FIX_MENUITEM_NAME, nppFixCsv_FunctionFix, NULL, false);
	nppFixCsv_SetPluginCommand(NPP_PLUGIN_SETTINGS_MENUITEM_INDEX, NPP_PLUGIN_SETTINGS_MENUITEM_NAME, nppFixCsv_FunctionSettingsDlg, NULL, false);
	nppFixCsv_SetPluginCommand(NPP_PLUGIN_ABOUT_MENUITEM_INDEX, NPP_PLUGIN_ABOUT_MENUITEM_NAME, nppFixCsv_FunctionAboutDlg, NULL, false);
}

//
// Initialize your plugin data here
// It will be called while plugin loading 
//
void nppFixCsv_PluginDllProcessAttach(HANDLE hModule) {
	fixCsvData.g_DllhInst = (HINSTANCE)hModule;
	memset(&(fixCsvData.fixingData), 0, sizeof(FixingData));
}

//
// nppFixCsv_PluginDllProcessDetach
//    
void nppFixCsv_PluginDllProcessDetach() {
	if (fixCsvData.fixingData.fixThreadHandle != NULL) {
		CloseHandle(fixCsvData.fixingData.fixThreadHandle);
		fixCsvData.fixingData.fixThreadHandle = NULL;
	}
	if (fixCsvData.fixingData.filler != NULL) {
		delete[] fixCsvData.fixingData.filler;
		fixCsvData.fixingData.filler = NULL;
	}
	integerSplitter_Init(&(fixCsvData.fixingData.integerSplitList));
	memset(&(fixCsvData.fixingData), 0, sizeof(FixingData));
	//	if (lengthsUnicodeDataStringSettings != NULL) {
//		delete[] lengthsUnicodeDataStringSettings;
//		lengthsUnicodeDataStringSettings = NULL;
//	}
}

//
// nppFixCsv_StartIni
//
errno_t nppFixCsv_StartIni() {
	TCHAR configDir[MAX_PATH];
	errno_t result;

	SendMessage(fixCsvData.nppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configDir);
	fixCsvData.iniData.errnoInited = EOTHER;
	fixCsvData.iniData.fileName = NPP_PLUGIN_NAME;
	result = ini_SetIniPath(&(fixCsvData.iniData), configDir);
	if (result == NOERROR) {

	}
	return result;
}

//
// setInfo lo utiliza NotePad++ para acceder al Plugin, y proporcionarle información a través de la estructura PFUNCSETINFO.
// NppData está definido en PluginInterface.h
//
extern "C" __declspec(dllexport) void setInfo(NppData notepadPlusData) {
	fixCsvData.nppData = notepadPlusData;
	nppFixCsv_CommandMenuInit();
}

//
// getName lo utiliza NotePad++ para acceder al Plugin.
//
extern "C" __declspec(dllexport) const TCHAR * getName() {
	return NPP_PLUGIN_NAME; // Definido en NppFixCsv.h
}

//
// getFuncsArray lo utiliza NotePad++ para acceder al Plugin, y averiguar el número de funciones, así como, tener una referencia a estas.
//
extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int* nbF) {
	*nbF = NPP_PLUGIN_FUNCTIONS; // NPP_PLUGIN_FUNCTIONS definido en NppFixCsv.h
	return fixCsvData.functionItems; // functionItems definido en NppFixCsv.cpp
}

//
// beNotified lo utiliza NotePad++ para acceder al Plugin, y notificarle cambios a través de la estructura PBENOTIFIED.
// SCNotification está definido en PluginInterface.h
//
extern "C" __declspec(dllexport) void beNotified(SCNotification * notifyCode) {
	switch (notifyCode->nmhdr.code)
	{
	case NPPN_READY: 
		enableMenuItem(fixCsvData.nppData._nppHandle, fixCsvData.functionItems[NPP_PLUGIN_FIX_MENUITEM_INDEX]._cmdID, FALSE);
		if (nppFixCsv_StartIni() != NOERROR) {
			// TODO Without iniData.
		}
		else {
			nppFixCsv_ReadSettingsIni();
		}
		break;
	case NPPN_SHUTDOWN:
		nppFixCsv_CommandMenuCleanUp();
		break;
	default:
		return;
	}
}

// Here you can process the Npp Messages 
// I will make the messages accessible little by little, according to the need of plugin development.
// Please let me know if you need to access to some messages :
// http://sourceforge.net/forum/forum.php?forum_id=482781
//
extern "C" __declspec(dllexport) LRESULT messageProc(UINT /*Message*/, WPARAM /*wParam*/, LPARAM /*lParam*/) {
	/*
		if (Message == WM_MOVE)
		{
			::MessageBox(NULL, "move", "", MB_OK);
		}
	*/
	return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode() {
	return TRUE;
}
#endif //UNICODE
