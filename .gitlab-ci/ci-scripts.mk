# SPDX-License-Identifier: CC0-1.0
# SPDX-FileCopyrightText: 2022 Collabora, Ltd. and the Monado contributors
#
# To generate all the templated files, run this from the root of the repo:
#   make -f .gitlab-ci/ci-scripts.mk

# These also all have their template named the same with a .template suffix.
FILES_IN_SUBDIR := \
    .gitlab-ci/distributions \
    .gitlab-ci/reprepro.sh \

CONFIG_FILE := .gitlab-ci/config.yml
all: .gitlab-ci.yml $(FILES_IN_SUBDIR)
.PHONY: all

clean:
	rm -f .gitlab-ci.yml $(FILES_IN_SUBDIR)
.PHONY: clean

# As the default thing for ci-fairy to template, this is special cased
.gitlab-ci.yml: .gitlab-ci/ci.template $(CONFIG_FILE)
	ci-fairy generate-template

# Everything else is structured alike
$(FILES_IN_SUBDIR): %: %.jinja $(CONFIG_FILE)
	ci-fairy generate-template --config=$(CONFIG_FILE) $< > $@
