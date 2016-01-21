/**************************************************************
 * *  Filename: betterEnumeration.cpp
 * *  Coded by: Dustin Dinh, Benjamin Fisher, Christina Nock
 * *  Purpose:  To run a better brute force enumeration algorithm for the 
 * *			max subarray problem.
 * *
 * ***************************************************************/

#include "betterEnumeration.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

/**************************************************************
 * *  Function name: bruteForceEnumeration
 * *  Parameters: integer vector as input to run algorithm on
 * *  Return value: int representing sum of	the max subarray
 * *  Description: Function will find the sub array with the max sum 
 * *  in the array arg
 * ***************************************************************/
int betterBruteMaxSum( std::vector <int> v )
{
	int maxSum = 0;				//maximum sum subarray calculated so far
	int currentSum = 0;			//current sum of current subarray
	/* Start at index 0 and iterate through until the end */
	for ( int startIndex = 0; startIndex < v.size(); startIndex++ )
	{
		/* Reset sum to 0 before calculating next summation */
		currentSum = 0;
		/* Each iteration will increase the sub array by one */
		for ( int i = startIndex; i < v.size(); i++ )
		{
			/* Calculate next summation between new startIndex and i values */
			currentSum += v[i];
			/* If current summation is greater than previous max update max and indices */
			if ( currentSum > maxSum )
				maxSum = currentSum;
		}
	}
	return maxSum;
}

/**************************************************************
 * *  Function name: bruteForceEnumeration
 * *  Parameters: integer vector as input to run algorithm on
 * *  Return value: int representing sum of	the max subarray
 * *  Description: Function will find the sub array with the max sum 
 * *  in the array arg
 * ***************************************************************/
int betterBruteMaxSum( std::vector <int> v, int &lowInd, int &highInd )
{
	int maxSum = 0;				//maximum sum subarray calculated so far
	int currentSum = 0;			//current sum of current subarray
	lowInd = 0;			//set the lower index to start summing from
	highInd = 0;			//set the higher index to end summing from
	/* Start at index 0 and iterate through until the end */
	for ( int startIndex = 0; startIndex < v.size(); startIndex++ )
	{
		/* Reset sum to 0 before calculating next summation */
		currentSum = 0;
		/* Each iteration will increase the sub array by one */
		for ( int i = startIndex; i < v.size(); i++ )
		{
			/* Calculate next summation between new startIndex and i values */
			currentSum += v[i];
			/* If current summation is greater than previous max update max and indices */
			if ( currentSum > maxSum )
			{
				maxSum = currentSum;
				lowInd = startIndex;
				highInd = i;
			}
		}
	}
	return maxSum;
}
