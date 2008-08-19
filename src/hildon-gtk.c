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
 * SECTION:hildon-gtk
 * @short_description: Additional functions for Gtk widgets
 * @see_also: #HildonButton
 *
 * Hildon provides some functions to extend the functionality of
 * existing Gtk widgets. This also includes convenience functions to
 * easily perform frequent tasks.
 */

#include "hildon-gtk.h"

#define                                         HILDON_HEIGHT_FINGER 70

#define                                         HILDON_HEIGHT_THUMB 105

#define                                         HILDON_WIDTH_FULLSCREEN \
                                                (gdk_screen_get_width (gdk_screen_get_default ()))

#define                                         HILDON_WIDTH_HALFSCREEN \
                                                (HILDON_WIDTH_FULLSCREEN / 2)

/**
 * hildon_gtk_widget_set_theme_size:
 * @widget: A #GtkWidget
 * @size: Flags indicating the size of the widget
 *
 * This function sets the requested size of a widget.
 **/
void
hildon_gtk_widget_set_theme_size                (GtkWidget      *widget,
                                                 HildonSizeType  size)
{
    gint width = -1;
    gint height = -1;
    const gchar *widget_name = NULL;

    g_return_if_fail (GTK_IS_WIDGET (widget));

    /* Requested height */
    if (size & HILDON_SIZE_FINGER_HEIGHT) {
        height = HILDON_HEIGHT_FINGER;
        widget_name = "hildon-finger-widget";
    } else if (size & HILDON_SIZE_THUMB_HEIGHT) {
        height = HILDON_HEIGHT_THUMB;
        widget_name = "hildon-thumb-widget";
    }

    /* Requested width */
    if (size & HILDON_SIZE_HALFSCREEN_WIDTH) {
        width = HILDON_WIDTH_HALFSCREEN;
    } else if (size & HILDON_SIZE_FULLSCREEN_WIDTH) {
        width = HILDON_WIDTH_FULLSCREEN;
    }

    gtk_widget_set_size_request (widget, width, height);

    if (widget_name)
        gtk_widget_set_name (widget, widget_name);
}

static void
image_visible_changed_cb                        (GtkWidget  *image,
                                                 GParamSpec *arg1,
                                                 gpointer   oldparent)
{
    if (!GTK_WIDGET_VISIBLE (image))
        gtk_widget_show (image);
}

static void
parent_changed_cb                               (GtkWidget  *image,
                                                 GParamSpec *arg1,
                                                 gpointer   oldparent)
{
    /* If the parent has really changed, remove the old signal handlers */
    if (image->parent != oldparent) {
        g_signal_handlers_disconnect_by_func (image, parent_changed_cb, oldparent);
        g_signal_handlers_disconnect_by_func (image, image_visible_changed_cb, NULL);
    }
}

static void
image_changed_cb                                (GtkButton  *button,
                                                 GParamSpec *arg1,
                                                 gpointer    user_data)
{
    GtkWidget *image = gtk_button_get_image (button);

    g_return_if_fail (image == NULL || GTK_IS_WIDGET (image));

    if (image != NULL) {
        /* If the button has a new image, show it */
        gtk_widget_show (image);
        /* Show the image no matter the value of gtk-button-images */
        g_signal_connect (image, "notify::visible", G_CALLBACK (image_visible_changed_cb), NULL);
        /* If the image is removed from the button, disconnect these handlers */
        g_signal_connect (image, "notify::parent", G_CALLBACK (parent_changed_cb), image->parent);
    }
}

static void
button_common_init                              (GtkWidget      *button,
                                                 HildonSizeType  size)
{
    /* Set requested size */
    hildon_gtk_widget_set_theme_size (button, size);

    /* Make sure that all images in this button are always shown */
    g_signal_connect (button, "notify::image", G_CALLBACK (image_changed_cb), NULL);
}

/**
 * hildon_gtk_button_new:
 * @size: Flags indicating the size of the new button
 *
 * This is a convenience function to create a #GtkButton setting its
 * size to one of the pre-defined Hildon sizes.
 *
 * Buttons created with this function also override the
 * "gtk-button-images" setting. Images set using
 * gtk_button_set_image() are always shown.
 *
 * Return value: A newly created #GtkButton widget.
 **/
GtkWidget *
hildon_gtk_button_new                           (HildonSizeType size)
{
    GtkWidget *button = gtk_button_new ();
    button_common_init (button, size);
    return button;
}

/**
 * hildon_gtk_toggle_button_new:
 * @size: Flags indicating the size of the new button
 *
 * This is a convenience function to create a #GtkToggleButton setting
 * its size to one of the pre-defined Hildon sizes.
 *
 * Buttons created with this function also override the
 * "gtk-button-images" setting. Images set using
 * gtk_button_set_image() are always shown.
 *
 * Return value: A newly created #GtkToggleButton widget.
 **/
GtkWidget *
hildon_gtk_toggle_button_new                    (HildonSizeType size)
{
    GtkWidget *button = gtk_toggle_button_new ();
    button_common_init (button, size);
    return button;
}

/**
 * hildon_gtk_radio_button_new:
 * @size: Flags indicating the size of the new button
 * @group: An existing radio button group, or %NULL if you are
 * creating a new group
 *
 * This is a convenience function to create a #GtkRadioButton setting
 * its size to one of the pre-defined Hildon sizes.
 *
 * Buttons created with this function also override the
 * "gtk-button-images" setting. Images set using
 * gtk_button_set_image() are always shown.
 *
 * Return value: A newly created #GtkRadioButton widget.
 **/
GtkWidget *
hildon_gtk_radio_button_new                     (HildonSizeType  size,
                                                 GSList         *group)
{
    GtkWidget *button = gtk_radio_button_new (group);
    button_common_init (button, size);
    return button;
}

/**
 * hildon_gtk_radio_button_new_from_widget:
 * @size: Flags indicating the size of the new button
 * @radio_group_member: widget to get radio group from or %NULL
 *
 * This is a convenience function to create a #GtkRadioButton setting
 * its size to one of the pre-defined Hildon sizes.
 *
 * Buttons created with this function also override the
 * "gtk-button-images" setting. Images set using
 * gtk_button_set_image() are always shown.
 *
 * Return value: A newly created #GtkRadioButton widget.
 **/
GtkWidget *
hildon_gtk_radio_button_new_from_widget         (HildonSizeType  size,
                                                 GtkRadioButton *radio_group_member)
{
    GtkWidget *button = gtk_radio_button_new_from_widget (radio_group_member);
    button_common_init (button, size);
    return button;
}

/**
 * hildon_gtk_tree_view_new:
 * @mode: the Hildon UI mode
 *
 * Creates a new #GtkTreeView widget with the Hildon UI mode set to
 * @mode
 *
 * Return value: A newly created #GtkTreeView widget.
 **/
GtkWidget *
hildon_gtk_tree_view_new                        (HildonUIMode mode)
{
    return g_object_new (GTK_TYPE_TREE_VIEW, "hildon-ui-mode", mode, NULL);
}

/**
 * hildon_gtk_tree_view_new_with_model:
 * @mode: the Hildon UI mode
 * @model: the model.
 *
 * Creates a new #GtkTreeView widget with the Hildon UI mode set to
 * @mode and the model initialized to @model.
 *
 * Return value: A newly created #GtkTreeView widget.
 **/
GtkWidget *
hildon_gtk_tree_view_new_with_model             (HildonUIMode  mode,
                                                 GtkTreeModel *model)
{
    return g_object_new (GTK_TYPE_TREE_VIEW, "hildon-ui-mode", mode, "model", model, NULL);
}

/**
 * hildon_gtk_icon_view_new:
 * @mode: the Hildon UI mode
 *
 * Creates a new #GtkIconView widget with the Hildon UI mode set to
 * @mode
 *
 * Return value: A newly created #GtkIconView widget
 **/
GtkWidget *
hildon_gtk_icon_view_new                        (HildonUIMode mode)
{
    return g_object_new (GTK_TYPE_ICON_VIEW, "hildon-ui-mode", mode, NULL);
}

/**
 * hildon_gtk_icon_view_new_with_model:
 * @mode: the Hildon UI mode
 * @model: The model.
 *
 * Creates a new #GtkIconView widget with the Hildon UI mode set to
 * @mode and the model intitialized to @model.
 *
 * Return value: A newly created #GtkIconView widget.
 **/
GtkWidget *
hildon_gtk_icon_view_new_with_model             (HildonUIMode  mode,
                                                 GtkTreeModel *model)
{
    return g_object_new (GTK_TYPE_ICON_VIEW, "hildon-ui-mode", mode, "model", model, NULL);
}