#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <ctime>
#include <nlohmann/json.hpp>
#include <httplib.h>

struct FileInfo {
    std::string name;
    bool isDirectory;
    std::uintmax_t size;
    std::string lastModified;
};

// 获得目录下的文件信息
std::vector<FileInfo> getFiles(const std::string &directoryPath) {
    std::vector<FileInfo> files;
    for (const auto &entry : std::filesystem::directory_iterator(directoryPath)) {
        FileInfo fileInfo;
        fileInfo.name = entry.path().filename().string();
        fileInfo.isDirectory = entry.is_directory();
        fileInfo.size = entry.is_directory() ? 0 : entry.file_size();

        auto ftime = entry.last_write_time(); // 文件最后修改时间
        auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
            ftime - decltype(ftime)::clock::now() + std::chrono::system_clock::now()); // 转换为系统时钟时间
        std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);
        fileInfo.lastModified = std::ctime(&cftime);

        files.push_back(fileInfo);
        // std::cout << fileInfo.name << std::endl;
    }
    return files;
}

int main() {
    httplib::Server svr;

    // 中间件：为所有响应添加 CORS 头
    svr.set_pre_routing_handler([](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        return httplib::Server::HandlerResponse::Unhandled;
    });

    svr.Options("/api/files", [](const httplib::Request &, httplib::Response &res) {
        res.set_content("", "text/plain");
    });

    svr.Get("/api/files", [](const httplib::Request &, httplib::Response &res) {
        std::cout << "Received request for /api/files" << std::endl;
        auto files = getFiles("./test");
        nlohmann::json jsonFiles = nlohmann::json::array();
        for (const auto &file : files) {
            jsonFiles.push_back({{"name", file.name},
                                 {"isDirectory", file.isDirectory},
                                 {"size", file.size},
                                 {"lastModified", file.lastModified}});
        }
        res.set_content(jsonFiles.dump(), "application/json");
    });

    std::cout << "Server is running on http://localhost:3001" << std::endl;
    if (!svr.listen("localhost", 3001)) {
        std::cerr << "Error starting server!" << std::endl;
        return 1;
    }

    return 0;
}