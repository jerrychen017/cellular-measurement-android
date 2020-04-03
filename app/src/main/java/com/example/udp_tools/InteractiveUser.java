package com.example.udp_tools;

import android.graphics.Paint;

import java.util.Random;

public class InteractiveUser {
    int id;
    String name;
    float x;
    float y;
    Paint paint;

    public InteractiveUser(int id, String name, float x, float y) {
        this.id = id;
        this.name = name;
        this.x = x;
        this.y = y;
        Paint newPaint = new Paint();
        Random rnd = new Random();
        newPaint.setARGB(255, rnd.nextInt(256), rnd.nextInt(256), rnd.nextInt(256));
        this.paint = newPaint;
    }

    public void setX(float x) {
        this.x = x;
    }

    public void setY(float y) {
        this.y = y;
    }
}
