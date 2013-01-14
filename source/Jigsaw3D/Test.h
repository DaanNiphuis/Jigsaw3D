#ifndef JIGSAWTEST_H
#define JIGSAWTEST_H

class PuzzleLayout;

class Test
{
public:
	// Caller is responsible for deleting the returned layout.
	static const PuzzleLayout* runDifficultTest();

private:
	Test();
	~Test();
};

#endif