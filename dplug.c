/* dplug.c - dplug implementation
   Copyright (c) 2023 bellrise */

#include "dplug.h"

#include <dlfcn.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct dplug
{
	void *handle;
	struct dplug_decl *decl;
	int n_methods;
	struct dplug_method *methods;
};

struct dplug *dplug_load(const char *path)
{
	struct dplug_method *method_walker;
	struct dplug *plugin;

	plugin = malloc(sizeof(*plugin));
	memset(plugin, 0, sizeof(*plugin));

	plugin->handle = dlopen(path, RTLD_LAZY);
	if (!plugin->handle)
		goto fail;

	plugin->decl = dlsym(plugin->handle, "__dplug_v1_decl");
	if (!plugin->decl)
		goto fail;

	if (plugin->decl->d_open)
		plugin->decl->d_open();

	plugin->methods = plugin->decl->d_methods;

	while (1) {
		method_walker = &plugin->methods[plugin->n_methods];
		if (!method_walker->e_name)
			break;
		plugin->n_methods++;
	}

	return plugin;
fail:
	free(plugin);
	return NULL;
}

int dplug_close(struct dplug *plugin)
{
	if (plugin->decl->d_close)
		plugin->decl->d_close();

	dlclose(plugin->handle);
	free(plugin);
	return 0;
}

void *dplug_call(struct dplug *plugin, const char *name, void *arg)
{
	struct dplug_method *method;

	for (int i = 0; i < plugin->n_methods; i++) {
		method = &plugin->methods[i];
		if (strcmp(name, method->e_name))
			continue;
		return method->e_method(arg);
	}

	return NULL;
}
