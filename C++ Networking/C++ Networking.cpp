// C++ Networking.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "networking.hpp"

int main() {
    SE7::networking::html_node test(
        SE7::networking::html_id, 
        "html", 
        { 
            std::make_pair(36, SE7::networking::make_html_attribute_id_attribute("id-data")),
            std::make_pair(37, SE7::networking::make_html_attribute_class_attribute("class-data")),
        },
        {
            std::make_pair(
                27, 
                SE7::networking::html_node(
                    27, 
                    "blah",
                    {
                        std::make_pair(36, SE7::networking::make_html_attribute_id_attribute("id-data")),
                        std::make_pair(37, SE7::networking::make_html_attribute_class_attribute("class-data"))
                    },
                    {
                        std::make_pair(
                            28,
                            SE7::networking::html_node(28, "blah")
                        )
                    }
                )
            ),
            std::make_pair(
                28,
                SE7::networking::html_node(28, "blah")
            ),
            std::make_pair(
                29,
                SE7::networking::html_node(29, "blah")
            )
        }
    );

    std::cout << test.to_string_all_including_sub_tags(true, true) << "\n";

    SE7::networking::html_doc html_document;

    html_document.add_child(std::move(test));

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
