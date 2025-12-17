#ifndef SCRIPTPAINTER_H
#define SCRIPTPAINTER_H


#include <QDialog>
#include <QScriptEngine>
#include <QPainter>
#include <QPen>

class ScriptPainter : public QDialog
{
    Q_OBJECT
public:
    explicit ScriptPainter(QWidget *parent = nullptr);

    void runscript(QString script);

protected:
    void paintEvent(QPaintEvent *event) override;

public slots:

    // функции который сообственно вызываются в .qs
    void drawFilledCircle(int x, int y, int r, const QString &color);
    void drawCircle(int x, int y, int r, const QString &color, int lineWidth);
    void drawTriangleShape(int x1, int y1,
                           int x2, int y2,
                           int x3, int y3,
                           const QString &fillColor,
                           const QString &borderColor,
                           int lineWidth);
    void drawRectangle(int x, int y, int w, int h,
                       const QString &fillColor,
                       const QString &borderColor,
                       int lineWidth);
    void drawLine(int x1, int y1, int x2, int y2,
                  const QString &color, int lineWidth);


private:
    QScriptEngine engine;
    QPainter *painter = nullptr;
};


#endif // SCRIPTPAINTER_H
