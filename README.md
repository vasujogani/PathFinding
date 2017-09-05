# PathFinding
This is an implementation of the RRT algorithm used for path-finding in C++. It also has a visual that demonstrates the algorithm and growth of the tree. 

# How it works?
Within a given space, it randomly picks a point which is considered a temporary goal. From that point, algorithm finds a closest neighbor from where it can take a step and reach the temporary goal and a connection is established between those two points. It repeats that same process until the destinate (end goal position) is reached.
