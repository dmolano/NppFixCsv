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

#ifndef PLUGINDEFINITION_H
#define PLUGINDEFINITION_H

// All difinitions of plugin interface
//
#include "PluginInterface.h"

//-------------------------------------//
//-- STEP 1. DEFINE YOUR PLUGIN NAME --//
//-------------------------------------//
// Here define your plugin name
//
const TCHAR NPP_PLUGIN_NAME[] = TEXT("NppFixCsv");

const WPARAM NOT_USED_WPARAM = 0L;

const LPARAM NOT_USED_LPARAM = 0L;

//-----------------------------------------------//
//-- STEP 2. DEFINE YOUR PLUGIN COMMAND NUMBER --//
//-----------------------------------------------//
//
// Here define the number of your plugin commands
//
const int NPP_PLUGIN_FUNCTIONS = 3;
// Action
const TCHAR NPP_PLUGIN_FIX_MENUITEM_NAME[] = TEXT("Fix");
const int NPP_PLUGIN_FIX_MENUITEM_INDEX = 0;
// Settings
const TCHAR NPP_PLUGIN_SETTINGS_MENUITEM_NAME[] = TEXT("Settings...");
const int NPP_PLUGIN_SETTINGS_MENUITEM_INDEX = 1;
// About
const TCHAR NPP_PLUGIN_ABOUT_MENUITEM_NAME[] = TEXT("About...");
const int NPP_PLUGIN_ABOUT_MENUITEM_INDEX = 2;

//
// Initialization of your plugin data
// It will be called while plugin loading
//
void nppFixCsv_PluginDllProcessAttach(HANDLE hModule);

//
// nppFixCsv_PluginDllProcessDetach
//
void nppFixCsv_PluginDllProcessDetach();

#endif //PLUGINDEFINITION_H
