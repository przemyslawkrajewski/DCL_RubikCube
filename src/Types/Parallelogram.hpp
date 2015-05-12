/*
 * Parallelogram.hpp
 *
 *  Created on: 12-05-2015
 *      Author: przemo
 */

#ifndef PARALLELOGRAM_HPP_
#define PARALLELOGRAM_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>

class Parallelogram
{
private:
	cv::Point middle;
	std::vector<cv::Point> corners;

	cv::Scalar color;

public:
	Parallelogram()
	{
		corners.clear();
		for(int i=0;i<4;i++) corners.push_back(cv::Point(0,0));
		middle = cv::Point(0,0);
	}
    cv::Point getMiddle()
    {
      if(middle.x==0 && middle.y==0)
      {
          middle.x=corners[0].x+corners[1].x+corners[2].x+corners[3].x;
          middle.y=corners[0].y+corners[1].y+corners[2].y+corners[3].y;
          middle.x=middle.x/4;
          middle.y=middle.y/4;
      }
      return middle;
    }
    void setMiddle(cv::Point middle)
    {
        this->middle.x = middle.x;
        this->middle.y = middle.y;
    }

    cv::Point getCorner( int number)
    {
        return corners[number];
    }
    void setCorner(cv::Point corner, int number)
    {
        corners[number].x=corner.x;
        corners[number].y=corner.y;
    }

    void calculateMiddle()
    {
        middle.x=corners[0].x+corners[1].x+corners[2].x+corners[3].x;
        middle.y=corners[0].y+corners[1].y+corners[2].y+corners[3].y;
        middle.x=middle.x/4;
        middle.y=middle.y/4;
    }

    cv::Scalar getColor() {
        return color;
    }

    void setColor(cv::Scalar color) {
        this->color = color;
    }

};


#endif /* PARALLELOGRAM_HPP_ */
