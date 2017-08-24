README
Conway's Game of Life

I struggled a bit to relearn C at the beginning of this assignment. I tried a
few different ways of solving this problem, none working until I took the simplest
approach of all the attempts I made.

I had around 268 lines of code after the second to last attempt to solve this
problem. I basically hard coded everything and it still didn't work. I thought
of a really basic solution after trying to debug my massive count function.
The basic solution was the one that worked.

My main method is at the top of the file, at line 14. My count function begins
at line 42. Count is what counts up all of the neighboring cells.
My rules function begins at line 56. It determines whether a cell lives or dies.
My adapt function begins at line 72. Adapt deals with the edge cases and is put
within the brackets of my world[][] array.
