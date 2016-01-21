/**************************************************************
 * *  Filename: enumeration.cpp
 * *  Coded by: Dustin Dinh, Benjamin Fisher, Christina Nock
 * *  Purpose:  To run the brute force algorithm for the 
 * *			max subarray problem.
 * *
 * ***************************************************************/
#include "enumeration.h"
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <limits>
#include <vector>
#include <fstream>
#include <stdlib.h>

/**************************************************************
 * *  Function name: bruteForceEnumeration
 * *  Parameters: integer vector as input to run algorithm on
 * *  Return value: int representing sum of	the max subarray
 * *  Description: Calculate the maximum sum subarray and keep track 
 * *  of the indices between which that sub-array is contained
 * ***************************************************************/
int bruteForceEnumeration( std::vector <int> v )
{
	int maxSum = 0;			//maximum sum subarray calculated so far
	int currentSum = 0;			//current sum of current subarray
	/* Iterate through all of the possible subarray sizes */
	for ( int subArraySize = 0; subArraySize < v.size(); subArraySize++)
	{
		/* Iterate through each subarray of size subArraySize in the list */
		for ( int i = subArraySize; i < v.size(); i++)
		{
			/* Reset sum to 0 before calculating next summation */
			currentSum = 0;
			/* Calculate next summation between new subArraySize and i values */
			for ( int j = subArraySize; j <= i; j++ )
				currentSum += v[j];
			/* If current summation is greater than previous max update max and indices */
			if ( currentSum > maxSum )
				maxSum = currentSum;
		}
	}
	return maxSum;
}

/**************************************************************
 * *  Function name: bruteForceEnumeration
 * *  Parameters: integer vector as input to run algorithm on, ints representing starting and ending index
 * *  Return value: int representing sum of	the max subarray
 * *  Description: Calculate the maximum sum subarray and keep track 
 * *  of the indices between which that sub-array is contained
 * ***************************************************************/
int bruteForceEnumeration(std::vector <int> v, int &lowInd, int &highInd)
{
	int maxSum = 0;				//maximum sum subarray calculated so far
	int currentSum = 0;			//current sum of current subarray
	int lowIndex = 0;			//set the lower index to start summing from
	int highIndex = 0;			//set the higher index to end summing from
	/* Iterate through all of the possible subarray sizes */
	for ( int subArraySize = 0; subArraySize < v.size(); subArraySize++ )
	{
		/* Iterate through each subarray of size subArraySize in the list */
		for ( int i = subArraySize; i < v.size(); i++ )
		{
			/* Reset sum to 0 before calculating next summation */
			currentSum = 0;
			/* Calculate next summation between new subArraySize and i values */
			for ( int j = subArraySize; j <= i; j++ )
			{
				currentSum += v[j];
			}
			/* If current summation is greater than previous max update max and indices */
			if ( currentSum > maxSum )
			{
				maxSum = currentSum;
				lowIndex = subArraySize;
				highIndex = i;
			}
		}
	}
	return maxSum;
}
