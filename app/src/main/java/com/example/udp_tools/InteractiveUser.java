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

    public InteractiveUser(int id, String name, float x, float y) {
        this.id = id;
        this.name = name;
        this.x = x;
        this.y = y;
        Paint newPaint = new Paint();
        Random rnd = new Random();
        int r = rnd.nextInt(256);
        int g = rnd.nextInt(256);
        int b = rnd.nextInt(256);
        newPaint.setARGB(255, r, g, b);
        this.circlePaint = newPaint;

        Paint newTextPaint= new Paint();
//        r = (~r) & 0xff;
//        g = (~g) & 0xff;
//        b = (~b) & 0xff;
        newTextPaint.setARGB(255, 0,0,0);
        newTextPaint.setTextSize(55f);
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
}
