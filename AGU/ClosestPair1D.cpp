#include "main.h"

void drawPoints(cv::Mat &pane, std::vector<int> &points) {
	pane = CL_BACKGROUND;
	int y = pane.rows / 2;
	cv::line(pane, cv::Point(20, y), cv::Point(pane.cols - 20, y), CL_GRAY);
	for (int i = 0; i < points.size(); i++)
	{
		cv::circle(pane, cv::Point(points[i] + 30, y), 5, CL_RED, -1);
		std::string ss = std::to_string(i + 1);
		cv::putText(pane, ss.c_str(), cv::Point(points[i] + 25, y - 30), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_WHITE, 1);
	}
}

void drawDistLines(cv::Mat &pane, int currentX, int currentDist, int minDistX, int minDist) {
	int yLineCurr = pane.rows / 2 + 40;
	int yLineMin = pane.rows / 2 + 80;

	if (currentX >= 0)
	{
		cv::line(pane, cv::Point(currentX + 30, yLineCurr), cv::Point(currentX + currentDist + 30, yLineCurr), CL_GREEN);
		cv::line(pane, cv::Point(currentX + 30, yLineCurr - 5), cv::Point(currentX + 30, yLineCurr + 5), CL_GREEN);
		cv::line(pane, cv::Point(currentX + currentDist + 30, yLineCurr - 5), cv::Point(currentX + currentDist + 30, yLineCurr + 5), CL_GREEN);
		std::string ss = std::to_string(currentDist);
		cv::putText(pane, ss.c_str(), cv::Point(currentX + 23 + currentDist / 2, yLineCurr - 15), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_GREEN, 1);
		cv::putText(pane, "Current: ", cv::Point(5, yLineCurr - 5), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_GREEN, 1);
	}
	
	cv::line(pane, cv::Point(minDistX + 30, yLineMin), cv::Point(minDistX + minDist + 30, yLineMin), CL_ORANGE, 2);
	cv::line(pane, cv::Point(minDistX + 30, yLineMin - 5), cv::Point(minDistX + 30, yLineMin + 5), CL_ORANGE, 2);
	cv::line(pane, cv::Point(minDistX + minDist + 30, yLineMin - 5), cv::Point(minDistX + minDist + 30, yLineMin + 5), CL_ORANGE, 2);
	std::string ss = std::to_string(minDist);
	cv::putText(pane, ss.c_str(), cv::Point(minDistX + 23 + minDist / 2, yLineMin - 15), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_ORANGE, 1);
	cv::putText(pane, "Minimum: ", cv::Point(5, yLineMin - 5), CV_FONT_HERSHEY_SIMPLEX, 0.4f, CL_ORANGE, 1);
}

void runClosestPair1D() {
	std::vector<int> pointsInLine(10);
	for (int i = 0; i < pointsInLine.size(); i++)
	{
		pointsInLine[i] = (int)getRandom(0, 820);
	}
	cv::Mat pane(300, 900, CV_8UC3);
	drawPoints(pane, pointsInLine);
	cv::imshow("Closest pair 1D", pane);
	cv::waitKey(5000);

	std::sort(pointsInLine.begin(), pointsInLine.end());
	drawPoints(pane, pointsInLine);
	cv::imshow("Closest pair 1D", pane);
	cv::waitKey(1500);

	int minDistIndex = 0;
	for (int i = 0; i < pointsInLine.size() - 1; i++)
	{
		drawPoints(pane, pointsInLine);
		int currentDist = pointsInLine[i + 1] - pointsInLine[i];
		int minDist = pointsInLine[minDistIndex + 1] - pointsInLine[minDistIndex];
		if (currentDist < minDist) {
			minDistIndex = i;
			minDist = pointsInLine[minDistIndex + 1] - pointsInLine[minDistIndex];
		}
		
		drawDistLines(pane, pointsInLine[i], currentDist, pointsInLine[minDistIndex], minDist);
		cv::imshow("Closest pair 1D", pane);
		cv::waitKey(800);
	}
	cv::waitKey(1000);
	drawPoints(pane, pointsInLine);
	int minDist = pointsInLine[minDistIndex + 1] - pointsInLine[minDistIndex];
	drawDistLines(pane, -1, 0, pointsInLine[minDistIndex], minDist);
	cv::imshow("Closest pair 1D", pane);

	cv::waitKey(0);
}