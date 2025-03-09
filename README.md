# RDBMS Project

## 📌 Overview

This project implements a Relational Database Management System (RDBMS) with two key components:
1. SimpleRA - A basic relational database management system (RDBMS) written in C++.
2. B+Tree Indexing - A data indexing method implemented over an existing unordered heap structure.

The project covers fundamental database functionalities such as table loading, listing, printing, sorting, and joining, along with efficient indexing mechanisms using B+ Trees.

## 🛠️ Features

✅ SimpleRA Functionalities:

1. Load Table: Loads a table into memory.
2. List Tables: Displays all loaded tables.
3. Print Table: Prints the contents of a table.
4. Join: Performs a basic SQL-style join operation.
5. Sort: Sorts table data based on a given column.

✅ Query Execution Pipeline:

Each query execution follows three main stages:
1. Syntactic Parser: Checks for SQL syntax errors.
2. Semantic Parser: Validates table and column existence.
3. Executor: If both parsers return valid responses, the query is executed.

✅ Buffer Management System:

1. Simulates Main Memory: Stores 4-5 pages at a time.
2. Page Management: Each page stores 4 rows; a 12-row table requires 3 pages.
3. Cursor-Based Navigation: Moves from one page to another dynamically.

✅ B+ Tree Indexing:

1. INSERT Key: Inserts a new key into the index.
2. DELETE Key: Removes a key from the index.
3. RANGE Query (key1 to key2): Fetches all keys in a given range.

## 📂 Project Structure
```bash
RDBMS/
│-- src/                         # Source files for database implementation
│   ├── main.cpp                 # Entry point for the RDBMS
│   ├── parser.cpp               # Handles syntactic and semantic parsing
│   ├── executor.cpp             # Executes valid queries
│   ├── buffer_manager.cpp       # Manages in-memory pages
│   ├── table_catalogue.cpp      # Stores metadata of loaded tables
│   ├── table_class.cpp          # Defines table structure and operations
│   ├── cursor.cpp               # Handles page navigation
│   ├── bplus_tree.cpp           # Implements B+ Tree indexing
│-- include/                     # Header files
│   ├── headers.h                # Main header file with function prototypes
│-- docs/                        # Documentation files
│   ├── README.md                # Project documentation
│-- makefile                     # Makefile to build the project
│-- LICENSE                      # Project License
```
