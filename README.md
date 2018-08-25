Consider the following optimization problem. We are given vertices V<sub>1</sub>, ..., V<sub>n</sub> and possible parent sets for each vertex. Each parent set has an associated cost. Let the O be an ordering (a permutation) of the vertices. we say that a parent set of a vertex V<sub>i</sub> is consistent with an ordering O if all of the parents come before the vertex in the ordering. Let mc(V<sub>i</sub>, O) be the minimum cost over all of the parent sets of vertex V<sub>i</sub> that are consistent with ordering O. The task is to find an ordering O that minimizes the total cost of : mc(V<sub>1</sub>, O) + … + mc(V<sub>n</sub>, O).
The problem is NP Complete and so presumably intractable. The local search algorithm provided in this repo attempts to find a good ordering of the variables for each of the data sets.


How To Run Code:
1. place data files in the same folder as main.cpp
2. compile by typing make in the same folder as main.cpp in terminal
3. to run, type: ./run
4. The program has the option of running a specified amount of time or run a
default of time. It will return the best found answer in the given time frame.
To enter a time limit, type "y" or "Y" and then enter the time limit in minutes.
Otherwise, input anything else and the default time limit is 60 mins.
5. Now you will be asked what data set you would like to use for the local search.
Enter the name (example: "data1.txt").
6. The program will run for the specified time on the data set.
7. Every time it discovers an ordering that is better than the best ordering thus far,
it will print the list out and its associated cost.
8. Finally, when the time limit is up, it will print the best ordering it has discoverd.
