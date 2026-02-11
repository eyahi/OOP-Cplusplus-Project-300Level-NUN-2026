/**
 * @file Styles.h
 * @author Muhammad Tasiu
 * @date 2026-01-21
 * @brief Premium styling constants and utilities for TravelBook UI
 */

#ifndef STYLES_H
#define STYLES_H

#include <nana/gui.hpp>
#include <nana/gui/widgets/label.hpp>
#include <nana/gui/widgets/button.hpp>
#include <nana/gui/widgets/listbox.hpp>

namespace styles {

    // ===== Premium Color Palette =====
    namespace colors {
        // Primary colors - Deep blue theme
        const nana::color primary{41, 98, 255};         // #2962FF - Vibrant blue
        const nana::color primaryDark{0, 57, 166};      // #0039A6 - Dark blue
        const nana::color primaryLight{130, 177, 255};  // #82B1FF - Light blue
        
        // Accent colors - Gold
        const nana::color accent{255, 171, 0};          // #FFAB00 - Gold
        const nana::color accentLight{255, 213, 79};    // #FFD54F - Light gold
        
        // Background colors - Dark theme
        const nana::color background{18, 18, 24};       // #121218 - Deep dark
        const nana::color surface{28, 28, 36};          // #1C1C24 - Surface
        const nana::color surfaceLight{42, 42, 54};     // #2A2A36 - Elevated surface
        const nana::color card{35, 35, 46};             // #23232E - Card background
        const nana::color cardHover{48, 48, 62};        // #30303E - Card hover
        
        // Text colors
        const nana::color textPrimary{255, 255, 255};   // White
        const nana::color textSecondary{180, 180, 195}; // Light gray
        const nana::color textMuted{120, 120, 140};     // Muted gray
        const nana::color textAccent{255, 200, 87};     // Gold text
        
        // Status colors
        const nana::color success{76, 175, 80};         // #4CAF50 - Green
        const nana::color warning{255, 152, 0};         // #FF9800 - Orange
        const nana::color error{244, 67, 54};           // #F44336 - Red
        
        // Alternating row colors for listboxes
        const nana::color rowEven{32, 32, 42};          // Darker row
        const nana::color rowOdd{38, 38, 50};           // Lighter row
        const nana::color rowSelected{41, 98, 255};     // Selected row (primary)
        const nana::color rowHover{50, 50, 65};         // Hover row
        
        // Button colors
        const nana::color buttonPrimary{41, 98, 255};
        const nana::color buttonPrimaryHover{61, 118, 255};
        const nana::color buttonSecondary{55, 55, 70};
        const nana::color buttonSecondaryHover{70, 70, 88};
        const nana::color buttonAccent{255, 171, 0};
        const nana::color buttonAccentHover{255, 190, 50};
        
        // Border colors
        const nana::color border{60, 60, 80};
        const nana::color borderLight{80, 80, 100};
    }
    
    // ===== Typography =====
    namespace fonts {
        // Font sizes
        const int titleSize = 28;
        const int subtitleSize = 13;
        const int headingSize = 16;
        const int bodySize = 11;
        const int buttonSize = 11;
        const int smallSize = 10;
        
        // Font family
        inline const char* family() { return "Segoe UI"; }
        
        // Pre-built fonts
        inline nana::paint::font title() {
            return nana::paint::font(family(), titleSize, {700});
        }
        
        inline nana::paint::font subtitle() {
            return nana::paint::font(family(), subtitleSize, {300});
        }
        
        inline nana::paint::font heading() {
            return nana::paint::font(family(), headingSize, {600});
        }
        
        inline nana::paint::font body() {
            return nana::paint::font(family(), bodySize);
        }
        
        inline nana::paint::font bodyBold() {
            return nana::paint::font(family(), bodySize, {600});
        }
        
        inline nana::paint::font button() {
            return nana::paint::font(family(), buttonSize, {600});
        }
        
        inline nana::paint::font small() {
            return nana::paint::font(family(), smallSize);
        }
    }
    
    // ===== Spacing Constants =====
    namespace spacing {
        const int xs = 4;
        const int sm = 8;
        const int md = 16;
        const int lg = 24;
        const int xl = 32;
        const int xxl = 48;
    }
    
    // ===== Button Styling Functions =====
    
    inline void applyPrimaryButton(nana::button& btn) {
        btn.bgcolor(colors::buttonPrimary);
        btn.fgcolor(colors::textPrimary);
        btn.typeface(fonts::button());
    }
    
    inline void applySecondaryButton(nana::button& btn) {
        btn.bgcolor(colors::buttonSecondary);
        btn.fgcolor(colors::textPrimary);
        btn.typeface(fonts::button());
    }
    
    inline void applyAccentButton(nana::button& btn) {
        btn.bgcolor(colors::buttonAccent);
        btn.fgcolor(colors::background);
        btn.typeface(fonts::button());
    }
    
    inline void applyDangerButton(nana::button& btn) {
        btn.bgcolor(colors::error);
        btn.fgcolor(colors::textPrimary);
        btn.typeface(fonts::button());
    }
    
    // ===== Label Styling Functions =====
    
    inline void applyTitleLabel(nana::label& lbl) {
        lbl.bgcolor(colors::background);
        lbl.fgcolor(colors::textPrimary);
        lbl.typeface(fonts::title());
    }
    
    inline void applySubtitleLabel(nana::label& lbl) {
        lbl.bgcolor(colors::background);
        lbl.fgcolor(colors::textSecondary);
        lbl.typeface(fonts::subtitle());
    }
    
    inline void applyHeadingLabel(nana::label& lbl) {
        lbl.bgcolor(colors::surface);
        lbl.fgcolor(colors::accent);
        lbl.typeface(fonts::heading());
    }
    
    inline void applyBodyLabel(nana::label& lbl) {
        lbl.bgcolor(colors::surface);
        lbl.fgcolor(colors::textPrimary);
        lbl.typeface(fonts::body());
    }
    
    inline void applyMutedLabel(nana::label& lbl) {
        lbl.bgcolor(colors::surface);
        lbl.fgcolor(colors::textSecondary);
        lbl.typeface(fonts::body());
    }
    
    // ===== Listbox Styling =====
    
    inline void applyListboxStyle(nana::listbox& lb) {
        lb.bgcolor(colors::card);
        lb.fgcolor(colors::textPrimary);
        
        // Apply scheme for alternating rows
        auto& scheme = lb.scheme();
        scheme.header_bgcolor = colors::surfaceLight;
        scheme.header_fgcolor = colors::textPrimary;
        scheme.item_selected = colors::rowSelected;
        scheme.item_highlighted = colors::rowHover;
    }
    
    // ===== Panel Styling =====
    
    template<typename Panel>
    inline void applyCardStyle(Panel& panel) {
        panel.bgcolor(colors::card);
    }
    
    template<typename Panel>
    inline void applySurfaceStyle(Panel& panel) {
        panel.bgcolor(colors::surface);
    }
}

#endif // STYLES_H
