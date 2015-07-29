#include "XBeeConnectDialog.h"
#include "ui_XBeeConnectDialog.h"

XBeeConnectDialog::XBeeConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::XBeeConnectDialog)
{
    // Setup UI
    ui->setupUi(this);
    // Create validator for baud rate
    intValidator = new QIntValidator(0, 4000000, this);
    // The string will not be inserted into the combobox
    ui->baudRateBox->setInsertPolicy(QComboBox::NoInsert);
    // Connect signal/slots
    connect(ui->detectPushButton, SIGNAL(released()), this, SLOT(fillPortsInfo()));
    connect(ui->serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
    connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(checkCustomBaudRatePolicy(int)));
    connect(ui->resetToDefaultsPushButton, SIGNAL(released()), this, SLOT(setUIDefaults()));
    // Initialize UI
    fillPortsParameters();
    fillPortsInfo();
    QStringList threadPriorities;
    threadPriorities<<"Idle"<<"Lowest"<<"Low"<<"Normal"<<"High"<<"Highest"<<"Time Critical"<<"Inherit";
    ui->threadPriorityBox->addItems(threadPriorities);
    // Set to defaults
    setUIDefaults();
    // Store settings from UI
    updateSettings();
}

XBeeConnectDialog::~XBeeConnectDialog()
{
    delete ui;
}

XBeeConnectDialog::Settings XBeeConnectDialog::settings() const
{
    return currentSettings;
}

void XBeeConnectDialog::setUIDefaults()
{
    // Baud rate
    if(DEFAULT_SERIAL_BAUDRATE == QSerialPort::Baud9600)
        ui->baudRateBox->setCurrentIndex(0);
    else if(DEFAULT_SERIAL_BAUDRATE == QSerialPort::Baud19200)
        ui->baudRateBox->setCurrentIndex(1);
    else if(DEFAULT_SERIAL_BAUDRATE == QSerialPort::Baud38400)
        ui->baudRateBox->setCurrentIndex(2);
    else if(DEFAULT_SERIAL_BAUDRATE == QSerialPort::Baud115200)
        ui->baudRateBox->setCurrentIndex(3);
    // Data bits
    if(DEFAULT_SERIAL_DATABITS == QSerialPort::Data5)
        ui->dataBitsBox->setCurrentIndex(0);
    else if(DEFAULT_SERIAL_DATABITS == QSerialPort::Data6)
        ui->dataBitsBox->setCurrentIndex(1);
    else if(DEFAULT_SERIAL_DATABITS == QSerialPort::Data7)
        ui->dataBitsBox->setCurrentIndex(2);
    else if(DEFAULT_SERIAL_DATABITS == QSerialPort::Data8)
        ui->dataBitsBox->setCurrentIndex(3);
    // Parity
    if(DEFAULT_SERIAL_PARITY == QSerialPort::NoParity)
        ui->parityBox->setCurrentIndex(0);
    else if(DEFAULT_SERIAL_PARITY == QSerialPort::EvenParity)
        ui->parityBox->setCurrentIndex(1);
    else if(DEFAULT_SERIAL_PARITY == QSerialPort::OddParity)
        ui->parityBox->setCurrentIndex(2);
    else if(DEFAULT_SERIAL_PARITY == QSerialPort::MarkParity)
        ui->parityBox->setCurrentIndex(3);
    else if(DEFAULT_SERIAL_PARITY == QSerialPort::SpaceParity)
        ui->parityBox->setCurrentIndex(4);
    // Stop bits
    if(DEFAULT_SERIAL_STOPBITS == QSerialPort::OneStop)
        ui->stopBitsBox->setCurrentIndex(0);
    else if(DEFAULT_SERIAL_STOPBITS == QSerialPort::OneAndHalfStop)
        ui->stopBitsBox->setCurrentIndex(1);
    else if(DEFAULT_SERIAL_STOPBITS == QSerialPort::TwoStop)
    {
        #ifdef Q_OS_WIN
            ui->stopBitsBox->setCurrentIndex(2);
        #else
            ui->stopBitsBox->setCurrentIndex(1);
        #endif
    }
    // Flow control
    if(DEFAULT_SERIAL_FLOWCONTROL == QSerialPort::NoFlowControl)
        ui->flowControlBox->setCurrentIndex(0);
    else if(DEFAULT_SERIAL_FLOWCONTROL == QSerialPort::HardwareControl)
        ui->flowControlBox->setCurrentIndex(1);
    else if(DEFAULT_SERIAL_FLOWCONTROL == QSerialPort::SoftwareControl)
        ui->flowControlBox->setCurrentIndex(2);
    // Thread sleep
    ui->threadSleepEdit->setText(QString::number(DEFAULT_XBEE_THREAD_SLEEP_MS));
    // Thread priority
    ui->threadPriorityBox->setCurrentIndex(DEFAULT_XBEE_THREAD_PRIO);
    // Packet buffer size
    ui->txBufferSizeEdit->setText(QString::number(DEFAULT_XBEE_PACKET_BUFFER_SIZE));
    // Drop packet
    ui->dropPacketCheckBox->setChecked(DEFAULT_DROP_PACKETS);
}

void XBeeConnectDialog::showPortInfo(int idx)
{
    if (idx != -1)
    {
        QStringList list = ui->serialPortInfoListBox->itemData(idx).toStringList();
        ui->descriptionLabel->setText(list.at(1));
        ui->manufacturerLabel->setText(list.at(2));
        ui->locationLabel->setText(list.at(3));
        ui->vidLabel->setText(list.at(4));
        ui->pidLabel->setText(list.at(5));
    }
}

void XBeeConnectDialog::checkCustomBaudRatePolicy(int idx)
{
    bool isCustomBaudRate = !ui->baudRateBox->itemData(idx).isValid();
    ui->baudRateBox->setEditable(isCustomBaudRate);
    if (isCustomBaudRate)
    {
        ui->baudRateBox->clearEditText();
        QLineEdit *edit = ui->baudRateBox->lineEdit();
        edit->setValidator(intValidator);
    }
}

void XBeeConnectDialog::fillPortsParameters()
{
    // Fill baud rate
    ui->baudRateBox->addItem(QLatin1String("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QLatin1String("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QLatin1String("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QLatin1String("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(QLatin1String("Custom"));

    // Fill data bits
    ui->dataBitsBox->addItem(QLatin1String("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QLatin1String("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QLatin1String("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QLatin1String("8"), QSerialPort::Data8);

    // Fill parity
    ui->parityBox->addItem(QLatin1String("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(QLatin1String("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(QLatin1String("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(QLatin1String("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(QLatin1String("Space"), QSerialPort::SpaceParity);

    // Fill stop bits
    ui->stopBitsBox->addItem(QLatin1String("1"), QSerialPort::OneStop);
    #ifdef Q_OS_WIN
        ui->stopBitsBox->addItem(QLatin1String("1.5"), QSerialPort::OneAndHalfStop);
    #endif
    ui->stopBitsBox->addItem(QLatin1String("2"), QSerialPort::TwoStop);

    // Fill flow control
    ui->flowControlBox->addItem(QLatin1String("None"), QSerialPort::NoFlowControl);
    ui->flowControlBox->addItem(QLatin1String("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowControlBox->addItem(QLatin1String("XON/XOFF"), QSerialPort::SoftwareControl);
}

/**
 * @brief XBeeConnectDialog::fillPortsInfo
 *
 */
void XBeeConnectDialog::fillPortsInfo()
{
    ui->serialPortInfoListBox->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QStringList list;
        list << info.portName()
             << info.description()
             << info.manufacturer()
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : QString())
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : QString());

        ui->serialPortInfoListBox->addItem(list.first(), list);
    }
}

void XBeeConnectDialog::updateSettings()
{
    // Port name
    currentSettings.name = ui->serialPortInfoListBox->currentText();

    // Baud rate
    if(ui->baudRateBox->currentIndex()==4)
    {
        // Custom baud rate
        currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
    }
    else
    {
        // Standard baud rate
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    // Data bits
    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsBox->currentText();

    // Parity
    currentSettings.parity = static_cast<QSerialPort::Parity>(ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityBox->currentText();

    // Stop bits
    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsBox->currentText();

    // Flow control
    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(ui->flowControlBox->itemData(ui->flowControlBox->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowControlBox->currentText();

    // Thread sleep
    currentSettings.threadSleep_ms = ui->threadSleepEdit->text().toInt();

    // Thread priority
    currentSettings.threadPriority = ui->threadPriorityBox->currentIndex();

    // Packet buffer size
    currentSettings.txPacketBufferSize = ui->txBufferSizeEdit->text().toInt();

    // Drop packet
    currentSettings.dropPacket = ui->dropPacketCheckBox->isChecked();
}

