#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Init the custom items
    this->init_window_items();

    //Init Platform Tester
    platfromTesterHandle =
                std::make_unique<ptester>(ui->textLogs);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_window_items(){
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    //this->setFixedSize(this->width(), this->height());

    //TODO: Try QT Validator
    this->setTextValidators();
    //this->ui->textIpAddr->setInputMask( "000.000.000.000" );

    this->ui->comboSubfunction->addItem("(0x1) StartRoutine");
    this->ui->comboSubfunction->addItem("(0x2) StopRoutine");
    this->ui->comboSubfunction->addItem("(0x1) RoutineResquestResults");

    QString itemString = "";
    for (int i = 0; i < 4; ++i) {
        itemString = "";
        this->ui->comboOptionBytes->addItem(QString("Start Camera# %1").arg(i));
    }

    //Disable the send button and enable after connect
    this->ui->btnSend->setEnabled(false);
    this->ui->btnSend->setStatusTip("You can't send without a connection");

    //Connect the signals with slots
    connect( this->ui->btnConnect, SIGNAL(released()), this, SLOT(handleConnectButton()) );
    connect( this->ui->btnSend, SIGNAL(released()), this, SLOT(handleSendButton()) );
    connect( this->ui->btnDisconnect, SIGNAL(released()), this, SLOT(handleDisconnectButton()) );
}

//TODO: Multiple access to textLogs (see if protection is needed)
void MainWindow::appendLog(geeds::gdpd_gp::LogType _logType, const QString& _msg){
    switch (_logType) {
    case geeds::gdpd_gp::LogType::Error:
        this->ui->textLogs->setTextColor(Qt::red);
        this->ui->textLogs->insertPlainText("[ERROR]\t");
        this->ui->textLogs->insertPlainText(_msg + "\n");
        break;
    case geeds::gdpd_gp::LogType::Warning:
        this->ui->textLogs->setTextColor(Qt::green);
        this->ui->textLogs->insertPlainText("[WARNING]\t");
        this->ui->textLogs->insertPlainText(_msg + "\n");
        break;
    case geeds::gdpd_gp::LogType::Info:
        this->ui->textLogs->setTextColor(Qt::blue);
        this->ui->textLogs->insertPlainText("[INFO]\t");
        this->ui->textLogs->insertPlainText(_msg + "\n");
        break;
    default:
        break;
    }
}

void MainWindow::setTextValidators(){
    //The ip Addres range
    QString IpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegularExpression IpRegex ("^" + IpRange
    + "(\\." + IpRange + ")?this->ui->btnSend->setEnabled(true);"
    + "(\\." + IpRange + ")?"
    + "(\\." + IpRange + ")?$");

    /* Not working with smart pointers
    //auto ipValidator = std::make_shared<QRegularExpressionValidator>(IpRegex, this);
    QSharedPointer<QRegularExpressionValidator> ipValidator =
            QSharedPointer<QRegularExpressionValidator>(new QRegularExpressionValidator(IpRegex, this));
    this->ui->textIpAddr->setValidator(ipValidator.get());
    */

    QRegularExpressionValidator* ipValidator =
                        new QRegularExpressionValidator(IpRegex, this);
    this->ui->textIpAddr->setValidator(ipValidator);

    //Port Ranges
    this->ui->textPort->setValidator( new QIntValidator(0, 65535, this) );

}

using tester_exception = geeds::gdpd_gp::TesterException;
void MainWindow::handleConnectButton(){
    QString str = ui->textIpAddr->text();
    //if( (false == ui->textIpAddr->hasAcceptableInput()) || (false == ui->textPort->hasAcceptableInput()) ){
    if( (false == ui->textPort->hasAcceptableInput()) ){
        QMessageBox::warning(this,"Warning!", " Ip Address or port values has invalid ranges");
    }else{
        try {
            const char* ipaddress = this->ui->textIpAddr->text().toStdString().c_str();
            uint16_t port = (uint16_t) this->ui->textPort->text().toShort();
            platfromTesterHandle->ConnetToDoIPServer(ipaddress, port);
            this->ui->btnSend->setEnabled(true);
        } catch (const tester_exception& e) {
            //Error Propagartion should be here
        }
    }
}

void MainWindow::handleSendButton(){
    if(!this->platfromTesterHandle->isConnected()){
        QMessageBox::information(this,"No Connection", "No connection is established");
    }else{
        int subfuncionId = this->ui->comboSubfunction->currentIndex() + 1;
        int instanceId = this->ui->comboOptionBytes->currentIndex() + 1;

#if defined (SEND_RECEIVE_ANOTHER_THREAD)
        platfromTesterHandle->init_buffers_async(DOIP_DIAG_MSG, subfuncionId, CAMERA_START_STOP_ROUTINE_ID, instanceId);
#elif defined (CPP_STD_ASYNC)
    //Returns future, but we don't care about the output
    //TODO: generalize std::async to be inside PlatformTester class, re-design is needed
    auto output = std::async(std::launch::async,[this]() {
                int _subfuncionId = this->ui->comboSubfunction->currentIndex() + 1;
                int _instanceId = this->ui->comboOptionBytes->currentIndex() + 1;
                this->platfromTesterHandle->DoIPRoutineControl(_subfuncionId, CAMERA_START_STOP_ROUTINE_ID, _instanceId);
                                        });
#else
        platfromTesterHandle->DoIPRoutineControl(subfuncionId, CAMERA_START_STOP_ROUTINE_ID, instanceId);
#endif
    }
}

void MainWindow::handleDisconnectButton(){
    if(platfromTesterHandle->isConnected() == false){
        QMessageBox::information(this,"No Connection", "No connection is established");
    }else{
        platfromTesterHandle->DisconnectFromDoIPServer();
        this->ui->btnSend->setEnabled(false);

     }
}

