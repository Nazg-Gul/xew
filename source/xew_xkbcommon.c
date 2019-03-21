// Copyright (c) 2018, X Wrangler authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// Author: Sergey Sharybin (sergey.vfx@gmail.com)

#define XEW_IMPL

#include "xew_xkbcommon.h"

#include "xew_util.h"

// Per-platform list of libraries to be queried/
#ifdef _WIN32
static const char* xkbcommon_paths[] = {NULL};
#elif defined(__APPLE__)
static const char* xkbcommon_paths[] = {NULL};
#else
static const char* xkbcommon_paths[] = {"libxkbcommon.so.1",
                                        "libxkbcommon.so",
                                        NULL};
#endif

static DynamicLibrary* xkbcommon_lib = NULL;

////////////////////////////////////////////////////////////////////////////////
// Functor definitions.

// xkbcommon.h
txkb_keysym_get_name xkb_keysym_get_name_impl;
txkb_keysym_from_name xkb_keysym_from_name_impl;
txkb_keysym_to_utf8 xkb_keysym_to_utf8_impl;
txkb_keysym_to_utf32 xkb_keysym_to_utf32_impl;
txkb_keysym_to_upper xkb_keysym_to_upper_impl;
txkb_keysym_to_lower xkb_keysym_to_lower_impl;
txkb_context_new xkb_context_new_impl;
txkb_context_ref xkb_context_ref_impl;
txkb_context_unref xkb_context_unref_impl;
txkb_context_set_user_data xkb_context_set_user_data_impl;
txkb_context_get_user_data xkb_context_get_user_data_impl;
txkb_context_include_path_append xkb_context_include_path_append_impl;
txkb_context_include_path_append_default xkb_context_include_path_append_default_impl;
txkb_context_include_path_reset_defaults xkb_context_include_path_reset_defaults_impl;
txkb_context_include_path_clear xkb_context_include_path_clear_impl;
txkb_context_num_include_paths xkb_context_num_include_paths_impl;
txkb_context_include_path_get xkb_context_include_path_get_impl;
txkb_context_set_log_level xkb_context_set_log_level_impl;
txkb_context_get_log_level xkb_context_get_log_level_impl;
txkb_context_set_log_verbosity xkb_context_set_log_verbosity_impl;
txkb_context_get_log_verbosity xkb_context_get_log_verbosity_impl;
txkb_context_set_log_fn xkb_context_set_log_fn_impl;
txkb_keymap_new_from_names xkb_keymap_new_from_names_impl;
txkb_keymap_new_from_file xkb_keymap_new_from_file_impl;
txkb_keymap_new_from_string xkb_keymap_new_from_string_impl;
txkb_keymap_new_from_buffer xkb_keymap_new_from_buffer_impl;
txkb_keymap_ref xkb_keymap_ref_impl;
txkb_keymap_unref xkb_keymap_unref_impl;
txkb_keymap_get_as_string xkb_keymap_get_as_string_impl;
txkb_keymap_min_keycode xkb_keymap_min_keycode_impl;
txkb_keymap_max_keycode xkb_keymap_max_keycode_impl;
txkb_keymap_key_for_each xkb_keymap_key_for_each_impl;
txkb_keymap_key_get_name xkb_keymap_key_get_name_impl;
txkb_keymap_key_by_name xkb_keymap_key_by_name_impl;
txkb_keymap_num_mods xkb_keymap_num_mods_impl;
txkb_keymap_mod_get_name xkb_keymap_mod_get_name_impl;
txkb_keymap_mod_get_index xkb_keymap_mod_get_index_impl;
txkb_keymap_num_layouts xkb_keymap_num_layouts_impl;
txkb_keymap_layout_get_name xkb_keymap_layout_get_name_impl;
txkb_keymap_layout_get_index xkb_keymap_layout_get_index_impl;
txkb_keymap_num_leds xkb_keymap_num_leds_impl;
txkb_keymap_led_get_name xkb_keymap_led_get_name_impl;
txkb_keymap_led_get_index xkb_keymap_led_get_index_impl;
txkb_keymap_num_layouts_for_key xkb_keymap_num_layouts_for_key_impl;
txkb_keymap_num_levels_for_key xkb_keymap_num_levels_for_key_impl;
txkb_keymap_key_get_syms_by_level xkb_keymap_key_get_syms_by_level_impl;
txkb_keymap_key_repeats xkb_keymap_key_repeats_impl;
txkb_state_new xkb_state_new_impl;
txkb_state_ref xkb_state_ref_impl;
txkb_state_unref xkb_state_unref_impl;
txkb_state_get_keymap xkb_state_get_keymap_impl;
txkb_state_update_key xkb_state_update_key_impl;
txkb_state_update_mask xkb_state_update_mask_impl;
txkb_state_key_get_syms xkb_state_key_get_syms_impl;
txkb_state_key_get_utf8 xkb_state_key_get_utf8_impl;
txkb_state_key_get_utf32 xkb_state_key_get_utf32_impl;
txkb_state_key_get_one_sym xkb_state_key_get_one_sym_impl;
txkb_state_key_get_layout xkb_state_key_get_layout_impl;
txkb_state_key_get_level xkb_state_key_get_level_impl;
txkb_state_serialize_mods xkb_state_serialize_mods_impl;
txkb_state_serialize_layout xkb_state_serialize_layout_impl;
txkb_state_mod_name_is_active xkb_state_mod_name_is_active_impl;
txkb_state_mod_names_are_active xkb_state_mod_names_are_active_impl;
txkb_state_mod_index_is_active xkb_state_mod_index_is_active_impl;
txkb_state_mod_indices_are_active xkb_state_mod_indices_are_active_impl;
txkb_state_key_get_consumed_mods2 xkb_state_key_get_consumed_mods2_impl;
txkb_state_key_get_consumed_mods xkb_state_key_get_consumed_mods_impl;
txkb_state_mod_index_is_consumed2 xkb_state_mod_index_is_consumed2_impl;
txkb_state_mod_index_is_consumed xkb_state_mod_index_is_consumed_impl;
txkb_state_mod_mask_remove_consumed xkb_state_mod_mask_remove_consumed_impl;
txkb_state_layout_name_is_active xkb_state_layout_name_is_active_impl;
txkb_state_layout_index_is_active xkb_state_layout_index_is_active_impl;
txkb_state_led_name_is_active xkb_state_led_name_is_active_impl;
txkb_state_led_index_is_active xkb_state_led_index_is_active_impl;

// xkbcommon-compose.h
txkb_compose_table_new_from_locale xkb_compose_table_new_from_locale_impl;
txkb_compose_table_new_from_file xkb_compose_table_new_from_file_impl;
txkb_compose_table_new_from_buffer xkb_compose_table_new_from_buffer_impl;
txkb_compose_table_ref xkb_compose_table_ref_impl;
txkb_compose_table_unref xkb_compose_table_unref_impl;
txkb_compose_state_new xkb_compose_state_new_impl;
txkb_compose_state_ref xkb_compose_state_ref_impl;
txkb_compose_state_unref xkb_compose_state_unref_impl;
txkb_compose_state_get_compose_table xkb_compose_state_get_compose_table_impl;
txkb_compose_state_feed xkb_compose_state_feed_impl;
txkb_compose_state_reset xkb_compose_state_reset_impl;
txkb_compose_state_get_status xkb_compose_state_get_status_impl;
txkb_compose_state_get_utf8 xkb_compose_state_get_utf8_impl;
txkb_compose_state_get_one_sym xkb_compose_state_get_one_sym_impl;


////////////////////////////////////////////////////////////////////////////////
// Function wrappers.

// xkbcommon.h
int xkb_keysym_get_name(xkb_keysym_t keysym, char* buffer, size_t size) {
  return xkb_keysym_get_name_impl(keysym, buffer, size);
}

xkb_keysym_t xkb_keysym_from_name(const char* name, enum xkb_keysym_flags flags) {
  return xkb_keysym_from_name_impl(name, flags);
}

int xkb_keysym_to_utf8(xkb_keysym_t keysym, char* buffer, size_t size) {
  return xkb_keysym_to_utf8_impl(keysym, buffer, size);
}

uint32_t xkb_keysym_to_utf32(xkb_keysym_t keysym) {
  return xkb_keysym_to_utf32_impl(keysym);
}

xkb_keysym_t xkb_keysym_to_upper(xkb_keysym_t ks) {
  return xkb_keysym_to_upper_impl(ks);
}

xkb_keysym_t xkb_keysym_to_lower(xkb_keysym_t ks) {
  return xkb_keysym_to_lower_impl(ks);
}

struct xkb_context* xkb_context_new(enum xkb_context_flags flags) {
  return xkb_context_new_impl(flags);
}

struct xkb_context* xkb_context_ref(struct xkb_context* context) {
  return xkb_context_ref_impl(context);
}

void xkb_context_unref(struct xkb_context* context) {
  return xkb_context_unref_impl(context);
}

void xkb_context_set_user_data(struct xkb_context* context, void* user_data) {
  return xkb_context_set_user_data_impl(context, user_data);
}

void* xkb_context_get_user_data(struct xkb_context* context) {
  return xkb_context_get_user_data_impl(context);
}

int xkb_context_include_path_append(struct xkb_context* context, const char* path) {
  return xkb_context_include_path_append_impl(context, path);
}

int xkb_context_include_path_append_default(struct xkb_context* context) {
  return xkb_context_include_path_append_default_impl(context);
}

int xkb_context_include_path_reset_defaults(struct xkb_context* context) {
  return xkb_context_include_path_reset_defaults_impl(context);
}

void xkb_context_include_path_clear(struct xkb_context* context) {
  return xkb_context_include_path_clear_impl(context);
}

unsigned int xkb_context_num_include_paths(struct xkb_context* context) {
  return xkb_context_num_include_paths_impl(context);
}

const char* xkb_context_include_path_get(struct xkb_context* context, unsigned int index) {
  return xkb_context_include_path_get_impl(context, index);
}

void xkb_context_set_log_level(struct xkb_context* context, enum xkb_log_level level) {
  return xkb_context_set_log_level_impl(context, level);
}

enum xkb_log_level xkb_context_get_log_level(struct xkb_context* context) {
  return xkb_context_get_log_level_impl(context);
}

void xkb_context_set_log_verbosity(struct xkb_context* context, int verbosity) {
  return xkb_context_set_log_verbosity_impl(context, verbosity);
}

int xkb_context_get_log_verbosity(struct xkb_context* context) {
  return xkb_context_get_log_verbosity_impl(context);
}

void xkb_context_set_log_fn(
    struct xkb_context* context,
    void (*log_fn)(struct xkb_context* context, enum xkb_log_level level, const char* format, va_list args)) {
  return xkb_context_set_log_fn_impl(context, log_fn);
}

struct xkb_keymap* xkb_keymap_new_from_names(
    struct xkb_context* context,
    const struct xkb_rule_names* names,
    enum xkb_keymap_compile_flags flags) {
  return xkb_keymap_new_from_names_impl(context, names, flags);
}

struct xkb_keymap* xkb_keymap_new_from_file(
    struct xkb_context* context,
    FILE* file,
    enum xkb_keymap_format format,
    enum xkb_keymap_compile_flags flags) {
  return xkb_keymap_new_from_file_impl(context, file, format, flags);
}

struct xkb_keymap* xkb_keymap_new_from_string(
    struct xkb_context* context,
    const char* string,
    enum xkb_keymap_format format,
    enum xkb_keymap_compile_flags flags) {
  return xkb_keymap_new_from_string_impl(context, string, format, flags);
}

struct xkb_keymap* xkb_keymap_new_from_buffer(
    struct xkb_context* context,
    const char* buffer,
    size_t length,
    enum xkb_keymap_format format,
    enum xkb_keymap_compile_flags flags) {
  return xkb_keymap_new_from_buffer_impl(context, buffer, length, format, flags);
}

struct xkb_keymap* xkb_keymap_ref(struct xkb_keymap* keymap) {
  return xkb_keymap_ref_impl(keymap);
}

void xkb_keymap_unref(struct xkb_keymap* keymap) {
  return xkb_keymap_unref_impl(keymap);
}

char* xkb_keymap_get_as_string(struct xkb_keymap* keymap, enum xkb_keymap_format format) {
  return xkb_keymap_get_as_string_impl(keymap, format);
}

xkb_keycode_t xkb_keymap_min_keycode(struct xkb_keymap* keymap) {
  return xkb_keymap_min_keycode_impl(keymap);
}

xkb_keycode_t xkb_keymap_max_keycode(struct xkb_keymap* keymap) {
  return xkb_keymap_max_keycode_impl(keymap);
}

void xkb_keymap_key_for_each(struct xkb_keymap* keymap, xkb_keymap_key_iter_t iter, void* data) {
  return xkb_keymap_key_for_each_impl(keymap, iter, data);
}

const char* xkb_keymap_key_get_name(struct xkb_keymap* keymap, xkb_keycode_t key) {
  return xkb_keymap_key_get_name_impl(keymap, key);
}

xkb_keycode_t xkb_keymap_key_by_name(struct xkb_keymap* keymap, const char* name) {
  return xkb_keymap_key_by_name_impl(keymap, name);
}

xkb_mod_index_t xkb_keymap_num_mods(struct xkb_keymap* keymap) {
  return xkb_keymap_num_mods_impl(keymap);
}

const char* xkb_keymap_mod_get_name(struct xkb_keymap* keymap, xkb_mod_index_t idx) {
  return xkb_keymap_mod_get_name_impl(keymap, idx);
}

xkb_mod_index_t xkb_keymap_mod_get_index(struct xkb_keymap* keymap, const char* name) {
  return xkb_keymap_mod_get_index_impl(keymap, name);
}

xkb_layout_index_t xkb_keymap_num_layouts(struct xkb_keymap* keymap) {
  return xkb_keymap_num_layouts_impl(keymap);
}

const char* xkb_keymap_layout_get_name(struct xkb_keymap* keymap, xkb_layout_index_t idx) {
  return xkb_keymap_layout_get_name_impl(keymap, idx);
}

xkb_layout_index_t xkb_keymap_layout_get_index(struct xkb_keymap* keymap, const char* name) {
  return xkb_keymap_layout_get_index_impl(keymap, name);
}

xkb_led_index_t xkb_keymap_num_leds(struct xkb_keymap* keymap) {
  return xkb_keymap_num_leds_impl(keymap);
}

const char* xkb_keymap_led_get_name(struct xkb_keymap* keymap, xkb_led_index_t idx) {
  return xkb_keymap_led_get_name_impl(keymap, idx);
}

xkb_led_index_t xkb_keymap_led_get_index(struct xkb_keymap* keymap, const char* name) {
  return xkb_keymap_led_get_index_impl(keymap, name);
}

xkb_layout_index_t xkb_keymap_num_layouts_for_key(struct xkb_keymap* keymap, xkb_keycode_t key) {
  return xkb_keymap_num_layouts_for_key_impl(keymap, key);
}

xkb_level_index_t xkb_keymap_num_levels_for_key(struct xkb_keymap* keymap, xkb_keycode_t key, xkb_layout_index_t layout) {
  return xkb_keymap_num_levels_for_key_impl(keymap, key, layout);
}

int xkb_keymap_key_get_syms_by_level(
    struct xkb_keymap* keymap,
    xkb_keycode_t key,
    xkb_layout_index_t layout,
    xkb_level_index_t level,
    const xkb_keysym_t** syms_out) {
  return xkb_keymap_key_get_syms_by_level_impl(keymap, key, layout, level, syms_out);
}

int xkb_keymap_key_repeats(struct xkb_keymap* keymap, xkb_keycode_t key) {
  return xkb_keymap_key_repeats_impl(keymap, key);
}

struct xkb_state* xkb_state_new(struct xkb_keymap* keymap) {
  return xkb_state_new_impl(keymap);
}

struct xkb_state* xkb_state_ref(struct xkb_state* state) {
  return xkb_state_ref_impl(state);
}

void xkb_state_unref(struct xkb_state* state) {
  return xkb_state_unref_impl(state);
}

struct xkb_keymap* xkb_state_get_keymap(struct xkb_state* state) {
  return xkb_state_get_keymap_impl(state);
}

enum xkb_state_component xkb_state_update_key(struct xkb_state* state, xkb_keycode_t key, enum xkb_key_direction direction) {
  return xkb_state_update_key_impl(state, key, direction);
}

enum xkb_state_component xkb_state_update_mask(
    struct xkb_state* state,
    xkb_mod_mask_t depressed_mods,
    xkb_mod_mask_t latched_mods,
    xkb_mod_mask_t locked_mods,
    xkb_layout_index_t depressed_layout,
    xkb_layout_index_t latched_layout,
    xkb_layout_index_t locked_layout) {
  return xkb_state_update_mask_impl(state, depressed_mods, latched_mods, locked_mods, depressed_layout, latched_layout, locked_layout);
}

int xkb_state_key_get_syms(struct xkb_state* state, xkb_keycode_t key, const xkb_keysym_t** syms_out) {
  return xkb_state_key_get_syms_impl(state, key, syms_out);
}

int xkb_state_key_get_utf8(struct xkb_state* state, xkb_keycode_t key, char* buffer, size_t size) {
  return xkb_state_key_get_utf8_impl(state, key, buffer, size);
}

uint32_t xkb_state_key_get_utf32(struct xkb_state* state, xkb_keycode_t key) {
  return xkb_state_key_get_utf32_impl(state, key);
}

xkb_keysym_t xkb_state_key_get_one_sym(struct xkb_state* state, xkb_keycode_t key) {
  return xkb_state_key_get_one_sym_impl(state, key);
}

xkb_layout_index_t xkb_state_key_get_layout(struct xkb_state* state, xkb_keycode_t key) {
  return xkb_state_key_get_layout_impl(state, key);
}

xkb_level_index_t xkb_state_key_get_level(struct xkb_state* state, xkb_keycode_t key, xkb_layout_index_t layout) {
  return xkb_state_key_get_level_impl(state, key, layout);
}

xkb_mod_mask_t xkb_state_serialize_mods(struct xkb_state* state, enum xkb_state_component components) {
  return xkb_state_serialize_mods_impl(state, components);
}

xkb_layout_index_t xkb_state_serialize_layout(struct xkb_state* state, enum xkb_state_component components) {
  return xkb_state_serialize_layout_impl(state, components);
}

int xkb_state_mod_name_is_active(struct xkb_state* state, const char* name, enum xkb_state_component type) {
  return xkb_state_mod_name_is_active_impl(state, name, type);
}

int xkb_state_mod_index_is_active(struct xkb_state* state, xkb_mod_index_t idx, enum xkb_state_component type) {
  return xkb_state_mod_index_is_active_impl(state, idx, type);
}

xkb_mod_mask_t xkb_state_key_get_consumed_mods2(struct xkb_state* state, xkb_keycode_t key, enum xkb_consumed_mode mode) {
  return xkb_state_key_get_consumed_mods2_impl(state, key, mode);
}

xkb_mod_mask_t xkb_state_key_get_consumed_mods(struct xkb_state* state, xkb_keycode_t key) {
  return xkb_state_key_get_consumed_mods_impl(state, key);
}

int xkb_state_mod_index_is_consumed2(
    struct xkb_state* state,
    xkb_keycode_t key,
    xkb_mod_index_t idx,
    enum xkb_consumed_mode mode) {
  return xkb_state_mod_index_is_consumed2_impl(state, key, idx, mode);
}

int xkb_state_mod_index_is_consumed(struct xkb_state* state, xkb_keycode_t key, xkb_mod_index_t idx) {
  return xkb_state_mod_index_is_consumed_impl(state, key, idx);
}

xkb_mod_mask_t xkb_state_mod_mask_remove_consumed(struct xkb_state* state, xkb_keycode_t key, xkb_mod_mask_t mask) {
  return xkb_state_mod_mask_remove_consumed_impl(state, key, mask);
}

int xkb_state_layout_name_is_active(struct xkb_state* state, const char* name, enum xkb_state_component type) {
  return xkb_state_layout_name_is_active_impl(state, name, type);
}

int xkb_state_layout_index_is_active(struct xkb_state* state, xkb_layout_index_t idx, enum xkb_state_component type) {
  return xkb_state_layout_index_is_active_impl(state, idx, type);
}

int xkb_state_led_name_is_active(struct xkb_state* state, const char* name) {
  return xkb_state_led_name_is_active_impl(state, name);
}

int xkb_state_led_index_is_active(struct xkb_state* state, xkb_led_index_t idx) {
  return xkb_state_led_index_is_active_impl(state, idx);
}

// xkbcommon-compose.h
struct xkb_compose_table* xkb_compose_table_new_from_locale(
    struct xkb_context* context,
    const char* locale,
    enum xkb_compose_compile_flags flags) {
  return xkb_compose_table_new_from_locale_impl(context, locale, flags);
}

struct xkb_compose_table* xkb_compose_table_new_from_file(
    struct xkb_context* context,
    FILE* file,
    const char* locale,
    enum xkb_compose_format format,
    enum xkb_compose_compile_flags flags) {
  return xkb_compose_table_new_from_file_impl(context, file, locale, format, flags);
}

struct xkb_compose_table* xkb_compose_table_new_from_buffer(
    struct xkb_context* context,
    const char* buffer,
    size_t length,
    const char* locale,
    enum xkb_compose_format format,
    enum xkb_compose_compile_flags flags) {
  return xkb_compose_table_new_from_buffer_impl(context, buffer, length, locale, format, flags);
}

struct xkb_compose_table* xkb_compose_table_ref(struct xkb_compose_table* table) {
  return xkb_compose_table_ref_impl(table);
}

void xkb_compose_table_unref(struct xkb_compose_table* table) {
  return xkb_compose_table_unref_impl(table);
}

struct xkb_compose_state* xkb_compose_state_new(struct xkb_compose_table* table, enum xkb_compose_state_flags flags) {
  return xkb_compose_state_new_impl(table, flags);
}

struct xkb_compose_state* xkb_compose_state_ref(struct xkb_compose_state* state) {
  return xkb_compose_state_ref_impl(state);
}

void xkb_compose_state_unref(struct xkb_compose_state* state) {
  return xkb_compose_state_unref_impl(state);
}

struct xkb_compose_table* xkb_compose_state_get_compose_table(struct xkb_compose_state* state) {
  return xkb_compose_state_get_compose_table_impl(state);
}

enum xkb_compose_feed_result xkb_compose_state_feed(struct xkb_compose_state* state, xkb_keysym_t keysym) {
  return xkb_compose_state_feed_impl(state, keysym);
}

void xkb_compose_state_reset(struct xkb_compose_state* state) {
  return xkb_compose_state_reset_impl(state);
}

enum xkb_compose_status xkb_compose_state_get_status(struct xkb_compose_state* state) {
  return xkb_compose_state_get_status_impl(state);
}

int xkb_compose_state_get_utf8(struct xkb_compose_state* state, char* buffer, size_t size) {
  return xkb_compose_state_get_utf8_impl(state, buffer, size);
}

xkb_keysym_t xkb_compose_state_get_one_sym(struct xkb_compose_state* state) {
  return xkb_compose_state_get_one_sym_impl(state);
}



////////////////////////////////////////////////////////////////////////////////
// Main wrangling logic.

static void atExitHandler(void) {
  if (xkbcommon_lib != NULL) {
    // Currently we ignore errors.
    // TODO(sergey): It might be a good idea to print errors to stderr at least?
    dynamic_library_close(xkbcommon_lib);
    xkbcommon_lib = NULL;
  }
}

static XewErrorCode installAtExitHandler(void) {
  int error = atexit(atExitHandler);
  if (error) {
    return XEW_ERROR_ATEXIT_FAILED;
  }
  return XEW_SUCCESS;
}

static XewErrorCode openLibraries() {
  xkbcommon_lib = xew_dynamic_library_open_find(xkbcommon_paths);
  if (xkbcommon_lib == NULL) {
    return XEW_ERROR_OPEN_FAILED;
  }
  return XEW_SUCCESS;
}

static void fetchPointersFromLibrary(void) {
  // xkbcommon.h
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_get_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_from_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_to_utf8);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_to_utf32);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_to_upper);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keysym_to_lower);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_new);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_ref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_unref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_set_user_data);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_get_user_data);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_include_path_append);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_include_path_append_default);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_include_path_reset_defaults);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_include_path_clear);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_num_include_paths);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_include_path_get);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_set_log_level);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_get_log_level);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_set_log_verbosity);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_get_log_verbosity);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_context_set_log_fn);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_new_from_names);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_new_from_file);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_new_from_string);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_new_from_buffer);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_ref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_unref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_get_as_string);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_min_keycode);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_max_keycode);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_key_for_each);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_key_get_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_key_by_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_num_mods);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_mod_get_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_mod_get_index);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_num_layouts);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_layout_get_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_layout_get_index);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_num_leds);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_led_get_name);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_led_get_index);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_num_layouts_for_key);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_num_levels_for_key);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_key_get_syms_by_level);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_keymap_key_repeats);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_new);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_ref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_unref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_get_keymap);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_update_key);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_update_mask);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_syms);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_utf8);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_utf32);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_one_sym);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_layout);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_level);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_serialize_mods);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_serialize_layout);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_name_is_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_names_are_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_index_is_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_indices_are_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_consumed_mods2);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_key_get_consumed_mods);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_index_is_consumed2);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_index_is_consumed);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_mod_mask_remove_consumed);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_layout_name_is_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_layout_index_is_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_led_name_is_active);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_state_led_index_is_active);
  // xkbcommon-compose.h
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_table_new_from_locale);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_table_new_from_file);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_table_new_from_buffer);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_table_ref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_table_unref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_new);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_ref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_unref);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_get_compose_table);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_feed);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_reset);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_get_status);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_get_utf8);
  _LIBRARY_FIND_IMPL(xkbcommon_lib, xkb_compose_state_get_one_sym);
}

XewErrorCode xewXKBCommonInit(void) {
  // Check whether we've already attempted to initialize this module.
  static int is_initialized = 0;
  static XewErrorCode result = 0;
  if (is_initialized) {
    return result;
  }
  is_initialized = 1;
  // Install handler which s run when application quits.
  result = installAtExitHandler();
  if (result != XEW_SUCCESS) {
    return result;
  }
  // Open libraries
  result = openLibraries();
  if (result != XEW_SUCCESS) {
    return result;
  }
  // Fetch all function pointers.
  fetchPointersFromLibrary();
  result = XEW_SUCCESS;
  return result;
}
