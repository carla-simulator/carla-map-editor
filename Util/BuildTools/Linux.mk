default: help

help:
	@less ${CARLA_BUILD_TOOLS_FOLDER}/Linux.mk.help

launch: CarlaEditor
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --build --launch

launch-only:
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaUE4.sh --launch

package: CarlaEditorEditor
	@${CARLA_BUILD_TOOLS_FOLDER}/Package.sh $(ARGS)

docs:
	@doxygen
	@echo "Documentation index at ./Doxygen/html/index.html"

clean:
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaEditor.sh --clean

rebuild: setup
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaEditor.sh --rebuild

hard-clean:
	@${CARLA_BUILD_TOOLS_FOLDER}/BuildCarlaEditor.sh --hard-clean
	@echo "To force recompiling dependencies run: rm -Rf ${CARLA_BUILD_FOLDER}"
