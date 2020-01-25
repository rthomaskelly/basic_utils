CXXFLAGS=-pthread -Wall -Wno-switch -std=c++1z -g
ROOT_DIR=.
BASE_SRC_DIR=..
COMMON_DIR=../..
THIRDPARTY_DIR=${COMMON_DIR}/3rdparty
GSL_DIR=${THIRDPARTY_DIR}/GSL/include/
TEST_DIR=${ROOT_DIR}/test
GTEST_DIR=${THIRDPARTY_DIR}/googletest/
GTEST_INCLUDE=${GTEST_DIR}/googletest/include/
GTEST_LIB=${GTEST_DIR}/build/lib/
INCLUDES=-I${BASE_SRC_DIR} -I${ROOT_DIR} -I${COMMON_DIR} -I${THIRDPARTY_DIR} -I${GSL_DIR} -I${GTEST_INCLUDE}
LIBS=-L${GTEST_LIB}
LINKS=-lgtest
TEST_TARGET=${TEST_DIR}/test
TARGET_1=test

$(BUILD):
	$(CXXFLAGS) $(INCLUDES) ./*.cpp -o $(TARGET)

$(TARGET_1):
	g++ $(CXXFLAGS) $(INCLUDES) $(LIBS) ./$(TARGET_1).cpp $(LINKS) -o bin/$(TARGET_1)

clean:
	rm -f bin/$(TARGET_1) *.o

clang:
	clang++ $(CXXFLAGS) $(INCLUDES) $(LIBS) ./$(TARGET_1).cpp $(LINKS) -o bin/$(TARGET_1)

debug:
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./*.cpp -o $(TARGET) -g

$(TEST_TARGET):
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/*.cpp -o$(TEST_TARGET) -g


