#Big ol' Inegers
I was assigned to make an ADT that can add, subtract, and multiply arbitrarily large numbers. The assignment was in C.
We implimented a naive solution to this because each element in the linked list is about twice as big as it needs to be.
This is so any two elements can be multiplied together and still fit inside a uint64.

After completing the assignment, I was dissapointed it wasn't in c++, as operator overloading is perfect for a project like this.
I also did some research into non-naive solutions to this problem. Additio and subtraction were about the same as before.
The problem was with multiplying. There are some insane mathamatical ways to multiply numbers more efficently than O(n^2), and
I didn't at the time posses the time or know-how to impliment them. I plan to return to this one day, and create C++ big int ADT.