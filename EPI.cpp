#include "EPI.h"

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
    	std::cout << binary << std::endl;


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

	std::cout << len << std::endl;

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
		/*std::cout << "rectangle created" << std::endl;*/
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
			std::max(R1.x, R2.x),
			std::max(R1.y, R2.y),
			std::min(R1.x + R1.width, R2.x + R2.width) - std::max(R1.x,R2.x),
			std::min(R1.y + R1.height, R2.y + R2.height) - std::max(R1.y,R2.y)
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

	std::cout << "R1 & R2: " <<res1.x << "," << res1.y << "," << res1.width << "," << res1.height << std::endl;
	std::cout << "R2 & R3: " <<res2.x << "," << res2.y << "," << res2.width << "," << res2.height << std::endl;
	std::cout << "R1 & R3: " <<res3.x << "," << res3.y << "," << res3.width << "," << res3.height << std::endl;

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

/***** <vector> *****
Dynamic size, stored in heap.
    

*********************

****** <array> ******
 Statically-size, stored in stack.

*********************

**** <alogrithm> ****
1. std::binary_search(A.begin(), A.end(), val, comp)

->	std::sort(A.begin(), A.end(), comp) to sort the 
	container first

-> 	comp is function that compares two elements, and
	return if the first one is 'lower' than the second one  
-> 	Return true if find val

-> 	Return false if not find val

->	The container must be sorted before using. if not,
	use if(std::find(A.begin(), A.end(), value) != A.end) instead.
->	Performs at most log2(N) + 1

	where N is the distance between begin and end

2. std::lower_bound(A.begin(), A.end(), val, comp)
   std::upper_bound(A.begin(), A.end(), val, comp)

->	std::sort(A.begin(), A.end(), comp) to sort the 
	container first

-> 	comp is function that compares two elements, and
	return if the first one is 'lower' than the second one 

->	Used if there are multiple elements in the range whose 
	value equals the value val.

->	a. for lower_bound, it returns the first iterator position
	such that the value is not less than val. i.e. *it >= val

	b. for upper_bound, it returns the first iterator position
	such that the values is larger than val. i.e. *it > val
	
	else return the A.end()

->	Performs at most log2(N) + 1
	where N is the distance between begin and end

3. std::fill(A.begin(), A.end(), val)

->	fill val from A.begin() to A.end()

->	Performs linearly

4. std::swap(x, y)

->	swap two vals

-> 	could swap to vector as well
	e.g. vector<int> A = {3,1}, B = {3,2}, after swap(A,B)
	A= [2,2,2] B = [1,1,1]

-> 	non-array: perform constant time
	array: linearly complexiy, swap each element in array

->	std::swap(vec1, vec2) will just call vec1.swap(vec2)

-> 	always use:

	using std::swap;
	swap(x, y);
	
	instead of std::swap(x,y) because the approach outlined
	about ADL(Argument-dependent lookup) can find a better 
	version.

5. min_element(A.begin(), A.end(), comp)
   max_element(A.begin(), A.end(), comp)

->	don't have to be sorted

->	comp is function that compares two elements, and
	return if the first one is 'lower' than the second one

->	min_element return the iterator position of minimum
	val in the range of [A.begin, A.end)
	
	max_element return the iterator position of maximum
	val in the range of [A.begin, A.end)

->	Perform linearly complexity

6. reverse(A.begin(), A.end())

-> 	reverse array, vector

-> 	perform half linearly complexity
	iterator of swap(first, last)

7. rotate(A.begin(), A.begin() + shift, A.end())

->	take values between A.begin() and A.begin() + shift
	to the left of A.end()

->	up to linear the distance (first, and last)

->	Here is a insert sort implementation by using 
	std::upper_bound and std::rotate
*/
void insert_sort(std::vector<int> & v){
	for (auto i = v.begin(); i != v.end(); i++){
		std::rotate(std::upper_bound(v.begin(), i, *i), i, i + 1);
	}
}
/*

8. std::sort(A.begin(), A.end(), comp)

->	Since C++ 11, the worst case of std::sort()
	will be guarantee the worst case is O(NlogN)

->	In the g++4.5, std::sort using Introsort to
	solve the problem.

	Under range[0,16], it will use insert sort, which
	is O(N^2) complexity.

	It mainly use quick-sort of median of three values,
	which is choosing the median of [first...middle...last]
	as the pivot. 
	The complexity of quick-sort could be O(N^2)

	So, once the recursion times for quick-sort are larger
	than the log2(len(Array)) * 2, it will change to
	heapsort() to ensure the overall complexity is less than
	O(NlogN)

	O(N + (N - 1) + ... + (N - log(N))) < O(NlogN) complexity for quick-sort
	O((N - log(N)) log(N - log(N))) < O(NlogN) for heap-sort 

	Thus, the overall complexity is less than 2 * O(NlogN) for the 
	std::sort()

->	comp is function that compares two elements, and
	return if the first one is 'lower' than the second one

	This comp could be a lambda, for example:
*/
void lambda_sort(std::vector<int> &v){

	return std::sort(v.begin(), v.end(), [](const int& a, const int& b){
		return b < a;
	});
} 
/*
->	The lambda function 
*********************/

int main(){

	int array[] = {3,1,2,5,4,0};

	std::vector<int> v(array, array + 6);

	lambda_sort(v);

	std::cout << v << std::endl;

	return 0;
}












