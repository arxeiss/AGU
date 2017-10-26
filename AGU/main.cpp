#include "main.h"

void runConvexHull();
void runPointInsidePolygonCheck();

int main(){
	srand(time(NULL));
	
	runPointInsidePolygonCheck();
	runConvexHull();

	return 0;
}