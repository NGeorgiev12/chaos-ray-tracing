#pragma once
#include <string>
#include "../math/CRTVector.h"
#include "../scene/BitMapBuffer.h"

enum class TextureType
{
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
};

union TextureData
{
	AlbedoData albedo;
	EdgesData edges;
	CheckerData checker;
	BitmapData bitmap;

	TextureData() : albedo() {}
	~TextureData() {}
};

class CRTexture
{
public:

	CRTexture() = default;
	CRTexture(const std::string& name, const std::string& type);

	CRTexture(const CRTexture& other) = delete;
	CRTexture& operator=(const CRTexture& other) = delete;

	CRTexture(CRTexture&& other) noexcept;
	CRTexture& operator=(CRTexture&& other) noexcept;

	~CRTexture();

	TextureType getType() const;
	const std::string& getName() const;
	const AlbedoData& getAlbedoData() const;
	const EdgesData& getEdgesData() const;
	const CheckerData& getCheckerData() const;
	const BitmapData& getBitmapData() const;

	void setType(TextureType type);
	void setName(const std::string& name);
	void setAlbedoData(AlbedoData&& albedo);
	//void setAlbedoData(const AlbedoData& albedo);
	void setEdgesData(EdgesData&& edges);
	//void setEdgesData(const EdgesData& edges);
	void setCheckerData(CheckerData&& checker);
	//void setCheckerData(const CheckerData& checker);
	void setBitmapData(BitmapData&& bitmap);
	//void setBitmapData(const BitmapData& bitmap);

private:

	std::string name;
	TextureType type = TextureType::INVALID;
	TextureData data;

};