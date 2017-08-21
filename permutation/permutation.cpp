/**
 * Author : Samson Koshy
 * Desc : Permutatation sigma K=2 to N for N!/(N-K)! 
 *        ordered non repeating combination
 *
 */

#include <algorithm>
#include <string>
#include <iostream>

int main()
{
    int count = 0;
    std::string s = "ABCDEFGHIJKL";
    std::clock_t begin = clock();
    std::sort(s.begin(), s.end());


    for ( unsigned int k = 2 ; k <= s.size(); ++k ) {


        do {
            for ( unsigned int i = 0 ; i < k; ++i ) {
//                std::cout << s[i] ;
                ;
            }
//            std::cout << std::endl;
//            std::cout << " Before Reverse " << " k:" << k << " s:" << s <<
//                      " s.begin():" << *(s.begin()) <<
//                      " s.begin()+k:" << *(s.begin()+k);
            count++;
            std::reverse(s.begin()+k,s.end());
//            std::cout << " After Reverse " << " k:" << k << " s:" << s <<
//                      " s.begin():" << *(s.begin()) <<
//                      " s.begin()+k:" << *(s.begin()+k)
//                      << std::endl;
        } while(std::next_permutation(s.begin(), s.end()));
    }

    std::clock_t end = clock();
    double time = double(end - begin ) / CLOCKS_PER_SEC;
    std::cout << count << " Elapsed Time " << time << std::endl;

}

