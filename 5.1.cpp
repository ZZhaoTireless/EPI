#include <iostream>
#include <string>
#include <bitset>

using namespace std;

/*
Brute-force
*/

int parity (unsigned long num){

	short parity = 0;

	while(num){

		/*Show binary form of 'num'*/
    	std::string binary = std::bitset<8>(num).to_string();
    	cout << binary << endl;
    	
    	
		parity ^= (num & 1); 
		num >>= 1;
	}
	return parity;

}

/*
Improve best and average case

x & (x - 1) -> each time eliminte one bit of 1 on most right position
*/

int parityImprove(unsigned long num){

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

int parityNextImprove(unsigned long long num){

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


int main(){

	cout << parityNextImprove(37) << endl;

	return 0;
}