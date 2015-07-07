/*!
 * \file
 * \brief 
 * \author Przemysław Krajewski
 */

#ifndef CVEQUALIZEHISTOGRAM_HPP_
#define CVEQUALIZEHISTOGRAM_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//#include "Types/stream_OpenCV.hpp"

namespace Processors {
namespace CvEqualizeHistogram {

using namespace cv;

/*!
 * \class CvEqualizeHistogram
 * \brief CvEqualizeHistogram processor class.
 *
 * CvEqualizeHistogram processor.
 */
class CvEqualizeHistogram: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvEqualizeHistogram(const std::string & name = "CvEqualizeHistogram");

	/*!
	 * Destructor
	 */
	virtual ~CvEqualizeHistogram();

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
	Base::DataStreamIn <Mat, Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_img;

	/// Output data stream - processed image
	Base::DataStreamOut <Mat> out_img;
	

};

} //: namespace CvEqualizeHistogram
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvEqualizeHistogram", Processors::CvEqualizeHistogram::CvEqualizeHistogram)

#endif /* CVEQUALIZEHISTOGRAM_HPP_ */
