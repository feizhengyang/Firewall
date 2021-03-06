#pragma once
#include "clrlistctrl.h"

enum
{
    AnyAddr=0,
    UniqueAddr,
    RangeAddr,
    UnknownAddr
};

enum
{
    RulesDirectionAny=0,
    RulesDirectionUp,
    RulesDirectionDown,
    RulesDirectionUnknown
};

enum
{
    RulesProtocolAny = 0
};

typedef struct _IP_RULES_ELEM
{
    LIST_ENTRY  list;          //应用层未用此成员

    UINT32   crcRuleName;   //IP规则名称的32位crc值（对应注册表中相应的项）
    
    union{
        UINT32   u32;
        struct
        {
            UINT32  RemoteAddrType :2;  //取值为AnyAddr,UniqueAddr,RangeAddr
            UINT32  LocalAddrType :2;   //取值为AnyAddr,UniqueAddr,RangeAddr
            UINT32  RemotePortType :2;  //取值为AnyAddr,UniqueAddr,RangeAddr
            UINT32  LocalPortType :2;   //取值为AnyAddr,UniqueAddr,RangeAddr
            UINT32  ProtocolType :8;//网络协议类型
            UINT32  Direction :2;//00：任意方向 01：上行  10:下行  11：未定义
            UINT32  Access  :1;//是否允许访问，1为允许
            UINT32  IcmpType :5;
            UINT32  IcmpCode :5;
            UINT32  Reserved :3;
        }Bits;
    }rule;

    UINT32   LocalAddr;
    UINT32   LocalAddr2;
    UINT32   RemoteAddr;
    UINT32   RemoteAddr2;
    UINT16   LocalPort;
    UINT16   LocalPort2;
    UINT16   RemotePort;
    UINT16   RemotePort2;

}IP_RULES_ELEM,*PIP_RULES_ELEM;
// CIpSettingDlg 对话框

class CIpSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CIpSettingDlg)

public:
	CIpSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIpSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IP_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    //重写
    BOOL OnInitDialog();

    //自定义成员函数及变量
    void UpdateRuleList();
    bool RegAddIpRule( PIP_RULES_ELEM rule,LPTSTR ruleName );
    bool RegDeleteIpRule( DWORD crcRuleName );

    //消息映射
	DECLARE_MESSAGE_MAP()
public:
    CClrListCtrl m_RuleList;
    afx_msg void OnLvnItemActivateRuleList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedButtonAddrule();
    afx_msg void OnBnClickedButtonEditrule();
    afx_msg void OnBnClickedButtonDelrule();
    afx_msg void OnBnClickedRadioAllow();
    afx_msg void OnBnClickedOk();
    afx_msg void OnNMClickRuleList(NMHDR *pNMHDR, LRESULT *pResult);
};
                                                                                                                                                                                 