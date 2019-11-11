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
// integerSplitter_Init
//
int integerSplitter_Init(IntegerSplitPtr* integerSplitPtr) {
	IntegerSplitPtr integerSplitListIndex = *integerSplitPtr;
	while (integerSplitListIndex != INTEGER_SPLITTER_NULL) {
		IntegerSplitPtr next = integerSplitListIndex->next;
		free((void*)integerSplitListIndex);
		integerSplitListIndex = next;
	}
	*integerSplitPtr = integerSplitListIndex;
	if (integerSplitListIndex != NULL) {
		return INTEGER_SPLITTER_NO_ERROR + 1;
	}
	return INTEGER_SPLITTER_NO_ERROR;
}

//
// integerSplitter_New
//
IntegerSplitPtr integerSplitter_New(int integer, int separator) {
	IntegerSplitPtr integerSplitPtrNew = (IntegerSplitPtr)malloc(sizeof(IntegerSplitType));
	if (integerSplitPtrNew != INTEGER_SPLITTER_NULL) {
		integerSplitPtrNew->integer = integer;
		integerSplitPtrNew->separator = separator;
		integerSplitPtrNew->next = INTEGER_SPLITTER_NULL;
	}
	return integerSplitPtrNew;
}

//
// integerSplitter_Add
//
IntegerSplitPtr integerSplitter_Add(IntegerSplitPtr* integerSplitPtr, int integer, int separator) {
	IntegerSplitPtr integerSplitPtrNew = INTEGER_SPLITTER_NULL;
	if (*integerSplitPtr == INTEGER_SPLITTER_NULL) {
		*integerSplitPtr = integerSplitter_New(integer, separator);
		integerSplitPtrNew = *integerSplitPtr;
	}
	else {
		IntegerSplitPtr integerSplitPtrIndex = *integerSplitPtr;
		while (integerSplitPtrIndex->next != INTEGER_SPLITTER_NULL) {
			integerSplitPtrIndex = integerSplitPtrIndex->next;
		}
		integerSplitPtrNew = integerSplitter_New(integer, separator);
		integerSplitPtrIndex->next = integerSplitPtrNew;
	}
	return integerSplitPtrNew;
}

//
// integerSplitter_AddDigit
//
int integerSplitter_AddDigit(int* digitsIndex, char* digits, int character) {
	int result;
	if ((*digitsIndex) < MAX_DIGITS_LENGTH) {
		digits[(*digitsIndex)++] = character;
		result = DIGIT_STATE_SPLITTER;
	}
	else {
		result = ERROR_STATE_SPLITTER;
	}
	return result;
}

//
// integerSplitter_addStoredData
//
int integerSplitter_addStoredData(char* digits, IntegerSplitPtr* integerSplitList, int character, int* digitsIndex, int* integerValueMax) {
	int state;
	char* endptr;
	int length = strtol(digits, &endptr, BASE_10);
	if (length > 0) {
		if (integerValueMax != NULL) {
			if (length > * integerValueMax) {
				*integerValueMax = length;
			}
		}
		integerSplitter_Add(integerSplitList, length, character);
		memset(digits, 0, MAX_DIGITS_LENGTH);
		*digitsIndex = 0;
		state = SEPARATOR_STATE_SPLITTER;
	}
	else {
		state = ERROR_STATE_SPLITTER;
	}
	return state;
}

//
// integerSplitter_Split
//
IntegerSplitPtr integerSplitter_Split(const char* string, int* integerValueMax) {
	IntegerSplitPtr integerSplitList = INTEGER_SPLITTER_NULL;
	size_t lengthString = strlen(string);
	size_t index;
	int state;
	char digits[MAX_DIGITS_LENGTH + 1];
	int digitsIndex;
	if (integerValueMax != NULL) {
		*integerValueMax = 0;
	}
	for (index = 0, state = INITIAL_STATE_SPLITTER; (index < lengthString) && (state != ERROR_STATE_SPLITTER); index++) {
		int character = string[index];
		switch (state) {
		case INITIAL_STATE_SPLITTER:
			memset(digits, 0, MAX_DIGITS_LENGTH);
			digitsIndex = 0;
			if (isdigit(character)) {
				// store the number, if you can do it.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = ERROR_STATE_SPLITTER;
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		case SEPARATOR_STATE_SPLITTER:
			if (isdigit(character)) {
				// store the number, if you can do it.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = ERROR_STATE_SPLITTER;
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		case DIGIT_STATE_SPLITTER:
			if (isdigit(character)) {
				// store the number, if you can do it.
				state = integerSplitter_AddDigit(&digitsIndex, digits, character);
			}
			else if (!(iswalnum(character) || iswspace(character))) {
				state = integerSplitter_addStoredData(digits, &integerSplitList, character, &digitsIndex, integerValueMax);
			}
			else {
				state = ERROR_STATE_SPLITTER;
			}
			break;
		default:
			state = ERROR_STATE_SPLITTER;
			break;
		}
	}
	if ((state != ERROR_STATE_SPLITTER) && (index == lengthString)) {
		state = integerSplitter_addStoredData(digits, &integerSplitList, LAST_SEPARATOR, &digitsIndex, integerValueMax);
	}
	if (state == ERROR_STATE_SPLITTER) {
		integerSplitter_Init(&integerSplitList);
	}
	return integerSplitList;
}

