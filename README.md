# ArabiMorph — Arabic Morphological Engine

> A desktop application for Arabic morphological analysis, built with **C++ and Qt**. It combines an **AVL Binary Search Tree** for root storage with a **Hash Table** for morphological schemes, enabling generation, validation, and visualization of Arabic word derivations.

---

##  Table of Contents

- [Overview](#overview)
- [Prerequisites & Installation](#prerequisites--installation)
- [Building the Project](#building-the-project)
- [Application Features](#application-features)
- [Data Files](#data-files)
- [Architecture](#architecture)

---

## Overview

ArabiMorph lets you:
- Store and manage Arabic **trilateral roots** (جذور) in a self-balancing AVL tree
- Load and manage **morphological schemes** (أوزان) in a hash table
- **Generate** derived Arabic words by applying a scheme to a root
- **Validate** whether a word morphologically belongs to a given root
- **Visualize** the AVL tree structure interactively in a graphical interface

---

## Prerequisites & Installation

### System Requirements

| Requirement | Version |
|-------------|---------|
| Operating System | Windows 10/11, Linux (Ubuntu 20.04+), macOS 12+ |
| C++ Compiler | GCC 9+ / MSVC 2019+ / Clang 10+ with C++17 support |
| Qt Framework | Qt 5.15+ or Qt 6.x |
| Build System | qmake (included with Qt) or CMake 3.16+ |

### Step 1 — Install Qt

**Windows:**
Download the Qt Online Installer from [https://www.qt.io/download](https://www.qt.io/download) and install the **Qt 6.x** or **Qt 5.15 LTS** component along with the MinGW or MSVC compiler kit.

**Linux (Ubuntu/Debian):**
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev g++ make
# or for Qt5:
sudo apt install qtbase5-dev qttools5-dev-tools g++ make
```

**macOS:**
```bash
brew install qt
export PATH="/opt/homebrew/opt/qt/bin:$PATH"
```

### Step 2 — Verify Compiler

```bash
g++ --version      # Should show GCC 9+ or higher
qmake --version    # Should show Qt version
```

---

## Building the Project

### Using qmake (Recommended)

```bash
# 1. Clone or extract the project
cd ARABIMORPH

# 2. Generate the Makefile
qmake arabic_morph.pro

# 3. Compile
make -j4           # Linux/macOS
mingw32-make       # Windows with MinGW

# 4. Run the application
./ARABIMORPH       # Linux/macOS
ARABIMORPH.exe     # Windows
```

### Using Qt Creator (GUI Method)

1. Open **Qt Creator**
2. Go to **File → Open File or Project**
3. Select `arabic_morph.pro`
4. Choose your compiler kit and click **Configure Project**
5. Press **Ctrl+R** (or the green ▶ button) to build and run

---

## Application Features

### 1.  Insert a Root (إدخال جذر)

Adds an Arabic trilateral root into the **AVL Binary Search Tree**.

**How to use:**
- Type the Arabic root (e.g., `كتب`, `درس`, `علم`) into the root input field
- Click **Insert** or press Enter
- The tree automatically **self-balances** using AVL rotations

**Output:**
- The root appears as a node in the visual tree panel on the right
- The node displays the root text and a depth badge (`d:N`)
- A green dot `●` appears on the node once it accumulates derivatives

---

### 2.  Delete a Root (حذف جذر)

Removes a root and all its derivative data from the AVL tree.

**How to use:**
- Enter the exact root string in the input field
- Click **Delete**

**Output:**
- The node is removed from the tree
- The tree re-balances automatically — surrounding nodes may shift positions in the visualization

---

### 3.  Search a Root (البحث عن جذر)

Checks whether a root exists in the AVL tree.

**How to use:**
- Enter the root to search for
- Click **Search**

**Output:**
- ✅ Found: the matching node is **highlighted** (selected, glowing green) in the tree visualization; its derivatives are shown in the info panel
- ❌ Not found: a "not found" message is displayed

---

### 4.  Generate Derivatives (توليد المشتقات)

Takes a root and a scheme, and produces the corresponding Arabic word.

**How to use:**
1. Make sure the root is already inserted in the tree
2. Enter the root (e.g., `كتب`)
3. Enter the scheme (e.g., `فَاعِل`) — it must be loaded in the hash table
4. Click **Generate**

**How it works — a simple example:**

Every scheme is a pattern built around three placeholder positions — one for each letter of the root. When you generate, the engine swaps those placeholders for the real root letters:

```
Root:    ك  ت  ب
         ↓  ↓  ↓
Scheme:  فَ اعِ لْ   →   كَاتِبْ
         1st 2nd 3rd
```

Another example with a different scheme:

```
Root:    ك  ت  ب
         ↓  ↓  ↓
Scheme:  مَفعُول   →   مَكتُوب
```

The engine simply reads the scheme's pattern, finds where each root letter goes, plugs them in, and returns the finished word.

**Output:**
- The generated word is shown on screen
- It is saved as a derivative of that root, with a frequency counter that increments each time it is generated
- The green dot on the tree node updates to reflect the new derivative count

---

### 5.  Validate a Word (التحقق من الكلمة)

Checks whether a given Arabic word morphologically derives from a given root by testing all known schemes.

**How to use:**
- Enter the word to validate
- Enter the root to check against
- Click **Validate**

**Output:**
- **OUI (نعم):** The word matches — displays the matching scheme name; the word is stored as a derivative in the AVL tree
- **NON (لا):** No scheme in the hash table produces this word from this root

---

### 6.  Load Roots from File (تحميل الجذور من ملف)

Bulk-imports Arabic roots from a plain text file (one root per line) into the AVL tree.

**How to use:**
- Click **Load Roots File**
- Select a `.txt` file (e.g., `data/Data.txt`)

**File format:**
```
كتب
درس
علم
فتح
```

**Output:**
- All roots are inserted into the AVL tree and displayed in the visualization panel

---

### 7.  Load Schemes from File (تحميل الأوزان من ملف)

Bulk-imports morphological schemes from a text file into the hash table.

**How to use:**
- Click **Load Schemes File**
- Select a `.txt` file (e.g., `data/c.txt`)

**File format** (one scheme per line, written with the Arabic root letters ف ع ل as placeholders):
```
فَعَلَ
مَفعُول
فَاعِل
فِعَال
```

The engine automatically converts ف → `root[0]`, ع → `root[1]`, ل → `root[2]` and stores the algorithm internally.

**Output:**
- Schemes are loaded into the hash table and are immediately available for generation and validation

---

### 8.  View All Derivatives of a Root (عرض المشتقات)

Displays all stored derivatives for a selected root and their frequency counts.

**How to use:**
- Click on a node in the tree visualization **or** search for a root
- The info/detail panel shows all derivatives with their usage frequency

**Output:**
```
Root: كتب
Derivatives:
  - كَتَبَ  (frequency: 2)
  - كَاتِب  (frequency: 1)
  - مَكتُوب (frequency: 1)
```

---

### 9.  AVL Tree Visualization (التصوير البياني للشجرة)

An interactive graphical view of the AVL Binary Search Tree, rendered in a dark-themed canvas.

**Features:**
- **Scroll to zoom** — use the mouse wheel to zoom in/out (20% to 300%)
- **Click a node** — selects it with a green glow; shows root details and derivatives
- **Hover a node** — highlights it in blue with a pointer cursor
- **Zoom buttons** — use the `+` / `−` / Reset buttons in the toolbar
- **Depth badge** — each node shows `d:N` where N is its depth from the tree root (root = 1)
- **Green dot** — appears on nodes that have stored derivatives; the number inside is the derivative count
- **Curved edges with arrows** — show parent–child relationships

---

### 10.  Morphological Family View (العائلة الصرفية)

Displays all roots that have a validated derivative under a specific scheme — effectively showing the "morphological family" of a given pattern.

**How to use:**
- Enter a scheme name
- Click **Show Family**

**Output:**
```
Morphological Family | Scheme: فَاعِل
  Root: كتب  →  Word: كَاتِب  (freq: 1)
  Root: درس  →  Word: دَارِس  (freq: 2)
  Root: علم  →  Word: عَالِم  (freq: 1)
```

---

## Data Files

The `data/` folder contains sample input files:

| File | Purpose |
|------|---------|
| `data/Data.txt` | Sample Arabic roots, one per line |
| `data/c.txt` | Sample morphological schemes (written with ف ع ل placeholders) |

You can replace or extend these files with your own data following the same format.

---

## Architecture

The project is built on two core data structures that work together:

```
Hash Table (Schemes)          AVL Binary Search Tree (Roots)
─────────────────────         ──────────────────────────────
  "فَعَلَ" → algo string   ←→   Node("كتب")
  "مَفعُول" → algo string       ├── derivatives: {كَتَبَ:2, مَكتُوب:1}
  "فَاعِل" → algo string        ├── left child
  ...                           └── right child (balanced)
```

| Component | File | Role |
|-----------|------|------|
| `BinarySearchTree` | `BinarySearchTree.cpp` | Self-balancing AVL tree for roots |
| `Node` | `Node.cpp` | AVL tree node holding a Root object |
| `Root` | `Root.cpp` | Stores root string + derivatives map |
| `hashmap` / `node` | `hashtable.cpp` | Hash table for scheme storage |
| `core_engine` | `core_engine.cpp` | `apply_root`, `generate`, `validate` logic |
| `MainWindow` | `MainWindow.cpp` | Qt main window and UI event handling |
| `TreeVisualizationWidget` | `TreeVisualizationWidget.cpp` | Interactive AVL tree painter widget |
