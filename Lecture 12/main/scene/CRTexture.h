// scene/CRTexture.h
#pragma once
#include <string>
#include "../math/CRTVector.h"
#include "../scene/BitMapBuffer.h"

enum class TextureType {
    ALBEDO,
    EDGES,
    CHECKER,
    BITMAP,
    INVALID
};

struct AlbedoData 
{
    CRTVector albedo;
};

struct EdgesData 
{
    CRTVector edgeColor;
    CRTVector innerColor;
    float edgeWidth = 0.f;
};

struct CheckerData 
{
    CRTVector colorA;
    CRTVector colorB;
    float squareSize = 0.f;
};

struct BitmapData 
{
    std::string filePath;
    BitMapBuffer buffer;

    BitmapData() = default;
    BitmapData(std::string&& path) : filePath(std::move(path)), buffer(filePath) {}
    BitmapData(BitmapData&&) noexcept = default;
    BitmapData& operator=(BitmapData&&) noexcept = default;
    BitmapData(const BitmapData&) = delete;
    BitmapData& operator=(const BitmapData&) = delete;
    ~BitmapData() = default;
};

union TextureData {
    AlbedoData albedo;
    EdgesData edges;
    CheckerData checker;
    BitmapData bitmap;

    TextureData() {}
    ~TextureData() {}
};

class CRTexture {
public:
    CRTexture();
    CRTexture(const std::string& name, const std::string& type);
    CRTexture(CRTexture&& other) noexcept;
    CRTexture& operator=(CRTexture&& other) noexcept;
    ~CRTexture();

    TextureType getType() const;
    const std::string& getName() const;
    const AlbedoData& getAlbedoData() const;
    const EdgesData& getEdgesData() const;
    const CheckerData& getCheckerData() const;
    const BitmapData& getBitmapData() const;

    void setType(TextureType t);
    void setName(const std::string& name);
    void setAlbedoData(AlbedoData&& albedo);
    void setEdgesData(EdgesData&& edges);
    void setCheckerData(CheckerData&& checker);
    void setBitmapData(BitmapData&& bitmap);

private:
    void destroyActive();
    void moveFrom(CRTexture&& other);

    std::string name;
    TextureType type = TextureType::INVALID;
    TextureData data;
};