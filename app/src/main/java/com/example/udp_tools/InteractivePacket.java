package com.example.udp_tools;

public class InteractivePacket {
    public int seq;
    public float x;
    public float y;
    public int id;
    public String name;

    public InteractivePacket(int seq, float x, float y, int id, String name) {
        this.seq = seq;
        this.x = x;
        this.y = y;
        this.id = id;
        this.name = name;
    }
}
