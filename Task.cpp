//
// Created by student on 07.11.2016.
//

#include "Task.h"

Task::Task(double _arrival, double _CpuWork, double _outputWork,int _id) {

    this->arrivalTime = _arrival;
    this->CpuWork = _CpuWork;
    this->outputWork = _outputWork;
    this->id=_id;
    this->state =-1;
    this->timespentIo =0;
    this->IOid =-1;
    this->isStalling=false;

}

Task::~Task() {

}

Task::Task() {

}
