//MaxthonDBManager.h        2012.7 by ������
#ifndef __MX_DB_MANAGER__
#define __MX_DB_MANAGER__

#include "chrome/utility/importer/SgDBManager.h"

class CMaxthonDBManager
{
public:
	CMaxthonDBManager();
	~CMaxthonDBManager();
	
	// lpszFile:��Ŀ¼��lpszFile:�ļ�
	BOOL Init( LPCWSTR lpszFilePath, LPCWSTR lpszFile, LPCWSTR lpszFileNameFlag = L"" );
	void UnInit();

	CppSQLite3DB* GetDB(){return m_pDB;};

	//ȡ��Maxthon���ղ��ļ�����Ŀ¼���ж���˻�ʱ������޸�Ϊ׼
	std::wstring GetMaxthonFavFilePath();

protected:
  // ������ݿ��ʽ�Ƿ�����
  static bool CheckDBIntegrity(CppSQLite3DB * pDB);

  // �����ݿ�
	BOOL OpenDB( LPCWSTR lpszFile);

	// �ر����ݿ�
	void CloseDB();

	//�����ļ�
	void SearchFile( LPCWSTR szFindDir, LPCWSTR szFindFileName, std::vector<FileNameAndTime>& FileList );


private:
	// SQLite3���ݿ��������
	CppSQLite3DB* m_pDB;

	// ��ǰ���ݿ���ļ�ȫ·��
	std::wstring m_strMaxthonDB;

	// ��ʶ���ݵĴ�״̬��TRUEΪ�ɹ��򿪣����Ҿ���У����
	bool  m_bDBOpen;

	std::wstring m_strIniFile;
};

#endif//__MX_DB_MANAGER__