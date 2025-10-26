#include <iostream>
#include <fstream>
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

// Read CSV file
std::vector<Text> readCSV(const std::string &filename) {
    std::vector<Text> texts;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return texts;
    }

    std::string line;
    if (!std::getline(file, line)) return texts; // skip header
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string idStr, content;
        if (!std::getline(ss, idStr, ',')) continue;
        if (!std::getline(ss, content)) continue;

        Text t;
        t.id = std::stoi(idStr);
        t.content = content;
        texts.push_back(t);
    }
    return texts;
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

    // Mode 2: Send random text from CSV
    std::string csvPath = "/Users/tapdiyaom/Desktop/typing-speed-website/backend/text_database.csv";
    std::vector<Text> texts = readCSV(csvPath);

    // Output JSON (C++ backend prints content-type header only for CGI)
    std::cout << "Content-type: application/json\n\n";

    if (texts.empty()) {
        std::cout << "{ \"error\": \"No texts found\" }";
        return 0;
    }

    std::srand(std::time(nullptr));
    int idx = std::rand() % texts.size();
    Text randomText = texts[idx];

    std::cout << "{ \"id\": " << randomText.id
              << ", \"text\": \"" << escapeJSON(randomText.content) << "\" }";

    return 0;
}