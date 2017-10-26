#include "main.h"

void runConvexHull();
void runPointInsidePolygonCheck();

int main(){
	srand(time(NULL));

	std::cout << "Insert index of algorithm to run:\n";
	std::cout << "\t1. Point in Polygon test\n";
	std::cout << "\t2. Convex hull - Jarvis march\n";

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
	default:
		std::cout << "Invalid algorithm";
	}

	return 0;
}