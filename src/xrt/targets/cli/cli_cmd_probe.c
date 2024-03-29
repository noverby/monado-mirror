// Copyright 2019-2023, Collabora, Ltd.
// SPDX-License-Identifier: BSL-1.0
/*!
 * @file
 * @brief  Just does a probe.
 * @author Jakob Bornecrantz <jakob@collabora.com>
 */

#include "xrt/xrt_space.h"
#include "xrt/xrt_system.h"
#include "xrt/xrt_device.h"
#include "xrt/xrt_prober.h"
#include "xrt/xrt_instance.h"
#include "xrt/xrt_config_drivers.h"

#include "cli_common.h"

#include <string.h>
#include <stdio.h>

static int
do_exit(struct xrt_instance **xi_ptr, int ret)
{
	xrt_instance_destroy(xi_ptr);

	printf(" :: Exiting '%i'\n", ret);

	return ret;
}

#define NUM_XDEVS 32

int
cli_cmd_probe(int argc, const char **argv)
{
	struct xrt_instance *xi = NULL;
	xrt_result_t xret = XRT_SUCCESS;
	int ret = 0;

	// Initialize the prober.
	printf(" :: Creating instance!\n");

	ret = xrt_instance_create(NULL, &xi);
	if (ret != 0) {
		return do_exit(&xi, 0);
	}

	// Need to prime the prober with devices before dumping and listing.
	printf(" :: Creating system devices!\n");

	struct xrt_system *xsys = NULL;
	struct xrt_system_devices *xsysd = NULL;
	struct xrt_space_overseer *xso = NULL;
	xret = xrt_instance_create_system( //
	    xi,                            // Instance
	    &xsys,                         // System
	    &xsysd,                        // System devices.
	    &xso,                          // Space overseer.
	    NULL);                         // System compositor.
	if (xret != XRT_SUCCESS) {
		printf("\tCall to xrt_instance_create_system failed! '%i'\n", xret);
		return do_exit(&xi, -1);
	}
	if (xsysd == NULL) {
		printf("\tNo xrt_system_devices returned!\n");
		return do_exit(&xi, -1);
	}

	struct xrt_prober *xp = NULL;
	xret = xrt_instance_get_prober(xi, &xp);
	if (xret != XRT_SUCCESS) {
		printf("\tNo xrt_prober could be created!\n");
		return do_exit(&xi, -1);
	}

	size_t builder_count;
	struct xrt_builder **builders;
	size_t num_entries;
	struct xrt_prober_entry **entries;
	struct xrt_auto_prober **auto_probers;
	ret = xrt_prober_get_builders(xp, &builder_count, &builders, &num_entries, &entries, &auto_probers);
	if (ret != 0) {
		do_exit(&xi, ret);
	}

	printf(" :: Regular built in drivers\n");
	for (size_t i = 0; i < num_entries; i++) {
		if (entries[i] == NULL) {
			continue;
		}

		// devices with the same driver name are usually grouped, don't print duplicates
		if (i > 0 && strcmp(entries[i - 1]->driver_name, entries[i]->driver_name) == 0) {
			continue;
		}

		printf("\t%s\n", entries[i]->driver_name);
	}

	for (size_t i = 0; i < XRT_MAX_AUTO_PROBERS; i++) {
		if (auto_probers[i] == NULL) {
			continue;
		}

		printf("\t%s\n", auto_probers[i]->name);
	}

	printf(" :: Additional built in drivers\n");
// special cased drivers that are not probed through prober entries/autoprobers
#ifdef XRT_BUILD_DRIVER_REMOTE
	printf("\tRemote Debugging\n");
#endif

#ifdef XRT_BUILD_DRIVER_V4L2
	printf("\tv4l2\n");
#endif

#ifdef XRT_BUILD_DRIVER_VF
	printf("\tvf\n");
#endif

	printf(" :: Destroying probed devices\n");

	xrt_space_overseer_destroy(&xso);
	xrt_system_devices_destroy(&xsysd);
	xrt_system_destroy(&xsys);

	// End of program
	printf(" :: All ok, shutting down.\n");

	// Finally done
	return do_exit(&xi, 0);
}
