# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/Leticia/esp/v4.4.2/esp-idf/components/bootloader/subproject"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix/tmp"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix/src"
  "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/Leticia/Documents/Projetos_ESP_IDF/simple_https_server/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
