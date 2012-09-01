#ifndef COMMON_H_
#define COMMON_H_

typedef unsigned int uint;

//Used as array index, do not change order.
struct Location {
	enum Enum {
		Front = 0,
		Back = 1,
		Left = 2,
		Right = 3,
		Top = 4,
		Bottom = 5,

		COUNT = 6
	};
};

#endif /* COMMON_H_ */
