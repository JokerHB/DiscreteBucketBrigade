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
#include <sstream>
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
    std::vector<Worker *> waiting;
    std::vector<Worker *> handoff;
    std::vector<Worker *> finish;

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

    void RemoveItem(std::vector<Worker *>& vec, Worker *item);

    void AddWaitWorker(Worker *worker);

    Worker *GetWatiWorker();

    bool IsWaitEmpty();

    void AddHandoffWorker(Worker *worker);

    Worker *GetHandoffWorker();

    bool IsHandoffEmpty();

    void AddFinishWorker(Worker *worker);

    Worker *GetFinishWorker();

    bool IsFinishEmpty();

    void Process(double workTime);

    bool CanWalkBack(Worker *worker);

    std::vector<Worker *> Handoff(int stationNum);

    void ArrangeWorker();

    std::string ToString();
};

#endif /* Station_h */
