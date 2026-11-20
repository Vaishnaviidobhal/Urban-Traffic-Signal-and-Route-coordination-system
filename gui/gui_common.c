#include "gui_common.h"

HWND output_box = NULL;
HWND source_box = NULL;
HWND destination_box = NULL;
HWND load_box = NULL;

int gui_get_selected(HWND box)
{
    char text[8];
    GetWindowTextA(box, text, (int)sizeof(text));
    return text[0] ? toupper((unsigned char)text[0]) - 'A' : -1;
}

void gui_show_text(const char *text)
{
    if (output_box)
        SetWindowTextA(output_box, text);
}

void gui_create_button(HWND parent, const char *label, int id, int x, int y, int width, int height)
{
    CreateWindowA("BUTTON", label, WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                  x, y, width, height, parent, (HMENU)(INT_PTR)id, GetModuleHandleA(NULL), NULL);
}
