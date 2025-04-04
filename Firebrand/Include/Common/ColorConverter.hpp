#ifndef __COLOR_CONVERTER_H__
#define __COLOR_CONVERTER_H__

#include <SFML/Graphics/Color.hpp>

namespace ColorConverter {
    sf::Color RgbToHsv(sf::Color rgb);
    sf::Color HsvToRgb(sf::Color hsv);
}
#endif