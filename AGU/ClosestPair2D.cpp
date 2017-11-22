#include "main.h"

class CP2DWrap
{
public:
	cv::Mat pane;
	std::vector<cv::Point> points;
	std::vector<Line> shortestDistances;
};

int showWithDelay(CP2DWrap &dataWrap, int delay = 0) {
	cv::imshow("Closest pair 2D", dataWrap.pane);
	return cv::waitKey(delay);
}
void drawPoints(cv::Mat &pane, std::vector<cv::Point> &points, int startIndex, int endIndex, int pointType = 0, bool clearPane = false, bool showIndex = false) {
	if (clearPane) {
		pane = CL_BACKGROUND;
	}
	cv::Scalar color = pointType == 0 ? CL_GREENBLUE : CL_BLUE_DARK;
	int radius = pointType == 0 ? 5 : 3;
	for (int i = startIndex; i <= endIndex; i++)
	{
		cv::circle(pane, points[i], radius, color, -1);
		if (showIndex) {
			std::stringstream ss;
			ss << (i + 1);
			cv::putText(pane, ss.str(), cv::Point(points[i].x , points[i].y - 8), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_WHITE, 1);
		}
	}
}
void drawPoints(CP2DWrap &dataWrap, int startIndex, int endIndex, int pointType = 0, bool clearPane = false, bool showIndex = false) {
	drawPoints(dataWrap.pane, dataWrap.points, startIndex, endIndex, pointType, clearPane, showIndex);
}
void drawAllPoints(CP2DWrap &dataWrap, int pointType = 1, bool clearPane = true, bool showIndex = false) {
	drawPoints(dataWrap, 0, dataWrap.points.size() - 1, pointType, clearPane, showIndex);
}
bool sortPointsByX(const cv::Point a, const cv::Point b) { return (a.x < b.x); }
bool sortPointsByY(const cv::Point a, const cv::Point b) { return (a.y < b.y); }
void drawBoundingBox(CP2DWrap &dataWrap, int startIndex, int endIndex) {
	int minX = startIndex == 0 ? dataWrap.points[startIndex].x - 25 :
		MIDDLE_NUM(dataWrap.points[startIndex].x, dataWrap.points[startIndex - 1].x);
	int maxX = endIndex == dataWrap.points.size() - 1 ? dataWrap.points[endIndex].x + 25 :
		MIDDLE_NUM(dataWrap.points[endIndex].x, dataWrap.points[endIndex + 1].x);
	cv::rectangle(dataWrap.pane, cv::Point(minX, 25), cv::Point(maxX, dataWrap.pane.rows - 25), CL_GRAY);
}
void drawStoredDistances(CP2DWrap &dataWrap) {
	for (int i = 0; i < dataWrap.shortestDistances.size(); i++)
	{
		doubleArrowedLine(dataWrap.pane, dataWrap.shortestDistances[i], CL_GRAY, 2, 7);
	}
}
void drawStripBase(CP2DWrap &dataWrap, std::vector<cv::Point> &points, int middleX, float stripSize, int startIndex, int endIndex, bool showIndex = false) {
	drawAllPoints(dataWrap);
	drawBoundingBox(dataWrap, startIndex, endIndex);

	cv::line(dataWrap.pane, cv::Point(middleX, 15), cv::Point(middleX, dataWrap.pane.rows - 25), CL_RED_DARK);
	int leftX = std::max<int>((int)(middleX - stripSize), dataWrap.points[startIndex].x);
	cv::line(dataWrap.pane, cv::Point(leftX, 15), cv::Point(leftX, dataWrap.pane.rows - 25), CL_GRAY);
	int rightX = std::min<int>((int)(middleX + stripSize), dataWrap.points[endIndex].x);
	cv::line(dataWrap.pane, cv::Point(rightX, 15), cv::Point(rightX, dataWrap.pane.rows - 25), CL_GRAY);

	drawPoints(dataWrap.pane, points, 0, points.size() - 1, 0, false, showIndex);
	drawStoredDistances(dataWrap);
}
void drawBase(CP2DWrap &dataWrap, int startIndex, int endIndex) {
	drawAllPoints(dataWrap);
	drawBoundingBox(dataWrap, startIndex, endIndex);
	drawPoints(dataWrap, startIndex, endIndex);
	drawStoredDistances(dataWrap);
}

/*
	http://www.geeksforgeeks.org/closest-pair-of-points/
*/

Line getShortestDistanceBruteForce(CP2DWrap &dataWrap, int startIndex, int endIndex) {
	float smallestDistance = FLT_MAX;
	int p1i = 0, p2i = 0;
	for (int i = startIndex; i <= endIndex; i++)
	{
		for (int j = i + 1; j <= endIndex; j++)
		{
			float dist = getLineLength(dataWrap.points[i], dataWrap.points[j]);
			
			drawBase(dataWrap, startIndex, endIndex);
			doubleArrowedLine(dataWrap.pane, dataWrap.points[i], dataWrap.points[j], CL_PURPLE_DARK, 2, 7);

			if (dist < smallestDistance) {
				smallestDistance = dist;
				p1i = i;
				p2i = j;
			}

			doubleArrowedLine(dataWrap.pane, dataWrap.points[p1i], dataWrap.points[p2i], CL_YELLOW, 2, 7);
			showWithDelay(dataWrap, 500);
		}
	}
	dataWrap.shortestDistances.push_back(Line(dataWrap.points[p1i], dataWrap.points[p2i]));
	return Line(dataWrap.points[p1i], dataWrap.points[p2i]);
}

Line getShortestDistanceWithStrip(CP2DWrap &dataWrap, std::vector<cv::Point> &stripPoints, Line closest, int middleX, float stripSize, int startIndex, int endIndex) {
	drawStripBase(dataWrap, stripPoints, middleX, stripSize, startIndex, endIndex, true);
	doubleArrowedLine(dataWrap.pane, closest, CL_YELLOW, 2, 7);
	showWithDelay(dataWrap, 500);
	if (stripPoints.size() < 2)
	{
		return closest;
	}

	std::sort(stripPoints.begin(), stripPoints.end(), sortPointsByY);

	drawStripBase(dataWrap, stripPoints, middleX, stripSize, startIndex, endIndex, true);
	doubleArrowedLine(dataWrap.pane, closest, CL_YELLOW, 2, 7);
	showWithDelay(dataWrap, 500);

	float closestDistance = getLineLength(closest);
	for (int i = 0; i < stripPoints.size() - 1; i++)
	{
		for (int j = i+1; j < stripPoints.size() &&
			(stripPoints[j].y - stripPoints[i].y < closestDistance); j++)
		{
			drawStripBase(dataWrap, stripPoints, middleX, stripSize, startIndex, endIndex);
			doubleArrowedLine(dataWrap.pane, stripPoints[i], stripPoints[j], CL_PURPLE_DARK, 2, 7);

			if (getLineLength(stripPoints[i], stripPoints[j]) < closestDistance )
			{
				closestDistance = getLineLength(stripPoints[i], stripPoints[j]);
				closest = Line(stripPoints[i], stripPoints[j]);
			}

			doubleArrowedLine(dataWrap.pane, closest, CL_YELLOW, 2, 7);
			showWithDelay(dataWrap, 500);
		}
	}
	return closest;
}

Line getShortestDistance(CP2DWrap &dataWrap, int startIndex, int endIndex) {
	drawBase(dataWrap, startIndex, endIndex);
	showWithDelay(dataWrap, 500);

	if (endIndex - startIndex < 3)
	{
		return getShortestDistanceBruteForce(dataWrap, startIndex, endIndex);
	}

	int middleIndex = ((float)(endIndex - startIndex) / 2.0f) + startIndex;
	Line closestLeft = getShortestDistance(dataWrap, startIndex, middleIndex);
	Line closestRight = getShortestDistance(dataWrap, middleIndex + 1, endIndex);
	
	drawBase(dataWrap, startIndex, endIndex);
	showWithDelay(dataWrap, 500);

	dataWrap.shortestDistances.pop_back();
	dataWrap.shortestDistances.pop_back();

	float fromLeftDistance = getLineLength(closestLeft),
		fromRightDistance = getLineLength(closestRight);

	Line closestBoth = (fromLeftDistance < fromRightDistance) ? closestLeft : closestRight;
	float closestBothDistance = getLineLength(closestBoth);

	drawBase(dataWrap, startIndex, endIndex);
	doubleArrowedLine(dataWrap.pane, closestBoth, CL_YELLOW, 2, 7);
	showWithDelay(dataWrap, 500);

	int middleX = MIDDLE_NUM(dataWrap.points[middleIndex].x, dataWrap.points[middleIndex + 1].x);
	std::vector<cv::Point> stripLinePoints;
	for (int i = startIndex; i <= endIndex; i++)
	{
		if (abs(dataWrap.points[i].x - middleX) < closestBothDistance)
		{
			stripLinePoints.push_back(dataWrap.points[i]);
		}
	}
	float stripHalfWidth = (float)(dataWrap.points[endIndex].x - dataWrap.points[startIndex].x) / 2.0f;
	if (closestBothDistance < stripHalfWidth)
	{
		stripHalfWidth = closestBothDistance;
	}
	Line finalClosest = getShortestDistanceWithStrip(dataWrap, stripLinePoints, closestBoth, middleX, stripHalfWidth, startIndex, endIndex);
	
	dataWrap.shortestDistances.push_back(finalClosest);
	return finalClosest;
}

void runClosestPair2D() {
	CP2DWrap dataWrap;
	dataWrap.pane = cv::Mat(500, 1100, CV_8UC3);
	dataWrap.points = std::vector<cv::Point>(50);

	for (int i = 0; i < dataWrap.points.size(); i++)
	{
		dataWrap.points[i] = cv::Point2i(getRandom(0, dataWrap.pane.cols - 100) + 50, getRandom(0, dataWrap.pane.rows - 100) + 50);
	}

	drawAllPoints(dataWrap, 0, true, true);
	showWithDelay(dataWrap, 5000);
	std::sort(dataWrap.points.begin(), dataWrap.points.end(), sortPointsByX);
	drawAllPoints(dataWrap, 0, true, true);
	showWithDelay(dataWrap, 1000);
		
	Line shortestDistance = getShortestDistance(dataWrap, 0, dataWrap.points.size() - 1);
	
	drawAllPoints(dataWrap, 1);
	std::vector<cv::Point> finalPoints(2);
	finalPoints[0] = shortestDistance.first;
	finalPoints[1] = shortestDistance.second;
	drawPoints(dataWrap.pane, finalPoints, 0, 1);
	doubleArrowedLine(dataWrap.pane, shortestDistance, CL_YELLOW, 2, 7);
	showWithDelay(dataWrap, 0);
}