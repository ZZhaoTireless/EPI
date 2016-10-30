#include <iostream>
#include <string>
#include <bitset>

using namespace std;

/*
Brute-force
*/

int parity (unsigned int num){

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

*/

int main(){

	cout << parity(36) << endl;

	return 0;
}