opt_pass* pass_by_name(const char* name) {
	if (!strcmp(name, "auto_inc_dec")) {
		return make_pass_inc_dec(g);
	}
	if (!strcmp(name, "*rebuild_cgraph_edges")) {
		return make_pass_rebuild_cgraph_edges(g);
	}
	if (!strcmp(name, "*remove_cgraph_callee_edges")) {
		return make_pass_remove_cgraph_callee_edges(g);
	}
	if (!strcmp(name, "cprop")) {
		return make_pass_rtl_cprop(g);
	}
	if (!strcmp(name, "cse1")) {
		return make_pass_cse(g);
	}
	if (!strcmp(name, "cse2")) {
		return make_pass_cse2(g);
	}
	if (!strcmp(name, "dse1")) {
		return make_pass_rtl_dse1(g);
	}
	if (!strcmp(name, "fwprop1")) {
		return make_pass_rtl_fwprop(g);
	}
	if (!strcmp(name, "fwprop2")) {
		return make_pass_rtl_fwprop_addr(g);
	}
	if (!strcmp(name, "rtl pre")) {
		return make_pass_rtl_pre(g);
	}
	if (!strcmp(name, "laddress")) {
		return make_pass_laddress(g);
	}
	if (!strcmp(name, "backprop")) {
		return make_pass_backprop(g);
	}
	if (!strcmp(name, "isolate-paths")) {
		return make_pass_isolate_erroneous_paths(g);
	}
	if (!strcmp(name, "printf-return-value")) {
		return make_pass_sprintf_length(g);
	}
	if (!strcmp(name, "store-merging")) {
		return make_pass_store_merging(g);
	}
	if (!strcmp(name, "slsr")) {
		return make_pass_strength_reduction(g);
	}
	if (!strcmp(name, "ce1")) {
		return make_pass_rtl_ifcvt(g);
	}
	if (!strcmp(name, "inline_param")) {
		return make_pass_inline_parameters(g);
	}
	if (!strcmp(name, "einline")) {
		return make_pass_early_inline(g);
	}
	if (!strcmp(name, "profile_estimate")) {
		return make_pass_profile(g);
	}
	if (!strcmp(name, "local-pure-const")) {
		return make_pass_local_pure_const(g);
	}
	if (!strcmp(name, "fnsplit")) {
		return make_pass_split_functions(g);
	}
	if (!strcmp(name, "loop2")) {
		return make_pass_loop2(g);
	}
	if (!strcmp(name, "loop2_init")) {
		return make_pass_rtl_loop_init(g);
	}
	if (!strcmp(name, "loop2_done")) {
		return make_pass_rtl_loop_done(g);
	}
	if (!strcmp(name, "loop2_invariant")) {
		return make_pass_rtl_move_loop_invariants(g);
	}
	if (!strcmp(name, "loop2_unroll")) {
		return make_pass_rtl_unroll_loops(g);
	}
	if (!strcmp(name, "loop2_doloop")) {
		return make_pass_rtl_doloop(g);
	}
	if (!strcmp(name, "profile_estimate")) {
		return make_pass_profile(g);
	}
	if (!strcmp(name, "reginfo")) {
		return make_pass_reginfo_init(g);
	}
	if (!strcmp(name, "cdce")) {
		return make_pass_call_cdce(g);
	}
	if (!strcmp(name, "crited")) {
		return make_pass_split_crit_edges(g);
	}
	if (!strcmp(name, "mergephi")) {
		return make_pass_merge_phi(g);
	}
	if (!strcmp(name, "cplxlower")) {
		return make_pass_lower_complex(g);
	}
	if (!strcmp(name, "retslot")) {
		return make_pass_return_slot(g);
	}
	if (!strcmp(name, "objsz")) {
		return make_pass_object_sizes(g);
	}
	if (!strcmp(name, "esra")) {
		return make_pass_sra_early(g);
	}
	if (!strcmp(name, "sra")) {
		return make_pass_sra(g);
	}
	if (!strcmp(name, "eipa_sra")) {
		return make_pass_early_ipa_sra(g);
	}
	if (!strcmp(name, "ccp")) {
		return make_pass_ccp(g);
	}
	if (!strcmp(name, "fab")) {
		return make_pass_fold_builtins(g);
	}
	if (!strcmp(name, "copyprop")) {
		return make_pass_copy_prop(g);
	}
	if (!strcmp(name, "dce")) {
		return make_pass_dce(g);
	}
	if (!strcmp(name, "cddce")) {
		return make_pass_cd_dce(g);
	}
	if (!strcmp(name, "dom")) {
		return make_pass_dominator(g);
	}
	if (!strcmp(name, "dse")) {
		return make_pass_dse(g);
	}
	if (!strcmp(name, "forwprop")) {
		return make_pass_forwprop(g);
	}
	if (!strcmp(name, "ifcombine")) {
		return make_pass_tree_ifcombine(g);
	}
	if (!strcmp(name, "fix_loops")) {
		return make_pass_fix_loops(g);
	}
	if (!strcmp(name, "loop")) {
		return make_pass_tree_loop(g);
	}
	if (!strcmp(name, "loopinit")) {
		return make_pass_tree_loop_init(g);
	}
	if (!strcmp(name, "sccp")) {
		return make_pass_scev_cprop(g);
	}
	if (!strcmp(name, "*record_bounds")) {
		return make_pass_record_bounds(g);
	}
	if (!strcmp(name, "ivopts")) {
		return make_pass_iv_optimize(g);
	}
	if (!strcmp(name, "loopdone")) {
		return make_pass_tree_loop_done(g);
	}
	if (!strcmp(name, "ch")) {
		return make_pass_ch(g);
	}
	if (!strcmp(name, "lim")) {
		return make_pass_lim(g);
	}
	if (!strcmp(name, "ivcanon")) {
		return make_pass_iv_canon(g);
	}
	if (!strcmp(name, "cunroll")) {
		return make_pass_complete_unroll(g);
	}
	if (!strcmp(name, "cunrolli")) {
		return make_pass_complete_unrolli(g);
	}
	if (!strcmp(name, "sincos")) {
		return make_pass_cse_sincos(g);
	}
	if (!strcmp(name, "bswap")) {
		return make_pass_optimize_bswap(g);
	}
	if (!strcmp(name, "widening_mul")) {
		return make_pass_optimize_widening_mul(g);
	}
	if (!strcmp(name, "release_ssa")) {
		return make_pass_release_ssa_names(g);
	}
	if (!strcmp(name, "phicprop")) {
		return make_pass_phi_only_cprop(g);
	}
	if (!strcmp(name, "phiopt")) {
		return make_pass_phiopt(g);
	}
	if (!strcmp(name, "cselim")) {
		return make_pass_cselim(g);
	}
	if (!strcmp(name, "phiprop")) {
		return make_pass_phiprop(g);
	}
	if (!strcmp(name, "pre")) {
		return make_pass_pre(g);
	}
	if (!strcmp(name, "fre")) {
		return make_pass_fre(g);
	}
	if (!strcmp(name, "reassoc")) {
		return make_pass_reassoc(g);
	}
	if (!strcmp(name, "sink")) {
		return make_pass_sink_code(g);
	}
	if (!strcmp(name, "strlen")) {
		return make_pass_strlen(g);
	}
	if (!strcmp(name, "alias")) {
		return make_pass_build_alias(g);
	}
	if (!strcmp(name, "ealias")) {
		return make_pass_build_ealias(g);
	}
	if (!strcmp(name, "thread")) {
		return make_pass_thread_jumps(g);
	}
	if (!strcmp(name, "ethread")) {
		return make_pass_early_thread_jumps(g);
	}
	if (!strcmp(name, "uncprop")) {
		return make_pass_uncprop(g);
	}
	if (!strcmp(name, "stdarg")) {
		return make_pass_stdarg(g);
	}
	if (!strcmp(name, "switchconv")) {
		return make_pass_convert_switch(g);
	}
	if (!strcmp(name, "tailr")) {
		return make_pass_tail_recursion(g);
	}
	if (!strcmp(name, "tailc")) {
		return make_pass_tail_calls(g);
	}
	if (!strcmp(name, "veclower2")) {
		return make_pass_lower_vector_ssa(g);
	}
	if (!strcmp(name, "vrp")) {
		return make_pass_vrp(g);
	}
	if (!strcmp(name, "evrp")) {
		return make_pass_early_vrp(g);
	}
	return NULL;
}