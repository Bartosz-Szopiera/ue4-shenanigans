// 2020 Purple Pill Productions.

#pragma once

#include "StaticData.h"

/**
*
 * Data formating example:
 *
 * 0,;0,prop1Name,1347,;3,prop2Name,isArray,textValue1,textValue2,;\n
 *
 * ; - properties delimiter
 * , - delimiter of tokens within property
 *
 * 0 - first zero - it's numerical value of type according to Types
 *
 * ;0,prop1Name,1347,; - this is first property
 *		0			- numerical value of type of value held according to ValueTypes
 *		prop1Name	- property name
 *		1347		- value
 *
 * ;3,prop2Name,,textValue1,textValue2,; - second property which is an array
 *		3			- numerical code for the value type
 *		prop2Name	- property name
 *		isArr		- this value is not used for anything but gives one more chunk that allows
 *						to identify series of chunks as an encoding of array-holding prop
 *		textValue1	- first of series of subsequent values the array is holding
 */

/**
 * Initializing static data
 */
FStaticData FSDStore::StaticData;

FString FSDStore::CurrentInstanceString;

FString FSDStore::StaticDataFString;
