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
    	
    	
		parity ^= (parity & 1); 

		cout << "Parity is :" << parity << endl;

		num >>= 1;
	}
	return parity;

}

int main(){

	cout << parity(37) << endl;

	return 0;
}