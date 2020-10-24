import random

n = [random.randrange(1, 100000) for i in range(10000)] # list Time O(n)
t = 11

def twoSum_O_n():
    c={} # key value store | Dictionary in python | Time O(1) Space O(n) where n is the number of keys
    import timeit
    start_time = timeit.default_timer()  
    for s in n:   # read Time O(n) Space O(n) 
        # check if the complement of s(item) is in c{}
        if s in c: # read O(1) happening n times 
                        # s(item from n) + x( i dont know ) = t(arget)
                        # example : 20 + x = 10 --> x = 10 - 20 --> -10
                        # example : 10 + x = 50 --> x = 50 + (-10)
                        #
            # O(1)             
            print(s, c[s], # reads the key (t-s) c[2] --> 7
                    )   # item in the list n
        else:
            c[t-s] = s # writes O(1) in time and Space 2 -> 7 | 7 --> 2 
            # print ("reached else ", (t-s),c[(t-s)])
    print("timeSum_O_n() for 10000 N takes ",timeit.default_timer() - start_time)


def twoSum_O_nsquare():
    import timeit
    start_time = timeit.default_timer()  
    for i in n:   # read Time O(n) Space O(n)  
        for j in n: # read Time O(n) Space O(n)  
            if i + j == t:
                print ( i,j )

    print("timeSum_O_nsquare() for 10000 N takes ",timeit.default_timer() - start_time)



twoSum_O_n()
twoSum_O_nsquare()
