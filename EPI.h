#ifndef EPI_H
#define EPI_H

#include <iostream>
#include <stdlib.h>
#include <string>

#include <bitset>
#include <time.h>
#include <limits>
#include <math.h>
#include <vector>
#include <deque>
#include <iterator>

#include <algorithm>


// Overload << to cout elements in vector easily for testing
template<class T>
std::ostream& operator<<(std::ostream& stream, const std::vector<T>& values)
{
	stream << "[ ";
	copy( begin(values), end(values), std::ostream_iterator<T>(stream, " ") );
	stream << ']';
	return stream;
}

template<class T>
std::ostream& operator<<(std::ostream& stream, const std::deque<T>& values)
{
	stream << "[ ";
	copy( begin(values), end(values), std::ostream_iterator<T>(stream, " ") );
	stream << ']';
	return stream;
}

#endif

