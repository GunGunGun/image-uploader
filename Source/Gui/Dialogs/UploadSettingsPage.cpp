/*

    Image Uploader -  free application for uploading images/files to the Internet

    Copyright 2007-2015 Sergey Svistunov (zenden2k@gmail.com)

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

*/

#include "UploadSettingsPage.h"
#include <uxtheme.h>

#include "wizarddlg.h"
#include "Gui/GuiTools.h"
#include <Func/WinUtils.h>

// CUploadSettingsPage
CUploadSettingsPage::CUploadSettingsPage()
{
        
}

CUploadSettingsPage::~CUploadSettingsPage()
{
}

void CUploadSettingsPage::TranslateUI()
{
    TRC(IDC_CONNECTIONSETTINGS, "��������� �����������");
    TRC(IDC_USEPROXYSERVER, "������������ ������-������");
    TRC(IDC_ADDRESSLABEL, "�����:");
    TRC(IDC_PORTLABEL, "����:");
    TRC(IDC_SERVERTYPE, "��� �������:");
    TRC(IDC_NEEDSAUTH, "���������� �����������");
    TRC(IDC_LOGINLABEL, "�����:");
    TRC(IDC_PASSWORDLABEL, "������:");
    TRC(IDC_AUTOCOPYTOCLIPBOARD, "������������� ���������� ���������� � ����� ������");
    TRC(IDC_UPLOADERRORLABEL, "������ ��������");
    TRC(IDC_IGNOREERRORS, "���������� ���������� ���� � ������ ������");
    TRC(IDC_RETRIES1LABEL, "���-�� ������� �������� �����:");
    TRC(IDC_RETRIES2LABEL, "���-�� ������� ��� ����� ��������:");
    TRC(IDC_UPLOADBUFFERLABEL, "������ ������ ������:");
    TRC(IDC_MAXTHREADSLABEL, "����� �������");
    TRC(IDC_EXECUTESCRIPTCHECKBOX, "��������� Squirrel-������ ��� ������ ������ (�����)");
}
    
LRESULT CUploadSettingsPage::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    TabBackgroundFix(m_hWnd);
    TranslateUI();

    BOOL temp;
    DoDataExchange(FALSE);
    SendDlgItemMessage(IDC_SERVERTYPECOMBO,CB_ADDSTRING,0,(WPARAM)_T("HTTP"));

    SendDlgItemMessage(IDC_SERVERTYPECOMBO,CB_ADDSTRING,0,(WPARAM)_T("SOCKS4"));
    SendDlgItemMessage(IDC_SERVERTYPECOMBO,CB_ADDSTRING,0,(WPARAM)_T("SOCKS4A"));
    SendDlgItemMessage(IDC_SERVERTYPECOMBO,CB_ADDSTRING,0,(WPARAM)_T("SOCKS5"));
    SendDlgItemMessage(IDC_SERVERTYPECOMBO,CB_ADDSTRING,0,(WPARAM)_T("SOCKS5(DNS)"));

    // ---- ������������� ��������� (����������) ----
    
    // ---- ���������� connection settings -----
    SetDlgItemText(IDC_ADDRESSEDIT, Settings.ConnectionSettings.ServerAddress);
    SendDlgItemMessage(IDC_NEEDSAUTH, BM_SETCHECK, (WPARAM) Settings.ConnectionSettings.NeedsAuth);
    SendDlgItemMessage(IDC_AUTOCOPYTOCLIPBOARD, BM_SETCHECK, (WPARAM) Settings.AutoCopyToClipboard);
    
        SendDlgItemMessage(IDC_USEPROXYSERVER, BM_SETCHECK, (WPARAM) Settings.ConnectionSettings.UseProxy);
    SetDlgItemText(IDC_PROXYLOGINEDIT, Settings.ConnectionSettings.ProxyUser);
    SetDlgItemText(IDC_PROXYPASSWORDEDIT, (CString)Settings.ConnectionSettings.ProxyPassword);
    SetDlgItemInt(IDC_UPLOADBUFFERSIZEEDIT,Settings.UploadBufferSize/1024);
    if(Settings.ConnectionSettings.ProxyPort) // ������ ���� ���� �� ����� ����
        SetDlgItemInt(IDC_PORTEDIT, Settings.ConnectionSettings.ProxyPort);


    SendDlgItemMessage(IDC_SERVERTYPECOMBO, CB_SETCURSEL, Settings.ConnectionSettings.ProxyType);
    SendDlgItemMessage(IDC_NEEDSAUTH, BM_SETCHECK, (WPARAM) Settings.ConnectionSettings.NeedsAuth);
    SetDlgItemInt(IDC_MAXTHREADSEDIT, Settings.MaxThreads);
    // ����������� ���������
    OnClickedUseProxy(BN_CLICKED, IDC_USEPROXYSERVER, 0, temp);
    GuiTools::SetCheck(m_hWnd, IDC_EXECUTESCRIPTCHECKBOX, Settings.ExecuteScript);
    SetDlgItemText(IDC_SCRIPTFILENAMEEDIT, IuCoreUtils::Utf8ToWstring(Settings.ScriptFileName).c_str());

    return 1;  // Let the system set the focus
}

LRESULT CUploadSettingsPage::OnClickedOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT CUploadSettingsPage::OnClickedCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    EndDialog(wID);
    return 0;
}

LRESULT CUploadSettingsPage::OnClickedUseProxy(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& bHandled)
{
    bool Checked = SendDlgItemMessage(IDC_USEPROXYSERVER, BM_GETCHECK)!=0;
    GuiTools::EnableNextN(GetDlgItem(wID),Checked? 8: 11, Checked);

    if(Checked)
        OnClickedUseProxyAuth(BN_CLICKED, IDC_NEEDSAUTH, 0, bHandled);

    //::EnableWindow(GetDlgItem(IDC_ADDRESSEDIT), Checked);
    return 0;
}
    
LRESULT CUploadSettingsPage::OnClickedUseProxyAuth(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    bool Checked = SendDlgItemMessage(wID, BM_GETCHECK)!=0;
    GuiTools::EnableNextN(GetDlgItem(wID), 4, Checked);
    return 0;
}

bool CUploadSettingsPage::Apply()
{
    DoDataExchange(TRUE);
    Settings.ConnectionSettings.UseProxy = SendDlgItemMessage(IDC_USEPROXYSERVER, BM_GETCHECK)!=0;
    Settings.ConnectionSettings.NeedsAuth = SendDlgItemMessage(IDC_NEEDSAUTH, BM_GETCHECK)!=0;
    Settings.AutoCopyToClipboard = SendDlgItemMessage(IDC_AUTOCOPYTOCLIPBOARD, BM_GETCHECK)!=0;
    TCHAR Buffer[128];

    GetDlgItemText(IDC_ADDRESSEDIT,Buffer, 128);
    Settings.ConnectionSettings.ServerAddress = Buffer;
    Settings.ConnectionSettings.ProxyPort = GetDlgItemInt(IDC_PORTEDIT);
    
    GetDlgItemText(IDC_PROXYLOGINEDIT, Buffer, 128);
    Settings.ConnectionSettings.ProxyUser = Buffer;
    GetDlgItemText(IDC_PROXYPASSWORDEDIT, Buffer, 128);
    Settings.ConnectionSettings.ProxyPassword = Buffer;
    Settings.ConnectionSettings.ProxyType = SendDlgItemMessage(IDC_SERVERTYPECOMBO, CB_GETCURSEL);
    Settings.UploadBufferSize = GetDlgItemInt(IDC_UPLOADBUFFERSIZEEDIT)*1024;
    if(!Settings.UploadBufferSize) Settings.UploadBufferSize = 65536;
    Settings.MaxThreads = GetDlgItemInt(IDC_MAXTHREADSEDIT);
    if (Settings.MaxThreads < 0 || Settings.MaxThreads > 50 )
    {
        Settings.MaxThreads = 3;
    }

    GuiTools::GetCheck(m_hWnd, IDC_EXECUTESCRIPTCHECKBOX, Settings.ExecuteScript);
    CString scriptFile = GuiTools::GetDlgItemText(m_hWnd, IDC_SCRIPTFILENAMEEDIT);

    if (Settings.ExecuteScript && !WinUtils::FileExists(scriptFile))
    {
        CString message;
        message.Format(TR("���� %s �� ����������"), scriptFile);
        MessageBox(message, APPNAME);
        return false;
    }
    Settings.ScriptFileName = IuCoreUtils::WstringToUtf8((LPCTSTR)scriptFile);

    return true;
}

LRESULT CUploadSettingsPage::OnBnClickedBrowseScriptButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    TCHAR Buf[MAX_PATH * 4];
    GuiTools::SelectDialogFilter(Buf, sizeof(Buf) / sizeof(TCHAR), 2,
        CString(_(".nut")),
        _T("*.nut;"),
        TR("��� �����"),
        _T("*.*"));

    CFileDialog fd(true, 0, 0, 4 | 2, Buf, m_hWnd);
   /* CString s;
    s = WinUtils::GetAppFolder();
    fd.m_ofn.lpstrInitialDir = s;*/
    if (fd.DoModal() != IDOK || !fd.m_szFileName[0]) return 0;

    SetDlgItemText(IDC_SCRIPTFILENAMEEDIT, fd.m_szFileName);

    return 0;
}
