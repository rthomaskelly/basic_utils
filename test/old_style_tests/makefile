CXX=g++
CXXFLAGS=-pthread -Wall -Wno-switch -std=c++1z
ROOT_DIR=.
BASE_SRC_DIR=..
COMMON_DIR=../..
GSL_DIR=${COMMON_DIR}/GSL/include/
TEST_DIR=${ROOT_DIR}/test
INCLUDES=-I${BASE_SRC_DIR} -I${ROOT_DIR} -I${COMMON_DIR} -I${GSL_DIR}
TEST_TARGET=${TEST_DIR}/test
TARGET_1=boxed_types_test
TARGET_2=traits_test
TARGET_3=string_predicates_test
TARGET_4=graph_test
TARGET_5=sparse_square_matrix_test
TARGET_6=deferred_graph_test
TARGET_7=iterable_algorithms_test
TARGET_8=rank_test
TARGET_9=statistics_test

$(BUILD):
	$(CXXFLAGS) $(INCLUDES) ./*.cpp -o $(TARGET)

$(TARGET_1):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_1).cpp -o bin/$(TARGET_1)
$(TARGET_2):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_2).cpp -o bin/$(TARGET_2)
$(TARGET_3):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_3).cpp -o bin/$(TARGET_3)
$(TARGET_4):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_4).cpp -o bin/$(TARGET_4)
$(TARGET_5):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_5).cpp -o bin/$(TARGET_5)
$(TARGET_6):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_6).cpp -o bin/$(TARGET_6)
$(TARGET_7):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_7).cpp -o bin/$(TARGET_7)
$(TARGET_8):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_8).cpp -o bin/$(TARGET_8)
$(TARGET_9):
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./$(TARGET_9).cpp -o bin/$(TARGET_9)

clean:
	rm -f bin/$(TARGET_1) *.o
	rm -f bin/$(TARGET_2) *.o
	rm -f bin/$(TARGET_3) *.o
	rm -f bin/$(TARGET_4) *.o
	rm -f bin/$(TARGET_5) *.o
	rm -f bin/$(TARGET_6) *.o
	rm -f bin/$(TARGET_7) *.o
	rm -f bin/$(TARGET_8) *.o
	rm -f bin/$(TARGET_9) *.o

clang:
	clang++ $(CXXFLAGS) $(INCLUDES) ./*.cpp -o $(TARGET)

debug:
	$(CXX) $(CXXFLAGS) $(INCLUDES) ./*.cpp -o $(TARGET) -g

$(TEST_TARGET):
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(TEST_DIR)/*.cpp -o$(TEST_TARGET) -g


