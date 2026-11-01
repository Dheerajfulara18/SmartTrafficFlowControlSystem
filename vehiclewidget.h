#ifndef VEHICLEWIDGET_H
#define VEHICLEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPainter>

struct MovingVehicle
{
    int id;
    QString type;
    int lane;
    qreal x;
    qreal y;
    qreal speed;
};

class VehicleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VehicleWidget(QWidget *parent = nullptr);

    void setSignalPhase(int phase);
    void resetVehicles();
    void startAnimation();
    void pauseAnimation();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void updateVehicles();

private:
    QTimer *animationTimer;
    QVector<MovingVehicle> vehicles;

    int signalPhase = 0;
    bool yellowVisible = true;
    int blinkCounter = 0;

    void generateVehicle();
    void drawVehicle(QPainter &painter,
                     const MovingVehicle &vehicle);
    // 2.5D Environment Drawing Functions
    void drawBackground(QPainter &painter);
    void drawRoads(QPainter &painter);
    void drawSidewalks(QPainter &painter);
    void drawRoadMarkings(QPainter &painter);
    void drawStreetElements(QPainter &painter);
    void drawVehicles(QPainter &painter);
};

#endif // VEHICLEWIDGET_H