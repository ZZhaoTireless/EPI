#include <iostream>
#include <string>
#include <bitset>

using namespace std;

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



int main(){

	//cout << exp(6,3) << endl;

	return 0;
}