#pragma once

#include <QWidget>
#include <QList>
#include <QVector>
#include "SaveManager.h"

class QComboBox;
class QListWidget;
class QCheckBox;
class QLineEdit;
class QPushButton;
class QTabWidget;
class QListWidgetItem;
class QGroupBox;

class EditorTabBase : public QWidget {
public:
    explicit EditorTabBase(QWidget* parent = nullptr) : QWidget(parent) {}
    virtual ~EditorTabBase() = default;

    virtual void loadData(SaveData* data) = 0;
    virtual void saveData(SaveData* data) = 0;
    virtual void refreshDisplay() {}

    void setShowInternalIDs(bool v) { m_showIDs = v; refreshDisplay(); }
    void setSortByAlphabet(bool v) { m_sortAlpha = v; refreshDisplay(); }

protected:
    SaveData* m_data = nullptr;
    bool m_showIDs = false;
    bool m_sortAlpha = false;
};

class WeaponTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit WeaponTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
    void refreshDisplay() override;
private slots:
    void onListSelectionChanged();
    void onApply();
    void onAdd();
    void onDelete();
private:
    void refreshOwnedList();
    void refreshAddPicker();
    void refreshEquippedPicker();
    QComboBox* m_equipped;
    QListWidget* m_list;
    QLineEdit* m_turf;
    QCheckBox* m_newFlag;
    QComboBox* m_addPicker;
    QPushButton* m_apply;
    QPushButton* m_add;
    QPushButton* m_del;
};

class GearSubTab : public QWidget {
    Q_OBJECT
public:
    explicit GearSubTab(QWidget* parent = nullptr);
    void bind(SaveData* data, OwnedGearS1* array, const wchar_t** names, int count,
              be32_t* equippedField, bool showIDs, bool sortAlpha);
    void refreshDisplay(bool showIDs, bool sortAlpha);
    void saveEquipped();
private slots:
    void onListSelectionChanged();
    void onApply();
    void onAdd();
    void onDelete();
private:
    void refreshList();
    void refreshAddPicker();
    void refreshEquippedPicker();
    SaveData* m_data = nullptr;
    OwnedGearS1* m_array = nullptr;
    const wchar_t** m_names = nullptr;
    int m_count = 0;
    be32_t* m_equippedField = nullptr;
    bool m_showIDs = false;
    bool m_sortAlpha = false;
    QComboBox* m_equipped;
    QListWidget* m_list;
    QComboBox* m_sub1;
    QComboBox* m_sub2;
    QComboBox* m_sub3;
    QLineEdit* m_level;
    QLineEdit* m_slots;
    QLineEdit* m_exp;
    QCheckBox* m_newFlag;
    QComboBox* m_addPicker;
    QPushButton* m_apply;
    QPushButton* m_add;
    QPushButton* m_del;
};

class GearTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit GearTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
    void refreshDisplay() override;
private:
    QTabWidget* m_subTabs;
    GearSubTab* m_head;
    GearSubTab* m_clothes;
    GearSubTab* m_shoes;
};

class PlayerTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit PlayerTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
    void refreshDisplay() override;
private:
    QLineEdit* m_level;
    QLineEdit* m_levelPts;
    QComboBox* m_rank;
    QLineEdit* m_rankPts;
    QLineEdit* m_money;
    QLineEdit* m_snails;
    QLineEdit* m_newsVersion;

    QComboBox* m_gender;
    QComboBox* m_skin;
    QComboBox* m_eye;

    QLineEdit* m_sfId;
    QComboBox* m_sfTeam;
    QLineEdit* m_sfPower;
    QLineEdit* m_sfXp;
    QComboBox* m_sfRank;

    QCheckBox* m_colorLock;
    QCheckBox* m_revY;
    QCheckBox* m_revX;
    QCheckBox* m_motion;
    QComboBox* m_miiverse;
    QLineEdit* m_camSens;
};

class HeroTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit HeroTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
private slots:
    void onStageChanged(int row);
    void onUnlockWorld();
    void onCompleteAll();
private:
    void loadStage(int index);
    void saveStage(int index);
    QLineEdit* m_powerEggs;
    QComboBox* m_shotLvl;
    QComboBox* m_tankLvl;
    QComboBox* m_bombSelect;
    QComboBox* m_splatLvl;
    QComboBox* m_burstLvl;
    QComboBox* m_seekerLvl;
    QPushButton* m_unlockWorld;
    QListWidget* m_stageList;
    QCheckBox* m_zapfish;
    QCheckBox* m_scroll;
    QPushButton* m_completeAll;
    QCheckBox* m_flagZapfish;
    QCheckBox* m_flagHerosuit;
    QCheckBox* m_flagCuttlefish;
    QLineEdit* m_inkrails;
    QLineEdit* m_events;
    int m_currentStage = 0;
};

class PlazaTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit PlazaTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
    void refreshDisplay() override;
private slots:
    void onPlazaSelectionChanged(int row);
    void onFillFake();
private:
    void loadInkling(int index);
    void saveInkling(int index);
    QListWidget* m_list;
    QLineEdit* m_name;
    QLineEdit* m_level;
    QComboBox* m_rank;
    QLineEdit* m_turf;
    QComboBox* m_gender;
    QComboBox* m_skin;
    QComboBox* m_eye;
    QLineEdit* m_pid;
    QComboBox* m_head;
    QComboBox* m_cloth;
    QComboBox* m_shoe;
    QComboBox* m_weapon;
    QPushButton* m_fillFake;
    int m_current = -1;
};

class AmiiboTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit AmiiboTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
    void refreshDisplay() override;
private slots:
    void onUnlockAll();
private:
    QTabWidget* m_subTabs;
    QVector<QComboBox*> m_girl;
    QVector<QComboBox*> m_boy;
    QVector<QComboBox*> m_squid;
    QPushButton* m_unlockAll;
};

class MinigameTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit MinigameTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
private:
    QCheckBox* m_sjLock;
    QCheckBox* m_sbLock;
    QCheckBox* m_srLock;
    QCheckBox* m_sbzLock;
    QLineEdit* m_sjScore;
    QLineEdit* m_sbScore;
    QLineEdit* m_srScore;
};

class SpykeTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit SpykeTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
private slots:
    void onCategoryChanged(int idx);
    void onSetNow(int idx);
private:
    struct OrderUI {
        QComboBox* category;
        QComboBox* gear;
        QComboBox* state;
        QLineEdit* ordered;
        QLineEdit* arrival;
        QPushButton* now;
    };
    void syncOrder(int idx);
    void saveOrder(int idx);
    void fillGearForCategory(QComboBox* gear, int catSel, uint32_t activeID);
    void buildOrderBox(int idx, const QString& title);
    OrderUI m_orders[3];
};

class BitFlagTab : public EditorTabBase {
    Q_OBJECT
public:
    explicit BitFlagTab(QWidget* parent = nullptr);
    void loadData(SaveData* data) override;
    void saveData(SaveData* data) override;
private:
    QCheckBox* m_flags[14];
};
