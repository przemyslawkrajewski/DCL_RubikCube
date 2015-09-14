/*!
 * \file
 * \brief
 * \author Przemo,,,
 */

#include <memory>
#include <string>

#include "FindCubeFace.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace FindCubeFace {

FindCubeFace::FindCubeFace(const std::string & name) :
		Base::Component(name)  {

	angleThresholdGroupSquares=30*CV_PI/180;
	angleThresholdInLine = CV_PI*15/180;
}

FindCubeFace::~FindCubeFace() {
}

void FindCubeFace::prepareInterface() {
	CLOG(LTRACE) << "FindCubeFace::prepareInterface\n";

	registerHandler("onNewParallelograms", boost::bind(&FindCubeFace::onNewParallelograms, this));

	registerStream("in_parallelograms", &in_parallelograms);

	registerStream("out_cubeface", &out_cubeface);

	addDependency("onNewParallelograms", &in_parallelograms);
}

bool FindCubeFace::onInit() {

	return true;
}

bool FindCubeFace::onFinish() {
	return true;
}

bool FindCubeFace::onStop() {
	return true;
}

bool FindCubeFace::onStart() {
	return true;
}

void FindCubeFace::onNewParallelograms()
{
	LOG(LTRACE) << "FindCubeFace::onNewParallelograms\n";
	try
	{
		vector<Parallelogram> parallelograms = in_parallelograms.read();
		vector< vector<Parallelogram> > groupsOfParallelograms;

		groupsOfParallelograms = groupParallelograms(parallelograms);

        vector<CubeFace> cubeFaces;

        cubeFaces = determineCubeFaces(groupsOfParallelograms);

        cubeFaces = fillNullTiles(cubeFaces);

        cubeFaces = rotateCube(cubeFaces);

        if(cubeFaces.size()!=0)
        	out_cubeface.write(cubeFaces[0]);

	} catch (...) {
		LOG(LERROR) << "FindCubeFace::onNewParallelograms failed\n";
	}
}

vector< vector<Parallelogram> > FindCubeFace::groupParallelograms( vector<Parallelogram> parallelograms)
{
	vector< vector<Parallelogram> > groupsOfParallelograms;

	//grouping parallelograms with similar opposite sides and angles
    while(parallelograms.size()!=0)
    {
        //Okreslamy katy nachylenia bokow
        vector<Parallelogram> groupOfParallelograms;
        double a01=measureAngle(parallelograms[0].getCorner(0),parallelograms[0].getCorner(1));
        double a03=measureAngle(parallelograms[0].getCorner(0),parallelograms[0].getCorner(3));
        double r01=measureDistance(parallelograms[0].getCorner(0),parallelograms[0].getCorner(1));
        double r03=measureDistance(parallelograms[0].getCorner(0),parallelograms[0].getCorner(3));

        if(a01<0) a01+=CV_PI;
        if(a03<0) a03+=CV_PI;

        if(a01>CV_PI) a01-=CV_PI;
        if(a03>CV_PI) a03-=CV_PI;

        double distanceThreshold;
        if (r01 < r03) distanceThreshold = r01 / 3;
        else distanceThreshold = r03 / 3;

        groupOfParallelograms.push_back(parallelograms[0]);
        parallelograms.erase(parallelograms.begin());

        //Pakujemy do jednej grupy wszystkie rownolegloboki ktore maja poodbne katy nachylenia
        for(vector<Parallelogram>::iterator j=parallelograms.begin();j!=parallelograms.end();)
        {
            double aj1=measureAngle(j->getCorner(0),j->getCorner(1));
            double aj3= measureAngle(j->getCorner(0),j->getCorner(3));
            double rj1=measureDistance(j->getCorner(0),j->getCorner(1));
            double rj3=measureDistance(j->getCorner(0),j->getCorner(3));
            if(aj1<0) aj1+=CV_PI;
            if(aj3<0) aj3+=CV_PI;

            if(aj1>CV_PI) aj1-=CV_PI;
            if(aj3>CV_PI) aj3-=CV_PI;

            if(  ((abs(a01 - aj1)<angleThresholdGroupSquares && abs(a03 - aj3)<angleThresholdGroupSquares)   ||   ( abs(a03 - aj1)<angleThresholdGroupSquares   &&   abs(a01 - aj3)<angleThresholdGroupSquares)) &&
                 ((abs(r01 - rj1)<distanceThreshold          && abs(r03 - rj3)<distanceThreshold)   ||   ( abs(r03 - rj1)<distanceThreshold   &&   abs(r01 - rj3)<distanceThreshold))   )
            {
                groupOfParallelograms.push_back(*j);
                j=parallelograms.erase(j);
            }
            else j++;
        }

        if(groupOfParallelograms.size()>4)	{groupsOfParallelograms.push_back(groupOfParallelograms);}
    }

    for(int i=0;i<groupsOfParallelograms.size();i++)
    {
    	//std::cout << groupsOfParallelograms[i].size() << "--";
        //Wywalamy równoległoboki które się nakładają
        for(int j=0;j<groupsOfParallelograms[i].size();j++)
        {
            double rj02 = measureDistance(groupsOfParallelograms[i][j].getCorner(0),groupsOfParallelograms[i][j].getCorner(2));
            Point pj = groupsOfParallelograms[i][j].getMiddle();
            double distanceThreshold =rj02/10;
            for(int l=0;l<groupsOfParallelograms[i].size();)
            {
                Point pl = groupsOfParallelograms[i][l].getMiddle();
                double rjl = measureDistance(pj,pl);
                if(rjl < distanceThreshold && j!=l) groupsOfParallelograms[i].erase(groupsOfParallelograms[i].begin()+l);
                else l++;
            }
        }
        //std::cout << groupsOfParallelograms[i].size() << "\n";
    }//*/
//std::cout << "\n";
    return groupsOfParallelograms;
}

vector<CubeFace> FindCubeFace::determineCubeFaces( vector< vector<Parallelogram> > groupsOfParallelograms )
{
    vector<CubeFace> cubeFaces;

    //Przepisywanie z zbioru nieregularnych kwadratow w zbior pol kostki
    for(vector< vector<Parallelogram> >::iterator group=groupsOfParallelograms.begin();group!=groupsOfParallelograms.end();group++)
    {
        CubeFace face;
        int indexesOfParallelogramsInline1[5];
        int indexesOfParallelogramsInline2[5];
        int parallelogramsInLine1=0;
        int parallelogramsInLine2=0;
        for(int j=0;j<group->size();j++)
        {
            int tmpIndexesInLine1[5];
            int tmpInLine1=1;
            int tmpIndexesInLine2[5];
            int tmpInLine2=1;

            for(int l=0;l<5;l++) tmpIndexesInLine1[l]=-1;
            for(int l=0;l<5;l++) tmpIndexesInLine2[l]=-1;
            tmpIndexesInLine1[2]=j;
            tmpIndexesInLine2[2]=j;

            double aj01=measureAngle((*group)[j].getCorner(0),(*group)[j].getCorner(1));
            double aj03=measureAngle((*group)[j].getCorner(0),(*group)[j].getCorner(3));
            for(int l=0;l<(*group).size();l++)
            {
                double ajl=measureAngle((*group)[j].getMiddle(),(*group)[l].getMiddle());
                if(j!=l && abs(aj01-ajl)<angleThresholdInLine)
                {
                    tmpInLine1++;
                    if(tmpIndexesInLine1[3]==-1) tmpIndexesInLine1[3]=l;
                    else
                    {
                        double r1 = measureDistance((*group)[tmpIndexesInLine1[2]].getMiddle(),(*group)[tmpIndexesInLine1[3]].getMiddle());
                        double r2 = measureDistance((*group)[tmpIndexesInLine1[2]].getMiddle(),(*group)[l].getMiddle());
                        if(r1>r2)
                        {
                            tmpIndexesInLine1[4]=tmpIndexesInLine1[3];
                            tmpIndexesInLine1[3]=l;
                        }
                        else tmpIndexesInLine1[4]=l;
                    }
                }
                else if(j!=l && abs(abs(aj01-ajl)-CV_PI)<angleThresholdInLine)
                {
                    tmpInLine1++;
                    if(tmpIndexesInLine1[1]==-1) tmpIndexesInLine1[1]=l;
                    else
                    {
                        double r1 = measureDistance((*group)[tmpIndexesInLine1[2]].getMiddle(),(*group)[tmpIndexesInLine1[1]].getMiddle());
                        double r2 = measureDistance((*group)[tmpIndexesInLine1[2]].getMiddle(),(*group)[l].getMiddle());
                        if(r1>r2)
                        {
                            tmpIndexesInLine1[0]=tmpIndexesInLine1[1];
                            tmpIndexesInLine1[1]=l;
                        }
                        else tmpIndexesInLine1[0]=l;
                    }
                }

                if(j!=l && abs(aj03-ajl)<angleThresholdInLine )
                {
                    tmpInLine2++;
                    if(tmpIndexesInLine2[3]==-1) tmpIndexesInLine2[3]=l;
                    else
                    {
                        double r1 = measureDistance((*group)[tmpIndexesInLine2[2]].getMiddle(),(*group)[tmpIndexesInLine2[3]].getMiddle());
                        double r2 = measureDistance((*group)[tmpIndexesInLine2[2]].getMiddle(),(*group)[l].getMiddle());
                        if(r1>r2)
                        {
                            tmpIndexesInLine2[4]=tmpIndexesInLine2[3];
                            tmpIndexesInLine2[3]=l;
                        }
                        else tmpIndexesInLine2[4]=l;
                    }

                }
                else if(j!=l && abs(abs(aj03-ajl)-CV_PI)<angleThresholdInLine)
                {
                    tmpInLine2++;
                    if(tmpIndexesInLine2[1]==-1) tmpIndexesInLine2[1]=l;
                    else
                    {
                        double r1 = measureDistance((*group)[tmpIndexesInLine2[2]].getMiddle(),(*group)[tmpIndexesInLine2[1]].getMiddle());
                        double r2 = measureDistance((*group)[tmpIndexesInLine2[2]].getMiddle(),(*group)[l].getMiddle());
                        if(r1>r2)
                        {
                            tmpIndexesInLine2[0]=tmpIndexesInLine2[1];
                            tmpIndexesInLine2[1]=l;
                        }
                        else tmpIndexesInLine2[0]=l;
                    }
                }
            }

            if(parallelogramsInLine1<=tmpInLine1 && parallelogramsInLine2<=tmpInLine2)
            {
                for(int l=0;l<5;l++) indexesOfParallelogramsInline1[l]=tmpIndexesInLine1[l];
                for(int l=0;l<5;l++) indexesOfParallelogramsInline2[l]=tmpIndexesInLine2[l];
                parallelogramsInLine1=tmpInLine1;
                parallelogramsInLine2=tmpInLine2;
            }
            if(parallelogramsInLine1==3 && parallelogramsInLine2==3) break;
        }
        //std::cout << group->size() << "  "<< parallelogramsInLine1 << "  " <<  parallelogramsInLine2 << "\n";
        if(parallelogramsInLine1==3 && parallelogramsInLine2==3)
        {
            int min1,min2;

            if(indexesOfParallelogramsInline1[0]!=-1) min1=0;
            else if (indexesOfParallelogramsInline1[1]!=-1) min1=1;
            else min1=2;

            if(indexesOfParallelogramsInline2[0]!=-1) min2=0;
            else if (indexesOfParallelogramsInline2[1]!=-1) min2=1;
            else min2=2;

            for(int l=min1;l<min1+3;l++) face.setTile((*group)[indexesOfParallelogramsInline1[l]],2-(l-min1),2-min2);
            for(int l=min2;l<min2+3;l++) face.setTile((*group)[indexesOfParallelogramsInline2[l]],2-(2-min1),l-min2);

            face.setHorizontalSize(3);
            face.setVerticalSize(3);
            face.setFound();
            cubeFaces.push_back(face);
        }

    }//*/
    return cubeFaces;
}

vector<CubeFace> FindCubeFace::fillNullTiles(vector<CubeFace> cubeFaces)
{
    for(vector<CubeFace>::iterator i=cubeFaces.begin();i!=cubeFaces.end();i++)
    {
        int indexOfFilledLineX=-3;
        int indexOfFilledLineY=-3;
        for(int j=0;j<3;j++)
        {
            if(i->getTile(j,0).getMiddle().x!=0 && i->getTile(j,1).getMiddle().x!=0)
            {
                indexOfFilledLineX=j;
                break;
            }
        }
        for(int j=0;j<3;j++)
        {
            if(i->getTile(0,j).getMiddle().x!=0 && i->getTile(1,j).getMiddle().x!=0)
            {
                indexOfFilledLineY=j;
                break;
            }
        }
        if(indexOfFilledLineX==-3 || indexOfFilledLineY==-3)
        {
            i=cubeFaces.erase(i);
            i--;
            continue;
        }
        Point vectorBetweenCorners[4];
        for(int j=0;j<4;j++)
        {
            vectorBetweenCorners[j].x=i->getTile(indexOfFilledLineX,indexOfFilledLineY).getCorner(j).x-i->getTile(indexOfFilledLineX,indexOfFilledLineY).getMiddle().x;
            vectorBetweenCorners[j].y=i->getTile(indexOfFilledLineX,indexOfFilledLineY).getCorner(j).y-i->getTile(indexOfFilledLineX,indexOfFilledLineY).getMiddle().y;
        }
        Point vectorBetweenTiles[3];
        for(int j=0;j<3;j++)
        {
            if(j==indexOfFilledLineY) continue;
            vectorBetweenTiles[j];
            vectorBetweenTiles[j].x=i->getTile(indexOfFilledLineX,j).getMiddle().x-i->getTile(indexOfFilledLineX,indexOfFilledLineY).getMiddle().x;
            vectorBetweenTiles[j].y=i->getTile(indexOfFilledLineX,j).getMiddle().y-i->getTile(indexOfFilledLineX,indexOfFilledLineY).getMiddle().y;
        }
        for(int j=0;j<3;j++)
        {
            if(j==indexOfFilledLineX) continue;
            for(int l=0;l<3;l++)
            {
                if(l==indexOfFilledLineY) continue;
                Parallelogram parallelogram;
                Point middle;
                middle.x=i->getTile(j,indexOfFilledLineY).getMiddle().x+vectorBetweenTiles[l].x;
                middle.y=i->getTile(j,indexOfFilledLineY).getMiddle().y+vectorBetweenTiles[l].y;
                parallelogram.setMiddle(middle);
                for(int k=0;k<4;k++)
                {
                    Point corner;
                    corner.x=middle.x+vectorBetweenCorners[k].x;
                    corner.y=middle.y+vectorBetweenCorners[k].y;
                    parallelogram.setCorner(corner,k);
                }
                i->setTile(parallelogram,j,l);
            }
        }
    }
    return cubeFaces;
}

vector<CubeFace> FindCubeFace::rotateCube(vector<CubeFace> cubeFaces)
{
    for(vector<CubeFace>::iterator i=cubeFaces.begin();i!=cubeFaces.end();i++)
    {
    	double a = measureAngle(i->getTile(1,1).getMiddle(),i->getTile(0,0).getMiddle());

    	if(a>CV_PI/2)
    	{
    		Parallelogram t;
    		t=i->getTile(2,2);
    		i->setTile(i->getTile(0,2),2,2);
    		i->setTile(i->getTile(0,0),0,2);
    		i->setTile(i->getTile(2,0),0,0);
    		i->setTile(t,2,0);

    		t=i->getTile(2,1);
    		i->setTile(i->getTile(1,2),2,1);
    		i->setTile(i->getTile(0,1),1,2);
    		i->setTile(i->getTile(1,0),0,1);
    		i->setTile(t,1,0);//*/

    	}
    	else if(a>0)
    	{
    		Parallelogram t;
    		t=i->getTile(2,2);
    		i->setTile(i->getTile(0,2),2,2);
    		i->setTile(i->getTile(0,0),0,2);
    		i->setTile(i->getTile(2,0),0,0);
    		i->setTile(t,2,0);

    		t=i->getTile(2,1);
    		i->setTile(i->getTile(1,2),2,1);
    		i->setTile(i->getTile(0,1),1,2);
    		i->setTile(i->getTile(1,0),0,1);
    		i->setTile(t,1,0);//*/
    		t=i->getTile(2,2);
    		i->setTile(i->getTile(0,2),2,2);
    		i->setTile(i->getTile(0,0),0,2);
    		i->setTile(i->getTile(2,0),0,0);
    		i->setTile(t,2,0);

    		t=i->getTile(2,1);
    		i->setTile(i->getTile(1,2),2,1);
    		i->setTile(i->getTile(0,1),1,2);
    		i->setTile(i->getTile(1,0),0,1);
    		i->setTile(t,1,0);//*/

    	}
    	else if(a>-CV_PI/2)
    	{
    		Parallelogram t;
    		t=i->getTile(2,0);
    		i->setTile(i->getTile(0,0),2,0);
    		i->setTile(i->getTile(0,2),0,0);
    		i->setTile(i->getTile(2,2),0,2);
    		i->setTile(t,2,2);

    		t=i->getTile(1,0);
    		i->setTile(i->getTile(0,1),1,0);
    		i->setTile(i->getTile(1,2),0,1);
    		i->setTile(i->getTile(2,1),1,2);
    		i->setTile(t,2,1);//*/
    	}
    }

    //Weryfikacja
    for(vector<CubeFace>::iterator i=cubeFaces.begin();i!=cubeFaces.end();)
    {
    	double r1 = measureDistance(i->getTile(0,0).getMiddle(),i->getTile(0,1).getMiddle());
    	double r2 = measureDistance(i->getTile(0,1).getMiddle(),i->getTile(0,2).getMiddle());
    	double r3 = measureDistance(i->getTile(0,0).getMiddle(),i->getTile(1,0).getMiddle());
    	double r4 = measureDistance(i->getTile(1,0).getMiddle(),i->getTile(2,0).getMiddle());
    	double d = (r1+r2+r3+r4)/(4*4);
    	if(abs(r1-r2)>d || abs(r1-r3)>d || abs(r1-r4)>d || abs(r2-r3)>d  || abs(r2-r4)>d || abs(r3-r4)>d)
    	{
    		i=cubeFaces.erase(i);
    		//std::cout << "Nah!\n";
    	}
    	else i++;
    }
    //Weryfikacja2
    for(vector<CubeFace>::iterator i=cubeFaces.begin();i!=cubeFaces.end();)
    {
    	double r[8];
    	r[0]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(0,0).getMiddle());
    	r[1]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(0,1).getMiddle());
    	r[2]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(0,2).getMiddle());
    	r[3]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(1,0).getMiddle());
    	r[4]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(2,0).getMiddle());
    	r[5]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(1,2).getMiddle());
    	r[6]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(2,1).getMiddle());
    	r[7]=  measureDistance(i->getTile(1,1).getMiddle(),i->getTile(2,2).getMiddle());

    	double d=0;
    	for (int j=0;j<8;j++) d+=r[j];
    	d = d/(8*2);
    	if(
    			abs(r[0]-r[2])>d || abs(r[0]-r[4])>d || abs(r[0]-r[7])>d || abs(r[2]-r[4])>d  || abs(r[2]-r[7])>d || abs(r[4]-r[7])>d ||
    			abs(r[1]-r[3])>d || abs(r[1]-r[5])>d || abs(r[1]-r[6])>d || abs(r[3]-r[5])>d  || abs(r[3]-r[6])>d || abs(r[5]-r[6])>d
    	  )
    	{
    		i=cubeFaces.erase(i);
    		//std::cout << "Nah!";
    	}
    	else i++;
    }//*/
    return cubeFaces;
}

double FindCubeFace::measureDistance(Point p1, Point p2)
{
	return sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
}
double FindCubeFace::measureAngle(Point p1, Point p2)
{
	return atan2(p2.y - p1.y, p2.x - p1.x);
}

} //: namespace FindCubeFace
} //: namespace Processors
