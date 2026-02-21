#pragma once
#include <QWidget>
#include <QScrollArea>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QPropertyAnimation>
#include <QMap>
#include <QPointF>
#include <QFont>
#include <functional>
#include "../include/BinarySearchTree.h"

// Stores computed layout info per node
struct NodeLayout {
    QPointF center;
    double x = 0, y = 0;
    double targetX = 0, targetY = 0;
    double animX = 0, animY = 0;
    bool highlighted = false;
    bool justAdded = false;
    float pulse = 0.0f;
};

class TreeCanvas : public QWidget {
    Q_OBJECT
    Q_PROPERTY(float animProgress READ animProgress WRITE setAnimProgress)

public:
    explicit TreeCanvas(QWidget* parent = nullptr);

    void setTree(BinarySearchTree* tree);
    void setHighlightedNode(const QString& root);
    void clearHighlight();
    void animateInsert(const QString& root);
    void animateDelete(const QString& root);

    float animProgress() const { return m_animProgress; }
    void  setAnimProgress(float v);

    void zoomIn();
    void zoomOut();
    void resetView();

signals:
    void nodeClicked(const QString& rootName);

protected:
    void paintEvent(QPaintEvent*) override;
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    void resizeEvent(QResizeEvent*) override;

private:
    void computeLayout();
    void computeLayout(Node* node, double x, double y, double spread);
    void drawEdges(QPainter& p, Node* node);
    void drawNodes(QPainter& p, Node* node);
    void drawNode(QPainter& p, Node* node, const NodeLayout& layout);
    void drawEmptyState(QPainter& p);

    QString hitTest(const QPointF& pos);
    QPointF nodeScreenPos(const QString& key);

    BinarySearchTree* m_tree = nullptr;
    QMap<QString, NodeLayout> m_layouts;

    // Pan/zoom
    QPointF m_pan;
    qreal   m_zoom = 1.0;
    QPointF m_lastMouse;
    bool    m_dragging = false;

    QString m_highlighted;
    QString m_justAdded;
    float   m_animProgress = 0.0f;

    QTimer*             m_pulseTimer;
    float               m_pulsePhase = 0.0f;

    // layout constants
    const double NODE_RADIUS = 36.0;
    const double LEVEL_HEIGHT = 110.0;
};
