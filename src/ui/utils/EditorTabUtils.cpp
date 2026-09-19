#include "EditorTabUtils.h"
#include "ItemData.h"

#include <QComboBox>
#include <QDateTime>
#include <QTimeZone>
#include <QVector>
#include <algorithm>

namespace etab {

QString w2q(const wchar_t* s) {
    if (!s) return {};
    return QString::fromWCharArray(s);
}

int32_t parseItemID(const QString& raw) {
    int i = 0;
    while (i < raw.size() && (raw[i] == QLatin1Char(' ') || raw[i] == QLatin1Char('[')))
        ++i;
    int j = i;
    bool any = false;
    while (j < raw.size() && raw[j].isDigit()) { any = true; ++j; }
    if (!any) return -1;
    bool ok = false;
    int v = raw.mid(i, j - i).toInt(&ok);
    return ok ? v : -1;
}

QString itemDisplay(const QString& raw, bool showIDs) {
    if (showIDs) return raw;
    int a = raw.indexOf(QLatin1Char('('));
    if (a != -1) {
        int b = raw.indexOf(QLatin1Char(')'), a);
        if (b != -1) return raw.mid(a + 1, b - a - 1);
    }
    int i = 0;
    while (i < raw.size() && (raw[i] == QLatin1Char(' ') || raw[i] == QLatin1Char('[')))
        ++i;
    int j = i;
    bool any = false;
    while (j < raw.size() && raw[j].isDigit()) { any = true; ++j; }
    if (any && j < raw.size() && raw[j] == QLatin1Char(' '))
        return raw.mid(j + 1);
    return raw;
}

struct ComboEntry {
    QString display;
    int32_t id;
};

static void buildList(const wchar_t** items, int count, bool filter,
                      bool sortAlpha, bool showIDs, QVector<ComboEntry>& out) {
    out.clear();
    out.reserve(count);
    for (int i = 0; i < count; ++i) {
        QString raw = w2q(items[i]);
        if (filter && (raw.contains(QLatin1String("(Test_Clt_")) || raw.contains(QLatin1String("(HAP"))))
            continue;
        ComboEntry e;
        e.id = parseItemID(raw);
        e.display = itemDisplay(raw, showIDs);
        out.append(e);
    }
    if (sortAlpha) {
        std::sort(out.begin(), out.end(), [](const ComboEntry& a, const ComboEntry& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    }
}

void fillItemCombo(QComboBox* cb, const wchar_t** items, int count,
                   bool filter, bool sortAlpha, bool showIDs) {
    cb->clear();
    QVector<ComboEntry> list;
    buildList(items, count, filter, sortAlpha, showIDs, list);
    for (const auto& e : list)
        cb->addItem(e.display, QVariant(e.id));
}

void fillPlazaGearCombo(QComboBox* cb, const wchar_t** items, int count,
                        bool sortAlpha, bool showIDs) {
    cb->clear();
    QVector<ComboEntry> list;
    buildList(items, count, true, sortAlpha, showIDs, list);
    for (const auto& e : list)
        cb->addItem(e.display, QVariant(e.id));
}

void fillAbilityCombo(QComboBox* cb, bool sortAlpha, bool showIDs) {
    cb->clear();
    QVector<ComboEntry> list;
    list.reserve(g_AbilityCount);
    for (int i = 0; i < g_AbilityCount; ++i) {
        ComboEntry e;
        e.id = g_AbilityIDs[i];
        e.display = itemDisplay(w2q(g_AbilityNames[i]), showIDs);
        list.append(e);
    }
    if (sortAlpha) {
        std::sort(list.begin(), list.end(), [](const ComboEntry& a, const ComboEntry& b) {
            return a.display.compare(b.display, Qt::CaseInsensitive) < 0;
        });
    }
    for (const auto& e : list)
        cb->addItem(e.display, QVariant(e.id));
}

void setComboById(QComboBox* cb, int32_t id) {
    for (int i = 0; i < cb->count(); ++i) {
        if (cb->itemData(i).toInt() == id) {
            cb->setCurrentIndex(i);
            return;
        }
    }
    if (cb->count() > 0) cb->setCurrentIndex(0);
    else cb->setCurrentIndex(-1);
}

int32_t comboId(const QComboBox* cb) {
    int idx = cb->currentIndex();
    if (idx < 0) return -1;
    return cb->itemData(idx).toInt();
}

int32_t readInt(QComboBox* cb) {
    return comboId(cb);
}

QString timeToText(uint32_t secs) {
    QDateTime dt;
    dt.setSecsSinceEpoch(static_cast<qint64>(secs));
    return dt.toString(QStringLiteral("yyyy-MM-dd HH:mm:ss"));
}

uint32_t textToTime(const QString& text) {
    QDateTime dt = QDateTime::fromString(text, QStringLiteral("yyyy-MM-dd HH:mm:ss"));
    if (!dt.isValid()) return 0;
    dt.setTimeZone(QTimeZone::systemTimeZone());
    return static_cast<uint32_t>(dt.toSecsSinceEpoch());
}

}
