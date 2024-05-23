#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "cpr/cpr.h"
#include "include/json.hpp"

// Task functions
void fetchTasks(std::vector<std::pair<int, std::string>>& tasks) {
    tasks.clear();
    auto response = cpr::Get(cpr::Url{ "http://localhost:3000/tasks" });
    if (response.status_code == 200) {
        auto json = nlohmann::json::parse(response.text);
        for (const auto& task : json) {
            tasks.push_back({ task["id"], task["description"] });
        }
    }
    else {
        std::cerr << "Failed to fetch tasks: " << response.status_code << std::endl;
    }
}

void addTask(const std::string& description) {
    auto response = cpr::Post(cpr::Url{ "http://localhost:3000/tasks" },
        cpr::Body{ "{\"description\": \"" + description + "\"}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 201) {
        std::cerr << "Failed to add task: " << response.status_code << std::endl;
    }
}

void updateTask(int id, const std::string& description, bool completed) {
    auto response = cpr::Put(cpr::Url{ "http://localhost:3000/tasks/" + std::to_string(id) },
        cpr::Body{ "{\"description\": \"" + description + "\", \"completed\": " + (completed ? "true" : "false") + "}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 200) {
        std::cerr << "Failed to update task: " << response.status_code << std::endl;
    }
}

void deleteTask(int id) {
    auto response = cpr::Delete(cpr::Url{ "http://localhost:3000/tasks/" + std::to_string(id) });
    if (response.status_code != 204) {
        std::cerr << "Failed to delete task: " << response.status_code << std::endl;
    }
}

// Goal functions
void fetchGoals(std::vector<std::pair<int, std::string>>& goals) {
    goals.clear();
    auto response = cpr::Get(cpr::Url{ "http://localhost:3000/goals" });
    if (response.status_code == 200) {
        auto json = nlohmann::json::parse(response.text);
        for (const auto& goal : json) {
            goals.push_back({ goal["id"], goal["description"] });
        }
    }
    else {
        std::cerr << "Failed to fetch goals: " << response.status_code << std::endl;
    }
}

void addGoal(const std::string& description) {
    auto response = cpr::Post(cpr::Url{ "http://localhost:3000/goals" },
        cpr::Body{ "{\"description\": \"" + description + "\"}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 201) {
        std::cerr << "Failed to add goal: " << response.status_code << std::endl;
    }
}

void updateGoal(int id, const std::string& description, bool completed) {
    auto response = cpr::Put(cpr::Url{ "http://localhost:3000/goals/" + std::to_string(id) },
        cpr::Body{ "{\"description\": \"" + description + "\", \"completed\": " + (completed ? "true" : "false") + "}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 200) {
        std::cerr << "Failed to update goal: " << response.status_code << std::endl;
    }
}

void deleteGoal(int id) {
    auto response = cpr::Delete(cpr::Url{ "http://localhost:3000/goals/" + std::to_string(id) });
    if (response.status_code != 204) {
        std::cerr << "Failed to delete goal: " << response.status_code << std::endl;
    }
}

// Note functions
void fetchNotes(std::vector<std::pair<int, std::string>>& notes) {
    notes.clear();
    auto response = cpr::Get(cpr::Url{ "http://localhost:3000/notes" });
    if (response.status_code == 200) {
        auto json = nlohmann::json::parse(response.text);
        for (const auto& note : json) {
            notes.push_back({ note["id"], note["content"] });
        }
    }
    else {
        std::cerr << "Failed to fetch notes: " << response.status_code << std::endl;
    }
}

void addNote(const std::string& content) {
    auto response = cpr::Post(cpr::Url{ "http://localhost:3000/notes" },
        cpr::Body{ "{\"content\": \"" + content + "\"}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 201) {
        std::cerr << "Failed to add note: " << response.status_code << std::endl;
    }
}

void updateNote(int id, const std::string& content) {
    auto response = cpr::Put(cpr::Url{ "http://localhost:3000/notes/" + std::to_string(id) },
        cpr::Body{ "{\"content\": \"" + content + "\"}" },
        cpr::Header{ {"Content-Type", "application/json"} });
    if (response.status_code != 200) {
        std::cerr << "Failed to update note: " << response.status_code << std::endl;
    }
}

void deleteNote(int id) {
    auto response = cpr::Delete(cpr::Url{ "http://localhost:3000/notes/" + std::to_string(id) });
    if (response.status_code != 204) {
        std::cerr << "Failed to delete note: " << response.status_code << std::endl;
    }
}
