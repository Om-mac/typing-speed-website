#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

struct Text {
    int id;
    std::string content;
};

// Escape JSON special characters
std::string escapeJSON(const std::string &s) {
    std::string result;
    for (char c : s) {
        switch(c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            default: result += c; break;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {

    // Mode 1: Calculate WPM if 2 arguments are passed
    if (argc == 3) {
        std::string typed = argv[1];
        double timeTaken = std::stod(argv[2]); // seconds

        std::istringstream iss(typed);
        int words = 0;
        std::string word;
        while (iss >> word) words++;

        int wpm = static_cast<int>((words / timeTaken) * 60);
        std::cout << "{ \"wpm\": " << wpm << " }";
        return 0;
    }

    // Mode 2: Send random text from internal database
    std::vector<Text> texts = {
        {1, "The quick brown fox jumps over the lazy dog."},
        {2, "Typing speed measures how fast and accurately you can type on a keyboard."},
        {3, "C++ is a powerful language used for systems, games, and backend applications."},
        {4, "Consistency and daily practice are key to improving your typing skills."},
        {5, "Artificial Intelligence is transforming industries and shaping the future of technology."},
        {6, "Backend development handles data, authentication, and server logic."},
        {7, "Always write clean, maintainable, and well-documented code."},
        {8, "Coding challenges are a great way to enhance problem-solving skills."},
        {9, "Success comes from small efforts repeated daily."},
        {10, "Speed is useful only when accuracy is not compromised."}
    };

    // Output JSON (for CGI-based backend)
    std::cout << "Content-type: application/json\n\n";

    if (texts.empty()) {
        std::cout << "{ \"error\": \"No texts available\" }";
        return 0;
    }

    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int idx = std::rand() % texts.size();
    Text randomText = texts[idx];

    std::cout << "{ \"id\": " << randomText.id
              << ", \"text\": \"" << escapeJSON(randomText.content) << "\" }";

    return 0;
}