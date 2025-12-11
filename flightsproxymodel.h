#pragma once

#include <QSortFilterProxyModel>

class FlightsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit FlightsProxyModel(QObject *parent = nullptr);

    void setOnlyConflicts(bool onlyConflicts);
    bool onlyConflicts() const { return m_onlyConflicts; }

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

private:
    bool m_onlyConflicts = false;
};
