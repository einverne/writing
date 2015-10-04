#include "Character.h"


Character::Character(void)
{
	bujianCount = 0;
	fontSize = 512;
}

Character::~Character(void)
{
	bujianList.clear();
}

bool Character::addBujian(Bujian bujian){
	
	vector<Bujian>::iterator it = bujianList.end();
	bujianList.insert(it,bujian);
	bujianCount++;
	return true;
}

//////////////////////////////////////////////////////////////////////////
//获取包围盒大小
CCSize Character::getBox(){
	float xmin=1000000,ymin=1000000;
	float xmax=0,ymax=0;
	vector<Bujian>::iterator iter = bujianList.begin();
	for (iter;iter != bujianList.end(); ++iter)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (stro_iter; stro_iter != strokeList.end(); ++stro_iter)
		{
			Stroke stroke = (Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.getpointList();
			vector<CCPoint>::iterator point_iter = pointList.begin();
			for (point_iter; point_iter != pointList.end() ; ++point_iter)
			{
				CCPoint point = (CCPoint)*point_iter;
				if (xmin>point.x)
				{
					xmin = point.x;
				}
				if (xmax<point.x)
				{
					xmax = point.x;
				}
				if (ymin>point.y)
				{
					ymin = point.y;
				}
				if (ymax<point.y)
				{
					ymax = point.y;
				}
			}
		}
	}
	float xd = xmax - xmin;
	float yd = ymax - ymin;
	float max=0;
	if (xd>yd)
	{
		max=xd;
	}else{
		max=yd;
	}

	//transform coordinate
	transformCoordinate(ccp(xmin,ymin),max);

	if (xd > yd)
	{
		return CCSize(xd,xd);
	}else{
		return CCSize(yd,yd);
	}
}

//////////////////////////////////////////////////////////////////////////
// 改变坐标系，将读取的xml坐标系做转换，符合cocos2d-x第一象限坐标系
//////////////////////////////////////////////////////////////////////////
void Character::transformCoordinate(CCPoint point,float length){
	vector<Bujian>::iterator iter = bujianList.begin();
	for (int bujiani = 0 ; bujiani < bujianCount ; ++ bujiani)
	{
		Bujian bujian = bujianList.at(bujiani);//(Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (int strokei = 0; strokei < bujian.strokeCount; ++strokei)
		{
			Stroke stroke = strokeList.at(strokei);//(Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.getpointList();
			for (int i = 0;i < stroke.getPointsCount(); i++)
			{
				CCPoint temppoint = pointList.at(i);
				//temppoint = temppoint - point;
				temppoint.y = - temppoint.y;
				//temppoint.y = temppoint.y + length;
				temppoint.y += fontSize;
				///////////////////////////////////////////////
// 				bujianList[bujiani].strokeList[strokei].pointList.erase(pointList.begin()+i);
// 				vector<CCPoint>::iterator po_iter = bujianList[bujiani].strokeList[strokei].pointList.begin();
// 				bujianList[bujiani].strokeList[strokei].pointList.insert(po_iter+i,ccp(temppoint.x,temppoint.y));
				bujianList[bujiani].strokeList[strokei].setpointList(i,ccp(temppoint.x,temppoint.y));
				///////////////////////////////////////////////
				//pointList.erase(pointList.begin()+i);
				//vector<CCPoint>::iterator po_iter = pointList.begin();
				//pointList.insert(po_iter+i,ccp(temppoint.x,temppoint.y));		//坐标转换
				//pointList[i] = ccp(temppoint.x,temppoint.y);
			}
		}
	}
}

int Character::getStrokeCount(){
	int count = 0;
	for (vector<Bujian>::iterator iter = bujianList.begin(); iter != bujianList.end(); ++ iter)
	{
		count += ((Bujian)*iter).strokeCount;
	}
	return count;
}

void Character::prepareDrawNode(){
	vector<Bujian>::iterator iter = bujianList.begin();
	for (int i = 0; i < bujianCount ; ++i)
	{
		Bujian bujian = (Bujian)*iter;
		vector<Stroke> strokeList = bujian.strokeList;
		vector<Stroke>::iterator stro_iter = strokeList.begin();
		for (int strokei = 0; strokei < bujian.strokeCount; ++strokei)
		{
			Stroke stroke = strokeList[strokei];//(Stroke)*stro_iter;
			vector<CCPoint> pointList = stroke.getpointList();
			vector<CCDrawNode*> nodeList = stroke.nodeList;
			vector<CCPoint>::iterator point_iter = pointList.begin();
			CCPoint prePoint = (CCPoint)(*point_iter);
			point_iter++;
			for (int pointi = 1; pointi < stroke.getPointsCount(); ++pointi)
			{
				CCPoint point = pointList[pointi];//(CCPoint)*point_iter;
				CCDrawNode* node = CCDrawNode::create();
				node->drawSegment(prePoint,point,5.0,ccc4f(180,180,180,180));
				prePoint = point;
// 				vector<CCDrawNode*>::iterator nodeIter = nodeList.end();
				//nodeList.insert(nodeIter,node);
				bujianList[i].strokeList[strokei].nodeList.push_back(node);
			}
		}
	}
}

/************************************************************************/
/* 依据传入CCSize，宽度，重新计算，点的坐标值，进行缩放操作，适合田字格大小                                                                     */
/************************************************************************/
void Character::resize(CCSize size){
	float width = size.width;
	float scale = width/this->fontSize;		//确定缩放比例
	//重置所有保存点
	for (int bujiani = 0 ; bujiani < bujianCount; ++ bujiani)
	{
		Bujian bujian = bujianList.at(bujiani);
		vector<Stroke> strokeList = bujian.strokeList;
		for (int strokei = 0 ;  strokei < bujian.strokeCount; ++strokei)
		{
			Stroke stroke = strokeList.at(strokei);
			//重置stroke首点
			bujianList[bujiani].strokeList[strokei].prePoint = stroke.prePoint*scale;

			vector<CCPoint> pointList = stroke.getpointList();
			for (int i =0 ; i < stroke.getPointsCount() ; ++ i)
			{
				CCPoint temppoint = pointList.at(i);
				bujianList[bujiani].strokeList[strokei].setpointList(i,temppoint*scale);
			}
		}
	}
	
}

//重采样
void Character::resample(){
	for (int bujiani = 0 ; bujiani < bujianCount ; ++ bujiani)
	{
		Bujian bujian  = bujianList.at(bujiani);
		for (int strokei = 0 ; strokei < bujian.strokeCount ; ++ strokei)
		{
			bujianList[bujiani].strokeList[strokei].resample();
		}
	}
}

/************************************************************************/
/* 传入第几笔no  笔画从1开始                                                        */
/************************************************************************/
Stroke Character::getStroke(int no){
	int totalStrokeCount = 0;
	int tag = 0;
	for (int i = 0; i < this->bujianCount ; ++i)
	{
		totalStrokeCount += bujianList[i].strokeCount;
	}
	if (no <= totalStrokeCount)
	{
		//小于全部笔画数
		for (int j = 0 ; j < this->bujianList.size() ; ++j )
		{
			Bujian bujian_temp = bujianList[j];
			for (int k = 0 ; k < bujian_temp.strokeList.size() ; ++k)
			{
				tag++;
				if (tag == no)
				{
					return bujian_temp.strokeList[k];
				}
			}
		}
	}
}
