#include "flightsproxymodel.h"
#include <QModelIndex>
#include <QVariant>

FlightsProxyModel::FlightsProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void FlightsProxyModel::setOnlyConflicts(bool onlyConflicts)
{
    if (m_onlyConflicts == onlyConflicts)
        return;

    m_onlyConflicts = onlyConflicts;
    invalidateFilter();
}

bool FlightsProxyModel::filterAcceptsRow(int sourceRow,
                                         const QModelIndex &sourceParent) const
{
    if (!m_onlyConflicts)
        return true;

    if (!sourceModel())
        return false;

    QModelIndex statusIndex = sourceModel()->index(sourceRow, 6, sourceParent);
    const QString status =
        sourceModel()->data(statusIndex, Qt::DisplayRole).toString();

    return (status == QStringLiteral("CONFLICT"));
}
