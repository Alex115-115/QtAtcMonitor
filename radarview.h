#pragma once

#include <QWidget>
#include "flightsmodel.h"

class RadarView : public QWidget
{
    Q_OBJECT

public:
    explicit RadarView(QWidget *parent = nullptr);

    void setModel(FlightsModel *model);

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onModelChanged();

private:
    FlightsModel *m_model = nullptr;
};
