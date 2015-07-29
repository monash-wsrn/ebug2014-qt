#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainWindow::fill_lstbxPortsAvailable();
    qDebug("Window is set up.");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newMsg(QByteArray bytesMsg){
    QString *strMsg = new QString();
    strMsg->append(QString::number(bytesMsg.size()));
    strMsg->append("|");
    int value=0;
    for(int i=0; i<bytesMsg.size(); i++)
    {
        value += bytesMsg.at(i);
        QByteArray *charAtI = new QByteArray();
        charAtI->append(bytesMsg.at(i));
        strMsg->append(charAtI->toHex());
        strMsg->append(" ");
    }
    //strMsg->append(bytesMsg.toHex());
    strMsg->append("|");
    strMsg->append(QString::number(value));
    if(value!=0)
        ui->textRaw->append(*strMsg);

}

void MainWindow::fill_lstbxPortsAvailable()
{
    ui->lstbxPortsAvailable->clear();
    foreach (const QSerialPortInfo &portInfo, QSerialPortInfo::availablePorts())
    {
        QStringList strlstPortInfo;
        strlstPortInfo << portInfo.portName()
             << portInfo.description()
             << portInfo.manufacturer()
             << portInfo.systemLocation()
             << (portInfo.vendorIdentifier() ? QString::number(portInfo.vendorIdentifier(), 16) : QString())
             << (portInfo.productIdentifier() ? QString::number(portInfo.productIdentifier(), 16) : QString());

        ui->lstbxPortsAvailable->addItem(strlstPortInfo.first(), strlstPortInfo);
    }

}

void MainWindow::on_lstbxPortsAvailable_currentIndexChanged(int index)
{
    fill_portInfoLabels(index);
}

void MainWindow::fill_portInfoLabels(int index){
    QStringList strlstIndexPortInfo = ui->lstbxPortsAvailable->itemData(index).toStringList();
    ui->labelDescription->setText(strlstIndexPortInfo.at(1));
    ui->labelManufacturer->setText(strlstIndexPortInfo.at(2));
    ui->labelLocation->setText(strlstIndexPortInfo.at(3));
    ui->labelVendorID->setText(strlstIndexPortInfo.at(4));
    ui->labelProductID->setText(strlstIndexPortInfo.at(5));
}

void MainWindow::on_btnDetect_clicked()
{
    qDebug("Starting CommsIn");
    comms = new CommsIn(ui->lstbxPortsAvailable->currentText());
    translate = new MessageTranslator();

    QObject::connect(comms, SIGNAL(newMsg(QByteArray)),SLOT(newMsg(QByteArray)));
    QObject::connect(comms, SIGNAL(newMsg(QByteArray)), translate, SLOT(newMsg(QByteArray)));

}
