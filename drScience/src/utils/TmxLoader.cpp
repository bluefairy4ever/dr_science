/*********************************************************************
Quinn Schwab
16/08/2010

Modified:
Matt Marchant
10/09/2012
Added SFML 2.0 support
Added support for compressed and uncompressed base64 encoded maps
Added support for zlib / gzip compressed maps
Added support for CSV encoded maps
Added custom exception for missing object properties
Added support for layer visibility attribute
Added support for tile spacing attributes
Added support for tile map trans attribute
Added support for object gid and visibility attributes
Added support for polygon and polyline objects

SFML Tiled Map Loader - https://github.com/bjorn/tiled/wiki/TMX-Map-Format

The zlib license has been used to make this software fully compatible
with SFML. See http://www.sfml-dev.org/license.php

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
   you must not claim that you wrote the original software.
   If you use this software in a product, an acknowledgment
   in the product documentation would be appreciated but
   is not required.

2. Altered source versions must be plainly marked as such,
   and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
   source distribution.
*********************************************************************/

#include "utils/TmxLoader.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <TinyXML/tinyxml.h>
#include <zlib.h>
#include <cstring>

using namespace tmx;


//custom exception thrown when specified object not found
class PropetyNotFound: public std::exception
{
	virtual const char* message() const throw()
	{
		return "Specified Property Not Found";
	}
} noProperty;


///***Object class***///

Object::Object() : name("NULL OBJECT"), type("NULL"), gid(0), visible(0){}

int Object::GetPropertyInt(std::string name, int defaultValue) const
{
	try
	{
		std::map<std::string,std::string>::const_iterator iter;
		iter = properties.find(name);
		if (iter != properties.end()) { 
			return atoi(iter->second.c_str());
		} else { 
			return defaultValue;
		}
	}
	catch(...)
	{
		throw noProperty;
	}
}

float Object::GetPropertyFloat(std::string name, float defaultValue) const
{
    try
	{
		std::map<std::string,std::string>::const_iterator iter;
		iter = properties.find(name);
		if (iter != properties.end()) { 
			return (float) strtod(iter->second.c_str(), 0);
		} else { 
			return defaultValue;
		}
	}
	catch(...)
	{
		throw noProperty;
	}
}

std::string Object::GetPropertyString(std::string name, std::string defaultValue) const
{
	std::map<std::string,std::string>::const_iterator iter;
	iter = properties.find(name);
	if (iter != properties.end()) { 
		return iter->second;
	} else { 
		return defaultValue;
	}
}



///***Tilemap class***///

TileMap::TileMap()
{
    //ctor
}

TileMap::~TileMap()
{
    //dtor
}

bool TileMap::LoadFromFile(std::string filename, std::string filepath)
{
    //clear any existing map data
	m_UnloadMap();

	filename = filepath + filename;

	TiXmlDocument levelFile(filename.c_str());

    if (!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }

    //Map element. This is the root element for the whole file.
    TiXmlElement *map;
    map = levelFile.FirstChildElement("map");
	//check map orientation type
	if(map->Attribute("orientation"))
	{
		std::string mapType = map->Attribute("orientation");
		if(mapType != "orthogonal")
		{
			std::cout << "Non-orthagonal maps are currently not supported" << std::endl;
			return false;
		}
	}

    //Set up misc map properties - make this safe by checking attribute exists to prevent assertion
	//failure if map is corrupt of attribute is missing
    m_width = (map->Attribute("width")) ? atoi(map->Attribute("width")) : 0;
    m_height = (map->Attribute("height")) ? atoi(map->Attribute("height")): 0;
    m_tileWidth = (map->Attribute("tilewidth")) ? atoi(map->Attribute("tilewidth")) : 0;
    m_tileHeight = (map->Attribute("tileheight")) ? atoi(map->Attribute("tileheight")) : 0;

    //Tileset stuff
    TiXmlElement *tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    m_firstTileID = (tilesetElement->Attribute("firstgid")) ? atoi(tilesetElement->Attribute("firstgid")) : 0;
	m_spacing = (tilesetElement->Attribute("spacing")) ? atoi(tilesetElement->Attribute("spacing")) : 0;
	m_margin = (tilesetElement->Attribute("margin")) ? atoi(tilesetElement->Attribute("margin")) : 0;

    //Tileset image
    TiXmlElement *image;
    image = tilesetElement->FirstChildElement("image");
    std::string imagepath = image->Attribute("source");
	//this assumes tilesheet is in the same directory as the tmx file
    if (!m_tilesetImage.loadFromFile(filepath + imagepath))//Load the tileset image
    {
        std::cout << "Failed to load tile sheet. NOTE .tsx file format currently unsupported." << std::endl;
        return false;
    }

	//load transparency colour from attribute if it exists, else set to magenta
	sf::Color transparency = (image->Attribute("trans")) ? m_ColourFromHex(image->Attribute("trans")) : sf::Color::Magenta;
    m_tilesetImage.createMaskFromColor(transparency);

    //Columns and rows (of tileset image)
    int columns = m_tilesetImage.getSize().x / m_tileWidth;
    int rows = m_tilesetImage.getSize().y / m_tileHeight;

	//load image into texture
	m_tilesetTexture.loadFromImage(m_tilesetImage);

    std::vector<sf::Rect<int> > subRects;//container of subrects (to divide the tilesheet image up)

    //tiles/subrects are counted from 0, left to right, top to bottom
    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < columns; x++)
        {
            sf::Rect <int> rect; //must account for any spacing or margin on the tileset
            rect.top = y * (m_tileHeight + m_spacing);
			rect.top += m_margin;
			rect.height = m_tileHeight;
            rect.left = x * (m_tileWidth + m_spacing);
			rect.left += m_spacing;
            rect.width = m_tileWidth;
            subRects.push_back(rect);
        }
    }

#pragma region tile layers

    //Layers
    TiXmlElement *layerElement;
    layerElement = map->FirstChildElement("layer");
    while (layerElement)
    {
        Layer layer;
		//check for layer visibility property
		int layerVisible = (layerElement->Attribute("Visible"))
			? atoi(layerElement->Attribute("visible")) : layerVisible = 1;

		//set opacity if layer is visible
		if(layerVisible)
		{
			if (layerElement->Attribute("opacity"))//check if opacity attribute exists
			{
				float opacity = (float)strtod(layerElement->Attribute("opacity"), 0);//convert the (string) opacity element to float
				layer.opacity = (int)(255.f * opacity);
			}
			else
			{
				layer.opacity = 255;//if the attribute doesnt exist, default to full opacity
			}
		}
		else layer.opacity = 0;

        //Tiles
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");
		int x, y;
		x = y = 0;

        if (!layerDataElement)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
			return false;
        }

		//check for compression and decompress with zlib if possible
		if(layerDataElement->Attribute("encoding"))
		{
			std::string encoding, compression;
			encoding = layerDataElement->Attribute("encoding");
			if(layerDataElement->Attribute("compression"))
				compression = layerDataElement->Attribute("compression");

			if(encoding == "base64")
			{
				std::cout << "Found Base64 encoded data" << std::endl;
				//decode and decompress
				int expectedSize = m_width * m_height * 4; //number of tiles * 4 bytes = 32bits / tile
				std::string data = layerDataElement->GetText();
				data = base64_decode(data);

				std::vector<unsigned char>byteArray;
				if(compression == "zlib" || compression == "gzip")
				{
					std::cout << "Found " << compression << " compressed data" << std::endl;
					int dataSize = data.length() * sizeof(unsigned char);
					byteArray =  m_Decompress(data.c_str(), dataSize, expectedSize);
				}
				else //already uncompressed
				{
					for(unsigned int i = 0; i < data.length(); i++)
						byteArray.push_back(data.c_str()[i]);
				}


				int actualSize = byteArray.size() * sizeof(unsigned char);

				if(actualSize != expectedSize)
				{
					std::cout << "Unexpected decompression size" << std::endl;
					return false;
				}

				for(int i = 0; i < expectedSize - 3; i +=4)
				{
					int tileGID = byteArray[i] | byteArray[i + 1] << 8 | byteArray[i + 2] << 16 | byteArray[i + 3] << 24;
					m_SetTile(subRects, layer, x, y, tileGID);

					x++;
					if(x == m_width)
					{
						x = 0;
						y++;
					}
				}
			}
			else if (encoding == "csv") //using comma separated values
			{
				std::cout << "Found csv encoded data" << std::endl;

				std::string data = layerDataElement->GetText();
				std::vector<int> tileGIDs;
				std::stringstream datastream(data);
				//parse csv string into vector of IDs
				int i;
				while (datastream >> i)
				{
					tileGIDs.push_back(i);
					if(datastream.peek() == ',')
						datastream.ignore();
				}

				for(unsigned int i = 0; i < tileGIDs.size(); i++)
				{
					m_SetTile(subRects, layer, x, y, tileGIDs[i]);
					x++;
					if(x == m_width)
					{
						x = 0;
						y++;
					}
				}
			}
			else
			{
				std::cout << "Unsupported compression format found" << std::endl;
				return false;
			}
		}
		else //treat as uncompressed
		{
			std::cout << "Found Unencoded data" << std::endl;

			TiXmlElement *tileElement;
			tileElement = layerDataElement->FirstChildElement("tile");

			if (!tileElement)
			{
				std::cout << "Bad map. No tile information found." << std::endl;
				return false;
			}

			while (tileElement)
			{
				int tileGID = atoi(tileElement->Attribute("gid"));
				m_SetTile(subRects, layer, x, y, tileGID);
				tileElement = tileElement->NextSiblingElement("tile");

				//increment x, y
				x++;
				if (x >= m_width)//if x has "hit" the end (right) of the map, reset it to the start (left)
				{
					x = 0;
					y++;
				}
			}
		}
        m_layers.push_back(layer);

        layerElement = layerElement->NextSiblingElement("layer");
    }
#pragma endregion


#pragma region object layers

    //Objects
    TiXmlElement *objectGroupElement;
    if (map->FirstChildElement("objectgroup"))//Check that there is atleast one object layer
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)//loop through object layers
        {
            TiXmlElement *objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");
            while (objectElement)//loop through objects
            {
                std::string objectType;
                if (objectElement->Attribute("type")) objectType = objectElement->Attribute("type");

                std::string objectName;
                if (objectElement->Attribute("name")) objectName = objectElement->Attribute("name");

				//check attributes exist as not all objects use them
                int x = (objectElement->Attribute("x")) ? atoi(objectElement->Attribute("x")) : 0;
                int y = (objectElement->Attribute("y")) ? atoi(objectElement->Attribute("y")) : 0;
                int width = (objectElement->Attribute("width")) ? atoi(objectElement->Attribute("width")) : 0;
                int height = (objectElement->Attribute("height")) ? atoi(objectElement->Attribute("height")) : 0;
				int gid = (objectElement->Attribute("gid")) ? atoi(objectElement->Attribute("gid")) : 0;
				int visible = (objectElement->Attribute("visible")) ? atoi(objectElement->Attribute("visible")) : 1; //tmx spec states defaults to 1

                Object object;
                object.name = objectName;
                object.type = objectType;
				object.gid = gid;
				object.visible = visible;

                sf::Rect <int> objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;

                if (objectType == "solid") m_solidObjects.push_back(objectRect);

                object.rect = objectRect;


				//add any custom object properties here
                TiXmlElement *properties;
                properties = objectElement->FirstChildElement("properties");
                if (properties)
                {
                    TiXmlElement *prop;
                    prop = properties->FirstChildElement("property");
                    if (prop)
                    {
                        while(prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");

                            object.properties[propertyName] = propertyValue;

                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }


				//parse any polygon or polylines
				TiXmlElement *poly = objectElement->FirstChildElement("polygon");
				if(!poly) poly = objectElement->FirstChildElement("polyline");
				if(poly) //parse points into sf::Vector2i
				{
					//split coords into pairs
					std::string pointlist = poly->Attribute("points");
					std::stringstream stream(pointlist);
					std::vector<std::string> points;
					std::string pointstring;
					while(std::getline(stream, pointstring, ' '))
						points.push_back(pointstring);
					//parse each pair into sf::vector2i
					for(unsigned int i = 0; i < points.size(); i++)
					{
						std::vector<int> coords;
						std::stringstream coordstream(points[i]);

						int j;
						while (coordstream >> j)
						{
							coords.push_back(j);
							if(coordstream.peek() == ',')
								coordstream.ignore();
						}
						object.polypoints.push_back(sf::Vector2i(coords[0], coords[1]));
					}
				}

                m_objects.push_back(object);

                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }

#pragma endregion

    return true;
}

Object TileMap::GetObject(std::string name)
{
    for (unsigned int i = 0; i < m_objects.size(); i++)
    {
        if (m_objects[i].name == name)
        {
            return m_objects[i];
        }
    }
	Object object;
	return object;
}

std::vector<Object> TileMap::GetObjects()
{
	return m_objects;
}

void TileMap::SetDrawingBounds(sf::Rect<float> bounds)
{
    m_drawingBounds = bounds;

    //Adjust the rect so that tiles are drawn just off screen, so you don't see them disappearing.
	m_drawingBounds.top -= m_tileHeight;
	m_drawingBounds.left -= m_tileWidth;
	m_drawingBounds.height += m_tileHeight;
	m_drawingBounds.width += m_tileWidth;
}

void TileMap::Draw(sf::RenderWindow &window)
{
    for (unsigned int layer = 0; layer < m_layers.size(); layer++)
    {
        for (unsigned int tile = 0; tile < m_layers[layer].tiles.size(); tile++)
        {
            if (m_drawingBounds.contains(m_layers[layer].tiles[tile].getPosition().x, m_layers[layer].tiles[tile].getPosition().y))
            {
                window.draw(m_layers[layer].tiles[tile]);
            }
        }
    }
}

bool TileMap::SolidPixel(int x, int y)
{
	//NOTE this could be better optimised with a quad tree save testing every object in the map

	for(unsigned int i = 0; i < m_solidObjects.size(); i++)
	{
		if(m_solidObjects[i].contains(x, y)) return true;
	}

	return false;
}

bool TileMap::SolidPixel(sf::Vector2i pixelPosition)
{
	return SolidPixel(pixelPosition.x, pixelPosition.y);
}

void TileMap::m_UnloadMap(void)
{
	m_solidObjects.clear();
	m_objects.clear();
	m_layers.clear();
}

std::vector<unsigned char> TileMap::m_Decompress(const char* source, int inSize, int expectedSize)
{
	std::vector<unsigned char> retVal;
	int currentSize = expectedSize;
	unsigned char* byteArray = new unsigned char[expectedSize / sizeof(unsigned char)];
	z_stream stream;
	stream.zalloc = Z_NULL;
	stream.zfree = Z_NULL;
	stream.opaque = Z_NULL;
	stream.next_in = (Bytef*)source;
	stream.avail_in = inSize;
	stream.next_out = (Bytef*)byteArray;
	stream.avail_out = expectedSize;

	if(inflateInit2(&stream, 15 + 32) != Z_OK)
	{
		std::cout << "inflate 2 failed" << std::endl;
		return retVal;
	}

	int result;
	do
	{
		result = inflate(&stream, Z_SYNC_FLUSH);

		switch(result)
		{
		case Z_NEED_DICT:
		case Z_STREAM_ERROR:
			result = Z_DATA_ERROR;
		case Z_DATA_ERROR:
		case Z_MEM_ERROR:
			inflateEnd(&stream);
			std::cout << result << std::endl;
			return retVal;
		}

		if(result != Z_STREAM_END)
		{
			int oldSize = currentSize;
			currentSize *= 2;
			unsigned char* newArray = new unsigned char[currentSize / sizeof(unsigned char)];
			std::memcpy(newArray, byteArray, currentSize / 2);
			delete[] byteArray;
			byteArray = newArray;

			stream.next_out = (Bytef*)(byteArray + oldSize);
			stream.avail_out = oldSize;

		}
	}
	while(result != Z_STREAM_END);

	if(stream.avail_in != 0)
	{
		std::cout << "stream.avail_in is 0" << std::endl;
		return retVal;
	}

	const int outSize = currentSize - stream.avail_out;
	inflateEnd(&stream);

	unsigned char* newArray = new unsigned char[outSize / sizeof(unsigned char)];
	std::memcpy(newArray, byteArray, outSize);
	delete[] byteArray;
	byteArray = newArray;

	//copy bytes to vector
	int length = currentSize / sizeof(unsigned char);
	for(int i = 0; i < length; i++)
		retVal.push_back(byteArray[i]);
	delete[] byteArray;
	return retVal;
}

sf::Color TileMap::m_ColourFromHex(const char* hexStr)
{
	unsigned int value, r, g, b;
	std::stringstream input(hexStr);
	input >> std::hex >> value;

	r = (value >> 16) & 0xff;
	g = (value >> 8) & 0xff;
	b = value & 0xff;

	return sf::Color(r, g, b);
}

void TileMap::m_SetTile(std::vector<sf::Rect<int> >& subRects, Layer& layer, int x, int y, int GID)
{
	int subRectToUse = GID - m_firstTileID;//Work out the subrect ID to 'chop up' the tilesheet image.
	if (subRectToUse >= 0)//we only need to (and only can) create a sprite/tile if there is one to display
	{
		sf::Sprite sprite(m_tilesetTexture, subRects[subRectToUse]); //create sprite from texture and subrect
		sprite.setPosition((float)(x * m_tileWidth), (float)(y * m_tileHeight));
		sprite.setColor(sf::Color(255, 255, 255, layer.opacity));//Set opacity of the tile.

		//add tile to layer
		layer.tiles.push_back(sprite);
	}
}

//base64 decode function taken from:
/*
   base64.cpp and base64.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";


static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string)
{
	int in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
	{
		char_array_4[i++] = encoded_string[in_]; in_++;
		if (i ==4)
		{
			for (i = 0; i <4; i++)
				char_array_4[i] = base64_chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = base64_chars.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}
