util_tests_signal_=util/tests/signal/Test.cpp

util_tests_signal_SRCS=\
     $(addprefix $(SRC_DIR)/, $(util_tests_signal_))
util_tests_signal_OBJS=\
     $(addprefix $(OBJ_DIR)/, $(util_tests_signal_:.cpp=.o))
