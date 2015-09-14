#include <QLabel>
#include <QDebug>

#include "power.h"
#include "constants.h"

Power::Power()
    :QObject(),
      m_contentHeight(30),
      m_bgContentHeight(60),
      m_frame(new QFrame),
      m_powerManagementFrame(new PowerManagement)
{

    Q_INIT_RESOURCE(widgets_theme_dark);
    Q_INIT_RESOURCE(widgets_theme_light);

    m_powerInterfaceManagement = new PowerInterfaceManagement;

    initUI();
    initData();
    initConnection();
}

Power::~Power()
{
    qDebug() << "~Power()";
    m_frame->deleteLater();

}

void Power::updateBatteryUsedControlUI() {
    if (!m_batteryIsPresent) {
         m_batterySettingDHeaderLine->hide();
         m_batterySettingExpand->hide();
         m_batteryCustomExtendBoard->hide();
    } else {
        m_batterySettingDHeaderLine->show();
        m_batterySettingExpand->show();
        m_batteryCustomExtendBoard->show();
    }
}

void Power::initClockWhenActiveUI()
{
    m_secondHSeparator = new DSeparatorHorizontal;
    m_chooseNeedPasswdLine = new DHeaderLine;
    m_chooseNeedPasswdLine->setTitle(tr("Password required to wake up"));
    m_chooseNeedPasswdButton = new DSwitchButton;
    m_chooseNeedPasswdLine->setContent(m_chooseNeedPasswdButton);
    m_chooseNeedPasswdLine->setFixedHeight(m_contentHeight);
    m_thirdHSeparator = new DSeparatorHorizontal(m_frame);

    lockWhenActiveLayout = new QVBoxLayout;
    lockWhenActiveLayout->setMargin(0);
    lockWhenActiveLayout->setSpacing(0);
    lockWhenActiveLayout->addWidget(m_secondHSeparator);
    lockWhenActiveLayout->addWidget(m_chooseNeedPasswdLine);
    lockWhenActiveLayout->addWidget(m_thirdHSeparator);
}

void Power::initPowerConnectionPanelUI() {
    m_prePowerSettingHeaderLine = new DHeaderLine;
    m_powerSettingDHeaderLine = new DHeaderLine;
    m_powerSettingDHeaderLine->setTitle(tr("Plugged in"));

    m_powerSettingExpand = new DBaseExpand;
    m_powerSettingExpand->setFixedHeight(m_bgContentHeight);
    m_powerSettingExpand->setHeader(m_powerSettingDHeaderLine);
    m_powerSettingExpand->setHeaderHeight(30);

    m_powerDynamicLabel = new DynamicLabel(m_powerSettingDHeaderLine);
    m_powerDynamicLabel->setDuration(500);

    m_powerPerformanceButtonGroup = new DButtonGrid(2, 2);

    m_powerPerformaceString << tr("Balanced") << tr("Power saver") << tr("High performance") << tr("Custom");
    m_powerPerformanceButtonGroup->addButtons(m_powerPerformaceString);
    m_powerPerformanceButtonGroup->setItemSize(150, 30);
    m_powerPerformanceButtonGroup->checkButtonByIndex(2);
    m_powerSettingExpand->setContent(m_powerPerformanceButtonGroup);
    m_powerSettingExpand->setExpand(true);
/////////////////////////////////////////////////////-- power connection custom setting expand
    m_powerCustomExtendBoard = new DExtendBoard;
    m_powerCustomExtendBoard->setExpand(true);

    powerConnectLayout = new QVBoxLayout;
    powerConnectLayout->setMargin(0);
    powerConnectLayout->setSpacing(0);
    powerConnectLayout->addWidget(m_prePowerSettingHeaderLine);
    powerConnectLayout->addWidget(m_powerSettingDHeaderLine);
    powerConnectLayout->addWidget(m_powerSettingExpand);
    powerConnectLayout->addWidget(m_powerCustomExtendBoard);

}
void Power::initBatteryUsedUI() {
    /////////////////////////////////////////////////////////--use battery setting panel
    m_preBatterySettingHeaderLine = new DHeaderLine;
    m_batterySettingDHeaderLine = new DHeaderLine;
    m_batterySettingDHeaderLine->setTitle(tr("On battery"));

    m_batterySettingExpand = new DBaseExpand;

    m_batteryDynamicLabel = new DynamicLabel(m_batterySettingDHeaderLine);
    m_batteryDynamicLabel->setDuration(500);


    m_batterySettingExpand->setFixedHeight(m_bgContentHeight);
    m_batterySettingExpand->setHeader(m_batterySettingDHeaderLine);
    m_batterySettingExpand->setHeaderHeight(m_contentHeight);
    m_batteryButtonGrid = new DButtonGrid(2, 2);
    m_powerPerformaceString << tr("Balanced") << tr("Power saver") << tr("High performance") << tr("Custom");
    m_batteryButtonGrid->addButtons(m_powerPerformaceString);
    m_batteryButtonGrid->setItemSize(150, 30);
    m_batteryButtonGrid->checkButtonByIndex(3);

    m_batterySettingExpand->setContent(m_batteryButtonGrid);
    m_batterySettingExpand->setExpand(true);
    m_batteryCustomExtendBoard = new DExtendBoard;
    m_batteryCustomExtendBoard->setExpand(true);


    batteryUsedLayout = new QVBoxLayout;
    batteryUsedLayout->setMargin(0);
    batteryUsedLayout->setSpacing(0);
    batteryUsedLayout->addWidget(m_preBatterySettingHeaderLine);
    batteryUsedLayout->addWidget(m_batterySettingDHeaderLine);
    batteryUsedLayout->addWidget(m_batterySettingExpand);
    batteryUsedLayout->addWidget(m_batteryCustomExtendBoard);

}
void Power::initUI() {

    linePowerAction  << tr("ShutDown") << tr("Suspend") << tr("Ask");
    m_pressPowerButtonActionFrame = new PressPowerButtonAction(linePowerAction);
    m_pressPowerButtonActionFrame->setTitle(tr("When I press the power button"));
    m_pressPowerButtonActionFrame->setFixedHeight(m_bgContentHeight);

    closeLaptopAction << tr("ShutDown") << tr("Suspend") << tr("Nothing");
    m_closeLaptopActionFrame = new PressPowerButtonAction(closeLaptopAction);
    m_closeLaptopActionFrame->setTitle(tr("When I close the lid"));
    m_closeLaptopActionFrame->setFixedHeight(m_bgContentHeight);

    initClockWhenActiveUI();
    initPowerConnectionPanelUI();
    initBatteryUsedUI();

    QVBoxLayout * layout = new QVBoxLayout(m_frame);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(m_powerManagementFrame);
    layout->addWidget(m_pressPowerButtonActionFrame);
    layout->addWidget(m_closeLaptopActionFrame);
    layout->addLayout(lockWhenActiveLayout);
    layout->addLayout(powerConnectLayout);
    layout->addLayout(batteryUsedLayout);
    layout->addStretch();
    m_frame->setLayout(layout);
}
void Power::initData() {
    ////////////////////////////////////////////////////--get battery setting
    m_batteryIsPresent = m_powerInterfaceManagement->getBatteryIsPresent();
    m_onBattery = m_powerInterfaceManagement->getBatteryon();
    m_batteryPercentage = m_powerInterfaceManagement->getBatteryPresent();

    ////////////////////////////////////////////////////--getLinePowerPlan
    updateLinePowerPlanUI();
    /////////////////////////////////////////////////////-- getLinePowerIdleDelay&SuspendDelay

    updateLinePowerIdleDelayUI();
    updateLinePowerSuspendDelayUI();
    updateBatteryPlanUI();
    updateBatteryIdleDelayUI();
    updateBatterySuspendDelayUI();

    ////////////////////////////////////////////////////--setting the UI according to the data
//    m_batteryIsPresent = true; //for testing
//    m_batteryPercentage = 60;
    m_powerManagementFrame->setElectricQuantity(m_batteryPercentage);
    m_powerManagementFrame->batteryReservedControl(m_batteryIsPresent);

    ////////////////////////////////////////////////////--get press power button action panel

    qint32 buttonId = m_powerInterfaceManagement->getPowerButtonAction();
    m_pressPowerButtonActionFrame->setPowerButtonAction(buttonId);

    qint32 buttonIndex = m_powerInterfaceManagement->getLidCloseAction();
    m_closeLaptopActionFrame->setPowerButtonAction(buttonIndex);
    ////////////////////////////////////////////////////-- lock when Active
    m_chooseNeedPasswdButton->setChecked(m_powerInterfaceManagement->getLockWhenActive());

    if (m_batteryIsPresent) {
        m_closeLaptopActionFrame->show();
    } else {
        m_closeLaptopActionFrame->hide();
    }
    updateBatteryUsedControlUI();

}

void Power::set4ButtonGridChecked(int idIndex, DButtonGrid *buttonGroup) {
    switch(idIndex) {
        case 0: { buttonGroup->checkButtonByIndex(3);break;}
        case 1: { buttonGroup->checkButtonByIndex(1);break;}
        case 2: { buttonGroup->checkButtonByIndex(0);break;}
        default: { buttonGroup->checkButtonByIndex(2);}
    }
}
void Power::set7ButtonGridChecked(int idIndex, DButtonGrid* buttonGroup) {
    switch(idIndex) {
        case 1: { buttonGroup->checkButtonByIndex(0); break;}
        case 5: { buttonGroup->checkButtonByIndex(1); break;}
        case 10: { buttonGroup->checkButtonByIndex(2); break;}
        case 15: { buttonGroup->checkButtonByIndex(3); break;}
        case 30: { buttonGroup->checkButtonByIndex(4); break;}
        case 60: { buttonGroup->checkButtonByIndex(5); break;}
        default: { buttonGroup->checkButtonByIndex(6);}
    }
}
void Power::initConnection() {
    connect(m_powerInterfaceManagement, SIGNAL(BatteryPercentageChanged(double)),
            m_powerManagementFrame, SLOT(setElectricQuantity(double)));
    connect(m_powerInterfaceManagement, SIGNAL(BatteryPercentageChanged()),
            this, SLOT(initData()));
    connect(m_powerInterfaceManagement, SIGNAL(BatteryIsPresentChanged()),
            this, SLOT(initData()));

    connect(m_powerManagementFrame, SIGNAL(Reset()),
            m_powerInterfaceManagement, SLOT(Reset()));

    connect(m_powerInterfaceManagement, SIGNAL(LockWhenActiveChanged()),
            this, SLOT(initData()));
    connect(m_chooseNeedPasswdButton, SIGNAL(checkedChanged(bool)),
            m_powerInterfaceManagement,
            SLOT(setLockWhenActive(bool)));
    connect(m_powerInterfaceManagement, SIGNAL(PowerButtonActionChanged()),
             SLOT(initData()));
    connect(m_pressPowerButtonActionFrame, SIGNAL(powerButtonAction(QString)),
            m_powerInterfaceManagement, SLOT(setPowerButtonAction(QString)));
    connect(m_powerInterfaceManagement, SIGNAL(LidClosedActionChanged()),
              SLOT(initData()));
    connect(m_closeLaptopActionFrame, SIGNAL(powerButtonAction(QString)),
            m_powerInterfaceManagement, SLOT(setLidCloseAction(QString)));
    connect(m_powerInterfaceManagement, SIGNAL(LinePowerPlanChanged()),
            this, SLOT(updateLinePowerPlanUI()));

    connect(m_powerPerformanceButtonGroup, SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setLinePowerPlan(QString)));
    connect(m_powerPerformanceButtonGroup, SIGNAL(buttonChecked(QString)),
            SLOT(setConnectPowerExpand(QString)));

    connect(m_powerPerformanceButtonGroup, SIGNAL(buttonMouseEntered(QString)),
            this, SLOT(showPowerTooltip(QString)));
    connect(m_powerPerformanceButtonGroup, SIGNAL(buttonMouseLeaved(QString)),
            this, SLOT(hidePowerTooltip(QString)));

    connect(m_powerInterfaceManagement, SIGNAL(LinePowerIdleDelayChanged()),
            this, SLOT(updateLinePowerIdleDelayUI()));
    connect(m_powerCustomExtendBoard->m_poweroffButtonGrid,
            SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setLinePowerIdleDelay(QString)));

    connect(m_powerInterfaceManagement, SIGNAL(LinePowerSuspendDelayChanged()),
            this, SLOT(updateLinePowerSuspendDelayUI()));
    connect(m_powerCustomExtendBoard->m_suspendButtonGrid,
            SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setLinePowerSuspendDelay(QString)));

    connect(m_powerInterfaceManagement, SIGNAL(BatteryPlanChanged()),
            this, SLOT(initData()));
    connect(m_batteryButtonGrid, SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setBatteryPlan(QString)));

    connect(m_batteryButtonGrid, SIGNAL(buttonMouseEntered(QString)),
            this, SLOT(showBatteryTooltip(QString)));

    connect(m_batteryButtonGrid, SIGNAL(buttonMouseLeaved(QString)),
            this, SLOT(hideBatteryTooltip(QString)));

    connect(m_batteryButtonGrid, SIGNAL(buttonChecked(QString)),
            SLOT(setUseBatteryExpand(QString)));
    connect(m_powerInterfaceManagement, SIGNAL(BatteryIdleDelayChanged()),
            this, SLOT(initData()));
    connect(m_batteryCustomExtendBoard->m_poweroffButtonGrid, SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setBatteryIdleDelay(QString)));

    connect(m_powerInterfaceManagement, SIGNAL(BatterySuspendDelayChanged()),
            this, SLOT(initData()));
    connect(m_batteryCustomExtendBoard->m_suspendButtonGrid, SIGNAL(buttonChecked(QString)),
            m_powerInterfaceManagement, SLOT(setBatterySuspendDelay(QString)));
}

QFrame* Power::getContent()
{
    return m_frame;
}
