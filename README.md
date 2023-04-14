# cop4520-assg3

## **Statments**

This repository contains two different programs, minotaurs-chain and mars-rover.

To run each program please clone the repository and follow the following instructions.

- Build the executables with the Makefile once located in the same directory as the project.

  ```
  make
  ```

- To run minotaurs-chain.cpp:

  - Use the following commands to compile and run minotaurs-chain.cpp.

    ```
    ./prog1
    ```

- To run mars-rover.cpp:

  - Use the following commands to compile and run mars-rover.cpp.

    ```
    ./prog2
    ```

Note that to compile this programs, you'll need a modern, up to date c++ compiler. The example above uses the most recent version of gcc.

Prog1 or minotaurs-chain is the program for the first half of assingment 3. It will print the linked list after the 500k gifts have been added and removed (which should be empty) and an extra linked list that Ill use to verify correctness later on.

Prog2 or mars-rover is the program for the second hald of assignment 3. It will print a report of the temperatures as requested in the assignment every hour.

\*NOTE ON PROG2: Prog 2 is currently not reporting every hour. Instead its reporting every 5 minutes and it gives the range with the one minute range with highest temp change. TO change back to an hour with with ranges of 10 mins change or any other custom settings, change the '5' in line 91 for however many minutes you want the program to take to generate a response. To change the range of time to find the highest temp difference, change '(diffInSeconds > 60 && diffInSeconds < 120)' in line 130 to however big of a gap in seconds you want.

## **Efficiency**

The efficiency of prog1 seems to be pretty good. There is no numbers to prove it but the simulation of adding, removing and looking for a gift concurrently by 4 threads finishes almost instantly. The linked list is basically a normal linked list which has a mutex to block itself while an operation is modifying the nodes. Having only one lock in the linked list could lead to traffic, but it seems to still be doing well with 500k gifts.

The efficincy for prog2 is also pretty good. Prog 2 works as a queue so pops and pushes are pretty efficient. Once again the only thing that affects the efficiency of the queue is a mutex that blocks the queue while undergoing an operation. In this case its not so bad for the queue to only have one lock as this problem as more about synchronization.

### Is there any more speed-ups that could be implmented?

For both prog1 and prog2 the way to speed them up and make them more efficient is the same. Both programs are using concurrent data structures with only one lock on them. The way to make them more effient is to either have each node on the data structures have a lock, so that there is not need to block the whole structure in an operation but only the nodes being affected, or make the functions of the data structure lock or wait free.

I attempted both ways to make these programs more efficient that I mentioned above, unfortunately although the solutison would sometimes give the right answer sometimes the programs would crash.

## \*_Proof of Correctness_

The proof of correctness in these programs are reflected in the data they print to the console.

Prog1 will print the empty linked list that results from adding and removing all 500k gifts. In additon prog1 will return another linked list, which you can check on the code, has concurrent inserts and deletes operations and the list at end is what it should be everytime.

Prog2 prints the report. To really check correctness you can print on inserts and do the calculations about the temperatures and then check against the report, however this is left as an excersice to the reader.

## **Experimental Evaluation**

Both programs were tested with different amounts of threads. In the case of prog1 I also tried smaller amounts of presents that were more testable. Prog 2 also works well with different amoutns of threads and how often reports are. Note that for prog 2 the concurrent queue will support multiple consumers, but the code in this program works with 1 consumer only.
