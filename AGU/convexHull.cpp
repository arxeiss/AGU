#include "main.h"

struct ConvexHullPoint
{
	cv::Point point;
	bool toProcess;
};

void drawPoints(cv::Mat &pane, std::vector<ConvexHullPoint> &points, cv::Point *activePoint = NULL) {
	for (int i = 0; i < points.size(); i++)
	{
		cv::circle(pane, points[i].point, points[i].toProcess ? 2 : 5, points[i].toProcess ? CL_BLUE : CL_RED , -1);
		if (activePoint != NULL && points[i].point == *activePoint)
		{
			cv::circle(pane, points[i].point, 8, CL_RED, 1);
		}
		/*std::string ss = std::to_string(i);
		cv::putText(pane, ss.c_str(), cv::Point(polygon[i].x - 10, polygon[i].y - 10), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_WHITE, 1);*/
	}
}
void drawLineToFindSmallestAngle(cv::Mat &pane, cv::Point sourcePoint, cv::Point destPoint, bool textBeforeEndpoint = false) {
	
	Line line = getInfiniteLinePoints(pane.cols, pane.rows, sourcePoint, destPoint, true);
	cv::line(pane, line.first, line.second, CL_RED_DARK);

	Vec2f lineVector = getNormalizedVector(destPoint - sourcePoint);
	cv::Point2f perpVector = getPerpendicularVector(lineVector);
	
	//cv::line(pane, textPosition, cv::Point(textPosition.x + perpVector.x * 10, textPosition.y + perpVector.y * 10), CL_WHITE);

	cv::putText(pane, "p", cv::Point(
		destPoint.x + (lineVector.x * 40 * (textBeforeEndpoint ? -1 : 1)) + perpVector.x * 15,
		destPoint.y + (lineVector.y * 40 * (textBeforeEndpoint ? -1 : 1)) + perpVector.y * 15
	), CV_FONT_HERSHEY_SIMPLEX, 0.6f, CL_GREEN, 1);

}
void drawHullPolygon(cv::Mat &pane, std::vector<cv::Point> &polygon, bool close = false) {
	if (polygon.size() < 2) { return; }
	int i = 0;
	for (; i < polygon.size() - 1; i++)
	{
		cv::line(pane, polygon[i], polygon[i + 1], CL_YELLOW_DARK, 3);
	}
	if (close) {
		cv::line(pane, polygon[i], polygon[0], CL_YELLOW_DARK, 3);
	}
}

int getMostLeftPointIndex(std::vector<ConvexHullPoint> &points) {
	int minX = INT_MAX;
	int index = 0;
	for (int i = 0; i < points.size(); i++) {
		if (points[i].point.x < minX)
		{
			minX = points[i].point.x;
			index = i;
		}
	}
	return index;
}
int getPointIndexWithMinimumAngle(std::vector<ConvexHullPoint> &points, cv::Point startLine, cv::Point endLine, cv::Point countInPoint, cv::Mat &pane, std::vector<cv::Point> &polygon) {
	Vec2f lineVector = getNormalizedVector(startLine, endLine);
	float angleCos = -10;
	int index = 0;
	int wait = polygon.size() < 3 ? 100 : 20;
	for (int i = 0; i < points.size(); i++)
	{
		if (points[i].point == startLine || points[i].point == endLine) {
			continue;
		}
		Vec2f pointVector = getNormalizedVector(points[i].point - countInPoint);
		float angle = lineVector.dot(pointVector);		

		pane = pane = CL_BACKGROUND;
		drawHullPolygon(pane, polygon);
		//cv::line(pane, startLine, cv::Point(startLine.x + lineVector.x * 45, startLine.y + lineVector.y * 45), CL_WHITE);
		//cv::line(pane, countInPoint, cv::Point(countInPoint.x + pointVector.x * 45, countInPoint.y + pointVector.y * 45), CL_WHITE);
		drawLineToFindSmallestAngle(pane, startLine, endLine, polygon.size() == 1);
		cv::line(pane, countInPoint, points[i].point, CL_GRAY);

		std::cout << "Points: start/doIn/end " << startLine << "/" << countInPoint << "/" << points[i].point << "\n\tlineVec/pointVec " << lineVector << "/" << pointVector << " - angle" << angle;
		if (angle > angleCos)
		{
			angleCos = angle;
			index = i;
			std::cout << " Change to " << i;
		}

		cv::line(pane, countInPoint, points[index].point, CL_YELLOW, 2);
		drawPoints(pane, points, &countInPoint);
		cv::circle(pane, points[i].point, 5, CL_WHITE, 1);

		cv::imshow("Convex hull", pane);
		cv::waitKey(wait);

		std::cout << "\n";
		
	}

	std::cout << "Final angle" << angleCos << " Point index " << index << "\n\n";

	cv::waitKey(wait);

	return index;
}

void runConvexHull() {
	
	cv::Mat pane(500, 700, CV_8UC3);
	int padding = 50;
	int maxWidth = pane.cols - 2 * padding;
	int maxHeight = pane.rows - 2 * padding;

	std::vector<ConvexHullPoint> points(50);
	std::vector<cv::Point> polygon;
	for (int i = 0; i < points.size(); i++)
	{
		points[i] = ConvexHullPoint{ cv::Point(
			(int)(maxWidth * getRandom()) + padding,
			(int)(maxHeight * getRandom()) + padding
		), true };
	}
	pane = pane = CL_BACKGROUND;
	drawPoints(pane, points);
	cv::imshow("Convex hull", pane);
	cv::waitKey(0);
	// Get start point
	int startIndex = getMostLeftPointIndex(points);
	points[startIndex].toProcess = false;	
	cv::Point prevPoint = points[startIndex].point;
	pane = pane = CL_BACKGROUND;
	drawPoints(pane, points);
	cv::imshow("Convex hull", pane);
	cv::waitKey(1000);
	polygon.push_back(points[startIndex].point);

	// Get next point
	int nextIndex = getPointIndexWithMinimumAngle(points, prevPoint, cv::Point(points[startIndex].point.x, pane.rows), prevPoint, pane, polygon);
	cv::Point nextPoint = points[nextIndex].point;
	points[nextIndex].toProcess = false;
	polygon.push_back(points[nextIndex].point);
	pane = pane = CL_BACKGROUND;
	drawHullPolygon(pane, polygon);
	drawPoints(pane, points);	
	cv::imshow("Convex hull", pane);
	//cv::waitKey(0);

	for (int i = 0; nextIndex != startIndex ; i++)
	{
		//drawLineToFindSmallestAngle(pane, prevPoint, nextPoint);
		cv::imshow("Convex hull", pane);
		nextIndex = getPointIndexWithMinimumAngle(points, prevPoint, nextPoint, nextPoint, pane, polygon);
		prevPoint = nextPoint;
 		nextPoint = points[nextIndex].point;
		points[nextIndex].toProcess = false;
		polygon.push_back(points[nextIndex].point);

		/*drawPoints(pane, points);
		cv::imshow("Convex hull", pane);
		cv::waitKey(5);*/
	}

	pane = pane = CL_BACKGROUND;
	drawHullPolygon(pane, polygon, true);
	drawPoints(pane, points);
	cv::imshow("Convex hull", pane);	
	cv::waitKey(0);

}