#pragma once
#include <QString>
#include <cstdint>

class QComboBox;

namespace etab {

QString w2q(const wchar_t* s);

int32_t parseItemID(const QString& raw);
QString itemDisplay(const QString& raw, bool showIDs);

void fillItemCombo(QComboBox* cb, const wchar_t** items, int count,
                   bool filter, bool sortAlpha, bool showIDs);
void fillPlazaGearCombo(QComboBox* cb, const wchar_t** items, int count,
                        bool sortAlpha, bool showIDs);
void fillAbilityCombo(QComboBox* cb, bool sortAlpha, bool showIDs);

void setComboById(QComboBox* cb, int32_t id);
int32_t comboId(const QComboBox* cb);

int32_t readInt(QComboBox* cb);

QString timeToText(uint32_t secs);
uint32_t textToTime(const QString& text);

}
