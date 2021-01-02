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
#include "../Tool/Direction.h"

struct Node
{
    Worker *worker;
    Node(Worker *worker) : worker(worker) {}
    bool operator<(const Node &b) const
    {
        return worker->GetID() - b.worker->GetID();
    }
};

class Station
{
private:
    const static double EPS;
    int id;
    State state;
    double workContent;
    Worker *worker;
    std::priority_queue<Node> waitingQueue;
    std::priority_queue<Node> handoffStack;
    std::priority_queue<Node> finishStack;

public:
    Station(int id, double workContent, State state = Idle);

    Station(int id, State state, double workContent);

    Station(int id, State state, Worker *worker, double workContent);

    Station(const Station &newStation);

    ~Station();

    int GetID();

    void FreeWorker();

    void SetWorker(Worker *woker);

    State GetState();

    void SetState(const State &state);

    double GetWorkContent();

    double GetProcessTime();

    void AddWaitWorker(Worker *worker);

    Worker *GetWatiWorker();

    bool IsWaitQueueEmpty();

    void AddHandoffWorker(Worker *worker);

    Worker *GetHandoffWorker();

    bool IshandoffStackEmpty();

    void AddFinishWorker(Worker *worker);

    Worker *GetFinishWorker();

    bool IsFinishStackEmpty();

    void Process(double workTime);

    std::vector<Worker *> Handoff(int stationNum);

    void ArrangeWorker();
};

#endif /* Station_h */
