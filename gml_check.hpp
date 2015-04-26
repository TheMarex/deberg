#ifndef GML_CHECK_H
#define GML_CHECK_H

#include <tinyxml2/tinyxml2.h>

#include <tuple>
#include <vector>
#include <sstream>

void gml_check(tinyxml2::XMLElement* root, const char* root_tag)
{
    if (root == nullptr)
        throw std::runtime_error("Invalid xml");
    if (root->NoChildren())
        throw std::runtime_error("Root has not children.");
    if (root->FirstChild()->ToElement()->Attribute("decimal") == nullptr)
        throw std::runtime_error("Coordinates has no decimal attribute.");
    if (root->FirstChild()->ToElement()->Attribute("cs") == nullptr)
        throw std::runtime_error("Coordinates has no cs attribute.");
    if (root->FirstChild()->ToElement()->Attribute("ts") == nullptr)
        throw std::runtime_error("Coordinates has no ts attribute.");

    using value_check = std::tuple<const char*, const char*, const char*>;
    std::vector<value_check> tag_checks = {
        value_check("", root_tag, root->Name()),
        value_check("", "gml:coordinates", root->FirstChild()->ToElement()->Name()),
        value_check("", ".", root->FirstChild()->ToElement()->Attribute("decimal")),
        value_check("", ",", root->FirstChild()->ToElement()->Attribute("cs")),
        value_check("", " ", root->FirstChild()->ToElement()->Attribute("ts")),
    };

    for (const auto& check : tag_checks) {
        if (std::string(std::get<1>(check)) != std::get<2>(check))
        {
            std::stringstream msg;
            msg << std::get<0>(check) << " " << std::get<1>(check) << " is not " << std::get<2>(check);
            throw std::runtime_error(msg.str());
        }
    }
}

#endif
