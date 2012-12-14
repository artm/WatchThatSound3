#include "Synced.hpp"

using namespace WTS;

Synced::Synced(qint64 at, QObject *parent)
    : QObject(parent)
    , m_at(at)
{
}

