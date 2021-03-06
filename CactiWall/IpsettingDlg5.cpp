PPROTO_ICMP;
    else if( s == _T("IGMP"))
        dlg.m_ProtocolType = IPPROTO_IGMP;
    else if( s == _T("RDP"))
        dlg.m_ProtocolType = IPPROTO_RDP;
    else if( s == _T("AH"))
        dlg.m_ProtocolType = IPPROTO_AH;
    else
    {
        _stscanf_s( s.GetBuffer(),_T("%d"),&a1 );
        dlg.m_ProtocolType = (BYTE)a1;
    }
    
    if( dlg.m_ProtocolType == IPPROTO_TCP ||
        dlg.m_ProtocolType == IPPROTO_UDP )
    {
        //源端口
        s = m_RuleList.GetItemText( m_RuleList.GetSelectionMark(),6 );
        if( s == _T("--"))
            dlg.m_LocalPortType = 0;
        else if( _tcsstr( s.GetBuffer(),_T("-")) == NULL )
        {
            dlg.m_LocalPortType = 1;
            _stscanf_s( s.GetBuffer(),_T("%d"),&a1 );
            dlg.m_LocalPort1 = a1;
        }
        else
        {
            dlg.m_LocalPortType = 2;
            _stscanf_s( s.GetBuffer(),_T("%d-%d"),&a1,&a2 );
            dlg.m_LocalPort1 = a1;
            dlg.m_LocalPort2 = a2;
        }

        //目标端口
        s = m_RuleList.GetItemText( m_RuleList.GetSelectionMark(),7 );
        if( s == _T("--"))
            dlg.m_RemotePortType = 0;
        else if( _tcsstr( s.GetBuffer(),_T("-")) == NULL )
        {
            dlg.m_RemotePortType = 1;
            _stscanf_s( s.GetBuffer(),_T("%d"),&a1 );
            dlg.m_RemotePort1 = a1;
        }
        else
        {
            dlg.m_RemotePortType = 2;
            _stscanf_s( s.GetBuffer(),_T("%d-%d"),&a1,&a2 );
            dlg.m_RemotePort1 = a1;
            dlg.m_RemotePort2 = a2;
        }
    }//end if tcp or udp

    if( dlg.m_ProtocolType == IPPROTO_ICMP )
    {
        s = m_RuleList.GetItemText( m_RuleList.GetSelectionMark(),8 );
        _stscanf_s( s.GetBuffer(),_T("%d,%d"),&a1,&a2 );
        
        //列表显示-1代表任意协议，对应号码为31
        if( a1 == -1 )a1 = 31;
        if( a2 == -1 )a2 = 31;
        dlg.m_IcmpType = (BYTE)a1;
        dlg.m_IcmpCode = (BYTE)a2;
    }

    if( IDOK == dlg.DoModal())
    {
        IP_RULES_ELEM rule = {0};

        //删除旧规则，crcName从列表数据中获取
        RegDeleteIpRule( m_RuleList.GetItemData(m_RuleList.GetSelectionMark()));
        
        //初始化一个IP_RULES_ELEM结构，添加新规则
        rule.crcRuleName = crc32_encode( (char *)dlg.m_RuleName.GetBuffer(),
                      dlg.m_RuleName.GetLength() * sizeof( TCHAR)); 
        rule.rule.Bits.Access = dlg.m_bAllow;
        rule.rule.Bits.Direction = dlg.m_Direction;
        rule.rule.Bits.IcmpCode = dlg.m_IcmpCode;
        rule.rule.Bits.IcmpType = dlg.m_IcmpType;
        rule.rule.Bits.LocalAddrType = dlg.m_LocalAddrType;
        rule.rule.Bits.LocalPortType = dlg.m_LocalPortType;
        rule.rule.Bits.ProtocolType = dlg.m_ProtocolType;
        rule.rule.Bits.RemoteAddrType = dlg.m_RemoteAddrType;
        rule.rule.Bits.RemotePortType = dlg.m_RemotePortType;
        rule.rule.Bits.Reserved = 0;
        rule.LocalAddr = htonl(dlg.m_LocalAddr1);
        rule.LocalAddr2 = htonl( dlg.m_LocalAddr2 );
        rule.LocalPort = htons( (USHORT)dlg.m_LocalPort1 );
        rule.LocalPort2 = htons( (USHORT)dlg.m_LocalPort2 );
        rule.RemoteAddr = htonl( dlg.m_RemoteAddr1 );
        rule.RemoteAddr2 = htonl( dlg.m_RemoteAddr2 );
        rule.RemotePort = htons( (USHORT)dlg.m_RemotePort1);
        rule.RemotePort2 = htons( (USHORT)dlg.m_RemotePort2);
        
        if(RegAddIpRule( &rule,dlg.m_RuleName.GetBuffer()))
           UpdateRuleList();
        else
            AfxMessageBox(_T("添加IP规则错误!"));
    }
    else
    {
    }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                               