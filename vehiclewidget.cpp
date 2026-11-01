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
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    int w = width();
    int h = height();

    int centerX = w / 2;
    int centerY = h / 2;

    // =================================
    // 1. BACKGROUND - CITY GROUND
    // =================================

    painter.fillRect(rect(), QColor("#172B27"));

    // Ground texture
    painter.setPen(QPen(QColor("#203A32"), 1));

    for (int x = 0; x < w; x += 35) {
        for (int y = 0; y < h; y += 35) {
            painter.drawPoint(x, y);
        }
    }

    // =================================
    // 2. ROAD SHADOW
    // =================================

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 90));

    painter.drawRect(
        centerX - 87 + 5,
        5,
        174,
        h - 10
        );

    painter.drawRect(
        5,
        centerY - 87 + 5,
        w - 10,
        174
        );

    // =================================
    // 3. MAIN ASPHALT ROAD
    // =================================

    QLinearGradient roadGradient(
        0, 0, w, h
        );

    roadGradient.setColorAt(0, QColor("#343C49"));
    roadGradient.setColorAt(0.5, QColor("#252E3A"));
    roadGradient.setColorAt(1, QColor("#343C49"));

    painter.setBrush(roadGradient);
    painter.setPen(Qt::NoPen);

    // Vertical road
    painter.drawRect(
        centerX - 82,
        0,
        164,
        h
        );

    // Horizontal road
    painter.drawRect(
        0,
        centerY - 82,
        w,
        164
        );

    // =================================
    // 4. ROAD EDGES
    // =================================

    painter.setPen(QPen(QColor("#64748B"), 3));

    painter.drawLine(
        centerX - 82, 0,
        centerX - 82, h
        );

    painter.drawLine(
        centerX + 82, 0,
        centerX + 82, h
        );

    painter.drawLine(
        0, centerY - 82,
        w, centerY - 82
        );

    painter.drawLine(
        0, centerY + 82,
        w, centerY + 82
        );

    // =================================
    // 5. SIDEWALK / CURB DETAILS
    // =================================

    painter.setPen(QPen(QColor("#94A3B8"), 4));

    painter.drawLine(
        centerX - 82, 0,
        centerX - 82, h
        );

    painter.drawLine(
        centerX + 82, 0,
        centerX + 82, h
        );

    painter.drawLine(
        0, centerY - 82,
        w, centerY - 82
        );

    painter.drawLine(
        0, centerY + 82,
        w, centerY + 82
        );

    // =================================
    // 6. LANE DIVIDER MARKINGS
    // =================================

    painter.setPen(
        QPen(
            QColor("#CBD5E1"),
            2,
            Qt::DashLine
            )
        );

    painter.drawLine(
        centerX, 0,
        centerX, centerY - 82
        );

    painter.drawLine(
        centerX, centerY + 82,
        centerX, h
        );

    painter.drawLine(
        0, centerY,
        centerX - 82, centerY
        );

    painter.drawLine(
        centerX + 82, centerY,
        w, centerY
        );

    // =================================
    // 7. ZEBRA CROSSINGS
    // =================================

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#E2E8F0"));

    // North crossing
    for (int x = centerX - 65; x <= centerX + 50; x += 20) {

        painter.drawRect(
            x,
            centerY - 112,
            12,
            24
            );
    }

    // South crossing
    for (int x = centerX - 65; x <= centerX + 50; x += 20) {

        painter.drawRect(
            x,
            centerY + 88,
            12,
            24
            );
    }

    // West crossing
    for (int y = centerY - 65; y <= centerY + 50; y += 20) {

        painter.drawRect(
            centerX - 112,
            y,
            24,
            12
            );
    }

    // East crossing
    for (int y = centerY - 65; y <= centerY + 50; y += 20) {

        painter.drawRect(
            centerX + 88,
            y,
            24,
            12
            );
    }

    // =================================
    // 8. STOP LINES
    // =================================

    painter.setPen(
        QPen(QColor("#F8FAFC"), 4)
        );

    // North
    painter.drawLine(
        centerX - 75,
        centerY - 125,
        centerX + 75,
        centerY - 125
        );

    // South
    painter.drawLine(
        centerX - 75,
        centerY + 125,
        centerX + 75,
        centerY + 125
        );

    // West
    painter.drawLine(
        centerX - 125,
        centerY - 75,
        centerX - 125,
        centerY + 75
        );

    // East
    painter.drawLine(
        centerX + 125,
        centerY - 75,
        centerX + 125,
        centerY + 75
        );

    // =================================
    // 9. CENTRAL INTERSECTION
    // =================================

    painter.setPen(Qt::NoPen);

    painter.setBrush(QColor("#303B49"));

    painter.drawRoundedRect(
        QRectF(
            centerX - 78,
            centerY - 78,
            156,
            156
            ),
        5,
        5
        );

    // Subtle central road markings
    painter.setPen(
        QPen(QColor("#64748B"), 1)
        );

    painter.drawRect(
        centerX - 75,
        centerY - 75,
        150,
        150
        );

    // =================================
    // 10. ROAD SURFACE DETAILS
    // =================================

    painter.setPen(
        QPen(QColor(255, 255, 255, 18), 1)
        );

    for (int x = 0; x < w; x += 45) {

        painter.drawLine(
            x,
            centerY - 78,
            x + 15,
            centerY - 78
            );
    }

    // =================================
    // 11. DRAW ALL VEHICLES
    // =================================

    for (const MovingVehicle &vehicle : vehicles) {

        drawVehicle(painter, vehicle);
    }
}