
#include "traffic_engine.h"

    /* Initialize the queue */
    void queueInit(VehicleQueue *queue)
{
    if (queue == 0) {
        return;
    }

    queue->front = 0;
    queue->rear = -1;
    queue->count = 0;
}

/* Add a vehicle to the queue */
int enqueue(VehicleQueue *queue, Vehicle vehicle)
{
    if (queue == 0 || queue->count >= MAX_VEHICLES) {
        return 0;
    }

    queue->rear = (queue->rear + 1) % MAX_VEHICLES;

    queue->items[queue->rear] = vehicle;
    queue->count++;

    return 1;
}

/* Remove the next vehicle from the queue */
int dequeue(VehicleQueue *queue, Vehicle *vehicle)
{
    if (queue == 0 || vehicle == 0 || queue->count == 0) {
        return 0;
    }

    *vehicle = queue->items[queue->front];

    queue->front = (queue->front + 1) % MAX_VEHICLES;
    queue->count--;

    return 1;
}

/* Return the number of waiting vehicles */
int queueSize(const VehicleQueue *queue)
{
    if (queue == 0) {
        return 0;
    }

    return queue->count;
}

/* Check whether the queue is empty */
int queueIsEmpty(const VehicleQueue *queue)
{
    return queue == 0 || queue->count == 0;
}

/* Check whether the queue is full */
int queueIsFull(const VehicleQueue *queue)
{
    return queue == 0 || queue->count >= MAX_VEHICLES;
}

