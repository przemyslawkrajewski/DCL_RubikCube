/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef DRAWRESULTS_HPP_
#define DRAWRESULTS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include "Types/Contour.hpp"

namespace Processors {
namespace DrawResults {

struct RGB
{
	RGB() {blue=green=red=0;}
	RGB(uchar b,uchar g,uchar r): blue(b),green(g),red(r)
	{}
    uchar blue;
    uchar green;
    uchar red;
};

using namespace cv;
using namespace std;
using namespace RubikCube;

/*!
 * \class DrawResults
 * \brief DrawResults processor class.
 *
 * DrawResults processor.
 */
class DrawResults: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	DrawResults(const std::string & name = "DrawResults");

	/*!
	 * Destructor
	 */
	virtual ~DrawResults();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();

	/*!
	 * Event handler function.
	 */
	void onNewImage();
	void onNewContours();

	/*!
	 * subfunctions drawing stuff
	 */
	void drawContours(Mat &img);

	/// Input data stream
	Base::DataStreamIn <Mat> in_img;
	Base::DataStreamIn < vector<Contour> > in_contours;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	//colors of things, set in constructor
	RGB contourColor;

	//things to draw
	vector<Contour> contours;

};

} //: namespace DrawResults
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DrawResults", Processors::DrawResults::DrawResults)

#endif /* DRAWRESULTS_HPP_ */
