# pils
Pure Integer Linear Solver

## Overview
This is a toy project to build a pure integer linear programming solver using only integer arithmetic.  The underlying algorithm is the revised lexicographic dual simplex algorithm.

Because this is an academic exercise, it's not fast, it doesn't support all problems (like unbounded ones), but it's not that complicated and it is exact.  If you find it useful, leave a note or an enhancement request (or even better, a PR).

## Building
The only dependency that the code has is a few tests that use GMP for big integer math.  If you don't have GMP installed, you can just remove the GMP test from the build script, but you'll need some kind of big integer library for all but the smallest problems.

After cloning, you should just be able to run
```
./build.sh
```
The build script uses the gcc toolchain, but if you prefer a different compiler, the build commands should be trivial to modify.
