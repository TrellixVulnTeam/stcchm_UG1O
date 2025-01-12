/*
 **********************************************************************
 ** MD5.h                                                            **
 **                                                                  **
 ** - Style modified by Tony Ray, January 2001                       **
 **   Added support for randomizing initialization constants         **
 ** - Style modified by Dominik Reichl, September 2002               **
 **   Optimized code                                                 **
 **                                                                  **
 **********************************************************************
 */

/*
 **********************************************************************
 ** MD5.h -- Header file for implementation of MD5                   **
 ** RSA Data Security, Inc. MD5 Message Digest Algorithm             **
 ** Created: 2/17/90 RLR                                             **
 ** Revised: 12/27/90 SRD,AJ,BSK,JT Reference C version              **
 ** Revised (for MD5): RLR 4/27/91                                   **
 **   -- G modified to have y&~z instead of y&z                      **
 **   -- FF, GG, HH modified to add in last register done            **
 **   -- Access pattern: round 2 works mod 5, round 3 works mod 3    **
 **   -- distinct additive constant for each step                    **
 **   -- round 4 added, working mod 7                                **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** License to copy and use this software is granted provided that   **
 ** it is identified as the "RSA Data Security, Inc. MD5 Message     **
 ** Digest Algorithm" in all material mentioning or referencing this **
 ** software or this function.                                       **
 **                                                                  **
 ** License is also granted to make and use derivative works         **
 ** provided that such works are identified as "derived from the RSA **
 ** Data Security, Inc. MD5 Message Digest Algorithm" in all         **
 ** material mentioning or referencing the derived work.             **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#ifndef ___MD5_H___
#define ___MD5_H___

#pragma once
#include "base/base_export.h"

/* Data structure for MD5 (Message Digest) computation */
typedef struct
{
	DWORD	i[2];		// Number of _bits_ handled mod 2^64
	DWORD	buf[4];		// Scratch buffer
	BYTE	in[64];		// Input buffer
	BYTE	digest[16];	// Actual digest after MD5Final call
} MD5_CTX;

static void MD5_Transform (DWORD *buf, DWORD *in);

BASE_EXPORT void MD5Init(MD5_CTX *mdContext, DWORD pseudoRandomNumber = 0);
BASE_EXPORT void MD5Update(MD5_CTX *mdContext, BYTE *inBuf, DWORD inLen);
BASE_EXPORT void MD5Final(MD5_CTX *mdContext);
BASE_EXPORT void MD5Hash(char* str, BYTE hash[16]);
#if defined(SE6)
BASE_EXPORT void MD5Hash(const wchar_t* str, BYTE hash[16]);
#endif

#endif /* ___MD5_H___ included */
