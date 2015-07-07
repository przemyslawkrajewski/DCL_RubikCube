/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef GETTILESCOLORS_HPP_
#define GETTILESCOLORS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include "Types/Contour.hpp"
#include "Types/Parallelogram.hpp"
#include "Types/CubeFace.hpp"

namespace Processors {
namespace GetTilesColors {

using namespace cv;
using namespace std;
using namespace RubikCube;

/*!
 * \class GetTilesColors
 * \brief GetTilesColors processor class.
 *
 * GetTilesColors processor.
 */
class GetTilesColors: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	GetTilesColors(const std::string & name = "GetTilesColors");

	/*!
	 * Destructor
	 */
	virtual ~GetTilesColors();

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
	void onNewCubeFace();
	
	/// Input data stream
	Base::DataStreamIn <Mat , Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_img;
	Base::DataStreamIn < CubeFace , Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_cubeface;

	/// Output data stream - processed image
	Base::DataStreamOut <CubeFace> out_cubeface;

	Mat img;

};

} //: namespace GetTilesColors
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("GetTilesColors", Processors::GetTilesColors::GetTilesColors)

#endif /* GETTILESCOLORS_HPP_ */
