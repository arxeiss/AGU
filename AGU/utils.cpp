#include "main.h"

float getRandom(float from, float to) {
	float rnd0to1 = ((double)rand() / (RAND_MAX));
	return rnd0to1 * (to - from) + from;
}
float getRandom(float to) {
	return getRandom(0.0f, to);
}

cv::Point getLineMiddle(cv::Point p1, cv::Point p2) {
	return cv::Point(std::min(p1.x, p2.x) + (abs(p1.x - p2.x) / 2), std::min(p1.y, p2.y) + (abs(p1.y - p2.y) / 2));
}
float getLineLength(const cv::Point p1, const cv::Point p2) {
	return sqrt(SQR(p1.x - p2.x) + SQR(p1.y - p2.y));
}
float getLineLength(const Line line) {
	return getLineLength(line.first, line.second);
}
int getPointSideToLine(cv::Point lineStart, cv::Point lineEnd, cv::Point testPoint) {
	// https://math.stackexchange.com/questions/274712/calculate-on-which-side-of-a-straight-line-is-a-given-point-located
	int d = (testPoint.x - lineStart.x)*(lineEnd.y - lineStart.y) -
		(testPoint.y - lineStart.y)*(lineEnd.x - lineStart.x);
	return d * -1;
}

bool isPointInsideTriangle(cv::Point p1, cv::Point p2, cv::Point p3, cv::Point searchPoint) {
	int t1 = getPointSideToLine(p1, p3, searchPoint) < 0 ? -1 : 1;
	int t2 = getPointSideToLine(p3, p2, searchPoint) < 0 ? -1 : 1;
	int t3 = getPointSideToLine(p2, p1, searchPoint) < 0 ? -1 : 1;
	return t1 == t2 && t2 == t3;
}

Line getInfiniteLinePoints(int width, int height, cv::Point p1, cv::Point p2, bool keepFirst, bool keepSecond) {
	int multiply = sqrt(SQR(width) + SQR(height));
	Vec2f lineVec = getNormalizedVector(p1, p2);

	if (!keepFirst) {
		p1.x = p1.x - lineVec.x * multiply;
		p1.y = p1.y - lineVec.y * multiply;
	}
	if (!keepSecond) {
		p2.x = p2.x + lineVec.x * multiply;
		p2.y = p2.y + lineVec.y * multiply;
	}
	cv::clipLine(cv::Rect(0, 0, width, height), p1, p2);
	return Line(p1, p2);
}

Vec2f getVector(cv::Point p1, cv::Point p2) {
	return Vec2f(p2 - p1);
}
Vec2f getNormalizedVector(Vec2f vec) {
	float length = sqrt(SQR(vec.x) + SQR(vec.y));
	vec.x /= length;
	vec.y /= length;
	return vec;
}
Vec2f getNormalizedVector(const cv::Point p1, const cv::Point p2) {
	return getNormalizedVector(p2 - p1);
}
Vec2f getPerpendicularVector(cv::Point p1, cv::Point p2, bool toRight) {
	return getPerpendicularVector(p2 - p1, toRight);
}
Vec2f getPerpendicularVector(Vec2f vec, bool toRight) {
	return Vec2f((toRight ? -vec.y : vec.y), (toRight ? vec.x : -vec.x));
}
Vec2f rotateVector(const Vec2f vec, float deg) {
	return Vec2f(
		vec.x * cos(DEG2RAD(deg)) - vec.y * sin(DEG2RAD(deg)),
		vec.y * cos(DEG2RAD(deg)) + vec.x * sin(DEG2RAD(deg))
	);
}

void doubleArrowedLine(cv::Mat &mat, cv::Point p1, cv::Point p2, cv::Scalar color, int thickness, int arrowSize) {
	cv::line(mat, p1, p2, color, thickness);

	Vec2f arrowVec1 = rotateVector(getNormalizedVector(p1, p2), 45);
	Vec2f arrowVec2 = getPerpendicularVector(arrowVec1, false);
	
	cv::line(mat, p1, cv::Point(p1.x + arrowVec1.x * arrowSize, p1.y + arrowVec1.y * arrowSize), color, thickness);	
	cv::line(mat, p1, cv::Point(p1.x + arrowVec2.x * arrowSize, p1.y + arrowVec2.y * arrowSize), color, thickness);

	cv::line(mat, p2, cv::Point(p2.x - arrowVec1.x * arrowSize, p2.y - arrowVec1.y * arrowSize), color, thickness);
	cv::line(mat, p2, cv::Point(p2.x - arrowVec2.x * arrowSize, p2.y - arrowVec2.y * arrowSize), color, thickness);

}
void doubleArrowedLine(cv::Mat &mat, Line line, cv::Scalar color, int thickness, int arrowSize) {
	doubleArrowedLine(mat, line.first, line.second, color, thickness, arrowSize);
}