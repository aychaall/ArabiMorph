#include "../include/TreeVisualizationWidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainterPath>
#include <cmath>

TreeVisualizationWidget::TreeVisualizationWidget(QWidget* parent)
    : QWidget(parent), m_tree(nullptr), m_zoomFactor(1.0) {
    setMinimumSize(600, 400);
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
}

void TreeVisualizationWidget::setTree(BinarySearchTree* tree) {
    m_tree = tree;
    refresh();
}

void TreeVisualizationWidget::refresh() {
    m_nodePositions.clear();
    if (m_tree && !m_tree->isEmpty()) {
        int scaledNodeW  = static_cast<int>(m_nodeWidth  * m_zoomFactor);
        int scaledLevelG = static_cast<int>(m_levelGap   * m_zoomFactor);
        int w = (m_tree->getNodeCount() + 1) * (scaledNodeW + m_hPad) + 200;
        int h = (m_tree->getHeight() * scaledLevelG) + 150;
        setMinimumSize(qMax(w, 600), qMax(h, 400));
    }
    update();
}

void TreeVisualizationWidget::zoomIn()    { m_zoomFactor = qMin(m_zoomFactor * 1.2, 3.0); refresh(); }
void TreeVisualizationWidget::zoomOut()   { m_zoomFactor = qMax(m_zoomFactor / 1.2, 0.2); refresh(); }
void TreeVisualizationWidget::resetZoom() { m_zoomFactor = 1.0; refresh(); }

QSize TreeVisualizationWidget::sizeHint() const {
    if (!m_tree || m_tree->isEmpty()) return QSize(600, 400);
    int scaledLevelG = static_cast<int>(m_levelGap * m_zoomFactor);
    int scaledNodeW  = static_cast<int>(m_nodeWidth * m_zoomFactor);
    int w = (m_tree->getNodeCount() + 1) * (scaledNodeW + m_hPad) + 200;
    int h = m_tree->getHeight() * scaledLevelG + 150;
    return QSize(qMax(w, 600), qMax(h, 400));
}

int TreeVisualizationWidget::treeWidth(Node* node, int spread) const {
    if (!node) return 0;
    return spread * 2;
}

int TreeVisualizationWidget::treeHeight() const {
    if (!m_tree) return 0;
    return static_cast<int>(m_tree->getHeight() * m_levelGap * m_zoomFactor) + 150;
}

void TreeVisualizationWidget::computePositions(Node* node, int x, int y, int spread, int depth) {
    if (!node) return;
    QString key = QString::fromStdString(node->getData());
    m_nodePositions[key] = QPoint(x, y);
    m_depthMap[key]      = depth;

    int scaledLevelG = static_cast<int>(m_levelGap * m_zoomFactor);
    int childSpread  = qMax(spread / 2,
                           static_cast<int>((m_nodeWidth * m_zoomFactor) / 2 + m_hPad));
    computePositions(node->getLeft(),  x - childSpread, y + scaledLevelG, childSpread, depth + 1);
    computePositions(node->getRight(), x + childSpread, y + scaledLevelG, childSpread, depth + 1);
}

void TreeVisualizationWidget::drawEdge(QPainter& p, QPoint from, QPoint to, bool isLeft) {
    Q_UNUSED(isLeft)
    int scaledNodeH  = static_cast<int>(m_nodeHeight * m_zoomFactor);
    int scaledLevelG = static_cast<int>(m_levelGap   * m_zoomFactor);

    QPen pen(QColor(100, 149, 237, 180), 2.0, Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    p.setPen(pen);

    QPainterPath path;
    path.moveTo(from.x(), from.y() + scaledNodeH / 2);
    QPoint ctrl1(from.x(), from.y() + scaledLevelG * 0.5);
    QPoint ctrl2(to.x(),   to.y()   - scaledLevelG * 0.3);
    path.cubicTo(ctrl1, ctrl2, QPoint(to.x(), to.y() - scaledNodeH / 2));
    p.drawPath(path);

    QPointF end(to.x(), to.y() - scaledNodeH / 2);
    QPointF dir = end - QPointF(ctrl2);
    double len = std::sqrt(dir.x()*dir.x() + dir.y()*dir.y());
    if (len > 0) {
        dir /= len;
        QPointF left(-dir.y(), dir.x());
        double arrowSize = qMax(4.0, 7.0 * m_zoomFactor);
        QPolygonF arrow;
        arrow << end
              << end - arrowSize * dir + arrowSize * 0.5 * left
              << end - arrowSize * dir - arrowSize * 0.5 * left;
        p.setBrush(QColor(100, 149, 237, 180));
        p.drawPolygon(arrow);
    }
}

void TreeVisualizationWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);

    QLinearGradient bgGrad(0, 0, 0, height());
    bgGrad.setColorAt(0, QColor(15, 20, 40));
    bgGrad.setColorAt(1, QColor(10, 15, 30));
    p.fillRect(rect(), bgGrad);

    p.setPen(QPen(QColor(255, 255, 255, 8), 1));
    for (int x = 0; x < width(); x += 30) p.drawLine(x, 0, x, height());
    for (int y = 0; y < height(); y += 30) p.drawLine(0, y, width(), y);

    if (!m_tree || m_tree->isEmpty()) {
        p.setPen(QColor(120, 140, 180));
        p.setFont(QFont("Arial", 14));
        p.drawText(rect(), Qt::AlignCenter, "Insert roots to visualize the AVL tree");
        return;
    }

    // Zoom indicator
    p.setPen(QColor(88, 166, 255));
    p.setFont(QFont("Courier New", 9));
    p.drawText(8, 18, QString("Zoom: %1%   (scroll to zoom)").arg(qRound(m_zoomFactor * 100)));

    m_nodePositions.clear();
    m_depthMap.clear();

    int scaledNodeW  = static_cast<int>(m_nodeWidth  * m_zoomFactor);
    int scaledLevelG = static_cast<int>(m_levelGap   * m_zoomFactor);
    int startX       = qMax(width() / 2,
                      scaledNodeW * (m_tree->getNodeCount() / 2 + 1));
    int initSpread   = qMax(static_cast<int>(width() / 4),
                          scaledNodeW * (m_tree->getNodeCount() / 2 + 1));
    computePositions(m_tree->getRoot(), startX, 60, initSpread, 1);

    // Edges
    for (auto it = m_nodePositions.begin(); it != m_nodePositions.end(); ++it) {
        Node* nd = m_tree->getRootNode(it.key().toStdString());
        if (!nd) continue;
        if (nd->getLeft()) {
            QString lkey = QString::fromStdString(nd->getLeft()->getData());
            if (m_nodePositions.contains(lkey))
                drawEdge(p, it.value(), m_nodePositions[lkey], true);
        }
        if (nd->getRight()) {
            QString rkey = QString::fromStdString(nd->getRight()->getData());
            if (m_nodePositions.contains(rkey))
                drawEdge(p, it.value(), m_nodePositions[rkey], false);
        }
    }

    // Nodes
    int scaledNodeH = static_cast<int>(m_nodeHeight * m_zoomFactor);
    int badgeW      = qMax(24, static_cast<int>(26 * m_zoomFactor));
    int badgeH      = qMax(14, static_cast<int>(16 * m_zoomFactor));
    int badgeFontSz = qMax(6,  static_cast<int>(7  * m_zoomFactor));
    int rootFontSz  = qMax(7,  static_cast<int>(10 * m_zoomFactor));
    int dotSz       = qMax(6,  static_cast<int>(8  * m_zoomFactor));

    for (auto it = m_nodePositions.begin(); it != m_nodePositions.end(); ++it) {
        QString key = it.key();
        QPoint  pos = it.value();
        bool isSelected = (key == m_selectedNode);
        bool isHovered  = (key == m_hoveredNode);

        int nx = pos.x() - scaledNodeW / 2;
        int ny = pos.y() - scaledNodeH / 2;

        // Glow
        if (isSelected) {
            for (int g = 3; g >= 1; g--) {
                p.setBrush(Qt::NoBrush);
                p.setPen(QPen(QColor(72, 199, 142, 60/g), g*3));
                p.drawRoundedRect(nx-g*2, ny-g*2, scaledNodeW+g*4, scaledNodeH+g*4, 12, 12);
            }
        } else if (isHovered) {
            for (int g = 2; g >= 1; g--) {
                p.setBrush(Qt::NoBrush);
                p.setPen(QPen(QColor(100, 149, 237, 50/g), g*2));
                p.drawRoundedRect(nx-g, ny-g, scaledNodeW+g*2, scaledNodeH+g*2, 12, 12);
            }
        }

        // Background
        QLinearGradient nodeGrad(nx, ny, nx, ny + scaledNodeH);
        if (isSelected) {
            nodeGrad.setColorAt(0, QColor(50, 120, 90));
            nodeGrad.setColorAt(1, QColor(30, 80, 60));
        } else if (isHovered) {
            nodeGrad.setColorAt(0, QColor(50, 70, 130));
            nodeGrad.setColorAt(1, QColor(30, 50, 100));
        } else {
            nodeGrad.setColorAt(0, QColor(35, 45, 80));
            nodeGrad.setColorAt(1, QColor(20, 28, 55));
        }
        p.setBrush(nodeGrad);

        QPen borderPen;
        if (isSelected)     borderPen = QPen(QColor(72, 199, 142), 2);
        else if (isHovered) borderPen = QPen(QColor(100, 149, 237), 1.5);
        else                borderPen = QPen(QColor(60, 80, 140), 1);
        p.setPen(borderPen);
        p.drawRoundedRect(nx, ny, scaledNodeW, scaledNodeH, 10, 10);

        // Depth badge — root=1, leaves=max
        Node* nd       = m_tree->getRootNode(key.toStdString());
        int depth      = m_depthMap.value(key, 0);
        int derivCount = nd ? nd->getRootObject().getDerivativeCount() : 0;

        p.setBrush(QColor(80, 100, 180, 200));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(nx + scaledNodeW - badgeW - 2, ny + 2, badgeW, badgeH, 4, 4);
        p.setPen(QColor(180, 200, 255));
        p.setFont(QFont("Courier New", badgeFontSz, QFont::Bold));
        p.drawText(QRect(nx + scaledNodeW - badgeW - 2, ny + 2, badgeW, badgeH),
                   Qt::AlignCenter, QString("d:%1").arg(depth));

        // Root text
        p.setPen(isSelected ? QColor(180, 255, 200) : QColor(220, 230, 255));
        p.setFont(QFont("Arial", rootFontSz, QFont::Bold));
        p.setLayoutDirection(Qt::RightToLeft);
        p.drawText(QRect(nx + 4, ny + 6, scaledNodeW - badgeW - 6, scaledNodeH - 12),
                   Qt::AlignCenter, key);
        p.setLayoutDirection(Qt::LeftToRight);

        // Derivative count dot
        if (derivCount > 0) {
            int dotX = nx + 6;
            int dotY = ny + scaledNodeH - dotSz - 4;
            p.setBrush(QColor(72, 199, 142));
            p.setPen(Qt::NoPen);
            p.drawEllipse(dotX, dotY, dotSz, dotSz);
            p.setPen(QColor(15, 30, 20));
            p.setFont(QFont("Arial", qMax(5, dotSz - 3), QFont::Bold));
            p.drawText(QRect(dotX, dotY, dotSz, dotSz),
                       Qt::AlignCenter, QString::number(derivCount));
        }
    }

    // Legend
    p.setFont(QFont("Arial", 8));
    p.setPen(QColor(100, 120, 180));
    p.drawText(10, height() - 40,
               "● Green dot = has derivatives  |  h:N = hauteur AVL (feuilles=1, racine=max)  |  Click node  |  Scroll to zoom");
}

void TreeVisualizationWidget::mousePressEvent(QMouseEvent* event) {
    int scaledNodeW = static_cast<int>(m_nodeWidth  * m_zoomFactor);
    int scaledNodeH = static_cast<int>(m_nodeHeight * m_zoomFactor);
    for (auto it = m_nodePositions.begin(); it != m_nodePositions.end(); ++it) {
        QRect nodeRect(it.value().x() - scaledNodeW/2,
                       it.value().y() - scaledNodeH/2,
                       scaledNodeW, scaledNodeH);
        if (nodeRect.contains(event->pos())) {
            m_selectedNode = it.key();
            emit nodeClicked(it.key());
            update();
            return;
        }
    }
    m_selectedNode.clear();
    update();
}

void TreeVisualizationWidget::mouseMoveEvent(QMouseEvent* event) {
    int scaledNodeW = static_cast<int>(m_nodeWidth  * m_zoomFactor);
    int scaledNodeH = static_cast<int>(m_nodeHeight * m_zoomFactor);
    QString prev = m_hoveredNode;
    m_hoveredNode.clear();
    for (auto it = m_nodePositions.begin(); it != m_nodePositions.end(); ++it) {
        QRect nodeRect(it.value().x() - scaledNodeW/2,
                       it.value().y() - scaledNodeH/2,
                       scaledNodeW, scaledNodeH);
        if (nodeRect.contains(event->pos())) {
            m_hoveredNode = it.key();
            setCursor(Qt::PointingHandCursor);
            break;
        }
    }
    if (m_hoveredNode.isEmpty()) setCursor(Qt::ArrowCursor);
    if (m_hoveredNode != prev) update();
}

void TreeVisualizationWidget::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y();
    if (delta > 0)
        m_zoomFactor = qMin(m_zoomFactor * 1.15, 3.0);
    else
        m_zoomFactor = qMax(m_zoomFactor / 1.15, 0.2);
    refresh();
    event->accept();
}
