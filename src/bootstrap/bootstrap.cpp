
#include "bootstrap.h"

namespace fs = boost::filesystem;


bool BOOTSTRAP::rmDirectory(const std::string& directory_path) {

    try {
        // Check if the directory exists
        if (fs::exists(directory_path)) {
            // Remove the directory and its contents
            fs::remove_all(directory_path);
            //std::cout << "Directory removed successfully." << std::endl;
        } else {
            //std::cerr << "Directory does not exist." << std::endl;
        }
    } catch (const fs::filesystem_error& ex) {
        //std::cerr << "Error removing directory: " << ex.what() << std::endl;
        return false;
    }

    return true;
}

bool BOOTSTRAP::isDirectory(const std::string& directory_path) {

    if (fs::exists(directory_path)) return true;

    return false;
}

// Callback function to write downloaded data to a file
size_t BOOTSTRAP::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::ofstream* file = static_cast<std::ofstream*>(userp);
    file->write(static_cast<const char*>(contents), total_size);
    return total_size;
}

// Function to download a file using libcurl
bool BOOTSTRAP::DownloadFile(const std::string& url, const std::string& outputFileName) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        //std::cerr << "Error initializing libcurl." << std::endl;
        return false;
    }

    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile.is_open()) {
        //std::cerr << "Error opening output file." << std::endl;
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); // Do not verify the peer's SSL certificate
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outputFile);

    CURLcode res = curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    outputFile.close();

    if (res != CURLE_OK) {
        //std::cerr << "Error downloading file: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}

bool BOOTSTRAP::extractZip(const std::string& zipFilePath, const std::string& outputFolderPath) {

    // Open the zip file
    unzFile zipFile = unzOpen(zipFilePath.c_str());
    if (!zipFile) {
        //std::cerr << "Error opening zip file: " << zipFilePath << std::endl;
        return false;
    }

    // Create the output folder if it doesn't exist
    if (!ensureOutputFolder(outputFolderPath)) {
        //std::cerr << "Error creating output folder: " << outputFolderPath << std::endl;
        unzClose(zipFile);
        return false;
    }

    // Go through each file in the zip and extract it
    unz_global_info globalInfo;
    if (unzGetGlobalInfo(zipFile, &globalInfo) != UNZ_OK) {
        //std::cerr << "Error getting global info from zip file." << std::endl;
        unzClose(zipFile);
        return false;
    }

    for (uLong i = 0; i < globalInfo.number_entry; ++i) {
        char fileName[256];
        unz_file_info fileInfo;

        if (unzGetCurrentFileInfo(zipFile, &fileInfo, fileName, sizeof(fileName), nullptr, 0, nullptr, 0) != UNZ_OK) {
            //std::cerr << "Error getting file info from zip file." << std::endl;
            unzClose(zipFile);
            return false;
        }

        if (unzOpenCurrentFile(zipFile) != UNZ_OK) {
            //std::cerr << "Error opening current file in zip." << std::endl;
            unzClose(zipFile);
            return false;
        }

        std::string outputPath = std::string(outputFolderPath) + "/" + fileName;

        if(endsWithSlash(outputPath))
            ensureOutputFolder(outputPath);
        else{
            std::ofstream outFile(outputPath, std::ios::binary);
            if (!outFile.is_open()) {
                //std::cerr << "Error creating output file: " << outputPath << std::endl;
                unzCloseCurrentFile(zipFile);
                unzClose(zipFile);
                return false;
            }

            // Read and write the file data
            char buffer[4096];
            int bytesRead;

            while ((bytesRead = unzReadCurrentFile(zipFile, buffer, sizeof(buffer))) > 0) {
                outFile.write(buffer, bytesRead);
            }
            outFile.close();
        }


        unzCloseCurrentFile(zipFile);

        if (unzGoToNextFile(zipFile) != UNZ_OK) {
            break;  // Reached the end of the zip file
        }
    }

    // Close the zip file
    unzClose(zipFile);
    //std::cout << "Zip extraction successful." << std::endl;

    return true;

}

bool BOOTSTRAP::ensureOutputFolder(const std::string& outputPath) {
    try {
        if (!fs::exists(outputPath)) {
            // Create the directory if it doesn't exist
            fs::create_directories(outputPath);
        } else if (!fs::is_directory(outputPath)) {
            // If it exists but is not a directory, print an error
            //std::cerr << "Error: Output path '" << outputPath << "' is not a directory." << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        // Handle any exceptions that may occur during filesystem operations
        //std::cerr << "Error creating output folder: " << e.what() << std::endl;
        return false;
    }

    return true;
}

bool BOOTSTRAP::endsWithSlash(const std::string& str) {
    // Check if the string ends with '/'
    return !str.empty() && str.back() == '/';
}
