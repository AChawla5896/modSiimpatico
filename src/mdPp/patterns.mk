# -----------------------------------------------------------------------
# File: src/mdPp/patterns.mk
#
# This makefile fragment contains the pattern rule used to compile all
# source files in the directory tree rooted at the directory src/mdPp, 
# which contains the source code for the MdPp namespace.  It must be 
# directly included by all makefile files in this directory tree, and 
# indirectly includes all relevant namespace-level configuration 
# (config.mk) and source-file list (source.mk) files from the src/mdPp, 
# src/simp and src/util directories.  This file must be included in 
# each makefile in src/mdPp after the definition of $(SRC_BLD_REL) and 
# after inclusion of the main build directory config file, 
# $(BLD_DIR)/config.mk.
# -----------------------------------------------------------------------
# Makefile fragment includes

# Build configuration files
# Define *_DEFS macro definitions, paths to libraries and executables
include $(BLD_DIR)/util/config.mk
include $(BLD_DIR)/simp/config.mk
include $(BLD_DIR)/mdPp/config.mk

# Source file lists
# Include recipes to build library targets
include $(SRC_DIR)/util/sources.mk
include $(SRC_DIR)/simp/sources.mk
include $(SRC_DIR)/mdPp/sources.mk

# -----------------------------------------------------------------------
# Makefile variable definitions

# Lists of all required C preprocessor macro definitions 
UTIL_DEFINES=$(UTIL_DEFS) 
SIMP_DEFINES=$(UTIL_DEFS) $(SIMP_DEFS)
MDPP_DEFINES=$(UTIL_DEFS) $(SIMP_DEFS) $(MDPP_DEFS)

# Lists of dependencies on configuration files 
# These are added to lists of dependencies generated by $(MAKEDEP)
UTIL_CFGS= -A$(BLD_DIR)/config.mk
UTIL_CFGS+= -A$(BLD_DIR)/util/config.mk
SIMP_CFGS= $(UTIL_CFGS)
SIMP_CFGS+= -A$(BLD_DIR)/simp/config.mk
MDPP_CFGS= $(SIMP_CFGS)
MDPP_CFGS+= -A$(BLD_DIR)/mdPp/config.mk

# All libraries needed by files in src/mdPp
LIBS=$(mdPp_LIB) $(simp_LIB) $(util_LIB)

# -----------------------------------------------------------------------
# Pattern rules

# Rule to compile all *.cpp class source files in src/mdPp
$(BLD_DIR)/mdPp/%.o: $(SRC_DIR)/mdPp/%.cpp
	@SDIR=$$(dirname "$@"); if [ ! -d "$$SDIR" ]; then mkdir -p "$$SDIR"; fi
	$(CXX) $(INCLUDES) $(MDPP_DEFINES) $(CXXFLAGS) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(MDPP_DEFINES) $(CXXFLAGS) $(MDPP_CFGS) -S$(SRC_DIR) -B$(BLD_DIR) $<
endif

# Rule to compile all *.cpp class source files in src/simp
$(BLD_DIR)/simp/%.o: $(SRC_DIR)/simp/%.cpp
	@SDIR=$$(dirname "$@"); if [ ! -d "$$SDIR" ]; then mkdir -p "$$SDIR"; fi
	$(CXX) $(INCLUDES) $(SIMP_DEFINES) $(CXXFLAGS) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(SIMP_DEFINES) $(CXXFLAGS) $(SIMP_CFGS) -S$(SRC_DIR) -B$(BLD_DIR) $<
endif

# Rule to compile all *.cpp class source files in src/util
$(BLD_DIR)/util/%.o: $(SRC_DIR)/util/%.cpp
	@SDIR=$$(dirname "$@"); if [ ! -d "$$SDIR" ]; then mkdir -p "$$SDIR"; fi
	$(CXX) $(INCLUDES) $(UTIL_DEFINES) $(CXXFLAGS) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(UTIL_DEFINES) $(CXXFLAGS) $(UTIL_CFGS) -S$(SRC_DIR) -B$(BLD_DIR) $<
endif

# Rule to compile all *.cc test programs in src/mdPp
$(BLD_DIR)/mdPp/tests/%.o: $(SRC_DIR)/mdPp/tests/%.cc
	$(CXX) $(INCLUDES) $(MDPP_DEFINES) $(TESTFLAGS) -c -o $@ $<
ifdef MAKEDEP
	$(MAKEDEP) $(INCLUDES) $(MDPP_DEFINES) $(TESTFLAGS) $(MDPP_CFGS) -S$(SRC_DIR) -B$(BLD_DIR) $<
endif

# Rule to link all *.cc test program executables in src/mdPp
$(BLD_DIR)/mdPp/tests/%: $(BLD_DIR)/mdPp/tests/%.o $(LIBS)
	$(CXX) -o $@ $< $(LIBS) $(LDFLAGS)

