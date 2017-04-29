#include "EPI.h"

/* Chapter 5 Primary Type */

/******* 5.1 Parity Problem *******/

/* The odd number of set bit returns 1, else return 0 */

/* Brute-force */

int parityBruteForce (unsigned long num) {

	short parity = 0;
	while (num) {

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

int parityOnlyViewSetBit(unsigned long num) {

	short parity = 0;

	while (num) {
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

int parityDivAndConq(unsigned long long num) {

	int shift = 32;
	do {
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

	} while (shift);

	return num & 1;
}

/******* 5.2 Swap Problem *******/

/* Swap the bits those are on the index i and index j */

/* Brute-force */

int swapBruteForce (unsigned long num, int i, int j) {

	if (i == j) return num;

	unsigned long maskOfI = (num >> i) & 0x1;
	unsigned long maskOfJ = (num >> j) & 0x1;

	if (maskOfI) num = num | (maskOfI << j);
	else num = num & ~(0x1 << j);

	if (maskOfJ) num = num | (maskOfJ << i);
	else num = num & ~(0x1 << i);

	return num;
}

/*
Improvement without save the value.

Note: the "exclusive or" with 1 swap the bit value
when values on i and j are different.

1 ^ 1 = 0, 0 ^ 1 = 1
*/

int swapByMask( unsigned long num, int i, int j) {

	if (((num >> i) & 0x1) != ((num >> j) & 0x1)) {
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

double expoRecursion(double x, int y) {

	/* Consider that y could be negative number*/
	if ( y < 0) {
		y = - y;
		x = 1.0 / x;
	}
	if (y && (y % 2)) {
		return x * expoRecursion(x, y - 1);
	}
	if (y && !(y % 2)) {
		return expoRecursion(x, y >> 1) * expoRecursion(x, y >> 1);
	}
	return 1;

}

/*
Now consider loop version for problem
*/

double expoLoop(double x, int y) {

	if (y < 0) {
		y = -y;
		x = 1.0 / x;
	}

	double ans = 1.0;

	while (y) {
		/*
		if the most right is set bit then multiply
		current x value once more */
		if ( y & 0x1) {
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

int revIntBruteForce(int n) {

	int numPos = 1;
	int prePos = 1;
	int ans = 0;

	bool negative = n < 0;

	n = negative ? -n : n;

	while (numPos <= n) {
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

int revIntImprove(int n) {

	int ans = 0;
	bool negative = n < 0;

	n = negative ? -n : n;

	while (n) {
		ans = ans * 10 + n % 10;
		n /= 10;
	}
	return negative ? -ans : ans;
}

/******* 5.3 Reverse bits Problem *******/

/*
Brute force through the way in 5.2
*/

unsigned long revBits(unsigned long num) {

	int len = (sizeof num) * 4;
	std::cout << len << std::endl;

	for (int i = 0; i < len / 2; i++) {
		if (((num >> i) & 0x1) != ((num >> (len - i - 1)) & 0x1)) {

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

int checkPalindrome(long num) {

	if (num < 0) return 0;

	long rev = 0;
	long orig = num;

	while (num) {
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

int checkPalindromeImprove(long num) {

	if (num < 0) return 0;

	int maskLength = floor(log10(num)) + 1;
	int numMask = pow(10, maskLength - 1);

	while (num) {
		int tail = num % 10;
		int head =  num / numMask;

		if (tail != head) return 0;

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

	rec(int x, int y, int width, int height): x(x), y(y), width(width), height(height) {
		/*std::cout << "rectangle created" << std::endl;*/
	}
} Rectangle;

int isIntersection(Rectangle & R1, Rectangle & R2) {
	return R1.x <= R2.x + R2.x + R2.width &&
	       R1.y <= R2.y + R2.y + R2.height &&
	       R2.x <= R1.x + R1.x + R1.width &&
	       R2.y <= R2.y + R2.y + R2.height;
}

Rectangle intersectionRec(Rectangle &R1, Rectangle &R2) {

	if (isIntersection(R1, R2)) {
		Rectangle result = Rectangle(
		                       std::max(R1.x, R2.x),
		                       std::max(R1.y, R2.y),
		                       std::min(R1.x + R1.width, R2.x + R2.width) - std::max(R1.x, R2.x),
		                       std::min(R1.y + R1.height, R2.y + R2.height) - std::max(R1.y, R2.y)
		                   );

		return result;
	}

	return Rectangle(0, 0, -1, -1);
}

void unitTestForRecItersec(void) {

	Rectangle R1 = Rectangle(0, 0, 1, 1);
	Rectangle R2 = Rectangle(1, 1, 2, 3);
	Rectangle R3 = Rectangle(2, 0, 1, 3);

	Rectangle res1 = intersectionRec(R1, R2); // one pix intersection
	Rectangle res2 = intersectionRec(R2, R3); // rectangle intersection
	Rectangle res3 = intersectionRec(R1, R3); // no intersection

	std::cout << "R1 & R2: " << res1.x << "," << res1.y << "," << res1.width << "," << res1.height << std::endl;
	std::cout << "R2 & R3: " << res2.x << "," << res2.y << "," << res2.width << "," << res2.height << std::endl;
	std::cout << "R1 & R3: " << res3.x << "," << res3.y << "," << res3.width << "," << res3.height << std::endl;

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
void insert_sort(std::vector<int> & v) {
	for (auto i = v.begin(); i != v.end(); i++) {
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
void lambda_sort(std::vector<int> &v) {

	return std::sort(v.begin(), v.end(), [](const int& a, const int& b) {
		return b < a;
	});
}
/*
->	The lambda function is a c++11 feature

*********************/

/******* 6.1 The Dutch National Flag Problem *******/

/*

take an array A, and index i, and rearrange
all the element that is smaller than pivot A[i] left
all the element that is same as A[i] middle
all the element that is larger than A[i] right

use loop two pinters
*/

void rearrange (std::vector<int> & v, int idx) {

	int smaller = 0, unclassified = 0, larger = v.size() - 1;
	int pivot = v[idx];

	while (unclassified < larger) {

		// Basic idea : smaller++ when smaller is smaller than pivot
		//				larger-- when larger is larger than pivot
		//				unclassified go through from left to right

		if (v[unclassified] < pivot) {
			std::swap(v[smaller++], v[unclassified++]);
		} else if (v[unclassified] == pivot) {
			unclassified++;
		} else {
			std::swap(v[unclassified], v[larger--]);
		}
	}
}

/******* 6.2 Increment an arbitrary-preciion Integer *******/

/*
<1,2,9> -> <1,3,0> [Easy]: O(N)
*/

void incre_arb_int(std::vector<int> & v) {

	for (auto it = v.rbegin(); it != v.rend(); ++it) {

		if ( *it != 9) {

			*it += 1;
			break;

		} else if (it == v.rend() - 1) {
			*it = 0;
			v.insert(v.begin(), 1);
		} else {
			*it = 0;
		}
	}
}

/******* 6.3 Multiply two arbitrary-preciion Integers *******/

/*
<-1,2> * <1,2> -> <-1,4,4>

** sometimes number iterator is better than pointer iterator***

->	using std::deque to remove leading zero convenient. However
	Elements in a deque are not contiguous in memory, and
	vector elements are guaranteed to be.
*/

std::deque<int> mult_arb_int (std::vector<int> v1, std::vector<int> v2) {

	std::deque<int> res(v1.size() + v2.size(), 0);

	int sign = v1[0] * v2[0] < 0 ? -1 : 1; // negative

	v1[0] = abs(v1[0]);
	v2[0] = abs(v2[0]);

	for (int i = v1.size() - 1; i >= 0; --i) {

		for (int j = v2.size() - 1; j >= 0; --j) {

			res[i + j + 1] += v1[i] * v2[j];
			res[i + j] += res[i + j + 1] / 10;
			res[i + j + 1] = res[i + j + 1] % 10;
		}
	}

	for (auto i = res.begin(); *i == 0; ++i) {
		res.pop_front();
	}

	res[0] = res[0] * sign;

	return res;
}

/******* 6.4 Advancing through an array *******/

/*

From begin advancing to last, the ith entry is the most steps
of advancing steps from i

1) Try from the end, back to end. Greedy Algorithm.
*/

bool advancing(std::vector<int> & v) {

	bool res = true;
	int idx = v.size() - 1;
	for (int i = v.size() - 1; i >= 0; --i) {
		if  (v[i] >= idx - i) {
			idx = i;
			res = true;
		} else {
			res = false;
		}
	}
	return res;
}

/******* 6.5 Del Duplicates from sorted array *******/

/*
<1,1,2,3,7,7,7> -> <1,2,3,7,0,0,0>. O(N) time and O(1) space
*/

int del_dup_sorted(std::vector<int> &v) {

	int res = 0;

	for (auto it = v.begin() + 1; it != v.end(); ++it) {

		if (v[res] != *it) v[++res] = *it;

		if (res != it - v.begin()) *it = 0;

	}
	return res + 1;
}

/******* 6.6 Buy and sell a stock once*******/

/*
the minimum may appear after the maximum
*/

int max_stock_diff(std::vector<int> & v) {

	int min_price_so_far = std::numeric_limits<int>::max();
	int max_profit = 0;

	for (int i = 0; i < v.size(); ++i) {

		if (min_price_so_far > v[i]) min_price_so_far = v[i];
		max_profit = std::max(max_profit, v[i] - min_price_so_far);
	}
	return max_profit;
}

/******* 6.7 Buy and sell a stock Twice*******/

// We could have a O(N) time complexity and O(N) space approach

/*
The idea is we create two Array to store the maximum the profit
A = max_profit in range(1 -> i) on ith position
B = max_profit in range(i -> n) on ith position
where 0 <= i <= n.

Then sum A and B one-by-one to get the maximum value.
*/
int max_stock_two(std::vector<int> &v) {

	std::vector<int> trade;

	int min_price_so_far = std::numeric_limits<int>::max();
	int max_profit = 0, max_price_so_far = 0, max_sum = 0;

	for (int i  = 0; i < v.size(); ++i) {
		if ( min_price_so_far > v[i]) min_price_so_far = v[i];
		max_profit = std::max(max_profit, v[i] - min_price_so_far);
		trade.push_back(max_profit);
	}
	max_profit = 0;

	for (int i = v.size() - 1; i > 0; --i) {
		if (max_price_so_far < v[i]) max_price_so_far = v[i];
		max_profit = std::max(max_profit, max_price_so_far - v[i]);
		trade[i] += max_profit;

		max_sum = trade[i] > max_sum ? trade[i] : max_sum;
	}

	return max_sum;
}


/******* 6.8 Enumerate all primes to N*******/

/*
Sieve of Eratosthenes (error-'tall-thi-nis) :)

Sieve of Eratosthenes:
->	time complexity: O(N * loglog(N))
->	space complexity in bit O(sqrt(N)),
	where each number has log(N) digits on average.
->	space complexity in number O(sqrt(N) / log(N))
*/

std::vector<int> primer_array(int N) {

	std::vector<short> v(N + 1, 1);
	v[0] = 0;
	v[1] = 0;
	for (int i = 2; i < ceil(std::sqrt(N)); ++i) {
		if (v[i]) {
			for (int j = i * i; j <= N; j += i) {
				v[j] = 0;
			}
		}
	}

	std::vector<int> res;
	for (int i = 0; i < N; ++i) {
		if (v[i]) res.push_back(i);
	}

	return res;
}

/******* 6.9 Permute the elements of an array*******/

/*
<2,0,1,3> & <a,b,c,d> -> <b,c,a,d>

2:0th in <2,0,1,3> -> 0:a to 2th
0:1th in <2,0,1,3> -> 1:b to 0th
1:2th in <2,0,1,3> -> 2:c to 1th
3:3th in <2,0,1,3> -> 3:d unchanged

->	std::list: unlike other standard sequence containers,
	list and forward_list objects are spectifically designed
	to be efficient inserting and removing elements in any
	position, even in the middle of the sequence.
*/

std::vector<char> permute(std::vector<char> &v, std::vector<int> &p) {
	std::vector<char> res(v.size(), 0);

	for (int i = 0; i < v.size(); ++i) {
		res[p[i]] = v[i];
	}

	return res;
}

//	Improve, we don't want to use O(N) extra space
//	Using O(1) instead

// 	The idea is there are some cycles in one permutation.
//	Loop the list to handle all cycles.

std::vector<char> permute_impv(std::vector<char> &v, std::vector<int> &p) {

	for (int i = 0; i < p.size(); ++i) {

		int next = i;

		while (p[next] >= 0) {

			int tmp = p[next];

			std::swap(v[i], v[p[next]]);

			p[next] -= p.size();

			next = tmp;
		}
	}

	//	restore the permutation list
	std::for_each(p.begin(), p.end(), [&, p](int x) {x += p.size();});
	return v;
}
/* 	C++11 Lambda
	[ ] -> what value you want to be captured in lambda function
		-> [xx] 	pass variable
		-> [&xx] 	pass by reference
		-> [&, xx] 	default passing by reference
		-> [=, &xx]	default passing by value, but passing xx by reference
	( ) -> variables as elements in iteration
	{ } -> lambda function definition
*/

/******* 6.10 Compute the next permutation*******/

/*
This problem is tricky.

->	<1,4,6,5,3,2> -> find <6,5,3,2>

-> 	find <4>, swap with <5> in <6,5,3,2> since 5 is the smallest
	element that is larger than 4

-> 	now we have <1,5,6,4,3,2>

->	we revers <6,4,3,2> to get <1,5,2,3,4,6> that is what we need

-> 	using reverse because after swap the sublist guarantee to decreasing order
*/

std::vector<int> next_permt(std::vector<int> &v) {

	for (auto it = v.rbegin(); it + 1 != v.rend(); ++it) {

		if (*(it + 1) < *it) {
			std::swap(*(it + 1),
			          *std::find_if(v.rbegin(), v.rend(),
			[&, it](int & x) {return x > *(it + 1);}));

			std::reverse(v.begin() - (it - v.rend()) - 1, v.end());

			// r_it - v.rend + 1 is the negative value of its index

			return v;
		}
	}

	return v;
}


/******* 6.11 Sample Offline Data*******/

/*
In the <random> library
->	std::uniform_int_distribution<int>{a,b} -> int
->	{min, max}(engin)
->	min, max must be 'int' but v.size() returns 'unsigned long'
->	static_cast<type>()
->	std::default_random_engine seed((std::random_device())())
*/

std::vector<int> random_subset(std::vector<int> &v, int k) {

	std::default_random_engine seed(
			(std::random_device())
			()
		);

	for (int i = 0; i < k; ++i) {
		std::swap(v[0], // --> swap two element in vector by index
			v[			// --> another index
				std::uniform_int_distribution<int> { // --> an integer equally prob
					i, 								 // --> from i
					static_cast<int>(v.size()) - 1 	 // --> to this number
				}
				(seed)								 // --> under this seed
			 ]
		);
	}

	return v;
}

/******* 6.12 Sample Online Data*******/



int main() {

	int array1[] = {1, 0, 3, 2};
	int array2[] = {6, 2, 1, 5, 4, 3, 0};

	std::vector<int> v1(array1, array1 + 4);
	std::vector<int> v2(array2, array2 + 7);

	std::cout << "test password" << std::endl;

	return 0;
}












