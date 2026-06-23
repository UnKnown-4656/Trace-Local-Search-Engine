# Universal Search Engine: TODO & Roadmap

This document outlines the tasks required to evolve the **Mini Search Engine** from a local CLI filename-indexing tool into a **Universal Search Engine Platform**. 

The goal is to build a high-performance, embedded or server-based engine that can index both **local files** and **arbitrary web/e-commerce catalog data**, exposing search queries via a clean **JSON HTTP API** with client SDKs for **Node.js (NPM)** and **Python**.

---

## 🎯 High-Level Vision
```
┌──────────────────────────────────────────────────────────────┐
│                  Universal Search Engine (C++)               │
│  - Inverted Index    - BM25 Relevance      - Multi-Field     │
│  - SQLite Storage    - Typo Tolerance      - Filter Queries  │
└──────────────────────────────┬───────────────────────────────┘
                               │
                       [ HTTP REST API ]
                               │
      ┌────────────────────────┼────────────────────────┐
      ▼                        ▼                        ▼
┌───────────┐            ┌───────────┐            ┌───────────┐
│ Node/NPM  │            │  Python   │            │ Local App │
│ Client    │            │  Client   │            │ Watcher   │
└───────────┘            └───────────┘            └───────────┘
```

---

## 📂 Tasks Checklist

### 1. ⚙️ Core Search Engine Enhancements (C++ Core)
- [ ] **Multi-Field Document Indexing**:
  - Refactor from indexing just a single path string to indexing structured documents containing arbitrary fields (e.g., `id`, `title`, `description`, `category`, `price`, `url`).
  - Store fields in a document table in SQLite and create separate posting lists per field.
- [ ] **Okapi BM25 Ranking Model**:
  - Replace simple token frequency count with Okapi BM25 relevance scoring.
  - Track document lengths and average document length across fields.
  - Calculate term frequency (TF) and inverse document frequency (IDF) dynamically.
- [ ] **Fuzzy Search & Typo Tolerance**:
  - Implement Levenshtein Distance or Jaro-Winkler algorithms to handle spelling mistakes.
  - Build a prefix/dictionary index using a Trie data structure to support fast prefix queries (auto-complete).
- [ ] **Improved Tokenization**:
  - Fix the CamelCase acronym splitting limitation (e.g., preventing `NAME` from being split into `["n", "a", "m", "e"]`).
  - Implement alphanumeric token splitting (e.g., `python3` -> `["python", "3"]`).
- [ ] **Synonyms & Stemming**:
  - Add support for synonym mapping (e.g., `cellphone` maps to `mobile`, `phone`).
  - Integrate a stemming algorithm (like the Porter Stemmer) to reduce words to their base form (e.g., `running`, `runs` -> `run`).
- [ ] **Stop-Word Filtering**:
  - Create a configurable stop-word blacklist (e.g., `the`, `a`, `and`, `of`) to avoid indexing highly frequent but meaningless tokens.
- [ ] **Filtering & Faceting (E-Commerce Use Case)**:
  - Support range filters (e.g., `price >= 10.00 AND price <= 50.00`).
  - Support exact attribute match filtering (e.g., `category = "Electronics"`).
- [ ] **Result Pagination**:
  - Implement offset/limit or cursor-based pagination to return results in pages instead of all at once.

---

### 2. 🌐 REST API & Daemon Layer
- [ ] **HTTP Server Integration**:
  - Integrate a lightweight C++ HTTP framework (such as `cpp-httplib`, `Crow`, or `oatpp`).
- [ ] **JSON Payload Serialization**:
  - Integrate `nlohmann/json` to handle query and index request/response serialization.
- [ ] **REST Endpoint Implementation**:
  - `POST /v1/indexes/:index_name/documents` - Batch index documents.
  - `POST /v1/indexes/:index_name/search` - Query the index with parameters for search term, query fields, filters, and pagination.
  - `DELETE /v1/indexes/:index_name/documents/:id` - Delete a document by ID.
  - `GET /v1/health` - Check service status, memory footprint, and document counts.
- [ ] **Multi-Index Support**:
  - Allow the creation of separate independent indexes (e.g., a `local_files` index and an `ecom_products` index) inside the same database or as separate database files.

---

### 3. 📦 Node.js / NPM Package Support
- [ ] **HTTP Client Library**:
  - Create a TypeScript client package `universal-search-node` to consume the API.
  - Support basic index operations, searches, and configurations.
- [ ] **Embedded Native Bindings (Optional)**:
  - Use `Node-API` (`napi`) to compile the C++ search engine directly into a native `.node` addon.
  - Allow Node.js/JavaScript developers to use the search engine locally as an in-process library without running the external C++ HTTP server.

---

### 4. 🐍 Python Client SDK
- [ ] **HTTP Client SDK**:
  - Create a Python package `universal-search-python` (installable via `pip`).
  - Build clean client wrappers to interact with the HTTP API endpoints.
  - Provide helper methods to upload and index pandas DataFrames directly.
- [ ] **Native Python Extension (Optional)**:
  - Create native bindings using `pybind11` to package the search engine as a binary extension module.
  - Enable fast, in-memory, single-process search in Python.

---

### 5. 🖥️ Local File Finder Enhancements
- [ ] **Real-Time Filesystem Watcher**:
  - Integrate OS-specific watcher notifications (e.g., `ReadDirectoryChangesW` on Windows, `inotify` on Linux, `FSEvents` on macOS) to update the index database dynamically as files are created, renamed, or deleted.
- [ ] **Incremental Indexing**:
  - Compare file modification timestamps (`last_write_time`) to skip re-indexing unchanged files.
- [ ] **Cross-Platform Path Standardization**:
  - abstract OS-specific directory structures and separators to support macOS, Windows, and Linux.

---

### 6. 🧪 Engineering & DevOps
- [ ] **CMake Build System**:
  - Maintain a clean `CMakeLists.txt` build configuration to compile the core engine, the daemon, and test binaries seamlessly on all major platforms.
- [ ] **Automated Testing Suite**:
  - Set up GoogleTest (GTest) for automated unit and integration tests.
  - Write test coverage for the Tokenizer, BM25 scoring, SQLite indexing, and HTTP endpoints.
- [ ] **Search Benchmarks**:
  - Build standard performance benchmarks to measure query latency (ms), index size (MB), and throughput (queries/sec) for varying datasets (e.g., 100k, 1M products).
- [ ] **CI/CD Pipeline**:
  - Add GitHub Actions to auto-run tests and compile binary releases for Windows, Linux, and macOS.
