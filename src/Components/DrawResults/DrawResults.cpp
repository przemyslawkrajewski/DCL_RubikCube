/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "DrawResults.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DrawResults {

DrawResults::DrawResults(const std::string & name) : Base::Component(name),
		contourColor(255,0,255)
{

}

DrawResults::~DrawResults() {
}

void DrawResults::prepareInterface() {
	CLOG(LTRACE) << "DrawResults::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&DrawResults::onNewImage, this));
	registerHandler("onNewContours", boost::bind(&DrawResults::onNewContours, this));
	registerHandler("onNewParallelograms", boost::bind(&DrawResults::onNewParallelograms, this));
	registerHandler("onNewCubeFace", boost::bind(&DrawResults::onNewCubeFace, this));

	registerStream("in_img", &in_img);
	registerStream("in_contours", &in_contours);
	registerStream("in_parallelograms", &in_parallelograms);
	registerStream("in_cubeface", &in_cubeface);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
	addDependency("onNewContours", &in_contours);
	addDependency("onNewParallelograms", &in_parallelograms);
	addDependency("onNewCubeFace", &in_cubeface);
}

bool DrawResults::onInit() {

	return true;
}

bool DrawResults::onFinish() {
	return true;
}

bool DrawResults::onStop() {
	return true;
}

bool DrawResults::onStart() {
	return true;
}

void DrawResults::onNewContours()
{
	LOG(LTRACE) << "DrawResults::onNewContours\n";
	try {
		contours.clear();
		contours = in_contours.read();
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewContours failed\n";
	}
}

void DrawResults::onNewParallelograms()
{
	LOG(LTRACE) << "DrawResults::onNewParallelograms\n";
	try {
		parallelograms.clear();
		parallelograms = in_parallelograms.read();
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewParallelograms failed\n";
	}
}

void DrawResults::onNewCubeFace()
{
	LOG(LTRACE) << "DrawResults::onNewCubeFace\n";
	try {
		cubeFace = in_cubeface.read();
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewCubeFace failed\n";
	}
}

void DrawResults::drawContours(Mat &img)
{
	for(vector<Contour>::iterator contour=contours.begin();contour!=contours.end();contour++)
	{
		for(Contour::iterator point=contour->begin();point!=contour->end();point++)
		{
			RGB& rgb = img.ptr<RGB>(point->y)[point->x];
			rgb=contourColor;
			//line( out,contours[i][j],contours[i][j+1],Scalar( 255, 255, 255 ),2,8 );
		}
	}
}

void DrawResults::drawParallelograms(Mat &img)
{
	for(vector<Parallelogram>::iterator p=parallelograms.begin();p!=parallelograms.end();p++)
	{
		cv::line( img,p->getCorner(0),p->getCorner(1),Scalar( 100, 100, 100 ),4 );
		cv::line( img,p->getCorner(1),p->getCorner(2),Scalar( 100, 100, 100 ),4 );
		cv::line( img,p->getCorner(2),p->getCorner(3),Scalar( 100, 100, 100 ),4 );
		cv::line( img,p->getCorner(3),p->getCorner(0),Scalar( 100, 100, 100 ),4 );
	}
}

void DrawResults::drawCubeFace(Mat &img)
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			Parallelogram p = cubeFace.getTile(i,j);
			cv::line( img,p.getCorner(0),p.getCorner(1),Scalar( 255, 255, 255 ),2 );
			cv::line( img,p.getCorner(1),p.getCorner(2),Scalar( 255, 255, 255 ),2 );
			cv::line( img,p.getCorner(2),p.getCorner(3),Scalar( 255, 255, 255 ),2 );
			cv::line( img,p.getCorner(3),p.getCorner(0),Scalar( 255, 255, 255 ),2 );
			cv::circle( img,p.getMiddle(),6,Scalar( 255, 255, 255 ), 2);
		}
	}
}

void DrawResults::drawTilesColors(Mat &img)
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cv::Scalar c = cubeFace.getTile(i,j).getColor();
			cv::rectangle(img, Point(i*50+10,j*50+10), Point(i*50+50,j*50+50), c,-1,8,0);
		}
	}
}

void DrawResults::onNewImage()
{
	LOG(LTRACE) << "DrawResults::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out = img.clone();

		//drawContours(out);
		//drawParallelograms(out);
		drawCubeFace(out);
		drawTilesColors(out);

		contours.clear();
		parallelograms.clear();
		cubeFace= CubeFace();

		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "DrawResults	::onNewImage failed\n";
	}
}

} //: namespace DrawResults
} //: namespace Processors
