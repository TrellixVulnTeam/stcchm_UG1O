//SgDBManager.h
#pragma once
#ifndef __SG_DB_MANAGER__
#define __SG_DB_MANAGER__
#include "local_bookmarks_misc.h"
#include "chrome/common/importer/360se_importer/database/CppSQLite3.h"
#include "chrome/common/importer/360se_importer/util/CodeMisc.h"

class CppSQLite3DB;

struct FileData
{
  std::wstring  strFullPath;    //�ļ�����·��
  FILETIME    ftLastWriteTime;  //�ļ�����޸�ʱ��
};

//
//wrf:�ѹ������ļ�������
//
class CSgDBManager
{
public:
  CSgDBManager();
  ~CSgDBManager();

  BOOL Init( LPCWSTR lpszFile, LPCWSTR lpszFileNameFlag = L"" );
  void UnInit();

  CppSQLite3DB* GetDB(){return m_pDB;};

  //ȡ��sogou���ղ��ļ�����Ŀ¼���ж���˻�ʱ������޸�Ϊ׼
  static std::wstring GetSgFavFilePath();
  static std::wstring GetSgFilePath();

  //�°��ղؼ��״μ���ʱ���������ȫ�°�װ�״����У�������Ƿ�����ѹ��������
  //������ڣ����Խ����ѹ�������ղؼС�������ܳɹ�����������
  void CheckFirstRun();

  //���������Ϊ�ǵ�һ������
  void SetNoFirstRun();

  //�ж��ѹ��ղ��ļ��Ƿ���ȷ
  BOOL IsValidityFav();

protected:
  // �����ݿ�
  BOOL OpenDB( LPCWSTR lpszFile);

  // �ر����ݿ�
  void CloseDB();

  //�����ļ�
  static void SearchFile( LPCWSTR szFindDir, LPCWSTR szFindFileName, std::vector<FileData>& FileList );


private:
  // SQLite3���ݿ��������
  CppSQLite3DB* m_pDB;

  // ��ǰ���ݿ���ļ�ȫ·��
  std::wstring m_strSGDB;

  // ��ʶ���ݵĴ�״̬��TRUEΪ�ɹ��򿪣����Ҿ���У����
  bool  m_bDBOpen;

  std::wstring m_strIniFile;
};

#endif//__SG_DB_MANAGER__