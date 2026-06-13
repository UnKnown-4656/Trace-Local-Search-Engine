# Project Roadmap: Mini Search Engine

This document outlines the evolutionary roadmap for the **Mini Search Engine** C++ project. It charts a realistic, step-by-step path for a single developer to build, refine, and scale this local indexing engine, prioritizing software craftsmanship, engineering fundamentals, and information retrieval (IR) concepts.

---

## 👁️ Project Vision

The **Mini Search Engine** is a self-educational local indexing utility. Today, it serves as a lightweight console tool that indexes file names in a specified folder and stores them in a flat SQLite schema to enable keyword search. 

The goal is to evolve this prototype into a robust, high-performance local daemon service that:
1. Indexes both **file metadata** and **unstructured text content** recursively.
2. Performs ranking based on modern **information retrieval models** (BM25) rather than simple token counts.
3. Operates as a background service exposing a clean **JSON HTTP API** for client queries.
4. Remains lightweight and educational, serving as a clean portfolio project showcasing systems programming, database design, and network communication in C++.

---

## 📊 Current Status

The features listed below are verified to exist in the current codebase:

* **Recursive Directory Traversal**: Scans folders recursively using C++17 `std::filesystem::directory_iterator` inside `Indexer::ScanFiles()`, with active try-catch blocks to skip protected directories.
* **Unified Delimiter-Based Tokenizer**: A single tokenization helper `Tokenize()` in `utils.cpp` that extracts tokens from filenames and queries by splitting on `_`, `.`, `-`, spaces, `(`, `)`, `[`, and `]`.
* **In-Memory Inverted Index**: Holds the vocabulary map in memory using `std::unordered_map<std::string, std::unordered_set<std::string>>` to provide $O(1)$ lookup speeds.
* **SQLite Persistence (Save & Load)**: Uses an embedded SQLite database (`index.db`) via prepared statements and transactional batch writes (`BEGIN TRANSACTION` / `COMMIT`) to speed up index serializations. Loads data back into memory via SQL row-by-row callbacks.
* **Query Processing (Scored Union)**: Normalizes query strings to lowercase, splits them into query tokens, and aggregates results.
* **Relevance Ranking (Token Count)**: Sorts search results using a standard descending comparator based on the absolute number of query tokens matching a file path.
* **Unexposed Exact Intersection (AND) Search**: Implements a set-intersection algorithm (`searchwithintersection()`) within the search engine module, though it is not wired to the interactive console loop.

---

## ⚙️ Phase 0 – Core Stability

*Focus: Clean up technical debt, improve the build experience, and make the tokenization system robust.*

* **Eliminate Header Namespace Pollution**: Remove `using namespace std;` from `sql_indexer.h` and `SearchEngine.h`. Replace all unqualified std types with explicit `std::` naming to adhere to professional C++ styling guidelines.
* **Optimize Parameter Passing**: Refactor signature interfaces (e.g., `save_index`, `load_index`, `search`) to pass string parameters by `const std::string&` (or `std::string_view`) rather than copying strings by value.
* **Fix Unsafe Lookup Exception Risk**: Refactor `searchwithintersection` to look up elements using `files.find()` instead of throwing `std::out_of_range` via map `.at()` queries on missing keywords.
* **Advanced Delimiter Tokenization**: Upgrade the core tokenizer to handle CamelCase splitting (e.g., `InvoiceManager` → `["invoice", "manager"]`) and alphanumeric splits (e.g., `python3` → `["python", "3"]`).
* **CMake Build Integration**: Add a root-level `CMakeLists.txt` file to compile the project seamlessly across different operating systems and compilers (GCC, MSVC, Clang).
* **Unit Testing Foundations**: Integrate a simple test runner to assert tokenizer output, ranking order correctness, and database consistency under simulated inputs.
* **Search Execution Statistics**: Update the CLI output to show runtime execution stats (e.g., query execution time in milliseconds, total files matched).

---

## 🔍 Phase 1 – Search Quality

*Focus: Transition the matching algorithm from simple token counting to standard information retrieval scoring.*

* **Term Frequency-Inverse Document Frequency (TF-IDF)**:
  * *Why*: Basic token counts treat common words (like "pdf" or "notes") with the same weight as rare, highly descriptive words. TF-IDF will discount common terms across the index, raising the ranking of files matching highly specific keywords.
* **Okapi BM25 Ranking Model**:
  * *Why*: The industry standard for classical keyword search. It builds upon TF-IDF by adding term frequency saturation (preventing a single term from inflating scores indefinitely) and document length normalization (preventing long filenames from ranking higher simply because they contain more words).
* **Exact Phrase Search**:
  * *Why*: Allows users to query terms wrapped in quotes (e.g., `"final report"`) to ensure matches only appear if the terms are found side-by-side in that exact order.
* **Fuzzy Matching & Edit Distance**:
  * *Why*: Incorporate Levenshtein distance calculations to handle spelling typos in queries (e.g., searching "reciept" will still find "receipt").
* **Stop-Word Filtering**:
  * *Why*: Create a blacklist of highly common English tokens (e.g., "a", "an", "the", "of", "and") to avoid cluttering the index database with terms that carry no search value.

---

## 📄 Phase 2 – Content Indexing

*Focus: Move beyond indexing filenames to parsing and searching the text content inside local documents.*

```
 ┌──────────────────────┐      ┌──────────────────────┐      ┌─────────────────────┐
 │    Local Document    │─────▶│   Content Extractor  │─────▶│   Inverted Index    │
 │ (txt, md, csv, pdf)  │ read │ (std::ifstream, etc) │ map  │ (token -> doc_id)   │
 └──────────────────────┘      └──────────────────────┘      └─────────────────────┘
```

* **Pluggable File Readers**: Design a virtual `FileReader` interface with custom implementations for different file extensions.
* **Plain Text Indexing**: Build an extractor using standard C++ file streams (`std::ifstream`) to index the body content of `.txt`, `.md`, and `.csv` files.
* **Document Chunking & Storage**: Instead of treating files as single large strings, divide file content into sections (chunks) during indexing to allow returning search results with contextual text snippets.
* **PDF Text Extraction**: Wrap a lightweight open-source C parser (e.g., a minimal wrapper around `Poppler` or `MuPDF`) to read and index text streams within local PDF documents.

---

## 🔄 Phase 3 – Metadata & Incremental Updates

*Focus: Optimize index updates and store file properties for advanced filtering.*

* **Metadata Indexing**: Expand the index schema to extract and record file size, file extension, and modification timestamps.
* **Filter Queries**: Support filtering searches by file properties (e.g., `ext:pdf`, `size > 10MB`, `date:last_week`).
* **Incremental Crawling**: Compare file modification timestamps (`std::filesystem::last_write_time`) against the database records to skip parsing files that have not changed since the last index run.
* **Stale Entry Pruning**: Implement a scanner to detect and delete records of files that have been moved, renamed, or deleted from disk, resolving index drift.
* **Filesystem Monitoring (Watch Service)**: Integrate a platform watcher (like `ReadDirectoryChangesW` on Windows or `inotify` on Linux) to dynamically update the SQLite database in real-time as files change, eliminating manual rescans.

---

## 🌐 Phase 4 – Service & API Layer

*Focus: Expose search operations over an HTTP service, decoupling the query engine from terminal input.*

* **Embedded HTTP Server**: Integrate a lightweight, header-only C++ web server (e.g., `cpp-httplib` or `Crow`) to let the application run continuously as a background service daemon.
* **JSON API Payload Handling**: Integrate `nlohmann/json` to handle structured queries and output responses.
* **REST API Endpoints**:
  * `GET /search?q=query&limit=20&ext=txt` - Performs searches and returns ranked matches in JSON format.
  * `GET /status` - Returns statistics (total files indexed, index size on disk, active directories).
  * `POST /index/rescan` - Triggers a background directory crawl.
* **Authentication**: Protect management endpoints with simple API key verification.

---

## 🖥️ Phase 5 – User Interfaces

*Focus: Build clean client programs to interact with the engine daemon.*

* **Lightweight CLI Client**: Create a separate console utility (`search-cli`) that queries the server API using simple HTTP requests. This provides instant results without loading database libraries in the client process.
* **Web UI Dashboard**: Write a responsive single-page web application (HTML5, CSS, JS) that connects to the local API. It will show search results, file previews, and indexing status.
* **Desktop Tray App**: Use a lightweight container framework (like Tauri) to wrap the web interface and run the C++ engine service inside the system tray.

---

## 🔬 Phase 6 – Advanced Search Engine Concepts

*Focus: Optimize index representations to scale storage and processing performance.*

* **Document ID Mapping**: Stop storing file path strings inside the token sets. Build a normalized lookup table mapping each unique file path to a 32-bit integer `DocID`.
* **Posting Lists Optimization**: Store tokens mapped to sorted lists of `DocIDs` (posting lists) to make intersection (`AND`) and union (`OR`) merge operations extremely fast using two-pointer algorithms.
* **Index Compression**: Implement compression for posting lists (such as variable-byte encoding or delta compression) to shrink the RAM and disk footprints.
* **Persistent Disk Lookups**: Transition from loading the entire index in memory to queryable B-Trees on disk, caching only hot terms.

---

## 🎓 Learning Goals

Each phase of this roadmap is designed to teach essential concepts in systems engineering and computer science:

| Phase | Core Concepts Taught | Applied Skills |
| :--- | :--- | :--- |
| **Phase 0** | Code Hygiene, Modular Design, Dependency Management | C++ header design, CMake configurations, Unit testing |
| **Phase 1** | Information Retrieval Theory, Probability Models | Vector Space Models, TF-IDF, BM25 scoring, Edit Distance |
| **Phase 2** | Text Parsing, Interface Design, Third-party Integrations | Stream parsers, OOP abstraction, C/C++ library wrapping |
| **Phase 3** | OS File Systems, Asynchronous Events, Incremental Crawls | Metadata management, OS file notifications, State sync |
| **Phase 4** | Web Protocols, Client-Server Decoupling, Serialization | HTTP REST API, JSON structures, Daemon architecture |
| **Phase 5** | Front-end Design, System Integrations, Client Utilities | Web development, IPC, Desktop application bundling |
| **Phase 6** | Low-Level Data Structures, Memory Profiling, Storage Savings | Posting lists, Variable-byte compression, Memory profiling |

---

## 🚫 Out of Scope

To ensure this project remains manageable and focused on search engine fundamentals, the following goals are explicitly excluded:

* **Distributed Indexing**: No multi-node search, network clustering, or server synchronization (the engine is strictly local-first).
* **Enterprise Security Policy Integration**: No complex user access control lists (ACLs) or enterprise authentication integrations (LDAP, Active Directory).
* **AI-Assisted Neural / Semantic Search**: Exclude large language models (LLMs), vector databases, and heavy machine learning embeddings to focus on classical keyword indexing.
* **Web Crawling**: No crawling of internet web pages or external websites. The engine is limited to the local filesystem.
