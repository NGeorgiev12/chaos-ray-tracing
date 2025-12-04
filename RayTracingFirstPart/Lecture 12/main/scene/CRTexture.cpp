// scene/CRTexture.cpp
#include "CRTexture.h"
#include <stdexcept>

static TextureType fromStringToTextureType(const std::string& type) 
{
    if (type == "albedo") return TextureType::ALBEDO;
    if (type == "edges") return TextureType::EDGES;
    if (type == "checker") return TextureType::CHECKER;
    if (type == "bitmap") return TextureType::BITMAP;

    return TextureType::INVALID;
}

CRTexture::CRTexture() : name(), type(TextureType::INVALID) {}

CRTexture::CRTexture(const std::string& name, const std::string& type)
    : name(name), type(fromStringToTextureType(type)) {

    switch (this->type) {
    case TextureType::ALBEDO:
        new (&data.albedo) AlbedoData();
        break;
    case TextureType::EDGES:
        new (&data.edges) EdgesData();
        break;
    case TextureType::CHECKER:
        new (&data.checker) CheckerData();
        break;
    case TextureType::BITMAP:
        new (&data.bitmap) BitmapData();
        break;
    default:
        break;
    }
}

CRTexture::CRTexture(CRTexture&& other) noexcept : name(std::move(other.name)), type(TextureType::INVALID) {
    moveFrom(std::move(other));
}

CRTexture& CRTexture::operator=(CRTexture&& other) noexcept {

    if (this != &other) {
        destroyActive();
        name = std::move(other.name);
        moveFrom(std::move(other));
    }
    return *this;
}

CRTexture::~CRTexture() {
    destroyActive();
}

void CRTexture::destroyActive() 
{
    switch (type) {
    case TextureType::ALBEDO:
        data.albedo.~AlbedoData();
        break;
    case TextureType::EDGES:
        data.edges.~EdgesData();
        break;
    case TextureType::CHECKER:
        data.checker.~CheckerData();
        break;
    case TextureType::BITMAP:
        data.bitmap.~BitmapData();
        break;
    default:
        break;
    }
    type = TextureType::INVALID;
}

void CRTexture::moveFrom(CRTexture&& other) 
{
    type = other.type;

    switch (type) {
    case TextureType::ALBEDO:
        new (&data.albedo) AlbedoData(std::move(other.data.albedo));
        break;
    case TextureType::EDGES:
        new (&data.edges) EdgesData(std::move(other.data.edges));
        break;
    case TextureType::CHECKER:
        new (&data.checker) CheckerData(std::move(other.data.checker));
        break;
    case TextureType::BITMAP:
        new (&data.bitmap) BitmapData(std::move(other.data.bitmap));
        break;
    default:
        break;
    }
    other.destroyActive();
}

const AlbedoData& CRTexture::getAlbedoData() const 
{
    if (type != TextureType::ALBEDO) 
        throw std::logic_error("Not an albedo texture");

    return data.albedo;
}
const EdgesData& CRTexture::getEdgesData() const 
{
    if (type != TextureType::EDGES) 
        throw std::logic_error("Not an edges texture");

    return data.edges;
}
const CheckerData& CRTexture::getCheckerData() const 
{
    if (type != TextureType::CHECKER) 
        throw std::logic_error("Not a checker texture");

    return data.checker;
}
const BitmapData& CRTexture::getBitmapData() const 
{
    if (type != TextureType::BITMAP) 
        throw std::logic_error("Not a bitmap texture");

    return data.bitmap;
}

void CRTexture::setType(TextureType t) {

    if (type != t) {
        destroyActive();
        type = t;
        switch (type) {
        case TextureType::ALBEDO:
            new (&data.albedo) AlbedoData();
            break;
        case TextureType::EDGES:
            new (&data.edges) EdgesData();
            break;
        case TextureType::CHECKER:
            new (&data.checker) CheckerData();
            break;
        case TextureType::BITMAP:
            new (&data.bitmap) BitmapData();
            break;
        default:
            break;
        }
    }
}

void CRTexture::setName(const std::string& name) 
{
    this->name = name;
}

void CRTexture::setAlbedoData(AlbedoData&& albedo) 
{
    destroyActive();
    new (&data.albedo) AlbedoData(std::move(albedo));
    type = TextureType::ALBEDO;
}

void CRTexture::setEdgesData(EdgesData&& edges) 
{
    destroyActive();
    new (&data.edges) EdgesData(std::move(edges));
    type = TextureType::EDGES;
}

void CRTexture::setCheckerData(CheckerData&& checker) 
{
    destroyActive();
    new (&data.checker) CheckerData(std::move(checker));
    type = TextureType::CHECKER;
}

void CRTexture::setBitmapData(BitmapData&& bitmap) 
{
    destroyActive();
    new (&data.bitmap) BitmapData(std::move(bitmap));
    type = TextureType::BITMAP;
}

TextureType CRTexture::getType() const
{
    return type;
}

const std::string& CRTexture::getName() const
{
    return name;
}
