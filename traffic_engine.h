#ifndef TRAFFIC_ENGINE_H
#define TRAFFIC_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VEHICLES 100
#define MAX_LANES 4

/* Vehicle categories */
typedef enum {
    VEHICLE_CAR,
    VEHICLE_BIKE,
    VEHICLE_BUS,
    VEHICLE_AMBULANCE,
    VEHICLE_FIRE_TRUCK
} VehicleType;

/* Vehicle information */
typedef struct {
    int id;
    VehicleType type;
    int arrivalTime;
    int waitTime;
    int lane;
} Vehicle;

/* FIFO Queue for vehicles */
typedef struct {
    Vehicle items[MAX_VEHICLES];
    int front;
    int rear;
    int count;
} VehicleQueue;

/* Queue operations */
void queueInit(VehicleQueue *queue);

int enqueue(VehicleQueue *queue, Vehicle vehicle);

int dequeue(VehicleQueue *queue, Vehicle *vehicle);

int queueSize(const VehicleQueue *queue);

int queueIsEmpty(const VehicleQueue *queue);

int queueIsFull(const VehicleQueue *queue);

#ifdef __cplusplus
}
#endif

#endif
