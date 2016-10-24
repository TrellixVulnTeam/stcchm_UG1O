#include "chrome/utility/importer/MaxthonDBManager.h"

#include "chrome/utility/importer/MxFavPOC.h"
#include <atlstr.h>


// TODO:�����ղؿ���
//#define MAXTHON_GUEST
// �����ղ��ļ���
#define FILE_MAXTHON_FAVORITE_DAT					L"Favorite.dat"

CMaxthonDBManager::CMaxthonDBManager() : m_pDB(NULL)
{
	TCHAR szAppDataPath[MAX_PATH] = {0};
	m_bDBOpen = FALSE;
}

CMaxthonDBManager::~CMaxthonDBManager()
{
	CloseDB();
}

//��ʼ��������lpszFileΪ�����ղ��ļ�Ŀ¼ȫ·��
BOOL CMaxthonDBManager::Init( LPCWSTR lpszFilePath, LPCWSTR lpszFile, LPCWSTR lpszFileNameFlag )
{
	if ( !lpszFilePath || !lpszFilePath[0] || !lpszFile || !lpszFile[0] || !lpszFileNameFlag )
	{
		return FALSE;
	}
	//��ȡTMP·��
	wchar_t szTmpPath[MAX_PATH+1] = {0};
	DWORD dwRetVal = GetTempPath( MAX_PATH, szTmpPath);
	if (dwRetVal > MAX_PATH || (dwRetVal == 0))
	{
		return FALSE;
	}

	std::wstring strMXFavFile = lpszFile;

	std::wstring strMXDat = SystemCommon::StringHelper::Format( L"%s%stmp.dat", szTmpPath, lpszFileNameFlag );//�������ݿ��ļ��Ŀ���
	SystemCommon::FilePathHelper::DeepCopyFile( strMXFavFile.c_str(), strMXDat );

	m_strMaxthonDB = SystemCommon::StringHelper::Format( L"%s%stmp.db", szTmpPath, lpszFileNameFlag );//���ܺ�İ������ݿ��ļ�

#ifdef MAXTHON_GUEST
	if (!mxFavoriteDecryptWithKey((char*)SystemCommon::StringHelper::Wstring2String(strMXDat.c_str()).c_str(),
		(char*)SystemCommon::StringHelper::Wstring2String(m_strMaxthonDB.c_str()).c_str(),
		"guestmaxthon3_favdb_txmood", 
		26))
	{
		return FALSE;
	}
#endif 
	ATL::CString strPath = lpszFile;
	int nPos = strPath.Find(L"Maxthon3\\Users");
	strPath = strPath.Right(strPath.GetLength() - nPos - 15);
	strPath = strPath.Left(strPath.Find(L"\\"));
  std::string strKey = CT2A(strPath).m_psz;
	strKey += "maxthon3_favdb_txmood"; 
	if (!mxFavoriteDecryptWithKey((char*)SystemCommon::StringHelper::Wstring2String(strMXDat.c_str()).c_str(),
		(char*)SystemCommon::StringHelper::Wstring2String(m_strMaxthonDB.c_str()).c_str(),
		(char*)strKey.c_str(), 
		strKey.length()))
	{
		return FALSE;
	}

	DeleteFile( strMXDat.c_str() );
	return OpenDB( m_strMaxthonDB.c_str() );
}

void CMaxthonDBManager::UnInit()
{
	CloseDB();
	DeleteFile( m_strMaxthonDB.c_str() );
}

bool CMaxthonDBManager::CheckDBIntegrity(CppSQLite3DB * pDB)
{
  bool bRet = false;
  CppSQLite3Query query;
  if (pDB->execQuery(query, "pragma integrity_check(1);") && 0 == query.eof())
  {
    const char* pok = query.fieldValue(0);
    bRet = (0 == _strnicmp(pok, "ok", 2) ? true : false);
  }
  query.finalize();
  return bRet;
}

BOOL CMaxthonDBManager::OpenDB( LPCWSTR lpszFile)
{
	if (NULL == lpszFile || 0 == lpszFile[0])
		return FALSE;

	BOOL bOpend = FALSE;
	if (NULL == m_pDB)
		m_pDB = new CppSQLite3DB();

	std::string strFile = UTF8Encode(lpszFile);
	if (m_pDB && m_pDB->open(strFile.c_str()))
	{
    if(CheckDBIntegrity(m_pDB))
		  bOpend = TRUE;
	}
	return bOpend;
}

void CMaxthonDBManager::CloseDB()
{
	if (m_pDB)
	{
		m_pDB->close();
		delete m_pDB;
		m_pDB = NULL;
	}
}

//ȡ��Maxthon���ղ��ļ�����Ŀ¼���ж���˻�ʱ������޸�Ϊ׼
std::wstring CMaxthonDBManager::GetMaxthonFavFilePath() 
{
	//ȡ�ð��ε��û�����Ŀ¼
	wchar_t szFindPath[MAX_PATH] = {0} ;
	SHGetFolderPath( NULL, CSIDL_APPDATA, NULL, 0, szFindPath ); 
	wcscat( szFindPath, L"\\Maxthon3\\Users"); 
	std::vector<FileNameAndTime> FileList;
	this->SearchFile( szFindPath, FILE_MAXTHON_FAVORITE_DAT, FileList );

	std::wstring strMxFileName = L"";

	if ( !FileList.empty() )
	{
		FILETIME StandardFileTime = FileList[0].ftLastWriteTime;
		strMxFileName = FileList[0].strFullPath;
		for (unsigned int i=1; i< FileList.size(); i++)
		{
			if( CompareFileTime( &StandardFileTime,  &(FileList[i].ftLastWriteTime) ) == -1 )
			{
				StandardFileTime = FileList[i].ftLastWriteTime;
				strMxFileName = FileList[i].strFullPath;
			}
		}		
	}
	return strMxFileName;
}

//�����ļ�
void CMaxthonDBManager::SearchFile( LPCWSTR szFindDir, LPCWSTR szFindFileName, std::vector<FileNameAndTime>& FileList )
{
  SystemCommon::FilePathHelper::SearchFile( szFindDir , szFindFileName , FileList );
}
