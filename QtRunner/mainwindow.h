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
    void proccesonDatagram(QNetworkDatagram& datagram);

private slots:

    void onReadyRead();
    void onStartListening();
    void onStartScriptClicked();
    void onClearButtonClicked();
    void onValueeChanged(int value);




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
    ScriptPainter *m_scriptpainter; // вынес в кастом класс, так как не хотелось наводить шума в основном классе
    QPushButton *m_startscript;
    QString m_senderIP{""};
    QPushButton *m_clearbutton;
    int m_senderPort{-1};

    QSpinBox *m_portSpinBox;


};
#endif // MAINWINDOW_H
