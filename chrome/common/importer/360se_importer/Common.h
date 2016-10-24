#pragma once
//
// �ղ����ݽṹ
//
struct FavItem
{
	int nID;					// id
	int nParentID;				// �����ļ��е�id
	bool bFolder;				// �Ƿ����ļ���
	bool bBest;					// �Ƿ��ҵ��
	int  nPos;					// �ղؼе�λ��
	std::wstring strTitle;		// ����
	std::wstring strURL;		// ����
	__int64 lCreateTime;		// ����ʱ��
	__int64 lLastModifyTime;	// �ϴ��޸�ʱ��
//	bool bDeleted;				// �Ƿ�ɾ��
	int nReserved;				// �����ֶ�
};

struct FavItemA
{
	int nID;					// id
	int nParentID;				// �����ļ��е�id
	bool bFolder;				// �Ƿ����ļ���
	bool bBest;					// �Ƿ��ҵ��
	int  nPos;					// �ղؼе�λ��
	std::string strTitle;		// ����
	std::string strURL;		// ����
	__int64 lCreateTime;		// ����ʱ��
	__int64 lLastModifyTime;	// �ϴ��޸�ʱ��
//	bool bDeleted;				// �Ƿ�ɾ��
	int nReserved;				// �����ֶ�
};

// �ƶ�֪ͨ
struct MoveItemNotifyParam
{
	int nID;
	int nPreParent;
	int nNowParent;
};

// �����ƶ����Ŀ¼,ʹ�ýṹ���¼�õ���˵�����ݣ���ö�β�ѯ����
struct MoveOperStruct
{
	// �ƶ���ԭ����
	int nSrcID;	
	int nSrcParentID;
	int nSrcPos;
	bool bSrcFolder;

	//�ƶ���Ŀ������
	int nDestID;	
	int nDestParentID;
	int nDestPos;
	bool bDestFolder;

	// �ƶ�ʱ�г�ͻ���������
	int nCollisionID;	
	int nCollisionParentID;
	int nCollisionPos;
};

// ����title������
struct DBTitleStor
{
	int nId;
	bool bFolder;
	std::wstring strTile;
	bool operator < (DBTitleStor s)
    {
		if(bFolder && !s.bFolder)
			return true;
		else if(!bFolder && s.bFolder)
			return false;

		// �޸������е��ַ����Ƚ��㷨
		return ::CompareString(MAKELCID(MAKELANGID(LANG_CHINESE,SUBLANG_CHINESE_SIMPLIFIED),SORT_CHINESE_PRCP), 
			0,//NORM_IGNORECASE | NORM_IGNOREKANATYPE | NORM_IGNORENONSPACE | NORM_IGNORESYMBOLS | NORM_IGNOREWIDTH | SORT_STRINGSORT | NORM_LINGUISTIC_CASING, 
			strTile.c_str(), -1, s.strTile.c_str(), -1)==CSTR_LESS_THAN ? true : false;
    }
};

// ���ݿ�Ļ�����Ϣ
struct DBInfo
{
	bool bComplete;			// ���ݿ��Ƿ�����
	char chVersion[128];	// ���ݿ�İ汾��
	int nVersion;			// ���ݿ�İ汾�ţ�<0��ʾ�ͣ�0��ʾ���ʣ�>0��ʾ�ߣ�
	int nFavCount;			  // �ղص���������
	__int64 lnSynTime;		 // �ϴ�ͬ��ʱ��
};

//////////////////////////////////////////////////////////////////////////

// tb_misc���е�key
#define FAVDB_SYN_TIME_KEY			"db_last_syn_time"
#define FAVDB_VERSION_KEY			"db_version"

// �������ݿ�İ汾��
#define FAVDB_VERSION_VALUE			"1"

///////////////////////////////////////////////////////////////////////////

#define ROOT_ITEM_ID            0
#define ROOT_ITEM_NET			0xFFFFFFEF
#define FAVBAR_SIDEBARID		0xFFFFFFEE

// �������ϲ˵���ID
#define ROOT_TOOLBAR_MENUID		2147483647

// ���Ӱ�ť��ID �����������ֵ
#define TOOLBAR_ADDBUTTON_ID	2147483646

// �ղؼе��ƶ���ʽ
#define MOVEITEM_COLLISION		1					// �Ƿ��ͻ
#define MOVEITEM_UP				2					// �ƶ���Ŀ��ID������
#define	MOVEITEM_DOWN			4					// �ƶ���Ŀ��ID������
#define MOVEITEM_IN				8					// �ƶ���Ŀ��ID������

// ���ݿ��������ֵ
#define FAV_DB_SUCCEEDED			0				// succ
#define FAV_DB_SQLITE_ERROR			-1				// database connet error,�ļ���ռ��
#define FAV_DB_PARAM_ERROR			-2				// param error
#define FAV_DB_ITEM_EXISTED			-3				// folder/link existed
#define FAV_DB_ITEM_NOT_EXISTED		-4				// folder/link not existed
#define FAV_DB_VERSION_HIGHER		-5				// folder/link not existed
#define FAV_DB_VERSION_LOWER		-6				// folder/link not existed
#define FAV_DB_DESTROYED			-7				// folder/link not existed

// ���ݿ���Ӧ����Ĳ���������
#define FAV_DB_ADD_ERROR			0				// ����һ���������
#define FAV_DB_DELETE_ERROR			-1				// ɾ��һ�����
#define FAV_DB_MODIFY_ERROR			-2				// �޸�һ���������
#define FAV_DB_BEST_CHANGE_ERROR	-3				// �ҵ��״̬�������
#define FAV_DB_MOVE_ERROR			-4				// �ƶ���������
#define FAV_DB_RESTORE_ERROR		-5				// �ָ����ݳ���
#define FAV_DB_BACKUP_ERROR			-6				// �������ݳ���
#define FAV_DB_RESTORE_SUCCESS		1				// �ָ����ݳɹ�
#define FAV_DB_BACKUP_SUCCESS		2				// �������ݳɹ�	
#define FAV_DB_AUTO_RESTORE_FAILD		3			// �Զ��ָ�ʧ��
#define FAV_DB_AUTO_RESTORE_SUCCEEDED	4			// �Զ��ָ��ɹ�

// ��ʶ��ǰ���ݿ��״̬
#define	 DB_STATUS_CACHE		0		// ���ػ���
#define  DB_STATUS_NEW			1		// �´���
#define  DB_STATUS_BACKUP		2		// �Զ��ָ�
//////////////////////////////////////////////////////////////////////////

//
// FavDBThreadWnd --> FaveriteControlWnd
// ���ݿ�����߳� --> ���߳�
//

// ֪ͨ���߳�������һ��
// wparam �������ID
#define WM_FAVCONTROL_ADD_ITEM			(WM_USER + 0x1001)				

// ֪ͨ���߳�ɾ����һ��
// wparam ɾ�����ID
#define WM_FAVCONTROL_DELETE_ITEM		(WM_USER + 0x1002)

// ֪ͨ���߳�һ�����Ѿ�������
// wparam ���������ID
#define WM_FAVCONTROL_RENAME_ITEM		(WM_USER + 0x1003)

// ֪ͨ���̵߳�ĳ���ҵ��״̬������
// wparam best״̬�������ĵ�ID
#define WM_FAVCONTROL_BESTCHANGE_ITEM	(WM_USER + 0x1004)

// ֪ͨ���߳�ĳ���Ѿ��ƶ�
// wparam �ƶ���idֵ�� lparam = makelparam��prevParentID�� ParentID��
#define WM_FAVCONTROL_MOVE_ITEM			(WM_USER + 0x1005)

// ֪ͨ���߳̽������¼����ղؼ�
// wparam,lparam û��ʹ��
#define WM_FAVCONTROL_RELOAD_DB			(WM_USER + 0x1006)

// ֪ͨ���߳̽������¼���һ���ļ���
// wparam Folder��ID�� lparamδʹ��
#define WM_FAVCONTROL_UPDATE_FOLDER		(WM_USER + 0x1007)

// ֪ͨ���߳̽�����Ӧ�Ĳ�������
// wparam ������ lparam δʹ��
#define WM_FAVCONTROL_OPERATE_ERROR		(WM_USER + 0x1008)

//
// FavDBThreadWnd --> FavSyncWnd
// ���ݿ�����߳� --> ͬ���߳�
//

//  ֪ͨͬ���̣߳��ղظı�
//  wParam��lParam��δʹ��
#define WM_FAVSYNC_FAVCHANGE			(WM_USER + 0x1009)

//
// ���� --> ����
//

//  ֪ͨ�ղؼ�����ֶ��������
//  WPARAM��title
//  LPARAM��text
#define WM_FAVPANE_DOWN           (WM_USER + 0x1010)

//  ֪ͨ�ղؼ�����ֶ��ϴ����
//  WPARAM��title
//  LPARAM��text
#define WM_FAVPANE_UPLOAD         (WM_USER + 0x1011)
//////////////////////////////////////////////////////////////////////////

//
// һЩ�����Ķ���
//

#define FAV_MENU_TITLE_ADDITEM				_T("��ӵ��ղؼ�...\tCtrl+D")
#define FAV_MENU_TITLE_ADDITEM_ALL			_T("�������ҳ�浽�ղؼ�...")
#define FAV_MENU_TITLE_CLEAN				_T("�����ղؼ�...")
#define FAV_MENU_TITLE_MORE					_T("���๦��")
#define FAV_MENU_TITLE_SETTING				_T("�ղ�������")
#define FAV_MENU_TITLE_BACKUP				_T("�����ղؼ�...")
#define FAV_MENU_TITLE_RESTORE				_T("��ԭ�ղؼ�...")
#define FAV_MENU_TITLE_IEIMPORT				_T("��IE�ղؼе���...")
#define FAV_MENU_TITLE_FILEIMPORT			_T("���ļ�����...")
#define FAV_MENU_TITLE_IMPORT				_T("�����ղؼ�")
#define FAV_MENU_TITLE_EXPORT				_T("�����ղؼ�...")
#define FAV_MENU_TITLE_ADD_TO_FOLDER		_T("��ӵ����ļ���...")
#define FAV_MENU_TITLE_SYNA_FAV				_T("ͬ���ղ�")
#define FAV_MENU_TITLE_UPFAV				_T("�ֶ��ϴ������ղؼ�")
#define FAV_MENU_TITLE_DOWNFAV				_T("�ֶ����������ղؼ�")

#define FAV_SYNC_ERR_NET				    _T("���ӳ�ʱ�������ղؼи���ʧ�ܡ����飺\r\n1. ��������; 2. ��������; 3. ����ǽ����")
//#define FAV_SYNC_SVR_ERR                    _T("����������ά�������������޸���ʱ�޷���\r\n�����������������ղؽ��Զ��ָ���")
#define FAV_SYNC_DBVER_ERR					_T("������汾���ͣ����������°汾�������")
#define FAV_SYNC_WORKING                    _T("���ڸ��������ղؼ�...")
#define FAV_SYNC_ERR_NETBUSY                _T("���緱æ����ʱ�޷��������������ղؼУ�\r\n�Ժ��Զ����ԡ�")
#define FAV_SYNC_ERR_MD5                    _T("�����жϣ������ղؼи���ʧ�ܡ�\r\n�Ժ��Զ����ԡ�")

#define FAV_PANE_TITLE_USER					_T("�������ղؼ�")
#define FAV_PANE_TITLE_NO_USER				_T("�����ղؼ�")
#define FAV_PANE_USERNAME					_T("�ʺ�:")
#define FAV_PANE_FAVCOUNT					_T("�ղ�:")
#define FAV_PANE_LAST_SYNC					_T("�ϴθ��������")

#define DEF_ONLINEFAV_UNLOGIN_TIP			_T("�����ʻ�δ��¼\r\n��¼��ʹ�������ղؼ�")
#define DEF_ONLINEFAV_LOGIN_TIP				_T("�����ʻ��ѵ�¼\r\n����ʹ�������ղؼ�")

// ��Ʒ���������ղ����Ŀռ�
// #define FAV_BAR_ADDBTN_TITLE				_T("����ղ�|")
// #define FAV_BAR_NETBTN_TITLE				_T("�����ղؼ�|")
#define FAV_BAR_ADDBTN_TITLE				_T("���|")
#define FAV_BAR_NETBTN_TITLE				_T("��¼|")

#define FAV_UP_DOWN_NET_WAIT				_T("���Ժ�")
#define	FAV_UP_DOWN_NET_OK					_T("ȷ��")

#define FAV_IMPORT_FOLDERNAME				_T("%d-%02d-%02d ����")
#define FAV_IMPORT_FOLDERNAME_EX			_T("%d-%02d-%02d ����(%d)")

#define FAV_IMPORT_FROM_IE					_T("��IE����")

#define FAV_NEW_FOLDER						_T("�½��ļ���")

//
//  ͳ���ֶ�
//
enum {
    COUNT_LOGINDLG = 0,     //  ��¼��չ��
    COUNT_LOGINDLG_CANCEL,  //  ��¼��ȡ��
    COUNT_LOGOUT,           //  �˳���¼
    COUNT_MULT_ACCOUNT,     //  �ж���˻�
    COUNT_FAV_SHOWTYPE,     //  �ղ�չ��	0: ��ʾ�ղ��������ز����  1:��ʾ�ղ��������ز����
                            //  2: �����ղ�������ʾ�����  3:��ʾ�ղ�������ʾ�����
    COUNT_FAV_COUNT,        //  �ղ�����
    COUNT_FAV_FAVMGR,       //  �����ղ�
    COUNT_FAV_DESTROY,      //  �ղ���
    COUNT_FAV_GUIDE_REG,       
};

enum {
    SUM_FAV_OPEN_MENU = 0,  //  �Ӳ˵������ղصĴ���
    SUM_FAV_OPEN_BAR,       //  ���ղ��������ղصĴ���
    SUM_FAV_OPEN_TREE,      //  �Ӳ�����ղصĴ���
    SUM_FAV_ADD_MENU,       //  �Ӳ˵������
    SUM_FAV_ADD_BAR,        //  ���ղ������
    SUM_FAV_ADD_TREE,       //  �Ӳ�������
    SUM_FAV_ADD_SHORTCUT,   //  �ÿ�ݼ����
    SUM_FAV_ADD_FOLDER,     //  ��ӵ��ļ���
    SUM_FAV_PANE_SHOW,      //  �ղ����չʾ
    SUM_FAV_FAVMGR_SHOW,    //  �����ղ�չ��
};

// �ղ�����ť�Ŀ�ȣ��Ͳ˵������
#define FAVBAR_BUTTON_DEFAULT_WIDTH 120
#define FAVMENU_DEFALT_WIDTH		120

// ID����Ч��
#define FAVID_VALID_NOT				0	// ID������
#define FAVID_VALID_ROOT			1	// ID�Ǹ�Ŀ¼
#define FAVID_VALID_EXIST			2	// ���ڸ�ID���ղؼ���




#define FAV_DB_NAME		_T("360sefav.db")		// ���ݿ�����