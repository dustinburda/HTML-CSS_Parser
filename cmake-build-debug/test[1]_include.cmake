if(EXISTS "/Users/dustin/Desktop/Programming/C++ Programming/BrowserEngineInternals/cmake-build-debug/test[1]_tests.cmake")
  include("/Users/dustin/Desktop/Programming/C++ Programming/BrowserEngineInternals/cmake-build-debug/test[1]_tests.cmake")
else()
  add_test(test_NOT_BUILT test_NOT_BUILT)
endif()