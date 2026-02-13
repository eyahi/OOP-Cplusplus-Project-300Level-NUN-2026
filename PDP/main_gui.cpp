#include "DiaryManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <ctime>

#define ICON_FA_SAVE   u8"\uf0c7"
#define ICON_FA_PLUS   u8"\uf067"
#define ICON_FA_BOOK   u8"\uf02d"

// set automatic date for diary entry
std::string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[20];
    sprintf(buf, "%02d-%02d-%04d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
    return std::string(buf);
}

void RenderDiaryApp(DiaryManager &manager) {
    static char titleBuf[128] = "";
    static char contentBuf[1024] = "";

    // set App size
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                               ImGui::GetIO().DisplaySize.y * 0.5f),
                        ImGuiCond_Once, ImVec2(0.5f, 0.5f));
ImGui::SetNextWindowPos(ImVec2(50, 50), ImGuiCond_Once);
ImGui::SetNextWindowSize(ImVec2(900, 650), ImGuiCond_Once);
ImGui::Begin(" My Personal Diary");


    ImGui::TextColored(ImVec4(0.10f, 0.14f, 0.20f, 1.0f), "Write a New Entry");
    ImGui::Separator();

    ImGui::InputTextWithHint("##title", "Enter Title...", titleBuf, IM_ARRAYSIZE(titleBuf));
    ImGui::InputTextMultiline("##content", contentBuf, IM_ARRAYSIZE(contentBuf),
                              ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 10));

    ImGui::Spacing();


if (ImGui::Button(ICON_FA_PLUS " Add Entry", ImVec2(130, 0))) {
    if (strlen(titleBuf) > 0 && strlen(contentBuf) > 0) {
        manager.addEntry(DiaryEntry(std::string(titleBuf), getCurrentDate(), std::string(contentBuf)));
        titleBuf[0] = '\0';
        contentBuf[0] = '\0';
    }
}
ImGui::SameLine();
if (ImGui::Button(ICON_FA_SAVE " Save All", ImVec2(130, 0))) {
    manager.saveToFile();
}

    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.10f, 0.14f, 0.20f, 1.0f), "Your Past Entries:");
    ImGui::Separator();

    // add scroll  feauture for entries
    ImGui::BeginChild("EntriesScroll", ImVec2(0, 0), true);
    for (const auto &entry : manager.getEntries()) {
        ImGui::TextColored(ImVec4(0.2f, 0.6f, 1.0f, 1.0f),
                           "%s (%s)", entry.getTitle().c_str(), entry.getDate().c_str());
        ImGui::TextWrapped("%s", entry.getContent().c_str());
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::End();
}

int main() {
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(800, 600, "Persona Diary", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

// Load normal font
ImFontConfig config;
config.MergeMode = false;
io.Fonts->AddFontFromFileTTF("fonts/OpenSans-Regular.ttf", 18.0f);

// Merge Font Awesome 7 icons
static const ImWchar icons_ranges[] = { 0xf000, 0xf8ff, 0 }; // icons unicode range
config.MergeMode = true;
config.PixelSnapH = true;
io.Fonts->AddFontFromFileTTF("fonts/Font Awesome 7 Free-Solid-900.otf", 18.0f, &config, icons_ranges);


    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    DiaryManager manager("diary_data.txt");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderDiaryApp(manager);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.07f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    manager.saveToFile();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
