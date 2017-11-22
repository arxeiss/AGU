#include "main.h"

void runConvexHull();
void runPointInsidePolygonCheck();
void runClosestPair1D();
void runClosestPair2D();

int main(){
	srand(time(NULL));

	runClosestPair2D();
	return 0;

	std::cout << "Insert index of algorithm to run:\n";
	std::cout << "\t1. Point in Polygon test\n";
	std::cout << "\t2. Convex hull - Jarvis march\n";
	std::cout << "\t3. Find closest pair in 1D\n";
	std::cout << "\t4. Find closest pair in 2D\n";

	int algo;
	std::cin >> algo;
	
	switch (algo)
	{
	case 1:
		runPointInsidePolygonCheck();
		break;
	case 2:
		runConvexHull();
		break;
	case 3:
		runClosestPair1D();
		break;
	case 4:
		runClosestPair2D();
		break;
	default:
		std::cout << "Invalid algorithm";
	}

	return 0;
}