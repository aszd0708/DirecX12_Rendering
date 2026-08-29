#pragma once

using INT8 = __int8;
using INT16 = __int16;
using INT = __int32;
using INT64 = __int64;
using UINT8 = unsigned __int8;
using UINT16 = unsigned __int16;
using UINT = unsigned __int32;
using UINT64 = unsigned __int64;

using BYTE = char;

enum eBlockSize : UINT16
{
	BYTE_64 = 64,
	BYTE_128 = 128,
	BYTE_256 = 256,
};