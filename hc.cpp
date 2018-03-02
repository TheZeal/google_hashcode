#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <queue>

int algo = 0;

#define noSCORE

using namespace std;

struct xy
{
    int x;
    int y;
};

struct xyt
{
    int x;
    int y;
    int t;
};

inline int my_abs( int n ) { return (n<0)?-n:n; }

int distance_xy( const xyt& from, const xyt & to )
{
    return my_abs( from.x-to.x ) + my_abs( from.y-to.y );
}

int wait_time( const xyt& from, const xyt & to )
{
    return (to.t-from.t) - distance_xy( from, to );
}

struct ride
{
    int n_;
    xyt from;
    xyt to;
    explicit ride( int n ) : n_{n} {}

    int score() const { return distance_xy( from, to ); }
};

struct car
{
    int n_;
    xyt pos_;
    explicit car( int n ) : n_{n}, pos_{ 0,0,0 } {}
};

class compare_cars
{
public:
    bool operator() (const car &, const car &)
    {
        return true;
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

    static bool can_go( const xyt &p, const ride &r )
    {
        return p.t + distance_xy( p, r.from ) < r.to.t-r.score();
    }

    int get_nearest_ride( const xyt &p )
    {
        int best = -1;
        int best_d = 1000000;

        double max_v = 0;
        for (int i=0;i!=rides.size();i++)
        {
            auto r = rides[i];
            if (can_go(p,r))
            {
                auto score = r.score();
                auto dist = distance_xy(p,r.from);
                auto t_start = p.t + dist;
                if (t_start<r.from.t)
                    dist += r.from.t-t_start;
                dist += score;
                double v = score/(double)dist;
                if (v>max_v)
                {
                    max_v = v;
                    best = i;
                }
            }
        }
        return best;
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
#ifdef SCORE       
        std::cout << score_ << std::endl;
#else
        for (auto &vc:car_rides)
        {
            std::cout << vc.size() << " ";
            for (auto r:vc)
                std::cout << r << " ";
            std::cout << std::endl;
        }
#endif
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
        // int ride_index = (algo==0)?p.get_best_ride( c.pos_, delta ):p.get_nearest_ride( c.pos_ );
        int ride_index = p.get_nearest_ride( c.pos_ );

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
            c.pos_ = ride.to;
            if(c.pos_.t + distance_xy(c.pos_, ride.from) > ride.from.t)
            {
                c.pos_.t = distance_xy(c.pos_, ride.from) + c.pos_.t + ride.score();
            }
            else
            {
                c.pos_.t = ride.from.t + ride.score();
            }
            q.emplace( c );
        }
    }

    sol.print();
}

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
    assert( argc==3 );

    algo = argv[1][0]-'0';

    ifstream pbfile;
    pbfile.open( argv[2] );

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

#ifdef SCORE  
    int score = 0;

    for (auto &r:p.rides)
        score += p.bonus + distance_xy( r.from, r.to );
    std::cout << "MAX SCORE=" << score << std::endl;
#endif

    solve( p );

    return 0;
}
