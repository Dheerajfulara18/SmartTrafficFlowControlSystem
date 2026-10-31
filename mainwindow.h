#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "traffic_engine.h"
#include "vehiclewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTimer;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // Traffic simulation state
    VehicleQueue lanes[MAX_LANES];

    int tick = 0;
    int nextVehicleId = 1;

    long long totalWaitTime = 0;
    int completedVehicles = 0;

    bool verticalGreen = true;

    // Simulation timer
    QTimer *simulationTimer = nullptr;

    // Vehicle animation widget
    VehicleWidget *vehicleWidget = nullptr;

    // Dashboard statistics
    QLabel *activeVehiclesValue = nullptr;
    QLabel *avgWaitValue = nullptr;
    QLabel *congestionValue = nullptr;
    QLabel *emergenciesValue = nullptr;
};

#endif // MAINWINDOW_H