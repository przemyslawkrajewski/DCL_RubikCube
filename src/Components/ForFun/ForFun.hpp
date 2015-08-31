/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef FORFUN_HPP_
#define FORFUN_HPP_

#include <vector>

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/calib3d/calib3d.hpp"

namespace Processors {
namespace ForFun {

using namespace cv;

/*!
 * \class ForFun
 * \brief ForFun processor class.
 *
 * ForFun processor.
 */
class ForFun: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ForFun(const std::string & name = "ForFun");

	/*!
	 * Destructor
	 */
	virtual ~ForFun();

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

} //: namespace ForFun
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ForFun", Processors::ForFun::ForFun)

#endif /* FORFUN_HPP_ */
