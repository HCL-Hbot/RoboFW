// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.0
// LVGL version: 8.3.11
// Project name: Gekozen ogen - functies

#include "../ui.h"

void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Sclera = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Sclera, &ui_img_background_oog_png);
    lv_obj_set_width(ui_Sclera, LV_SIZE_CONTENT);   /// 247
    lv_obj_set_height(ui_Sclera, LV_SIZE_CONTENT);    /// 248
    lv_obj_set_align(ui_Sclera, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Sclera, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Sclera, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Spinner_animatie = lv_spinner_create(ui_Screen1, 1000, 90);
    lv_obj_set_width(ui_Spinner_animatie, 177);
    lv_obj_set_height(ui_Spinner_animatie, 179);
    lv_obj_set_align(ui_Spinner_animatie, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Spinner_animatie, LV_OBJ_FLAG_CLICKABLE);      /// Flags
    lv_obj_set_style_arc_color(ui_Spinner_animatie, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_Spinner_animatie, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Pupil = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Pupil, &ui_img_oog_gekozen_8_gesplitst_png);
    lv_obj_set_width(ui_Pupil, LV_SIZE_CONTENT);   /// 156
    lv_obj_set_height(ui_Pupil, LV_SIZE_CONTENT);    /// 156
    lv_obj_set_align(ui_Pupil, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Pupil, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Pupil, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Vraagteken_animatie = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Vraagteken_animatie, &ui_img_vraagteken_png);
    lv_obj_set_width(ui_Vraagteken_animatie, LV_SIZE_CONTENT);   /// 26
    lv_obj_set_height(ui_Vraagteken_animatie, LV_SIZE_CONTENT);    /// 39
    lv_obj_set_align(ui_Vraagteken_animatie, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Vraagteken_animatie, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Vraagteken_animatie, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_Vraagteken_animatie, 400);

    ui_Ooglid_boven = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Ooglid_boven, &ui_img_bovenkant_ooglid_png);
    lv_obj_set_width(ui_Ooglid_boven, LV_SIZE_CONTENT);   /// 241
    lv_obj_set_height(ui_Ooglid_boven, LV_SIZE_CONTENT);    /// 337
    lv_obj_set_x(ui_Ooglid_boven, -1);
    lv_obj_set_y(ui_Ooglid_boven, -183);
    lv_obj_set_align(ui_Ooglid_boven, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Ooglid_boven, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Ooglid_boven, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Ooglid_onder = lv_img_create(ui_Screen1);
    lv_img_set_src(ui_Ooglid_onder, &ui_img_onderkant_ooglid_png);
    lv_obj_set_width(ui_Ooglid_onder, LV_SIZE_CONTENT);   /// 241
    lv_obj_set_height(ui_Ooglid_onder, LV_SIZE_CONTENT);    /// 337
    lv_obj_set_x(ui_Ooglid_onder, -1);
    lv_obj_set_y(ui_Ooglid_onder, 188);
    lv_obj_set_align(ui_Ooglid_onder, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Ooglid_onder, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_Ooglid_onder, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

}
