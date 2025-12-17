#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_scriptengine(new QScriptEngine(this)),
      m_udpsocket(new QUdpSocket(this)),
      m_startbutton(new QPushButton(tr("Точка доступа"), this)),
      m_textBrowser(new QTextBrowser(this)),
      m_scriptpainter(new ScriptPainter(this)),
      m_startscript(new QPushButton(this))

{

    connect(m_udpsocket, &QUdpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_startbutton, &QPushButton::clicked, this, &MainWindow::onStartListening);
    connect(m_startscript, &QPushButton::clicked, this, &MainWindow::onStartScriptClicked);


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Your IP: " + getLocalIp(), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(titleLabel);

    QHBoxLayout *portLayout = new QHBoxLayout();
    portLayout->addWidget(new QLabel("Port:", this));
    QSpinBox *portSpinBox = new QSpinBox(this);
    portSpinBox->setRange(1024, 65535);
    portSpinBox->setValue(m_port);
    portLayout->addWidget(portSpinBox);
    layout->addLayout(portLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_startbutton);
    buttonLayout->addWidget(m_startscript);

    layout->addLayout(buttonLayout);


    layout->addWidget(m_textBrowser);
    resize(600, 500);
}

QString MainWindow::getLocalIp(){
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for (const QHostAddress &address : list) {
            if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                !address.isLoopback())
                return address.toString();
        }
        return "127.0.0.1"; // fallback
}

MainWindow::~MainWindow() {}


bool MainWindow::loadscript(const QString &path){

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, tr("Не получается открыть файл"), tr("asdasd"));
        return false;
    }


    QTextStream in(&file);
    QString script = in.readAll();
    file.close();

    QScriptValue scriptvalue = m_scriptengine->evaluate(script);

    if (m_scriptengine->hasUncaughtException()){
        QMessageBox::critical(this, tr("Ошибке в скрипте"), tr("Это конец"));
        return false;
    }


    return true;
}


void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
}

void MainWindow::proccesonDatagram(QNetworkDatagram& datagram){
    QString data =  QString::fromUtf8(datagram.data());
    m_textBrowser->append(data);
}

void MainWindow::onReadyRead(){

    while(m_udpsocket->hasPendingDatagrams()){
        QNetworkDatagram datagram = m_udpsocket->receiveDatagram();
        proccesonDatagram(datagram);
    }
}

void MainWindow::onStartListening(){
    if (m_udpsocket->state() == QAbstractSocket::BoundState){
        m_udpsocket->close();
        qDebug() << "Socket Closed";
    }

    if (m_udpsocket->bind(QHostAddress::Any, m_port)){
    } else{

    }

    m_startbutton->setText("Точка доступа нажата");
}

void MainWindow::onStartScriptClicked(){
    m_scriptpainter->runscript(m_textBrowser->toPlainText());
    m_scriptpainter->show();
}
