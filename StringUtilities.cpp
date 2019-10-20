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
// Convert the wchar_t string to a char* string.
//
wchar_t* char2wchar_t(char* original) {
	// newsize describes the length of the
	 // wchar_t string called wcstring in terms of the number
	 // of wide characters, not the number of bytes.
	size_t newSize = strlen(original) + 1;
	// The following creates a buffer large enough to contain
	// the exact number of characters in the original string
	// in the new format. If you want to add more characters
	// to the end of the string, increase the value of newsize
	// to increase the size of the buffer.
	wchar_t* wcstring = new wchar_t[newSize];
	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newSize, original, _TRUNCATE);
	return wcstring;
}

//
// Convert the wchar_t string to a char* string.
//
char* wchar_t2char(wchar_t* original) {
	// Record the length of the original string and add 1 to it to
	// account for the terminating null character.
	size_t originalSize = wcslen(original) + 1;
	size_t convertedChars = 0;
	// Allocate two bytes in the multibyte output string for every wide
	// character in the input string (including a wide character
	// null). Because a multibyte character can be one or two bytes,
	// you should allot two bytes for each character. Having extra
	// space for the new string is not an error, but having
	// insufficient space is a potential security problem.
	const size_t newSize = originalSize * 2;
	// The new string will contain a converted copy of the original
	// string.
	char* newString = new char[newSize];
	// Put a copy of the converted string into nstring
	wcstombs_s(&convertedChars, newString, newSize, original, _TRUNCATE);
	return newString;
}
