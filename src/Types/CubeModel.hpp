/*
 * CubeModel.hpp
 *
 *  Created on: 15 lip 2015
 *      Author: pkrajews
 */

#ifndef CUBEMODEL_HPP_
#define CUBEMODEL_HPP_

#include "Types/Objects3D/Object3D.hpp"

class CubeModel: public Types::Objects3D::Object3D
{
public:
	CubeModel() :
		Types::Objects3D::Object3D()
	{
	}

	CubeModel(const CubeModel& o)
	{
		imagePointsSet = o.imagePointsSet;
		modelPointsSet = o.imagePointsSet;
		positionSet = o.positionSet;
		imagePoints = o.imagePoints;
		modelPoints = o.modelPoints;
//		position = o.position;
	}

	virtual void draw(cv::Mat& image, cv::Scalar color, int offsetX = 0, int offsetY = 0)
	{

	}

	virtual ~CubeModel()
	{
	}
	virtual Types::Objects3D::Object3D* clone()
	{
		CubeModel* p = new CubeModel();
		p->setModelPoints(this->getModelPoints());
		p->setImagePoints(this->getImagePoints());
		return p;
	}
};



#endif /* CUBEMODEL_HPP_ */
