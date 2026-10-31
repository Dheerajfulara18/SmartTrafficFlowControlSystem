#include "vehiclewidget.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QPaintEvent>
#include <QtMath>

VehicleWidget::VehicleWidget(QWidget *parent)
    : QWidget(parent),
    animationTimer(new QTimer(this)),
    verticalGreen(true)
{
    setMinimumSize(600, 350);

    connect(animationTimer, &QTimer::timeout,
            this, &VehicleWidget::updateVehicles);
}

void VehicleWidget::setTrafficSignals(bool green)
{
    verticalGreen = green;
}

// NEW FUNCTION
void VehicleWidget::startAnimation()
{
    animationTimer->start(30);
}

// NEW FUNCTION
void VehicleWidget::pauseAnimation()
{
    animationTimer->stop();
}

void VehicleWidget::resetVehicles()
{
    vehicles.clear();
    update();
}

void VehicleWidget::generateVehicle()
{
    if (vehicles.size() >= 12)
        return;

    MovingVehicle vehicle;

    vehicle.id = vehicles.size() + 1;

    int lane = QRandomGenerator::global()->bounded(4);

    vehicle.lane = lane;

    int type = QRandomGenerator::global()->bounded(100);

    if (type < 5)
        vehicle.type = "AMBULANCE";
    else if (type < 20)
        vehicle.type = "BIKE";
    else
        vehicle.type = "CAR";

    vehicle.speed = 2.0;

    if (lane == 0) {
        vehicle.x = width() / 2.0 - 45;
        vehicle.y = -40;
    }
    else if (lane == 1) {
        vehicle.x = width() + 40;
        vehicle.y = height() / 2.0 + 45;
    }
    else if (lane == 2) {
        vehicle.x = width() / 2.0 + 45;
        vehicle.y = height() + 40;
    }
    else {
        vehicle.x = -40;
        vehicle.y = height() / 2.0 - 45;
    }

    vehicles.append(vehicle);
}

void VehicleWidget::updateVehicles()
{
    if (QRandomGenerator::global()->bounded(100) < 8)
        generateVehicle();

    for (int i = 0; i < vehicles.size(); ++i) {

        MovingVehicle &vehicle = vehicles[i];

        // CHANGED: Check signal according to vehicle lane
        bool green;

        if (vehicle.lane == 0 || vehicle.lane == 2)
            green = verticalGreen;
        else
            green = !verticalGreen;

        if (!green)
            continue;

        if (vehicle.lane == 0)
            vehicle.y += vehicle.speed;

        else if (vehicle.lane == 1)
            vehicle.x -= vehicle.speed;

        else if (vehicle.lane == 2)
            vehicle.y -= vehicle.speed;

        else
            vehicle.x += vehicle.speed;
    }

    for (int i = vehicles.size() - 1; i >= 0; --i) {

        const MovingVehicle &vehicle = vehicles[i];

        if (vehicle.x < -60 ||
            vehicle.x > width() + 60 ||
            vehicle.y < -60 ||
            vehicle.y > height() + 60) {

            vehicles.removeAt(i);
        }
    }

    update();
}

void VehicleWidget::drawVehicle(
    QPainter &painter,
    const MovingVehicle &vehicle)
{
    QRectF body(
        vehicle.x - 12,
        vehicle.y - 20,
        24,
        40
        );

    if (vehicle.type == "AMBULANCE")
        painter.setBrush(QColor("#EF4444"));

    else if (vehicle.type == "BIKE")
        painter.setBrush(QColor("#FBBF24"));

    else
        painter.setBrush(QColor("#38BDF8"));

    painter.setPen(Qt::NoPen);

    painter.drawRoundedRect(body, 5, 5);

    painter.setBrush(QColor("#E2E8F0"));

    painter.drawRoundedRect(
        vehicle.x - 8,
        vehicle.y - 12,
        16,
        10,
        3,
        3
        );
}

void VehicleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    painter.setBrush(QColor("#0B1220"));
    painter.drawRect(rect());

    painter.setPen(QPen(QColor("#64748B"), 2));

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.setBrush(QColor("#263449"));

    painter.drawRect(
        centerX - 55,
        0,
        110,
        height()
        );

    painter.drawRect(
        0,
        centerY - 55,
        width(),
        110
        );

    painter.setPen(QPen(Qt::white, 2, Qt::DashLine));

    painter.drawLine(
        centerX,
        0,
        centerX,
        centerY - 55
        );

    painter.drawLine(
        centerX,
        centerY + 55,
        centerX,
        height()
        );

    painter.drawLine(
        0,
        centerY,
        centerX - 55,
        centerY
        );

    painter.drawLine(
        centerX + 55,
        centerY,
        width(),
        centerY
        );

    for (const MovingVehicle &vehicle : vehicles)
        drawVehicle(painter, vehicle);
}