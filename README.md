# Db Create

## Overview
**Db Create** is a command-line database creation and management system written in C++. It is designed to simulate how visual database tools work, but entirely through a CLI interface. Users can create databases, define schemas, manipulate data, and perform bulk operations, all while maintaining strong type safety and clean architecture.

This project focuses on core database concepts, object-oriented design, and extensibility, and is actively being expanded with additional features.

---

## Current Features
- Create, rename, and delete databases  
- Define database schemas by adding columns with different data types  
- Add, edit, and delete rows  
- Rename and delete columns  
- Bulk edit column values  
- Print database contents to the console  
- Safe memory management using smart pointers (RAII)  

All functionality is currently implemented **in memory**, allowing rapid iteration and testing of database behaviors.

---

## Design & Architecture
A core challenge of this project was handling columns of different data types while storing them in a single collection. This is solved through a **polymorphic design**:

- Each column inherits from a shared abstract base class (`BaseCol`)
- Concrete columns are templated (`Column<T>`) to maintain type safety
- Polymorphism enables storing heterogeneous column types in a single container
- This avoids common limitations of purely templated containers while preserving flexibility

The project emphasizes:
- Object-oriented design
- Clear separation of responsibilities
- Consistent naming and documentation
- Safe resource management using RAII and smart pointers

---

## Technology Stack
- **Language:** C++ (C++20)  
- **Development Environment:** VS Code  
- **Version Control:** Git & GitHub  

---

## Example Operations
From the CLI, users can:
- Create and manage databases
- Modify schemas dynamically
- Perform bulk data updates
- Inspect database contents in real time

The interface is designed to mirror how users think about databases, even without a graphical UI.

---

## Planned Features (Work in Progress)
Db Create is actively evolving. Planned enhancements include:
- File-based persistence (save/load databases)
- User authentication and role-based access control
- Multi-user collaboration with permission levels
- Data filtering and query-style operations
- Exporting data to CSV / Excel
- Importing external data sources
- Improved CLI UX and command validation
- Performance optimizations for large datasets

---

## Status
This project is a **work in progress**, with core functionality implemented and additional features under active development. The current focus is on expanding persistence, access control, and usability while maintaining clean, extensible code.

---

## Purpose
Db Create was built to deepen understanding of:
- Database fundamentals
- Advanced C++ features (templates, polymorphism, RAII)
- Designing scalable systems from scratch
- Writing clean, maintainable, well-documented code

