#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_edittext(new QTextEdit(this)),
      m_loadbutton(new QPushButton(tr("Load file from file manager"), this)),
      m_savebutton(new QPushButton(tr("Save file"), this)),
      m_sendbutton(new QPushButton(tr("Send button"), this)),
      m_udpsocket(new QUdpSocket(this)),
      m_dialog(new QDialog(this)),
      m_ipEdit(new QLineEdit(this)),
      m_port(new QSpinBox(this)),
      m_sendfile(new QPushButton(tr("send file"), m_dialog)),
      m_curfilepath("")
{
    setupUI(); // тут у меня было время, так что весь create view я еще успел вынести в отдельную функцию

    m_udpsocket->bind(QHostAddress::Any, 0);


    // ну простые коннекты
    connect(m_loadbutton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(m_savebutton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(m_sendbutton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    connect(m_sendfile, &QPushButton::clicked, this, &MainWindow::onSendFileButtonClicked);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI(){
    QWidget *mainwidget = new QWidget(this);
    setCentralWidget(mainwidget);

    QVBoxLayout *mainlayout = new QVBoxLayout(mainwidget);

    QHBoxLayout *buttonslayout = new QHBoxLayout(); // ну тут я добавляю кнопки для загрузки файла/сохранения/и хз

    buttonslayout->addWidget(m_loadbutton);
    buttonslayout->addWidget(m_savebutton);
    buttonslayout->addWidget(m_sendbutton);

    mainlayout->addLayout(buttonslayout);
    mainlayout->addWidget(m_edittext);

    QVBoxLayout *mainLayoutt = new QVBoxLayout(m_dialog);

    QGroupBox *addressGroup = new QGroupBox("Recipient", m_dialog);

    QGridLayout *addressLayout = new QGridLayout(addressGroup);

    addressLayout->addWidget(new QLabel("IP Address:", addressGroup), 0, 0);

    m_ipEdit->setText("127.0.0.1");
    m_ipEdit->setPlaceholderText("192.168.1.100");
    addressLayout->addWidget(m_ipEdit, 0, 1);

    addressLayout->addWidget(new QLabel("Port:", addressGroup), 1, 0);

    m_port->setRange(1024, 65535);
    m_port->setValue(12345);
    addressLayout->addWidget(m_port, 1, 1);

    mainLayoutt->addWidget(addressGroup);

    QTextBrowser *textBrowser = new QTextBrowser(m_dialog );
    textBrowser->setReadOnly(true);
    mainLayoutt->addWidget(textBrowser);


    // получаем возмоные айпишки  доступные

    QString ipList = "Available IP addresses:\n";
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
       if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
           ipList += "  " + address.toString() + "\n";
       }
    }
    textBrowser->append(ipList);
    mainLayoutt->addWidget(m_sendfile);

    resize(600, 400);

}


void MainWindow::onLoadButtonClicked(){ // настройка файл диалога
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setSidebarUrls({QUrl::fromLocalFile(QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first())});


    const QStringList filters({"Only .qs (*.qs)",
                               "Any files (*)"
                              });

    dialog.setNameFilters(filters);

    if (dialog.exec()){
        QStringList selectedfiles = dialog.selectedFiles();

        if (!selectedfiles.isEmpty()){
            QString path = selectedfiles.first();
            laodfile(path);
        }
    }
}

void MainWindow::onSendButtonClicked(){
    m_dialog->show();

}

void MainWindow::onSaveButtonClicked(){ // просто сохрон пути

    if (m_curfilepath == ""){
        return;
    }

    savefile(m_curfilepath);
}

void MainWindow::laodfile(const QString& path){
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::critical(this, tr("Не получается открыть файл"), tr("asdasd"));
        return;
    }

    QTextStream in(&file);

    QString text = in.readAll();

    m_edittext->setPlainText(text);
    file.close();
    m_curfilepath = path;
}

void MainWindow::savefile(const QString& path){
    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::critical(this, tr("Не получается открыть файл"), tr("asdasd"));
        return;
    }

    QTextStream out(&file);
    out << m_edittext->toPlainText();
    file.close();
}

void MainWindow::onSendFileButtonClicked(){ // отправка скрипта
    QString ip =  m_ipEdit->text().trimmed();
    quint16 port = static_cast<quint16>(m_port->value());

    QHostAddress address;

    if(!address.setAddress(ip)){
        qDebug() << "Не работает";
        m_dialog->close();
        return;
    }


    QByteArray message = m_edittext->toPlainText().toUtf8();



    const int chunkSize = 1024;  // делим файл на чанки, чтобы если что нормально закинуть, так как udp мало
    for (int i = 0; i < message.size(); i += chunkSize) {
        QByteArray chunk = message.mid(i, chunkSize);
        qint64 bytesSent = m_udpsocket->writeDatagram(chunk, address, port);
        if (bytesSent == -1){
            qDebug() << "Что-то не работает";

        } else{
            qDebug() << "Все рабоатет";
            m_dialog->close();
        }
    }

}
