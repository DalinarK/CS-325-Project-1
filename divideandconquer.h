#include <vector>		//we are using vector as input
#include <algorithm>	//for max function
#include <limits.h>		//for INT_MIN representing negative infinity

struct maximumSubarray
{
	int currentSum;
	int lowIndex;
	int highIndex;
};

int executeDivideAndConquer( std::vector <int> v );
int executeDivideAndConquer( std::vector <int> v, int &lowIndex, int &highIndex );
int findMaximumSubarray( std::vector <int> v, int lowIndex, int highIndex );
maximumSubarray findMaximumSubarray2(std::vector <int> v, int lowIndex, int highIndex );
int findMaximumCrossingSubarray( std::vector<int> v, int lowIndex, int highIndex, int midIndex );
maximumSubarray findMaximumCrossingSubarray2(std::vector<int> vec, int lowIndex, int highIndex, int midIndex );
