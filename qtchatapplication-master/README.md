 Chat Application

 Project Overview

This project is a desktop chat application built using C++ and the Qt framework (Qt Widgets). It provides a graphical user interface for sending and displaying chat messages in real time, with support for message timestamps and a structured UI similar to modern messaging applications.

The application was developed as a learning and demonstration project to showcase Qt GUI development, signal–slot communication and clean separation of UI and logic using Qt Designer.



 System Architecture

Programming Language:C++
GUI Framework: Qt (Qt Widgets)
UI Design Tool:Qt Designer (`.ui` files)
Programming Model: Event-driven (Signals & Slots)



  Key Features

Intuitive graphical chat interface
Message input and display functionality
Timestamp display for chat messages
Qt signal–slot mechanism for user interactions
Clean separation of UI design and business logic


  Software Requirements

 Qt Version: Qt 6.10.1
Compiler: MinGW
Operating System: Windows
Build System: qmake 


 Build and Run Instructions

 Using Qt Creator 

1. Launch Qt Creator
2. Select Open Project
3. Open the project file ('.pro')
4. Configure the appropriate Qt Kit (Qt 6 recommended)
5. Click Run to build and execute the application



 User Interface Description

The graphical interface is defined in `mainwindow.ui` and includes:

* A text input field for composing messages
* A message display area for sent and received chats
* Time labels indicating when messages are sent

Qt Designer is used for layout and styling, while all interactive behavior is implemented in C++.


 License

This project is distributed under the terms specified in the **LICENSE.txt** file.


 Future Enhancements

* Client–server communication using TCP/IP
* User authentication system
* Chat history persistence (database or file storage)
* Emoji



 Author

Developed as an educational project using **C++ and Qt** by:
* Udeme Esien 20230041
* Sultan Owolawi 20230888
* Aidan Ikhumetse 20230077




