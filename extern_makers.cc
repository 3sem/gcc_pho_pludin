extern simple_ipa_opt_pass *make_pass_ipa_chkp_versioning (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_chkp_early_produce_thunks (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_chkp_produce_thunks (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_chkp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_chkp_opt (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_asan (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_asan_O0 (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tsan (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tsan_O0 (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sancov (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sancov_O0 (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_cf (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_refactor_eh (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_eh (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_eh_dispatch (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_resx (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_build_cfg (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_tree_profile (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_cleanup_eh (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sra (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sra_early (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_ipa_sra (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tail_recursion (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tail_calls (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_fix_loops (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_loop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_no_loop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_loop_init (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lim (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_unswitch (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_loop_split (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_predcom (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_iv_canon (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_scev_cprop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_empty_loop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_record_bounds (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_graphite (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_graphite_transforms (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_if_conversion (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_loop_distribution (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_vectorize (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_simduid_cleanup (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_slp_vectorize (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_complete_unroll (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_complete_unrolli (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_parallelize_loops (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_loop_prefetch (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_iv_optimize (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_loop_done (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_ch (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_ch_vect (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_ccp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_split_paths (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_phi_only_cprop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_build_ssa (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_build_alias (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_build_ealias (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_dominator (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_dce (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_cd_dce (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_call_cdce (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_merge_phi (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_thread_jumps (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_thread_jumps (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_split_crit_edges (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_laddress (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_pre (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_profile (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_strip_predict_hints (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_complex_O0 (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_complex (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_vector (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_vector_ssa (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_omp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_diagnose_omp_blocks (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_expand_omp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_expand_omp_ssa (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_omp_target_link (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_oacc_device_lower (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_omp_device_lower (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_object_sizes (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_strlen (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_fold_builtins (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_post_ipa_warn (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_stdarg (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_warn_uninitialized (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_late_warn_uninitialized (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_cse_reciprocals (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_cse_sincos (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_optimize_bswap (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_store_merging (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_optimize_widening_mul (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_warn_function_return (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_warn_function_noreturn (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_cselim (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_phiopt (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_forwprop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_phiprop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tree_ifcombine (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_dse (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_nrv (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rename_ssa_copies (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sink_code (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_fre (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_check_data_deps (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_copy_prop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_isolate_erroneous_paths (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_vrp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_vrp (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_uncprop (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_return_slot (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_reassoc (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_rebuild_cgraph_edges (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_remove_cgraph_callee_edges (gcc::context
							      *ctxt);
extern gimple_opt_pass *make_pass_build_cgraph_edges (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_local_pure_const (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_nothrow (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tracer (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_warn_unused_result (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_diagnose_tm_blocks (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_tm (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tm_init (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tm_mark (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tm_memopt (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_tm_edges (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_split_functions (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_feedback_split_functions (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_strength_reduction (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_vtable_verify (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_ubsan (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sanopt (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_oacc_kernels (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_oacc (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_oacc_kernels (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_gen_hsail (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_warn_nonnull_compare (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_sprintf_length (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_walloca (gcc::context *ctxt);

/* IPA Passes */
extern simple_ipa_opt_pass *make_pass_ipa_lower_emutls (gcc::context *ctxt);
extern simple_ipa_opt_pass
							      *make_pass_ipa_function_and_variable_visibility (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_tree_profile (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_auto_profile (gcc::context *ctxt);

extern simple_ipa_opt_pass *make_pass_build_ssa_passes (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_chkp_instrumentation_passes (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_local_optimization_passes (gcc::context *ctxt);

extern ipa_opt_pass_d *make_pass_ipa_whole_program_visibility (gcc::context
							       *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_increase_alignment (gcc::context
							      *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_inline (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_free_lang_data (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_free_inline_summary (gcc::context
							       *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_cp (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_icf (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_devirt (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_reference (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_hsa (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_pure_const (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_pta (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_ipa_tm (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_target_clone (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_dispatcher_calls (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_omp_simd_clone (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_profile (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_cdtor_merge (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_single_use (gcc::context *ctxt);
extern ipa_opt_pass_d *make_pass_ipa_comdats (gcc::context *ctxt);
extern simple_ipa_opt_pass *make_pass_materialize_all_clones (gcc::context *
							      ctxt);

extern gimple_opt_pass *make_pass_cleanup_cfg_post_optimizing (gcc::context
							       *ctxt);
extern gimple_opt_pass *make_pass_fixup_cfg (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_backprop (gcc::context *ctxt);

extern rtl_opt_pass *make_pass_expand (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_instantiate_virtual_regs (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_fwprop (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_fwprop_addr (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_jump (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_jump2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_lower_subreg (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_cse (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_fast_rtl_dce (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_ud_rtl_dce (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_dce (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_dse1 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_dse2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_dse3 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_cprop (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_pre (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_hoist (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_store_motion (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_cse_after_global_opts (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_ifcvt (gcc::context *ctxt);

extern rtl_opt_pass *make_pass_into_cfg_layout_mode (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_outof_cfg_layout_mode (gcc::context *ctxt);

extern rtl_opt_pass *make_pass_loop2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_loop_init (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_move_loop_invariants (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_unroll_loops (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_doloop (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_loop_done (gcc::context *ctxt);

extern rtl_opt_pass *make_pass_web (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_cse2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_df_initialize_opt (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_df_initialize_no_opt (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_reginfo_init (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_inc_dec (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_stack_ptr_mod (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_initialize_regs (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_combine (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_if_after_combine (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_ree (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_partition_blocks (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_match_asm_constraints (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_split_all_insns (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_fast_rtl_byte_dce (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_lower_subreg2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_mode_switching (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_sms (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_sched (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_live_range_shrinkage (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_ira (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_reload (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_clean_state (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_branch_prob (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_value_profile_transformations (gcc::context
							      *ctxt);
extern rtl_opt_pass *make_pass_postreload_cse (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_gcse2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_split_after_reload (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_branch_target_load_optimize1 (gcc::context
							     *ctxt);
extern rtl_opt_pass *make_pass_thread_prologue_and_epilogue (gcc::context
							     *ctxt);
extern rtl_opt_pass *make_pass_stack_adjustments (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_sched_fusion (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_peephole2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_if_after_reload (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_regrename (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_cprop_hardreg (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_reorder_blocks (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_branch_target_load_optimize2 (gcc::context
							     *ctxt);
extern rtl_opt_pass *make_pass_leaf_regs (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_split_before_sched2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_compare_elim_after_reload (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_sched2 (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_stack_regs (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_stack_regs_run (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_df_finish (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_compute_alignments (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_duplicate_computed_gotos (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_variable_tracking (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_free_cfg (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_machine_reorg (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_cleanup_barriers (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_delay_slots (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_split_for_shorten_branches (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_split_before_regstack (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_convert_to_eh_region_ranges (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_shorten_branches (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_set_nothrow_function_flags (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_dwarf2_frame (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_final (gcc::context *ctxt);
extern rtl_opt_pass *make_pass_rtl_seqabstr (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_release_ssa_names (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_early_inline (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_inline_parameters (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_update_address_taken (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_convert_switch (gcc::context *ctxt);
extern gimple_opt_pass *make_pass_lower_vaarg (gcc::context *ctxt);
