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

#ifndef TMX_LOADER_H
#define TMX_LOADER_H

#include <string>
#include <vector>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>


namespace tmx
{
	//Object class. Everything is kept public in case you want to get a different type to the supported ones.
	//Also you will be able to modify things. For example doors/changing walls etc (Just give the door a static type and a unique name)
	//Kept in same files as level just for convenience. (only have to add two files to your project)
	class Object
	{
		public:
			Object();
			int GetPropertyInt(std::string name, int defaultValue) const;
			float GetPropertyFloat(std::string name, float defaultValue) const;
			std::string GetPropertyString(std::string name, std::string defaultValue) const;
			std::string name;
			std::string type;
			sf::Rect <int> rect;
			std::map <std::string, std::string> properties;//All properties of the object. Values are stored as strings and mapped by strings(names provided in editor).
			int gid, visible; //optional object attributes
			std::vector<sf::Vector2i> polypoints; //list of points stored in polyline and polygon objects
	};

	struct Layer
	{
		int opacity;
		std::vector <sf::Sprite> tiles;
	};

	class TileMap
	{
		public:
			TileMap();
			virtual ~TileMap();
			//Loads the map. Returns false if it fails.
			bool LoadFromFile(std::string filename, std::string filepath = "");
			//Returns the first object found with the given name. This is why we need unique names :)
			Object GetObject(std::string name);
			// Get all of the objects
			std::vector<Object> GetObjects();
			//Returns true if the given pixel is solid.
			bool SolidPixel(int x, int y);
			bool SolidPixel(sf::Vector2i pixelPosition);
			//Set the area to draw. This rect is usually provided directly from the view you are using.
			void SetDrawingBounds(sf::Rect<float> bounds);
			//Draws the map to the provided window.
			void Draw(sf::RenderWindow &window);

		private:
			//Width(tiles), height(tiles), tile width(pixels), tile height(pixels) of the map.
			int m_width, m_height, m_tileWidth, m_tileHeight;
			//spacing between tiles on the tile image
			int m_spacing, m_margin;
			//Used to offset the tile number.
			int m_firstTileID;
			//Used to clip anything off screen.
			sf::Rect<float> m_drawingBounds;
			//The tileset image.
			sf::Image m_tilesetImage;
			sf::Texture m_tilesetTexture;
			//This stores all the solid areas (objects with type 'solid'). This gets checked by the IsSolid function.
			std::vector< sf::Rect <int> > m_solidObjects;
			//This stores all objects (including 'solid' types)
			std::vector<Object> m_objects;
			//This stores each layer of sprites/tiles so they can be drawn in order.
			std::vector <Layer> m_layers;
			//Unloads existing map data so we can load a new map into the same instance
			void m_UnloadMap(void);
			//method for decompressing zlib compressed strings
			std::vector<unsigned char> m_Decompress(const char* source, int inSize, int expectedSize);
			//utility for converting hex string to sf::color
			sf::Color m_ColourFromHex(const char* hexStr);
			//sets a tile on a layer from given GID and position
			void m_SetTile(std::vector< sf::Rect<int> >& subRects, Layer& layer, int x, int y, int GID);
	};

};

//method for decoding base64 encoded strings
std::string base64_decode(std::string const& string);



#endif // TMX_LOADER_H
