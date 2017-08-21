/**
 * Author : Samson Koshy
 * Desc : depth first limit offline search on a state space
 *
 */

#include <set>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <queue>

/**
 *  Data Structure to hold the Vacuum State
 *  Dirt and Agent exist implicitly when assigned a location
 */
template <typename Location>
struct VacuumState {

    // collection of unique keys, sorted by keys
    // implemented as Red-black tree
    // c++ STL
    // Locations of the Dirt
    // http://en.cppreference.com/w/cpp/container/set
    std::set<Location> dirtLoc;

    // Location of the agent
    Location agentLoc;

};
// Supporting operators for the VacuumState Structure
template <typename Location>
bool operator< ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) {

  if ( lhs.agentLoc < rhs.agentLoc )
    return true;

  if ( lhs.dirtLoc.size() < rhs.dirtLoc.size() )
      return true;

  if ( lhs.dirtLoc.size() == rhs.dirtLoc.size() ) {
      auto lhsit = lhs.dirtLoc.begin();
      auto rhsit = rhs.dirtLoc.begin();
      for ( ; lhsit != lhs.dirtLoc.end(); ++lhsit, ++rhsit ) {
          if ( *lhsit < *rhsit) {
              return true;
          }
      }
  }

  return false;
}
template <typename Location>
bool operator== ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) {
    return  !(lhs < rhs) && !(rhs < lhs);
}
template <typename Location>
bool operator> ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) { return operator< (rhs,lhs); }
template <typename Location>
bool operator>= ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) { return !operator< (lhs,rhs); }
template <typename Location>
bool operator<= ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) { return !operator> (lhs,rhs); }
template <typename Location>
bool operator!= ( const VacuumState<Location>& lhs, const VacuumState<Location>& rhs ) { return !operator== (lhs,rhs); }


/**
 * possible actions and directions to be performed on the vacuum state
 *
 */
enum action { SUCK , N, E, S, W };
std::string printAction( action a ) {

    switch (a) {
        case SUCK : {
                    return "Suck";
        }
        case N : {
            return "North";
        }
        case E : {
            return "East";
        }
        case S : {
            return "South";
        }
        case W : {
            return "West";
        }
    }

    return "undefined";
}

/**
 * possible directions for the agent
 */
enum direction {  NORTH, EAST, SOUTH, WEST };

/**
 * Provides methods to set a state and
 * handle state change with an action is performed
 * 2x2 grid
 * Location 00 01 11 10  like a gray code
 * Action follows the order Suck, North, East, South, West ( Clockwise direction )
 * StateMaanger only supports type std::string
 */
template<typename T> class StateManager;
template <>
class StateManager < std::string > {

private :
    VacuumState < std::string > state;

    // collection of key-value pairs, hashed by keys
    // implemented with hash Table
    // c++ STL
    // http://en.cppreference.com/w/cpp/container/unordered_multimap
    const std::unordered_multimap < std::string,std::pair<std::string, direction > > directionGraph {
        {"00", {"01", NORTH} },
        {"00", {"10", EAST } },
        {"01", {"11", EAST } },
        {"01", {"00", SOUTH } },
        {"11", {"10", SOUTH } },
        {"11", {"01", WEST } },
        {"10", {"11", NORTH } },
        {"10", {"00", WEST } }
    };

    // goal state has been reached
    bool hasReachedGoalState;

    // The goal States
    // No Dirt
    // Agent can be in any location
    // total 4 possible goal state
    // collection of unique keys, sorted by keys
    // implemented as Red-black tree
    // c++ STL
    // http://en.cppreference.com/w/cpp/container/set
    const std::set< VacuumState < std::string > > goalStates {
        {
                {},     // no dirt
                "00"    // agent's location
        },
        {
                {},     // no dirt
                "01"    // agent's location
        },
        {
                {},     // no dirt
                "11"    // agent's location
        },
        {
                {},     // no dirt
                "10"    // agent's location
        }
    };

    void updateReachedGoalState() {

        auto goalState = this->goalStates.find(this->state);
        if ( goalState != this->goalStates.end() ) {
            this->hasReachedGoalState = true;
        }
    }

public :

    // initial state
    // agent is on 00 location
    StateManager() {

        this->state = {
                {"00", "01", "11", "10"},
                "00"
        };

        this->hasReachedGoalState = false;
        this->updateReachedGoalState();

    }

    VacuumState<std::string> getState() {

        return this->state;
    }

    VacuumState<std::string> setNextState( action a) {

        // check if the goal state is reached

        if ( this->hasReachedGoalState ) {

            return this->state;
        }

        switch ( a ) {

            case  SUCK : {

                // Suck action assumed to have taken place in location this->state.agentLoc
                // remove Dirt from this->state.agentLoc Location
                auto dirt = this->state.dirtLoc.find(this->state.agentLoc);

                if (dirt != this->state.dirtLoc.end()) {

                    this->state.dirtLoc.erase(dirt);

                    // check if the goal state has reached
                    this->updateReachedGoalState();
                }
                break;
            }

            case N : {

                // or move up
                // move the agent only
                // an agent can only have one unique direction based on the directinoGraph and it relies of that.
                auto range = this->directionGraph.equal_range(this->state.agentLoc);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second.second == NORTH) {
                        this->state.agentLoc = it->second.first;
                        // check if the goal state has reached
                        this->updateReachedGoalState();
                    }

                }
                break;
            }

            case E : {

                // or move up
                // move the agent only
                // an agent can only have one unique direction based on the directinoGraph and it relies of that.
                auto range = this->directionGraph.equal_range(this->state.agentLoc);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second.second == EAST) {
                        this->state.agentLoc = it->second.first;
                        // check if the goal state has reached
                        this->updateReachedGoalState();
                    }
                }
                break;
            }

            case S : {

                // or move up
                // move the agent only
                // an agent can only have one unique direction based on the directinoGraph and it relies of that.
                auto range = this->directionGraph.equal_range(this->state.agentLoc);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second.second == SOUTH) {
                        this->state.agentLoc = it->second.first;
                        // check if the goal state has reached
                        this->updateReachedGoalState();
                    }
                }
                break;
            }

            case W : {

                // or move up
                // move the agent only
                // an agent can only have one unique direction based on the directinoGraph and it relies of that.
                auto range = this->directionGraph.equal_range(this->state.agentLoc);
                for (auto it = range.first; it != range.second; ++it) {
                    if (it->second.second == WEST) {
                        this->state.agentLoc = it->second.first;
                        // check if the goal state has reached
                        this->updateReachedGoalState();
                    }
                }
                break;
            }
        }

        return this->state;
    }

    // expose if the goal state has reached
    bool isJobDone() {

        return this->hasReachedGoalState;
    }

    /**
     * Print the current state in a friendly format
     * @return void
     */
    void printState() {

        std::string loc00 = ( (this->state.dirtLoc.find("00") != this->state.dirtLoc.end() ) ? "*" :" ");
        std::string loc01 = ( (this->state.dirtLoc.find("01") != this->state.dirtLoc.end() ) ? "*" :" ");
        std::string loc11 = ( (this->state.dirtLoc.find("11") != this->state.dirtLoc.end() ) ? "*" :" ");
        std::string loc10 = ( (this->state.dirtLoc.find("10") != this->state.dirtLoc.end() ) ? "*" :" ");
        std::string locA00 =( (this->state.agentLoc == "00" ) ? "A" : " " );
        std::string locA01 =( (this->state.agentLoc == "01" ) ? "A" : " " );
        std::string locA11 =( (this->state.agentLoc == "11" ) ? "A" : " " );
        std::string locA10 =( (this->state.agentLoc == "10" ) ? "A" : " " );

        std::cout
 <<"     __________     " << std::endl
 <<"    /    |     \\    " << std::endl
 <<"    | " << locA01 << loc01 << " | " << locA11 << loc11 << "  |    "  << std::endl
 <<"    |----|-----|    "  << std::endl
 <<"    | " << locA00 << loc00 << " | " << locA10 << loc10 << "  |    "  << std::endl
 <<"    \\____|_____/    "
 << std::endl;

        return;
    }
};



int main() {

    // allocate the StateManager on the stack
    StateManager<std::string> s;

    // a set of explored states
    std::set<VacuumState<std::string>> vs;
    VacuumState<std::string> v;

    // a queue holds the action sequence
    std::queue < action > qa ;
    qa.push(SUCK);
    qa.push(N);
    qa.push(E);
    qa.push(S);
    qa.push(W);
    // used for resetting the queue
    std::queue<action> empty;

    action a = SUCK;

    // iterate over the actions in the order  SUCK , N, E, S, W  from action enumeration until a goalstate is found
    // increment every time setNextState is called : total actions :
    // backtrack if the agent was in that state before : calculate the cost and don't include the backtrack
    // print the state after the action has been attempted

    // metrics
    unsigned int cost = 0 , depth = 0, actions = 0;
    unsigned int level = 1;
    const unsigned int MAXDEPTH = 10;

    std::cout << "Initial State " << std::endl;
    auto inserted = vs.insert(s.getState());

    // make sure that the job is done or that it stays within the depth of 10.
    while ( ! s.isJobDone() && depth < MAXDEPTH  ) {

        // tested that the goal state has not been reached

        // check if explored already
        if ( ! inserted.second ) {

            --depth;
            --level;
            std::cout << "BackTracked due to action " << printAction(a) << std::endl;

        } else {

            s.printState();
            std::cout << " Cost so far : " << cost << " | depth at : " << depth << " | Level at : " << level << " | Actions so far : " << actions << std::endl;
            std::cout << " ----------------- " << std::endl;
            //reset for the next state
            if ( a != SUCK ) {
                qa = empty;
                qa.push(SUCK);
                qa.push(N);
                qa.push(E);
                qa.push(S);
                qa.push(W);
            }
        }
        // get the action
        a = qa.front();
        qa.pop();
        qa.push(a);
        inserted = vs.insert(s.setNextState(a));
        std::cout << "Action : " << printAction(a) << std::endl;
        ++cost; ++level; ++actions; ++depth;
    }

    s.printState();
    std::cout << " Cost so far : " << cost << " | depth at : " << depth << " | Level at : " << level << " | Actions so far : " << actions << std::endl;

}

