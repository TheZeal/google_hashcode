//  Cleaned-up google hash code entry

#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <queue>
#include <cmath>

/// A point in time
struct xyt
{
    int x;
    int y;
    int t;
};

/// Physical distance between two point-in-time (time is ignored)
int distance_xy( const xyt& from, const xyt & to )
{
    return std::abs( from.x-to.x ) + std::abs( from.y-to.y );
}

/// How long you will wait at destination if you start from original point
int wait_time( const xyt& from, const xyt & to )
{
    return (to.t-from.t) - distance_xy( from, to );
}

/// #### Add a last-pick method
/// A ride
struct ride
{
    int n_;     /// Ride number

    xyt from;   /// Pick place and time should pick the person
    xyt to;     /// Drop place and time

    explicit ride( int n ) : n_{n} {}

    int score() const { return distance_xy( from, to ); }
};

/// A car
struct car
{
    int n_;     /// Car number

    xyt pos_;   /// Current car position

    explicit car( int n ) : n_{n}, pos_{ 0,0,0 } {}
};

class compare_cars
{
public:
    bool operator() (const car &a, const car &b)
    {
        return a.pos_.t>b.pos_.t;
        // return true;
    }
};

void print( const car &c )
{
    std::cout << c.n_ << ":" << c.pos_.x << "," << c.pos_.y << "@" << c.pos_.t << std::endl;
}

bool possible( ride &r, int delay )
{
    return wait_time( r.from, r.to )>=delay;
}

struct pb
{
    int width;
    int height;
    int car_count;
    int ride_count;
    int bonus;
    int max_time;

    std::vector<ride> rides;
    std::vector<car> cars;

    int get_best_ride( const xyt &p, int &delta )
    {
        int best = -1;
        int best_d = 1000000;

        for (int i=0;i!=rides.size();i++)
        {
            auto r = rides[i];
            auto d = wait_time( p, r.from );
            if (d>=0 && d<best_d)
            {
                best = i;
                best_d = d;
            }
        }

        delta = 0;

        if (best!=-1)
            return best;

        best = -1;
        best_d = 1000000;
        for (int i=0;i!=rides.size();i++)
        {
            auto r = rides[i];
            auto d = -wait_time( p, r.from );
            if (d<best_d)
                if (possible(r,d))
                {
                    best = i;
                    best_d = d;
                }
        }
        
        delta = best_d;
        if (best!=-1)
            return best;

        return -1;
    }
};


struct soluce
{
    std::vector<std::vector<int>> car_rides;
    int score_ = 0;

    soluce( size_t car_count ) { car_rides.resize( car_count ); }

    void add( int car, int ride, int score ) { car_rides[car].push_back( ride ); score_ += score; }

    void print() const
    {
        std::cerr << "  SOL SCORE : " << score_ << std::endl;
        for (auto &vc:car_rides)
        {
            std::cout << vc.size() << " ";
            for (auto r:vc)
                std::cout << r << " ";
            std::cout << std::endl;
        }
    }
};

void solve( pb &p )
{
    auto sol = soluce{ p.cars.size() };

    std::priority_queue<car,std::vector<car>, compare_cars> q;

    for (auto c:p.cars)
        q.emplace( c );

    while (!q.empty())
    {
        auto c = q.top();
        q.pop();
        int delta = -1;
        int ride_index = p.get_best_ride( c.pos_, delta );

        if (ride_index!=-1)
        {
            auto ride = p.rides[ride_index];

            int score = 0;
            if (delta==0)
                score += p.bonus;
            score += distance_xy( ride.from, ride.to );
            sol.add( c.n_, ride.n_, score );

            // std::cout << "[" << ride_index << "] wait=" << wait_time( c.pos_, ride.from ) << " score=" << score << std::endl;
            p.rides.erase( std::begin(p.rides)+ride_index );
            auto pre_ride_travel_distance = distance_xy(c.pos_, ride.from);
            if(c.pos_.t + pre_ride_travel_distance > ride.from.t)
            {
                c.pos_.t += pre_ride_travel_distance + ride.score();
            }
            else
            {
                c.pos_.t = ride.from.t + ride.score();
            }
            c.pos_.x = ride.to.x;
            c.pos_.y = ride.to.y;
            q.emplace( c );
        }
    }

    sol.print();
}

void read( std::istream &i, ride &r )
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

    std::ifstream pbfile;
    pbfile.open( argv[1] );

    std::cerr << "HC  DATA FILE " << argv[1] << std::endl;

    pb p;
    pbfile >> p.height;
    pbfile >> p.width;
    pbfile >> p.car_count;
    pbfile >> p.ride_count;
    pbfile >> p.bonus;
    pbfile >> p.max_time;
    
    for (auto i=0;i!=p.ride_count;i++)
    {
        ride r{i};
        read( pbfile, r );
        p.rides.push_back( r );
    }

    for (auto i=0;i!=p.car_count;i++)
    {
        p.cars.push_back( car{ i } );
    }

    pbfile.close();

    int score = 0;

    for (auto &r:p.rides)
        score += p.bonus + distance_xy( r.from, r.to );
    std::cerr << "  MAX SCORE : " << score << std::endl;

    solve( p );

    return 0;
}
