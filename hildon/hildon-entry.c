/*
 * This file is a part of hildon
 *
 * Copyright (C) 2008 Nokia Corporation, all rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; version 2 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 */

/**
 * SECTION:hildon-entry
 * @short_description: Widget representing a text entry in the Hildon framework.
 *
 * The #HildonEntry is a GTK widget which represents a text entry. It
 * is derived from the #GtkEntry widget and provides additional
 * commodities specific to the Hildon framework.
 *
 * Besides all the features inherited from #GtkEntry, a #HildonEntry
 * can also have a placeholder text. This text will be shown if the
 * entry is empty and doesn't have the input focus, but it's otherwise
 * ignored. Thus, calls to hildon_entry_get_text() will never return
 * the placeholder text, not even when it's being displayed.
 *
 * Although #HildonEntry is derived from #GtkEntry,
 * gtk_entry_get_text() and gtk_entry_set_text() must never be used to
 * get/set the text in this widget. hildon_entry_get_text() and
 * hildon_entry_set_text() must be used instead.
 *
 * <example>
 * <title>Creating a HildonEntry with a placeholder</title>
 * <programlisting>
 * GtkWidget *
 * create_entry (void)
 * {
 *     GtkWidget *entry;
 * <!-- -->
 *     entry = hildon_entry_new (HILDON_SIZE_AUTO);
 *     hildon_entry_set_placeholder (HILDON_ENTRY (entry), "First name");
 * <!-- -->
 *     return entry;
 * }
 * </programlisting>
 * </example>
 */

#include                                        "hildon-entry.h"
#include                                        "hildon-helper.h"

G_DEFINE_TYPE                                   (HildonEntry, hildon_entry, GTK_TYPE_ENTRY);

enum {
    PROP_SIZE = 1
};

#define                                         HILDON_ENTRY_GET_PRIVATE(obj) \
                                                (G_TYPE_INSTANCE_GET_PRIVATE ((obj), \
                                                HILDON_TYPE_ENTRY, HildonEntryPrivate));

struct                                          _HildonEntryPrivate
{
    gchar *placeholder;
    gboolean showing_placeholder;
};

static void
set_property                                    (GObject      *object,
                                                 guint         prop_id,
                                                 const GValue *value,
                                                 GParamSpec   *pspec)
{
    switch (prop_id)
    {
    case PROP_SIZE:
        hildon_gtk_widget_set_theme_size (GTK_WIDGET (object), g_value_get_flags (value));
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
hildon_entry_show_placeholder (HildonEntry *entry)
{
    HildonEntryPrivate *priv = HILDON_ENTRY (entry)->priv;

    priv->showing_placeholder = TRUE;
    gtk_entry_set_text (GTK_ENTRY (entry), priv->placeholder);
    hildon_helper_set_logical_color (GTK_WIDGET (entry),
				     GTK_RC_TEXT, GTK_STATE_NORMAL, "ReversedSecondaryTextColor");
}

static void
hildon_entry_hide_placeholder (HildonEntry *entry, const gchar *text)
{
    HildonEntryPrivate *priv = HILDON_ENTRY (entry)->priv;

    priv->showing_placeholder = FALSE;
    gtk_entry_set_text (GTK_ENTRY (entry), text);
    hildon_helper_set_logical_color (GTK_WIDGET (entry),
				     GTK_RC_TEXT, GTK_STATE_NORMAL, "ReversedTextColor");
}

/**
 * hildon_entry_set_text:
 * @entry: a #HildonEntry
 * @text: the new text
 *
 * Sets the text in @entry to @text, replacing its current contents.
 *
 * Note that you must never use gtk_entry_set_text() to set the text
 * of a #HildonEntry.
 *
 * Since: 2.2
 */
void
hildon_entry_set_text                           (HildonEntry *entry,
                                                 const gchar *text)
{
    g_return_if_fail (HILDON_IS_ENTRY (entry) && text != NULL);

    if (text[0] == '\0' && !GTK_WIDGET_HAS_FOCUS (entry)) {
	    hildon_entry_show_placeholder (entry);
    } else {
	    hildon_entry_hide_placeholder (entry, text);
    }
}

/**
 * hildon_entry_get_text:
 * @entry: a #HildonEntry
 *
 * Gets the current text in @entry.
 *
 * Note that you must never use gtk_entry_get_text() to get the text
 * from a #HildonEntry.
 *
 * Also note that placeholder text (set using
 * hildon_entry_set_placeholder()) is never returned. Only text set by
 * hildon_entry_set_text() or typed by the user is considered.
 *
 * Returns: the text in @entry. This text must not be modified or
 * freed.
 *
 * Since: 2.2
 */
const gchar *
hildon_entry_get_text                           (HildonEntry *entry)
{
    g_return_val_if_fail (HILDON_IS_ENTRY (entry), NULL);

    if (entry->priv->showing_placeholder) {
        return "";
    }

    return gtk_entry_get_text (GTK_ENTRY (entry));
}

/**
 * hildon_entry_set_placeholder:
 * @entry: a #HildonEntry
 * @text: the new text
 *
 * Sets the placeholder text in @entry to @text.
 *
 * Since: 2.2
 */
void
hildon_entry_set_placeholder                    (HildonEntry *entry,
                                                 const gchar *text)
{
    g_return_if_fail (HILDON_IS_ENTRY (entry) && text != NULL);

    g_free (entry->priv->placeholder);
    entry->priv->placeholder = g_strdup (text);

    /* Show the placeholder if it needs to be updated or if should be shown now. */
    if (entry->priv->showing_placeholder ||
        (!GTK_WIDGET_HAS_FOCUS (entry) && gtk_entry_get_text (GTK_ENTRY (entry)) [0] == '\0')) {
        hildon_entry_show_placeholder (entry);
    }
}

/**
 * hildon_entry_new:
 * @size: The size of the entry
 *
 * Creates a new entry.
 *
 * Returns: a new #HildonEntry
 *
 * Since: 2.2
 */
GtkWidget *
hildon_entry_new                                (HildonSizeType size)
{
    return g_object_new (HILDON_TYPE_ENTRY, "size", size, NULL);
}

static gboolean
hildon_entry_focus_in_event                     (GtkWidget     *widget,
                                                 GdkEventFocus *event)
{
    if (HILDON_ENTRY (widget)->priv->showing_placeholder) {
	    hildon_entry_hide_placeholder (HILDON_ENTRY (widget), "");
    }

    if (GTK_WIDGET_CLASS (hildon_entry_parent_class)->focus_in_event) {
        return GTK_WIDGET_CLASS (hildon_entry_parent_class)->focus_in_event (widget, event);
    } else {
        return FALSE;
    }
}

static gboolean
hildon_entry_focus_out_event                    (GtkWidget     *widget,
                                                 GdkEventFocus *event)
{
    if (gtk_entry_get_text (GTK_ENTRY (widget)) [0] == '\0') {
        hildon_entry_show_placeholder (HILDON_ENTRY (widget));
    }

    if (GTK_WIDGET_CLASS (hildon_entry_parent_class)->focus_out_event) {
        return GTK_WIDGET_CLASS (hildon_entry_parent_class)->focus_out_event (widget, event);
    } else {
        return FALSE;
    }
}

static void
hildon_entry_finalize                           (GObject *object)
{
    HildonEntryPrivate *priv = HILDON_ENTRY (object)->priv;

    g_free (priv->placeholder);

    if (G_OBJECT_CLASS (hildon_entry_parent_class)->finalize)
        G_OBJECT_CLASS (hildon_entry_parent_class)->finalize (object);
}

static void
hildon_entry_class_init                         (HildonEntryClass *klass)
{
    GObjectClass *gobject_class = (GObjectClass *)klass;
    GtkWidgetClass *widget_class = (GtkWidgetClass *)klass;

    gobject_class->set_property = set_property;
    gobject_class->finalize = hildon_entry_finalize;
    widget_class->focus_in_event = hildon_entry_focus_in_event;
    widget_class->focus_out_event = hildon_entry_focus_out_event;

    g_object_class_install_property (
        gobject_class,
        PROP_SIZE,
        g_param_spec_flags (
            "size",
            "Size",
            "Size request for the entry",
            HILDON_TYPE_SIZE_TYPE,
            HILDON_SIZE_AUTO_WIDTH | HILDON_SIZE_FINGER_HEIGHT,
            G_PARAM_CONSTRUCT | G_PARAM_WRITABLE));

    g_type_class_add_private (klass, sizeof (HildonEntryPrivate));
}

static void
hildon_entry_init                               (HildonEntry *self)
{
    self->priv = HILDON_ENTRY_GET_PRIVATE (self);
    self->priv->placeholder = g_strdup ("");
    self->priv->showing_placeholder = FALSE;
}
