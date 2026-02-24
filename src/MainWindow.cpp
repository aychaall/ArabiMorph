#include "../include/MainWindow.h"
#include <QApplication>
#include <QScrollBar>
#include <QDateTime>
#include <QTextStream>
#include <QString>
#include <QHeaderView>
#include <QGridLayout>
#include <QSizePolicy>

// ─────────────────────────────────────────────────────────────────────────────
//  Constructor & UI Setup
// ─────────────────────────────────────────────────────────────────────────────

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    m_tree    = new BinarySearchTree();
    m_hashmap = new struct hashmap();
    set_hashmap(m_hashmap, 10000);

    setupStyleSheet();
    setupUI();
    setWindowTitle("محرك الصرف العربي  |  Arabic Morphological Engine");
    resize(1280, 800);
    setMinimumSize(900, 600);
}

MainWindow::~MainWindow() {
    delete m_tree;
    delete m_hashmap;
}

void MainWindow::setupStyleSheet() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #0d1117;
        }
        QTabWidget::pane {
            border: 1px solid #21262d;
            background: #0d1117;
            border-radius: 8px;
        }
        QTabBar::tab {
            background: #161b22;
            color: #8b949e;
            padding: 10px 22px;
            border: 1px solid #21262d;
            border-bottom: none;
            border-radius: 6px 6px 0 0;
            font-size: 13px;
            font-family: 'Segoe UI', Arial;
            margin-right: 3px;
        }
        QTabBar::tab:selected {
            background: #1f2937;
            color: #e6edf3;
            border-color: #388bfd;
            border-bottom: 2px solid #388bfd;
        }
        QTabBar::tab:hover:!selected {
            background: #1c2128;
            color: #c9d1d9;
        }
        QGroupBox {
            border: 1px solid #21262d;
            border-radius: 8px;
            margin-top: 16px;
            padding: 12px;
            color: #8b949e;
            font-size: 11px;
            font-family: 'Segoe UI', Arial;
            background: #0d1117;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 6px;
            color: #58a6ff;
            font-weight: bold;
            font-size: 11px;
        }
        QLineEdit {
            background: #161b22;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 8px 12px;
            color: #e6edf3;
            font-size: 14px;
            font-family: 'Arial', sans-serif;
            selection-background-color: #388bfd;
        }
        QLineEdit:focus {
            border-color: #388bfd;
            background: #1c2433;
        }
        QLineEdit::placeholder {
            color: #484f58;
        }
        QPushButton {
            background: #21262d;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 8px 16px;
            color: #c9d1d9;
            font-size: 12px;
            font-family: 'Segoe UI', Arial;
            font-weight: 500;
        }
        QPushButton:hover {
            background: #30363d;
            border-color: #8b949e;
            color: #e6edf3;
        }
        QPushButton:pressed {
            background: #0d1117;
        }
        QPushButton#btnPrimary {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #1f6feb, stop:1 #1158c7);
            border-color: #388bfd;
            color: white;
            font-weight: bold;
        }
        QPushButton#btnPrimary:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #388bfd, stop:1 #1f6feb);
        }
        QPushButton#btnSuccess {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #238636, stop:1 #196127);
            border-color: #2ea043;
            color: white;
            font-weight: bold;
        }
        QPushButton#btnSuccess:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #2ea043, stop:1 #238636);
        }
        QPushButton#btnDanger {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #b91c1c, stop:1 #991b1b);
            border-color: #ef4444;
            color: white;
            font-weight: bold;
        }
        QPushButton#btnDanger:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #dc2626, stop:1 #b91c1c);
        }
        QPushButton#btnWarning {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #d97706, stop:1 #b45309);
            border-color: #f59e0b;
            color: white;
            font-weight: bold;
        }
        QPushButton#btnWarning:hover {
            background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
                stop:0 #f59e0b, stop:1 #d97706);
        }
        QTextEdit {
            background: #0d1117;
            border: 1px solid #21262d;
            border-radius: 6px;
            padding: 8px;
            color: #e6edf3;
            font-family: 'Courier New', monospace;
            font-size: 12px;
            selection-background-color: #388bfd;
        }
        QListWidget {
            background: #161b22;
            border: 1px solid #21262d;
            border-radius: 6px;
            color: #e6edf3;
            font-family: Arial;
            font-size: 13px;
        }
        QListWidget::item {
            padding: 6px 10px;
            border-bottom: 1px solid #21262d;
        }
        QListWidget::item:selected {
            background: #1f6feb;
            color: white;
        }
        QListWidget::item:hover {
            background: #1c2128;
        }
        QTableWidget {
            background: #0d1117;
            border: 1px solid #21262d;
            border-radius: 6px;
            color: #e6edf3;
            gridline-color: #21262d;
            font-size: 12px;
            selection-background-color: #1f6feb;
        }
        QTableWidget::item {
            padding: 6px;
        }
        QHeaderView::section {
            background: #161b22;
            color: #8b949e;
            border: none;
            border-bottom: 1px solid #30363d;
            border-right: 1px solid #21262d;
            padding: 8px;
            font-weight: bold;
            font-size: 11px;
        }
        QScrollBar:vertical {
            background: #0d1117;
            width: 8px;
            border-radius: 4px;
        }
        QScrollBar::handle:vertical {
            background: #30363d;
            border-radius: 4px;
            min-height: 20px;
        }
        QScrollBar::handle:vertical:hover { background: #484f58; }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }
        QScrollBar:horizontal {
            background: #0d1117;
            height: 8px;
        }
        QScrollBar::handle:horizontal {
            background: #30363d;
            border-radius: 4px;
        }
        QLabel {
            color: #8b949e;
            font-family: 'Segoe UI', Arial;
        }
        QLabel#title {
            color: #e6edf3;
            font-size: 16px;
            font-weight: bold;
        }
        QLabel#stat {
            color: #58a6ff;
            font-size: 13px;
            font-weight: bold;
        }
        QStatusBar {
            background: #161b22;
            color: #8b949e;
            border-top: 1px solid #21262d;
            font-size: 11px;
        }
        QSplitter::handle {
            background: #21262d;
            width: 2px;
            height: 2px;
        }
        QScrollArea {
            border: none;
            background: transparent;
        }
    )");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Main UI Layout
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Header bar
    QFrame* header = new QFrame();
    header->setFixedHeight(56);
    header->setStyleSheet("background: #161b22; border-bottom: 1px solid #21262d;");
    QHBoxLayout* hdrLayout = new QHBoxLayout(header);
    hdrLayout->setContentsMargins(20, 0, 20, 0);

    QLabel* headerTitle = new QLabel("  محرك الصرف العربي  |  Arabic Morphological Engine");
    headerTitle->setObjectName("title");
    headerTitle->setStyleSheet("color: #e6edf3; font-size: 17px; font-weight: bold;");

    QLabel* badge = new QLabel("AVL + HashTable");
    badge->setStyleSheet(
        "background: #1f3a5f; color: #58a6ff; border: 1px solid #388bfd;"
        "border-radius: 10px; padding: 2px 10px; font-size: 11px; font-weight: bold;");

    m_statusLabel = new QLabel("Ready");
    m_statusLabel->setStyleSheet("color: #2ea043; font-size: 11px;");
    m_statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    hdrLayout->addWidget(headerTitle);
    hdrLayout->addWidget(badge);
    hdrLayout->addStretch();
    hdrLayout->addWidget(m_statusLabel);
    mainLayout->addWidget(header);

    // Tabs
    m_tabs = new QTabWidget();
    m_tabs->setTabPosition(QTabWidget::North);
    mainLayout->addWidget(m_tabs);

    setupRootTab();
    setupSchemeTab();
    setupEngineTab();
    setupAboutTab();

    // Status bar
    statusBar()->showMessage("Arabic Morphological Engine ready — AVL Tree + Hash Table");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Tab 1: Root Management (AVL Tree)
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupRootTab() {
    QWidget* tab = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(tab);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    // ── Left panel: controls ──────────────────────────────────────────────────
    QWidget* leftPanel = new QWidget();
    leftPanel->setFixedWidth(280);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(10);

    // Input group
    QGroupBox* inputGroup = new QGroupBox("Root Operations  (الجذور)");
    QVBoxLayout* inputLayout = new QVBoxLayout(inputGroup);
    inputLayout->setSpacing(8);

    m_rootInput = new QLineEdit();
    m_rootInput->setPlaceholderText("أدخل الجذر  |  Enter Arabic root...");
    m_rootInput->setLayoutDirection(Qt::RightToLeft);
    m_rootInput->setFont(QFont("Arial", 14));
    m_rootInput->setMinimumHeight(40);
    inputLayout->addWidget(m_rootInput);

    auto* btnInsert = new QPushButton("⊕  Insert Root");
    btnInsert->setObjectName("btnSuccess");
    btnInsert->setMinimumHeight(36);
    auto* btnSearch = new QPushButton("⌕  Search Root");
    btnSearch->setObjectName("btnPrimary");
    btnSearch->setMinimumHeight(36);
    auto* btnDelete = new QPushButton("⊖  Delete Root");
    btnDelete->setObjectName("btnDanger");
    btnDelete->setMinimumHeight(36);
    auto* btnDisplay = new QPushButton("≡  Display All (in-order)");
    btnDisplay->setMinimumHeight(36);
    auto* btnDerivs = new QPushButton("⊞  Show Derivatives");
    btnDerivs->setMinimumHeight(36);
    auto* btnLoadFile = new QPushButton("⊘  Load from File");
    btnLoadFile->setMinimumHeight(36);

    inputLayout->addWidget(btnInsert);
    inputLayout->addWidget(btnSearch);
    inputLayout->addWidget(btnDelete);
    inputLayout->addWidget(btnDisplay);
    inputLayout->addWidget(btnDerivs);
    inputLayout->addWidget(btnLoadFile);
    leftLayout->addWidget(inputGroup);

    // Stats group
    QGroupBox* statsGroup = new QGroupBox("Tree Statistics");
    QGridLayout* statsGrid = new QGridLayout(statsGroup);
    statsGrid->setSpacing(6);

    statsGrid->addWidget(new QLabel("Nodes:"), 0, 0);
    m_statNodes = new QLabel("0");
    m_statNodes->setObjectName("stat");
    statsGrid->addWidget(m_statNodes, 0, 1);

    statsGrid->addWidget(new QLabel("Height:"), 1, 0);
    m_statHeight = new QLabel("0");
    m_statHeight->setObjectName("stat");
    statsGrid->addWidget(m_statHeight, 1, 1);

    statsGrid->addWidget(new QLabel("Empty:"), 2, 0);
    m_statEmpty = new QLabel("Yes");
    m_statEmpty->setObjectName("stat");
    statsGrid->addWidget(m_statEmpty, 2, 1);

    leftLayout->addWidget(statsGroup);

    // Derivatives panel
    QGroupBox* derivGroup = new QGroupBox("Node Derivatives  (المشتقات)");
    QVBoxLayout* derivLayout = new QVBoxLayout(derivGroup);
    m_selectedRootLabel = new QLabel("Click a node to inspect");
    m_selectedRootLabel->setStyleSheet("color: #58a6ff; font-style: italic; font-size: 11px;");
    derivLayout->addWidget(m_selectedRootLabel);
    m_derivativesList = new QListWidget();
    m_derivativesList->setMaximumHeight(160);
    derivLayout->addWidget(m_derivativesList);
    leftLayout->addWidget(derivGroup);

    leftLayout->addStretch();

    // ── Right panel: Log + Tree Visualizer ───────────────────────────────────
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(8);

    // Tree visualizer
    QGroupBox* vizGroup = new QGroupBox("AVL Tree Visualization");
    QVBoxLayout* vizLayout = new QVBoxLayout(vizGroup);
    vizLayout->setContentsMargins(4, 12, 4, 4);

    m_treeScroll = new QScrollArea();
    m_treeScroll->setWidgetResizable(true);
    m_treeScroll->setMinimumHeight(350);
    m_treeScroll->setStyleSheet("background: transparent; border: none;");

    // Zoom toolbar
    QHBoxLayout* zoomBar = new QHBoxLayout();
    zoomBar->setSpacing(4);
    auto* btnZoomIn  = new QPushButton("⊕ Zoom In");
    auto* btnZoomOut = new QPushButton("⊖ Zoom Out");
    auto* btnZoomRst = new QPushButton("↺ Reset Zoom");
    btnZoomIn->setMaximumWidth(100);
    btnZoomOut->setMaximumWidth(100);
    btnZoomRst->setMaximumWidth(110);
    btnZoomIn->setToolTip("Zoom in (also: scroll wheel)");
    btnZoomOut->setToolTip("Zoom out (also: scroll wheel)");
    btnZoomRst->setToolTip("Reset zoom to 100%");
    zoomBar->addWidget(btnZoomIn);
    zoomBar->addWidget(btnZoomOut);
    zoomBar->addWidget(btnZoomRst);
    zoomBar->addStretch();
    vizLayout->addLayout(zoomBar);

    m_treeViz = new TreeVisualizationWidget();
    m_treeViz->setTree(m_tree);
    m_treeScroll->setWidget(m_treeViz);
    vizLayout->addWidget(m_treeScroll);

    connect(btnZoomIn,  &QPushButton::clicked, m_treeViz, &TreeVisualizationWidget::zoomIn);
    connect(btnZoomOut, &QPushButton::clicked, m_treeViz, &TreeVisualizationWidget::zoomOut);
    connect(btnZoomRst, &QPushButton::clicked, m_treeViz, &TreeVisualizationWidget::resetZoom);
    rightLayout->addWidget(vizGroup, 3);

    // Log area
    QGroupBox* logGroup = new QGroupBox("Operation Log");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    m_rootLog = new QTextEdit();
    m_rootLog->setReadOnly(true);
    m_rootLog->setMaximumHeight(140);
    logLayout->addWidget(m_rootLog);
    rightLayout->addWidget(logGroup, 1);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    // Connect signals
    connect(btnInsert,   &QPushButton::clicked, this, &MainWindow::onInsertRoot);
    connect(btnSearch,   &QPushButton::clicked, this, &MainWindow::onSearchRoot);
    connect(btnDelete,   &QPushButton::clicked, this, &MainWindow::onDeleteRoot);
    connect(btnDisplay,  &QPushButton::clicked, this, &MainWindow::onDisplayAllRoots);
    connect(btnDerivs,   &QPushButton::clicked, this, &MainWindow::onShowDerivatives);
    connect(btnLoadFile, &QPushButton::clicked, this, &MainWindow::onLoadRootsFromFile);
    connect(m_treeViz,   &TreeVisualizationWidget::nodeClicked,
            this, &MainWindow::onTreeNodeClicked);
    connect(m_rootInput, &QLineEdit::returnPressed, this, &MainWindow::onInsertRoot);

    m_tabs->addTab(tab, "🌳  AVL Tree — Roots");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Tab 2: Scheme Management (Hash Table)
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupSchemeTab() {
    QWidget* tab = new QWidget();
    QHBoxLayout* mainLayout = new QHBoxLayout(tab);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    // Left controls
    QWidget* leftPanel = new QWidget();
    leftPanel->setFixedWidth(310);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setSpacing(10);

    // Insert/Search/Delete group
    QGroupBox* opsGroup = new QGroupBox("Scheme Operations  (الأوزان)");
    QVBoxLayout* opsLayout = new QVBoxLayout(opsGroup);
    opsLayout->setSpacing(8);

    m_schemeInput = new QLineEdit();
    m_schemeInput->setPlaceholderText("أدخل الوزن  |  Enter scheme (e.g. فاعل)...");
    m_schemeInput->setLayoutDirection(Qt::RightToLeft);
    m_schemeInput->setFont(QFont("Arial", 14));
    m_schemeInput->setMinimumHeight(40);
    opsLayout->addWidget(m_schemeInput);

    auto* btnInsertS = new QPushButton("⊕  Insert Scheme");
    btnInsertS->setObjectName("btnSuccess");
    btnInsertS->setMinimumHeight(36);
    auto* btnSearchS = new QPushButton("⌕  Search Scheme");
    btnSearchS->setObjectName("btnPrimary");
    btnSearchS->setMinimumHeight(36);
    auto* btnDeleteS = new QPushButton("⊖  Delete Scheme");
    btnDeleteS->setObjectName("btnDanger");
    btnDeleteS->setMinimumHeight(36);
    auto* btnDisplayS = new QPushButton("≡  Display All Schemes");
    btnDisplayS->setMinimumHeight(36);
    auto* btnLoadS = new QPushButton("⊘  Load from File");
    btnLoadS->setMinimumHeight(36);

    opsLayout->addWidget(btnInsertS);
    opsLayout->addWidget(btnSearchS);
    opsLayout->addWidget(btnDeleteS);
    opsLayout->addWidget(btnDisplayS);
    opsLayout->addWidget(btnLoadS);
    leftLayout->addWidget(opsGroup);

    // Update group
    QGroupBox* updateGroup = new QGroupBox("Update Scheme");
    QVBoxLayout* updateLayout = new QVBoxLayout(updateGroup);
    updateLayout->setSpacing(8);

    m_schemeOldInput = new QLineEdit();
    m_schemeOldInput->setPlaceholderText("Old scheme name...");
    m_schemeOldInput->setLayoutDirection(Qt::RightToLeft);
    m_schemeOldInput->setFont(QFont("Arial", 13));
    m_schemeNewInput = new QLineEdit();
    m_schemeNewInput->setPlaceholderText("New scheme name...");
    m_schemeNewInput->setLayoutDirection(Qt::RightToLeft);
    m_schemeNewInput->setFont(QFont("Arial", 13));

    auto* btnUpdate = new QPushButton("↺  Update Scheme");
    btnUpdate->setObjectName("btnWarning");
    btnUpdate->setMinimumHeight(36);

    updateLayout->addWidget(m_schemeOldInput);
    updateLayout->addWidget(m_schemeNewInput);
    updateLayout->addWidget(btnUpdate);
    leftLayout->addWidget(updateGroup);
    leftLayout->addStretch();

    // Right: table + log
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(8);

    QGroupBox* tableGroup = new QGroupBox("Scheme Database  (قاعدة الأوزان)");
    QVBoxLayout* tableLayout = new QVBoxLayout(tableGroup);
    m_schemeTable = new QTableWidget(0, 3);
    m_schemeTable->setHorizontalHeaderLabels({"Bucket", "Scheme  (الوزن)", "Algorithm  (الخوارزمية)"});
    m_schemeTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_schemeTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_schemeTable->setColumnWidth(0, 60);
    m_schemeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_schemeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLayout->addWidget(m_schemeTable);
    rightLayout->addWidget(tableGroup, 3);

    QGroupBox* logGroup = new QGroupBox("Operation Log");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    m_schemeLog = new QTextEdit();
    m_schemeLog->setReadOnly(true);
    m_schemeLog->setMaximumHeight(130);
    logLayout->addWidget(m_schemeLog);
    rightLayout->addWidget(logGroup, 1);

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel, 1);

    connect(btnInsertS,  &QPushButton::clicked, this, &MainWindow::onInsertScheme);
    connect(btnSearchS,  &QPushButton::clicked, this, &MainWindow::onSearchScheme);
    connect(btnDeleteS,  &QPushButton::clicked, this, &MainWindow::onDeleteScheme);
    connect(btnDisplayS, &QPushButton::clicked, this, &MainWindow::onDisplayAllSchemes);
    connect(btnLoadS,    &QPushButton::clicked, this, &MainWindow::onLoadSchemesFromFile);
    connect(btnUpdate,   &QPushButton::clicked, this, &MainWindow::onUpdateScheme);
    connect(m_schemeInput, &QLineEdit::returnPressed, this, &MainWindow::onInsertScheme);

    m_tabs->addTab(tab, "# Hash Table — Schemes");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Tab 3: Morphological Engine
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupEngineTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(tab);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    QHBoxLayout* topRow = new QHBoxLayout();
    topRow->setSpacing(12);

    // Generate word panel
    QGroupBox* genGroup = new QGroupBox("Generate Word  (توليد الكلمة)");
    QVBoxLayout* genLayout = new QVBoxLayout(genGroup);
    genLayout->setSpacing(8);

    QLabel* genDesc = new QLabel("Apply a morphological scheme to a root to generate a word.");
    genDesc->setWordWrap(true);
    genDesc->setStyleSheet("color: #6e7681; font-size: 11px; margin-bottom:4px;");
    genLayout->addWidget(genDesc);

    m_engRootInput = new QLineEdit();
    m_engRootInput->setPlaceholderText("Root  (الجذر)  e.g. كتب");
    m_engRootInput->setLayoutDirection(Qt::RightToLeft);
    m_engRootInput->setFont(QFont("Arial", 14));
    m_engRootInput->setMinimumHeight(40);

    m_engSchemeInput = new QLineEdit();
    m_engSchemeInput->setPlaceholderText("Scheme  (الوزن)  e.g. فاعل");
    m_engSchemeInput->setLayoutDirection(Qt::RightToLeft);
    m_engSchemeInput->setFont(QFont("Arial", 14));
    m_engSchemeInput->setMinimumHeight(40);

    auto* btnGenerate = new QPushButton("⚡  Generate Word");
    btnGenerate->setObjectName("btnPrimary");
    btnGenerate->setMinimumHeight(42);
    btnGenerate->setFont(QFont("Segoe UI", 13, QFont::Bold));

    genLayout->addWidget(new QLabel("Root  (الجذر):"));
    genLayout->addWidget(m_engRootInput);
    genLayout->addWidget(new QLabel("Scheme  (الوزن):"));
    genLayout->addWidget(m_engSchemeInput);
    genLayout->addWidget(btnGenerate);

    // Validate word panel
    QGroupBox* valGroup = new QGroupBox("Validate Word  (التحقق من الكلمة)");
    QVBoxLayout* valLayout = new QVBoxLayout(valGroup);
    valLayout->setSpacing(8);

    QLabel* valDesc = new QLabel("Check whether a word morphologically belongs to a root.");
    valDesc->setWordWrap(true);
    valDesc->setStyleSheet("color: #6e7681; font-size: 11px; margin-bottom:4px;");
    valLayout->addWidget(valDesc);

    m_engWordInput = new QLineEdit();
    m_engWordInput->setPlaceholderText("Word to validate  (الكلمة)");
    m_engWordInput->setLayoutDirection(Qt::RightToLeft);
    m_engWordInput->setFont(QFont("Arial", 14));
    m_engWordInput->setMinimumHeight(40);

    m_engValidRootInput = new QLineEdit();
    m_engValidRootInput->setPlaceholderText("Against root  (الجذر)");
    m_engValidRootInput->setLayoutDirection(Qt::RightToLeft);
    m_engValidRootInput->setFont(QFont("Arial", 14));
    m_engValidRootInput->setMinimumHeight(40);

    auto* btnValidate = new QPushButton("✓  Validate Word");
    btnValidate->setObjectName("btnSuccess");
    btnValidate->setMinimumHeight(42);
    btnValidate->setFont(QFont("Segoe UI", 13, QFont::Bold));

    valLayout->addWidget(new QLabel("Word  (الكلمة):"));
    valLayout->addWidget(m_engWordInput);
    valLayout->addWidget(new QLabel("Root  (الجذر):"));
    valLayout->addWidget(m_engValidRootInput);
    valLayout->addWidget(btnValidate);

    // Morphological family panel
    QGroupBox* famGroup = new QGroupBox("Morphological Family  (العائلة الصرفية)");
    QVBoxLayout* famLayout = new QVBoxLayout(famGroup);
    famLayout->setSpacing(8);

    QLabel* famDesc = new QLabel("Find all roots that have a validated derivative for the given scheme.");
    famDesc->setWordWrap(true);
    famDesc->setStyleSheet("color: #6e7681; font-size: 11px; margin-bottom:4px;");
    famLayout->addWidget(famDesc);

    m_engFamilySchemeInput = new QLineEdit();
    m_engFamilySchemeInput->setPlaceholderText("Scheme  (الوزن)  e.g. فاعل");
    m_engFamilySchemeInput->setLayoutDirection(Qt::RightToLeft);
    m_engFamilySchemeInput->setFont(QFont("Arial", 14));
    m_engFamilySchemeInput->setMinimumHeight(40);

    auto* btnFamily = new QPushButton("🔍  Find Family");
    btnFamily->setObjectName("btnWarning");
    btnFamily->setMinimumHeight(42);
    btnFamily->setFont(QFont("Segoe UI", 13, QFont::Bold));

    famLayout->addWidget(new QLabel("Scheme  (الوزن):"));
    famLayout->addWidget(m_engFamilySchemeInput);
    famLayout->addStretch();
    famLayout->addWidget(btnFamily);

    topRow->addWidget(genGroup);
    topRow->addWidget(valGroup);
    topRow->addWidget(famGroup);
    mainLayout->addLayout(topRow);

    // Engine log
    QGroupBox* logGroup = new QGroupBox("Engine Output  (المخرجات)");
    QVBoxLayout* logLayout = new QVBoxLayout(logGroup);
    m_engineLog = new QTextEdit();
    m_engineLog->setReadOnly(true);
    m_engineLog->setMinimumHeight(220);
    logLayout->addWidget(m_engineLog);

    auto* btnClearLog = new QPushButton("Clear Log");
    btnClearLog->setMaximumWidth(100);
    logLayout->addWidget(btnClearLog, 0, Qt::AlignRight);

    mainLayout->addWidget(logGroup, 1);

    connect(btnGenerate, &QPushButton::clicked, this, &MainWindow::onGenerateWord);
    connect(btnValidate, &QPushButton::clicked, this, &MainWindow::onValidateWord);
    connect(btnFamily,   &QPushButton::clicked, this, &MainWindow::onMorphologicalFamily);
    connect(btnClearLog, &QPushButton::clicked, m_engineLog, &QTextEdit::clear);

    m_tabs->addTab(tab, "⚡  Morphological Engine");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Tab 4: About
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::setupAboutTab() {
    QWidget* tab = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tab);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(20);

    QLabel* title = new QLabel("محرك البحث المورفولوجي العربي\nArabic Morphological Search Engine");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #58a6ff; font-size: 22px; font-weight: bold; line-height: 1.5;");
    layout->addWidget(title);

    QLabel* separator = new QLabel();
    separator->setFixedHeight(1);
    separator->setStyleSheet("background: #21262d;");
    layout->addWidget(separator);

    QLabel* info = new QLabel(
        "<div style='line-height:2; color:#c9d1d9; font-size:13px;'>"
        "<b style='color:#58a6ff;'>Data Structures:</b><br>"
        "&nbsp;&nbsp;• <b>AVL Tree</b> (BinarySearchTree) — stores and balances Arabic roots with O(log n) operations<br>"
        "&nbsp;&nbsp;• <b>Hash Table</b> (hashmap) — stores morphological schemes with chaining collision resolution<br><br>"
        "<b style='color:#58a6ff;'>Features:</b><br>"
        "&nbsp;&nbsp;• Insert / Search / Delete Arabic roots (AVL auto-balancing)<br>"
        "&nbsp;&nbsp;• Insert / Search / Delete / Update morphological schemes<br>"
        "&nbsp;&nbsp;• Load roots or schemes from external text files<br>"
        "&nbsp;&nbsp;• Generate words: apply a scheme's algorithm to a root<br>"
        "&nbsp;&nbsp;• Validate words: check if a word belongs to a root<br>"
        "&nbsp;&nbsp;• Morphological family: find all roots sharing a scheme<br>"
        "&nbsp;&nbsp;• Interactive AVL tree visualization with click-to-inspect<br>"
        "&nbsp;&nbsp;• Derivative tracking per root with frequency counts<br><br>"
        "<b style='color:#58a6ff;'>Algorithm Detail:</b><br>"
        "&nbsp;&nbsp;Schemes use tokens <code style='color:#79c0ff'>root[0]</code>, <code style='color:#79c0ff'>root[1]</code>, <code style='color:#79c0ff'>root[2]</code><br>"
        "&nbsp;&nbsp;representing ف (fa), ع (ain), ل (lam) — the three root letters.<br>"
        "&nbsp;&nbsp;The engine substitutes these with the actual root letters to form words."
        "</div>"
        );
    info->setWordWrap(true);
    info->setTextFormat(Qt::RichText);
    layout->addWidget(info);
    layout->addStretch();

    m_tabs->addTab(tab, "ℹ  About");
}

// ─────────────────────────────────────────────────────────────────────────────
//  Helper Methods
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::log(const QString& msg, const QString& color) {
    // Determine which tab is active and log to appropriate widget
    QTextEdit* logWidget = nullptr;
    int idx = m_tabs->currentIndex();
    if (idx == 0) logWidget = m_rootLog;
    else if (idx == 1) logWidget = m_schemeLog;
    else logWidget = m_engineLog;

    if (!logWidget) return;
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss");
    logWidget->append(QString("<span style='color:#484f58;'>[%1]</span> "
                              "<span style='color:%2;'>%3</span>")
                          .arg(time).arg(color).arg(msg));
    logWidget->verticalScrollBar()->setValue(
        logWidget->verticalScrollBar()->maximum());
}

void MainWindow::logSuccess(const QString& msg) { log("✓ " + msg, "#2ea043"); }
void MainWindow::logError(const QString& msg)   { log("✗ " + msg, "#f85149"); }
void MainWindow::logInfo(const QString& msg)    { log("ℹ " + msg, "#58a6ff"); }

void MainWindow::updateTreeStats() {
    m_statNodes->setText(QString::number(m_tree->getNodeCount()));
    m_statHeight->setText(QString::number(m_tree->getHeight()));
    m_statEmpty->setText(m_tree->isEmpty() ? "Yes" : "No");
    m_statEmpty->setStyleSheet(m_tree->isEmpty()
                                   ? "color: #f85149; font-size:13px; font-weight:bold;"
                                   : "color: #2ea043; font-size:13px; font-weight:bold;");
    m_statusLabel->setText(QString("Roots: %1  |  Height: %2")
                               .arg(m_tree->getNodeCount())
                               .arg(m_tree->getHeight()));
}

void MainWindow::refreshTreeView() {
    m_treeViz->refresh();
    updateTreeStats();
}

void MainWindow::refreshSchemeTable() {
    m_schemeTable->setRowCount(0);
    for (int i = 0; i < (int)m_hashmap->v.size(); i++) {
        struct node* cur = m_hashmap->v[i];
        while (cur) {
            int row = m_schemeTable->rowCount();
            m_schemeTable->insertRow(row);
            auto* bucketItem = new QTableWidgetItem(QString::number(i));
            bucketItem->setTextAlignment(Qt::AlignCenter);
            bucketItem->setForeground(QColor(88, 166, 255));
            m_schemeTable->setItem(row, 0, bucketItem);

            auto* keyItem = new QTableWidgetItem(QString::fromStdString(cur->key));
            keyItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            keyItem->setFont(QFont("Arial", 13));
            m_schemeTable->setItem(row, 1, keyItem);

            auto* algoItem = new QTableWidgetItem(QString::fromStdString(cur->value.algo));
            algoItem->setFont(QFont("Courier New", 11));
            algoItem->setForeground(QColor(121, 192, 255));
            m_schemeTable->setItem(row, 2, algoItem);

            cur = cur->next;
        }
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Root Tab Slots
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onInsertRoot() {
    QString input = m_rootInput->text().trimmed();
    if (input.isEmpty()) { logError("Empty input — please enter a root."); return; }
    m_tree->insert(Root(input.toStdString()));
    logSuccess(QString("Root \"%1\" inserted into AVL tree.").arg(input));
    m_rootInput->clear();
    refreshTreeView();
    m_treeViz->selectNode(input);
}

void MainWindow::onSearchRoot() {
    QString input = m_rootInput->text().trimmed();
    if (input.isEmpty()) { logError("Enter a root to search."); return; }
    bool found = m_tree->search(input.toStdString());
    if (found) {
        logSuccess(QString("Root \"%1\" found in AVL tree.").arg(input));
        m_treeViz->selectNode(input);
        onTreeNodeClicked(input);
    }
    else logError(QString("Root \"%1\" not found.").arg(input));
}

void MainWindow::onDeleteRoot() {
    QString input = m_rootInput->text().trimmed();
    if (input.isEmpty()) { logError("Enter a root to delete."); return; }
    if (!m_tree->search(input.toStdString())) {
        logError(QString("Root \"%1\" not found — nothing to delete.").arg(input));
        return;
    }
    m_tree->deleteN(Root(input.toStdString()));
    logSuccess(QString("Root \"%1\" deleted from AVL tree.").arg(input));
    m_rootInput->clear();
    m_derivativesList->clear();
    m_selectedRootLabel->setText("Click a node to inspect");
    refreshTreeView();
}

void MainWindow::onDisplayAllRoots() {
    if (m_tree->isEmpty()) { logInfo("Tree is empty."); return; }
    vector<Root> roots = m_tree->getAllRoots();
    logInfo(QString("All %1 root(s) in order:").arg(roots.size()));
    QStringList parts;
    for (auto& r : roots) parts << QString::fromStdString(r.getRoot());
    log("  " + parts.join("  →  "), "#c9d1d9");
}

void MainWindow::onShowDerivatives() {
    QString input = m_rootInput->text().trimmed();
    if (input.isEmpty()) { logError("Enter a root to show its derivatives."); return; }
    Node* nd = m_tree->getRootNode(input.toStdString());
    if (!nd) { logError(QString("Root \"%1\" not found.").arg(input)); return; }
    onTreeNodeClicked(input);
}

void MainWindow::onTreeNodeClicked(const QString& rootName) {
    m_selectedRootLabel->setText(QString("Root: %1").arg(rootName));
    m_derivativesList->clear();

    Node* nd = m_tree->getRootNode(rootName.toStdString());
    if (!nd) return;

    auto derivs = nd->getRootObject().getDerivatives();
    if (derivs.empty()) {
        m_derivativesList->addItem("(No derivatives yet)");
        logInfo(QString("Root \"%1\" has no derivatives yet.").arg(rootName));
    } else {
        for (auto& [word, freq] : derivs) {
            auto* item = new QListWidgetItem(
                QString("%1   (freq: %2)").arg(QString::fromStdString(word)).arg(freq));
            item->setTextAlignment(Qt::AlignRight);
            m_derivativesList->addItem(item);
        }
        logInfo(QString("Root \"%1\" — %2 derivative(s):")
                    .arg(rootName).arg(derivs.size()));
        for (auto& [word, freq] : derivs)
            log(QString("  • %1  (×%2)").arg(QString::fromStdString(word)).arg(freq),
                "#79c0ff");
    }
}

void MainWindow::onLoadRootsFromFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Load Roots from File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;
    bool ok = m_tree->loadRootsFromFile(fileName.toStdString());
    if (ok) {
        logSuccess(QString("Roots loaded from \"%1\". Tree now has %2 root(s).")
                       .arg(fileName).arg(m_tree->getNodeCount()));
        refreshTreeView();
    } else {
        logError(QString("Could not open file \"%1\".").arg(fileName));
    }
}

// ─────────────────────────────────────────────────────────────────────────────
//  Scheme Tab Slots
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onInsertScheme() {
    QString input = m_schemeInput->text().trimmed();
    if (input.isEmpty()) { logError("Empty input."); return; }
    insert(m_hashmap, input.toStdString());
    logSuccess(QString("Scheme \"%1\" inserted.").arg(input));
    m_schemeInput->clear();
    refreshSchemeTable();
}

void MainWindow::onSearchScheme() {
    QString input = m_schemeInput->text().trimmed();
    if (input.isEmpty()) { logError("Enter a scheme to search."); return; }
    int pos = search(input.toStdString(), m_hashmap);
    if (pos != -1)
        logSuccess(QString("Scheme \"%1\" found in bucket %2.").arg(input).arg(pos));
    else
        logError(QString("Scheme \"%1\" not found.").arg(input));
}

void MainWindow::onDeleteScheme() {
    QString input = m_schemeInput->text().trimmed();
    if (input.isEmpty()) { logError("Enter a scheme to delete."); return; }
    del(input.toStdString(), m_hashmap);
    logSuccess(QString("Scheme \"%1\" deleted.").arg(input));
    m_schemeInput->clear();
    refreshSchemeTable();
}

void MainWindow::onDisplayAllSchemes() {
    refreshSchemeTable();
    logInfo(QString("%1 scheme(s) in hash table.").arg(m_schemeTable->rowCount()));
}

void MainWindow::onUpdateScheme() {
    QString oldKey = m_schemeOldInput->text().trimmed();
    QString newKey = m_schemeNewInput->text().trimmed();
    if (oldKey.isEmpty() || newKey.isEmpty()) {
        logError("Please fill both old and new scheme names.");
        return;
    }
    ::update(oldKey.toStdString(), newKey.toStdString(), m_hashmap);
    logSuccess(QString("Scheme updated: \"%1\" → \"%2\".").arg(oldKey).arg(newKey));
    m_schemeOldInput->clear();
    m_schemeNewInput->clear();
    refreshSchemeTable();
}

void MainWindow::onLoadSchemesFromFile() {
    QString fileName = QFileDialog::getOpenFileName(
        this, "Load Schemes from File", "", "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        logError(QString("Could not open \"%1\".").arg(fileName));
        return;
    }
    int count = 0;
    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty()) {
            insert(m_hashmap, line.toStdString());
            count++;
        }
    }
    file.close();
    logSuccess(QString("%1 scheme(s) loaded from \"%2\".").arg(count).arg(fileName));
    refreshSchemeTable();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Engine Tab Slots
// ─────────────────────────────────────────────────────────────────────────────

void MainWindow::onGenerateWord() {
    QString root   = m_engRootInput->text().trimmed();
    QString scheme = m_engSchemeInput->text().trimmed();

    if (root.isEmpty() || scheme.isEmpty()) {
        m_engineLog->append("<span style='color:#f85149;'>✗ Please enter both root and scheme.</span>");
        return;
    }

    // Find scheme algo in hashmap
    int pos = search(scheme.toStdString(), m_hashmap);
    if (pos == -1) {
        m_engineLog->append(QString("<span style='color:#f85149;'>✗ Scheme \"%1\" not found in hash table.</span>").arg(scheme));
        return;
    }
    struct node* cur = m_hashmap->v[pos];
    while (cur && cur->key != scheme.toStdString()) cur = cur->next;
    if (!cur) {
        m_engineLog->append("<span style='color:#f85149;'>✗ Scheme not found.</span>");
        return;
    }

    string word = apply_algo(cur->value.algo, root.toStdString());
    QString qword = QString::fromStdString(word);

    m_engineLog->append(QString(
                            "<div style='margin:6px 0; padding:10px; background:#1a2744; border-left:3px solid #388bfd; border-radius:4px;'>"
                            "<span style='color:#8b949e;'>Root:</span> <b style='color:#79c0ff; font-size:16px;'>%1</b>&nbsp;&nbsp;"
                            "<span style='color:#8b949e;'>Scheme:</span> <b style='color:#79c0ff; font-size:16px;'>%2</b>&nbsp;&nbsp;"
                            "<span style='color:#8b949e;'>→</span>&nbsp;&nbsp;"
                            "<b style='color:#56d364; font-size:18px;'>%3</b>"
                            "</div>").arg(root).arg(scheme).arg(qword));


    Node* nd = m_tree->getRootNode(root.toStdString());
    if (nd) {
        nd->getRootObject().addderviation(word);
        m_engineLog->append(QString("<span style='color:#2ea043;'>✓ \"%1\" stored as derivative of \"%2\" in AVL tree.</span>")
                                .arg(qword).arg(root));
        refreshTreeView();
    } else {
        m_engineLog->append(QString("<span style='color:#d29922;'>ℹ Root \"%1\" not in AVL tree. Insert it (Tab 1) to track derivatives.</span>")
                                .arg(root));
    }
}

void MainWindow::onValidateWord() {
    QString word = m_engWordInput->text().trimmed();
    QString root = m_engValidRootInput->text().trimmed();

    if (word.isEmpty() || root.isEmpty()) {
        m_engineLog->append("<span style='color:#f85149;'>✗ Please enter both word and root.</span>");
        return;
    }

    // Inline validate: search all schemes for a match
    string result = "NON";
    for (int i = 0; i < (int)m_hashmap->v.size(); i++) {
        struct node* cn = m_hashmap->v[i];
        while (cn) {
            string generated = apply_algo(cn->value.algo, root.toStdString());
            if (generated == word.toStdString()) {
                result = "OUI|" + cn->key;
                break;
            }
            cn = cn->next;
        }
        if (result != "NON") break;
    }
    QString qresult = QString::fromStdString(result);

    if (qresult.startsWith("OUI|")) {
        QString matchedScheme = qresult.mid(4);
        m_engineLog->append(QString(
                                "<div style='margin:6px 0; padding:10px; background:#0f2d1a; border-left:3px solid #2ea043; border-radius:4px;'>"
                                "<b style='color:#56d364; font-size:16px;'>✓ OUI</b> — "
                                "<span style='color:#c9d1d9;'>\"%1\" matches scheme <b style='color:#79c0ff;'>%2</b> with root <b style='color:#79c0ff;'>%3</b></span>"
                                "</div>").arg(word).arg(matchedScheme).arg(root));

        // Store in AVL tree
        Node* nd = m_tree->getRootNode(root.toStdString());
        if (nd) {
            nd->getRootObject().addderviation(word.toStdString());
            m_engineLog->append(QString("<span style='color:#2ea043;'>✓ \"%1\" stored as derivative of \"%2\".</span>")
                                    .arg(word).arg(root));
            refreshTreeView();
        }
    } else {
        m_engineLog->append(QString(
                                "<div style='margin:6px 0; padding:10px; background:#2d0f0f; border-left:3px solid #f85149; border-radius:4px;'>"
                                "<b style='color:#f85149; font-size:16px;'>✗ NON</b> — "
                                "<span style='color:#c9d1d9;'>\"%1\" does not match any scheme for root \"%2\".</span>"
                                "</div>").arg(word).arg(root));
    }
}

void MainWindow::onMorphologicalFamily() {
    QString scheme = m_engFamilySchemeInput->text().trimmed();
    if (scheme.isEmpty()) {
        m_engineLog->append("<span style='color:#f85149;'>✗ Please enter a scheme.</span>");
        return;
    }

    // Inline morphological family
    vector<pair<string,string>> results;
    {
        int pos = search(scheme.toStdString(), m_hashmap);
        if (pos != -1) {
            struct node* sn = m_hashmap->v[pos];
            while (sn && sn->key != scheme.toStdString()) sn = sn->next;
            if (sn) {
                string algo = sn->value.algo;
                vector<Root> allRoots = m_tree->getAllRoots();
                for (Root& r : allRoots) {
                    string expectedWord = apply_algo(algo, r.getRoot());
                    auto derivs = r.getDerivatives();
                    if (derivs.count(expectedWord))
                        results.push_back({r.getRoot(), expectedWord});
                }
            }
        }
    }

    m_engineLog->append(QString(
                            "<div style='margin:6px 0; padding:8px; background:#1f1a00; border-left:3px solid #d29922; border-radius:4px;'>"
                            "<b style='color:#f0c000; font-size:14px;'>Morphological Family — Scheme: %1</b>"
                            "</div>").arg(scheme));

    if (results.empty()) {
        m_engineLog->append("<span style='color:#8b949e;'>  No validated derivatives found for this scheme.</span>");
    } else {
        m_engineLog->append(QString("<span style='color:#d29922;'>  %1 result(s):</span>").arg(results.size()));
        for (auto& [root, word] : results) {
            m_engineLog->append(QString(
                                    "  <span style='color:#8b949e;'>Root</span> "
                                    "<b style='color:#79c0ff;'>%1</b>"
                                    " <span style='color:#484f58;'>→</span> "
                                    "<b style='color:#56d364;'>%2</b>")
                                    .arg(QString::fromStdString(root))
                                    .arg(QString::fromStdString(word)));
        }
    }
}
