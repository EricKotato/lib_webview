// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#include "webview/platform/linux/webview_linux_webkitgtk_library.h"

#include "base/platform/linux/base_linux_library.h"

namespace Webview::WebKitGTK::Library {

ResolveResult Resolve(bool wayland) {
	const auto lib = base::Platform::LoadLibrary("libwebkitgtk-6.0.so.4", RTLD_NODELETE)
			?: base::Platform::LoadLibrary("libwebkit2gtk-4.1.so.0", RTLD_NODELETE)
			?: base::Platform::LoadLibrary("libwebkit2gtk-4.0.so.37", RTLD_NODELETE);
	const auto result = lib
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_init_check)
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_widget_get_type)
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_widget_grab_focus)
		&& (LOAD_LIBRARY_SYMBOL(lib, gtk_window_set_child)
			|| (LOAD_LIBRARY_SYMBOL(lib, gtk_container_get_type)
				&& LOAD_LIBRARY_SYMBOL(lib, gtk_container_add)))
		&& (wayland
			|| (LOAD_LIBRARY_SYMBOL(lib, gtk_widget_get_native)
				&& LOAD_LIBRARY_SYMBOL(lib, gtk_native_get_surface))
			|| LOAD_LIBRARY_SYMBOL(lib, gtk_widget_get_window))
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_window_new)
		&& (LOAD_LIBRARY_SYMBOL(lib, gtk_window_destroy)
			|| LOAD_LIBRARY_SYMBOL(lib, gtk_widget_destroy))
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_widget_set_visible)
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_window_get_type)
		&& LOAD_LIBRARY_SYMBOL(lib, gtk_window_set_decorated)
		&& (wayland
			|| LOAD_LIBRARY_SYMBOL(lib, gdk_x11_surface_get_xid)
			|| LOAD_LIBRARY_SYMBOL(lib, gdk_x11_window_get_xid))
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_new)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_get_type)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_get_user_content_manager)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_user_content_manager_register_script_message_handler)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_get_settings)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_settings_set_javascript_can_access_clipboard)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_settings_set_enable_developer_extras)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_load_uri)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_reload_bypass_cache)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_user_script_new)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_user_content_manager_add_script)
		&& (LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_evaluate_javascript)
			|| LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_run_javascript))
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_uri_request_get_uri)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_policy_decision_ignore)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_navigation_policy_decision_get_type)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_script_dialog_get_dialog_type)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_script_dialog_get_message)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_script_dialog_confirm_set_confirmed)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_script_dialog_prompt_get_default_text)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_script_dialog_prompt_set_text)
		&& LOAD_LIBRARY_SYMBOL(lib, webkit_web_view_set_background_color);
	if (!result) {
		return ResolveResult::NoLibrary;
	}
	LOAD_LIBRARY_SYMBOL(lib, gtk_widget_show_all);
	LOAD_LIBRARY_SYMBOL(lib, webkit_javascript_result_get_js_value);
	{
		const auto available1 = LOAD_LIBRARY_SYMBOL(lib, jsc_value_to_string);

		const auto available2 = LOAD_LIBRARY_SYMBOL(lib, webkit_javascript_result_get_global_context)
			&& LOAD_LIBRARY_SYMBOL(lib, webkit_javascript_result_get_value)
			&& LOAD_LIBRARY_SYMBOL(lib, JSValueToStringCopy)
			&& LOAD_LIBRARY_SYMBOL(lib, JSStringGetMaximumUTF8CStringSize)
			&& LOAD_LIBRARY_SYMBOL(lib, JSStringGetUTF8CString)
			&& LOAD_LIBRARY_SYMBOL(lib, JSStringRelease);
		if (!available1 && !available2) {
			return ResolveResult::NoLibrary;
		}
	}
	{
		const auto available1 = LOAD_LIBRARY_SYMBOL(lib, webkit_navigation_policy_decision_get_navigation_action)
			&& LOAD_LIBRARY_SYMBOL(lib, webkit_navigation_action_get_request);

		const auto available2 = LOAD_LIBRARY_SYMBOL(lib, webkit_navigation_policy_decision_get_request);

		if (!available1 && !available2) {
			return ResolveResult::NoLibrary;
		}
	}
	if (LOAD_LIBRARY_SYMBOL(lib, gdk_set_allowed_backends)) {
		gdk_set_allowed_backends(wayland ? "wayland" : "x11");
	}
	return gtk_init_check(0, 0)
		? ResolveResult::Success
		: ResolveResult::CantInit;
}

} // namespace Webview::WebKitGTK::Library
