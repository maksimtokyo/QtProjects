#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "scriptpainter.h"

#include <QMainWindow>
#include <QScriptEngine>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QString>
#include <QPainter>
#include <QUdpSocket>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QNetworkInterface>
#include <QNetworkDatagram>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void paintEvent(QPaintEvent *event) override;
    void proccesonDatagram(QNetworkDatagram& datagram);

private slots:

    void onReadyRead();
    void onStartListening();
    void onStartScriptClicked();




private:

    bool loadscript(const QString& path);
    QString getLocalIp();


    QTextBrowser *m_textBrowser;
    QScriptEngine *m_scriptengine;
    QPushButton *m_startbutton;
    QUdpSocket *m_udpsocket;
    quint16 m_port = 12345;
    QHostAddress m_lastSenderAddress;
    quint16 m_lastSenderPort;
    ScriptPainter *m_scriptpainter;
    QPushButton *m_startscript;


};
#endif // MAINWINDOW_H
