BLD_DIR_REL =../../..
include $(BLD_DIR_REL)/config.mk
include $(SRC_DIR)/mcMd/patterns.mk

all: $(mcMd_analyzers_simulation_OBJS)

clean:
	rm -f $(mcMd_analyzers_simulation_OBJS) $(mcMd_analyzers_simulation_OBJS:.o=.d)

clean-deps:
	rm -f $(mcMd_analyzers_simulation_OBJS:.o=.d)

-include $(mcMd_analyzers_simulation_OBJS:.o=.d)

