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
#include "pch.h"

#define INTEGER_SPLITTER_NULL 0
#define INTEGER_SPLITTER_NO_ERROR 0
#define INITIAL_STATE_SPLITTER 0
#define SEPARATOR_STATE_SPLITTER 1
#define DIGIT_STATE_SPLITTER 2
#define ERROR_STATE_SPLITTER 3
#define MAX_DIGITS_LENGTH 5 // 99999
#define BASE_10 10
#define LAST_SEPARATOR 0

//
// IntegerSplit
// 
typedef struct IntegerSplit { //name of the structure
	int integer;
	int separator;
	struct IntegerSplit* next;
}IntegerSplitType;// type of data to declare the structure

typedef IntegerSplitType* IntegerSplitPtr;

//
// integerSplitter_Init
//
int integerSplitter_Init(IntegerSplitPtr* integerSplitPtr);

//
// integerSplitter_Split
//
IntegerSplitPtr integerSplitter_Split(const char* string);