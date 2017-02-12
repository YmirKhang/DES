//
// Created by student on 07.11.2016.
//

#include "CPU.h"

CPU::CPU(int _id, double _freq ) {

    this->id=_id;
    this->frequency = _freq;
    this->activeTime =0;
    this->isavaiable=true;
}

CPU::~CPU() {

}

