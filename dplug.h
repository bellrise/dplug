/* dplug.h - utility for building loadable modules
   Copyright (c) 2023 bellrise */

#ifndef DPLUG_H
#define DPLUG_H

struct dplug;

typedef void *(*dplug_method_f)(void *);
typedef int (*dplug_open_f)(void);
typedef int (*dplug_close_f)(void);

struct dplug_method
{
	const char *e_name;
	dplug_method_f e_method;
};

struct dplug_decl
{
	const char *d_name;
	const char *d_desc;
	int d_version;
	dplug_open_f d_open;
	dplug_close_f d_close;
	struct dplug_method d_methods[];
};

#define DPLUG_OK     0
#define DPLUG_FAIL   1
#define DPLUG_NODECL 2

struct dplug *dplug_load(const char *path);
int dplug_close(struct dplug *plugin);
void *dplug_call(struct dplug *plugin, const char *name, void *arg);

#define DPLUG_PLUGIN(...)                                                      \
 extern const struct dplug_decl __dplug_v1_decl;                               \
 const struct dplug_decl __dplug_v1_decl = __VA_ARGS__;

#endif /* DPLUG_H */
