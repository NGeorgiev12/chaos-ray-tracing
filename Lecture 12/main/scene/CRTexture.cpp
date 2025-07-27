#include "CRTexture.h"

static TextureType fromStringToTextureType(const std::string& type) {

	if (type == "albedo") return TextureType::ALBEDO;
	if (type == "edges") return TextureType::EDGES;
	if (type == "checker") return TextureType::CHECKER;
	if (type == "bitmap") return TextureType::BITMAP;

	return TextureType::INVALID;

}
CRTexture::CRTexture(const std::string& name, const std::string& type) 
	: name(name), type(fromStringToTextureType(type))
{
}


void CRTexture::setType(TextureType type)
{
	this->type = type;
}

void CRTexture::setName(const std::string& name)
{
	this->name = name;
}

void CRTexture::setAlbedoData(AlbedoData&& albedo)
{
    data.albedo = std::move(albedo);
}

//void CRTexture::setAlbedoData(const AlbedoData& albedo)
//{
//	data.albedo = albedo;
//}

void CRTexture::setEdgesData(EdgesData&& edges)
{
    data.edges = std::move(edges);
}

//void CRTexture::setEdgesData(const EdgesData& edges)
//{
//	data.edges = edges;
//}

void CRTexture::setCheckerData(CheckerData&& checker)
{
    data.checker = std::move(checker);
}

//void CRTexture::setCheckerData(const CheckerData& checker)
//{
//	data.checker = checker;
//}

void CRTexture::setBitmapData(BitmapData&& bitmap)
{
    data.bitmap = std::move(bitmap);
}

//void CRTexture::setBitmapData(const BitmapData& bitmap)
//{
//	data.bitmap = bitmap;
//}

CRTexture::CRTexture(CRTexture&& other) noexcept
    : name(std::move(other.name)),
    type(other.type)
{
    switch (type)
    {
    case TextureType::ALBEDO:
        data.albedo = other.data.albedo;
        break;
    case TextureType::EDGES:
        data.edges = other.data.edges;
        break;
    case TextureType::CHECKER:
        data.checker = other.data.checker;
        break;
    case TextureType::BITMAP:
        new (&data.bitmap) BitmapData(std::move(other.data.bitmap));
        break;
    default:
        break;
    }
}

CRTexture& CRTexture::operator=(CRTexture&& other) noexcept
{
    if (this != &other)
    {
        this->~CRTexture(); 

        name = std::move(other.name);
        type = other.type;

        switch (type)
        {
        case TextureType::ALBEDO:
            data.albedo = other.data.albedo;
            break;
        case TextureType::EDGES:
            data.edges = other.data.edges;
            break;
        case TextureType::CHECKER:
            data.checker = other.data.checker;
            break;
        case TextureType::BITMAP:
            new (&data.bitmap) BitmapData(std::move(other.data.bitmap));
            break;
        default:
            break;
        }
    }
    return *this;
}

CRTexture::~CRTexture()
{
    if (type == TextureType::BITMAP)
    {
        data.bitmap.~BitmapData();
    }
}

TextureType CRTexture::getType() const
{
	return type;
}

const std::string& CRTexture::getName() const
{
	return name;
}

const AlbedoData& CRTexture::getAlbedoData() const
{
	return data.albedo;
}

const EdgesData& CRTexture::getEdgesData() const
{
	return data.edges;
}

const CheckerData& CRTexture::getCheckerData() const
{
	return data.checker;
}

const BitmapData& CRTexture::getBitmapData() const
{
	return data.bitmap;
}
