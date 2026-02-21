#include "../include/TreeVisualizationWidget.h"
#include <QPainterPath>
#include <QFontMetrics>
#include <QtMath>
#include <algorithm>

// ─────────────────────────────────────────────
// TreeCanvas
// ─────────────────────────────────────────────
TreeCanvas::TreeCanvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(600, 400);
    setMouseTracking(true);
    setAttribute(Qt::WA_OpaquePaintEvent);

    m_pulseTimer = new QTimer(this);
    connect(m_pulseTimer, &QTimer::timeout, this, [this]() {
        m_pulsePhase += 0.08f;
        if (m_pulsePhase > 2 * M_PI) m_pulsePhase -= 2 * M_PI;
        update();
    });
    m_pulseTimer->start(30);

    m_pan = QPointF(0, 0);
}

void TreeCanvas::setTree(BinarySearchTree* tree) {
    m_tree = tree;
    computeLayout();
    update();
}

void TreeCanvas::setHighlightedNode(const QString& root) {
    m_highlighted = root;
    update();
}

void TreeCanvas::clearHighlight() {
    m_highlighted.clear();
    update();
}

void TreeCanvas::animateInsert(const QString& root) {
    m_justAdded = root;
    m_highlighted = root;
    computeLayout();

    QPropertyAnimation* anim = new QPropertyAnimation(this, "animProgress");
    anim->setDuration(500);
    anim->setStartValue(0.0f);
    anim->setEndValue(1.0f);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    update();
}

void TreeCanvas::animateDelete(const QString& root) {
    m_highlighted.clear();
    m_justAdded.clear();
    computeLayout();
    update();
}

void TreeCanvas::setAnimProgress(float v) {
    m_animProgress = v;
    update();
}

void TreeCanvas::zoomIn()    { m_zoom = qMin(m_zoom * 1.2, 4.0); update(); }
void TreeCanvas::zoomOut()   { m_zoom = qMax(m_zoom / 1.2, 0.2); update(); }
void TreeCanvas::resetView() { m_zoom = 1.0; m_pan = QPointF(0,0); update(); }

// ── Layout ──────────────────────────────────
void TreeCanvas::computeLayout() {
    m_layouts.clear();
    if (!m_tree || m_tree->isEmpty()) return;
    computeLayout(m_tree->getRoot(), 0.0, 0.0, 400.0);
}

void TreeCanvas::computeLayout(Node* node, double x, double y, double spread) {
    if (!node) return;
    NodeLayout lay;
    lay.x = x;  lay.y = y;
    lay.highlighted = (QString::fromStdString(node->getData()) == m_highlighted);
    lay.justAdded   = (QString::fromStdString(node->getData()) == m_justAdded);
    m_layouts[QString::fromStdString(node->getData())] = lay;

    double childY = y + LEVEL_HEIGHT;
    double half   = spread / 2.0;
    computeLayout(node->getLeft(),  x - half, childY, half);
    computeLayout(node->getRight(), x + half, childY, half);
}

// ── Paint ────────────────────────────────────
void TreeCanvas::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setRenderHint(QPainter::TextAntialiasing, true);

    // Background gradient
    QLinearGradient bg(0, 0, 0, height());
    bg.setColorAt(0.0, QColor(10, 12, 22));
    bg.setColorAt(1.0, QColor(18, 22, 42));
    p.fillRect(rect(), bg);

    // Grid dots
    p.setPen(QColor(255,255,255,18));
    for (int gx = 0; gx < width(); gx += 40)
        for (int gy = 0; gy < height(); gy += 40)
            p.drawEllipse(QPointF(gx, gy), 1, 1);

    if (!m_tree || m_tree->isEmpty()) {
        drawEmptyState(p);
        return;
    }

    // Apply pan + zoom centered on widget
    p.save();
    p.translate(width() / 2.0 + m_pan.x(), 80 + m_pan.y());
    p.scale(m_zoom, m_zoom);

    // Draw edges first
    drawEdges(p, m_tree->getRoot());
    // Then nodes on top
    drawNodes(p, m_tree->getRoot());

    p.restore();

    // HUD: node count + height
    int count  = m_tree->getNodeCount();
    int height_ = m_tree->getHeight();
    QString info = QString("Nodes: %1   Height: %2   Zoom: %3×")
                   .arg(count).arg(height_).arg(m_zoom, 0, 'f', 1);

    QFont hudFont("Courier New", 10);
    p.setFont(hudFont);
    p.setPen(QColor(100, 180, 255, 160));
    p.drawText(QRect(12, height() - 28, width() - 24, 20),
               Qt::AlignLeft | Qt::AlignVCenter, info);
}

void TreeCanvas::drawEdges(QPainter& p, Node* node) {
    if (!node) return;

    auto drawCurve = [&](Node* from, Node* to) {
        if (!from || !to) return;
        QString fk = QString::fromStdString(from->getData());
        QString tk = QString::fromStdString(to->getData());
        if (!m_layouts.contains(fk) || !m_layouts.contains(tk)) return;

        QPointF fp(m_layouts[fk].x, m_layouts[fk].y);
        QPointF tp(m_layouts[tk].x, m_layouts[tk].y);

        // Control point for bezier
        QPointF cp((fp.x() + tp.x()) / 2, (fp.y() + tp.y()) / 2 - 20);

        QPainterPath path;
        path.moveTo(fp.x(), fp.y() + NODE_RADIUS);
        path.quadTo(cp, QPointF(tp.x(), tp.y() - NODE_RADIUS));

        // Edge glow
        bool hiFrom = m_layouts[fk].highlighted;
        bool hiTo   = m_layouts[tk].highlighted;
        if (hiFrom || hiTo) {
            QPen glowPen(QColor(80, 200, 255, 60), 8);
            glowPen.setCapStyle(Qt::RoundCap);
            p.strokePath(path, glowPen);
        }

        QLinearGradient eg(fp, tp);
        eg.setColorAt(0.0, QColor(60, 130, 220, 180));
        eg.setColorAt(1.0, QColor(100, 200, 255, 120));
        QPen edgePen(QBrush(eg), 2.0);
        edgePen.setCapStyle(Qt::RoundCap);
        p.strokePath(path, edgePen);
    };

    drawCurve(node, node->getLeft());
    drawCurve(node, node->getRight());

    drawEdges(p, node->getLeft());
    drawEdges(p, node->getRight());
}

void TreeCanvas::drawNodes(QPainter& p, Node* node) {
    if (!node) return;
    drawNodes(p, node->getLeft());
    drawNodes(p, node->getRight());

    QString key = QString::fromStdString(node->getData());
    if (m_layouts.contains(key))
        drawNode(p, node, m_layouts[key]);
}

void TreeCanvas::drawNode(QPainter& p, Node* node, const NodeLayout& lay) {
    double cx = lay.x, cy = lay.y;
    double r  = NODE_RADIUS;

    bool isHighlighted = lay.highlighted;
    bool isJustAdded   = lay.justAdded;

    // Scale animation for newly added node
    double scale = 1.0;
    if (isJustAdded && m_animProgress < 1.0f) {
        scale = 0.3 + 0.7 * m_animProgress;
    }

    // Pulse for highlighted
    float pulse = isHighlighted ? (0.08f * sinf(m_pulsePhase)) : 0.0f;
    double rr = (r + r * pulse) * scale;

    p.save();

    // Outer glow for highlighted
    if (isHighlighted) {
        QRadialGradient glow(cx, cy, rr * 2.5);
        glow.setColorAt(0.0, QColor(80, 200, 255, 80));
        glow.setColorAt(1.0, QColor(0, 0, 0, 0));
        p.setBrush(glow);
        p.setPen(Qt::NoPen);
        p.drawEllipse(QPointF(cx, cy), rr * 2.5, rr * 2.5);
    }

    // Shadow
    p.setBrush(QColor(0,0,0,100));
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx + 3, cy + 4), rr, rr);

    // Node body gradient
    QRadialGradient grad(cx - rr*0.3, cy - rr*0.3, rr * 2.0);
    if (isJustAdded) {
        grad.setColorAt(0.0, QColor(80, 220, 150));
        grad.setColorAt(0.5, QColor(30, 160, 100));
        grad.setColorAt(1.0, QColor(10, 80, 60));
    } else if (isHighlighted) {
        grad.setColorAt(0.0, QColor(120, 200, 255));
        grad.setColorAt(0.5, QColor(50, 130, 220));
        grad.setColorAt(1.0, QColor(20, 60, 140));
    } else {
        // Depth-based color: root is brightest
        int depth = (int)(lay.y / LEVEL_HEIGHT);
        int b = qMax(60, 200 - depth * 30);
        grad.setColorAt(0.0, QColor(b, b+20, 255));
        grad.setColorAt(0.5, QColor(b/2, b/2+10, 180));
        grad.setColorAt(1.0, QColor(b/4, b/4, 80));
    }

    p.setBrush(grad);

    // Border
    QPen borderPen;
    if (isHighlighted)
        borderPen = QPen(QColor(120, 220, 255), 2.5);
    else if (isJustAdded)
        borderPen = QPen(QColor(80, 255, 160), 2.5);
    else
        borderPen = QPen(QColor(80, 120, 200, 180), 1.5);
    p.setPen(borderPen);
    p.drawEllipse(QPointF(cx, cy), rr, rr);

    // Inner highlight shimmer
    QRadialGradient shimmer(cx - rr * 0.3, cy - rr * 0.4, rr * 0.7);
    shimmer.setColorAt(0.0, QColor(255, 255, 255, 80));
    shimmer.setColorAt(1.0, QColor(255, 255, 255, 0));
    p.setBrush(shimmer);
    p.setPen(Qt::NoPen);
    p.drawEllipse(QPointF(cx, cy), rr, rr);

    // ── Text ──────────────────────
    QString text = QString::fromStdString(node->getData());
    int derivCount = node->getRootObject().getDerivativeCount();

    QFont mainFont("Arial", 11, QFont::Bold);
    p.setFont(mainFont);
    p.setPen(QColor(255, 255, 255, 230));

    // Truncate long Arabic text
    QFontMetrics fm(mainFont);
    QString display = fm.elidedText(text, Qt::ElideRight, (int)(rr * 1.6));

    // Center text in circle
    QRect textRect((int)(cx - rr), (int)(cy - rr/2), (int)(rr*2), (int)(rr));
    p.drawText(textRect, Qt::AlignCenter, display);

    // Derivative count badge
    if (derivCount > 0) {
        double bx = cx + rr * 0.7, by = cy - rr * 0.7;
        p.setBrush(QColor(255, 160, 60));
        p.setPen(QColor(200, 100, 20));
        p.drawEllipse(QPointF(bx, by), 10, 10);
        QFont badgeFont("Arial", 7, QFont::Bold);
        p.setFont(badgeFont);
        p.setPen(Qt::white);
        p.drawText(QRect((int)(bx-9),(int)(by-8),18,16), Qt::AlignCenter,
                   QString::number(derivCount));
    }

    // Height label (small)
    QFont hFont("Courier New", 7);
    p.setFont(hFont);
    p.setPen(QColor(200, 220, 255, 120));
    p.drawText(QRect((int)(cx - rr), (int)(cy + rr*0.1), (int)(rr*2), (int)(rr*0.8)),
               Qt::AlignCenter, QString("h=%1").arg(node->getHeight()));

    p.restore();
}

void TreeCanvas::drawEmptyState(QPainter& p) {
    p.save();
    float alpha = 0.5f + 0.5f * sinf(m_pulsePhase * 0.5f);
    p.setOpacity(alpha);

    QFont f("Arial", 22);
    p.setFont(f);
    p.setPen(QColor(80, 130, 200));
    p.drawText(rect(), Qt::AlignCenter,
               QString("∅\n\nالشجرة فارغة\nTree is empty\n\nAdd a root to begin"));
    p.restore();
}

// ── Hit testing ─────────────────────────────
QString TreeCanvas::hitTest(const QPointF& screenPos) {
    // Transform screen pos to tree coords
    QPointF tp = (screenPos - QPointF(width()/2.0 + m_pan.x(), 80 + m_pan.y())) / m_zoom;

    for (auto it = m_layouts.begin(); it != m_layouts.end(); ++it) {
        QPointF nc(it.value().x, it.value().y);
        if (QLineF(tp, nc).length() <= NODE_RADIUS * 1.2)
            return it.key();
    }
    return QString();
}

// ── Mouse ────────────────────────────────────
void TreeCanvas::mousePressEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        QString hit = hitTest(e->pos());
        if (!hit.isEmpty()) {
            setHighlightedNode(hit);
            emit nodeClicked(hit);
        } else {
            m_dragging = true;
            m_lastMouse = e->pos();
        }
    }
}

void TreeCanvas::mouseMoveEvent(QMouseEvent* e) {
    if (m_dragging) {
        QPointF delta = e->pos() - m_lastMouse;
        m_pan += delta;
        m_lastMouse = e->pos();
        update();
    }
    // Cursor change
    QString hit = hitTest(e->pos());
    setCursor(hit.isEmpty() ? Qt::ArrowCursor : Qt::PointingHandCursor);
}

void TreeCanvas::mouseReleaseEvent(QMouseEvent*) { m_dragging = false; }

void TreeCanvas::wheelEvent(QWheelEvent* e) {
    double factor = e->angleDelta().y() > 0 ? 1.12 : 0.9;
    m_zoom = qBound(0.2, m_zoom * factor, 4.0);
    update();
}

void TreeCanvas::resizeEvent(QResizeEvent*) { update(); }
