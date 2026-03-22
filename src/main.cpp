#define NOMINMAX

#define _WIN32_WINNT 0x0601
#define WINVER 0x0601

#include <windows.h>
#include <cstring>
#include <iostream>
#include <dsetup.h>
#include "resource.h"
#define DIRECTINPUT_VERSION 0x0800
#include <cstring>
#include <d3d9.h>
#include <dinput.h>
#include <d3dx9.h>
#include "bitmapUtils.h"
HINSTANCE ghInstance = NULL;
LOGFONTA GLOBAL_FONT = { 0 };
/* this wont work in dx9 and i cannot make cmake work with dx8
bool check_if_needs_directx() {
    int directx_version;
    DWORD local_c[4];
    DWORD needed_version;

    directx_version = DirectXSetupGetVersion(&needed_version, local_c);
    if ((directx_version != 0) && (0x40007 < needed_version)) {
        return true;
    }
    return false;
}

int directx_checker_dialog(void) {
    bool doesnt_needs_to_install_directx;
    int clicked_option;
    LPCSTR window_message;
    undefined2 extraout_var;
    undefined2 extraout_var_00;
    undefined2 extraout_var_01;
    undefined1 local_214[516];
    undefined1 *local_10;
    undefined4 local_c;
    undefined1 directx_check_passed;
    UINT DIALOG_TYPE_FLAG;
    LPCSTR window_text;

    directx_check_passed = 1;
    doesnt_needs_to_install_directx = check_if_needs_directx();
    if (!doesnt_needs_to_install_directx) {
        /* yes / no buttons = 4
        DIALOG_TYPE_FLAG = 4;
        window_text = (LPCSTR) FUN_0054fd25(1, CONCAT22(extraout_var, DAT_0057910a), 0x10);
        window_message = (LPCSTR) FUN_0054fd25(8, CONCAT22(extraout_var_01, DAT_0057910a), 0x800);
        clicked_option = MessageBoxA(window_handler, window_message, window_text, DIALOG_TYPE_FLAG);
        if (clicked_option == 6) {
            FUN_00401053(local_214);
            FUN_00565860(local_214, s_\Install\dxsetup.exe_005793cc);
            local_10 = local_214;
            local_c = 0;
            FUN_00565bbe(0, local_214, &local_10);
        } else {
            /* Error Dialog

            DIALOG_TYPE_FLAG = 0x10;
            window_text = (LPCSTR) FUN_0054fd25(1, DAT_0057910a, 0x10);
            window_message = (LPCSTR) FUN_0054fd25(8, CONCAT22(extraout_var_00, DAT_0057910a), 0x1000);
            MessageBoxA(window_handler, window_message, window_text, DIALOG_TYPE_FLAG);
            directx_check_passed = 0;
        }
    }
    return directx_check_passed;
}
*/

struct EngineContext {
    IDirect3DDevice9* device;
    IDirect3D9* d3d;
    HWND hwnd;
    IDirectInput8* dinput;


};
int get_lang_bitmask() {
    LANGID langid;
    int lang_bitmask;

    langid = GetUserDefaultLangID();
    switch (langid & 0xff) {
        case 7:
            lang_bitmask = 4;
            break;
        default:
            lang_bitmask = 2;
            break;
        case 9:
            lang_bitmask = 1;
            break;
        case 10:
            lang_bitmask = 0x10;
            break;
        case 0xc:
            lang_bitmask = 2;
            break;
        case 0x10:
            lang_bitmask = 0x20;
            break;
        case 0x13:
            lang_bitmask = 8;
            break;
        case 0x16:
            lang_bitmask = 0x40;
    }
    return lang_bitmask;
}
/*
EngineContext directx_init(HWND windowInstance, HINSTANCE hInstance, int mystery_param) {
    // DirectDrawEnumerate(directx_callback_func,0,7); THIS IS DEPRECATED IN DX9
        EngineContext ctx = {};
        ctx.d3d = Direct3DCreate9(D3D_SDK_VERSION);
        ctx.hwnd = windowInstance;
        D3DPRESENT_PARAMETERS pp = {};
        pp.Windowed = FALSE;
        pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pp.hDeviceWindow = windowInstance;
        pp.BackBufferFormat = D3DFMT_UNKNOWN;
        ctx.d3d->CreateDevice(
          D3DADAPTER_DEFAULT,
          D3DDEVTYPE_HAL,
          windowInstance,
          D3DCREATE_HARDWARE_VERTEXPROCESSING,
          &pp,
          &ctx.device
      );

        DirectInput8Create(
            hInstance,
            DIRECTINPUT_VERSION,
            IID_IDirectInput8,
            (void**)&ctx.dinput,
            nullptr);
        return ctx;


}*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    // TODO: Understand how the fuck does the game actually get the name as i cannot find it in the strings or resources
    LPCSTR modal_title = "Sheep Raider Decomp";

    switch (message) {
        case WM_ERASEBKGND: {
            HDC hdc = (HDC)wParam;

            // Load bitmap (resource ID 115 = 0x73)
            HBITMAP hBitmap = LoadBitmapA(ghInstance, MAKEINTRESOURCE(115));
            if (!hBitmap)
                return FALSE;

            BITMAP bm;
            GetObjectA(hBitmap, sizeof(BITMAP), &bm);

            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hOld = (HBITMAP)SelectObject(hdcMem, hBitmap);

            // Blit bitmap to dialog background
            BitBlt(
                hdc,
                0, 0,
                bm.bmWidth,
                bm.bmHeight,
                hdcMem,
                0, 0,
                SRCCOPY
            );

            // Cleanup
            SelectObject(hdcMem, hOld);
            DeleteDC(hdcMem);
            DeleteObject(hBitmap);

            return TRUE;
        }
        case WM_INITDIALOG:
            {
            SetWindowTextA(hDlg, modal_title);
            struct ButtonInfo {
                int id;
                int bmpId;
                COLORREF color;
            };

            ButtonInfo buttons[] = {
                {1100, 145, 0x000000},
                {1102, 145, 0x000000},
                {1103, 145, 0x000000},
                {1104, 145, 0x000000},
                {1105, 145, 0x000000},
                {1106, 145, 0x000000},
                {1108, 145, 0x000000},
                {1101, 146, 0xF6EEEE}, // Play
                {1109, 146, 0xF6EEEE}, //Quit
            };

            for (auto& b : buttons)
            {
                char text[256];
                GetDlgItemText(hDlg, b.id, text, sizeof(text));

                HBITMAP hBmp = drawTextOverBitmap(hDlg, b.bmpId, text, b.color);

                HWND hBtn = GetDlgItem(hDlg, b.id);
                SendMessage(hBtn, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBmp);
            }
            return TRUE;
    }

        case WM_COMMAND:
        {
            if (HIWORD(wParam) == BN_CLICKED)
            {
                // Extract the original control ID
                int raw_id = LOWORD(wParam);

                // Apply the same offset as the original program
                int clicked_button_id = raw_id - 1100; // 0x44C

                switch (clicked_button_id)
                {
                    case 0:
                        // corresponds to button ID 1100
                        MessageBoxA(hDlg, "Install DirectX btn", "Info", MB_OK);
                        break;

                    case 1:
                        // corresponds to button ID 1101 (Play in your mapping)

                        EndDialog(hDlg, 0);
                        break;

                        break;

                    case 9:
                        // corresponds to button ID 1109 (Quit)
                        EndDialog(hDlg, 1);
                        break;

                    default:
                        break;
                }
            }
            return TRUE;
        }
    }
    return FALSE;
}

void contextHandler(const EngineContext & ctx)
{
    HDC hdc = GetDC(ctx.hwnd);

}

void create_window(HINSTANCE hInstance) {
    HWND main_window;
    WNDCLASS wc = {};

    char CLASS_NAME[256];
    strncpy(CLASS_NAME, "Sheep,_Dog'n_Wolf_D3D", 0xFF);
    CLASS_NAME[0xFF - 1] = '\0';

    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    main_window = CreateWindow(
        CLASS_NAME, // class name
        "", // window title
        WS_OVERLAPPEDWINDOW, // style
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    MSG msg = {};

    int result = DialogBox(hInstance, MAKEINTRESOURCE(IDD_LAUNCHER), NULL, DialogProc);

    if (result == 1) {
        return;
    }
    ShowWindow(main_window, SW_SHOW);
    UpdateWindow(main_window);
    //EngineContext ctx = directx_init(main_window, hInstance, 3);
    //contextHandler(ctx);
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow) {
    ghInstance = hInstance;
    // Get The actual font
    strcpy(GLOBAL_FONT.lfFaceName, "Tahoma");
    GLOBAL_FONT.lfHeight = -11;
    create_window(hInstance);

    return 0;
}
