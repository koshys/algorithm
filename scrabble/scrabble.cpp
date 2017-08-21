/**
 * Author : Samson Koshy
 * Desc : scrabble 4x4
 *
 */

#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <random>
#include <string>
#include <ctime>


/**
 *  1. load the sowpods into an ordered redblack tree std::set
 *  2. scrabble board 15x15 that will hold the letters
 *  3. mirror scrabble board that holds the points, so the points can be associated with the letters.
 *     for this iteration, there is only one double word in the center
 *  4. hold the alphabets with points in an ordered map , alphabet is the key and the points is the value.
 *  4. get the picking of the letters to the rack working
 */

/**
 * overall game board constants
 */
const int LEN = 15;
const char BLANK = '_';
const int RACKSIZE = 7;

/**
 * represent the points as well as the quantity in alpha
 */
typedef struct aData { int points; int quantity; } alphaData;

class Board {

private:
    // hold all the legal words in here
    std::set<std::string> sowpods;
    bool sowpodsLoaded;

    // the actual board
    char b[LEN][LEN];

    // the rack
    std::string rack;

    // best word choice
    std::map<int,std::string> bestWords;

    // test cases
    std::set<std::string> tests;

    // utility

    bool loadSowpods() {

        // load from the file
        std::ifstream ifs;
        std::string value;
        ifs.open("SOWPODS_complete.txt");
        if (ifs.is_open()) {

            while (!ifs.eof()) {
                std::getline(ifs, value);
                if ( value.size() != 0 ) {
                    if (!value.empty() && value[value.size() - 1] == '\r')
                        value.erase(value.size() - 1);
                    this->sowpods.insert(value);
                }
                value.clear();
            }
        } else {
            std::cout << "error opening file SOWPODS_complete.txt" << std::endl;
            return false;
        }
        ifs.close();
        if (ifs.is_open()) {
            std::cout << "error closing file SOWPODS_complete.txt" << std::endl;
            return false;
        }

        return true;
    }

    void loadTestCases() {

        // load from the file
        std::ifstream ifs;
        std::string value;
        ifs.open("test_rack_file.txt");
        if (ifs.is_open()) {

            while (!ifs.eof()) {
                std::getline(ifs, value);
                if ( value.size() != 0 ) {
                    if (!value.empty() && value[value.size() - 1] == '\r')
                        value.erase(value.size() - 1);
                    this->tests.insert(value);
                }
                value.clear();
            }
        } else {
            std::cout << "error opening file test_rack_file.txt" << std::endl;
        }
        ifs.close();
        if (ifs.is_open()) {
            std::cout << "error closing file test_rack_file.txt" << std::endl;

        }
    }

    double elapsed_secs;
    std::string startingSquare;

    /**
     * generate 7 letter string from the 100 letters
     * @return string
     */
    std::string generateRack() {

        std::random_device rd;
        std::mt19937 mt(rd());
        // 0-26 is A-Z and BLANK
        std::uniform_int_distribution<int> dist(0, 26);

        std::string rack;
        std::map<char, int> picks;
        std::map<char, int>::iterator it;
        int rn = 0;
        char rl = '\0'; // random letter in the range of the uniform distribution

        // get all the seven letters or blanks
        for (unsigned int i = 0; i < RACKSIZE; ++i) {

            // pick a letter
            rn = dist(mt);
            if (rn == 26) {
                rl = BLANK;
            } else {
                rl = static_cast<char>(rn + 65);
            }

            // check if we had already picked this letter
            it = picks.find(rl);

            // letter has been picked but exceeded what was allowed in quantity for the letter
            // therefore pick another from the random number generator
            while (it != picks.end() && (it->second + 1) > this->alpha.find(rl)->second.quantity) {

                //std::cout << "got to pick again, old value .." << it->first << " position " << i << " q : " << it->second << std::endl;
                // pick a letter
                rn = dist(mt);
                if (rn == 26) {
                    rl = BLANK;
                } else {
                    rl = static_cast<char>(rn + 65);
                }
                it = picks.find(rl);

            }

            if (it == picks.end()) {

                // note that this letter has been picked 1 time.
                picks.emplace(std::make_pair(rl, 1));

            } else {

                // keep track of the times the letter was picked up
                it->second++;
            }

            // the actual letter that is picked to the rack
            rack.insert(i, 1, rl);

        }

        return rack;

    }

    /**
     * get word with the heighest point
     * from the Rack that meet with sowpods
     *
     * produce permutations choose k from N until k = N
     * Sigma[k=2 to N ] N!/ ( N-k)!
     *
     * @param rack
     */
    void chooseWordFromRack(std::string rack) {

        std::clock_t begin = clock();

        // make sure its first sorted
        // http://en.cppreference.com/w/cpp/algorithm/sort
        // O(nlogn)
        std::sort(rack.begin(),rack.end());

        std::string s;
        unsigned long pos = 0;

        for ( unsigned int k = 2 ; k <= rack.size(); ++k ) {

            do {
                for ( unsigned int i = 0 ; i < k; ++i ) {
                    s.insert(i,1,rack[i]) ;
                }
                //std::cout << " string " << s << std::endl;

                // s is now a candidate
                pos = s.find(BLANK);
                if ( pos < s.size() ) {
                    while ( pos < s.size() ) {
                        // s contains blanks
                        // find returns the position

                        // try all the alphabets for BLANK
                        for ( auto it = this->alpha.begin(); it != this->alpha.end(); ++it) {

                            // it is a sorted map.. should be the last key
                            if ( it->first == BLANK ) {
                                break;
                            }

                            // make sure we dont pick more letters than allowed
                            if ( std::count(s.begin(), s.end(),it->first) + 1 <= it->second.quantity ) {
                                s.replace(pos, 1, 1, it->first);
                            } else {
                                continue;
                            }

                            // check if s is in sowpods
                            auto search = this->sowpods.find(s);
                            if ( search != this->sowpods.end() ) {

                                // calculate points except for the blanks, thats pos
                                int points = 0;
                                for ( unsigned long p = 0 ; p < s.size(); ++p ) {
                                    if ( p != pos ) {
                                        points += this->alpha.find(s[p])->second.points;
                                    }
                                }
                                // add to ordered map where the key is the points, and later pick the last key
                                // that would be the best word
                                this->bestWords.emplace(std::make_pair(points,s));
                            }

                        }


                        pos = s.find(BLANK);
                    }
                } else {

                    // check if s is in sowpods
                    auto search = this->sowpods.find(s);
                    if ( search != this->sowpods.end() ) {

                        // calculate points
                        int points = 0;
                        for (unsigned long p = 0; p < s.size(); ++p) {
                            points += this->alpha.find(s[p])->second.points;
                        }
                        // add to ordered map where the key is the points, and later pick the last key
                        // that would be the best word
                        this->bestWords.emplace(std::make_pair(points, s));
                    }
                }
                // done with this permutation
                s.clear();


                // meet half way N/2
                std::reverse( rack.begin() + k ,rack.end() );

                // O(N/2) per output
            } while(std::next_permutation(rack.begin(), rack.end()));

        }

        clock_t end = clock();
        this->elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    }


public :

    //double word
    // for this requirement we are only figuring out the very first word
    // so just the fact b[7][7] needs to be identified that word is doubled.
    const int dwRow = 7;
    const int dwCol = 7;

    // individual alphabet points
    const std::map<char, alphaData> alpha = {
            {'A',   {1,  9}},
            {'B',   {3,  2}},
            {'C',   {3,  2}},
            {'D',   {2,  4}},
            {'E',   {1,  12}},
            {'F',   {4,  2}},
            {'G',   {2,  3}},
            {'H',   {4,  2}},
            {'I',   {1,  9}},
            {'J',   {8,  1}},
            {'K',   {5,  1}},
            {'L',   {1,  4}},
            {'M',   {3,  2}},
            {'N',   {1,  6}},
            {'O',   {1,  8}},
            {'P',   {3,  2}},
            {'Q',   {10, 1}},
            {'R',   {1,  6}},
            {'S',   {1,  4}},
            {'T',   {1,  6}},
            {'U',   {1,  4}},
            {'V',   {4,  2}},
            {'W',   {4,  2}},
            {'X',   {8,  1}},
            {'Y',   {4,  2}},
            {'Z',   {10, 1}},
            {BLANK, {0,  2}}
    };


    Board() {

        // load the sowpods into an ordered redblack tree std::set
        this->sowpodsLoaded = this->loadSowpods();

        // initialize the board
        for (int i = 0; i < LEN; ++i) {
            for (int j = 0; j < LEN; ++j) {
                this->b[i][j] = '\0';
            }
        }

        this->loadTestCases();

    }

    ~Board() {

        this->sowpods.clear();
        this->tests.clear();
        this->bestWords.clear();
    }

    /**
     * get stats
     */

    void stats() {

        // this is from the bag
        //this->rack = this->generateRack();

        // this is from the test cases
        for ( auto it = this->tests.begin(); it != this->tests.end(); ++it) {

            this->rack = *(it);
            this->chooseWordFromRack(this->rack);

            // figure out where to put it on the board
            std::string w = this->bestWords.rbegin()->second;
            int totalPoints = 0;

            if ( w.size() < 5 ) {

                for ( unsigned long i = 0 ; i < w.size(); ++i)
                    this->b[this->dwRow][this->dwCol+i] = w[i];

                this->startingSquare = std::to_string(this->dwRow) + "," + std::to_string(this->dwCol);

            } else if ( w.size() == 5 ){

                if ( this->alpha.find(w[0])->second.points >= this->alpha.find(w[4])->second.points ) {

                    totalPoints = this->alpha.find(w[0])->second.points * 2;
                    for ( unsigned long i = 0 ; i < w.size(); ++i)
                        this->b[this->dwRow][3+i] = w[i];

                    this->startingSquare = std::to_string(this->dwRow) + "," + std::to_string(3);

                } else {

                    totalPoints = this->alpha.find(w[4])->second.points * 2;
                    for ( unsigned long i = 0 ; i < w.size(); ++i)
                        this->b[this->dwRow][this->dwCol+i] = w[i];

                    this->startingSquare = std::to_string(this->dwRow) + "," + std::to_string(this->dwCol);
                }

            } else {

                totalPoints = this->alpha.find(w[4])->second.points * 2;
                for ( unsigned long i = 0 ; i < w.size(); ++i)
                    this->b[this->dwRow][this->dwCol+i] = w[i];

                this->startingSquare = std::to_string(this->dwRow) + "," + std::to_string(this->dwCol);

            }
            totalPoints += this->bestWords.rbegin()->first * 2;


            std::cout << "RACK:" << this->rack << std::endl ;
            std::cout << "PLACE WORD " << w << " " << this->startingSquare << " "<< totalPoints << std::endl;
            std::cout << "ELAPSED TIME:" << this->elapsed_secs << " seconds" << std::endl;


            // from the bag
            //std::cout << "PLACE WORD " << this->bestWords.rbegin()->second << " Points " << this->bestWords.rbegin()->first << std::endl;


            this->bestWords.clear();
            totalPoints = 0;
            this->startingSquare.clear();
            w.clear();
        }





    }

};

int main() {

    Board* s = new Board();

    s->stats();








    return 0;
}

