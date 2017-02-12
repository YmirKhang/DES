//
// Created by student on 07.11.2016.
//

#ifndef PROJECT2_CPU_H
#define PROJECT2_CPU_H

//This is the Cpu class, takes the parameters from the input and instantiates the cpu.
//Has some fields to make calculations and a constructor.

class CPU {
public:
    double frequency;
    int id;
    double activeTime;
    bool isavaiable;

    CPU(int _id, double _freq);
    ~CPU();

};


#endif //PROJECT2_CPU_H
