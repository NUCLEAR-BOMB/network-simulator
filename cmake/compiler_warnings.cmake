include_guard(GLOBAL)

function(target_add_warnings target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE 
        /Wall /WX /permissive- /Za
        /wd5027 # 'type': move assignment operator was implicitly defined as deleted
        /wd4626 # 'derived class': assignment operator was implicitly defined as deleted because a base class assignment operator is inaccessible or deleted
        /wd5026 # 'type': move constructor was implicitly defined as deleted
        /wd4625 # 'derived class': copy constructor was implicitly defined as deleted because a base class copy constructor is inaccessible or deleted
        /wd5264 # 'variable-name': 'const' variable is not used
        /wd5262 # implicit fall-through occurs here between cases
        /wd4820 # 'bytes' bytes padding added after construct 'member_name'
        /wd4514 # 'function' : unreferenced inline function has been removed
        /wd5246 # 'member': the initialization of a subobject should be wrapped in braces
        /wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
        /wd4710 # 'function' : function not inlined
        /wd4711 # function 'function' selected for inline expansion
        /wd4866 # 'file(line_number)' compiler may not enforce left-to-right evaluation order for call to operator_name
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE
        -Wall # Enables all the warnings
        -Wextra # Enables some extra warning flags that are not enabled by -Wall
        -Wpedantic # Issue all the warnings demanded by strict ISO C and ISO C++
        -Wconversion # Warn for implicit conversions that may alter a value
        -Werror # Make all warnings into errors.
        -Wcast-align # Warn whenever a pointer is cast such that the required alignment of the target is increased
        -Wunused # -Wunused options combined
        -Wconversion # Warn for implicit conversions that may alter a value
        -Wsign-conversion # Warn for implicit conversions that may change the sign of an integer value
        -Wdouble-promotion # Give a warning when a value of type float is implicitly promoted to double
        -Wold-style-cast # Warn for C style casting
        -Wshadow # Warn whenever a local variable or type declaration shadows another variable
        )
    endif()
endfunction()
