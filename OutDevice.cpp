//
// Created by student on 07.11.2016.
//

#include "OutDevice.h"

OutDevice::OutDevice(int _id, double _timequant) {
    this->id= _id;
    this->timequant = _timequant;
    this->activeTime = 0;
    this->isavailable =true;
    this->isbeginstalled=false;
}

OutDevice::~OutDevice() {

}
