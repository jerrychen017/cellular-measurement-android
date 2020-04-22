package com.example.udp_tools;

import android.graphics.Paint;

import java.util.Random;

public class InteractiveUser {
    int id;
    String name;
    float x;
    float y;
    Paint textPaint;
    Paint circlePaint;

    public InteractiveUser(int id, String name, float x, float y, int canvasWidth) {
        this.id = id;
        this.name = name;
        this.x = x;
        this.y = y;
        Paint newPaint = new Paint();
        int r;
        int g;
        int b;
        switch (id) {
            case 0: // red
                r = 255;
                g = 0;
                b = 0;
                break;
            case 1: // blue
                r = 0;
                g = 0;
                b = 255;
                break;
            case 2: // lime
                r = 0;
                g = 255;
                b = 0;
                break;
            case 3: // yellow
                r = 255;
                g = 255;
                b = 0;
                break;
            case 4: // coral
                r = 255;
                g = 127;
                b = 80;
                break;
            case 5: // Magenta
                r = 255;
                g = 0;
                b = 255;
                break;
            case 6: // silver
                r = 192;
                g = 192;
                b = 192;
                break;
            case 7: // grey
                r = 128;
                g = 128;
                b = 128;
                break;
            case 8: // olive
                r = 128;
                g = 128;
                b = 0;
                break;
            default: // purple
                r = 128;
                g = 0;
                b = 128;
                break;
        }
        newPaint.setARGB(255, r, g, b);
        this.circlePaint = newPaint;

        Paint newTextPaint= new Paint();
        newTextPaint.setARGB(255, 0,0,0);
//        newTextPaint.setTextSize(55f);
        newTextPaint.setTextSize(canvasWidth / 25);
        newTextPaint.setAntiAlias(true);
        newTextPaint.setTextAlign(Paint.Align.CENTER);

        this.textPaint = newTextPaint;

    }

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }

    public void setName(String name) {
        this.name = name;
    }
}
