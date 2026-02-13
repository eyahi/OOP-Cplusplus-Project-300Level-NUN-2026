# 📝 Persona Diary Application
## Names and ID
- Faizah Yakubu - 20232908
- Huzaifa Al-amin - 20232219
##  Overview
**Persona Diary** is a simple GUI-based diary application written in C++ using Object-Oriented Programming (OOP) principles and Dear ImGui for the interface.  
It allows users to add personal diary entries, save them to file, and view previous entries in a clean, scrollable layout.

---

##  Features
-  Add new diary entries with title, date, and content  
- Save all entries to file  
- View past entries in a scrollable list  
- Clean and minimal GUI using Dear ImGui  
- Demonstrates OOP concepts (Encapsulation, Classes, File I/O)

---

## Project Structure
📁 PDP (Personal Diary Project)
│
├── DiaryEntry.h # Defines the DiaryEntry class
├── DiaryManager.h # Manages list of entries and file saving
├── DiaryManager.cpp # Implements DiaryManager methods
├── main_gui.cpp # Main GUI logic using Dear ImGui
├── fonts/ # Contains OpenSans and FontAwesome font files
└── README.md # Project documentation

---

## Object-Oriented Concepts Used

| Concept | Implementation |
|----------|----------------|
| **Class & Objects** | `DiaryEntry` and `DiaryManager` classes |
| **Encapsulation** | Class members made private with getters/setters |
| **File Handling** | Entries saved and loaded from `.txt` file |
| **Composition** | `DiaryManager` contains a vector of `DiaryEntry` objects |
| **Abstraction** | Simplified GUI actions for user interaction |

---

## How It Works
1. User types a **title** and **content**, then clicks “Add Entry.”  
2. The entry is created and stored in memory.  
3. “Save All” writes all entries to a local file.  
4. Past entries appear in a scrollable list.  
5. GUI built using ImGui and GLFW/OpenGL backend.

---

## Technologies Used
- C++17  
- Dear ImGui  
- GLFW  
- OpenGL  
- Font Awesome  
- OpenSans Font  

---

## How to Run
1. Clone or download the project folder.  
2. Open in VS Code or your IDE.  
3. Build using your C++ compiler with linked ImGui, GLFW, and OpenGL libraries.  
4. Run the `.exe` — enjoy your diary app!

## 🗂 UML Overview
+-----------------+ +------------------+
| DiaryEntry | | DiaryManager |
+-----------------+ +------------------+
| - title |<>------>| - entries |
| - date | | - filename |
| - content | +------------------+
+-----------------+ | + addEntry() |
| + get/set() | | + saveToFile() |
| + constructor() | | + getEntries() |
+-----------------+ +------------------+

