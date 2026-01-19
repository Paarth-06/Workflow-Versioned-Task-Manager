# Versioned Workflow & Task Management System (C++ OOP Demo)

## 📖 Overview
This project is a **Versioned Workflow and Task Management System** implemented in **C++** to demonstrate intermediate-to-advanced **Object-Oriented Programming (OOP)** and **STL-based system design** concepts.

The system allows users to:
- manage tasks under individual user profiles
- track complete task history using snapshots
- restore tasks to any previously saved version

Instead of implementing simple undo logic, this project focuses on **snapshot-based state preservation and rollback**, similar to real-world version control systems.

---

## 🧠 Concepts Used

- **Namespaces**
  - `wl` is used to logically group all workflow-related classes and data.

- **Object-Oriented Programming**
  - Classes model users and their associated tasks.
  - Inheritance is used to extend user functionality for task management.

- **Encapsulation**
  - Task data is represented using a structured data type (`snap`).
  - Internal state is modified only through controlled class methods.

- **STL Containers**
  - `unordered_map` for fast lookup of users and tasks.
  - `vector` for maintaining ordered task history (snapshots).

- **Snapshot-Based Version Control**
  - Every task mutation stores a full snapshot of task state.
  - Allows rollback to any historical version.

---

## 🏗️ System Structure

### `snap` (Task & Snapshot Structure)
Acts as a blueprint for both:
- current task state
- historical versions of a task

**Attributes:**
- `task_id`
- `task_name`
- `task_status`
- `task_note`

---

### `User`
Handles all user-related operations.

**Responsibilities:**
- Create users
- Remove users
- View all users
- View a specific user

**Shared Data Managed:**
- User profile information
- Task storage
- Task history

---

### `Tasks` (Derived Class)
Extends `User` and manages all task-related operations.

**Responsibilities:**
- Create tasks
- Remove tasks
- Update task title, status, and notes
- Take task snapshots
- Restore tasks to any selected historical version
- View task history
- View active tasks for a user

---

## ⚙️ Features

### User Management
- Add user
- Remove user
- View all users
- View a specific user

### Task Management
- Add task
- Remove task
- Update task title
- Update task status
- Update task notes
- View tasks for a user

### Version Control
- Automatic snapshot creation on:
  - task creation
  - task updates
  - task deletion
- Task history stored using `vector<snap>`
- Custom rollback: user selects which version to restore

---

## 🔄 Rollback Mechanism

Each snapshot stores the **entire task state** at a specific moment in time.

When restoring:
1. All available versions are displayed with version indices.
2. The user selects the desired version.
3. The current task state is replaced with the selected snapshot.

This approach restores **state**, not individual changes, ensuring reliable recovery.

---

## 🧪 How the Program Works

1. User creates a profile.
2. Tasks are created under a user.
3. Every task modification automatically creates a snapshot.
4. Task history is preserved in chronological order.
5. User can restore a task to any previous version.

---

## ▶️ Sample Flow

```text
Enter User ID : 1
Enter Task ID : 101
Enter Task Title : Fix login bug
Enter Task Status : TO-DO
Enter Task Note : urgent
Task added successfully...

Change task status → IN_PROGRESS
Saved in memory...

View history
Select version index → 0
Version restored...
