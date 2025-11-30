# ----- REAL TARGETS -----

.ver: ${PROJ_NAME}.h
	@ver -s -n lib${PROJ_NAME} -d ${PROJ_DESC}
	@echo "Updated project metadata"
	
.__headlib__:
	@touch .__headlib__
	@echo "Updated header-only library marker file"

# ----- PHONY TARGETS -----

.PHONY: \
	all \
	clean \
    init \
    init-git \
	install \
	uninstall \
	ver

all: ${PROJ_NAME}.h .ver .__headlib__
	@echo "Built lib${PROJ_NAME} version ${PROJ_VER} (build $$(ver -b))"

clean:
	@rm -rf build
	@echo "All build artifacts removed"

init:
	@touch ${PROJ_NAME}.h
	@echo "Created ${PROJ_NAME}.h"
	@touch .gitignore
	@echo "Created .gitignore"
	@touch .__headlib__
	@echo "Created .__headlib__"
	@echo "Project successfully initialized"

init-git:
	@git init
	@echo "Initialized empty git repository"
	@git add --all
	@echo "Added all existing files to the repository"
	@git commit -m "v${PROJ_VER}; init commit"
	@echo "Created first commit"
	@git branch -M main
	@echo "Set the primary branch to 'main'"
	@git remote add origin git@github.com:kawright/${PROJ_NAME}.git
	@echo "Linked repository to remote GitHub repository"
	@git push -u origin main
	@echo "Pushed first commit to remote repository"
	@echo "Git repository successfully initialized"

install:
	@mkdir -p /usr/local/etc/lib${PROJ_NAME}
	@cp ${PROJ_NAME}.h /usr/local/include/${PROJ_NAME}.h
	@echo "Copied ${PROJ_NAME}.h"
	@cp .ver /usr/local/etc/lib${PROJ_NAME}/.ver
	@echo "Copied .ver"
	@cp .__headlib__ /usr/local/etc/lib${PROJ_NAME}/.__headlib__
	@echo "Copied .__headlib__"
	@echo "lib${PROJ_NAME} version ${PROJ_VER} successfully installed"

uninstall:
	@rm /usr/local/include/${PROJ_NAME}.h
	@echo "Deleted ${PROJ_NAME}.h"
	@rm -rf /usr/local/etc/lib${PROJ_NAME}
	@echo "Deleted all configuration files"
	@echo "lib${PROJ_NAME} successfully uninstalled"

ver:
	@ver -V ${PROJ_VER}
	@echo "Updated version number to ${PROJ_VER}"
