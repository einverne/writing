//
//  TDInvFileUtils.h
//  MyCocoa2DTest
//
//  Created by 韦 柱全 on 13-2-27.
//
//

#ifndef __MyCocoa2DTest__TDInvFileUtils__
#define __MyCocoa2DTest__TDInvFileUtils__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
using namespace std;

/** 负责操作文件储存和读取
 */

class TDInvFileUtils {
public:
    /** 读取本地文件，返回数据 */
    static string getFileByName(string pFileName);
    
    /** 储存内容到文件 */
    static bool saveFile(char* pContent,string pFileName);
    
};

#endif /* defined(__MyCocoa2DTest__TDInvFileUtils__) */