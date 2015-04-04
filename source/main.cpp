
//
//    DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                   Version 2, December 2004
//
//Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
//Everyone is permitted to copy and distribute verbatim or modified
//copies of this license document, and changing it is allowed as long
//as the name is changed.
//
//           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//  TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
// 0. You just DO WHAT THE FUCK YOU WANT TO.

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/shader.hpp>
#include <vector>

// Mine
#include "AppController.h"
#include "LuaScriptor.h"
#include "model/ObjectModel.h"
#include "nodes/DDNodes.h"
#include "interop/lua_core/lua_core.h"
#include "interop/class/exports.h"

int main(void)
{
	AppController app;
	if (!app.initAppGL(4,4, GLFW_OPENGL_CORE_PROFILE))
	{
		return -1;
	}
	app.setDirector(EsDirector::instance());
	LuaCore::injectLuaCore();
	doExports();

	EsNode* root = FieldNode::create(20);
	EsDirector::instance()->setRoot(root);
	app.loadObject();

	//root->addSub(AxisNode::create());
	//for(int i=-5;i<=0;++i){
	//	auto one = CubeNode::create(0.15 * i , 0.15 * i, 0.2, 0.2, 0.2);
	//	float scale = abs(i) * 0.07 + 0.3;
	//	//one->setScale(scale,scale,scale);
	//	one->shiftXZ();
	//	one->setColor(glm::vec4(0,0.3,0.3,0.5));
	//	root->addSub(one);
	//}

	//BoxNode *box = BoxNode::create(0,0);
	//box->setColor(DDColor(0,1,1,1));
	//box->setScale(2);
	//root->addSub(box);

	//One test:
	//BlenderNode *b = BlenderNode::create("objects/uvsphere8.json", true);
	//b->setFaceDefaultColor(DDColor(1,0,0,1));
	//b->setScale(0.1);
	//root->addSub(b);

	//EsNode *b = MimicNode::create();
	//////b->setScale(0.005);
	//b->setScale(0.025);
	//b->setTransform(0, 0, 0);
	//root->addSub(b);

	//LightNode* lightNode = LightNode::create(0.6, 0.6, 0.6);
	//lightNode->setScale(0.01);
	//EsDirector::instance()->setLight(lightNode);
	//root->addSub(lightNode);
	//root->setVisible(false);

	//DDColor colors[]={DDColor(0,0,1,0.5), DDColor(0,1,1,1), DDColor(1,0,0,0.5)};
	//for(int i=-1;i<=1;++i){
	//	BlenderNormal *box = BlenderNormal::create("objects/uvsphere.json");
	//	box->setTransform(0.75 * i, 0, 0);
	//	box->setScale(0.2);
	//	box->setColor(colors[i+1]);
	//	root->addSub(box);
	//}

	//FieldNode *sub = FieldNode::create(10);
	//sub->setTransform(glm::translate(glm::vec3(0, -0.1,0)));
	//sub->setScale(0.2, 0.2,0.2);
	//sub->setColor(glm::vec4(0.8, 0, 0.8, 1));
	//root->addSub(sub);

	
	app.mainLoop();
	return 0;
}

