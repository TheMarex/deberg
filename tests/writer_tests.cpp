#include "../line_writer.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

BOOST_AUTO_TEST_SUITE(writer_tests)

BOOST_AUTO_TEST_CASE(line_writer_tests)
{
    std::vector<poly_line> lines {
        {0, {coordinate {0, 0}, coordinate {1, 1}, coordinate {1, 0}, coordinate {2, 3}}},
        {1, {coordinate {0, 0}, coordinate {1, 1}}},
        {2, {coordinate {0.555555, 0}, coordinate {1, 1}}},
    };
    std::stringstream output;
    line_writer writer(output);

    writer.write(lines);

    std::string correct_output = "0:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">0,0 1,1 1,0 2,3 </gml:coordinates></gml:LineString>\n1:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">0,0 1,1 </gml:coordinates></gml:LineString>\n2:<gml:LineString srsName=\"EPSG:54004\" xmlns:gml=\"http://www.opengis.net/gml\"><gml:coordinates decimal=\".\" cs=\",\" ts=\" \">0.555555,0 1,1 </gml:coordinates></gml:LineString>\n";

    BOOST_CHECK_EQUAL(correct_output, output.str());

}

BOOST_AUTO_TEST_SUITE_END()
