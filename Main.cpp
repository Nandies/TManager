#ifdef APIENTRY
#undef APIENTRY
#endif

#define _USE_MATH_DEFINES // For M_PI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "cpr/cpr.h"
#include "include/json.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include <cmath> // Include cmath for M_PI
#include <algorithm>
#include "fetch.h"
#include "fonts.h"
#include "colors.h"

// Define M_PI if not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Animation helpers
float EaseInOutSine(float t) {
    return -(cos(M_PI * t) - 1) / 2;
}

float Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

void ShowTaskManager();

int main(int argc, char** argv) {
    // Setup GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Task Manager", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Apply the custom colors
    SetCustomColors();

    // Load custom fonts
    LoadCustomFonts();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show Task Manager
        ShowTaskManager();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.11f, 0.15f, 0.17f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void ShowTaskManager() {
    static std::vector<std::pair<int, std::string>> tasks;
    static std::vector<std::pair<int, std::string>> goals;
    static std::vector<std::pair<int, std::string>> notes;

    static bool showAddTaskPopup = false;
    static bool showAddGoalPopup = false;
    static bool showAddNotePopup = false;

    static auto start = std::chrono::high_resolution_clock::now();
    static float popupAlpha = 0.0f;

    // Fetch data from server
    fetchTasks(tasks);
    fetchGoals(goals);
    fetchNotes(notes);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Task Manager", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (ImGui::BeginTabBar("Sections")) {
        if (ImGui::BeginTabItem("Tasks")) {
            ImGui::Text("Tasks:");
            for (const auto& task : tasks) {
                ImGui::BulletText("%s", task.second.c_str());
                ImGui::SameLine();
                if (ImGui::Button(("Remove##task" + std::to_string(task.first)).c_str())) {
                    deleteTask(task.first);
                }
            }
            if (ImGui::Button("Add Task")) {
                showAddTaskPopup = true;
                start = std::chrono::high_resolution_clock::now();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Goals")) {
            ImGui::Text("Goals:");
            for (const auto& goal : goals) {
                ImGui::BulletText("%s", goal.second.c_str());
                ImGui::SameLine();
                if (ImGui::Button(("Remove##goal" + std::to_string(goal.first)).c_str())) {
                    deleteGoal(goal.first);
                }
            }
            if (ImGui::Button("Add Goal")) {
                showAddGoalPopup = true;
                start = std::chrono::high_resolution_clock::now();
            }
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Notes")) {
            ImGui::Text("Notes:");
            for (const auto& note : notes) {
                ImGui::BulletText("%s", note.second.c_str());
                ImGui::SameLine();
                if (ImGui::Button(("Remove##note" + std::to_string(note.first)).c_str())) {
                    deleteNote(note.first);
                }
            }
            if (ImGui::Button("Add Note")) {
                showAddNotePopup = true;
                start = std::chrono::high_resolution_clock::now();
            }
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    // Fade-in effect for popups
    auto now = std::chrono::high_resolution_clock::now();
    float elapsed = std::chrono::duration<float>(now - start).count();
    popupAlpha = EaseInOutSine(elapsed > 1.0f ? 1.0f : elapsed);

    // Popup for adding task
    if (showAddTaskPopup) {
        ImGui::OpenPopup("Add Task");
        ImGui::SetNextWindowBgAlpha(popupAlpha);
    }
    if (ImGui::BeginPopupModal("Add Task", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char taskDesc[128] = "";
        ImGui::InputText("Description", taskDesc, IM_ARRAYSIZE(taskDesc));
        if (ImGui::Button("Add")) {
            addTask(taskDesc);
            strcpy_s(taskDesc, sizeof(taskDesc), "");
            ImGui::CloseCurrentPopup();
            showAddTaskPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            showAddTaskPopup = false;
        }
        ImGui::EndPopup();
    }

    // Popup for adding goal
    if (showAddGoalPopup) {
        ImGui::OpenPopup("Add Goal");
        ImGui::SetNextWindowBgAlpha(popupAlpha);
    }
    if (ImGui::BeginPopupModal("Add Goal", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char goalDesc[128] = "";
        ImGui::InputText("Description", goalDesc, IM_ARRAYSIZE(goalDesc));
        if (ImGui::Button("Add")) {
            addGoal(goalDesc);
            strcpy_s(goalDesc, sizeof(goalDesc), "");
            ImGui::CloseCurrentPopup();
            showAddGoalPopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            showAddGoalPopup = false;
        }
        ImGui::EndPopup();
    }

    // Popup for adding note
    if (showAddNotePopup) {
        ImGui::OpenPopup("Add Note");
        ImGui::SetNextWindowBgAlpha(popupAlpha);
    }
    if (ImGui::BeginPopupModal("Add Note", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        static char noteContent[128] = "";
        ImGui::InputText("Content", noteContent, IM_ARRAYSIZE(noteContent));
        if (ImGui::Button("Add")) {
            addNote(noteContent);
            strcpy_s(noteContent, sizeof(noteContent), "");
            ImGui::CloseCurrentPopup();
            showAddNotePopup = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            showAddNotePopup = false;
        }
        ImGui::EndPopup();
    }

    ImGui::End();
}