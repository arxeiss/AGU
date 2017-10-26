#include "main.h"

void showIm(cv::Mat pane) {
	cv::imshow("Point inside polygon", pane);
}

void printStep(int startIndex, int currentIndex, int endIndex, int direction = 0) {
	std::string format = "start %d, current %d, end %d = direction %s\n";
	if (direction == 0) {
		format = "start %d, current %d, end %d\n";
	}
	std::printf(format.c_str(), startIndex, currentIndex, endIndex, direction < 0 ? "left" : "right");
}

void drawPoint(cv::Mat &pane, cv::Point &point, bool main = false, bool highlight = false) {
	cv::Scalar color = CL_BLUE;
	if (main) {
		color = CL_RED_DARK;
	}
	if (highlight)
	{
		color = CL_BLUE_DARK;
	}
	cv::circle(pane, point, highlight || main ? 6 : 3, color, -1);
}

void drawArrowedLine(cv::Mat &pane, cv::Point &p1, cv::Point &p2, int directionLabel) {
	cv::Point middle = getLineMiddle(p1, p2);
	cv::Point2f perpVector = getNormalizedVector(getPerpendicularVector(p1, p2, directionLabel > 0));
	//cv::line(pane, middle, cv::Point(middle.x + perpVector.x * 10, middle.y + perpVector.y * 10), CL_WHITE);
	cv::putText(pane, directionLabel < 0 ? "L" : "R", cv::Point(middle.x + perpVector.x * 15, middle.y + perpVector.y * 15), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_GREEN_DARK, 1);
	cv::arrowedLine(pane, p1, p2, CL_GREEN_DARK, 2, 8, 0, 0.03);
}

void drawLines(cv::Mat &pane, std::vector<cv::Point> &polygon) {
	int i = 0;
	for (; i < polygon.size() - 1; i++)
	{
		cv::line(pane, polygon[i], polygon[i + 1], CL_YELLOW);
	}
	cv::line(pane, polygon[i], polygon[0], CL_YELLOW);
}

void drawTitledPoints(cv::Mat &pane, std::vector<cv::Point> &polygon, cv::Point &searchPoint) {
	for (int i = 0; i < polygon.size(); i++)
	{
		drawPoint(pane, polygon[i]);
		std::stringstream ss;
		ss << i;
		cv::putText(pane, ss.str(), cv::Point(polygon[i].x - 10, polygon[i].y- 10 ), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_WHITE, 1);
	}
	drawPoint(pane, searchPoint, true);
}

void runPointInsidePolygonCheck() {
	int width = 600, height = 400;
	cv::Mat pane(height, width, CV_8UC3);
	
	std::vector<cv::Point> polygon;
	cv::Point searchPoint;

	searchPoint = cv::Point(400, 180);
	//searchPoint = cv::Point(350, 40);
	//searchPoint = cv::Point(130, 310);	

	polygon.push_back(cv::Point(60, 200));
	polygon.push_back(cv::Point(200, 80));
	polygon.push_back(cv::Point(280, 60));
	polygon.push_back(cv::Point(450, 100));
	polygon.push_back(cv::Point(570, 220));
	polygon.push_back(cv::Point(550, 300));
	polygon.push_back(cv::Point(490, 350));
	polygon.push_back(cv::Point(230, 370));
	polygon.push_back(cv::Point(120, 320));

	pane = pane = CL_BACKGROUND;
	drawLines(pane, polygon);
	drawTitledPoints(pane, polygon, searchPoint);
	showIm(pane);
	cv::waitKey(10000);

	int startIndex = 0, currentIndex, endIndex = polygon.size() - 1;
	int direction = 0;
	while (abs(endIndex - startIndex) > 1)
	{
		pane = pane = CL_BACKGROUND;
		drawLines(pane, polygon);

		currentIndex = (endIndex - startIndex + 1) / 2 + startIndex;
		direction = getPointSideToLine(polygon[0], polygon[currentIndex], searchPoint);
		printStep(startIndex, currentIndex, endIndex, direction);

		drawArrowedLine(pane, polygon[0], polygon[currentIndex], direction);
		drawTitledPoints(pane, polygon, searchPoint);
		showIm(pane);
		cv::waitKey(2000);
		
		if (direction < 0)
		{
			endIndex = currentIndex;
		}
		else
		{
			startIndex = currentIndex;
		}
	}
	std::printf("Final: ");
	printStep(startIndex, currentIndex, endIndex);

	cv::Point p2 = (direction > 0 ? polygon[currentIndex] : polygon[startIndex]),
		p3 = (direction > 0 ? polygon[endIndex] : polygon[currentIndex]);
	bool isInsidePolygon = isPointInsideTriangle(polygon[0], p2, p3, searchPoint);
	
	pane = pane = CL_BACKGROUND;
	drawLines(pane, polygon);
	drawTitledPoints(pane, polygon, searchPoint);
	drawPoint(pane, polygon[0], false, true);
	drawPoint(pane, p2, false, true);
	drawPoint(pane, p3, false, true);
	showIm(pane);
	cv::waitKey(1500);

	drawArrowedLine(pane, polygon[0], p2, getPointSideToLine(polygon[0], p2, searchPoint));
	showIm(pane);
	cv::waitKey(1500);
	
	drawArrowedLine(pane, p2, p3, getPointSideToLine(p2, p3, searchPoint));
	showIm(pane);
	cv::waitKey(1500);
	
	drawArrowedLine(pane, p3, polygon[0], getPointSideToLine(p3, polygon[0], searchPoint));
	showIm(pane);
	if (isInsidePolygon) {
		std::printf("\nPoint is inside polygon - on the same side of all triangle edges");
	}
	else
	{
		std::printf("\nPoint is NOT inside polygon - NOT on the same side of all triangle edges");
	}
	cv::waitKey(0);

	return;

}