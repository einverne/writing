#include "ReadXML.h"
#include "Bujian.h"
#include "Stroke.h"
#include "Character.h"

USING_NS_CC;
using namespace std;

CReadXML::CReadXML(const char* xmlcontent)
{
	TiXmlDocument* document = new TiXmlDocument();
	document->Parse(xmlcontent,0,TIXML_ENCODING_UTF8);
    parseXML(document);
}


CReadXML::CReadXML(string xmlpath)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	unsigned long size = 0;
	char* pFileContent = (char*)CCFileUtils::sharedFileUtils()->getFileData(xmlpath.c_str(),"r",&size);
	TiXmlDocument* document = new TiXmlDocument();
	document->Parse(pFileContent,0,TIXML_ENCODING_UTF8);
#endif
#if CC_TARGET_PLATFORM ==  CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	TiXmlDocument* document = new TiXmlDocument(xmlpath.c_str());
	document->LoadFile();
#endif
	parseXML(document);
}


CReadXML::~CReadXML(void)
{
}

Character CReadXML::getCharacter() const{
	return character;
}

void CReadXML::parseXML(TiXmlDocument* document){
    TiXmlElement* rootElement = document->RootElement();
	const char* name = rootElement->Attribute("name");
	character.setName(name);
	const char* seq = rootElement->Attribute("stroke_seq_id");
	character.setStrokeSeq(seq);
	const char* unit_seq_id = rootElement->Attribute("unit_seq_id");
	character.setRadicalSeq(unit_seq_id);
    TiXmlElement* outlineElement = rootElement->FirstChildElement();
    Bujian bujian;
    for (TiXmlElement* strokeElement= outlineElement->FirstChildElement();strokeElement;strokeElement = strokeElement->NextSiblingElement())
    {
        TiXmlElement* trunpoint = strokeElement->FirstChildElement();
        Stroke stroke;
        for (trunpoint;trunpoint;trunpoint = trunpoint->NextSiblingElement())
        {
            const char* x = trunpoint->Attribute("x");
            const char* y = trunpoint->Attribute("y");
            const char* statusPoint = trunpoint->Attribute("status");
            stroke.addStatus(statusPoint);
            stroke.AddPoint(ccp(atof(x),atof(y)));
        }
        bujian.addStroke(stroke);
    }
    character.addBujian(bujian);
}


TemplateCharacter CReadXML::getTemplateCharacterFromBuffer(string xmlcontent, string mark_buffer){
    TemplateCharacter template_character;
    //////////////////////
    // 1. 字形信息
    TiXmlDocument* document = new TiXmlDocument();
    document->Parse(xmlcontent.c_str(),0,TIXML_ENCODING_UTF8);
    TiXmlElement* rootElement = document->RootElement();
    TiXmlElement* outlineElement = rootElement->FirstChildElement();
    TiXmlElement* strokeElement = outlineElement->FirstChildElement();
    // 读取笔画信息
    string stroke_name;  // 笔画名
    list<Segment> seg_list;  // 一个笔画所包含的笔段列表
    int seg_ind=0;   // 笔段索引
    Segment seg;    // 笔段临时变量
    list<CCPoint> biduan_list;   // 一个笔段包含的点列表
	for (TiXmlElement* strokeElement = outlineElement->FirstChildElement(); strokeElement!=NULL; strokeElement = strokeElement->NextSiblingElement())
    {
        stroke_name = strokeElement->Attribute("name");      // 笔画 name
        //TRACE(strokename.c_str());
        CCLog("%s",stroke_name.c_str());
        string Sname = stroke_name.c_str();
        
        ////////////
        seg.Clear();
        biduan_list.clear();
        seg_list.clear();
        int name_ind=0;
        
        for (TiXmlElement* trunpoint= strokeElement->FirstChildElement(); trunpoint!=NULL; trunpoint = trunpoint->NextSiblingElement())
        {
            const char* x = trunpoint->Attribute("x");
            const char* y = trunpoint->Attribute("y");
            const char* status =  trunpoint->Attribute("status");
            // 没有特殊点
            if (status[0]=='0')
            {
                biduan_list.push_back(CCPoint(atof(x),atof(y)));
            }
            else if(status[0]=='1')		//存在特殊点
            {
                biduan_list.push_back(CCPoint(atof(x),atof(y)));
                //特殊处理“横折斜勾”
                if(Sname=="横折斜勾" && name_ind==1)
                {
                    seg.Init(Sname.substr(name_ind*2,4),seg_ind,biduan_list);
                    name_ind++;
                }
                else
                    seg.Init(Sname.substr(name_ind*2,2),seg_ind,biduan_list);
                
                name_ind++;
                seg_ind++;
                seg_list.push_back(seg);
                biduan_list.clear();
                biduan_list.push_back(CCPoint(atof(x),atof(y)));
            }
        }
        seg.Init(Sname.substr(name_ind*2,2),seg_ind,biduan_list);		//保存笔段
        seg_ind++;
        seg_list.push_back(seg);
        biduan_list.clear();
        ///
        template_character.AppendStroke(Sname,seg_list);
    }
    
    //读取结构信息
    TiXmlElement* viewElement = outlineElement->NextSiblingElement();
    
    TiXmlAttribute * viewatt=viewElement->FirstAttribute();
    if(viewatt!=NULL)
    {
        string struct_name = viewElement->Attribute("name");
		CCLog("struct name: %s", struct_name.c_str());
        template_character.m_struct_name_=struct_name.c_str();
    }
    else
        template_character.m_struct_name_="no_struct_name";
    
    //////////////
    delete document;

	// 完成正字xml信息解析
    
    //2.标注信息——不稳定笔段 读取 marked info 对应表中 mark 字段
    document = new TiXmlDocument();
    document->Parse(mark_buffer.c_str(), 0, TIXML_ENCODING_UTF8);
    
    TiXmlElement* instable_segments = document->RootElement();
    string instablestr=instable_segments->Attribute("index");
    
    vector<string> strvec = DataTool::spliteStringBy(instablestr, "/");
//    StringSplit::String_Split(instablestr,'/',strvec);
    for(int i=0;i<strvec.size();i++)
    {
        template_character.AddInstableSegment(atoi(strvec[i].c_str()));
    }
    
    //2.标注信息——交搭关系
    template_character.InitSegmentRelation();
    TiXmlElement* segment_relation = instable_segments->NextSiblingElement();
    for (TiXmlElement* relation = segment_relation->FirstChildElement(); relation!=NULL; relation = relation->NextSiblingElement())
    {
        string segment_index=relation->Attribute("segment");
        string segment_relationship=relation->Attribute("relationship");
        
//        StringSplit::String_Split(relationship,'/',strvec);
        strvec = DataTool::spliteStringBy(segment_relationship, "/");
        
        for(int i=0; i<strvec.size(); i++)
        {
            template_character.SetRelation(atoi(segment_index.c_str()), i, strvec[i]);
        }
    }
    
    //2.标注信息——部件包围盒
    list<int>  temp_seglist;
    list<int>  temp_toplist;
    list<int>  temp_bottomlist;
    list<int>  temp_leftlist;
    list<int>  temp_rightlist;
    
    TiXmlElement* structbox = segment_relation->NextSiblingElement();
    for (TiXmlElement* boundingboxelement= structbox->FirstChildElement(); boundingboxelement!=NULL; boundingboxelement = boundingboxelement->NextSiblingElement())
    {
        string m_struct =  boundingboxelement->Attribute("struct");
        string m_top =  boundingboxelement->Attribute("top");
        string m_bottom =  boundingboxelement->Attribute("bottom");
        string m_left =  boundingboxelement->Attribute("left");
        string m_right =  boundingboxelement->Attribute("right");
        
        temp_seglist.clear();
        temp_toplist.clear();
        temp_bottomlist.clear();
        temp_leftlist.clear();
        temp_rightlist.clear();
        
        vector<string> seg_vec;
        vector<string> top_vec;
        vector<string> bottom_vec;
        vector<string> left_vec;
        vector<string> right_vec;
        
//        StringSplit::String_Split(m_struct,'/',seg_vec);
        seg_vec = DataTool::spliteStringBy(m_struct, "/");
        for (int i=0;i<seg_vec.size();i++)
        {
            temp_seglist.push_back(atoi(seg_vec[i].c_str()));
        }
        
//        StringSplit::String_Split(m_top,'/',top_vec);
        top_vec = DataTool::spliteStringBy(m_top, "/");
        for (int i=0;i<top_vec.size();i++)
        {
            temp_toplist.push_back(atoi(top_vec[i].c_str()));
        }
        
//        StringSplit::String_Split(m_bottom,'/',bottom_vec);
        bottom_vec = DataTool::spliteStringBy(m_bottom, "/");
        for (int i=0;i<bottom_vec.size();i++)
        {
            temp_bottomlist.push_back(atoi(bottom_vec[i].c_str()));
        }
        
//        StringSplit::String_Split(m_left,'/',left_vec);
        left_vec = DataTool::spliteStringBy(m_left, "/");
        for (int i=0;i<left_vec.size();i++)
        {
            temp_leftlist.push_back(atoi(left_vec[i].c_str()));
        }
        
//        StringSplit::String_Split(m_right,'/',right_vec);
        right_vec = DataTool::spliteStringBy(m_right, "/");
        for (int i=0;i<right_vec.size();i++)
        {
            temp_rightlist.push_back(atoi(right_vec[i].c_str()));
        }
        
        CComponent m_com;
        m_com.setdata(temp_seglist,temp_toplist,temp_bottomlist,temp_leftlist,temp_rightlist);
        template_character.Add_one_component_setting(m_com);
    }
    
    delete document;
    
    //3.初始化信息
    template_character.draw_point_=-1;
    template_character.draw_index_=-1;
    
    ///////////////////////////////////////
    
    
    return template_character;
}

ScriptCharacter CReadXML::getScriptCharacterFromBuffer(string xmlcontent,string mark_buffer){
    ScriptCharacter schar;
    
    
    return schar;
}
