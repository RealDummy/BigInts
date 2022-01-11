# Big ol' Integers

## C
I was assigned to make an ADT that can add, subtract, and multiply arbitrarily large numbers. The assignment was in C.
We implemented a naive solution to this because each element in the linked list is about twice as big as it needs to be.
This is so any two elements can be multiplied together and still fit inside a uint64.
 
## C++
After completing the assignment, I was disappointed it wasn't in c++, as operator overloading is perfect for a project like this.
I also did some research into non-naive solutions to this problem. Addition and subtraction were about the same as before.
The problem was with multiplying. There are some insane mathematical ways to multiply numbers more efficiently than O(n^2), and
I didn't possess the time or know-how to implement them. I plan to return to this one day, and create C++ big int ADT.

