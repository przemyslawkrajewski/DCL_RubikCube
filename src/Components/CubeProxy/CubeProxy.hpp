/*!
 * \file
 * \brief 
 * \author Przemo,,,
 */

#ifndef CUBEPROXY_HPP_
#define CUBEPROXY_HPP_

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

#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include "rubik_cube/Cube_face_color.h"

using namespace std;
using namespace cv;
using namespace RubikCube;

namespace Processors {
namespace CubeProxy {

/*!
 * \class CubeProxy
 * \brief CubeProxy processor class.
 *
 * CubeProxy processor.
 */
class CubeProxy: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CubeProxy(const std::string & name = "CubeProxy");

	/*!
	 * Destructor
	 */
	virtual ~CubeProxy();

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


	// Input data streams

	Base::DataStreamIn<CubeFace , Base::DataStreamBuffer::Newest,Base::Synchronization::Mutex> in_cubeface;

	// Output data streams

	// Handlers
	Base::EventHandler2 h_onNewCubeFace;
	Base::Property<std::string> ros_topic_name;
	Base::Property<std::string> ros_namespace;


	// Handlers
	void onNewCubeFace();

	ros::Publisher pub;
	ros::Subscriber sub;
	ros::NodeHandle * nh;

	void callback(const rubik_cube::Cube_face_colorConstPtr& msg);

};

} //: namespace CubeProxy
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CubeProxy", Processors::CubeProxy::CubeProxy)

#endif /* CUBEPROXY_HPP_ */
