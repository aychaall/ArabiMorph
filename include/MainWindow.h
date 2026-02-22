#pragma once
#include <QMainWindow>
#include <QTabWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QFrame>
#include <QSpinBox>
#include <QFont>

#include "BinarySearchTree.h"
#include "hashtable.h"
#include "core_engine.h"
#include "TreeVisualizationWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    // Root / AVL
    void onInsertRoot();
    void onSearchRoot();
    void onDeleteRoot();
    void onLoadRootsFromFile();
    void onDisplayAllRoots();
    void onTreeNodeClicked(const QString& rootName);
    void onShowDerivatives();

    // Hash Table / Schemes
    void onInsertScheme();
    void onSearchScheme();
    void onDeleteScheme();
    void onUpdateScheme();
    void onDisplayAllSchemes();
    void onLoadSchemesFromFile();

    // Engine
    void onGenerateWord();
    void onValidateWord();
    void onMorphologicalFamily();

private:
    void setupUI();
    void setupStyleSheet();
    void setupRootTab();
    void setupSchemeTab();
    void setupEngineTab();
    void setupAboutTab();

    void log(const QString& msg, const QString& color = "#e2e8f0");
    void logSuccess(const QString& msg);
    void logError(const QString& msg);
    void logInfo(const QString& msg);
    void updateTreeStats();
    void refreshTreeView();
    void refreshSchemeTable();

    // Data
    BinarySearchTree* m_tree;
    struct hashmap*   m_hashmap;

    // UI elements
    QTabWidget*          m_tabs;

    // Root Tab
    QLineEdit*           m_rootInput;
    QTextEdit*           m_rootLog;
    TreeVisualizationWidget* m_treeViz;
    QScrollArea*         m_treeScroll;
    QLabel*              m_statNodes;
    QLabel*              m_statHeight;
    QLabel*              m_statEmpty;
    QListWidget*         m_derivativesList;
    QLabel*              m_selectedRootLabel;

    // Scheme Tab
    QLineEdit*           m_schemeInput;
    QLineEdit*           m_schemeOldInput;
    QLineEdit*           m_schemeNewInput;
    QTableWidget*        m_schemeTable;
    QTextEdit*           m_schemeLog;

    // Engine Tab
    QLineEdit*           m_engRootInput;
    QLineEdit*           m_engSchemeInput;
    QLineEdit*           m_engWordInput;
    QLineEdit*           m_engValidRootInput;
    QLineEdit*           m_engFamilySchemeInput;
    QTextEdit*           m_engineLog;

    // Status
    QLabel*              m_statusLabel;
};
