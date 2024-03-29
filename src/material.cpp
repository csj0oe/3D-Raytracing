#include "material.h"

#include <Eigen/Geometry>
#include <iostream>
#include <math.h>

void Material::loadTextureFromFile(const std::string& fileName)
{
    if (fileName.size()==0)
        std::cerr << "Material error : no texture file name provided" << std::endl;
    else
        m_texture = new Bitmap(fileName);
}

Diffuse::Diffuse(const PropertyList &propList)
{
    m_diffuseColor = propList.getColor("diffuse",Color3f(0.2));

    std::string texturePath = propList.getString("texture","");
    if(texturePath.size()>0){
        filesystem::path filepath = getFileResolver()->resolve(texturePath);
        loadTextureFromFile(filepath.str());
        setTextureScale(propList.getFloat("scale",1));
        setTextureMode(TextureMode(propList.getInteger("mode",0)));
    }
}

Color3f Diffuse::diffuseColor(const Vector2f& uv) const
{
    // texture
    if(texture() == nullptr)
        return m_diffuseColor;

    int tu = fmod(uv.x()/textureScaleU(), 1.0f) * texture()->cols();
    int tv = fmod(uv.y()/textureScaleV(), 1.0f) * texture()->rows();

    if (textureMode() == REPLACE)
        return (*texture())(tv,tu);

    return (*texture())(tv, tu) * m_diffuseColor;
}

REGISTER_CLASS(Diffuse, "diffuse")
