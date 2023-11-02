# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/matiasnd/esp/esp-idf/components/bootloader/subproject"
  "/home/matiasnd/awg/ESP_code/build/bootloader"
  "/home/matiasnd/awg/ESP_code/build/bootloader-prefix"
  "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/tmp"
  "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/src/bootloader-stamp"
  "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/src"
  "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/matiasnd/awg/ESP_code/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
