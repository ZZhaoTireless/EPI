#include <iostream>
#include <string>

#include <bitset>
#include <time.h>
#include <limits>
#include <math.h>

using namespace std;

/* Chapter 5 Primary Type */

/******* 5.1 Parity Problem *******/

/* The odd number of set bit returns 1, else return 0 */

/* Brute-force */

int parityBruteForce (unsigned long num){

	short parity = 0;

	while(num){
    	
		parity ^= (num & 1); 

		num >>= 1;
	}
	
	return parity;

}

/* 
Improve best and average case

x & (x - 1) -> each time eliminte the lowest set bit

x & ~(x - 1) -> each time extract the lowest set bit

lowest set bit -> first bit of 1 from most right position
*/

int parityOnlyViewSetBit(unsigned long num){

	short parity = 0;

	while (num){

		parity ^= 1;

		num &= (num - 1);
	}

	return parity;
}

/*
Improve the runtime to O(logN) with the property of XOR

long long type is 64 bit, int is 16 bit, and long is 32 bit.

XOR between the top half and the bottom half of a number keeps
the parity of 1's and 0's for the whole number.
*/

int parityDivAndConq(unsigned long long num){

	int shift = 32;

	do{
		/*Show binary form of 'num'*/
    	std::string binary = std::bitset<64>(num).to_string();
    	cout << binary << endl;


		num ^= (num >> shift);

		/* This equls to 

		num ^= num >> 32;  
			-> 0x00000000XXXXXXXX ^ 0xXXXXXXXXYYYYYYYY 
			-> 0xXXXXXXXX ^ 0xYYYYYYYY
		
		num ^= num >> 16;
		num ^= num >> 8;
		num ^= num >> 4;
		num ^= num >> 2;
		num ^= num >> 1; */

		shift >>= 1;

	} while(shift);

	return num & 1;

}

/******* 5.2 Swap Problem *******/

/* Swap the bits those are on the index i and index j */

/* Brute-force */

int swapBruteForce (unsigned long num, int i, int j){

	if (i == j) return num;

	unsigned long maskOfI = (num >> i) & 0x1;

	unsigned long maskOfJ = (num >> j) & 0x1;

    if(maskOfI) num = num | (maskOfI << j);
    else num = num & ~(0x1 << j);

    if(maskOfJ) num = num | (maskOfJ << i);
    else num = num & ~(0x1 << i);

	return num;	

}

/*
Improvement without save the value. 

Note: the "exclusive or" with 1 swap the bit value 
when values on i and j are different.

1 ^ 1 = 0, 0 ^ 1 = 1
*/

int swapByMask( unsigned long num, int i, int j){

	if(((num >> i) & 0x1) != ((num >> j) & 0x1)){

		unsigned int mask = (1L << i) | (1L << j);

		num ^= mask;
	}
	return num;
}


/******* 5.7 Exponentiation Problem *******/

/*
Calculate x^y where x is double y is integer

Follow the bit of y to recursively calculate.
*/

double expoRecursion(double x, int y){

	/* Consider that y could be negative number*/

	if( y < 0) {
		y = - y;
		x = 1.0 / x;
	}

	if (y && (y % 2)){
		return x * expoRecursion(x, y - 1);
	} 

	if(y && !(y % 2)){
		return expoRecursion(x, y >> 1) * expoRecursion(x, y >> 1);
	}

	return 1;

}

/*
Now consider loop version for problem
*/

double expoLoop(double x, int y){

	if(y < 0){
		y = -y;
		x = 1.0 / x;
	}

	double ans = 1.0;

	while (y){

		/* 
		if the most right is set bit then multiply
		current x value once more */
		if( y & 0x1){
			ans *= x;
		}

		/* 
		Each time y = y / (2 ^ n) ->
		x = x ^ [y  * (2 ^ n)]
		looping until y = 1
		*/
		x *= x;
		y = y >> 1;	
	}

	return ans;
}

/******* 5.8 Reverse Integer Problem *******/

/*
Reverse the input integer

Brute-force: loop with 10X for each digit
*/

int revIntBruteForce(int n){

	int numPos = 1;
	int prePos = 1;
	int ans = 0;

	bool negative = n < 0;

	n = negative ? -n : n;

	while(numPos <= n){

		ans *= 10;

		numPos *= 10;

		prePos = numPos / 10;

		ans += (n % numPos) / prePos ;
	}

	return negative ? -ans : ans;
}

/*
A better approch
*/

int revIntImprove(int n){

	int ans = 0;

	bool negative = n < 0;

	n = negative ? -n : n;

	while(n){

		ans = ans * 10 + n % 10;

		n /= 10;

	}

	return negative ? -ans : ans;
}

/******* 5.3 Reverse bits Problem *******/

/*
Brute force through the way in 5.2
*/

unsigned long revBits(unsigned long num){

	int len = (sizeof num) * 4;

	cout << len << endl;

	for(int i = 0; i < len / 2; i++){

		if (((num >> i) & 0x1) != ((num >> (len - i - 1)) & 0x1)){

			unsigned long mask = (1L << i) | (1L << (len - i - 1));

			num ^= mask;
		}	
	}

	return num;
}

/*

Another better way, similar as problem 5.2, we could use
a table to save the reverse like 16 bits as the key, and
the responding value is it's reverse.

The running time complexity would be O(n/L) where L is 
the length of the key.

The space complexity would be O(L * 2^(L + 1)) as worst case

*/


/******* 5.9 Check Palindrome Problem *******/

/*
The rmethod in 5.8 for reverse digits would be useful

It may have same time complexsity of worst case as optimal
method, but the average time complexsity is not optimal
*/

int checkPalindrome(long num){

	if(num < 0) return 0;

	long rev = 0;
	long orig = num;

	while(num){

		rev = rev * 10 + num % 10;

		num /= 10;
	}

	return rev == orig ? 1 : 0;
}

/* 
There is a better way. 

When check failed, stop running immediately.

Note: the length of most significant digit would
be given by floor(log10(X)) + 1.
*/

int checkPalindromeImprove(long num){

	if(num < 0) return 0;

	int maskLength = floor(log10(num)) + 1;

	int numMask = pow(10, maskLength - 1);

	while(num){

		int tail = num % 10;

		int head =  num / numMask;

		if(tail != head) return 0;

		/* Remove head and tail for parameter */
		num %= numMask;
		num /= 10;

		numMask /= 100;
	}

	return 1;

} 

/******* 5.11 Rectangle Intersection Problem *******/

/*
If the rectrangle is formed by lowest width and height,
and we has width and height as well. We have O(1) time to check
if two rectangle has intersection and what the rectangle that 
such intersection formed.
*/

typedef struct rec {
	int x, y, width, height;

	rec(int x, int y, int width, int height): x(x),y(y),width(width),height(height){
		/*cout << "rectangle created" << endl;*/
	}
} Rectangle;

int isIntersection(Rectangle & R1, Rectangle & R2){
	return R1.x <= R2.x + R2.x + R2.width &&
		   R1.y <= R2.y + R2.y + R2.height &&
		   R2.x <= R1.x + R1.x + R1.width &&
		   R2.y <= R2.y + R2.y + R2.height;
}

Rectangle intersectionRec(Rectangle &R1, Rectangle &R2){

	if(isIntersection(R1, R2)){
		Rectangle result = Rectangle(
			max(R1.x, R2.x),
			max(R1.y, R2.y),
			min(R1.x + R1.width, R2.x + R2.width) - max(R1.x,R2.x),
			min(R1.y + R1.height, R2.y + R2.height) - max(R1.y,R2.y)
		);

		return result;
	}

	return Rectangle(0,0,-1,-1);
}

void unitTestForRecItersec(void){

	Rectangle R1 = Rectangle(0,0,1,1);
	Rectangle R2 = Rectangle(1,1,2,3);
	Rectangle R3 = Rectangle(2,0,1,3);

	Rectangle res1 = intersectionRec(R1, R2); // one pix intersection
	Rectangle res2 = intersectionRec(R2, R3); // rectangle intersection
	Rectangle res3 = intersectionRec(R1, R3); // no intersection

	cout << "R1 & R2: " <<res1.x << "," << res1.y << "," << res1.width << "," << res1.height << endl;
	cout << "R2 & R3: " <<res2.x << "," << res2.y << "," << res2.width << "," << res2.height << endl;
	cout << "R1 & R3: " <<res3.x << "," << res3.y << "," << res3.width << "," << res3.height << endl;

}

/* Chapter 6 Arrays */

/*
1.	Even tough it is easy to use O(n) space for
	arry related quesiotn, it is better to solve
	it without alloctating addtional storage.
	
2.	Take advantage for the face that you can operate 
	efficiently on both ends.

3.  When operating 2D array, use parallel logic for rows
	and for columns.

*/

//**** <vector> ****
/*	Dynamic size, stored in heap.
    
*/
//******************

//**** <array> ****
// Statically-size, stored in stack.
//
//*****************

//**** <alogrithm> ****


//*********************

int main(){

	unitTestForRecItersec();

	return 0;
}












