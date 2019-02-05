#include <nlohmann/json.hpp>
#include "base64.h"

#include <iostream>

using json = nlohmann::json;

int main(int argc, char* argv[])
{
    std::vector<char> data(1024 * 1024);
    for (int i = 0; i < 10; ++i)
    {
        json doc;
        doc["gnaa"] = "gnuff";
        doc["haus"] = 1.1234;
        doc["payload"] = base64_encode((const unsigned char*)data.data(), data.size());

        auto jsonOutput = doc.dump();

        std::cout << jsonOutput.size() << std::endl;
    }
    return 0;
}