//
// Created by student on 07.11.2016.
//

#ifndef PROJECT2_TASK_H
#define PROJECT2_TASK_H

// This is the Task class header.
// This class has lots of fields to make the required calculations.
// Tasks can be also used as events, during the simulation the state of the task increments
// in order to change the event type and make the necessary pushes to right queues.

class Task {
public:
    double arrivalTime;
    double exitTime;
    double CpuWork;
    double outputWork;
    int id;
    int state;
    int CPUid;
    int IOid;
    bool isStalling;
    double timespentCpu;
    double timespentIo;
    double eventtime;

    Task();
    Task(double _arrival, double _CpuWork, double _outputWork,int _id);

    ~Task();

};


#endif //PROJECT2_TASK_H
