/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "ForFun.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace ForFun {

ForFun::ForFun(const std::string & name) :
		Base::Component(name)  {

}

ForFun::~ForFun() {
}

void ForFun::prepareInterface()
{
	CLOG(LTRACE) << "ForFun::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&ForFun::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);

}

bool ForFun::onInit() {

	return true;
}

bool ForFun::onFinish() {
	return true;
}

bool ForFun::onStop() {
	return true;
}

bool ForFun::onStart() {
	return true;
}

void ForFun::onNewImage()
{
	LOG(LTRACE) << "ForFun::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();
		cv::Mat gray = img.clone();
		cvtColor( out, gray, CV_RGB2GRAY );

		Size patternsize(5,4);
		vector<Point2f> centers;
		std::vector <cv::Point3f> modelPoints;



		Size imageSize = Size(img.cols, img.rows);
		 std:;vector< Mat> rvecs, tvecs;
		 Mat cameraMat(3,3, CV_64F);
		 Mat distCoeffs(8,1, CV_64F);

		 modelPoints.clear();
		for (int i=0;i<5;i++)
			for(int j=0;j<4;j++)
			{
				cv::Point3f p((i)*0.025,(j)*0.025,0);
				modelPoints.push_back(p);
				//imagePoints.push_back(cubeFace.getTile(i,j).getMiddle());
			}

		vector< vector< Point2f> > imageP;  imageP.push_back(centers);
		vector< vector< Point3f> > objectP;  objectP.push_back(modelPoints);

		bool patternfound = findCirclesGrid(gray, patternsize, centers);
		drawChessboardCorners(out, patternsize, Mat(centers), patternfound);

		calibrateCamera(objectP,imageP,imageSize,cameraMat,distCoeffs,rvecs,tvecs);

		//Laplacian( out, out,  CV_16S, 3, 1, 0, BORDER_DEFAULT );
		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "ForFun::onNewImage failed\n";
	}
}

} //: namespace ForFun
} //: namespace Processors
