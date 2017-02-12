//
// Created by student on 07.11.2016.
//

#ifndef PROJECT2_OUTDEVICE_H
#define PROJECT2_OUTDEVICE_H

//This header contains the IoDevice class, it has fields to make the calculations after the simulation
// And it has a constructor to construct after the input has been read.

class OutDevice {
public:
    int id;
    double timequant;
    double activeTime;
    bool isavailable;
    bool isbeginstalled;
    OutDevice(int _id, double _timequant);
    ~OutDevice();

};


#endif //PROJECT2_OUTDEVICE_H
