/*
Student Name:A. Emirhan Karagul
Student Number: 2013400057
Project Number: 2
Operating System: Cmpe250.ova
Compile Status: compiled
Program Status: working
Notes: Program is completely working. Comments of the classes are in the header files.
*/



#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include "Task.h"
#include "CPU.h"
#include "OutDevice.h"
#include <vector>
#include <iterator>
#include <string>
#include <queue>
#include <zconf.h>

using namespace std;

const int NUM_SECONDS = 1;

//To compare the events by their event times in order the order the tasks chronologically.
// If event times are equal, this compares them by another priorities.
struct LessThanByEventTime
{
    bool operator()(const Task& lhs, const Task& rhs) const
    {
        if(lhs.eventtime==rhs.eventtime){
            if(lhs.state==rhs.state){
                if(lhs.outputWork == rhs.outputWork){
                    return lhs.CPUid>rhs.CPUid;
                }
                return lhs.outputWork >rhs.outputWork;

            }
            return lhs.state < rhs.state;
        }
        return lhs.eventtime > rhs.eventtime;
    }
};

//To compare the tasks by their Cpuworks for the shortest job first
//To push them to first level prioqueue.

struct LessThanByCPUWork
{
    bool operator()(const Task& lhs, const Task& rhs) const
    {
            if(lhs.CpuWork==rhs.CpuWork){
                return lhs.outputWork > rhs.outputWork;
            }
        return lhs.CpuWork > rhs.CpuWork;
    }
};

//Checks if there is any available IO Device.

bool isIoDevavailable(vector<OutDevice> &v){
    for (int i = 0; i <v.size() ; ++i) {
        if(v[i].isavailable){
            return true;
        }
    }
    return false;
}

// Sends the task to the topmost available IO Device.
// Calculates the event time and pushes it back to the event queue.

void sendtoIoDev(vector<OutDevice> &v, Task &t, double time, priority_queue<Task, vector<Task>, LessThanByEventTime> & q){
    for (int i = 0; i <v.size() ; ++i) {
        double proctime;
        if(t.IOid>-1&&v[t.IOid].isavailable){
            if(v[t.IOid].timequant<t.outputWork) {
                proctime = v[t.IOid].timequant;
                t.eventtime = time + proctime;
                t.IOid = t.IOid;
                t.timespentIo += proctime;
                t.outputWork -= proctime;
                t.state = 2;
                v[t.IOid].isavailable = false;
                v[t.IOid].activeTime += proctime;
                q.push(t);
            }else{
                proctime = t.outputWork;
                t.eventtime = time + proctime;
                t.IOid = t.IOid;
                t.timespentIo += proctime;
                t.outputWork -= proctime;
                t.state = 3; //as in finished
                v[t.IOid].isavailable = false;
                v[t.IOid].activeTime += proctime;
                q.push(t);

            }
            break;
        }
            if(v[i].isavailable){
                if(v[i].timequant<t.outputWork) {
                    proctime = v[i].timequant;
                    t.eventtime = time + proctime;
                    t.IOid = i;
                    t.timespentIo += proctime;
                    t.outputWork -= proctime;
                    t.state = 2;
                    v[i].isavailable = false;
                    v[i].activeTime += proctime;
                    q.push(t);
                }else{
                    proctime = t.outputWork;
                    t.eventtime = time + proctime;
                    t.IOid = i;
                    t.timespentIo += proctime;
                    t.outputWork -= proctime;
                    t.state = 3; //as in finished
                    v[i].isavailable = false;
                    v[i].activeTime += proctime;
                    q.push(t);

                }
                break;
            }
    }
}

//Checks if any of the CPUs is available.

bool isCpusavailable(vector<CPU> &v){
    for (int i = 0; i <v.size() ; ++i) {
        if(v[i].isavaiable){
            return true;
        }
    }
    return false;
}

//Sends the task to the topmost available CPU.
//Calculates the exit time and pushes the task to the event queue.

void sendtoCPU(vector<CPU> &v, Task &t, double time,priority_queue<Task, vector<Task>, LessThanByEventTime> & q){
    for (int i = 0; i <v.size() ; ++i) {
        double proctime;
        if(v[i].isavaiable){
            proctime = t.CpuWork/v[i].frequency;
            t.eventtime = proctime+time;
            t.CPUid = i;
            t.timespentCpu = proctime;
            t.state=1;
            v[i].isavaiable = false;
            v[i].activeTime += proctime;

            q.push(t);
            break;
        }
    }
}


//Main Stuff

int main (int argc, char* argv[]) {

    //These lines handle the input.


    if (argc != 3) {
        cout << "Run the code with the following command: ./project2 [input_file] [output_file]" << endl;
        return 1;
    }

    cout << "input file: " << argv[1] << endl;
    cout << "output file: " << argv[2] << endl;

    ifstream infile(argv[1]);



    int numofCPUs;
    infile>>numofCPUs;

        //Instantiate the processors and push them into a vector

        vector<CPU> processors;
        double freq;
    for (int i = 0; i <numofCPUs ; ++i) {
        infile>>freq;
        CPU *temp = new CPU(i+1,freq);
        processors.push_back(*temp);
    }

    int numofOutDevice;
    infile>>numofOutDevice;

        //Instantiate the IODevices and push them into a vector.

        vector <OutDevice> outdevices;
        double quanta;
    for (int j = numofCPUs+2; j <numofCPUs+2+numofOutDevice ; ++j) {
        infile>>quanta;
        OutDevice *temp = new OutDevice(j-numofCPUs-1,quanta);
        outdevices.push_back(*temp);
    }
    vector<string> tokens;
    int numofTasks;
    infile>>numofTasks;
    vector<Task> tasks;

        //Instantiates the tasks with given parameters.
        //Right after pushed them to the event queue with lesser event times.
    double a, b ,c;
    for (int k =numofCPUs+numofOutDevice+3; k <numofCPUs+numofOutDevice+numofTasks+3 ; ++k) {

        infile>>a;
        infile>>b;
        infile>>c;
        Task *temp = new Task(a,b,c,k-numofCPUs-numofOutDevice-2);
        temp->eventtime=temp->arrivalTime;
        tasks.push_back(*temp);

    }

        priority_queue<Task, vector<Task>, LessThanByEventTime> orderedevents;
        priority_queue<Task, vector<Task>, LessThanByCPUWork> firstlevelqueue;

    for (int i = 0; i <tasks.size() ; ++i) {
        orderedevents.push(tasks[i]);
    }


    queue<Task> secondlevel;
    vector<Task> finished;
    double time =0;
    int maxsize1st=0;
    int maxsize2nd=0;

    Task currEvent = orderedevents.top();
    Task temp = orderedevents.top();
    Task temp2 = orderedevents.top();

    //This is the main loop of the simulation
    // All The events are handled here

    while(true){
        if(orderedevents.empty()) break;
       // for(int i = 0 ; i < NUM_SECONDS ; i++) { usleep(1000 * 1000); }

        currEvent = orderedevents.top();
        time=currEvent.eventtime;
        orderedevents.pop();


        if(currEvent.state == -1) { //For the arrival events


            if (firstlevelqueue.empty()&&isCpusavailable(processors)) {
                sendtoCPU(processors, currEvent, time, orderedevents);

            } else if(!firstlevelqueue.empty()&&isCpusavailable(processors)){

                if(currEvent.CpuWork<firstlevelqueue.top().CpuWork){
                    sendtoCPU(processors, currEvent, time, orderedevents);
                }else{
                    firstlevelqueue.push(currEvent);
                }
            }else{
                firstlevelqueue.push(currEvent);
                if(firstlevelqueue.size()>maxsize1st){
                    maxsize1st = (int) firstlevelqueue.size();
                }
            }
        }else if(currEvent.state ==1){ // For the cpu exit events.
            processors[currEvent.CPUid].isavaiable=true;


            if(secondlevel.empty() && isIoDevavailable(outdevices)){

                sendtoIoDev(outdevices, currEvent, time, orderedevents);
            }else if(!secondlevel.empty() && isIoDevavailable(outdevices)){

                    if(currEvent.outputWork<secondlevel.front().outputWork){
                        sendtoIoDev(outdevices, currEvent, time, orderedevents);
                    }else{
                        secondlevel.push(currEvent);
                    }

            }else{
                secondlevel.push(currEvent);
            }
            if(!firstlevelqueue.empty()) {
                temp = firstlevelqueue.top();
                firstlevelqueue.pop();
                sendtoCPU(processors,temp, time, orderedevents);
            }
            if(secondlevel.size()>maxsize2nd){
                maxsize2nd = (int) secondlevel.size();
            }

        }else if(currEvent.state==2){  // Handles the tasks that are returning from the Io Devices and not yet finished
            outdevices[currEvent.IOid].isavailable=true;
            if(!isIoDevavailable(outdevices)){
                secondlevel.push(currEvent);
            }else{
                if(secondlevel.empty()){ //kendi işlemcisine gönder
                    sendtoIoDev(outdevices,currEvent,time,orderedevents);
                }else{
                    temp2= secondlevel.front();
                    secondlevel.pop();
                    sendtoIoDev(outdevices,temp2,time,orderedevents);
                    secondlevel.push(currEvent);
                }
            }


        }else{  //Handles the finished tasks.
            outdevices[currEvent.IOid].isavailable=true;
            currEvent.exitTime=time;
            finished.push_back(currEvent);
            if(!secondlevel.empty()) {
                temp2 = secondlevel.front();
                secondlevel.pop();
                sendtoIoDev(outdevices, temp2, time, orderedevents);
            }
        }


    }

    double maxCPUactive =0;
    int maxCPU =0;

    for (int l = 0; l <processors.size() ; ++l) {
            if(processors[l].activeTime>maxCPUactive){
                maxCPU = l+1;
                maxCPUactive = processors[l].activeTime;
            }


    }


    double maxOutactive =0;
    double maxIO;

    for (int l = 0; l <outdevices.size() ; ++l) {
        if(outdevices[l].activeTime>maxOutactive){
            maxIO = l+1;
            maxOutactive = outdevices[l].activeTime;
        }


    }
        double timespent;
    for (int m = 0; m <finished.size() ; ++m) {
        timespent += finished[m].exitTime-finished[m].arrivalTime;
    }
    timespent= timespent / finished.size();

        double longestwait=0;
        double currentwait=0;
        double allwait=0;
    for (int m = 0; m <finished.size() ; ++m) {
        currentwait = finished[m].exitTime-finished[m].arrivalTime-finished[m].timespentCpu-finished[m].timespentIo;
        allwait += currentwait/finished.size();
            if(currentwait>longestwait){
                longestwait = currentwait;
            }

    }


    freopen(argv[2],"w",stdout);
    cout<<maxsize1st<<endl;
    cout<<maxsize2nd<<endl;
    cout<<maxCPU<<endl;
    cout<<maxIO<<endl;
    printf("%.6f", allwait);
    cout<<endl;
    printf("%.6f", longestwait);
    cout<<endl;
    printf("%.6f", timespent);




}




