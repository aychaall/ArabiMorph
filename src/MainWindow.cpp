#include "../include/MainWindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QScrollBar>
#include <QFont>
#include <QFontDatabase>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include <QSpacerItem>

// ─────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_tree(new BinarySearchTree())
{
    setWindowTitle("ArabiMorph — Arabic Morphological Search Engine");
    setMinimumSize(1280, 780);

    // Center on screen
    QScreen* screen = QApplication::primaryScreen();
    if (screen) {
        QRect sg = screen->availableGeometry();
        resize(qMin(1600, sg.width() - 80), qMin(900, sg.height() - 80));
        move(sg.center() - rect().center());
    }

    setupUI();
    setupStylesheet();

    m_toastTimer = new QTimer(this);
    m_toastTimer->setSingleShot(true);
    connect(m_toastTimer, &QTimer::timeout, this, [this]() {
        m_toastLabel->hide();
    });

    statusBar()->showMessage("جاهز — Ready");
    refreshStats();
}

MainWindow::~MainWindow() { delete m_tree; }

// ─────────────────────────────────────────────────────────────────
// UI Setup
// ─────────────────────────────────────────────────────────────────
void MainWindow::setupUI() {
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Left sidebar ──────────────────────────────────────────
    m_leftPanel = new QWidget();
    m_leftPanel->setObjectName("leftPanel");
    m_leftPanel->setFixedWidth(280);

    QVBoxLayout* leftLayout = new QVBoxLayout(m_leftPanel);
    leftLayout->setContentsMargins(16, 16, 16, 16);
    leftLayout->setSpacing(12);

    // Logo / title
    QLabel* logoLabel = new QLabel("🌳 ArabiMorph");
    logoLabel->setObjectName("logoLabel");
    logoLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(logoLabel);

    QLabel* subLabel = new QLabel("محرك البحث المورفولوجي العربي\nArabic Morphological BST");
    subLabel->setObjectName("subLabel");
    subLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(subLabel);

    // Separator
    QFrame* sep1 = new QFrame(); sep1->setFrameShape(QFrame::HLine);
    sep1->setObjectName("separator");
    leftLayout->addWidget(sep1);

    // ── Insert group ─────────────────────────────────────────
    QGroupBox* insertGroup = new QGroupBox("إضافة جذر — Insert Root");
    insertGroup->setObjectName("sideGroup");
    QVBoxLayout* ig = new QVBoxLayout(insertGroup);
    ig->setSpacing(8);

    m_rootInput = new QLineEdit();
    m_rootInput->setObjectName("arabicInput");
    m_rootInput->setPlaceholderText("أدخل الجذر ... (e.g. كتب)");
    m_rootInput->setLayoutDirection(Qt::RightToLeft);

    m_insertBtn = new QPushButton("＋ Insert");
    m_insertBtn->setObjectName("primaryBtn");
    m_insertBtn->setCursor(Qt::PointingHandCursor);

    ig->addWidget(m_rootInput);
    ig->addWidget(m_insertBtn);
    leftLayout->addWidget(insertGroup);

    // ── Search group ──────────────────────────────────────────
    QGroupBox* searchGroup = new QGroupBox("بحث — Search");
    searchGroup->setObjectName("sideGroup");
    QVBoxLayout* sg_ = new QVBoxLayout(searchGroup);
    sg_->setSpacing(8);

    m_searchInput = new QLineEdit();
    m_searchInput->setObjectName("arabicInput");
    m_searchInput->setPlaceholderText("ابحث عن جذر ...");
    m_searchInput->setLayoutDirection(Qt::RightToLeft);

    m_searchBtn = new QPushButton("🔍 Search");
    m_searchBtn->setObjectName("secondaryBtn");
    m_searchBtn->setCursor(Qt::PointingHandCursor);

    sg_->addWidget(m_searchInput);
    sg_->addWidget(m_searchBtn);
    leftLayout->addWidget(searchGroup);

    // ── Delete group ──────────────────────────────────────────
    QGroupBox* deleteGroup = new QGroupBox("حذف — Delete");
    deleteGroup->setObjectName("sideGroup");
    QVBoxLayout* dg = new QVBoxLayout(deleteGroup);
    dg->setSpacing(8);

    m_deleteBtn = new QPushButton("✕ Delete Selected");
    m_deleteBtn->setObjectName("dangerBtn");
    m_deleteBtn->setCursor(Qt::PointingHandCursor);

    dg->addWidget(m_deleteBtn);
    leftLayout->addWidget(deleteGroup);

    // ── Derivative group ─────────────────────────────────────
    QGroupBox* derivGroup = new QGroupBox("مشتقات — Derivatives");
    derivGroup->setObjectName("sideGroup");
    QVBoxLayout* dvg = new QVBoxLayout(derivGroup);
    dvg->setSpacing(8);

    m_derivativeInput = new QLineEdit();
    m_derivativeInput->setObjectName("arabicInput");
    m_derivativeInput->setPlaceholderText("المشتق ... (e.g. كاتب)");
    m_derivativeInput->setLayoutDirection(Qt::RightToLeft);

    m_addDerivBtn = new QPushButton("⊕ Add Derivative");
    m_addDerivBtn->setObjectName("accentBtn");
    m_addDerivBtn->setCursor(Qt::PointingHandCursor);

    dvg->addWidget(m_derivativeInput);
    dvg->addWidget(m_addDerivBtn);
    leftLayout->addWidget(derivGroup);

    // ── File load ─────────────────────────────────────────────
    m_loadFileBtn = new QPushButton("📂 Load from File");
    m_loadFileBtn->setObjectName("ghostBtn");
    m_loadFileBtn->setCursor(Qt::PointingHandCursor);
    leftLayout->addWidget(m_loadFileBtn);

    QFrame* sep2 = new QFrame(); sep2->setFrameShape(QFrame::HLine);
    sep2->setObjectName("separator");
    leftLayout->addWidget(sep2);

    // ── Root list ─────────────────────────────────────────────
    QLabel* listLabel = new QLabel("الجذور — Roots");
    listLabel->setObjectName("sectionLabel");
    leftLayout->addWidget(listLabel);

    m_rootList = new QListWidget();
    m_rootList->setObjectName("rootList");
    leftLayout->addWidget(m_rootList);

    // ── Stats ─────────────────────────────────────────────────
    m_statsLabel = new QLabel("Nodes: 0 | Height: 0");
    m_statsLabel->setObjectName("statsLabel");
    m_statsLabel->setAlignment(Qt::AlignCenter);
    leftLayout->addWidget(m_statsLabel);

    // ── Centre: tree canvas ───────────────────────────────────
    QWidget* centerWidget = new QWidget();
    centerWidget->setObjectName("centerWidget");
    QVBoxLayout* cl = new QVBoxLayout(centerWidget);
    cl->setContentsMargins(0, 0, 0, 0);
    cl->setSpacing(0);

    // Toolbar for zoom
    QWidget* toolbar = new QWidget();
    toolbar->setObjectName("toolbar");
    toolbar->setFixedHeight(46);
    QHBoxLayout* tbl = new QHBoxLayout(toolbar);
    tbl->setContentsMargins(12, 4, 12, 4);
    tbl->setSpacing(6);

    QLabel* treeTitle = new QLabel("  AVL Tree Visualization");
    treeTitle->setObjectName("toolbarTitle");
    tbl->addWidget(treeTitle);
    tbl->addStretch();

    m_zoomOutBtn   = new QToolButton(); m_zoomOutBtn->setText("−");
    m_zoomInBtn    = new QToolButton(); m_zoomInBtn->setText("+");
    m_resetViewBtn = new QToolButton(); m_resetViewBtn->setText("⌂");
    for (auto* b : {m_zoomOutBtn, m_zoomInBtn, m_resetViewBtn}) {
        b->setObjectName("toolBtn");
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(30, 30);
        tbl->addWidget(b);
    }

    cl->addWidget(toolbar);

    m_treeCanvas = new TreeCanvas();
    m_treeCanvas->setTree(m_tree);
    cl->addWidget(m_treeCanvas, 1);

    // Toast label (overlay)
    m_toastLabel = new QLabel(centerWidget);
    m_toastLabel->setObjectName("toastLabel");
    m_toastLabel->setAlignment(Qt::AlignCenter);
    m_toastLabel->setFixedHeight(40);
    m_toastLabel->hide();

    // ── Right: detail panel ───────────────────────────────────
    m_detailPanel = new QWidget();
    m_detailPanel->setObjectName("detailPanel");
    m_detailPanel->setFixedWidth(260);
    QVBoxLayout* rl = new QVBoxLayout(m_detailPanel);
    rl->setContentsMargins(16, 16, 16, 16);
    rl->setSpacing(10);

    m_detailTitle = new QLabel("Node Details");
    m_detailTitle->setObjectName("detailTitle");
    m_detailTitle->setAlignment(Qt::AlignCenter);
    rl->addWidget(m_detailTitle);

    m_detailText = new QTextEdit();
    m_detailText->setObjectName("detailText");
    m_detailText->setReadOnly(true);
    rl->addWidget(m_detailText, 1);

    // Assemble main layout
    mainLayout->addWidget(m_leftPanel);
    mainLayout->addWidget(centerWidget, 1);
    mainLayout->addWidget(m_detailPanel);

    // ── Connections ───────────────────────────────────────────
    connect(m_insertBtn,    &QPushButton::clicked, this, &MainWindow::onInsertClicked);
    connect(m_searchBtn,    &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    connect(m_deleteBtn,    &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
    connect(m_addDerivBtn,  &QPushButton::clicked, this, &MainWindow::onAddDerivativeClicked);
    connect(m_loadFileBtn,  &QPushButton::clicked, this, &MainWindow::onLoadFileClicked);
    connect(m_zoomInBtn,    &QToolButton::clicked, m_treeCanvas, &TreeCanvas::zoomIn);
    connect(m_zoomOutBtn,   &QToolButton::clicked, m_treeCanvas, &TreeCanvas::zoomOut);
    connect(m_resetViewBtn, &QToolButton::clicked, m_treeCanvas, &TreeCanvas::resetView);
    connect(m_treeCanvas,   &TreeCanvas::nodeClicked, this, &MainWindow::onNodeSelected);
    connect(m_rootList,     &QListWidget::itemClicked, this, &MainWindow::onRootListItemClicked);

    // Enter key shortcuts
    connect(m_rootInput,  &QLineEdit::returnPressed, this, &MainWindow::onInsertClicked);
    connect(m_searchInput,&QLineEdit::returnPressed, this, &MainWindow::onSearchClicked);

    m_detailText->setHtml(
        "<div style='color:#6080a0;text-align:center;margin-top:40px;font-family:Arial;'>"
        "<p style='font-size:28px;'>🌿</p>"
        "<p>Click a node in the tree<br>to see details here</p>"
        "</div>"
    );
}

// ─────────────────────────────────────────────────────────────────
// Stylesheet
// ─────────────────────────────────────────────────────────────────
void MainWindow::setupStylesheet() {
    setStyleSheet(R"(
QMainWindow {
    background-color: #0a0c16;
}
QWidget {
    font-family: "Segoe UI", "Arial", sans-serif;
    color: #d0ddf0;
}

/* ── Left Panel ── */
#leftPanel {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #0f1220, stop:1 #0a0c18);
    border-right: 1px solid #1e2a45;
}
#logoLabel {
    font-size: 22px;
    font-weight: bold;
    color: #7ab8f5;
    padding: 8px 0;
    letter-spacing: 1px;
}
#subLabel {
    font-size: 10px;
    color: #4a6080;
    line-height: 1.5;
}
#separator {
    color: #1e2a45;
    background: #1e2a45;
    max-height: 1px;
    border: none;
}
#sectionLabel {
    font-size: 11px;
    color: #5a80aa;
    font-weight: bold;
    letter-spacing: 0.5px;
    text-transform: uppercase;
}
QGroupBox#sideGroup {
    background: rgba(20, 30, 55, 0.6);
    border: 1px solid #1e3060;
    border-radius: 8px;
    margin-top: 6px;
    padding: 8px;
    font-size: 11px;
    color: #6080a8;
    font-weight: bold;
}
QGroupBox#sideGroup::title {
    subcontrol-origin: margin;
    left: 10px;
    padding: 0 4px;
    color: #5090c0;
}

/* ── Inputs ── */
#arabicInput {
    background: #0d1525;
    border: 1px solid #2a4070;
    border-radius: 6px;
    padding: 8px 12px;
    font-size: 14px;
    color: #e0eeff;
    selection-background-color: #2060a0;
}
#arabicInput:focus {
    border-color: #4090e0;
    background: #101a2e;
}
#arabicInput::placeholder {
    color: #304060;
}

/* ── Buttons ── */
QPushButton, QToolButton {
    border-radius: 6px;
    padding: 8px 14px;
    font-size: 12px;
    font-weight: bold;
    border: none;
}
#primaryBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #3070d0, stop:1 #1a50b0);
    color: white;
}
#primaryBtn:hover {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #4080e0, stop:1 #2060c0);
}
#primaryBtn:pressed { background: #1040a0; }

#secondaryBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #206060, stop:1 #104040);
    color: #80e0c0;
}
#secondaryBtn:hover { background: #207060; }

#dangerBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #802020, stop:1 #501010);
    color: #ffa0a0;
}
#dangerBtn:hover { background: #a02828; }

#accentBtn {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #605010, stop:1 #402808);
    color: #ffd080;
}
#accentBtn:hover { background: #706018; }

#ghostBtn {
    background: rgba(30, 50, 90, 0.5);
    color: #6090b8;
    border: 1px solid #2a3a60;
}
#ghostBtn:hover {
    background: rgba(40, 70, 110, 0.7);
    color: #80b0d0;
}

/* ── Root List ── */
#rootList {
    background: #080e1a;
    border: 1px solid #1a2a45;
    border-radius: 6px;
    color: #a0c0e0;
    font-size: 13px;
    padding: 4px;
}
#rootList::item {
    padding: 6px 10px;
    border-radius: 4px;
    margin: 1px 0;
}
#rootList::item:hover {
    background: rgba(40, 80, 140, 0.5);
    color: #c0e0ff;
}
#rootList::item:selected {
    background: rgba(50, 100, 180, 0.7);
    color: white;
}

#statsLabel {
    font-size: 10px;
    color: #3a5070;
    padding: 4px;
    border: 1px solid #1a2a40;
    border-radius: 4px;
    background: #080e1a;
}

/* ── Center ── */
#centerWidget {
    background: #0a0c16;
}
#toolbar {
    background: #0d1220;
    border-bottom: 1px solid #1a2540;
}
#toolbarTitle {
    font-size: 13px;
    font-weight: bold;
    color: #5080b0;
    letter-spacing: 1px;
}
#toolBtn {
    background: rgba(30, 50, 90, 0.6);
    color: #80b0e0;
    border: 1px solid #2a3a60;
    font-size: 16px;
    font-weight: bold;
    border-radius: 5px;
}
#toolBtn:hover {
    background: rgba(50, 80, 130, 0.8);
    color: #a0d0ff;
}

/* ── Right Panel ── */
#detailPanel {
    background: qlineargradient(x1:0,y1:0,x2:0,y2:1,
        stop:0 #0d1220, stop:1 #080d18);
    border-left: 1px solid #1e2a45;
}
#detailTitle {
    font-size: 14px;
    font-weight: bold;
    color: #6090c0;
    padding: 8px 0;
    border-bottom: 1px solid #1a2840;
}
#detailText {
    background: transparent;
    border: none;
    color: #a0c0e0;
    font-size: 12px;
    line-height: 1.6;
}
#detailText QScrollBar:vertical {
    background: #0a1020;
    width: 6px;
    border-radius: 3px;
}
#detailText QScrollBar::handle:vertical {
    background: #2a4060;
    border-radius: 3px;
}

/* ── Toast ── */
#toastLabel {
    background: rgba(20, 60, 120, 0.95);
    color: #80d0ff;
    border: 1px solid #3070c0;
    border-radius: 20px;
    font-size: 13px;
    font-weight: bold;
    padding: 0 20px;
}

/* ── ScrollBars ── */
QScrollBar:vertical {
    background: #080e1a;
    width: 6px;
    border-radius: 3px;
    margin: 0;
}
QScrollBar::handle:vertical {
    background: #2a4060;
    border-radius: 3px;
    min-height: 20px;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }

/* ── Status bar ── */
QStatusBar {
    background: #0a0c16;
    color: #3a5070;
    font-size: 10px;
    border-top: 1px solid #1a2030;
}
    )");
}

// ─────────────────────────────────────────────────────────────────
// Slots
// ─────────────────────────────────────────────────────────────────
void MainWindow::onInsertClicked() {
    QString text = m_rootInput->text().trimmed();
    if (text.isEmpty()) { showToast("⚠ الجذر فارغ — Empty root", false); return; }

    std::string s = text.toStdString();
    if (m_tree->search(s)) {
        showToast("⚠ الجذر موجود — Root already exists", false);
        return;
    }
    m_tree->insert(Root(s));
    m_treeCanvas->animateInsert(text);
    m_rootInput->clear();
    refreshRootList();
    refreshStats();
    showToast("✓ تم إضافة الجذر — Root inserted: " + text, true);
    statusBar()->showMessage("Inserted: " + text);
}

void MainWindow::onSearchClicked() {
    QString text = m_searchInput->text().trimmed();
    if (text.isEmpty()) return;

    std::string s = text.toStdString();
    if (m_tree->search(s)) {
        m_treeCanvas->setHighlightedNode(text);
        showNodeDetails(text);
        showToast("✓ الجذر موجود — Found: " + text, true);
        // Highlight in list
        for (int i = 0; i < m_rootList->count(); ++i) {
            if (m_rootList->item(i)->text() == text) {
                m_rootList->setCurrentRow(i);
                break;
            }
        }
    } else {
        m_treeCanvas->clearHighlight();
        showToast("✗ غير موجود — Not found: " + text, false);
    }
    statusBar()->showMessage("Search: " + text);
}

void MainWindow::onDeleteClicked() {
    QListWidgetItem* item = m_rootList->currentItem();
    if (!item) {
        // Try from search field
        QString text = m_searchInput->text().trimmed();
        if (text.isEmpty()) { showToast("⚠ حدد جذراً — Select a root first", false); return; }
        item = nullptr;
        // find it
        for (int i = 0; i < m_rootList->count(); ++i) {
            if (m_rootList->item(i)->text() == text) {
                item = m_rootList->item(i);
                break;
            }
        }
        if (!item) { showToast("✗ الجذر غير موجود — Root not found", false); return; }
    }

    QString text = item->text();
    m_tree->deleteN(Root(text.toStdString()));
    m_treeCanvas->animateDelete(text);
    refreshRootList();
    refreshStats();
    m_detailText->setHtml(
        "<div style='color:#6080a0;text-align:center;margin-top:40px'>"
        "<p style='font-size:22px;'>🗑</p><p>Node deleted</p></div>"
    );
    showToast("✓ تم الحذف — Deleted: " + text, true);
    statusBar()->showMessage("Deleted: " + text);
}

void MainWindow::onAddDerivativeClicked() {
    QListWidgetItem* item = m_rootList->currentItem();
    QString deriv = m_derivativeInput->text().trimmed();

    if (!item) { showToast("⚠ حدد جذراً — Select a root first", false); return; }
    if (deriv.isEmpty()) { showToast("⚠ المشتق فارغ — Empty derivative", false); return; }

    QString rootName = item->text();
    Node* node = m_tree->getRootNode(rootName.toStdString());
    if (node) {
        node->getRootObject().addderviation(deriv.toStdString());
        m_derivativeInput->clear();
        showNodeDetails(rootName);
        m_treeCanvas->update();
        showToast("✓ تمت إضافة المشتق — Derivative added", true);
    }
}

void MainWindow::onLoadFileClicked() {
    QString fname = QFileDialog::getOpenFileName(this,
        "Load Roots File", "", "Text Files (*.txt);;All Files (*)");
    if (fname.isEmpty()) return;

    if (m_tree->loadRootsFromFile(fname.toStdString())) {
        m_treeCanvas->setTree(m_tree);
        refreshRootList();
        refreshStats();
        showToast("✓ تم تحميل الملف — File loaded", true);
        statusBar()->showMessage("Loaded: " + fname);
    } else {
        showToast("✗ فشل التحميل — Failed to load file", false);
    }
}

void MainWindow::onNodeSelected(const QString& rootName) {
    showNodeDetails(rootName);
    // sync list selection
    for (int i = 0; i < m_rootList->count(); ++i) {
        if (m_rootList->item(i)->text() == rootName) {
            m_rootList->setCurrentRow(i);
            break;
        }
    }
    statusBar()->showMessage("Selected: " + rootName);
}

void MainWindow::onRootListItemClicked(QListWidgetItem* item) {
    if (!item) return;
    QString name = item->text();
    m_treeCanvas->setHighlightedNode(name);
    showNodeDetails(name);
}

// ─────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────
void MainWindow::refreshRootList() {
    m_rootList->clear();
    auto roots = m_tree->getAllRoots();
    for (auto& r : roots) {
        QListWidgetItem* it = new QListWidgetItem(
            QString::fromStdString(r.getRoot()));
        m_rootList->addItem(it);
    }
}

void MainWindow::refreshStats() {
    int n = m_tree->getNodeCount();
    int h = m_tree->getHeight();
    m_statsLabel->setText(
        QString("Nodes: <b>%1</b>   Height: <b>%2</b>").arg(n).arg(h));
    m_statsLabel->setTextFormat(Qt::RichText);
}

void MainWindow::showNodeDetails(const QString& rootName) {
    Node* node = m_tree->getRootNode(rootName.toStdString());
    if (!node) {
        m_detailTitle->setText("Not Found");
        m_detailText->setHtml(
            "<div style='color:#803030;text-align:center;margin-top:30px'>"
            "<p>✗ Root not found</p></div>");
        return;
    }

    Root& r = node->getRootObject();
    auto derivs = r.getDerivativesList();

    m_detailTitle->setText("🌿 " + rootName);

    QString html = "<div style='font-family:Arial; color:#a0c8e0; line-height:1.7;'>";

    // Header box
    html += QString(
        "<div style='background:rgba(20,50,100,0.6); border:1px solid #2a5080; "
        "border-radius:8px; padding:12px; margin-bottom:12px; text-align:center;'>"
        "<span style='font-size:20px; color:#c0e8ff; font-weight:bold;'>%1</span><br>"
        "<span style='font-size:11px; color:#4a7090;'>AVL Height: %2</span>"
        "</div>"
    ).arg(rootName).arg(node->getHeight());

    // Stats row
    html += "<div style='display:flex; gap:8px; margin-bottom:12px;'>";
    html += QString(
        "<div style='background:rgba(30,60,100,0.5); border-radius:6px; "
        "padding:8px; text-align:center; flex:1; border:1px solid #1a3a60;'>"
        "<div style='font-size:18px; color:#60c0ff; font-weight:bold;'>%1</div>"
        "<div style='font-size:10px; color:#3a6080;'>Derivatives</div>"
        "</div>"
    ).arg(derivs.size());

    QString leftStr  = node->getLeft()  ? QString::fromStdString(node->getLeft()->getData())  : "—";
    QString rightStr = node->getRight() ? QString::fromStdString(node->getRight()->getData()) : "—";

    html += QString(
        "<div style='background:rgba(30,60,100,0.5); border-radius:6px; "
        "padding:8px; text-align:center; flex:1; border:1px solid #1a3a60;'>"
        "<div style='font-size:12px; color:#4090d0; font-weight:bold;'>%1</div>"
        "<div style='font-size:10px; color:#3a6080;'>Left</div>"
        "</div>"
    ).arg(leftStr);

    html += QString(
        "<div style='background:rgba(30,60,100,0.5); border-radius:6px; "
        "padding:8px; text-align:center; flex:1; border:1px solid #1a3a60;'>"
        "<div style='font-size:12px; color:#4090d0; font-weight:bold;'>%1</div>"
        "<div style='font-size:10px; color:#3a6080;'>Right</div>"
        "</div>"
    ).arg(rightStr);
    html += "</div>";

    // Derivatives list
    if (derivs.empty()) {
        html += "<div style='color:#3a6080; text-align:center; padding:16px;'>"
                "لا توجد مشتقات<br><i>No derivatives yet</i></div>";
    } else {
        html += "<div style='font-size:11px; color:#3a6080; "
                "text-transform:uppercase; letter-spacing:1px; margin-bottom:6px;'>"
                "Derivatives</div>";
        for (auto& d : derivs) {
            int freq = r.getFrequency(d);
            html += QString(
                "<div style='display:flex; background:rgba(20,40,70,0.6); "
                "border-left:3px solid #3060a0; padding:6px 10px; "
                "border-radius:0 4px 4px 0; margin:2px 0; "
                "font-size:13px; direction:rtl;'>"
                "<span style='color:#c0ddf0; flex:1;'>%1</span>"
                "<span style='background:rgba(50,100,180,0.5); "
                "color:#80b0e0; padding:1px 6px; border-radius:10px; "
                "font-size:10px; margin-left:6px;'>×%2</span>"
                "</div>"
            ).arg(QString::fromStdString(d)).arg(freq);
        }
    }
    html += "</div>";

    m_detailText->setHtml(html);
}

void MainWindow::showToast(const QString& msg, bool success) {
    m_toastLabel->setText(msg);
    m_toastLabel->setStyleSheet(
        success
        ? "background:rgba(10,60,40,0.95); color:#60e0a0; "
          "border:1px solid #20a060; border-radius:20px; font-size:12px; "
          "font-weight:bold; padding:0 20px;"
        : "background:rgba(60,10,10,0.95); color:#e08080; "
          "border:1px solid #a02020; border-radius:20px; font-size:12px; "
          "font-weight:bold; padding:0 20px;"
    );

    // Position toast at bottom-center of tree canvas
    int tw = qMin(500, m_treeCanvas->width() - 40);
    int tx = m_treeCanvas->mapTo(centralWidget(), QPoint(0,0)).x()
             + (m_treeCanvas->width() - tw) / 2;
    int ty = m_treeCanvas->mapTo(centralWidget(), QPoint(0, m_treeCanvas->height()-60)).y();
    m_toastLabel->setGeometry(tx, ty, tw, 40);
    m_toastLabel->show();
    m_toastLabel->raise();

    m_toastTimer->start(2500);
}
