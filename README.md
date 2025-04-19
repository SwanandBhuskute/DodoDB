# 🚀 DodoDB
DodoDB is a lightweight, beginner-friendly database engine written in C++, designed for learning database internals from scratch. It features a REPL interface, persistent file storage, table management, and simple SQL-like operations.

### Tech Stack

- **Language**: C++
- **CLI**: Custom REPL built using [linenoise](https://github.com/antirez/linenoise)
- **File Structure**: Modular (`src/`, `utils/`, `data/`, `meta/`)

## ✅ Phases Completed

### Phase 0: Project Setup & Simple REPL CLI
- Initialized the base project structure
- Implemented a simple command-line interface (REPL)

---

### Phase 1: In-Memory Table with Hardcoded Schema
- Created a single hardcoded `users` table in memory
- Supported `insert`, `select`, `update`, `delete` operations (runtime only)
- **Files**: `table.cpp`, `table.h`

---

### Phase 2: Persistent File-Based Storage
- The database must be able to store and retrieve data persistently — not just in RAM, but in files (on disk), like how real databases store .db files or flat files.
- Data now survives program when restarts
- **Files**: `/data/{tableName}.table.txt`, `table.cpp`, `table.h` 

---

### Phase 3: Multi-Table Management
- Allowed creating, selecting, and switching between multiple tables via CLI
- Maintained metadata for the current active table across sessions
- **Files**: `table_manager.cpp`, `table_manager.h`, `/meta/`

---

### Phase 4: Enhanced REPL using `linenoise`
- Added command history support
- Made the REPL loop stable and error-tolerant
- **Files**: `linenoise.c`, `linenoise.h`, `/meta/dodo_history.txt`

---
### Phase 5: Support Git-like versioning and rollback support
- Implemented saving versions on every insert, update, delete command to new v1/2/3.txt
- Rollback to previous version implemented (all later versions will be deleted)
- **Files**: `/data/{tableName}/versions/`, `table.cpp`, `table.h`

## Future scope
### Phase 6: Multiple Databases Support
Planning to implement something like supporting multiple databases and tables inside them. Currently all tables get created under single data/ folder.
Files (maybe): database_management.cpp and database_management.h


## How to run
- Clone the project and type `make run` in the terminal of the project
- If you get the error `The term 'make' is not recognized` then make sure
- - You have downloaded Git for Windows
- - You can try this command in your Windows Powershell Administrator - `choco install make`
- Then try again `make run`

## Available commands
- `create table tableName (field1 type, field2 type, field3 type, ...)`
- `insert value1 value2 value3` (as per fields)
- `update set fieldName=value where fieldName=value`
- `delete where fieldName=value`
- `select` - print table data
- `rollback v1` - rollback to specific version
- `curr table` - for viewing current table
- `use table tableName` - for using specific table
- `describe` - show table info