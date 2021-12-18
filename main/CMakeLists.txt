idf_component_register(SRCS "main.cpp" "motor.cpp" "scr_main.cpp" "single_encoder.cpp" INCLUDE_DIRS ".")

set_source_files_properties("main.cpp" "motor.cpp" "scr_main.cpp" "single_encoder.cpp"
PROPERTIES COMPILE_FLAGS
"-fpermissive -Wno-unused-variable -std=gnu++17"
)

# component_compile_options(-std=c++17).