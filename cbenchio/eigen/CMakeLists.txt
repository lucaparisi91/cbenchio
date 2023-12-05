include(FetchContent)
FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG master
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE)
set(EIGEN_BUILD_DOC "VALUE" CACHE BOOL OFF )
set(BUILD_TESTING "VALUE" CACHE BOOL OFF )
set(EIGEN_BUILD_PKGCONFIG "VALUE" CACHE BOOL OFF )

FetchContent_MakeAvailable(Eigen)