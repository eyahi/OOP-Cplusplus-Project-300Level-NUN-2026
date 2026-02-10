#define WINVER 0x0600
#define _WIN32_WINNT 0x0600
#define TME_NONCLIENT 0x00000010
#define GET_XBUTTON_WPARAM(wParam) HIWORD(wParam)
#define RTL_OSVERSIONINFOEXW OSVERSIONINFOEXW

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

struct Product {
    int id;
    std::string name;
    std::string category;
    float price;
    int quantity;
};

struct User {
    std::string username;
    std::string password; // Encrypted
};

struct CartItem {
    int productId;
    std::string name;
    float price;
    int quantity;
};

struct Sale {
    int id;
    std::string date;
    std::vector<CartItem> items;
    float total;
    float discount;
};

std::vector<Product> products;
std::vector<User> users;
std::vector<Sale> sales;

std::string encrypt(const std::string& s, int shift = 3) {
    std::string res = s;
    for (char& c : res) {
        if (isalpha(c)) {
            char base = isupper(c) ? 'A' : 'a';
            c = (c - base + shift) % 26 + base;
        }
    }
    return res;
}

std::string decrypt(const std::string& s, int shift = 3) {
    return encrypt(s, 26 - shift);
}

void loadUsers() {
    std::ifstream file("data/users.txt");
    users.clear();
    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string username, password;
            std::getline(ss, username, ',');
            std::getline(ss, password);
            users.push_back({username, password});
        }
    }
}

void saveUsers() {
    std::ofstream file("data/users.txt");
    for (const auto& u : users) {
        file << u.username << "," << u.password << "\n";
    }
}

void loadProducts() {
    std::ifstream file("data/products.txt");
    products.clear();
    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int id, quantity;
            std::string name, category;
            float price;
            ss >> id;
            ss.ignore();
            std::getline(ss, name, ',');
            std::getline(ss, category, ',');
            ss >> price >> quantity;
            products.push_back({id, name, category, price, quantity});
        }
    }
}

void saveProducts() {
    std::ofstream file("data/products.txt");
    for (const auto& p : products) {
        file << p.id << "," << p.name << "," << p.category << "," << p.price << "," << p.quantity << "\n";
    }
}

void loadSales() {
    std::ifstream file("data/sales.txt");
    sales.clear();
    if (file) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            int id;
            std::string date;
            float total, discount;
            ss >> id;
            ss.ignore();
            std::getline(ss, date, ',');
            ss >> total >> discount;
            sales.push_back({id, date, {}, total, discount});
        }
    }
}

void saveSales() {
    std::ofstream file("data/sales.txt");
    for (const auto& s : sales) {
        file << s.id << "," << s.date << "," << s.total << "," << s.discount << "\n";
    }
}
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <GL/gl.h>
#include <tchar.h>

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ZyLog", NULL };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"ZyLog v2.0 - Inventory Management System", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, NULL, NULL, wc.hInstance, NULL);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    loadUsers();
    loadProducts();
    loadSales();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    // Our state
    bool loggedIn = false;
    std::string currentUser;
    std::vector<CartItem> cart;
    float discountPercent = 0.0f;
    std::string search = "";
    char newName[128] = "";
    char newCategory[128] = "";
    float newPrice = 0.0f;
    int newQuantity = 0;
    int editId = -1;

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (!loggedIn) {
            ImGui::Begin("Login");
            static char username[128] = "";
            static char password[128] = "";
            ImGui::InputText("Username", username, 128);
            ImGui::InputText("Password", password, 128, ImGuiInputTextFlags_Password);
            if (ImGui::Button("Login")) {
                std::string enc_pass = encrypt(password);
                for (const auto& u : users) {
                    if (u.username == username && u.password == enc_pass) {
                        loggedIn = true;
                        currentUser = username;
                        break;
                    }
                }
                if (!loggedIn) {
                    ImGui::TextColored(ImVec4(1,0,0,1), "Invalid credentials");
                }
            }
            if (ImGui::Button("Register")) {
                if (strlen(username) > 0 && strlen(password) > 0) {
                    users.push_back({username, encrypt(password)});
                    saveUsers();
                    ImGui::TextColored(ImVec4(0,1,0,1), "Registered successfully");
                } else {
                    ImGui::TextColored(ImVec4(1,0,0,1), "Enter username and password");
                }
            }
            ImGui::End();
        } else {
            ImGui::Begin(("ZyLog v2.0 - " + currentUser).c_str());
            if (ImGui::BeginTabBar("Tabs")) {
                if (ImGui::BeginTabItem("Inventory")) {
                    ImGui::InputText("Search", &search[0], search.capacity());
                    std::vector<Product> filtered;
                    for (const auto& p : products) {
                        if (p.name.find(search) != std::string::npos || p.category.find(search) != std::string::npos) {
                            filtered.push_back(p);
                        }
                    }
                    if (ImGui::BeginTable("Products", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                        ImGui::TableSetupColumn("ID");
                        ImGui::TableSetupColumn("Name");
                        ImGui::TableSetupColumn("Category");
                        ImGui::TableSetupColumn("Price");
                        ImGui::TableSetupColumn("Quantity");
                        ImGui::TableSetupColumn("Actions");
                        ImGui::TableHeadersRow();
                        for (const auto& p : filtered) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%d", p.id);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s", p.name.c_str());
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%s", p.category.c_str());
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%.2f", p.price);
                            ImGui::TableSetColumnIndex(4);
                            if (p.quantity < 5) {
                                ImGui::TextColored(ImVec4(1,0,0,1), "%d", p.quantity);
                            } else {
                                ImGui::Text("%d", p.quantity);
                            }
                            ImGui::TableSetColumnIndex(5);
                            if (ImGui::Button(("Edit##" + std::to_string(p.id)).c_str())) {
                                editId = p.id;
                                strcpy(newName, p.name.c_str());
                                strcpy(newCategory, p.category.c_str());
                                newPrice = p.price;
                                newQuantity = p.quantity;
                            }
                            ImGui::SameLine();
                            if (ImGui::Button(("Delete##" + std::to_string(p.id)).c_str())) {
                                products.erase(std::remove_if(products.begin(), products.end(), [&](const Product& pr) { return pr.id == p.id; }), products.end());
                                saveProducts();
                            }
                        }
                        ImGui::EndTable();
                    }
                    ImGui::InputText("Name", newName, 128);
                    ImGui::InputText("Category", newCategory, 128);
                    ImGui::InputFloat("Price", &newPrice);
                    ImGui::InputInt("Quantity", &newQuantity);
                    if (editId == -1) {
                        if (ImGui::Button("Add Product")) {
                            int id = products.empty() ? 1 : products.back().id + 1;
                            products.push_back({id, newName, newCategory, newPrice, newQuantity});
                            saveProducts();
                            newName[0] = 0;
                            newCategory[0] = 0;
                            newPrice = 0;
                            newQuantity = 0;
                        }
                    } else {
                        if (ImGui::Button("Update Product")) {
                            for (auto& p : products) {
                                if (p.id == editId) {
                                    p.name = newName;
                                    p.category = newCategory;
                                    p.price = newPrice;
                                    p.quantity = newQuantity;
                                    break;
                                }
                            }
                            saveProducts();
                            editId = -1;
                            newName[0] = 0;
                            newCategory[0] = 0;
                            newPrice = 0;
                            newQuantity = 0;
                        }
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("POS")) {
                    ImGui::Text("Products:");
                    for (const auto& p : products) {
                        if (ImGui::Button((p.name + " - $" + std::to_string(p.price)).c_str())) {
                            bool found = false;
                            for (auto& c : cart) {
                                if (c.productId == p.id) {
                                    c.quantity++;
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                cart.push_back({p.id, p.name, p.price, 1});
                            }
                        }
                    }
                    ImGui::Separator();
                    ImGui::Text("Cart:");
                    float total = 0;
                    for (auto& c : cart) {
                        ImGui::Text("%s x%d = %.2f", c.name.c_str(), c.quantity, c.price * c.quantity);
                        total += c.price * c.quantity;
                        ImGui::SameLine();
                        if (ImGui::Button(("-##" + std::to_string(c.productId)).c_str())) {
                            if (c.quantity > 1) {
                                c.quantity--;
                            } else {
                                cart.erase(std::remove_if(cart.begin(), cart.end(), [&](const CartItem& ci) { return ci.productId == c.productId; }), cart.end());
                            }
                        }
                    }
                    ImGui::InputFloat("Discount %", &discountPercent);
                    float discountAmount = total * discountPercent / 100;
                    float finalTotal = total - discountAmount;
                    ImGui::Text("Subtotal: %.2f", total);
                    ImGui::Text("Discount: %.2f", discountAmount);
                    ImGui::Text("Total: %.2f", finalTotal);
                    if (ImGui::Button("Checkout")) {
                        int saleId = sales.empty() ? 1 : sales.back().id + 1;
                        std::time_t t = std::time(nullptr);
                        char date[20];
                        std::strftime(date, sizeof(date), "%Y-%m-%d", std::localtime(&t));
                        sales.push_back({saleId, date, cart, finalTotal, discountAmount});
                        saveSales();
                        for (const auto& c : cart) {
                            for (auto& p : products) {
                                if (p.id == c.productId) {
                                    p.quantity -= c.quantity;
                                    break;
                                }
                            }
                        }
                        saveProducts();
                        cart.clear();
                        discountPercent = 0;
                        ImGui::Text("Sale completed!");
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Analytics")) {
                    ImGui::Text("Sales History:");
                    if (ImGui::BeginTable("Sales", 4, ImGuiTableFlags_Borders)) {
                        ImGui::TableSetupColumn("ID");
                        ImGui::TableSetupColumn("Date");
                        ImGui::TableSetupColumn("Total");
                        ImGui::TableSetupColumn("Discount");
                        ImGui::TableHeadersRow();
                        for (const auto& s : sales) {
                            ImGui::TableNextRow();
                            ImGui::TableSetColumnIndex(0);
                            ImGui::Text("%d", s.id);
                            ImGui::TableSetColumnIndex(1);
                            ImGui::Text("%s", s.date.c_str());
                            ImGui::TableSetColumnIndex(2);
                            ImGui::Text("%.2f", s.total);
                            ImGui::TableSetColumnIndex(3);
                            ImGui::Text("%.2f", s.discount);
                        }
                        ImGui::EndTable();
                    }
                    if (ImGui::Button("Export to CSV")) {
                        std::ofstream file("sales_export.csv");
                        file << "ID,Date,Total,Discount\n";
                        for (const auto& s : sales) {
                            file << s.id << "," << s.date << "," << s.total << "," << s.discount << "\n";
                        }
                        ImGui::Text("Exported to sales_export.csv");
                    }
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Dashboard")) {
                    int totalProducts = products.size();
                    int totalSales = sales.size();
                    float totalRevenue = 0;
                    for (const auto& s : sales) {
                        totalRevenue += s.total;
                    }
                    ImGui::Text("Total Products: %d", totalProducts);
                    ImGui::Text("Total Sales: %d", totalSales);
                    ImGui::Text("Total Revenue: %.2f", totalRevenue);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}