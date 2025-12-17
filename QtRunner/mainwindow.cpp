#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_scriptengine(new QScriptEngine(this)),
      m_udpsocket(new QUdpSocket(this)),
      m_startbutton(new QPushButton(tr("Точка доступа"), this)),
      m_textBrowser(new QTextBrowser(this)),
      m_scriptpainter(new ScriptPainter(this)),
      m_startscript(new QPushButton(tr("run Script"), this)),
      m_clearbutton(new QPushButton(tr("clear script"), this)),
      m_portSpinBox(new QSpinBox(this))

{

    connect(m_udpsocket, &QUdpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_startbutton, &QPushButton::clicked, this, &MainWindow::onStartListening);
    connect(m_startscript, &QPushButton::clicked, this, &MainWindow::onStartScriptClicked);
    connect(m_clearbutton, &QPushButton::clicked, this, &MainWindow::onClearButtonClicked);
    connect(m_portSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onValueeChanged);


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Your IP: " + getLocalIp(), this); // получаем нашш ip
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("Port:", this));
    // QSpinBox *portSpinBox = new QSpinBox(this);
    m_portSpinBox->setRange(1024, 65535);
    m_portSpinBox->setValue(m_port);
    portLayout->addWidget(m_portSpinBox);
    layout->addLayout(portLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_startbutton);
    buttonLayout->addWidget(m_startscript);
    buttonLayout->addWidget(m_clearbutton);

    layout->addLayout(buttonLayout);


    layout->addWidget(m_textBrowser);
    resize(600, 500);
}

QString MainWindow::getLocalIp(){
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : list) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&!address.isLoopback())
                return address.toString();
        }
        return "127.0.0.1"; // fallback
}

MainWindow::~MainWindow() {}

void MainWindow::proccesonDatagram(QNetworkDatagram& datagram){ // ну из-за того что udp отправляется чанками, есть шанс, что файл будет слишком большой и придется несколько раз обрабатывать (очевидно что если с разных ip отправить, то текст уже просто напросто умрет)
    QString data =  QString::fromUtf8(datagram.data());
    m_textBrowser->append(data);
}

void MainWindow::onReadyRead(){

    while(m_udpsocket->hasPendingDatagrams()){ // ну просто обработка
        QNetworkDatagram datagram = m_udpsocket->receiveDatagram();


        QString senderIP = datagram.senderAddress().toString();
        int senderPort = datagram.senderPort();

        if (senderIP != m_senderIP){
            m_textBrowser->clear();
            m_senderIP = senderIP;
        }

        if (senderPort != m_senderPort){
            m_textBrowser->clear();
            m_senderPort = senderPort;
        }
        proccesonDatagram(datagram);
    }
}

void MainWindow::onStartListening() {
    if (m_udpsocket->state() == QAbstractSocket::BoundState) {
        m_udpsocket->close();
        qDebug() << "Socket Closed";
    }

    m_port = m_portSpinBox->value();

    if (m_udpsocket->bind(QHostAddress::Any, m_port)) {
        qDebug() << "Listening on port" << m_port;
        m_startbutton->setText("Точка доступа");
        m_startbutton->setEnabled(false);
    } else {

        QMessageBox::critical(this, tr("Ошибка привязки порта"),
                              tr("Не удалось привязать сокет к порту %1.\nПричина: %2")
                              .arg(m_port)
                              .arg(m_udpsocket->errorString()));
        qDebug() << "Bind failed:" << m_udpsocket->errorString();
    }
}

void MainWindow::onStartScriptClicked(){
    m_scriptpainter->runscript(m_textBrowser->toPlainText());
    m_scriptpainter->show();
    m_startbutton->setEnabled(true);
}


void MainWindow::onClearButtonClicked(){
    m_textBrowser->clear();
}

void MainWindow::onValueeChanged(int value){
    m_startbutton->setEnabled(true);
}
