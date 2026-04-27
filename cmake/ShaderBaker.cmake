find_program(DXC_COMPILER dxc PATHS ENV VULKAN_SDK PATH_SUFFIXES bin)
if(NOT DXC_COMPILER)
    message(FATAL_ERROR "Could not find dxc.exe! Ensure Vulkan SDK is installed.")
endif()

set(SHADER_OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/NONG/src/compiled_shaders")
file(MAKE_DIRECTORY ${SHADER_OUT_DIR})

set(GEN_CPP_CODE "#include \"NONG/shader.h\"\n\nnamespace NONG::BakedShaders {\n")

# Find ALL .hlsl files (both vert and frag)
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/NONG/src/shaders/*.hlsl")
set(COMPILED_SHADER_FILES "")

foreach(SHADER_FILE IN LISTS SHADER_FILES)
    # Extract "sprite.vert.hlsl"
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME) 
    # Strip to "sprite.vert"
    string(REPLACE ".hlsl" "" BASE_NAME ${FILE_NAME})     
    # Convert "sprite.vert" to "sprite_vert" for the C++ struct name
    string(REPLACE "." "_" CPP_NAME ${BASE_NAME})         

    # Determine compiler flags based on the file extension
    if(BASE_NAME MATCHES "\\.vert$")
        set(DXC_PROFILE "vs_6_0")
        set(DXC_ENTRY "VSMain")
    elseif(BASE_NAME MATCHES "\\.frag$")
        set(DXC_PROFILE "ps_6_0")
        set(DXC_ENTRY "PSMain")
    else()
        continue() # Skip unrecognized files
    endif()

    set(OUT_DXIL "${SHADER_OUT_DIR}/${BASE_NAME}.dxil")
    set(OUT_SPV  "${SHADER_OUT_DIR}/${BASE_NAME}.spv")
    
    list(APPEND COMPILED_SHADER_FILES ${OUT_DXIL} ${OUT_SPV})

    # The actual compilation commands
    add_custom_command(
        OUTPUT ${OUT_DXIL} ${OUT_SPV}
        # Added -Qstrip_debug to prevent bloat, and -Vi to force dxc to print validation status
        COMMAND ${DXC_COMPILER} -T ${DXC_PROFILE} -E ${DXC_ENTRY} -Qstrip_debug -Vi -Fo ${OUT_DXIL} ${SHADER_FILE}
        COMMAND ${DXC_COMPILER} -spirv -fvk-use-dx-layout -T ${DXC_PROFILE} -E ${DXC_ENTRY} -Fo ${OUT_SPV} ${SHADER_FILE}
        DEPENDS ${SHADER_FILE}
    )

    # Generate the isolated C++ struct!
    string(APPEND GEN_CPP_CODE "
    const unsigned char ${CPP_NAME}_spv[] = { 
        #embed \"${OUT_SPV}\" 
    };
    const unsigned char ${CPP_NAME}_dxil[] = { 
        #embed \"${OUT_DXIL}\" 
    };

    extern const EmbeddedShaderData ${CPP_NAME} = {
        ${CPP_NAME}_spv, sizeof(${CPP_NAME}_spv),
        ${CPP_NAME}_dxil, sizeof(${CPP_NAME}_dxil)
    };
")
endforeach()

string(APPEND GEN_CPP_CODE "}\n")

set(GENERATED_CPP_FILE "${SHADER_OUT_DIR}/generated_shaders.cpp")
file(WRITE ${GENERATED_CPP_FILE} "${GEN_CPP_CODE}")

set_source_files_properties(${GENERATED_CPP_FILE} PROPERTIES OBJECT_DEPENDS "${COMPILED_SHADER_FILES}")
add_custom_target(CompileShaders ALL DEPENDS ${COMPILED_SHADER_FILES})