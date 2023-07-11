#include "stdio.h"
#include "string.h"

#include "gcc-plugin.h"
#include "plugin-version.h"

#include "tree-pass.h"
#include "context.h"
#include "pass_manager.h"
#include "function.h"
#include <typeinfo>

int plugin_is_GPL_compatible;

extern char *concat(const char*, ...);
extern opt_pass *current_pass;
extern gcc::context* g;

#include "extern_makers.cc"
#include "pass_makers.cc"

static struct plugin_info plugin_name = {"0.1", "GCC pass reorder experiment"};

class dummy_pass: public opt_pass {
	public:
		dummy_pass(const pass_data& data, gcc::context* g) : opt_pass(data, g) {}

		opt_pass* clone() override {return new dummy_pass(*this);} 
};

///go through the pass tree and replace every pass with a dummy
static void disable_pass_tree (opt_pass *pass, struct plugin_name_args* plugin_info) {
	while (pass) {
		
		//construct a dummy pass and its info
		struct pass_data dummy_pass_data = {
			GIMPLE_PASS,
			"*plugin_dummy_pass",
			OPTGROUP_NONE,
			TV_PLUGIN_INIT,
			0,
			0,
			0,
			0,
			0,
		};
		opt_pass* dummy_pass_pass = new dummy_pass(dummy_pass_data, g);
		struct register_pass_info dummy_pass_info = {
			dummy_pass_pass,
			pass->name,
			1,
			PASS_POS_REPLACE,
		};

		printf("%s %d\n", pass->name, pass->static_pass_number);
		
		
		//replace current pass with dummy
		register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &dummy_pass_info);

		opt_pass *next = pass->next;
		pass = next;
	}
}

///go through the pass tree and replace every pass with a dummy
static void print_pass_tree (opt_pass *pass) {
	while (pass) {
		
		print_pass_tree(pass->sub);

		printf("%s\n", pass->name);
		opt_pass *next = pass->next;
		pass = next;
	}
}

///prints as much information about a pass as it can, human-readable
static void pass_dump_gate_callback(void* gcc_data, void* user_data) {
	if (current_pass) {
		printf("\n");
		printf("Name: %s\nProperties:\n\tRequired: 0x%x\n\tProvided: 0x%x\n\tDestroyed: 0x%x\n", 
				current_pass->name, 
				current_pass->properties_required, 
				current_pass->properties_provided, 
				current_pass->properties_destroyed);
		printf("Type: %d\n", current_pass->type);
		printf("Gate status: %d\n", *(int*)gcc_data);
		printf("Subpasses: %s", current_pass->sub ? "yes\n" : "no\n");
		printf("TODO before: 0x%X\n", current_pass->todo_flags_start);
		printf("TODO after: 0x%X\n", current_pass->todo_flags_finish);
		printf("Static number %d\n", current_pass->static_pass_number);
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

static void insert_marker_pass(struct plugin_name_args* plugin_info, enum opt_pass_type type, const char* name, enum pass_positioning_ops pos, int count = 1, const char* postfix = "") {
		struct pass_data dummy_pass_data = {
			type,
			concat("*plugin_dummy_pass", postfix, NULL),
			OPTGROUP_NONE,
			TV_PLUGIN_INIT,
			0,
			0,
			0,
			0,
			0,
		};
		opt_pass* dummy_pass_pass = new dummy_pass(dummy_pass_data, g);
		struct register_pass_info dummy_pass_info = {
			dummy_pass_pass,
			name,
			count,
			pos,
		};
	
		register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &dummy_pass_info);
}

static void clear_pass_tree_gate_callback(void* gcc_data, void* used_data) {
	static bool discard_flag = false;
	if (!strncmp("*plugin_dummy_pass", current_pass->name, 18)) {
		discard_flag = !discard_flag;
		return;
	}
	if (discard_flag) {
		*(bool*)gcc_data = 0;
	}
}

static void pass_exec_callback(void* gcc_data, void* used_data) {
	printf("%s\n", current_pass->name);
};

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
	register_callback(plugin_info->base_name, PLUGIN_INFO, NULL, &plugin_name);

	for (int i = 0; i < plugin_info->argc; i++) {
		//choose dump format based on "dump_format" (short/long/executed) argument
		if (!strcmp(plugin_info->argv[i].key, "dump_format")) {
			if (!strcmp(plugin_info->argv[i].value, "short")) {
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, pass_dump_short_gate_callback, NULL);
			} else if (!strcmp(plugin_info->argv[i].value, "long")) {
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, pass_dump_gate_callback, NULL);
			} else if (!strcmp(plugin_info->argv[i].value, "executed")) {
				register_callback(plugin_info->base_name, PLUGIN_PASS_EXECUTION, pass_exec_callback, NULL);
			} else {
				fprintf(stderr, "Incorrect plugin dump_format value\n");
				return -1;
			}
		}

		//choose pass reorder operation
		if (!strcmp(plugin_info->argv[i].key, "pass_remove")) {
			if (!strcmp(plugin_info->argv[i].value, "clear")) {
				insert_marker_pass(plugin_info, GIMPLE_PASS, "*warn_unused_result", PASS_POS_INSERT_BEFORE, 1);
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, clear_pass_tree_gate_callback, NULL);
			} else if (!strcmp(plugin_info->argv[i].value, "by_marker")) {
				for (int j = 0; j < plugin_info->argc; j++) {
					if (!strcmp(plugin_info->argv[j].key, "mark_pass_before")) {
						char* arg = xstrdup(plugin_info->argv[j].value);
						char* name = strtok(arg, ".");
						int type = atoi(strtok(arg, "."));
						insert_marker_pass(plugin_info, static_cast<opt_pass_type>(type), name, PASS_POS_INSERT_BEFORE, 1);
					}
					if (!strcmp(plugin_info->argv[j].key, "mark_pass_after")) {
						char* arg = xstrdup(plugin_info->argv[j].value);
						char* name = strtok(arg, ".");
						int type = atoi(strtok(arg, "."));
						insert_marker_pass(plugin_info, static_cast<opt_pass_type>(type), name, PASS_POS_INSERT_AFTER, 1);
					}
				}
				register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, clear_pass_tree_gate_callback, NULL);
			} else {
				fprintf(stderr, "Incorrect plugin pass_reorder value\n");
				return -1;
			}
		}

		if (!strcmp(plugin_info->argv[i].key, "pass_replace")) {
			FILE* passes_file = fopen(plugin_info->argv[i].value, "r");
			if (passes_file == NULL) {
				fprintf(stderr, "Could not open file with pass order information\n");
				return -1;
			}
			
			//insert required marker passes
			insert_marker_pass(plugin_info, opt_pass_type::GIMPLE_PASS, "inline_param", PASS_POS_INSERT_BEFORE, 1, "_list1");
			insert_marker_pass(plugin_info, opt_pass_type::GIMPLE_PASS, "inline_param", PASS_POS_INSERT_AFTER, 56);
			insert_marker_pass(plugin_info, opt_pass_type::GIMPLE_PASS, "*strip_predict_hints", PASS_POS_INSERT_AFTER, 1, "_list2");
			insert_marker_pass(plugin_info, opt_pass_type::GIMPLE_PASS, "local-pure-const", PASS_POS_INSERT_AFTER, 201);
			insert_marker_pass(plugin_info, opt_pass_type::RTL_PASS, "dfinit", PASS_POS_INSERT_AFTER, 1, "_list3");
			insert_marker_pass(plugin_info, opt_pass_type::RTL_PASS, "init-regs", PASS_POS_INSERT_BEFORE, 1);
			register_callback(plugin_info->base_name, PLUGIN_OVERRIDE_GATE, clear_pass_tree_gate_callback, NULL);

			opt_pass* loop2 = pass_by_name("loop2");
			opt_pass* loop2init = pass_by_name("loop2_init");
            opt_pass* loop2done = pass_by_name("loop2_done");
            opt_pass* doloop = pass_by_name("loop2_invariant");

			loop2->sub = loop2init;
			loop2init->sub = doloop;
			doloop->sub = loop2done;
			
			struct register_pass_info loop2_data = {loop2, "*plugin_dummy_pass_list3", 1, PASS_POS_INSERT_BEFORE};
			register_callback(plugin_info->base_name, PLUGIN_PASS_MANAGER_SETUP, NULL, &loop2_data);
			
			fclose(passes_file);
		}

		//create file with make_* functions for passes
		if (!strcmp(plugin_info->argv[i].key, "generate_makers")) {
			FILE* passes_file = fopen(plugin_info->argv[i].value, "r");
			if (passes_file == NULL) {
				fprintf(stderr, "Plugin could not open file with passes to generate makers for\n");
				return -1;
			}

			FILE* makers_file = fopen("pass_makers.cc", "w");
			if (passes_file == NULL) {
				fprintf(stderr, "Plugin could not create file for makers\n");
				fclose(passes_file);
				return -1;
			}

			fprintf(makers_file, "opt_pass* pass_by_name(const char* name) {\n");
			
			char* line = NULL;
			size_t len = 0;
			while (getline(&line, &len, passes_file) != -1) {
				char* space_ptr = strchr(line, ' ');
				if (*space_ptr == 0) {
					fprintf(stderr, "Passes file is corrupted on line [%s]\n", line);
					free(line);
					fclose(passes_file);
					fclose(makers_file);
					return -1;
				}
				*space_ptr = 0;
				space_ptr += 1;

				int namelen = strlen(space_ptr);

				fprintf(makers_file, "\tif (!strcmp(name, \"%.*s\")) {\n\t\treturn make_%s(g);\n\t}\n", namelen - 1, space_ptr, line);
			}
			fprintf(makers_file, "\treturn NULL;\n}");

			free(line);
			fclose(passes_file);
			fclose(makers_file);
		}
	}
	
	return 0;
}

