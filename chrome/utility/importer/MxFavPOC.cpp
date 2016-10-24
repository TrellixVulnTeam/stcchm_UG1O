// mx_crack.cpp : Defines the entry point for the console application.
//



/* --
#
# Maxthon Favorite Decrypt POC
#
# Copyright (C) 2012 Liao
#

@author:        Liao
@version:       0.1
@organization:  www.360.cn

�����ղؼ���Կ��guestmaxthon3_favdb_txmood
�����ղؼ���Կ��[�û�����]maxthon3_favdb_txmood

-- */

//
// �����������㷨��֤����, �������ԣ�����ʹ�õ���Ʒ������д�����򣬲������쳣����Ͱ�ȫ���!!!
//
#include <stdlib.h>
#include <malloc.h>
#include <windows.h>
#include <wincrypt.h>
#pragma comment(lib, "Advapi32.lib")

#define MAXTHON_DECRYPT_OK		1
#define MAXTHON_FAILED_FAILED	0
#define CRYPT_OFFSET   			8

typedef struct _MXCRYPTBLOCK 
{
	HCRYPTKEY hMxReadKey;      
	HCRYPTKEY hMxWriteKey;     
	DWORD     dwMxPageSize;      
	LPVOID    pMxCryptData;    		
	DWORD     dwMxCryptDataSize; 
	DWORD     reserved;  

} MXCRYPTBLOCK, *LPMXCRYPTBLOCK;

HCRYPTPROV g_phProv = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static 
LPMXCRYPTBLOCK 
CreateCryptBlock(HCRYPTKEY hKey) 
{
	LPMXCRYPTBLOCK pMxBlock = NULL;

	pMxBlock = (LPMXCRYPTBLOCK)malloc(sizeof(MXCRYPTBLOCK));
	if (pMxBlock != NULL)
	{
		ZeroMemory(pMxBlock, sizeof(MXCRYPTBLOCK));
		pMxBlock->hMxReadKey = hKey;
		pMxBlock->hMxWriteKey = hKey;
		pMxBlock->dwMxPageSize = 0x400;
		pMxBlock->dwMxCryptDataSize = 0x400;

		if (pMxBlock->pMxCryptData) 
		{
			free(pMxBlock->pMxCryptData);
			pMxBlock->pMxCryptData= NULL;
		}

		if (CryptEncrypt(hKey, 0, TRUE, 0, NULL, &pMxBlock->dwMxCryptDataSize, pMxBlock->dwMxCryptDataSize * 2)) 
			pMxBlock->pMxCryptData = malloc(pMxBlock->dwMxCryptDataSize + (CRYPT_OFFSET * 2));
	}

	return pMxBlock;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static 
BOOL 
mxInitializeProvider() 
{
	BOOL bRet = FALSE; 

	if (g_phProv) 
	{
		bRet = TRUE;
	} 
	else 
	{
		if (CryptAcquireContextA(&g_phProv, 0, "Microsoft Base Cryptographic Provider v1.0", PROV_RSA_FULL, 0)
			|| CryptAcquireContextA(&g_phProv, 0, "Microsoft Base Cryptographic Provider v1.0", PROV_RSA_FULL, CRYPT_NEWKEYSET)
			|| (bRet = CryptAcquireContextA(&g_phProv, 0, "Microsoft Base Cryptographic Provider v1.0", PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) != 0 )
			bRet = TRUE;
	}	
	return bRet;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static 
HCRYPTKEY 
mxDeriveKey(
			const BYTE *pbKey, 
			DWORD dwKeyLen
			) 
{
	HCRYPTHASH hHash = 0;
	HCRYPTKEY  hKey  = 0;

	if (!pbKey || !dwKeyLen) {
		return 0;
	}

	if (!mxInitializeProvider()) {
		return -1;
	}

	if (CryptCreateHash(g_phProv, CALG_SHA1, 0, 0, &hHash)) {
		if (CryptHashData(hHash, pbKey, dwKeyLen, 0)) {
			CryptDeriveKey(g_phProv, CALG_RC4, hHash, 0, &hKey);
		}
		CryptDestroyHash(hHash);
	}  
	return hKey;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD 
mxFavoriteDecryptWithKey(
						 char *favorite_file, 
						 char *decrypt_file, 
						 char* mxkey, 
						 DWORD mxkey_len
						 ) 
{
	HANDLE hFavFile = NULL;
	HANDLE hDecryptFile = NULL;
	DWORD fav_size;
	BYTE *page;
	DWORD offset;
	DWORD page_num;
	DWORD bytes;

	HCRYPTKEY hKey;
	HCRYPTHASH hHash = 0;
	DWORD dwDataLen = 0x400;
	DWORD *pdwDataLen = &dwDataLen;
	LPMXCRYPTBLOCK pMxBlock = NULL; 

	DWORD page_size;


	hKey = mxDeriveKey((BYTE *)mxkey,  mxkey_len);
	if (hKey == -1) 
		return MAXTHON_FAILED_FAILED;

	pMxBlock = CreateCryptBlock(hKey);
	if (pMxBlock == NULL)
		return MAXTHON_FAILED_FAILED;

	if (!pMxBlock->hMxReadKey) {
		return  MAXTHON_FAILED_FAILED;
	}
	page_size = pMxBlock->dwMxPageSize;

	hFavFile = CreateFileA(favorite_file,
		FILE_GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if ( hFavFile == INVALID_HANDLE_VALUE ) 
		return MAXTHON_FAILED_FAILED;

	fav_size = GetFileSize(hFavFile, NULL);
	if ( !fav_size ) 
	{
		CloseHandle(hFavFile);
		return MAXTHON_FAILED_FAILED;
	}

	if(fav_size < 0x20) 
	{
		CloseHandle(hFavFile);
		return MAXTHON_FAILED_FAILED;
	}

	page = (BYTE*)malloc(page_size);
	if (!page) 
	{
		CloseHandle(hFavFile);
		return MAXTHON_FAILED_FAILED;
	}

	hDecryptFile = CreateFileA(decrypt_file,
		FILE_GENERIC_WRITE|FILE_GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if ( hDecryptFile == INVALID_HANDLE_VALUE ) 
	{
		CloseHandle(hFavFile);
		return MAXTHON_FAILED_FAILED;
	}
	SetFilePointer(hDecryptFile, 0, NULL, FILE_BEGIN);

	offset = 0;
	page_num = 1;
	while( offset + page_size <= fav_size ) 
	{
		if( !ReadFile(hFavFile, page, page_size, &bytes, NULL) ) 
		{
			CloseHandle(hFavFile);
			CloseHandle(hDecryptFile);
			return MAXTHON_FAILED_FAILED;
		}

		CryptDecrypt(hKey, hHash, TRUE, 0, page, pdwDataLen);


		if ( !WriteFile(hDecryptFile, page, page_size, &bytes, NULL) ) 
		{
			CloseHandle(hFavFile);
			CloseHandle(hDecryptFile);
			return MAXTHON_FAILED_FAILED;
		}
		page_num++;
		offset += page_size;
	}

	free(page);
	CryptDestroyKey(hKey);

	if ( hFavFile ) 
		CloseHandle(hFavFile);

	if ( hDecryptFile )
		CloseHandle(hDecryptFile);

	if (pMxBlock)
		free(pMxBlock);

	return MAXTHON_DECRYPT_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
#ifndef _MAIN_
#define _MAIN_

void main(int argc, char *argv[]) 
{
	// ����
	if (mxFavoriteDecryptWithKey("Favorite.dat", "Favorite.db", "guestmaxthon3_favdb_txmood", 26))
		printf("[+] ���ܳɹ�");
	else
		printf("[-] ����ʧ��");
}

#endif
*/
