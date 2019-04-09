#pragma once
#include "Grid.hpp"
#include "colorDefine.hpp"

struct RccSet
{
    int   m_index;
    Gtype m_type;
    bool  m_testMode;
    bool  m_rgbMode;
    float m_pRGB[3];
    int   m_blurSize;
    int   m_radius;
    int   m_horizontal;
    int   m_vertical;
    int   m_rotateAngle;
    int   m_perspective;
    int   m_aspectRatio;
    int   m_brightness;
    int   m_contrast;
    int   m_cols;
    int   m_rows;
    std::vector<HSV> m_colors;

    RccSet(const int& index, const Gtype& type, const std::vector<HSV>& colors, const int cols, const int rows)
        : m_index      (index),
          m_type       (type),
          m_testMode   (false),
          m_rgbMode    (false),
          m_pRGB       {1, 1, 1},
          m_blurSize   (0),
          m_radius     (0),
          m_horizontal (0),
          m_vertical   (0),
          m_rotateAngle(0),
          m_perspective(100),
          m_aspectRatio(100),
          m_brightness (0),
          m_contrast   (50),
          m_colors     (colors),
          m_cols       (cols),
          m_rows       (rows)
    {
    }

    RccSet(const RccSet& rs)
        : m_index      (rs.m_index),
          m_type       (rs.m_type),
          m_testMode   (rs.m_testMode),
          m_rgbMode    (rs.m_rgbMode),
          m_blurSize   (rs.m_blurSize),
          m_radius     (rs.m_radius),
          m_horizontal (rs.m_horizontal),
          m_vertical   (rs.m_vertical),
          m_rotateAngle(rs.m_rotateAngle),
          m_perspective(rs.m_perspective),
          m_aspectRatio(rs.m_aspectRatio),
          m_brightness (rs.m_brightness),
          m_contrast   (rs.m_contrast),
          m_colors     (rs.m_colors),
          m_cols       (rs.m_cols),
          m_rows       (rs.m_rows)
    {
        for (int i = 0; i < 3; ++i) {
            m_pRGB[i] = rs.m_pRGB[i];
        }
    }

    RccSet& operator= (const RccSet& rs)
    {
        m_index       = rs.m_index;
        m_type        = rs.m_type;
        m_testMode    = rs.m_testMode;
        m_rgbMode     = rs.m_rgbMode;
        m_blurSize    = rs.m_blurSize;
        m_radius      = rs.m_radius;
        m_horizontal  = rs.m_horizontal;
        m_vertical    = rs.m_vertical;
        m_rotateAngle = rs.m_rotateAngle;
        m_perspective = rs.m_perspective;
        m_aspectRatio = rs.m_aspectRatio;
        m_brightness  = rs.m_brightness; // 这里没有pre
        m_contrast    = rs.m_contrast;   // 这里没有pre
        m_colors      = rs.m_colors;
        m_cols        = rs.m_cols;
        m_rows        = rs.m_rows;
        for (int i = 0; i < 3; ++i) {
            m_pRGB[i] = rs.m_pRGB[i];
        }
        return *this;
    }

    void DefaultColors()
    {
        m_colors.clear();
        m_colors.push_back(g_hsvAll);
        m_colors.push_back(g_hsvWhite);
        m_colors.push_back(g_hsvRed);
        m_colors.push_back(g_hsvOrange);
        m_colors.push_back(g_hsvYellow);
        m_colors.push_back(g_hsvGreen);
        m_colors.push_back(g_hsvBlue);
    }
};
