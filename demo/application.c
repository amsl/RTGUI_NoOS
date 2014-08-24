#include <rtgui/rtgui.h>
#include <rtgui/rtgui_system.h>
#include <rtgui/widgets/button.h>
#include <rtgui/widgets/panel.h>
#include <rtgui/widgets/label.h>
#include <rtgui/widgets/listbox.h>

struct rtgui_win *main_win;
static void demo_test_entry(void *parameter)
{
	rtgui_panel_t *panel;
	rtgui_label_t *label;
	rtgui_listbox_t *box;
    struct rtgui_rect rect;

    /* create a full screen window */
    rtgui_gdev_get_rect(rtgui_gdev_get(), &rect);
	panel = rtgui_panel_create(0,0,RC_W(rect),RC_H(rect));

	///{{{
	label = rtgui_label_create(panel, "Hello RTGUI NoOS!", 10, 10, 150, 20);
	RTGUI_WIDGET_BC(label) = Red;
	label = rtgui_label_create(panel, "Demo Code Test!", 10, 40, 150, 20);
	RTGUI_WIDGET_BC(label) = Blue;
	rtgui_button_create(panel, "TEST BUTTON", 10, 80, 100, 25);

	///}}}
    rtgui_panel_show(panel);
}

void rt_application_init(void)
{
	demo_test_entry(RT_NULL);
}
