
// #include the hpp file for tests.
#include "test.hpp"
#include "test-ssd1306.hpp"

namespace CStest {

    void runTests(const CScore::Assertion::verbosity level) {


        TestSsd1306 testSsd1306(level, "TestSsd1306");
        testSsd1306.doTests();
    }


    void Test::executeAll() const {
        for (const auto& func : functionList_) {
            func(getVerbosityLevel());
        }
    }

    void Test::displayMessage(const CScore::Assertion::verbosity level, const std::string &message) {

        if (CScore::Assertion::VERBOSE == level) {
            std::cout << message << std::endl;
        }
    }

    void Test::displayMessage(const std::string &message) const {
        displayMessage(getVerbosityLevel(), message);
    }
}
