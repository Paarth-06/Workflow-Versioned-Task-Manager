# Versioned Task Management System (C++ OOP Demo)

## 📖 Overview
This project is a **Versioned Task Management System** implemented in **C++** to demonstrate advanced **Object-Oriented Programming (OOP)** and system design concepts.

The system allows users to create tasks, update task status, and maintain a complete version history of each task using **snapshot-based rollback**.

Unlike simple undo mechanisms, this project focuses on **state preservation and restoration**.

---

## 🧠 Concepts Used

- **Namespaces**
  - `wk` is used to logically group all classes and shared data.

- **Object-Oriented Programming**
  - Classes are used to model users and tasks.
  - Inheritance is used to extend user functionality.

- **Encapsulation**
  - Task data is stored using structured data types (`snap`).
  - Internal state is modified only through class methods.

- **STL Containers**
  - `unordered_map` for fast lookup of users and tasks.
  - `vector` for maintaining ordered task history (snapshots).

- **Snapshot-Based Version Control**
  - Every task modification stores a full snapshot of the task state.
  - Snapshots allow safe rollback to previous versions.

---

## 🏗️ System Structure

### `snap` (Task & Snapshot Structure)
Represents both:
- the current state of a task
- historical versions of the task

**Attributes:**
- `id`
- `title`
- `status`
- `note`

---

### `User`
Handles user-related operations.

**Responsibilities:**
- Create users
- Remove users
- View all users

**Shared Data:**
- User profile information
- Task storage
- Task history

---

### `Tasks` (Derived Class)
Extends `User` to handle all task-related operations.

**Responsibilities:**
- Create tasks
- Remove tasks
- Update task status
- Take snapshots
- Roll back tasks to previous versions
- View tasks

---

## ⚙️ Features

### User Management
- Add user
- Remove user
- View all users

### Task Management
- Add task
- Remove task
- Update task status
- View tasks for a user

### Version Control
- Automatic snapshot creation on:
  - task creation
  - task updates
  - task deletion
- Rollback to the **last saved version**
- Task history preserved using `vector<snap>`

---

## 🔄 Rollback Mechanism

Each snapshot stores the **entire task state** at a specific moment in time.

Rollback restores a task by replacing the current state with a previously saved snapshot.  
This ensures reliable recovery without manual reversal of changes.

---

## 🧪 How the Program Works

1. User creates a profile.
2. Tasks are added under a specific user.
3. Any task change automatically creates a snapshot.
4. Task history is preserved in order.
5. User can revert a task to the previously saved version.

---

## ▶️ Sample Flow

```text
Enter your user id : 1
Enter task id : 101
Enter task title : Fix login bug
Enter task status : todo
Enter task note : urgent
Task created successfully

Change task status → in_progress
Snapshot taken

Rollback → previous version restored

---

## 🔮 Future Scope

```text
Custom rollback mechanism to display all available task versions and allow users to select a specific version to restore

File persistence to store users, tasks, and history across sessions

Stronger typing for task data instead of string-based storage

Improved separation of responsibilities to reduce shared state

Enhanced input validation and error handling

These features were intentionally deferred to keep the current version stable and conceptually focused.