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