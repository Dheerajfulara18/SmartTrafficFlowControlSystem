#include "vehiclewidget.h"

#include <QPainter>
#include <QRandomGenerator>
#include <QPaintEvent>
#include <QtMath>

VehicleWidget::VehicleWidget(QWidget *parent)
    : QWidget(parent),
    animationTimer(new QTimer(this))
{
    setMinimumSize(600, 350);

    connect(animationTimer, &QTimer::timeout,
            this, &VehicleWidget::updateVehicles);
}

void VehicleWidget::setSignalPhase(int phase)
{
    signalPhase = phase;
    yellowVisible = true;
    blinkCounter = 0;
    update();
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
    // Generate vehicles occasionally
    if (QRandomGenerator::global()->bounded(100) < 8)
        generateVehicle();

    // Yellow signal blinking
    if (signalPhase == 1 || signalPhase == 3)
    {
        blinkCounter++;

        if (blinkCounter >= 15)
        {
            yellowVisible = !yellowVisible;
            blinkCounter = 0;
        }
    }
    else
    {
        yellowVisible = true;
        blinkCounter = 0;
    }

    const qreal safeDistance = 48.0;

    int cx = width() / 2;
    int cy = height() / 2;

    for (int i = 0; i < vehicles.size(); ++i)
    {
        MovingVehicle &vehicle = vehicles[i];

        // Check which direction has green signal
        bool green = false;

        if (vehicle.lane == 0 || vehicle.lane == 2)
            green = (signalPhase == 0);

        else
            green = (signalPhase == 2);

        // Position along direction of movement
        qreal position = 0;

        if (vehicle.lane == 0)
            position = vehicle.y;

        else if (vehicle.lane == 1)
            position = width() - vehicle.x;

        else if (vehicle.lane == 2)
            position = height() - vehicle.y;

        else
            position = vehicle.x;

        // Actual stop-line position
        qreal stopLine = 0;

        if (vehicle.lane == 0)
            stopLine = cy - 130;

        else if (vehicle.lane == 1)
            stopLine = width() - (cx + 130);

        else if (vehicle.lane == 2)
            stopLine = height() - (cy + 130);

        else
            stopLine = cx - 130;

        // Check vehicle ahead
        bool vehicleAhead = false;

        for (int j = 0; j < vehicles.size(); ++j)
        {
            if (i == j)
                continue;

            const MovingVehicle &other = vehicles[j];

            if (other.lane != vehicle.lane)
                continue;

            qreal otherPosition = 0;

            if (other.lane == 0)
                otherPosition = other.y;

            else if (other.lane == 1)
                otherPosition = width() - other.x;

            else if (other.lane == 2)
                otherPosition = height() - other.y;

            else
                otherPosition = other.x;

            if (otherPosition > position &&
                otherPosition - position < safeDistance)
            {
                vehicleAhead = true;
                break;
            }
        }

        // Maintain safe distance
        if (vehicleAhead)
            continue;

        // Stop before the intersection on red/yellow
        if (!green && position < stopLine &&
            position + vehicle.speed >= stopLine)
        {
            continue;
        }

        // Move vehicle
        if (vehicle.lane == 0)
            vehicle.y += vehicle.speed;

        else if (vehicle.lane == 1)
            vehicle.x -= vehicle.speed;

        else if (vehicle.lane == 2)
            vehicle.y -= vehicle.speed;

        else
            vehicle.x += vehicle.speed;
    }

    // Remove vehicles outside screen
    for (int i = vehicles.size() - 1; i >= 0; --i)
    {
        const MovingVehicle &vehicle = vehicles[i];

        if (vehicle.x < -60 ||
            vehicle.x > width() + 60 ||
            vehicle.y < -60 ||
            vehicle.y > height() + 60)
        {
            vehicles.removeAt(i);
        }
    }

    update();
}
void VehicleWidget::drawVehicle(
    QPainter &painter,
    const MovingVehicle &vehicle)
{
    painter.save();

    // Position
    painter.translate(vehicle.x, vehicle.y);

    // Vehicle direction
    if (vehicle.lane == 0)
        painter.rotate(180);
    else if (vehicle.lane == 1)
        painter.rotate(-90);
    else if (vehicle.lane == 2)
        painter.rotate(0);
    else
        painter.rotate(90);

    painter.setRenderHint(QPainter::Antialiasing);

    // =================================
    // 1. BIKE DESIGN
    // =================================

    if (vehicle.type == "BIKE")
    {
        // Shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 100));

        painter.drawEllipse(-7, -17, 14, 36);

        // Wheels
        painter.setBrush(QColor("#111827"));

        painter.drawRoundedRect(
            QRectF(-3, -19, 6, 10), 3, 3
            );

        painter.drawRoundedRect(
            QRectF(-3, 9, 6, 10), 3, 3
            );

        // Main body
        painter.setBrush(QColor("#F59E0B"));

        painter.drawRoundedRect(
            QRectF(-5, -10, 10, 21), 4, 4
            );

        // Seat
        painter.setBrush(QColor("#1E293B"));

        painter.drawRoundedRect(
            QRectF(-4, -3, 8, 7), 3, 3
            );

        // Headlight
        painter.setBrush(QColor("#FEF3C7"));

        painter.drawEllipse(-2, -17, 4, 4);
    }

    // =================================
    // 2. AMBULANCE DESIGN
    // =================================

    else if (vehicle.type == "AMBULANCE")
    {
        // Shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 110));

        painter.drawRoundedRect(
            QRectF(-13, -21, 27, 46), 6, 6
            );

        // Body
        painter.setBrush(QColor("#F8FAFC"));
        painter.setPen(QPen(QColor("#94A3B8"), 1));

        painter.drawRoundedRect(
            QRectF(-11, -22, 22, 44), 5, 5
            );

        // Side red stripe
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#DC2626"));

        painter.drawRect(-10, -2, 20, 5);

        // Front windshield
        painter.setBrush(QColor("#7DD3FC"));

        painter.drawRoundedRect(
            QRectF(-8, -17, 16, 8), 3, 3
            );

        // Rear window
        painter.drawRoundedRect(
            QRectF(-8, 10, 16, 7), 2, 2
            );

        // Red cross
        painter.setBrush(QColor("#DC2626"));

        painter.drawRect(-2, -1, 4, 11);
        painter.drawRect(-6, 2, 12, 4);

        // Emergency beacon
        painter.setBrush(QColor("#EF4444"));

        painter.drawRoundedRect(
            QRectF(-5, -7, 10, 3), 2, 2
            );

        // Wheels
        painter.setBrush(QColor("#111827"));

        painter.drawRoundedRect(
            QRectF(-13, -14, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(9, -14, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(-13, 7, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(9, 7, 4, 9), 2, 2
            );
    }

    // =================================
    // 3. BUS DESIGN
    // =================================

    else if (vehicle.type == "BUS")
    {
        // Shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 110));

        painter.drawRoundedRect(
            QRectF(-13, -29, 27, 61), 5, 5
            );

        // Bus body
        painter.setBrush(QColor("#EAB308"));
        painter.setPen(QPen(QColor("#A16207"), 1));

        painter.drawRoundedRect(
            QRectF(-12, -30, 24, 60), 5, 5
            );

        // Front windshield
        painter.setBrush(QColor("#7DD3FC"));

        painter.drawRoundedRect(
            QRectF(-9, -25, 18, 8), 3, 3
            );

        // Passenger windows
        painter.setBrush(QColor("#BAE6FD"));

        for (int y = -13; y <= 12; y += 9)
        {
            painter.drawRoundedRect(
                QRectF(-9, y, 18, 6), 2, 2
                );
        }

        // Rear window
        painter.drawRoundedRect(
            QRectF(-8, 21, 16, 5), 2, 2
            );

        // Wheels
        painter.setBrush(QColor("#111827"));

        painter.drawRoundedRect(
            QRectF(-14, -20, 4, 11), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(10, -20, 4, 11), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(-14, 12, 4, 11), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(10, 12, 4, 11), 2, 2
            );
    }

    // =================================
    // 4. FIRE TRUCK DESIGN
    // =================================

    else if (vehicle.type == "FIRE_TRUCK")
    {
        painter.setPen(Qt::NoPen);

        // Shadow
        painter.setBrush(QColor(0, 0, 0, 110));

        painter.drawRoundedRect(
            QRectF(-13, -29, 27, 60), 5, 5
            );

        // Body
        painter.setBrush(QColor("#B91C1C"));

        painter.drawRoundedRect(
            QRectF(-12, -30, 24, 60), 5, 5
            );

        // Windshield
        painter.setBrush(QColor("#7DD3FC"));

        painter.drawRoundedRect(
            QRectF(-9, -25, 18, 8), 3, 3
            );

        // Ladder
        painter.setPen(QPen(QColor("#FDE68A"), 2));

        painter.drawLine(-5, -12, -5, 20);
        painter.drawLine(5, -12, 5, 20);

        for (int y = -8; y <= 18; y += 7)
            painter.drawLine(-5, y, 5, y);

        // Wheels
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#111827"));

        painter.drawRoundedRect(
            QRectF(-14, -20, 4, 10), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(10, -20, 4, 10), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(-14, 12, 4, 10), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(10, 12, 4, 10), 2, 2
            );
    }

    // =================================
    // 5. NORMAL CAR DESIGN
    // =================================

    else
    {
        // Shadow
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 110));

        painter.drawRoundedRect(
            QRectF(-13, -21, 27, 44), 7, 7
            );

        // Car body gradient
        QLinearGradient gradient(-12, 0, 12, 0);

        gradient.setColorAt(0, QColor("#0369A1"));
        gradient.setColorAt(0.5, QColor("#38BDF8"));
        gradient.setColorAt(1, QColor("#0369A1"));

        painter.setBrush(gradient);
        painter.setPen(QPen(QColor("#BAE6FD"), 0.8));

        painter.drawRoundedRect(
            QRectF(-11, -21, 22, 42), 6, 6
            );

        // Front windshield
        painter.setBrush(QColor("#BAE6FD"));

        painter.drawRoundedRect(
            QRectF(-8, -16, 16, 9), 3, 3
            );

        // Rear windshield
        painter.drawRoundedRect(
            QRectF(-8, 8, 16, 7), 3, 3
            );

        // Roof
        painter.setBrush(QColor(255, 255, 255, 35));

        painter.drawRoundedRect(
            QRectF(-7, -5, 14, 11), 3, 3
            );

        // Wheels
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor("#111827"));

        painter.drawRoundedRect(
            QRectF(-13, -14, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(9, -14, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(-13, 7, 4, 9), 2, 2
            );

        painter.drawRoundedRect(
            QRectF(9, 7, 4, 9), 2, 2
            );

        // Headlights
        painter.setBrush(QColor("#FEF3C7"));

        painter.drawRoundedRect(
            QRectF(-8, -21, 5, 2), 1, 1
            );

        painter.drawRoundedRect(
            QRectF(3, -21, 5, 2), 1, 1
            );

        // Tail lights
        painter.setBrush(QColor("#EF4444"));

        painter.drawRoundedRect(
            QRectF(-8, 19, 5, 2), 1, 1
            );

        painter.drawRoundedRect(
            QRectF(3, 19, 5, 2), 1, 1
            );
    }

    painter.restore();
}
void VehicleWidget::drawBackground(QPainter &painter)
{
    painter.fillRect(rect(), QColor("#263C32"));

    // Ground texture
    painter.setPen(QPen(QColor(255, 255, 255, 12), 1));

    for (int x = 0; x < width(); x += 30) {
        for (int y = 0; y < height(); y += 30) {
            painter.drawPoint(x, y);
        }
    }
}
void VehicleWidget::drawRoads(QPainter &painter)
{
    int cx = width() / 2;
    int cy = height() / 2;

    // Road shadows
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 65));

    painter.drawRect(cx - 88, 5, 176, height() - 10);
    painter.drawRect(5, cy - 88, width() - 10, 176);

    // Asphalt gradient
    QLinearGradient roadGradient(0, 0, width(), height());

    roadGradient.setColorAt(0, QColor("#3B4552"));
    roadGradient.setColorAt(0.5, QColor("#252E3A"));
    roadGradient.setColorAt(1, QColor("#343E4B"));

    painter.setBrush(roadGradient);

    painter.drawRect(cx - 82, 0, 164, height());
    painter.drawRect(0, cy - 82, width(), 164);

    // Road edges
    painter.setPen(QPen(QColor("#A3ADB8"), 3));

    painter.drawLine(cx - 82, 0, cx - 82, height());
    painter.drawLine(cx + 82, 0, cx + 82, height());

    painter.drawLine(0, cy - 82, width(), cy - 82);
    painter.drawLine(0, cy + 82, width(), cy + 82);
}
void VehicleWidget::drawSidewalks(QPainter &painter)
{
    int cx = width() / 2;
    int cy = height() / 2;

    painter.setPen(Qt::NoPen);

    // Outer pavement strips
    painter.setBrush(QColor("#64748B"));

    painter.drawRect(cx - 94, 0, 12, height());
    painter.drawRect(cx + 82, 0, 12, height());

    painter.drawRect(0, cy - 94, width(), 12);
    painter.drawRect(0, cy + 82, width(), 12);

    // Pavement highlights
    painter.setPen(QPen(QColor("#CBD5E1"), 2));

    painter.drawLine(cx - 94, 0, cx - 94, height());
    painter.drawLine(cx + 94, 0, cx + 94, height());

    painter.drawLine(0, cy - 94, width(), cy - 94);
    painter.drawLine(0, cy + 94, width(), cy + 94);
}
void VehicleWidget::drawRoadMarkings(QPainter &painter)
{
    int cx = width() / 2;
    int cy = height() / 2;

    painter.setPen(QPen(QColor("#E2E8F0"), 2, Qt::DashLine));

    // Lane dividers
    painter.drawLine(cx, 0, cx, cy - 82);
    painter.drawLine(cx, cy + 82, cx, height());

    painter.drawLine(0, cy, cx - 82, cy);
    painter.drawLine(cx + 82, cy, width(), cy);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor("#F1F5F9"));

    // North and South zebra crossings
    for (int x = cx - 65; x <= cx + 50; x += 20) {
        painter.drawRoundedRect(x, cy - 112, 12, 24, 2, 2);
        painter.drawRoundedRect(x, cy + 88, 12, 24, 2, 2);
    }

    // East and West zebra crossings
    for (int y = cy - 65; y <= cy + 50; y += 20) {
        painter.drawRoundedRect(cx - 112, y, 24, 12, 2, 2);
        painter.drawRoundedRect(cx + 88, y, 24, 12, 2, 2);
    }

    // Stop lines
    painter.setPen(QPen(QColor("#FFFFFF"), 4));

    painter.drawLine(cx - 75, cy - 125, cx + 75, cy - 125);
    painter.drawLine(cx - 75, cy + 125, cx + 75, cy + 125);

    painter.drawLine(cx - 125, cy - 75, cx - 125, cy + 75);
    painter.drawLine(cx + 125, cy - 75, cx + 125, cy + 75);
}
void VehicleWidget::drawStreetElements(QPainter &painter)
{
    int cx = width() / 2;
    int cy = height() / 2;

    // Central junction
    painter.setPen(QPen(QColor("#64748B"), 2));
    painter.setBrush(QColor("#303B49"));

    painter.drawRoundedRect(
        QRectF(cx - 78, cy - 78, 156, 156),
        5, 5
        );

    // Subtle junction markings
    painter.setPen(QPen(QColor("#94A3B8"), 1));

    painter.drawLine(cx - 60, cy - 60, cx + 60, cy - 60);
    painter.drawLine(cx - 60, cy + 60, cx + 60, cy + 60);
    // =================================
    // TRAFFIC SIGNAL SYSTEM
    // =================================

    auto drawSignal = [&](int x, int y, bool vertical)
    {
        painter.setPen(Qt::NoPen);

        // Signal pole
        painter.setBrush(QColor("#64748B"));
        painter.drawRoundedRect(
            QRectF(x + 7, y + 42, 6, 22), 2, 2
            );

        // Signal housing
        painter.setBrush(QColor("#111827"));
        painter.setPen(QPen(QColor("#64748B"), 1));

        painter.drawRoundedRect(
            QRectF(x, y, 20, 48), 5, 5
            );

        bool redActive = false;
        bool yellowActive = false;
        bool greenActive = false;

        if (vertical)
        {
            redActive = (signalPhase == 2 || signalPhase == 3);
            yellowActive = (signalPhase == 1);
            greenActive = (signalPhase == 0);
        }
        else
        {
            redActive = (signalPhase == 0 || signalPhase == 1);
            yellowActive = (signalPhase == 3);
            greenActive = (signalPhase == 2);
        }

        // Red light
        painter.setBrush(
            redActive ? QColor("#FF3030") : QColor("#451A1A")
            );
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(x + 4, y + 4, 12, 12);

        // Yellow light
        painter.setBrush(
            yellowActive && yellowVisible
                ? QColor("#FACC15")
                : QColor("#594512")
            );
        painter.drawEllipse(x + 4, y + 18, 12, 12);

        // Green light
        painter.setBrush(
            greenActive ? QColor("#22C55E") : QColor("#14532D")
            );
        painter.drawEllipse(x + 4, y + 32, 12, 12);
    };

    // Four signal heads
    drawSignal(cx - 112, cy - 112, true);
    drawSignal(cx + 92, cy + 55, true);

    drawSignal(cx + 92, cy - 112, false);
    drawSignal(cx - 112, cy + 55, false);
}
void VehicleWidget::drawVehicles(QPainter &painter)
{
    for (const MovingVehicle &vehicle : vehicles)
    {
        drawVehicle(painter, vehicle);
    }
}

void VehicleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    drawBackground(painter);
    drawRoads(painter);
    drawSidewalks(painter);
    drawRoadMarkings(painter);
    drawStreetElements(painter);
    drawVehicles(painter);
}