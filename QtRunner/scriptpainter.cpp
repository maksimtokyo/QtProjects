#include "scriptpainter.h"
#include <QFile>
#include <QPen>
#include <QBrush>

ScriptPainter::ScriptPainter(QWidget *parent)
    : QDialog(parent)
{
    resize(800, 600);
    setWindowTitle("Script Painter Dialog");

    // Регистрируем объект в скрипте
    QScriptValue obj = engine.newQObject(this);

    engine.globalObject().setProperty("drawFilledCircle",
        obj.property("drawFilledCircle"));
    engine.globalObject().setProperty("drawCircle",
        obj.property("drawCircle"));
    engine.globalObject().setProperty("drawTriangleShape",
        obj.property("drawTriangleShape"));
    engine.globalObject().setProperty("drawRectangle",
        obj.property("drawRectangle"));
    engine.globalObject().setProperty("drawLine",
        obj.property("drawLine"));

}

void ScriptPainter::paintEvent(QPaintEvent *event)
{

    // в первый раз ничего не отрисуется скорее всего, но это не проблема, хотя небольшой костыль получается
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    painter = &p;

    QScriptValue func = engine.globalObject().property("drawAllShapes");
    if (func.isFunction())
        func.call();

    painter = nullptr;
}

void ScriptPainter::drawFilledCircle(int x, int y, int r, const QString &color)
{
    if (!painter) return;
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(color));
    painter->drawEllipse(QPoint(x, y), r, r);
}

void ScriptPainter::drawCircle(int x, int y, int r,
                               const QString &color, int lineWidth)
{
    if (!painter) return;
    QPen pen{QColor(color)};
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);
    painter->drawEllipse(QPoint(x, y), r, r);
}

void ScriptPainter::drawTriangleShape(int x1, int y1,
                                      int x2, int y2,
                                      int x3, int y3,
                                      const QString &fillColor,
                                      const QString &borderColor,
                                      int lineWidth)
{
    if (!painter) return;

    QPolygon poly;
    poly << QPoint(x1, y1)
         << QPoint(x2, y2)
         << QPoint(x3, y3);

    painter->setBrush(QColor(fillColor));
    QPen pen{QColor(borderColor)};
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->drawPolygon(poly);
}

void ScriptPainter::drawRectangle(int x, int y, int w, int h,
                                  const QString &fillColor,
                                  const QString &borderColor,
                                  int lineWidth)
{
    if (!painter) return;

    painter->setBrush(QColor(fillColor));
    QPen pen{QColor(borderColor)};
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->drawRect(x, y, w, h);
}

void ScriptPainter::drawLine(int x1, int y1, int x2, int y2,
                             const QString &color, int lineWidth)
{
    if (!painter) return;

    QPen pen{QColor(color)};
    pen.setWidth(lineWidth);
    painter->setPen(pen);
    painter->drawLine(x1, y1, x2, y2);
}

void ScriptPainter::runscript(QString script){
    engine.evaluate(script);
    update();
}
