#ifndef CHOSEDIALOG_H
#define CHOSEDIALOG_H

#include "datetimeutil.h"
#include "searchinput.h"
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

using namespace dcc;

namespace dcc {
namespace datetime {

class MapWidget;
class ChoseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoseDialog(QWidget* parent = 0);
    void setFlag(bool flag);

signals:
    void addTimezone(const Timezone& tz);
    void timezoneChanged(const Timezone& tz);
    void curTimezone(const Timezone& tz);

public slots:
    void onReturn();
    void onAdd();

private:
    bool m_flag;
    MapWidget* m_widget;
    dcc::widgets::SearchInput* m_search;
    QPushButton* m_cancel;
    QPushButton* m_add;
};


class MapWidget : public QFrame
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget* parent = 0);
    void setTimezones(QList<Timezone*> timezones);
    QStringList citys() const;
    Timezone timezone() const;

signals:
    void cityChanged(const QString& city);

public slots:
    void onLocateCity(const QString& city);
    void onChoseCity(QAction* act);

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent *e);

private:
    void initData();

private:
    QMenu *m_menu;
    QPixmap m_map;
    Timezone m_curTimezone;
    QList<Timezone*> m_timezones;
    QList<QString> m_timezoneLists;
    QString m_city;
};

}
}
#endif // CHOSEDIALOG_H
