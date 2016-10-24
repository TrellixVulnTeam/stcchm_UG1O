#pragma once
#include <vector>
#include <string>
#include <xstring>

#include "chrome/common/importer/360se_importer/Common.h"
#include "chrome/common/importer/360se_importer/MiscFunc.h"
#include "chrome/utility/importer/360se_importer.h"
#include "third_party/sqlite/sqlite3.h"
#include "sql/connection.h"
using namespace std;
// wanyong 20100331 �ղ����ݿ������

struct FavItem;
struct DBInfo;
class CppSQLite3DB;
//class CFavDBThreadWnd;

struct BookmarkEntryWrapper {
	bool in_toolbar;
	std::wstring url;
	std::vector<std::wstring> path;
	std::wstring title;
	__int64 creation_time;

	BookmarkEntryWrapper() : in_toolbar(false) {}
};
//typedef std::vector<BookmarkEntryWrapper> BookmarkVectorWrapper;



//
// ���ݿ�����࣬�����̣߳�CFavDBThreadWnd
//
class CFavDBManager
{
public:
	//  [7/23/2010 by ken] 

	bool ImportAllItems(vector<FavItem> & vecItems);

	bool ArrangeAllItems(vector<FavItem> const & vecItems,SafeBrowserImporter::BookmarkVector & result);

	std::vector<std::wstring> GenPath(FavItem,vector<FavItem> const &);

	// �����˹������߳�ָ�룬���ʵ���ʱ�������Ҫ����һЩ���ߺ�����������ͬһ�̣߳���˲�������⡣
	CFavDBManager(/*CFavDBThreadWnd * pFavDBThreadWnd*/);
	~CFavDBManager();

	// У�����ݿ��ļ����ԣ�����TRUE˵�����ݿ��ļ�����
	static bool CheckDBFileAttribute(LPCTSTR lpszFile);

	// ������ݿ��ʽ�Ƿ�����
	static bool CheckDBIntegrity(CppSQLite3DB * pDB);
	
	// ������ݵİ汾��
	// bAutoUpgrade: �����ݿ�İ汾�Ƚϵ͵�ʱ���Զ����������ڵ��°汾
	static int CheckDBVersion(CppSQLite3DB * pDB);
	static bool CheckDBVersion(CppSQLite3DB * pDB, bool bAutoUpgrade);

	// ȷ�����ݿ�ͱ�����
	// bForceCreate: �������ڵ�ʱ���Ƿ�ǿ�ƴ���
	static bool IsTablesExist(CppSQLite3DB * pDB, bool bForceCreate); 


	//////////////////////////////////////////////////////////////////////////

	// �л����ݿ�
	int ChangeDBPath(LPCTSTR lpszDBPath);

	// �ϲ����ݿ⣬�������������ݺϲ����ء�
	//		lpszLocalDBPath:	�����ղ����ݿ�·��
	//		lpszServerDBPath;	�������ղ����ݿ�·��
	//bool MergeDB(LPCTSTR lpszLocalDBPath, LPCTSTR lpszServerDBPath);

	// �ж��ļ���/�����Ƿ���ڣ���������򷵻�ID
	int ExistItem(int nParentID, bool bFolder, LPCTSTR lpszTitle);

	// ����ID����Ԫ��
	bool IsExistID(int nId);

	// ����ļ���/����
	int AddFolder(int nParentID, LPCTSTR lpszTitle, int nPos = -1);
	int AddItem(int nParentID, LPCTSTR lpszTitle, LPCTSTR lpszURL, int nPos = -1, bool bIsBest = false);

	// ��ȡָ���ļ����������
	int GetAllItems(int nParentID, vector<FavItem> & vecItems);

	// ���ָ����
	int GetItem(int nID, FavItem * pItem);

	// ����ĳ��Ϊ���ҵ����
	int SetItemBest(int nId,  bool bBest);

	// ɾ��ĳһ��
	int DeleteItem(int nId, bool bFolder);
	int BatchDelete(std::vector<int>* batchItems);

	// �޸�һ��,lpszUrl=NULL��ʾ�޸ĵ�ΪĿ¼
	// *****��Ҫȥ��title��url�еķǷ��ַ���'��
	int ModifyItem(int nId, LPCTSTR lpszTitle, LPCTSTR lpszUrl = NULL);

	// �ƶ�ĳһ�����֮ǰ��Ҫ����CheckItemCollisionȷ����û�г�ͻ
	int MoveItem(int nId, int nTargetId, int nMoveStyle, bool bBatch);

	// �ж�nTargetIdĿ¼�£��Ƿ�����nId��ͬ�����Ŀ¼
	int CheckItemCollision(int nId, int nTargetId);

	// �ж�nDestId�Ƿ�ΪnSrcId�����nSrcId����Ϊһ��Ŀ¼
	bool IsChild(int nSrcId, int nDestId);

	// ����title���򣬲��ݹ飬ֻ��ָ���ļ�������
	// �ļ�����ǰ�棬link�ں��棬����ֱ���title����
	int SortFolderByTitle(int nParentId);

	// ������ӵ�ʱ������
	int SortFolderByCreateTime(int nParentId);

	// ��������֧��Ƕ�ף�������һ��ȫ�ֵ��������
	bool BeginTransaction(bool bExistCommit = FALSE);
	bool RollbackTransaction();
	bool CommitTransaction();
	void EndTransaction();

	// �������ݿ�
	bool BackupDB(LPCTSTR lpszDBFilePath);
	
	// �ָ����ݿ�
	bool RestoreDB(LPCTSTR lpszDBFilePath);

	// ���Favorite������
	int GetFavoriteTotal();

	// ������ݿ����Ϣ
	int GetLocalDBInfo(DBInfo* pInfo);
	int GetServerDBInfo(LPCTSTR lpszDBFilePath, DBInfo* pInfo);

	// �������ݿ⣬��lpszDBFilePath���ݿ⸲�ǵ�����
	// ����֮ǰ������ص���GetLocalDBInfo��GetServerDBInfo�Ի�����ݵ���Ϣ����ȷ�����Ը��Ǻ��ڵ��ñ�����
	bool OverwriteDB(LPCTSTR lpszDBFilePath);

	// ������ݿ��·��
	bool GetDBPath(LPTSTR lpszDBPath, DWORD dwSize);

	// ����DB���ͬ��ʱ��Ϊ��ǰʱ��
	void SetDBLastSynTime();

	inline int GetDBStatus(){return m_nDBStatus;}

	bool GetAllFolders(vector<FavItem> & vecItems);

	bool GetPathFromId(TCHAR* szPaht, int nSize, int nId);
	
	int GetFolderIdFromPath(LPCTSTR lpszFolderPath);

	// ����ղ�����ʾ��Ŀ¼��ID��
	int GetFavShowFolderId();

	// �����ղ�����ʾ��Ŀ¼��ID��
	bool SetFavShowFolderId(int nFolderId);

	int SearchItem(vector<FavItem>& vecItems,LPCTSTR lpszKeyWords);

	// ���´������ݿ⣨������ݿ⣩
	bool ReNewDB();

	// ��ȡ/�������ݿ���Զ������״̬
	int GetAutoLoad(int& nAutoLoad);
	int SetAutoLoad(int nAutoLoad);
protected:

	// ��ʼ�����ݿ⣺��������ļ��Ƿ���ڣ����ݿ��ʽ�Ƿ����������Ƿ���ڡ�����
	bool InitDB();
	
	void UninitDB();

	// �Զ��ظ����ݿ�
	bool RestoreDBAuto();

	// �ճ����ݣ�ֻ���ڱ��ݱ������ݿ⣬���������Ѿ����ݵ��������ϣ����豸��
	bool DailyBackup();


	
	//////////////////////////////////////////////////////////////////////////

	// ����ļ���/����
	// *****��Ҫȥ��title��url�еķǷ��ַ���'��
	int AddItem(int nParentID, bool bFolder, LPCTSTR lpszTitle, LPCTSTR lpszURL, int nPos = -1, bool bIsBest = false);

	// ɾ����/Ŀ¼
	int DeleteItem(int nId, int nParentId, int nPos, bool bMoveNexts = TRUE);
	int DeleteFolder(int nId, int nParentId, int nPos, bool bMoveNexts = TRUE);

	int DeleteItemNotify(int nId, int nParentId, int nPos, bool bMoveNexts = TRUE);
	int DeleteFolderNotify(int nId, int nParentId, int nPos, bool bMoveNexts = TRUE);

	// ������ĳ�ֶ������ļ��У����ݹ�
	int SortFolderByField(int nParentId, LPCSTR lpFildeName, bool bAsc = TRUE);

	// Move��û�г�ͻʱ���ƶ���/Ŀ¼���µ�Ŀ¼��
	// ��ҪMoveOperStruct���ֶΣ�nSrcID,nSrcParentID,nSrcPos,  nDestParentID,nDestPos
	// ����1����nSrcParentIDĿ¼��λ�ô���nSrcPos����������
	// ����2����nDestParentIDĿ¼��λ��С��nDestPos����������
	// ����3������nSrcID�ĸ��ڵ�(��ΪnDestParentID)��λ��(��ΪnDestPos)��ʱ��
	int MoveItem2Folder(MoveOperStruct* pMS, bool bMovePrevs, bool bMoveNexts);

	// Merge���г�ͻʱ���ϲ���/Ŀ¼���µ�Ŀ¼��
	// ��ҪMoveOperStruct���ֶΣ�nSrcID,nSrcParentID,nSrcPos,  nDestParentID,nDestPos,  nCollisionID,nCollisionParentID,nCollisionPos
	// ����1������MoveItem2Folder���ƶ�nSrcID��nDestParentID�е�nDestPosλ��
	// ����2��ɾ����ͻ���nCollisionID��
	int MergeItem2Folder(MoveOperStruct* pMS, bool bMovePrevs);
	// ��ҪMoveOperStruct���ֶΣ�nSrcID,nSrcParentID,nSrcPos,  nDestParentID,nDestPos,  nCollisionID,nCollisionParentID,nCollisionPos
	// ����1����ѯnSrcIdĿ¼����nCollisionId��ͻ����/Ŀ¼
	// ����2������MergeItem2Folder�͵ݹ����MergeFolder2Folder���ϲ���Щ���Ŀ¼
	// ����3����ѯnSrcIdĿ¼����nCollisionId����ͻ����/Ŀ¼
	// ����4������MoveItem2Folder���ƶ���Щ��/Ŀ¼
	// ����5��ɾ���Լ�
	// ����6������MoveItem2Folder����nCollisionID���ƶ���nDestPosλ��
	int MergeFolder2Folder(MoveOperStruct* pMS, bool bMovePrevs);

	int GetDBInfo(CppSQLite3DB * pDB, DBInfo* pInfo);

	// �����Ҫ���ݵ�Ŀ���ļ���
	CString GetDailyBackupFilePath();

	// ɾ�����ڵı����ļ���ֻ����7������
	void DeleteOldBackupFile();

	// ������б����ļ����������ļ�����С��������
	void GetBackupFiles(std::vector<CString>& listFiles);

	// ���ĳ��Ŀ¼�����е�Ŀ¼
	void GetAllChildrenFolder(std::vector<int>& vecChildren, int nParnet);

	// ��ʽ��Ϊ utf8 �ַ���
	string GetUTF8String(LPCTSTR lpszString);

	bool MoveErrorDBFile(LPCTSTR lpszDBFile);

	

private:

	// ���ݿ��̵߳Ĵ���ָ�룬��������ת�����ݿ�֪ͨ
	//CFavDBThreadWnd * m_pFavDBThreadWnd;

	// SQLite3���ݿ��������
	CppSQLite3DB * m_pDB;

	// ��ǰ���ݿ���ļ�ȫ·��
	CString m_strDBPath;

	// ��ʶ�Ƿ���û���ύ������
	bool   m_bOpenTransaction;

	// ��ʶ���ݵĴ�״̬��TRUEΪ�ɹ��򿪣����Ҿ���У����
	bool  m_bDBOpen;

	// ��ʶ���ݿ��Ƿ��иı�
	bool m_bDBChanged;

	// ��־��ǰ���ݿ��״̬
	int	m_nDBStatus;
};


class SafeBrowserDBImporter{

public :
	SafeBrowserDBImporter(){}
	~SafeBrowserDBImporter(){}

	static sql::Connection* OpenAndCheck(std::wstring const & filename);
	static bool ImportData(sql::Connection* sqlite,vector<FavItem> & vecItems);
	static bool ArrangeAllItems(vector<FavItem> const & vecItems,SafeBrowserImporter::BookmarkVector & result);
	static std::vector<std::wstring> GenPath(FavItem item,vector<FavItem> const & vecItems);
};