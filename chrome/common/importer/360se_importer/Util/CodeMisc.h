#pragma once

// wanyong 20100401 �ṩһЩ�����ĺ���

#ifdef _UNICODE
#define UTF8Encode Util::CodeMisc::UTF8EncodeW
#define UTF8Decode Util::CodeMisc::UTF8DecodeW
#else
#define UTF8Encode Util::CodeMisc::UTF8EncodeA
#define UTF8Decode Util::CodeMisc::UTF8DecodeA
#endif


namespace Util {
namespace CodeMisc {

/*
void UnicodeToUtf8(char * utf8);
CString UTF8toUnicode(char *s);
*/

// unicodeת����utf8
std::string UTF8EncodeW(LPCWSTR lpszUnicodeSrc);
// ansiת����utf8
std::string UTF8EncodeA(LPCSTR lpszUnicodeSrc);
// utf8ת����unicode
std::wstring UTF8DecodeW(LPCSTR lpszUtf8Src);
// utf8ת����ansi
std::string UTF8DecodeA(LPCSTR lpszUtf8Src);

BOOL GetPEFileVersion( LPCTSTR lpszFilePath, LPTSTR pszVersion, DWORD dwSize);

//  ��չ���ͳ��
void ClickStat();

LPCTSTR GetSEMidString();
} // end namespace Base
} // end namespace Util
