/*
 * CubeFace.hpp
 *
 *  Created on: 12-05-2015
 *      Author: przemo
 */

#ifndef CUBEFACE_HPP_
#define CUBEFACE_HPP_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include "Parallelogram.hpp"

namespace RubikCube
{
	class CubeFace
	{
	private:
		Parallelogram tiles[3][3];
		int horizontalSize;
		int verticalSize;
		cv::Point corners[4];

	public:
		CubeFace()
		{
			horizontalSize=0;
			verticalSize=0;
			for(int i=0;i<4;i++) corners[i]=cv::Point(0,0);
		}

		int getHorizontalSize() {return horizontalSize;}
		void setHorizontalSize(int horizontalSize) {this->horizontalSize = horizontalSize;}
		int getVerticalSize() {return verticalSize;}
		void setVerticalSize(int verticalSize) {this->verticalSize = verticalSize;}

		Parallelogram getTile(int number1,int number2)
		{
			return tiles[number1][number2];
		}

		void setTile(Parallelogram tile,int number1,int number2)
		{
			tiles[number1][number2] = tile;
		}

		cv:: Point getCorner(int number) {
			return corners[number];
		}

		void setCorner(cv::Point point,int number)
		{
			corners[number].x = point.x;
			corners[number].y = point.y;
		}

		//public void measureCorners
	};

}
#endif /* CUBEFACE_HPP_ */
