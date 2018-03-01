#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

using namespace std;

//  A car in the simulation

struct xyt
{
    int x;
    int y;
    int t;
};

struct ride
{
    xyt from;
    xyt to;
};

struct pb
{
    int width;
    int height;
    int car_count;
    int ride_count;
    int bonus;
    int max_time;

    std::vector<ride> rides;
};

void read( istream &i, ride &r )
{
    i >> r.from.y;
    i >> r.from.x;
    i >> r.to.y;
    i >> r.to.x;
    i >> r.from.t;
    i >> r.to.t;
}

int main( int argc, char **argv )
{
    assert( argc==2 );

    ifstream pbfile;
    pbfile.open( argv[1] );

    pb p;
    pbfile >> p.height;
    pbfile >> p.width;
    pbfile >> p.car_count;
    pbfile >> p.ride_count;
    pbfile >> p.bonus;
    pbfile >> p.max_time;
    
    for (auto i=0;i!=p.ride_count;i++)
    {
        ride r;
        read( pbfile, r );
        p.rides.push_back( r );
    }

    pbfile.close();
    return 0;
}
