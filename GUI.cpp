
#include <windows.h>
#include <commctrl.h>
#include "TaskManager.cpp"

#define ID_LISTBOX 1001
#define ID_EDIT_TASK 1002
#define ID_EDIT_DATE 1003
#define ID_ADD_BTN 1004
#define ID_COMPLETE_BTN 1005
#define ID_DELETE_BTN 1006
#define ID_CLEAR_BTN 1007
#define ID_EDIT_BTN 1008

TodoManager* manager = nullptr;
HWND hListBox, hEditTask, hEditDate;
HFONT hTitleFont, hButtonFont, hTextFont;

void RefreshList(HWND hwnd) {
    SendMessage(hListBox, LB_RESETCONTENT, 0, 0);
    const vector<Task>& tasks = manager->getAllTasks();
    for (const auto& task : tasks) {
        string item = to_string(task.getId()) + ". ";
        item += task.isCompleted() ? "[DONE] " : "[ ] ";
        item += task.getDescription() + " | Due: " + task.getDueDate();
        SendMessageA(hListBox, LB_ADDSTRING, 0, (LPARAM)item.c_str());
    }
}

HFONT CreateCustomFont(int height, int weight, const TCHAR* faceName) {
    return CreateFont(height, 0, 0, 0, weight, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, faceName);
}

bool validateDateFormat(const string& dateStr) {
    if (dateStr.length() != 10) return false;
    int dashCount = 0;
    for (size_t i = 0; i < dateStr.length(); i++) {
        char c = dateStr[i];
        if (c == '-') dashCount++;
        else if (c < '0' || c > '9') return false;
    }
    return (dashCount == 2);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            hTitleFont = CreateCustomFont(28, FW_BOLD, TEXT("Segoe UI"));
            hButtonFont = CreateCustomFont(18, FW_SEMIBOLD, TEXT("Segoe UI"));
            hTextFont = CreateCustomFont(16, FW_NORMAL, TEXT("Segoe UI"));
            
            HWND hTitle = CreateWindow(TEXT("STATIC"), TEXT("TO-DO LIST MANAGER"), 
                WS_VISIBLE | WS_CHILD | SS_CENTER,
                10, 15, 660, 40, hwnd, NULL, NULL, NULL);
            SendMessage(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
            
            HWND hTaskLabel = CreateWindow(TEXT("STATIC"), TEXT("Task Description:"), 
                WS_VISIBLE | WS_CHILD, 10, 70, 150, 20, hwnd, NULL, NULL, NULL);
            SendMessage(hTaskLabel, WM_SETFONT, (WPARAM)hTextFont, TRUE);
            
            hEditTask = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""), 
                WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
                10, 95, 440, 30, hwnd, (HMENU)ID_EDIT_TASK, NULL, NULL);
            SendMessage(hEditTask, WM_SETFONT, (WPARAM)hTextFont, TRUE);
            
            HWND hDateLabel = CreateWindow(TEXT("STATIC"), TEXT("Date:"), 
                WS_VISIBLE | WS_CHILD, 460, 70, 100, 20, hwnd, NULL, NULL, NULL);
            SendMessage(hDateLabel, WM_SETFONT, (WPARAM)hTextFont, TRUE);
            
            hEditDate = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT(""), 
                WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
                460, 95, 210, 30, hwnd, (HMENU)ID_EDIT_DATE, NULL, NULL);
            SendMessage(hEditDate, WM_SETFONT, (WPARAM)hTextFont, TRUE);
            
            HWND hAddBtn = CreateWindow(TEXT("BUTTON"), TEXT("+ Add Task"), 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 140, 160, 40, hwnd, (HMENU)ID_ADD_BTN, NULL, NULL);
            SendMessage(hAddBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            
            HWND hEditBtn = CreateWindow(TEXT("BUTTON"), TEXT("Edit Task"), 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                180, 140, 160, 40, hwnd, (HMENU)ID_EDIT_BTN, NULL, NULL);
            SendMessage(hEditBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            
            HWND hCompleteBtn = CreateWindow(TEXT("BUTTON"), TEXT("Mark Complete"), 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                350, 140, 160, 40, hwnd, (HMENU)ID_COMPLETE_BTN, NULL, NULL);
            SendMessage(hCompleteBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            
            HWND hDeleteBtn = CreateWindow(TEXT("BUTTON"), TEXT("Delete"), 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                520, 140, 150, 40, hwnd, (HMENU)ID_DELETE_BTN, NULL, NULL);
            SendMessage(hDeleteBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            
            hListBox = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("LISTBOX"), TEXT(""), 
                WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_NOTIFY,
                10, 195, 660, 330, hwnd, (HMENU)ID_LISTBOX, NULL, NULL);
            SendMessage(hListBox, WM_SETFONT, (WPARAM)hTextFont, TRUE);
            
            HWND hClearBtn = CreateWindow(TEXT("BUTTON"), TEXT("Clear All Tasks"), 
                WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 540, 660, 40, hwnd, (HMENU)ID_CLEAR_BTN, NULL, NULL);
            SendMessage(hClearBtn, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
            
            manager = new TodoManager();
            RefreshList(hwnd);
            break;
        }
        
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC)wParam;
            SetTextColor(hdcStatic, RGB(20, 50, 100));
            SetBkColor(hdcStatic, RGB(230, 240, 255));
            return (LRESULT)CreateSolidBrush(RGB(230, 240, 255));
        }
        
        case WM_CTLCOLOREDIT: {
            HDC hdcEdit = (HDC)wParam;
            SetTextColor(hdcEdit, RGB(0, 0, 0));
            SetBkColor(hdcEdit, RGB(255, 255, 255));
            return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
        }
        
        case WM_CTLCOLORLISTBOX: {
            HDC hdcList = (HDC)wParam;
            SetTextColor(hdcList, RGB(0, 0, 0));
            SetBkColor(hdcList, RGB(250, 252, 255));
            return (LRESULT)CreateSolidBrush(RGB(250, 252, 255));
        }
        
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case ID_ADD_BTN: {
                    char taskBuffer[512], dateBuffer[128];
                    GetWindowTextA(hEditTask, taskBuffer, 512);
                    GetWindowTextA(hEditDate, dateBuffer, 128);
                    
                    if (strlen(taskBuffer) == 0) {
                        MessageBoxA(hwnd, "Please enter a task description!", 
                                   "Input Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    if (strlen(dateBuffer) == 0) {
                        MessageBoxA(hwnd, "Please enter a date!", 
                                   "Input Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    if (!validateDateFormat(string(dateBuffer))) {
                        MessageBoxA(hwnd, 
                            "Invalid date format!\n\nPlease use: YYYY-MM-DD\nExample: 2026-01-25", 
                            "Invalid Date", MB_OK | MB_ICONERROR);
                        break;
                    }
                    
                    manager->addTask(string(taskBuffer), string(dateBuffer));
                    SetWindowTextA(hEditTask, "");
                    SetWindowTextA(hEditDate, "");
                    RefreshList(hwnd);
                    MessageBoxA(hwnd, "Task added successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    break;
                }
                
                case ID_EDIT_BTN: {
                    int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index == LB_ERR) {
                        MessageBoxA(hwnd, "Please select a task to edit!", 
                                   "Selection Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    const vector<Task>& tasks = manager->getAllTasks();
                    if (index >= tasks.size()) break;
                    
                    char taskBuffer[512], dateBuffer[128];
                    GetWindowTextA(hEditTask, taskBuffer, 512);
                    GetWindowTextA(hEditDate, dateBuffer, 128);
                    
                    if (strlen(taskBuffer) == 0 || strlen(dateBuffer) == 0) {
                        MessageBoxA(hwnd, "Please enter both task and date!", 
                                   "Input Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    if (!validateDateFormat(string(dateBuffer))) {
                        MessageBoxA(hwnd, "Invalid date format!\n\nPlease use: YYYY-MM-DD", 
                                   "Invalid Date", MB_OK | MB_ICONERROR);
                        break;
                    }
                    
                    manager->editTask(tasks[index].getId(), string(taskBuffer), string(dateBuffer));
                    SetWindowTextA(hEditTask, "");
                    SetWindowTextA(hEditDate, "");
                    RefreshList(hwnd);
                    MessageBoxA(hwnd, "Task updated successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    break;
                }
                
                case ID_COMPLETE_BTN: {
                    int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index == LB_ERR) {
                        MessageBoxA(hwnd, "Please select a task!", 
                                   "Selection Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    const vector<Task>& tasks = manager->getAllTasks();
                    if (index >= tasks.size()) break;
                    
                    manager->markComplete(tasks[index].getId());
                    RefreshList(hwnd);
                    MessageBoxA(hwnd, "Task marked as complete!", "Success", MB_OK | MB_ICONINFORMATION);
                    break;
                }
                
                case ID_DELETE_BTN: {
                    int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                    if (index == LB_ERR) {
                        MessageBoxA(hwnd, "Please select a task!", 
                                   "Selection Required", MB_OK | MB_ICONWARNING);
                        break;
                    }
                    
                    const vector<Task>& tasks = manager->getAllTasks();
                    if (index >= tasks.size()) break;
                    
                    int result = MessageBoxA(hwnd, "Are you sure you want to delete this task?", 
                                           "Confirm Delete", MB_YESNO | MB_ICONQUESTION);
                    
                    if (result == IDYES) {
                        manager->deleteTask(tasks[index].getId());
                        SetWindowTextA(hEditTask, "");
                        SetWindowTextA(hEditDate, "");
                        RefreshList(hwnd);
                        MessageBoxA(hwnd, "Task deleted successfully!", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
                
                case ID_LISTBOX: {
                    if (HIWORD(wParam) == LBN_SELCHANGE) {
                        int index = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
                        if (index != LB_ERR) {
                            const vector<Task>& tasks = manager->getAllTasks();
                            if (index < tasks.size()) {
                                SetWindowTextA(hEditTask, tasks[index].getDescription().c_str());
                                SetWindowTextA(hEditDate, tasks[index].getDueDate().c_str());
                            }
                        }
                    }
                    break;
                }
                
                case ID_CLEAR_BTN: {
                    int result = MessageBoxA(hwnd, 
                        "Are you sure you want to clear ALL tasks?\nThis cannot be undone!", 
                        "Confirm Clear All", MB_YESNO | MB_ICONWARNING);
                    
                    if (result == IDYES) {
                        manager->clearAll();
                        SetWindowTextA(hEditTask, "");
                        SetWindowTextA(hEditDate, "");
                        RefreshList(hwnd);
                        MessageBoxA(hwnd, "All tasks cleared!", "Success", MB_OK | MB_ICONINFORMATION);
                    }
                    break;
                }
            }
            break;
        }
        
        case WM_DESTROY:
            if (hTitleFont) DeleteObject(hTitleFont);
            if (hButtonFont) DeleteObject(hButtonFont);
            if (hTextFont) DeleteObject(hTextFont);
            if (manager) delete manager;
            PostQuitMessage(0);
            return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    
    const TCHAR CLASS_NAME[] = TEXT("TodoAppClass");
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(230, 240, 255));
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    if (!RegisterClass(&wc)) {
        MessageBoxA(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, TEXT("To-Do List Manager Pro"),
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 700, 660,
        NULL, NULL, hInstance, NULL);
    
    if (hwnd == NULL) {
        MessageBoxA(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}