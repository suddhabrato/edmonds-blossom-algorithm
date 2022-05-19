
# Implementation of Edmonds' Blossom Algorithm
## Overview
The *blossom algorithm* is an algorithm in graph theory for constructing maximum matchings on graphs. 
The algorithm was developed by *Jack Edmonds* in 1961, and published in 1965. 
Given a general graph G = (V, E), the algorithm finds a matching M such that each vertex in V is incident with at most one edge in M and |M| is maximized. 
The matching is constructed by iteratively improving an initial empty matching along augmenting paths in the graph.
Unlike bipartite matching, the key new idea is that an odd-length cycle in the graph (blossom) is contracted to a single vertex, with the search continuing iteratively in the contracted graph.

## Our objective
Through this project, we aim to implement the blossom algorithm and visualize the steps leading to the maximum matching by using a graphical interface. The user creates the graph on screen by virtue of mouse inputs and obtains the maximum matching for the given input graph. The interface also allows the user to control the steps which show the implementation of the algorithm. 
## Prerequisites
The environment in which the code is to be executed, must have the **MinGW (gcc) compiler** installed and the **OpenGL API** set up. [*Here*](https://medium.com/@bhargav.chippada/how-to-setup-opengl-on-mingw-w64-in-windows-10-64-bits-b77f350cea7e) is a comprehensive tutorial on how to do the same in the Windows environment. 
## How to use the code

Compile the C++ file with the following headers in the terminal

```bash
  g++ graph.cpp -lGL -lGLU -lglut -o x.out
```

Execute the compiled binary file

```bash
  ./x.out
```

In the window that pops up, create a graph by placing vertices with left-mouse clicks. We draw edges between vertices by clicking on one vertex of the edge and connecting it with the other vertex as shown in the demo below. Once the graph has been formed, right-click the mouse to terminate the input stage and initiate the Blossom algorithm.

![](https://github.com/not-so-frank-sinatra/edmonds-blossom-algorithm/blob/main/Inputs.gif)

Now head over to the terminal and press the *Enter* key to see how the algorithm proceeds step-by-step till the maximum matching is obtained. Once we obtain the maximum matching, the program shall ask us to press the *Enter* key thrice to terminate the program.

![](https://github.com/not-so-frank-sinatra/edmonds-blossom-algorithm/blob/main/Matching.gif)

## Understanding the symbols
The graph represents matched edges in 🔴 *red* color and unmatched edges in ⚫ *black* color. The table below lists the various colors used with the vertices to denote actions:

| Color             | Description                                                        |
| ----------------- | ------------------------------------------------------------------ |
|🔴| The active vertex which is being studied |
|🟢| The vertex for which we are finding the augmenting path |
|🔵| The vertex whose children are being explored |
|🟡| The vertex belonging to a blossom |
|🟠| The vertex is the father of the active vertex |
|⚪| A regular vertex which is none of the above |

**Note**: To have an in-depth understanding of how the code works line-by-line, refer to this [report](https://github.com/not-so-frank-sinatra/edmonds-blossom-algorithm/blob/main/Report_Implementation_of_Edmonds_Blossom_Algorithm.pdf) we have prepared.
## Technologies used 

**Language used:** C++

**Libraries & APIs used:** OpenGL, GLUT

## Applications of the Blossom algorithm
This polynomial time algorithm is used in several applications including the *assignment problem*, the *marriage problem*, and the *Hamiltonian cycle* and path problems (i.e., *Traveling Salesman Problem*).
## Authors

- [@suddhabratoghosh](https://github.com/not-so-frank-sinatra)

- [@bikramjitsaha](https://github.com/bikiCoder23)

- [@rahulsharma](https://www.linkedin.com/in/rahul-sharma-97ab8a18b)

## Acknowledgements

Throughout the course of this project, we were mentored by Prof. Dr. Chandan Giri. 
We are really grateful to sir for his active cooperation and support. 
Here are some resources which were really helpful to us.

 - [Blossom algorithm](https://en.wikipedia.org/wiki/Blossom_algorithm)
 - [Edmonds's Blossom Algorithm - Maximum matchings in general graphs](https://algorithms.discrete.ma.tum.de/graph-algorithms/matchings-blossom-algorithm/index_en.html)
 - [CS494 Lecture Notes - Edmonds' General Matching Algorithm](http://web.eecs.utk.edu/~jplank/plank/classes/cs494/494/notes/Edmonds/index.html)

