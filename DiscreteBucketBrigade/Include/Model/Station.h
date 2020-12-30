//
//  Station.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Station_h
#define Station_h

#include <stack>
#include <queue>
#include <cmath>
#include <iostream>
#include "./Worker.h"
#include "../Tool/State.h"

class Station
{
private:
    const static double EPS;
    int id;
    State state;
    double workContent;
    Worker *worker;
    std::queue<Worker *> waitingQueue;
    std::stack<Worker *> handoffStack;
    std::stack<Worker *> finishStack;

public:
    Station(int id, double workContent, State state = Idle);

    Station(int id, State state, double workContent);

    Station(int id, State state, Worker *worker, double workContent);

    Station(const Station &newStation);

    ~Station();

    int GetID();

    void SetWorker(Worker *woker);

    State GetState();

    void SetState(const State &state);

    double GetWorkContent();

    double GetProcessTime();

    void AddWaitWorker(Worker *worker);

    Worker *GetWatiWorker();

    void AddHandoffWorker(Worker *worker);

    Worker *GetHandoffWorker();

    void AddFinishWorker(Worker *worker);

    Worker *GetFinishWorker();

    Worker *Process(double workTime);

    Worker *ArrangeWorker();
};

#endif /* Station_h */
