#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <ctime>
#include <limits>

//Max Sum Sub-Array struct
struct MSS
{
	int sum;
	int start;
	int end;
	MSS(){sum = std::numeric_limits<int>::min(); start = 0; end = 0;}
} result;

/*
	Function will find the sub array with the max sum in the array arg. 
	Brute force, time complexity of n^3
	Iterate over all of the ones sub arrays and find the max ones subarray, 
	then iterate over all the subarrays of size two and find the maxSum, 
	repeat until there are no more subarrays to check of size <= to the size of the array
*/
void bruteMaxSumSub(int array[], MSS &result, int size){	
	result.sum = std::numeric_limits<int>::min();
	result.start = 0;
	result.end = 0;
	int sumOfCur = 0;
	int start = 0;
	int end = size - 1;	
	//iterate through all of the possible subarray sizes
	for ( int subArraySize = 0; subArraySize <= end; ++subArraySize){		
		start = 0;
		//iterate through each subarray of size subArraySize in the list
		while(start <= end){			
			if((start + subArraySize) <= end)	{		
				sumOfCur = 0;
				//x
				//calculate the sum of individual subarrays
				for( int x = 0; x <= subArraySize; ++x   ){					
					sumOfCur += array[start + x];	
				}	
				if(sumOfCur  > result.sum){	
					result.sum = sumOfCur;
					result.end = start + subArraySize;
					result.start = start;
				}
			}
			++start;		
		}		
	}
}

/*
Function will find the sub array with the max sum in the array arg.
Improved brute force, time complexity of n^2
Start at lowest index and then find the max sub array of all subarrays starting from lowest to the end. 
Repeat with the next index until the end of the array is reached
ie: MSS of size lowest, then MSS of size lowest + 1, then MMS of size lowest +2, …. , MSS of sizelowest + end.

*/
void betterBruteMaxSum(int array[],  MSS &result, int size){
	result.sum = std::numeric_limits<int>::min();
	result.start = 0;
	result.end = 0;
	int sumOfCur = 0;
	int start = 0;
	int end = size - 1;	
	//start at index 0 and iterate through until the end
	for ( int startIndex= 0;  startIndex <= end; ++startIndex){
		sumOfCur = 0;
		//each iteration will increase the sub array by one
		for(int subArraySize = 0; subArraySize <= size; ++subArraySize){			
			if((startIndex + subArraySize <= end)){
				sumOfCur += array[startIndex + subArraySize];
				if(sumOfCur  > result.sum){		
					result.sum = sumOfCur;
					result.end = startIndex + subArraySize;
					result.start = startIndex;
				}
			}
		}						
	}
}

/*
Function will find the sub array with the max sum.
Use divide and conquer approach.  Divide array into halves until a subarray of one is reached. 
Then calculate maxSum for three cases. Case 1: maxSum is in leftSubArray.  Case 2: maxSum is int rightSubArray.  
Case 3: maxSum is a subarray that spans the left and right subarrays.
Base case the subarray maxSum equals the value of the index.
*/
MSS* divideAndConquerMaxSub(int array[], int start, int end){
	MSS *result = NULL;
	MSS *leftMSS = NULL;
	MSS *rightMSS = NULL;

	int mid = (end + start)/2;

	//base case
	if(start >= end){
		result = new MSS;
		result->sum = array[start]; result->start = result->end = end;
	}else{
		leftMSS  = divideAndConquerMaxSub( array, start, mid);
		rightMSS = divideAndConquerMaxSub( array, mid+1, end);
	}	

	//find span on way up tree and find the max sum sub array by comparing
	// max of span, left and right max sums
	if(result == NULL){
					
		int leftSpan = std::numeric_limits<int>::min();
		int rightSpan = std::numeric_limits<int>::min();
		int tempSum = 0;
		int spanStart = 0;
		int spanEnd = 0;
		//calculate the left span
		for(int x = mid; x >= start; --x){
			tempSum += array[x];
			if(leftSpan < tempSum){spanStart = x;  leftSpan = tempSum;}
		}
		
		//calculate the right span
		tempSum = 0;
		for(int x = mid+1; x <= end; ++x){
			tempSum += array[x];
			if(rightSpan < tempSum){spanEnd=x; rightSpan = tempSum;}
		}
		//determine which is the largest sum: left, right or span
		if ((leftSpan + rightSpan) > leftMSS->sum && (leftSpan + rightSpan) > rightMSS->sum){			
			leftMSS->sum = leftSpan + rightSpan;
			leftMSS->start = spanStart; 
			leftMSS->end = spanEnd;
			result = leftMSS;
			delete rightMSS;
			rightMSS = 0;
		}else if(leftMSS->sum > rightMSS->sum){
			delete rightMSS;
			rightMSS = 0;
			result = leftMSS;				
		}else{
			delete leftMSS;
			leftMSS = 0;
			result = rightMSS;
		}
	}

	return result;
}

/*
Function will find the sub array with the max sum.
Iterate over the array once, keeping track of two sumSubArrays.  
The first sumSubArray will store the current sum of the subarray found during the search.  
If the sum of this subarray drops below zero then it will reset its self on the next index.  
The second sum will store the maximum sum subarray found so far and will only be updated 
if a higher value is found using the first sumSubArray.
*/
void linearMaxSub(int array[], int size,  MSS &result){
	MSS sumCur;

	for (int x = 0; x < size; ++x){
		//current sum is neg so discard the sub array and start again
		if(sumCur.sum <= 0){
			sumCur.sum = array[x];
			sumCur.start = x;
			sumCur.end = x;
		}else{
			//current sum is positive so keep adding elements
			sumCur.sum += array[x];
			sumCur.end = x;
		}
		//check is the result is less than the cur if so update result
		if(result.sum < sumCur.sum)	{
			result.sum = sumCur.sum;
			result.start = sumCur.start;
			result.end = sumCur.end;
		}			
	}
}

int * createArray(int size){
	int *result = new int[size];

	int isNeg = 0;

	for(int i = 0; i < size; ++i){
		result[i] = rand() % 100 + 1;
		isNeg = rand() % 2;

		if(isNeg){
			result[i] *= -1;
		}
	}
	return result;
}

int main(){
	MSS * myResult;

	/* initialize random seed: */
	srand (time(NULL));

	int num [] = {	5, 10, 15, -20, -30, 40, 45, -46, 5, 10, 65, -23, 20, 41, 5, -33,
					29, 19, -51, 100, -99, 13, 77, -6, 44, 17, -9, -2, 1, 31, -89, 63,
					5, 10, 15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, 41, 5, -33,
					29, 19, -51, -100, -99, 13, 77, -6, -44, 17, -9, -2, 1, 31, -89, -63,
					5, 10, 15, -20, -30, 40, -45, -46, 5, 10, 65, -23, 20, 41, 5, -33,
					-29, 19, -51, 100, -99, 13, 77, -6, 44, 17, -9, -2, 1, 31, -89, 63,
					-5, 10, -15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, 41, 5, -33,
					-29, 19, -51, -100, -99, 13, 77, -6, -44, -17, -9, -2, 1, 31, -89, -63,
					5, -10, -15, -20, -30, 40, -45, -46, 5, 10, 65, -23, 20, 41, 5, -33,
					29, 19, -51, 100, -99, 13, -77, -6, -44, 17, -9, -2, 1, 31, -89, 63,
					5, 10, 15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, 41, 5, -33,
					29, 19, -51, -100, -99, 13, 77, -6, -44, 17, -9, -2, 1, 31, -89, -63,
					5, 10, -15, -20, -30, 40, -45, -46, 5, -10, 65, -23, 20, 41, 5, -33,
					-29, 19, -51, -100, -99, 13, 77, -6, -44, 17, -9, -2, 1, 31, -89, 63,
					-5, 10, -15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, -41, 5, -33,
					-29, 19, -51, -100, -99, 13, 77, -6, -44, -17, -9, -2, 1, -31, -89, -63,
					5, 10, 15, -20, -30, 40, 45, -46, 5, 10, 65, -23, 20, -41, 5, -33,
					29, 19, -51, 100, -99, 13, 77, -6, 44, 17, -9, -2, 1, -31, -89, 63,
					5, 10, 15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, -41, 5, -33,
					29, 19, -51, -100, -99, 13, 77, -6, -44, 17, -9, -2, 1, -31, -89, -63,
					5, 10, 15, -20, -30, 40, -45, -46, 5, -10, 65, -23, 20, -41, 5, -33,
					-29, 19, -51, 100, -99, 13, 77, -6, -44, 17, -9, -2, 1, -31, -89, 63,
					-5, 10, -15, -20, -30, -40, -45, -46, 5, 10, 65, -23, -20, -41, 5, -33,
					-29, -19, -51, -100, -99, 13, -77, -6, -44, -17, -9, -2, 1, -31, -89, -63,
					5, -10, -15, -20, -30, 40, -45, -46, 5, 10, 65, -23, 20, -41, 5, -33,
					29, 19, -51, 100, -99, 13, -77, -6, -44, 17, -9, -2, 1, -31, -89, 63,
					5, 10, 15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, -41, 5, -33,
					29, 19, -51, -100, -99, 13, -77, -6, -44, 17, -9, -2, 1, -31, -89, -63,
					5, 10, -15, -20, -30, 40, -45, -46, 5, -10, 65, -23, 20, -41, 5, -33,
					-29, -19, -51, -100, -99, 13, -77, -6, -44, 17, -9, -2, 1, -31, -89, 63,
					-5, -10, -15, -20, -30, -40, 45, -46, 5, 10, 65, -23, -20, -41, 5, -33,
					-29, -19, -51, -100, -99, 13, -77, -6, -44, -17, -9, -2, 1, -31, -89, -63};
	int sizeOfNum = sizeof(num)/sizeof(num[0]);
	    
	printf("Max Sum Sub-Array Algorithms\n\n");
	printf("size = %d\n\n", sizeOfNum);
	std::clock_t    start;
    
	printf("\n\nBrute Force Max Sum Sub-Array\n\n");
	
	start = std::clock();
	bruteMaxSumSub(num, result, sizeOfNum);
	std::cout << "Brute Force Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of bruteMaxSumSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	printf("\n\nImproved Brute Force Max Sum Sub-Array\n\n");

	start = std::clock();
	betterBruteMaxSum(num, result, sizeOfNum);
	std::cout << "Better Brute Force Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of betterBruteMaxSumSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	printf("\n\nDivide and Conquer Max Sum Sub-Array\n\n");
		
	start = std::clock();
	myResult = divideAndConquerMaxSub(num, 0, sizeOfNum - 1);
	std::cout << "Divide and Conquer Max Sum Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of divideAndConquerMaxSub, max sum = %d\nSubArray*************\n", myResult->sum);
	for(int i = myResult->start; i <= myResult->end; ++i){
		printf("%d\t", num[i]);
	}

	printf("\n\nLinear Max Sum Sub-Array\n\n");
		
	start = std::clock();
	linearMaxSub(num, sizeOfNum, result);
	std::cout << "Linear Max Sum Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms\n";
	printf("Result of linearMaxSub, max sum = %d\nSubArray*************\n", result.sum);
	for(int i = result.start; i <= result.end; ++i){
		printf("%d\t", num[i]);
	}

	std::cout << std::endl << std::endl;
	return 0;
}