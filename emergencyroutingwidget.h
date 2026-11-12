#ifndef EMERGENCYROUTINGWIDGET_H
#define EMERGENCYROUTINGWIDGET_H

#include <QWidget>
#include <QString>
#include <QVector>

#include <queue>
#include <vector>

class QComboBox;
class QPushButton;
class QLabel;
class QTableWidget;

struct EmergencyRequest
{
    int id;
    QString vehicleType;
    int source;
    int destination;
    QString priority;
    int priorityRank;
    int sequence;
    int distance;
    QString route;
};

struct EmergencyCompare
{
    bool operator()(const EmergencyRequest &a,
                    const EmergencyRequest &b) const
    {
        if (a.priorityRank != b.priorityRank)
            return a.priorityRank > b.priorityRank;

        return a.sequence > b.sequence;
    }
};

class EmergencyRoutingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmergencyRoutingWidget(QWidget *parent = nullptr);

private:
    // Locations
    QStringList locations;

    // Graph
    QVector<QVector<std::pair<int, int>>> graph;

    // Priority Queue
    std::priority_queue<
        EmergencyRequest,
        std::vector<EmergencyRequest>,
        EmergencyCompare
        > emergencyQueue;

    // Counters
    int nextRequestId = 1;
    int sequenceNumber = 0;
    int totalDispatched = 0;

    // Input controls
    QComboBox *vehicleCombo;
    QComboBox *sourceCombo;
    QComboBox *destinationCombo;
    QComboBox *priorityCombo;

    // Buttons
    QPushButton *addEmergencyButton;
    QPushButton *dispatchButton;
    QPushButton *clearHistoryButton;

    // Dashboard labels
    QLabel *waitingValue;
    QLabel *dispatchedValue;
    QLabel *criticalValue;
    QLabel *nextEmergencyValue;

    // Tables
    QTableWidget *waitingTable;
    QTableWidget *historyTable;

    // Route information
    QLabel *routeResultLabel;
    QLabel *statusLabel;

    // Functions
    void setupUI();
    void setupGraph();
    void addRoad(int from, int to, int distance);

    bool findShortestRoute(
        int source,
        int destination,
        int &totalDistance,
        QString &route
        );

    void addEmergency();
    void dispatchNextEmergency();

    void updateWaitingTable();
    void updateDashboard();

    void applyStyle();
};

#endif // EMERGENCYROUTINGWIDGET_H