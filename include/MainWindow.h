#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QGroupBox>
#include <QSplitter>
#include <QStatusBar>
#include <QScrollArea>
#include <QToolButton>
#include <QFrame>
#include <QToolButton>
#include "../include/BinarySearchTree.h"
#include "../include/TreeVisualizationWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onInsertClicked();
    void onSearchClicked();
    void onDeleteClicked();
    void onAddDerivativeClicked();
    void onNodeSelected(const QString& rootName);
    void onLoadFileClicked();
    void refreshStats();
    void refreshRootList();
    void onRootListItemClicked(QListWidgetItem* item);

private:
    void setupUI();
    void setupStylesheet();
    void showNodeDetails(const QString& rootName);
    void updateTreeView();
    QString buildDerivativesHtml(Node* node);
    void showToast(const QString& msg, bool success = true);

    // Data
    BinarySearchTree* m_tree;

    // Central
    QSplitter*     m_splitter;
    TreeCanvas*    m_treeCanvas;

    // Left panel
    QWidget*       m_leftPanel;
    QLineEdit*     m_rootInput;
    QLineEdit*     m_searchInput;
    QLineEdit*     m_derivativeInput;
    QPushButton*   m_insertBtn;
    QPushButton*   m_searchBtn;
    QPushButton*   m_deleteBtn;
    QPushButton*   m_addDerivBtn;
    QPushButton*   m_loadFileBtn;
    QToolButton*   m_zoomInBtn;
    QToolButton*   m_zoomOutBtn;
    QToolButton*   m_resetViewBtn;
    QListWidget*   m_rootList;

    // Right panel: details
    QWidget*       m_detailPanel;
    QLabel*        m_detailTitle;
    QTextEdit*     m_detailText;
    QLabel*        m_statsLabel;

    // Toast
    QLabel*        m_toastLabel;
    QTimer*        m_toastTimer;
};
