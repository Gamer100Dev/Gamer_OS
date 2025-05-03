#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <filesystem>

namespace fs = std::filesystem;

// write a length‑prefixed string
void writeString(std::ofstream& out, const std::string& str) {
    uint32_t len = static_cast<uint32_t>(str.length());
    out.write(reinterpret_cast<const char*>(&len), sizeof(len));
    out.write(str.c_str(), len);
}

int main() {
    std::string title, path, icon, author, output_path;

    std::cout << "=== .eg Game File Builder ===\n";

    std::cout << "Enter game title: ";
    std::getline(std::cin, title);

    std::cout << "Enter binary path: ";
    std::getline(std::cin, path);

    std::cout << "Enter icon path: ";
    std::getline(std::cin, icon);

    std::cout << "Enter author name: ";
    std::getline(std::cin, author);

    std::cout << "Enter output .eg file path (or directory): ";
    std::getline(std::cin, output_path);

    // 1) Strip surrounding single‑quotes if present
    if (output_path.size() > 1 &&
        output_path.front() == '\'' &&
        output_path.back()  == '\'') {
        output_path = output_path.substr(1, output_path.size() - 2);
    }

    fs::path outPath(output_path);

    // 2) If user gave a directory (or ended with '/'), synthesize filename
    if (fs::is_directory(outPath) || output_path.back() == '/') {
        // sanitize title to make a filename
        std::string fname = title;
        for (auto &c : fname) if (std::isspace((unsigned char)c)) c = '_';
        fname += ".eg";
        outPath /= fname;
    }

    // 3) Ensure parent directory exists
    if (outPath.has_parent_path()) {
        fs::create_directories(outPath.parent_path());
    }

    // 4) Open for binary writing
    std::ofstream out(outPath, std::ios::binary);
    if (!out) {
        std::cerr << "Error: could not open file for writing: "
                  << outPath << "\n";
        return 1;
    }

    // 5) Write fields
    writeString(out, title);
    writeString(out, path);
    writeString(out, icon);
    writeString(out, author);

    out.close();

    std::cout << "Success! .eg file written to: " << outPath << "\n";
    return 0;
}
