add_library(animal STATIC
    animal.c
)

target_include_directories(animal PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_features(animal PUBLIC
    c_std_99
)

# MSVC 默认使用系统代码页，显式指定 UTF-8 以正确处理中文注释
if(MSVC)
    target_compile_options(animal PUBLIC
        /utf-8
    )
endif()
