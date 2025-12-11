#include "radarview.h"

#include <QPainter>
#include <QPaintEvent>
#include <QtMath>
#include <limits>

RadarView::RadarView(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(200);
}

void RadarView::setModel(FlightsModel *model)
{
    if (m_model) {
        disconnect(m_model, nullptr, this, nullptr);
    }

    m_model = model;

    if (m_model) {
        connect(m_model, &QAbstractItemModel::dataChanged,
                this, &RadarView::onModelChanged);
        connect(m_model, &QAbstractItemModel::modelReset,
                this, &RadarView::onModelChanged);
        connect(m_model, &QAbstractItemModel::rowsInserted,
                this, &RadarView::onModelChanged);
        connect(m_model, &QAbstractItemModel::rowsRemoved,
                this, &RadarView::onModelChanged);
    }

    update();
}

void RadarView::onModelChanged()
{
    update();
}

void RadarView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const int w = width();
    const int h = height();

    p.fillRect(rect(), Qt::black);

    int size = qMin(w, h) - 20;
    if (size <= 0)
        return;

    QPoint center(w / 2, h / 2);
    int radius = size / 2;

    QPen pen(Qt::green);
    pen.setWidth(1);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(center, radius, radius);

    if (!m_model || m_model->rowCount() == 0) {
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, tr("No flights"));
        return;
    }

    double minLat = std::numeric_limits<double>::max();
    double maxLat = std::numeric_limits<double>::lowest();
    double minLon = std::numeric_limits<double>::max();
    double maxLon = std::numeric_limits<double>::lowest();

    for (int row = 0; row < m_model->rowCount(); ++row) {
        double lat = m_model->data(m_model->index(row, 1)).toDouble(); // col 1 = Lat
        double lon = m_model->data(m_model->index(row, 2)).toDouble(); // col 2 = Lon
        minLat = std::min(minLat, lat);
        maxLat = std::max(maxLat, lat);
        minLon = std::min(minLon, lon);
        maxLon = std::max(maxLon, lon);
    }

    if (qFuzzyCompare(minLat, maxLat)) {
        minLat -= 0.5;
        maxLat += 0.5;
    }
    if (qFuzzyCompare(minLon, maxLon)) {
        minLon -= 0.5;
        maxLon += 0.5;
    }

    for (int row = 0; row < m_model->rowCount(); ++row) {
        double lat = m_model->data(m_model->index(row, 1)).toDouble();
        double lon = m_model->data(m_model->index(row, 2)).toDouble();
        QString status = m_model->data(m_model->index(row, 6)).toString();
        QString id = m_model->data(m_model->index(row, 0)).toString();

        bool conflict = (status == "CONFLICT");

        double xNorm = (lon - minLon) / (maxLon - minLon);
        double yNorm = (lat - minLat) / (maxLat - minLat);

        double dx = (xNorm - 0.5) * 2.0;
        double dy = (0.5 - yNorm) * 2.0;

        double r = std::sqrt(dx*dx + dy*dy);
        if (r > 1.0) {
            dx /= r;
            dy /= r;
        }

        QPoint pt(center.x() + int(dx * radius),
                  center.y() + int(dy * radius));

        p.setPen(Qt::NoPen);
        p.setBrush(conflict ? Qt::red : Qt::green);
        p.drawEllipse(pt, 4, 4);

        p.setPen(Qt::white);
        p.drawText(pt + QPoint(6, -6), id);
    }
}
