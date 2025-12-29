#ifndef ImGuiAndroid_Draww
#define ImGuiAndroid_Draww

#include "ImGui/imgui_internal.h"
#include "Struc.h"
#include "Vector2.h"

namespace DrawAddLine
{
    void DrawLine(ImVec2 start, ImVec2 end, ImVec4 color, float thickness = 3.0f) {
        auto background = ImGui::GetBackgroundDrawList();
        if(background) {
            background->AddLine(start, end, ImColor(color.x, color.y, color.z, color.w), thickness);
        }
    }
    
    
    
    void DrawCrosshair(ImVec4 color, Vector2 center, float size = 20) {
        float x = center.X - (size / 2.0f);
        float y = center.Y - (size / 2.0f);
        DrawLine(ImVec2(x, center.Y), ImVec2(x + size, center.Y), ImVec4(120, 120, 120, 120));
        DrawLine(ImVec2(center.X, y), ImVec2(center.X, y + size), ImVec4(120, 120, 120, 120));
    }
    
    void DrawBox(Rect rect, ImVec4 color) {
        ImVec2 v1(rect.x, rect.y);
        ImVec2 v2(rect.x + rect.width, rect.y);
        ImVec2 v3(rect.x + rect.width, rect.y + rect.height);
        ImVec2 v4(rect.x, rect.y + rect.height);

        DrawLine(v1, v2, color);
        DrawLine(v2, v3, color);
        DrawLine(v3, v4, color);
        DrawLine(v4, v1, color);
    }

    void DrawCircle(float X, float Y, float radius, bool filled, ImVec4 color) {
        auto background = ImGui::GetBackgroundDrawList();
        if(background) {
            if(filled) {
                background->AddCircleFilled(ImVec2(X, Y), radius, ImColor(color.x, color.y, color.z, color.w));
            }
            else {
                background->AddCircle(ImVec2(X, Y), radius, ImColor(color.x, color.y, color.z, color.w));
            }
        }
    }
    
    void DrawText2(float fontSize, ImVec2 position, ImVec4 color, const char *text)
    {
        auto background = ImGui::GetBackgroundDrawList();

        if(background)
        {
            background->AddText(NULL, fontSize, position, ImColor(color.x, color.y, color.z, color.w), text);
        }
    }
    
    
    
    void DrawHorizontalHealthBar(Vector2 screenPos, float width, float maxHealth, float currentHealth) {
        screenPos -= Vector2(0.0f, 8.0f);
        DrawBox(Rect(screenPos.X, screenPos.Y, width + 2, 5.0f), ImVec4(0, 0, 0, 255));
        screenPos += Vector2(1.0f, 1.0f);
        ImVec4 clr = ImVec4(0, 255, 0, 255);
        float hpWidth = (currentHealth * width) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = ImVec4(255, 255, 0, 255);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = ImVec4(255, 0, 0, 255);
        }
        if (currentHealth <= (maxHealth * 0.0)) {
            clr = ImVec4(0, 0, 0, 0);
        }
        DrawBox(Rect(screenPos.X, screenPos.Y, hpWidth, 3.0f), clr);
    }
    
    float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y, float Object_z)
    {
        float x, y, z;
        x = Self_x - Object_x;
        y = Self_y - Object_y;
        z = Self_z - Object_z;
        return (float)(sqrt(x * x + y * y + z * z));
    }
    
/*
    void DrawVerticalHealth(Vector2 end, float h, float health) {

        float x = end.X;
        float y = end.Y;
        h = -h;

        Color clr = Color(0, 255, 0, 255);

        float hpwidth = h-h*health/100;

        if (health <= (100 * 0.6)) {
            clr = Color(255, 255, 0, 255);
        }
        if (health < (100 * 0.3)) {
            clr = Color(255, 0, 0, 255);
        }

        Rect hpbar((x+h/4)-8,y,4.0f,-h);
        Rect hp((x+h/4)-8,y-hpwidth,2.0f,-h+hpwidth);

        DrawBox(Color(0,0,0,255), 3, hpbar);

        DrawBox(clr,3, hp);

    }*/
    
	
    
}


#endif
