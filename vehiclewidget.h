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

    void setTrafficSignals(bool verticalGreen);
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

    bool verticalGreen;

    void generateVehicle();
    void drawVehicle(QPainter &painter,
                     const MovingVehicle &vehicle);
};

#endif // VEHICLEWIDGET_H