# 🏥 Clinic Management System in C

A fully modular, menu-driven **Clinic & Patient Management System** implemented in standard C. The project showcases fundamental software engineering practices, dynamic memory allocation using Singly Linked Lists, input buffer management, and dual-mode authorization (Admin vs. User).

---

## 📌 Features & Capabilities

### 🔑 Admin Mode (Password Protected)
* **Authentication**: Secured with password validation (`1234`) and limited to 3 attempts before system shutdown.
* **Add Patient Record**: Creates a new record with unique ID validation, non-empty name, age range check (1–120), and gender selection (`M`/`F`).
* **Edit Patient Record**: Modifies existing patient details while retaining the allocated ID.
* **Reserve Doctor Slot**: Books 1 of 5 fixed daily slots (`2:00pm - 5:00pm`) for registered patients, preventing duplicate reservations.
* **Cancel Reservation**: Revokes a booked time slot for a given patient ID.
* **Delete Patient Record**: Removes patient node from memory and automatically frees any associated slot reservation.

### 👤 User Mode (Public Access)
* **View Patient Record**: Displays patient details (ID, Name, Age, Gender) by searching the ID.
* **View Today's Reservations**: Renders a real-time status of all 5 daily slots showing whether each is `Available` or `Reserved (Patient ID)`.

---

## ⚙️ Technical Highlights & Data Architecture

* **Custom Type Definitions**: Utilizes standard Embedded C fixed-width type aliases (`u8`, `u16`, `u32`, `s8`, etc.) for clean memory abstraction.
* **Dynamic Memory Management**: Employs Singly Linked Lists (`patient` struct) with dynamic allocation (`malloc` / `free`) for dynamic record storage.
* **Safe Input Handling**: Custom buffer clearance (`flush_input`) and robust numeric/string validations (`read_valid_u16`, `read_valid_u8`) to prevent infinite loops and buffer overflows.
* **Memory Leak Prevention**: Systematic cleanup mechanism (`free_all_patients`) executed prior to system termination.

---

## 📋 Available Time Slots

The system manages 5 fixed daily slots:
1. `2:00pm to 2:30pm`
2. `2:30pm to 3:00pm`
3. `3:00pm to 3:30pm`
4. `4:00pm to 4:30pm`
5. `4:30pm to 5:00pm`

---

## 🚀 How to Build and Run

### 1. Prerequisites
* Any C IDE / Compiler (e.g., Code::Blocks, VS Code with GCC, or Dev-C++).

### 2. Running the Project
1. Open the project/source file (`clinic_management_system.c`) in your IDE/Compiler.
2. Compile and Build the file to generate the executable file (`.exe`).
3. Run the generated executable file directly by double-clicking it from your folder/Desktop.
