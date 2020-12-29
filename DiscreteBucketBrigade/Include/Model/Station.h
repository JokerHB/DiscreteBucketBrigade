//
//  Station.h
//  DiscreteBucketBrigade
//
//  Created by JokerHB on 2020/12/29.
//

#ifndef Station_h
#define Station_h

#include "../Tool/State.h"

class Station
{
private:
    int id;
    State state;
    double workContent;

public:
    Station(int id, double workContent, State state = Idle);

    Station(int id, State state, double workContent);

    Station(const Station &newStation);

    ~Station();

    int GetID();

    State GetState();

    void SetState(const State &state);

    double GetWorkContent();
};

#endif /* Station_h */
