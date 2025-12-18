#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardPaths>
#include <QStringList>
#include <QFile>
#include <QMessageBox>
#include <QUdpSocket>
#include <QHostAddress>
#include <QMenu>
#include <QNetworkDatagram>
#include <QDialog>
#include <QLineEdit>
#include <QGroupBox>
#include <QSpinBox>
#include <QLabel>
#include <QTextBrowser>
#include <QNetworkInterface>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void onLoadButtonClicked();
    void onSaveButtonClicked();
    void onSendButtonClicked();
    void onSendFileButtonClicked();

private:

    void setupUI();
    void laodfile(const QString& path);
    void savefile(const QString& path);

    QTextEdit *m_edittext;
    QDialog *m_dialog;
    QPushButton *m_loadbutton;
    QPushButton *m_savebutton;
    QPushButton *m_sendbutton;
    QPushButton *m_sendfile;
    QUdpSocket *m_udpsocket;
    QLineEdit *m_ipEdit;
    QSpinBox  *m_port;

    QString m_curfilepath;
};
#endif // MAINWINDOW_H
