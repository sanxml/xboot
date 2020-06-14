#ifndef __XUI_SLIDER_H__
#define __XUI_SLIDER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <xui/xui.h>

enum {
	XUI_SLIDER_PRIMARY		= (0x0 << 8),
	XUI_SLIDER_SECONDARY	= (0x1 << 8),
	XUI_SLIDER_SUCCESS		= (0x2 << 8),
	XUI_SLIDER_INFO			= (0x3 << 8),
	XUI_SLIDER_WARNING		= (0x4 << 8),
	XUI_SLIDER_DANGER		= (0x5 << 8),
	XUI_SLIDER_LIGHT		= (0x6 << 8),
	XUI_SLIDER_DARK			= (0x7 << 8),

	XUI_SLIDER_HORIZONTAL	= (0x0 << 11),
	XUI_SLIDER_VERTICAL		= (0x1 << 11),
};

int xui_slider_ex(struct xui_context_t * ctx, float * value, float low, float high, float step, int opt);

static inline int xui_slider_int_ex(struct xui_context_t * ctx, int * value, int low, int high, int step, int opt)
{
	float v;
	int change;

	xui_push_id(ctx, &value, sizeof(int *));
	v = *value;
	change = xui_slider_ex(ctx, &v, low, high, step, opt);
	*value = v;
	xui_pop_id(ctx);
	return change;
}

static inline int xui_slider_char_ex(struct xui_context_t * ctx, char * value, char low, char high, char step, int opt)
{
	float v;
	int change;

	xui_push_id(ctx, &value, sizeof(char *));
	v = *value;
	change = xui_slider_ex(ctx, &v, low, high, step, opt);
	*value = v;
	xui_pop_id(ctx);
	return change;
}

static inline int xui_slider(struct xui_context_t * ctx, float * value, float low, float high, float step)
{
	return xui_slider_ex(ctx, value, low, high, step, XUI_SLIDER_PRIMARY | XUI_SLIDER_HORIZONTAL);
}

static inline int xui_slider_int(struct xui_context_t * ctx, int * value, int low, int high, int step)
{
	return xui_slider_int_ex(ctx, value, low, high, step, XUI_SLIDER_PRIMARY | XUI_SLIDER_HORIZONTAL);
}

static inline int xui_slider_char(struct xui_context_t * ctx, char * value, char low, char high, char step)
{
	return xui_slider_char_ex(ctx, value, low, high, step, XUI_SLIDER_PRIMARY | XUI_SLIDER_HORIZONTAL);
}

#ifdef __cplusplus
}
#endif

#endif /* __XUI_SLIDER_H__ */