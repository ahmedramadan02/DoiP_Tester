#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegExpValidator>
#include <QMessageBox>

#include <memory>

#include <future>
#include "PlatformTester.h"
#include "TesterException.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleConnectButton();
    void handleSendButton();
    void handleDisconnectButton();

private:
    Ui::MainWindow *ui;

    //Connection Handle
    using ptester =  geeds::gdpd_gp::PlatformTester;
    std::unique_ptr<ptester> platfromTesterHandle;

    //Other funnctions
    void init_window_items();
    void appendLog(geeds::gdpd_gp::LogType _logType, const QString& _msg);
    void setTextValidators();

    //Custom private members
};
#endif // MAINWINDOW_H
