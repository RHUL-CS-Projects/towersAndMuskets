#ifndef TRANSPARENTMATERIALSHADER_H
#define TRANSPARENTMATERIALSHADER_H

#include <irrlicht/irrlicht.h>
#include <Game.h>

using namespace irr;

class TransparentMaterialShader : public irr::video::IShaderConstantSetCallBack {
public:
	static s32 materialID;
	const video::SMaterial* usedMaterial;
	
	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		irr::video::IVideoDriver* driver = services->getVideoDriver();
		
		core::matrix4 worldViewProj;
		worldViewProj = driver->getTransform(video::ETS_PROJECTION);
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		
		s32 TextureLayerID = 0;
		services->setPixelShaderConstant("myTexture", &TextureLayerID, 1);
		services->setPixelShaderConstant("alpha", &usedMaterial->MaterialTypeParam, 1);
	}
	
	virtual void OnSetMaterial(const video::SMaterial& material) {
		usedMaterial = &material;
	}
};

#endif