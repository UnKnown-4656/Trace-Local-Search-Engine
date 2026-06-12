<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-blue?style=for-the-badge&logo=cplusplus&logoColor=white" />
  <img src="https://img.shields.io/badge/SQLite-3-003B57?style=for-the-badge&logo=sqlite&logoColor=white" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white" />
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" />
</p>

<h1 align="center">🔍 Mini Search Engine</h1>

<p align="center">
  <b>A local file-search engine built from scratch in C++17</b><br/>
  <sub>Recursive indexing · Inverted index · SQLite persistence · Ranked search results</sub>
</p>

---

## 📖 About

**Mini Search Engine** is a high-performance local file-search tool that indexes your filesystem and lets you find any file instantly using keyword queries — like a personal Google for your hard drive.

It recursively scans a directory tree, tokenizes every filename into searchable keywords, stores the resulting **inverted index** in an embedded SQLite database, and returns search results ranked by relevance score.

### Why This Project?

| Problem | Solution |
|---------|----------|
| Windows search is slow on large drives | Pre-built index enables **instant** lookups |
| Can't search by partial filename keywords | Tokenization splits filenames into searchable parts |
| Search results aren't ranked | Frequency-based scoring surfaces the best matches first |
| Index is lost when app closes | SQLite persistence means **zero re-scan** on restart |

---

## ✨ Features

- 🗂️ **Recursive Directory Scanning** — Traverses all subdirectories using C++17 `std::filesystem`
- 🔤 **Smart Tokenization** — Splits filenames by common delimiters (`_`, `.`, `-`, `space`, `()`, `[]`) and lowercases for case-insensitive matching
- 📊 **Inverted Index** — Hash-map-based index with O(1) average-case lookups
- 💾 **SQLite Persistence** — Transactional batch writes with prepared statements; sub-second index loading
- 🏆 **Ranked Results** — Multi-keyword queries scored by token-match frequency
- 🔗 **Intersection Search** — AND-logic search returning only files matching *all* keywords
- 🔄 **Live Re-indexing** — Re-scan and update the index on demand
- ⚡ **Fast Startup** — Loads pre-built index from disk; skips scanning if `index.db` exists

---

## 🏗️ Architecture

```
┌─────────────┐      ┌───────────────┐      ┌─────────────────┐
│  Filesystem  │─────▶│    Indexer     │─────▶│  SQLite (disk)  │
│              │ scan │  Tokenize +   │ save │   index.db      │
└─────────────┘      │  Build Index  │      └────────┬────────┘
                     └───────┬───────┘               │ load
                             │                       │
                     ┌───────▼───────┐      ┌────────▼────────┐
                     │ In-Memory Map │◀─────│  SQLite (disk)  │
                     │ token → paths │      └─────────────────┘
                     └───────┬───────┘
                             │ const ref
                     ┌───────▼───────┐      ┌─────────────────┐
                     │ SearchEngine  │─────▶│  Ranked Results │
                     │ Score + Sort  │      │  (SearchResult) │
                     └───────────────┘      └─────────────────┘
```

The project follows a **modular OOP architecture** with clear separation of concerns:

| Module | Responsibility |
|--------|---------------|
| **Indexer** | Filesystem crawling, tokenization, inverted index construction, SQLite persistence |
| **SearchEngine** | Query tokenization, index lookup, scoring, result ranking |
| **Utils** | Shared helpers (case conversion, user input) |
| **main.cpp** | Application entry point, CLI menu loop, orchestration |

> The `SearchEngine` receives the index via **dependency injection** (const reference) — it never touches the filesystem or database directly.

---

## 📁 Project Structure

```
MINI_SEARCH_ENGINE/
├── src/                        # Production source code
│   ├── main.cpp                # Entry point & CLI loop
│   ├── Indexer/
│   │   ├── sql_indexer.h       # Indexer class declaration
│   │   ├── sql_indexer.cpp     # Implementation (SQLite-backed)
│   │   └── sql/                # Vendored SQLite3 amalgamation
│   │       ├── sqlite3.h
│   │       ├── sqlite3.c
│   │       └── sqlite3ext.h
│   ├── SearchEngine/
│   │   ├── SearchEngine.h      # SearchEngine class + SearchResult struct
│   │   └── SearchEngine.cpp    # Search & ranking implementations
│   └── Utils/
│       ├── utils.h             # Utility declarations
│       └── utils.cpp           # ToLower, input helpers
├── experiments/                # Algorithm prototypes & explorations
│   ├── trie.cpp                # Trie-based prefix search
│   ├── intersection.cpp        # Set-intersection algorithm
│   ├── neural.cpp              # Linear regression experiment
│   └── sqllite.cpp/            # SQLite integration prototype
├── .gitignore
└── README.md
```

---

## 🚀 Getting Started

### Prerequisites

- **C++17** compatible compiler (GCC 8+, MSVC 2017+, Clang 7+)
- **Windows OS** (uses `std::filesystem` with Windows paths)

### Build

```bash
# Clone the repository
git clone https://github.com/UnKnown-4656/Trace-Local-Search-Engine.git
cd Trace-Local-Search-Engine

# Compile with g++ (MinGW)
g++ -std=c++17 -o search.exe main.cpp Indexer/sql_indexer.cpp SearchEngine/SearchEngine.cpp Utils/utils.cpp Indexer/sql/sqlite3.o
```

> **Note**: The SQLite amalgamation (`sqlite3.o`) is pre-compiled. To rebuild it:
> ```bash
> gcc -c Indexer/sql/sqlite3.c -o Indexer/sql/sqlite3.o
> ```

### Run

```bash
./search.exe
```

### Usage

```
1. Search for a file
2. Rescan files
3. Exit
Enter your choice: 1
Enter Target File Name: report pdf
D:\Documents\annual_report.pdf
D:\Work\project_report-final.pdf
D:\Downloads\report_summary.pdf
```

---

## ⚙️ How It Works

### 1. Indexing

The **Indexer** recursively walks a directory tree. For each file, it tokenizes the filename:

```
Input:  "Project_Report-v2.pdf"
Tokens: ["project", "report", "v2", "pdf"]
```

Each token is mapped to the file's full path in an **inverted index**:

```
"project" → { "D:\Work\Project_Report-v2.pdf" }
"report"  → { "D:\Work\Project_Report-v2.pdf", "D:\Docs\report.txt" }
"pdf"     → { "D:\Work\Project_Report-v2.pdf", "D:\Docs\invoice.pdf" }
```

### 2. Persistence

The index is stored in SQLite with transactional batch inserts:

```sql
CREATE TABLE file_index (
    token      TEXT,
    file_paths TEXT,
    UNIQUE(token, file_paths)
);
```

### 3. Searching

When a user queries `"report pdf"`, the engine:

1. **Tokenizes** the query → `["report", "pdf"]`
2. **Looks up** each token in the index
3. **Scores** each matching path by how many tokens it matches
4. **Sorts** results by score (descending)

```
Score 2: D:\Work\Project_Report-v2.pdf      ← matches "report" AND "pdf"
Score 1: D:\Docs\report.txt                 ← matches "report" only
Score 1: D:\Docs\invoice.pdf                ← matches "pdf" only
```

---

## 🧠 Algorithms & Data Structures

| Component | Algorithm / Structure | Complexity |
|-----------|----------------------|------------|
| **Index** | Inverted index (`unordered_map<string, unordered_set<string>>`) | O(1) lookup |
| **Tokenization** | Delimiter-based character scanner | O(n) per filename |
| **Scored Search** | Union + frequency counting | O(Q × P) |
| **Intersection Search** | Multi-set intersection (AND) | O(Q × R) |
| **Ranking** | `std::sort` with descending comparator | O(R log R) |
| **Persistence** | SQLite prepared statements + transactions | O(N) batch insert |

Where Q = query tokens, P = paths per token, R = result count, N = total index entries.

---

## 🛣️ Roadmap

- [ ] **Configurable scan paths** via CLI arguments
- [ ] **CMake build system** for reproducible builds
- [ ] **TF-IDF scoring** for better ranking
- [ ] **Fuzzy matching** for typo tolerance
- [ ] **File metadata indexing** (size, date, extension)
- [ ] **Incremental re-indexing** (only scan changed files)
- [ ] **Auto-complete suggestions** using Trie
- [ ] **Boolean query syntax** (`AND`, `OR`, `NOT`)
- [ ] **Result pagination**
- [ ] **Full-text content search** (inside PDFs, DOCX, TXT)
- [ ] **Cross-platform support** (Linux, macOS)
- [ ] **Web-based GUI**

---

## 🧪 Experiments

The `experiments/` folder contains standalone algorithm prototypes explored during development:

| File | Description |
|------|-------------|
| `trie.cpp` | Trie data structure with insert, search, and prefix-collection |
| `intersection.cpp` | Set-intersection algorithm for AND-logic keyword matching |
| `neural.cpp` | Multi-variable linear regression (gradient descent) for salary prediction |
| `tokenization_testing.cpp` | Tokenizer validation with test cases |
| `sqllite.cpp/` | SQLite C API integration prototype |

---

## 🔧 Tech Stack

| Technology | Purpose |
|-----------|---------|
| **C++17** | Core language (`std::filesystem`, modern STL) |
| **SQLite 3** | Embedded database for index persistence |
| **STL Containers** | `unordered_map`, `unordered_set`, `vector` for in-memory index |
| **std::filesystem** | Cross-platform directory traversal |

---

## 📝 Lessons Learned

- **Inverted indexes** are the backbone of all search engines — from Google to Elasticsearch
- **SQLite transactions** improve batch insert throughput by 100×+ over individual inserts
- **Tokenization quality** directly determines search accuracy
- **Dependency injection** (passing the index by const reference) keeps modules loosely coupled
- Evolving from a monolithic procedural design to **modular OOP** dramatically improves maintainability

---

## 🤝 Contributing

Contributions are welcome! Here's how to get started:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

---

<p align="center">
  <sub>Built with ❤️ in C++17</sub><br/>
  <sub>⭐ Star this repo if you found it useful!</sub>
</p>
