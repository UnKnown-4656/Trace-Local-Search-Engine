# Mini Search Engine

A local file-search tool built in C++17 that indexes filenames on your computer and lets you search for them using keywords.

---

## Overview

This project is a command-line file-search utility. It scans a folder on your hard drive, breaks every filename into keywords (tokens), and stores those keywords in an index. When you search for something, it looks up your keywords in the index and shows you matching files, sorted by how many of your keywords each file matched.

I built this as a learning project to understand how search engines work at a fundamental level — specifically **inverted indexes**, **tokenization**, and **relevance ranking**. It started as a single-file procedural program and was gradually refactored into a modular, object-oriented design with SQLite-based persistence.

The project is in an early/prototype stage. The core pipeline works — you can scan a directory, save the index, reload it, and search — but there are known limitations and several features I'd like to add in the future.

---

## Features

### Implemented

- ✅ Recursive directory scanning using C++17 `std::filesystem`
- ✅ Filename tokenization (splits on `_`, `.`, `-`, spaces, parentheses, brackets)
- ✅ Case-insensitive search (all tokens lowercased at index time)
- ✅ Inverted index stored in memory (`unordered_map<string, unordered_set<string>>`)
- ✅ SQLite persistence — index saved to disk so you don't re-scan every time
- ✅ Transactional database writes (batch inserts wrapped in `BEGIN`/`COMMIT`)
- ✅ Keyword search with basic ranking (files matching more keywords score higher)
- ✅ Set-intersection search (find files matching *all* keywords, not just any)
- ✅ On-demand re-scanning from the CLI menu
- ✅ Basic error handling for filesystem permission errors

### Not Yet Implemented

- 🚧 Searching inside file contents (only filenames are indexed)
- 🚧 Fuzzy/approximate matching (typos won't find results)
- 🚧 Configurable scan directory (currently hardcoded)
- 🚧 File metadata search (size, date, extension filters)
- 🚧 Build system (no CMake or Makefile yet)
- 🚧 Automated tests
- 🚧 Incremental re-indexing (full re-scan every time)
- 🚧 Cleaning up stale entries for deleted files

---

## Architecture

The project has four main parts:

### Indexer (`src/Indexer/`)

This is responsible for:
- Walking through folders recursively and finding all files
- Breaking each filename into tokens (e.g., `"my_report.pdf"` → `["my", "report", "pdf"]`)
- Building an inverted index — a map where each keyword points to a set of file paths that contain it
- Saving that index to an SQLite database, and loading it back later

### SearchEngine (`src/SearchEngine/`)

This handles the actual searching:
- Takes the user's query and tokenizes it the same way filenames were tokenized
- Looks up each query token in the inverted index
- Counts how many query tokens matched each file path (this becomes the "score")
- Sorts results by score so the most relevant files appear first

There's also an intersection-based search method that only returns files matching *every* keyword, but it's not currently exposed in the menu.

### Utils (`src/Utils/`)

Small helper functions shared across the project:
- `ToLower()` — converts a string to lowercase (currently unused since tokenizers handle this inline)
- `input()` — prompts the user and reads a full line of input (supports spaces)

### SQLite (`src/Indexer/sql/`)

The project vendors the SQLite amalgamation (the entire SQLite library as a single `.c` file). This is compiled and linked directly into the binary — no external database server needed.

The database schema is simple:
```sql
CREATE TABLE file_index (
    token      TEXT,
    file_paths TEXT,
    UNIQUE(token, file_paths)
);
```

Each row maps one token to one file path. The `UNIQUE` constraint prevents duplicate entries on re-index.

---

## Example Workflow

```
1. Application starts
2. Checks if "index.db" exists on disk
   ├── YES → Loads the saved index into memory (fast)
   └── NO  → Scans the filesystem recursively (slower, first run only)
              → Saves the index to "index.db"
3. User sees the menu:
   ┌─────────────────────────┐
   │ 1. Search for a file    │
   │ 2. Rescan files         │
   │ 3. Exit                 │
   └─────────────────────────┘
4. User types: "report pdf"
5. Engine tokenizes query → ["report", "pdf"]
6. Looks up each token in the index:
   "report" → { fileA, fileB }
   "pdf"    → { fileA, fileC }
7. Scores:
   fileA → 2 (matched both tokens)
   fileB → 1 (matched "report" only)
   fileC → 1 (matched "pdf" only)
8. Returns results sorted by score:
   fileA
   fileB
   fileC
```

---

## Project Structure

```
MINI_SEARCH_ENGINE/
│
├── src/                          # Main source code
│   ├── main.cpp                  # Entry point and CLI menu loop
│   ├── Indexer/
│   │   ├── sql_indexer.h         # Indexer class declaration
│   │   ├── sql_indexer.cpp       # Indexer implementation
│   │   ├── sql/                  # Vendored SQLite3 library
│   │   │   ├── sqlite3.h
│   │   │   ├── sqlite3.c
│   │   │   └── sqlite3.o         # Pre-compiled SQLite object
│   │   └── old/                  # Previous version (flat-file persistence)
│   │       ├── Indexer.h
│   │       └── Indexer.cpp
│   ├── SearchEngine/
│   │   ├── SearchEngine.h        # SearchEngine class + SearchResult struct
│   │   └── SearchEngine.cpp      # Search and ranking logic
│   └── Utils/
│       ├── utils.h
│       └── utils.cpp
│
├── experiments/                  # Standalone algorithm experiments
│   ├── trie.cpp                  # Trie data structure prototype
│   ├── intersection.cpp          # Set-intersection algorithm test
│   ├── neural.cpp                # Linear regression experiment (unrelated)
│   ├── tokenization_testing.cpp  # Tokenizer test
│   └── sqllite.cpp/              # Early SQLite integration tests
│
├── old/                          # Archived: original monolithic version
│   ├── header.h
│   └── functions.cpp
│
└── .gitignore
```

### Notes on `old/` and `experiments/`

These folders are not part of the working application. `old/` contains the original single-file version before I refactored into separate classes. `experiments/` contains standalone programs I wrote while learning specific algorithms and APIs. I kept them in the repo to document my learning process, but they don't compile with or link into the main binary.

---

## Technical Challenges Solved

### Tokenization

Filenames don't follow a single naming convention. `project_report.pdf`, `Project-Report.pdf`, and `project report.pdf` should all be searchable by "project" and "report". The tokenizer handles this by splitting on multiple delimiters and lowercasing everything. Getting the delimiter set right (and keeping it consistent between indexing and searching) was an iterative process — I initially missed parentheses and brackets.

### Ranking

Simply returning all files that match *any* keyword produces too many results. I implemented a scoring system where each file's score equals the number of query tokens it matched. This means a file matching 3 out of 3 keywords appears above a file matching only 1. It's a simple approach — not TF-IDF or BM25 — but it noticeably improves result quality for multi-keyword queries.

### Persistence with SQLite

My first version saved the index as a pipe-delimited text file (`token|path`). It worked, but was slow for large indexes and had no protection against corruption. Switching to SQLite with prepared statements and transactions was a significant improvement. Wrapping all inserts in a single transaction (`BEGIN`/`COMMIT`) made saving dramatically faster because SQLite doesn't need to sync to disk after every individual insert.

### Recursive File Traversal

Using `std::filesystem::directory_iterator` with recursion was straightforward, but handling permission-denied errors was important. Some system directories throw exceptions when accessed. Wrapping the traversal in try-catch blocks ensures the scanner skips inaccessible folders instead of crashing.

---

## What I Learned

Writing this project taught me several things I wouldn't have learned from textbooks alone:

- **Inverted indexes** — I now understand why they're the core data structure behind search. Mapping keywords to documents (instead of scanning every document for keywords) makes lookup essentially instant.

- **Hash maps in practice** — Using `unordered_map` and `unordered_set` gave me hands-on experience with hash-based containers, their trade-offs versus ordered containers, and how to choose the right one for a use case.

- **SQLite C API** — Working with `sqlite3_prepare_v2`, parameter binding, `sqlite3_step`, and callbacks taught me how real database APIs work at a low level. The difference between individual inserts and transactional batches was eye-opening (100x+ performance difference).

- **Refactoring** — The project started as one big file with free functions. Extracting classes (`Indexer`, `SearchEngine`), separating headers from implementations, and using dependency injection (passing the index by const reference instead of making SearchEngine access the database) made the code much easier to reason about.

- **C++17 features** — `std::filesystem` for directory traversal, range-based for loops, structured bindings, and `auto` type deduction. These modern features made the code significantly cleaner compared to the C-style filesystem APIs I would have needed otherwise.

- **Trade-offs** — I explored several approaches (Trie, multimap, intersection-only search) in the experiments folder before settling on the current design. Each had pros and cons, and going through that process helped me understand that engineering is about choosing the right trade-off, not finding a perfect solution.

---

## Current Limitations

Being honest about what this project doesn't do:

| Limitation | Detail |
|-----------|--------|
| **Filename-only search** | Only filenames are indexed. File contents are never read or searched. |
| **Hardcoded scan path** | The directory to scan is hardcoded to `D:\Downloads` in `main.cpp`. Changing it requires editing code and recompiling. |
| **No fuzzy matching** | Typos in queries won't match. Searching "reprot" won't find "report". |
| **No incremental updates** | Re-scanning rebuilds the entire index from scratch. There's no diffing or timestamp checking. |
| **Stale entries persist** | If a file is deleted, its entry stays in the database until a full re-scan + fresh save. |
| **Tokenizer inconsistency** | The Indexer splits on 8 delimiters but the SearchEngine splits on only 4. This is a known bug — files with `()` or `[]` in their names may not be findable. |
| **No build system** | No CMake or Makefile. You need to know the exact compiler command. |
| **No tests** | No unit or integration test suite. The `experiments/` folder has ad-hoc tests but nothing automated. |
| **Windows only** | Uses hardcoded Windows paths (`D:\\`). Would need path adjustments for Linux/macOS. |
| **No result pagination** | All results print at once, which can flood the terminal for broad queries. |

---

## Future Roadmap

### Short-Term (next few weeks)

- [ ] Fix the tokenizer inconsistency (unify into a single shared function)
- [ ] Accept the scan directory as a command-line argument
- [ ] Add `#pragma once` to all headers
- [ ] Remove `using namespace std;` from header files
- [ ] Add a CMakeLists.txt or Makefile
- [ ] Wire the intersection search to the CLI menu
- [ ] Clean commented-out code

### Medium-Term (1–2 months)

- [ ] Add file extension filtering (e.g., search only `.pdf` files)
- [ ] Implement incremental re-indexing using file modification timestamps
- [ ] Prune stale database entries for deleted files
- [ ] Add result pagination (show 10 at a time)
- [ ] Improve ranking with TF-IDF (weigh rare tokens higher)
- [ ] Add basic unit tests

### Long-Term (exploring)

- [ ] Index file contents (at least plain text files)
- [ ] Fuzzy matching for typo tolerance
- [ ] Auto-complete suggestions using a Trie
- [ ] A simple GUI or web interface
- [ ] Cross-platform support

---

## Build Instructions

### Prerequisites

- A C++17 compatible compiler (GCC 8+, MSVC 2017+, or Clang 7+)
- Windows (currently; see Limitations)

### Compile

```bash
cd src

# If sqlite3.o doesn't exist, compile it first:
gcc -c Indexer/sql/sqlite3.c -o Indexer/sql/sqlite3.o

# Compile the project:
g++ -std=c++17 -o search.exe main.cpp Indexer/sql_indexer.cpp SearchEngine/SearchEngine.cpp Utils/utils.cpp Indexer/sql/sqlite3.o
```

### Important

Before building, open `main.cpp` and change the hardcoded path on line 6 to a directory you want to index:

```cpp
fs::path MyPath = "D:\\Downloads";  // ← Change this to your target folder
```

---

## Usage

```
$ ./search.exe

1. Search for a file
2. Rescan files
3. Exit
Enter your choice: 1
Enter Target File Name: lecture notes

D:\University\lecture_notes_chapter3.pdf
D:\University\lecture-notes-final.docx
```

- **Option 1** — Enter one or more keywords. Results are sorted by relevance (files matching more keywords appear first).
- **Option 2** — Re-scans the filesystem and updates the database. Use this after adding or removing files.
- **Option 3** — Exits the program.

On first run, scanning may take a few seconds depending on the size of the target directory. On subsequent runs, the saved index loads almost instantly.

---

## Tech Stack

| Technology | Why |
|-----------|-----|
| C++17 | Modern language features (`std::filesystem`, range-based loops) |
| SQLite 3 | Lightweight embedded database — no server setup needed |
| STL containers | `unordered_map` and `unordered_set` for the inverted index |

---

*Built as a learning project to explore search engine fundamentals, database integration, and software architecture in C++.*
