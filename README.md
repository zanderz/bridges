# bridges

A C++ program that simulates a team hiking through a forest at night.

bridges1.yaml illustrates the operation of this program using the following inputs: the hikers cross 3
bridges, at the first bridge (100 ft long) 4 hikers cross (hiker A can cross at 100 ft/minute, B
at 50 ft/minute, C at 20 ft/minute, and D at 10 ft/minute), at the second bridge (250 ft long)
an additional hiker crosses with the team (E at 2.5 ft/minute), and finally at the last bridge
(150 ft long) two hikers are encountered (F at 25 ft/minute and G at 15 ft/minute).

The answer: 245 minutes

## Strategy
A type of recursive dynamic programming backtracking DFS algorithm.
At each bridge, all combinations of hiker pairs are considered for moving forward.
For each try, the 2 candidates are moved to the next bridge, then that bridge is recursively
traversed. After that, each hiker is chosen to carry the flashlight back, and the recursive
attempt is made again.

For each configuration of hikers and bridges, the time taken to reach it is recorded in a map
keyed by a hash of the current configuration of all hikers. If the configuration was already
encountered with a faster time, the current attempt is aborted. This also prevents hikers from wandering around in circles.

Note this means hikers are free to leave their group behind; it could be an advantage, for example, for a very fast hiker to backtrack to the start. A simpler solution that crosses only one bridge at a time would be easier to implement but would likely not yield an optimal result.

## Assumptions
* Flashlight is always carried back alone; it is assumed it will never be faster for 2 to go back
* Flashlight is always carried forward in pairs; it is assumed it will never be faster for 1 to go forward alone
* Error checking and edge cases are not accounted for
  * Assume no 2 hikers have exactly the same speed (could easily be allowed)
  * Assume an input file is given by name
  * Assume a very basic subset of yaml - it isn't real yaml parsing
  * Assume at least 2 hikers, etc.

## Testing
Development and testing using the very similar scenario outlined in
https://en.wikipedia.org/wiki/Bridge_and_torch_problem
yields the same result (15 minutes), provided here in bridges2.yaml

## Room for improvement
* If there is a more elegant, less brute-force formula for solving, I didn't think of it
* For very large groups of hikers and bridges, hashing the whole thing at each step may prove prohibitive. A merkle tree could yield improvements.
* Memory use is minimal except for the stack variables in the recursive calls and the map of configuration hashes. Copying the hiker configuration instead of hashing it may be faster but use more memory.
* Built and run on Windows. Should be fine on other platforms but not tested.
