#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include "modulewidget.h"

#include <QPushButton>

class DisplayModel;

namespace dcc {

class SettingsGroup;
class NextPageWidget;

namespace display {

class DisplayWidget : public ModuleWidget
{
    Q_OBJECT

public:
    explicit DisplayWidget();

    void setModel(DisplayModel *model);

signals:
    void showResolutionPage() const;
    void requestRotate() const;
    void requestCustom() const;

private slots:
    void onScreenListChanged() const;

private:
    DisplayModel *m_model;
    NextPageWidget *m_resolution;
    QPushButton *m_rotate;
    QPushButton *m_custom;
};

} // namespace display

} // namespace dcc

#endif // DISPLAYWIDGET_H