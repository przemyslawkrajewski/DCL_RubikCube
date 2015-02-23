/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef CVMEDIANBLUR_HPP_
#define CVMEDIANBLUR_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace Processors {
namespace CvMedianBlur {

using namespace cv;

/*!
 * \class CvMedianBlur
 * \brief CvMedianBlur processor class.
 *
 * CvMedianBlur processor.
 */
class CvMedianBlur: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CvMedianBlur(const std::string & name = "CvMedianBlur");

	/*!
	 * Destructor
	 */
	virtual ~CvMedianBlur();

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
	
	//Size of the filter
	Base::Property<int> kernelSize;


};

} //: namespace CvMedianBlur
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CvMedianBlur", Processors::CvMedianBlur::CvMedianBlur)

#endif /* CVMEDIANBLUR_HPP_ */
