#include <iostream>
#include <Windows.h>
#include "resource.h"

#include <SDK/Optional/StringHash.hpp>

void UpdateHash(HWND p_Dlg)
{
    int m_TextLength = GetWindowTextLengthA(GetDlgItem(p_Dlg, IDC_EDIT1));
    if (!m_TextLength)
    {
        SetDlgItemTextA(p_Dlg, IDC_EDIT2, "");
        return;
    }

    char* m_Text = new char[m_TextLength + 1];
    GetDlgItemTextA(p_Dlg, IDC_EDIT1, m_Text, m_TextLength + 1);

    uint32_t m_Hash = (IsDlgButtonChecked(p_Dlg, IDC_CHECK1) ? SDK::StringHashUpper32(m_Text) : SDK::StringHash32(m_Text));
    char m_FormatHash[16];
    sprintf_s(m_FormatHash, sizeof(m_FormatHash), "0x%X", m_Hash);
    SetDlgItemTextA(p_Dlg, IDC_EDIT2, m_FormatHash);

    delete[] m_Text;
}

BOOL CALLBACK DialogProc(HWND p_Dlg, UINT p_Msg, WPARAM p_WParam, LPARAM p_LParam)
{
    switch (p_Msg)
    {
        case WM_KEYDOWN:
            return TRUE;
        case WM_INITDIALOG: 
        {
            HICON m_IconHandle = LoadIconA(GetModuleHandleA(0), MAKEINTRESOURCEA(IDI_ICON1));
            if (m_IconHandle)
            {
                SendMessageA(p_Dlg, WM_SETICON, ICON_SMALL, (LPARAM)(m_IconHandle));
                SendMessageA(p_Dlg, WM_SETICON, ICON_BIG, (LPARAM)(m_IconHandle));
            }
            return TRUE;
        }
        case WM_DESTROY:
            EndDialog(p_Dlg, TRUE);
            break;
        case WM_COMMAND:
        {
            switch (LOWORD(p_WParam))
            {
                case IDCANCEL:
                    EndDialog(p_Dlg, TRUE); break;
                case IDC_EDIT1:
                {
                    if (HIWORD(p_WParam) == EN_CHANGE)
                        UpdateHash(p_Dlg);
                }
                break;
                case IDC_CHECK1:
                {
                    if (HIWORD(p_WParam) == BN_CLICKED)
                        UpdateHash(p_Dlg);
                }
                break;
            }
        }
        break;
    }

    return FALSE;
}

int WINAPI WinMain(HINSTANCE p_Instance, HINSTANCE p_PrevInstance, PSTR p_CmdLine, int p_CmdShow)
{
    DialogBoxA(p_Instance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)DialogProc);
    return 0;
}