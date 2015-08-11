/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "CubeProxy.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CubeProxy {

CubeProxy::CubeProxy(const std::string & name) :
		Base::Component(name) ,
		ros_topic_name("ros.topic_name", std::string("cube_face_color")),
		ros_namespace("ros.namespace", std::string("discode"))
{
		registerProperty(ros_topic_name);
		registerProperty(ros_namespace);
}

CubeProxy::~CubeProxy() {
}

void CubeProxy::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_cubeface", &in_cubeface);
	registerStream("in_homogMatrix", &in_homogMatrix);

	// Register handlers
	h_onNewCubeFace.setup(boost::bind(&CubeProxy::onNewCubeFace, this));
	registerHandler("onNewCubeFace", &h_onNewCubeFace);
	addDependency("onNewCubeFace", &in_cubeface);

	h_onNewHomoMatrix.setup(boost::bind(&CubeProxy::onNewHomoMatrix, this));
	registerHandler("onNewHomoMatrix", &h_onNewHomoMatrix);
	addDependency("onNewHomoMatrix", &in_homogMatrix);

}

bool CubeProxy::onInit()
{
	static char * tmp = NULL;
	static int tmpi;
	ros::init(tmpi, &tmp, std::string(ros_namespace), ros::init_options::NoSigintHandler);
	nh = new ros::NodeHandle;
	pub = nh->advertise<rubik_cube::Cube_face_color>(ros_topic_name, 1000);
	//sub = nh->subscribe("cube_face_color", 1, &CubeProxy::callback, this);
	return true;
}

bool CubeProxy::onFinish() {
	return true;
}

bool CubeProxy::onStop() {
	return true;
}

bool CubeProxy::onStart() {
	return true;
}

void CubeProxy::publish()
{
	if(cube.isFound() &&  (cubePosition.x!=0 || cubePosition.y!=0))
	{
		rubik_cube::Cube_face_color msg;

		cv::Point p1 = cube.getTile(0,0).getMiddle();
		cv::Point p2 = cube.getTile(2,2).getMiddle();

		msg.tile1.red    = cube.getTile(0,0).getColor()[2];
		msg.tile1.green  = cube.getTile(0,0).getColor()[1];
		msg.tile1.blue   = cube.getTile(0,0).getColor()[0];
		msg.tile2.red    = cube.getTile(0,1).getColor()[2];
		msg.tile2.green  = cube.getTile(0,1).getColor()[1];
		msg.tile2.blue   = cube.getTile(0,1).getColor()[0];
		msg.tile3.red    = cube.getTile(0,2).getColor()[2];
		msg.tile3.green  = cube.getTile(0,2).getColor()[1];
		msg.tile3.blue   = cube.getTile(0,2).getColor()[0];
		msg.tile4.red    = cube.getTile(1,0).getColor()[2];
		msg.tile4.green  = cube.getTile(1,0).getColor()[1];
		msg.tile4.blue   = cube.getTile(1,0).getColor()[0];
		msg.tile5.red    = cube.getTile(1,1).getColor()[2];
		msg.tile5.green  = cube.getTile(1,1).getColor()[1];
		msg.tile5.blue   = cube.getTile(1,1).getColor()[0];
		msg.tile6.red    = cube.getTile(1,2).getColor()[2];
		msg.tile6.green  = cube.getTile(1,2).getColor()[1];
		msg.tile6.blue   = cube.getTile(1,2).getColor()[0];
		msg.tile7.red    = cube.getTile(2,0).getColor()[2];
		msg.tile7.green  = cube.getTile(2,0).getColor()[1];
		msg.tile7.blue   = cube.getTile(2,0).getColor()[0];
		msg.tile8.red    = cube.getTile(2,1).getColor()[2];
		msg.tile8.green  = cube.getTile(2,1).getColor()[1];
		msg.tile8.blue   = cube.getTile(2,1).getColor()[0];
		msg.tile9.red    = cube.getTile(2,2).getColor()[2];
		msg.tile9.green  = cube.getTile(2,2).getColor()[1];
		msg.tile9.blue   = cube.getTile(2,2).getColor()[0];
		msg.x = cubePosition.x;
		msg.y = cubePosition.y;
		msg.z_rot = atan2(p2.y - p1.y, p2.x - p1.x);

		pub.publish(msg);
		ros::spinOnce();

		cube = CubeFace();
		cubePosition.x = cubePosition.y = 0;
	}
}

void CubeProxy::onNewCubeFace()
{
	cube = in_cubeface.read();

	publish();
}

void CubeProxy::onNewHomoMatrix()
{
	Types::HomogMatrix hm;
	hm = in_homogMatrix.read();
	//std::cout << hm.elements[0][0] << "  " << hm.elements[0][1] << "  " << hm.elements[0][2] << "\n";
	//std::cout << hm.elements[1][0] << "  " << hm.elements[1][1] << "  " << hm.elements[1][2] << "\n";
	//std::cout << hm.elements[2][0] << "  " << hm.elements[2][1] << "  " << hm.elements[2][2] << "\n";

	cubePosition.x = hm.elements[0][3]*1000;
	cubePosition.y = hm.elements[1][3]*1000;

	publish();
}

void CubeProxy::callback(const rubik_cube::Cube_face_colorConstPtr& msg) {
	CLOG(LNOTICE) << "Received from ROS: " << msg->tile1.red;
}


} //: namespace CubeProxy
} //: namespace Processors
