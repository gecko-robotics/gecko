find_package(nlohmann_json REQUIRED
    PATHS /opt/gecko/lib/cmake
)
if(nlohmann_json_FOUND)
    message(STATUS ">> json found: ${nlohmann_json_VERSION}")
endif()
