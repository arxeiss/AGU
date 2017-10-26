#pragma once

#define SQR(x) ((x)*(x))
#define DEG2RAD(deg) ((deg) * M_PI / 180.0f)
#define INT_FLOOR(x) ((int)std::floor(x))
#define INT_CEIL(x) ((int)std::ceil(x))

#define CL_BACKGROUND cv::Scalar(55, 55, 55)
#define CL_WHITE cv::Scalar(255, 255, 255)
#define CL_GRAY cv::Scalar(90, 90, 90)

#define CL_RED cv::Scalar(79, 79, 250)
#define CL_RED_DARK cv::Scalar(17, 17, 196)
#define CL_BLUE cv::Scalar(243, 198, 77)
#define CL_BLUE_DARK cv::Scalar(241, 119, 0)
#define CL_GREEN cv::Scalar(91, 237, 62)
#define CL_GREEN_DARK cv::Scalar(39, 234, 0)
#define CL_YELLOW cv::Scalar(85, 234, 247)
#define CL_YELLOW_DARK cv::Scalar(9, 188, 226)
#define CL_PURPLE cv::Scalar(240, 68, 242)
#define CL_PURPLE_DARK cv::Scalar(212, 28, 214)
#define CL_ORANGE cv::Scalar(38, 186, 255)
#define CL_GREENBLUE cv::Scalar(123, 231, 18)

using Vec2f = cv::Point2f;
using Line = std::pair<cv::Point, cv::Point>;

float getRandom(float from, float to);
float getRandom(float to = 1.0f);

cv::Point getLineMiddle(const cv::Point p1, const cv::Point p2);
int getPointSideToLine(const cv::Point lineStart, const cv::Point lineEnd, const cv::Point testPoint);
bool isPointInsideTriangle(const cv::Point p1, const cv::Point p2, const cv::Point p3, const cv::Point searchPoint);

Line getInfiniteLinePoints(const int width, const int height, cv::Point p1, cv::Point p2, const bool keepFirst = false, const bool keepSecond = false);

Vec2f getVector(const cv::Point p1, const cv::Point p2);
Vec2f getNormalizedVector(const Vec2f vec);
Vec2f getNormalizedVector(const cv::Point p1, const cv::Point p2);
Vec2f getPerpendicularVector(const cv::Point p1, const cv::Point p2, const bool toRight = true);
Vec2f getPerpendicularVector(const Vec2f vec, const bool toRight = true);