#include "stdio.h"
#include "string.h"

#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree-pass.h"

int plugin_is_GPL_compatible;

extern opt_pass *current_pass;

static struct plugin_info plugin_name = {"0.1", "GCC pass reorder experiment"};

///prints as much information about a pass as it can, human-readable
static void pass_dump_gate_callback(void* gcc_data, void* user_data) {
	if (current_pass) {
		printf("\n");
		printf("Name: %s\nProperties:\n\tRequired: 0x%x\n\tProvided: 0x%x\n\tDestroyed: 0x%x\n", 
				current_pass->name, 
				current_pass->properties_required, 
				current_pass->properties_provided, 
				current_pass->properties_destroyed);
		printf("Gate status: %d\n", *(int*)gcc_data);
		printf("Subpasses: %s", current_pass->sub ? "yes\n" : "no\n");
		printf("TODO before: 0x%X\n", current_pass->todo_flags_start);
		printf("TODO after: 0x%X\n", current_pass->todo_flags_finish);
	}
}

///prints pass data in format "name properties_required properties_provided properties_broken"
static void pass_dump_short_gate_callback(void* gcc_data, void* user_data) {
	if (current_pass) {
		printf("%s %d %d %d\n", current_pass->name, 
				current_pass->properties_required, 
				current_pass->properties_provided, 
				current_pass->properties_destroyed);
	}
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
	register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, &plugin_name);

	///choose dump format based on "dump_format" (short/long) argument
	for (int i = 0; i < plugin_info->argc; i++) {
		if (!strcmp(plugin_info->argv[i].key, "dump_format")) {
			if (!strcmp(plugin_info->argv[i].value, "short")) {
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, pass_dump_short_gate_callback, NULL);
			} else if (!strcmp(plugin_info->argv[i].value, "long")) {
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, pass_dump_gate_callback, NULL);
			} else {
				printf("Incorrect plugin dump_format value\n");
			}
		}
	}
	
	return 0;
}
