/**************************************************************
 * *  Filename: divideandconquer.cpp
 * *  Coded by: Benjamin Fisher, Dustin Dinh, Christina Nock
 * *  Purpose:  To run the divide and conquer algorithm for the 
 * *			max subarray problem.
 * *
 * ***************************************************************/
#include "divideandconquer.h"

/**************************************************************
 * *  Function name: executeDivideAndConquer
 * *  Parameters: integer vector as input to run algorithm on
 * *  Return value: int representing sum of	the values in the subarray A[low..high]
 * *  Description: This function executes the divide and conquer implementation of find maximum subarray 
 * ***************************************************************/
int executeDivideAndConquer( std::vector <int> v )
{
	int lowIndex = 0;								//set the lower index to start dividing from 
	int highIndex = v.size() - 1; 					//set the higher index to end dividing from
	return findMaximumSubarray( v, lowIndex, highIndex );		
}

/**************************************************************
 * *  Function name: executeDivideAndConquer
 * *  Parameters: integer vector as input to run algorithm on
 * *  Return value: struct representing the maximum subarray in A[low..high]
 * *  Description: This function executes the divide and conquer implementation of find maximum subarray 
 * ***************************************************************/
int executeDivideAndConquer( std::vector <int> v, int &lowIndex, int &highIndex )
{
	maximumSubarray m;
	lowIndex = 0;									//set the lower index to start dividing from 
	highIndex = v.size() - 1;						//set the higher index to end dividing from
	m = findMaximumSubarray2( v, lowIndex, highIndex );
	lowIndex = m.lowIndex;							//set the low index of the max subarray
	highIndex = m.highIndex;						//set the high index of the max subarray
	return m.currentSum;							//set the highest sum so far
}

/**************************************************************
 * *  Function name: findMaximumSubarray
 * *  Parameters: integer vector as input to run algorithm on, integers representing the lower index to start dividing from and the higher index to stop dividing
 * *  Return value: struct representing the maximum subarray with the largest sum
 * *  Description: This function checks for the base case in which case it returns the single element and for the recursive case where divide and conquer is used to
 * *  divide the subarray into equal parts and recursively find the maxes, eventually comparing the maxes with each other to return the subarray with the largest sum
 * ***************************************************************/
int findMaximumSubarray( std::vector <int> v, int lowIndex, int highIndex )
{
	/****************************************************************
	* * The base case occurs when there is only one element in the input, and then the single element is returned
	*****************************************************************/
	if ( lowIndex == highIndex  )
	{
		return v[lowIndex];
	}
	/****************************************************************
	* * Any contiguous subarray A[i..j] must lie in exactly one of the following places, from pg. 70 of Cormen's Algorithms:
	*****************************************************************/
	int midIndex = ( lowIndex + highIndex ) / 2;
	/****************************************************************
	* * Take the subarray with the largest sum of the above three from pg. 70 of Cormen's Algorithms:
	*****************************************************************/
	return std::max(std::max(findMaximumSubarray( v, lowIndex, midIndex ), findMaximumSubarray( v, midIndex + 1, highIndex )), findMaximumCrossingSubarray( v, lowIndex, highIndex, midIndex ));

}

/**************************************************************
 * *  Function name: findMaximumSubarray
 * *  Parameters: integer vector as input to run algorithm on, integers representing the lower index to start dividing from and the higher index to stop dividing
 * *  Return value: struct representing the maximum subarray with the largest sum
 * *  Description: This function checks for the base case in which case it returns the single element and for the recursive case where divide and conquer is used to
 * *  divide the subarray into equal parts and recursively find the maxes, eventually comparing the maxes with each other to return the subarray with the largest sum
 * ***************************************************************/
maximumSubarray findMaximumSubarray2(std::vector <int> v, int lowIndex, int highIndex )
{
	maximumSubarray m;
	/****************************************************************
	* * The base case occurs when there is only one element in the input, and then the single element is returned
	*****************************************************************/
	if ( lowIndex == highIndex )
	{
		//If so, set values of high low indexes and max
		m.lowIndex = lowIndex;
		m.highIndex = highIndex;
		m.currentSum = v[lowIndex];
		return m;
	}
	/****************************************************************
	* * Recursive case is divide and conquer where we divide the subarray into two subarrays of as equal size as possible from pg. 70 of Cormen's Algorithms
	*****************************************************************/
	int midIndex = ( lowIndex + highIndex ) / 2;
	/****************************************************************
	* * Any contiguous subarray A[i..j] must lie in exactly one of the following places, from pg. 70 of Cormen's Algorithms:
	*****************************************************************/
	/* Entirely in the subarray A[low..mid] so that low <= i <= j <= mid from pg. 70 of Cormen's Algorithms */
	maximumSubarray leftSubarray;
	leftSubarray = findMaximumSubarray2( v, lowIndex, midIndex );
	/* Entirely in the subarray A[mid + 1..high] so that mid <= i <= j <= high from pg. 70 of Cormen's Algorithms */
	maximumSubarray rightSubarray;
	rightSubarray = findMaximumSubarray2( v, midIndex + 1, highIndex );
	/* Crossing the midpoint so that low <= i <= mid <= j <= high from pg. 70 of Cormen's Algorithms */
	maximumSubarray crossingSubarray;
	crossingSubarray = findMaximumCrossingSubarray2( v, lowIndex, highIndex, midIndex );
	/****************************************************************
	* * Take the subarray with the largest sum of the above three from pg. 70 of Cormen's Algorithms:
	*****************************************************************/
	if ( leftSubarray.currentSum >= rightSubarray.currentSum && leftSubarray.currentSum >= crossingSubarray.currentSum )
	{
		return leftSubarray;
	}
	else if ( rightSubarray.currentSum >= leftSubarray.currentSum && rightSubarray.currentSum >= crossingSubarray.currentSum )
	{
		return rightSubarray;
	}
	else
	{
		return crossingSubarray;
	}

}

/**************************************************************
 * *  Function name: findMaximumCrossingSubarray
 * *  Parameters: integer vector as input to run algorithm on, integers representing the lower index to start dividing from and the higher index to stop dividing
 * *  Return value: int representing sum of	the values in the subarray A[max left index..max right index]
 * *  Description: This function takes an input vector array and indices low, high and mid and returns a tuple containing the indices demarcating a maximum subarray 
 * *  that crosses the midpoint, along with the sum of the values in a maximum subarray, from pg. 70 in Cormen's algorithm
 ***************************************************************/
int findMaximumCrossingSubarray( std::vector<int> v, int lowIndex, int highIndex, int midIndex )
{
	/****************************************************************
	* * Find maximum subarray for the left half, from A[low..mid] from pg. 72 of Cormen's Algorithms:
	*****************************************************************/
	int leftSum = INT_MIN;		//holds the greatest sum found so far on the left side
	int currentSum = 0;				//holds the sum of the entries in A[i..mid]
	/* Start from mid and go to low so that every subarray it forms is of the form A[i..mid]*/
	for ( int i = midIndex; i >= lowIndex; i-- )
	{
		/* Add up the elements from low to mid */
		currentSum += v[i];
		/* When we find a subarray A[i..mid] with a sum greater than leftSum, update leftSum with this value */
		if ( currentSum > leftSum)
		{
			leftSum = currentSum;
		}
	}
    /****************************************************************
	* * Find maximum subarray for the right half, from A[mid+1..high] from pg. 72 of Cormen's Algorithms:
	*****************************************************************/
	int rightSum = INT_MIN;
	currentSum = 0;
	/* Start from mid+1 and go to high so that every subarray it forms is of the form A[mid+1..j]*/
	for (int j = midIndex + 1; j <= highIndex; j++)
	{
		/* Add up the elements from mid+1 to high */
		currentSum += v[j];
		/* When we find a subarray A[mid+1,high] with a sum greater than rightSum, update rightSum with this value */
		if ( currentSum > rightSum )
		{
			rightSum = currentSum;
		}
	}
	/* Return indices leftSum and rightSum to demarcate a maximum subarray crossing the midpoint, along with the sum of
	the values in the subarray A[max left index..max right index] */
	return leftSum + rightSum;
}

/**************************************************************
 * *  Function name: findMaximumCrossingSubarray
 * *  Parameters: integer vector as input to run algorithm on, integers representing the lower index to start dividing from and the higher index to stop dividing
 * *  Return value: struct representing the maximum subarray with the largest sum
 * *  Description: This function takes an input vector array and indices low, high and mid and returns a tuple containing the indices demarcating a maximum subarray 
 * *  that crosses the midpoint, along with the sum of the values in a maximum subarray, from pg. 70 in Cormen's algorithm
 ***************************************************************/
maximumSubarray findMaximumCrossingSubarray2(std::vector<int> vec, int lowIndex, int highIndex, int midIndex )
{
	/****************************************************************
	* * Find maximum subarray for the left half, from A[low..mid] from pg. 72 of Cormen's Algorithms:
	*****************************************************************/
	maximumSubarray m;
	int leftSum = INT_MIN;				//holds the greatest sum found so far on the left side
	int maxLeftIndex, maxRightIndex;	//holds the indexes of subarray where largest sum is found 
	int currentSum = 0;					//holds the sum of the entries in A[mid+1, j]
	/* Start from mid+1 and go to high so that every subarray it forms is of the form A[mid+1..j]*/
	for ( int i = midIndex; i >= lowIndex; i-- )
	{
		/* Add up the elements from low to mid */
		currentSum += vec[i];
		/* When we find a subarray A[i..mid] with a sum greater than leftSum, update leftSum with this value and update value of left index of maximum subarray*/
		if ( currentSum > leftSum ) 
		{
			leftSum = currentSum;
			maxLeftIndex = i;
		}
	}
	/****************************************************************
	* * Find maximum subarray for the right half, from A[mid+1..high] from pg. 72 of Cormen's Algorithms:
	*****************************************************************/
	int rightSum = INT_MIN;				//holds the greatest sum found so far on the right side
	currentSum = 0;						//reset the current sum of right side to 0
	/* Start from mid+1 and go to high so that every subarray it forms is of the form A[mid+1..j]*/
	for ( int j = midIndex + 1; j <= highIndex; j++ )
	{
		/* Start from mid+1 and go to high so that every subarray it forms is of the form A[mid+1..j]*/
		currentSum += vec[j];
		if ( currentSum > rightSum )
		{
			rightSum = currentSum;
			maxRightIndex = j;
		}
	}
	/* Return indices leftSum and rightSum to demarcate a maximum subarray crossing the midpoint, along with the sum of
	the values in the subarray A[max left index..max right index] */
	m.currentSum = leftSum + rightSum;
	m.lowIndex = maxLeftIndex;
	m.highIndex = maxRightIndex;

	return m;

}
