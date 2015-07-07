/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef FINDCONTOURS_HPP_
#define FINDCONTOURS_HPP_

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
namespace FindContours {

using namespace cv;
using namespace std;
using namespace RubikCube;

/*!
 * \class FindContours
 * \brief FindContours processor class.
 *
 * FindContours processor.
 */
class FindContours: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FindContours(const std::string & name = "FindContours");

	/*!
	 * Destructor
	 */
	virtual ~FindContours();

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

	/// Input data stream
	Base::DataStreamIn <Mat , Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_img;

	/// Output data stream - found contours
	Base::DataStreamOut < vector<Contour> > out_contours;
	

};

} //: namespace FindContours
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FindContours", Processors::FindContours::FindContours)

#endif /* FINDCONTOURS_HPP_ */
