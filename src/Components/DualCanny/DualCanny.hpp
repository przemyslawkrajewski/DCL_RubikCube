/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef DUALCANNY_HPP_
#define DUALCANNY_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Processors {
namespace DualCanny {

using namespace cv;

/*!
 * \class DualCanny
 * \brief DualCanny processor class.
 *
 * DualCanny processor.
 */
class DualCanny: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	DualCanny(const std::string & name = "DualCanny");

	/*!
	 * Destructor
	 */
	virtual ~DualCanny();

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
	Base::DataStreamIn <Mat> in_img;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;

	//Size of the dilation linking lines
	Base::Property<int> dilationSize;

	//Size of the erosion making lines thinner
	Base::Property<int> erosionSize;


	//Thresholds for Canny
	Base::Property<int> lowerThreshold;
	Base::Property<int> higherThreshold;

};

} //: namespace DualCanny
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DualCanny", Processors::DualCanny::DualCanny)

#endif /* DUALCANNY_HPP_ */
