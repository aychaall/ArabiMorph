#pragma once
#include <QWidget>
#include <QMap>
#include <QPoint>
#include <QString>
#include "../include/BinarySearchTree.h"

class TreeVisualizationWidget : public QWidget {
    Q_OBJECT
public:
    explicit TreeVisualizationWidget(QWidget* parent = nullptr);
    void setTree(BinarySearchTree* tree);
    void refresh();
    void selectNode(const QString& name) { m_selectedNode = name; update(); }

    // Zoom controls (also callable from MainWindow zoom buttons)
    void zoomIn();
    void zoomOut();
    void resetZoom();

    QSize sizeHint() const override;

signals:
    void nodeClicked(const QString& rootName);

protected:
    void paintEvent(QPaintEvent*)     override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*)  override;
    void wheelEvent(QWheelEvent*)      override;

private:
    BinarySearchTree* m_tree;

    // Zoom
    double m_zoomFactor = 1.0;

    // Layout constants (unscaled)
    const int m_nodeWidth  = 80;
    const int m_nodeHeight = 40;
    const int m_levelGap   = 90;
    const int m_hPad       = 20;

    // Runtime state
    QMap<QString, QPoint> m_nodePositions;
    QMap<QString, int>    m_depthMap;
    QString               m_selectedNode;
    QString               m_hoveredNode;

    // Helpers
    int  treeWidth(Node* node, int spread) const;
    int  treeHeight() const;
    void computePositions(Node* node, int x, int y, int spread, int depth = 1);
    void drawEdge(QPainter& p, QPoint from, QPoint to, bool isLeft);
};
