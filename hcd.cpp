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


bool can_go( const xyt &c, const ride &r )
{
    return c.t + distance_xy( c, r.from ) < r.to.t-r.score();
}

//  Returns the "value" of using this car on this ride
//  Returns -1 is the car cannot take the ride
double value_point_ride( const xyt &p, const ride &r )
{
    if (!can_go(p,r))
        return -1;

    int score = r.score();
    auto time = distance_xy(p,r.from);
    auto t_start = p.t + time;
    if (t_start<r.from.t)
        time += r.from.t-t_start;
    time += score;
    return score/(double)time;
}

double value_car_ride( const car &c, const ride &r ) { return value_point_ride( c.pos_, r ); }

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

    double best_value_ride( const ride &r )
    {
        double max_value = -1;
        std::vector<car> vc;
        int count = 0;

        while (!q.empty() && count<50)
        {
            auto c = q.top();
            vc.push_back( c );
            q.pop();
            count ++;

            auto vc = value_car_ride( c, r );
            if (vc>max_value)
                max_value = vc;
        }
        for (auto &c:vc)
            q.emplace( c );
        return max_value;
    }

    int get_nearest_ride( const xyt &p )
    {
        int best_absolute_index = -1;
        double best_absolute_v = 0;

        int best = -1;
        double max_v = 0;

        int valid_ride_count = 0;

        for (int i=0;i!=rides.size();i++)
        {
            auto r = rides[i];
            auto value = value_point_ride(p,r);
            if (value>=0)
            {
                valid_ride_count++;
                if (value>max_v)
                {
                    if (value>best_absolute_v)
                    {
                        best_absolute_v = value;
                        best_absolute_index = i;
                    }

                    if (value>=best_value_ride(r))
                    {
                        // std::cerr << "VAL = " << value << " BEST = " << best_value_ride(r) << std::endl;
                        max_v = value;
                        best = i;
                    }
                    else
                    {
                    }

                    //  Check if any car has a better value for this ride
                }
            }
        }
        if (best==-1)
        {
            //  std::cerr << "[" << valid_ride_count << "]" << std::endl;
            std::cerr << "#" << std::flush;
            return best_absolute_index;
        }
        else
        {
            if(best_absolute_index == best)
                std::cerr << "$" << std::flush;
        }
        return best;
    }

    std::priority_queue<car,std::vector<car>, compare_cars> q;

    void solve()
    {
        auto sol = soluce{ cars.size() };
        int ride_count = 0;

        for (auto c:cars)
            q.emplace( c );

        while (!q.empty())
        {
            std::cerr << "." << std::flush;

            auto c = q.top();
            q.pop();

            int ride_index = get_nearest_ride( c.pos_);

            if (ride_index!=-1)
            {
                auto ride = rides[ride_index];

                int score = ride.score();

                // std::cout << "[" << ride_index << "] wait=" << wait_time( c.pos_, ride.from ) << " score=" << score << std::endl;
                rides.erase( std::begin(rides)+ride_index );
                auto pre_ride_travel_distance = distance_xy(c.pos_, ride.from);
                if(c.pos_.t + pre_ride_travel_distance > ride.from.t)
                {
                    c.pos_.t += pre_ride_travel_distance + ride.score();
                }
                else
                {
                    score += bonus;
                    c.pos_.t = ride.from.t + ride.score();
                }
                c.pos_.x = ride.to.x;
                c.pos_.y = ride.to.y;
                q.emplace( c );
                sol.add( c.n_, ride.n_, score );

                ride_count++;
            }
            else
                std::cerr << "*" << std::flush;
        }

        std::cerr << "RIDE COUNT = " << ride_count << std::flush;
        sol.print();
    }

};



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

    std::cerr << "HCD DATA FILE " << argv[1] << std::endl;

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

    p.solve();

    return 0;
}
